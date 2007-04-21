/*  -*- LPC -*-  */
/*  cloner_control.c

  This room is used to add or list filename mappings for the cloner.

    Change log:

  15/3/96 - Created by Ceres

*/
#include "path.h"

inherit "/std/room/basic_room";

#define CLONER "/global/cloner"


int do_add(string from, string to);
int do_remove(string from);
int do_list(string matching);

void setup() {
  set_light(100);
  set_short("Cloner Room");
  set_long("This room is used to look at or add filename mappings for the "+
     "cloner object.  It's a very, very, very, very boring room.\n\n"+
     "  add <from> <to> - adds a mapping.\n"+
     "  remove <from> - removes a mapping.\n"+
     "  list [str] - lists mappings which contain the optional string "+
     "str.\n");
  add_exit("southwest" , ROOM+"development" , "door");
}

void init() {
  ::init();

  if (!this_player()) return;

  this_player()->add_command("add", this_object(),
    "<string:small'from object'> <string:small'to object'>",
    (: do_add($4[0], $4[1]) :)
    );
  this_player()->add_command("remove", this_object(),
    "<string:small'from object'>",
    (: do_remove($4[0]) :)
    );
  this_player()->add_command("list", this_object(),
    "<string:small'matching'>",
    (: do_list($4[0]) :)
    );
  this_player()->add_command("list", this_object(),
    "",
    (: do_list("") :)
    );
}

int do_add(string from, string to) {
  CLONER->add_mapping(from, to);
  return 1;
}

int do_remove(string str) {
  CLONER->remove_mapping(str);
  return 1;
}

int do_list(string from) {
  this_player()->more_string(CLONER->list_mappings(from));
  return 1;
}
