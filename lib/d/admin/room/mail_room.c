inherit "/std/room/basic_room";

#include "path.h"
#include <mail.h>

#define NAME (string)this_player()->query_name()
#define HIGHLORD() (master()->high_programmer(NAME))

int do_add(string list, string name);
varargs int do_list(string str);
int do_create(string str);
int do_remove(string list, string names);

void setup() {
  set_short("Mail control room");
  set_long(
"This is the mail control room.  You can see several frogs hopping around "
"looking important, sorting mail into nice little boxes, putting them in "
"their backpacks and hopping off into the distance.  Over to the west of "
"the room is a small control panel, with odd levers and lots of strange "
"buttons.\n");
  set_light(100);

  add_exit("south", ROOM + "development", "door"); 

  add_item("important looking post office frog",
           "They are rushing around in a seemless unending hurry.  They are "
           "all wearing nice little hats that have the post office symbol on "
           "them.");
  add_item("box",
           "There are several nice boxes around the room.  They all appear "
           "to be labeled in some arcane script.  If you squint at them they "
           "form interesting patterns.");
  add_item("backpack",
           "Every frog has its own backback.  Some are full, some are empty, "
           "but they all have them.  The regulation backback is about big "
           "enough to contain over a hundred letters.  Some of them look "
           "battered and worn as if they have seen many years of service.");
  add_item("control panel",
           "It is covered with odd levers and buttons.  Some of them are "
           "labeled in a readable script.  They are \"add\", \"list\", "
           "\"listcreate\" and \"remove\".");
} /* setup() */

void init() {
  ::init();

  if (!this_player()) return;

  this_player()->add_command(
    "add", this_object(),
    "<string:small'list'> <string:long'names'>",
    (: do_add($4[0], $4[1]) :) );
  this_player()->add_command(
    "list", this_object(),
    "<string:small'list'>",
    (: do_list($4[0]) :) );
  this_player()->add_command(
    "list", this_object(),
    "",
    (: do_list() :) );
  this_player()->add_command(
    "remove", this_object(),
    "<string:small'list'> <string:long'names'>",
    (: do_remove($4[0], $4[1]) :) );
  this_player()->add_command(
    "listcreate", this_object(),
    "<string:small'list'>",
    (: do_create($4[0]) :) );
} /* init() */

int do_add(string list, string name) {
  string *names;

  if (!MAIL_TRACK->query_list(list)) {
    notify_fail("The mailing list \"" + list + "\" does not exist.\n");
    return 0;
  }
  if (!(MAIL_TRACK->query_controller(list, NAME) || HIGHLORD())) {
    notify_fail("You have to be a controller of a list to be able to add a "
                "member.\n");
    return 0;
  }
  names = explode(replace(lower_case(name), " ", ","), ",") - ({ "" });
  if (!sizeof(names)) {
    notify_fail("It helps if you tell me who to add.\n");
    return 0;
  }
  foreach (name in names) {
    if (name[0] == '*') {
      /* They are trying to add a controller. */
      name = name[1..];
      if (!"/secure/login"->test_user(name)) {
        write("The player \"" + name + "\" does not exist.\n");
      } else if (MAIL_TRACK->add_controller(list, name)) {
        write("Added " + name + " as a controller of \"" + list + "\".\n");
      } else {
        write("Failed to add " + name + " as a controller of \"" +
              list + "\".\n");
      }
    } else if (!"/secure/login"->test_user(name) &&
               !MAIL_TRACK->query_list(name)) {
      write("The player \"" + name + "\" does not exist.\n");
    } else if (MAIL_TRACK->add_member(list, name)) {
      write("Added " + name + " to the mailing list.\n");
    } else {
      write("Failed to add " + name + " to the mailing list.\n");
    }
  }
  return 1;
} /* do_add() */

varargs int do_list(string str) {
  string *strs, *cont, creator;

  if (!str) {
    strs = MAIL_TRACK->query_mailing_lists();
    write("$I$5=Current mailing lists: " + query_multiple_short(strs) + ".\n");
  } else {
    strs = MAIL_TRACK->query_members(str);
    cont = MAIL_TRACK->query_controllers(str);
    if (!sizeof(strs)) {
      notify_fail("The mailing list \""+str+"\" does not exist.\n");
      return 0;
    }
    if ((creator = MAIL_TRACK->query_creator(str))) {
      write("The list \"" + str + "\" was created by " + creator + ".\n");
    }
    if (sizeof(cont)) {
      write("$I$5=Controllers of list \"" + str + "\": " +
            query_multiple_short(cont) + ".\n");
    }
    write("$I$5=Members of list \"" + str + "\": " +
          query_multiple_short(strs) + ".\n");
  }
  return 1;
} /* do_list() */

int do_remove(string list, string name) {
  string *names;

  if (!MAIL_TRACK->query_list(list)) {
    notify_fail("The mailing list \""+list+"\" does not exist.\n");
    return 0;
  }
  if (!(MAIL_TRACK->query_controller(list, NAME) || HIGHLORD())) {
    notify_fail("You have to be a controller of a list to be able to "+
                "remove a member.\n");
    return 0;
  }
  names = explode(replace(lower_case(name), " ", ","), ",") - ({ "" });
  if (!sizeof(names)) {
    notify_fail("It helps if you tell me who to remove.\n");
    return 0;
  }
  foreach (name in names) {
    if (name[0] == '*') {
      /* They are trying to remove a controller. */
      name = name[1..];
      if (MAIL_TRACK->remove_controller(list, name)) {
        write("Removed " + name + " as a controller of \"" + list + "\".\n");
      } else {
        write("Failed to remove " + name + " as a controller of \"" +
              list + "\".\n");
      }
    } else if (MAIL_TRACK->query_controller(list, name) && (name != NAME)) {
      write("Could not remove " + name + ", because " + name + " is a "
            "controller of \"" + list + "\".\n");
    } else if (MAIL_TRACK->remove_member(list, name)) {
      write("Removed " + name + " from the mailing list.\n");
    } else {
      write("Could not remove " + name + " from the mailing list.\n");
    }
  }
  return 1;
} /* do_remove() */

int do_create(string str) {
  if (MAIL_TRACK->query_list(str)) {
    notify_fail("The list \"" + str + "\" already exists.\n");
    return 0;
  }
  if (sscanf(str, "%*s %*s") == 2 || sscanf(str, "%*s,%*s")) {
    notify_fail("The mailing list name cannot have spaces or commas in it.\n");
    return 0;
  }
  if (MAIL_TRACK->create_list(str, this_player()->query_name())) {
    write("Created the mailing list \""+str+"\".\n");
    return 1;
  }
  notify_fail("Failed to create the mailing list.\n");
  return 0;
} /* do_create() */
