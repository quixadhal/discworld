#include "log.h"

void smart_log(string type, string text) {
  string where, temp1, temp2, junk, *j, person;

  where = file_name(environment(this_player()));
  text = where + "\n" + ctime(time()) + " " + text;
  log_file(type, text);
  if (where[0] == '/')
    where = where[1..10000];
  switch (where[0..1]) {
    case "w/" :
      sscanf(where, "w/%s/%s", temp1, temp2);
      if (file_size("/w/"+temp1) == -2) {
        person = temp1;
      } else
        person = "pinkfish";
      break;
    case "d/" :
      sscanf(where, "d/%s/%s", temp1, temp2);
      person = (string)("/d/"+temp1+"/master")->log_who(where);
      break;
  }
  if (where[0..4] == "room/")
    person = "bastian";
  j = explode(where, "/");
  write_file((temp1 = (implode(j[0..sizeof(j)-2], "/") + "/" + ERROR_LOG)),
              type+" : "+text);
  temp1 = read_file("/w/"+person+"/"+PLAYER_ERROR_LOG);
  if (!temp1)
    temp1 = "";
  j = explode(temp1, "\n");
  if (!j)
    j = ({ });
  if (member_array(where, j) == -1)
    j += ({ where });
  temp1 = implode(j, "\n")+ "\n";
  "/secure/master"->do_log(person, temp1);
}
