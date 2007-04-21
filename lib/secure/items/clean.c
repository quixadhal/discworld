#include "standard.h"
inherit "/std/object";

setup()
{
  set_name("test");
  set_short("test");
}

init()
{
  ::init();
  add_action("purge","purge");
}

// mixed *stat(string str, int|DEFAULT 0);
// ({ file_size, last_time_file_touched, time_object_loaded }) 

#define DAYS60 5184000
#define DAYS90 7776000
 

int purge(string str)
{
  mixed *s, *dir, *let;
  int i,j,t;
  string tmp;

  let = ({"a","b","c","d","e","f","g","h","i","j","k","l",
                  "m","n","o","p","q","r","s","t","u","v","w","x",
                  "y","z", });

  seteuid("Root");
  t=time();

  for (j=0;j<26;j++)
  {
    dir=get_dir("/players/"+let[j]+"/*.o");
    for (i=0;i<sizeof(dir);i++)
    {
      s=stat("/players/"+let[j]+"/"+dir[i]);
      if ( sizeof(s) && ((t-(int)s[1]) > DAYS60))
      {
        sscanf(dir[i],"%s.o",tmp);
        if (!"/secure/master"->query_promoted(tmp))
        {
          log_file("EXPIRED",dir[i]+" unused for > 60 days, moved\n");
          rename("/players/"+let[j]+"/"+dir[i],"/players/OLD/"+dir[i]);
        }
        else
          log_file("EXPIRED",dir[i]+" unused for > 60 days, promoted, not moved\n");
      }
    }
  }

  "/obj/handlers/library"->clean_library();
  "/room/bankmain"->clean_bank();
  "/obj/handlers/mail_track"->clean_post();

  return 1;
}