inherit "/std/room";
#include "path.h"

void setup() {
  set_co_ord(({ 1000, 1000, 1000 }));
/* Make the co-ord wacko, so nothing wanders in here by acident. */
  set_short("Mailing room");
  set_light(50);
  set_zone("post office");
  set_long(
"A dark dim squashy room with large hulking writing desks that manage "+
"to take up most of the available space.  All you can hear in the room "+
"is the not at all silent scratchings of quills as mail is written and "+
"read.  A thought pops into your head suddenly, It says 'Type \"mail\" to "+
"read your mail, or \"mail <person>\" to mail to someone.'\n");

  add_item("large hulking writing desk",
           "Large black desks, with huge solid bits all over them, you "+
           "are sure you would never want to pick one up, ignoreing the fact "+
           "they are bolted down.\n");
  add_item("quill",
           "There are quills attacked to each of the desks, it is sort of "+
           "hovering around in the ink well actually.\n");

  add_exit("north", ROOM+"post", "door");
  modify_exit("north", ({ "open", 1 }));
} /* setup() */

void init() {
  ::init();
  add_action("do_mail", "mail");
  add_action("do_mail", "read");
} /* init() */

int do_mail(string str) {
  return (int)"/obj/handlers/mail_track"->mail(str);
} /* do_mail() */

int send_message(string to) {
  return (int)(ROOM+"post")->send_message(to);
} /* send_message() */
