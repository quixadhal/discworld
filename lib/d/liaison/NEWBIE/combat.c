/*
 * $Id: combat.c,v 1.11 2003/02/19 06:54:24 ceres Exp $
 */

//swiped from the Thieves' Guild room with modifications - Rue
/*combat classroom*/

/*
 * This is the central combat room. Newbies enter here and are then escorted
 * to a training room to practise on the dummy.  Greg controls who gets
 * put in which room. To add more rooms, just create them and then add them to
 * gregs arrays of available rooms.
 */
#include "path.h"
#define MAX 100;

inherit PATH+"inside";

object instructor;
object sign;

void setup() {
  set_zone( "Newbie" );
  set_short("Combat Training Room");
  add_property("determinate", "");
  set_light( 75 );
  add_property("no teleport", 1);
  
  set_long("This is a small training room, designed to help the new "
           "adventurer get off to a good start on Discworld.  The air is "
           "stale with the combined stench of sweat, beer, and old "
           "warriors.  There is a small sign attached to the wall.\n");
  add_item( "floor", "The floor is made of flagstones, cemented together "
            "with centuries of accumulated muck.\n");
  add_item( "wall", "The walls are marked with mysterious stains that may or "
            "may not be blood.  But surely training dummies don't bleed.\n");
  add_item( "ceiling", "The ceiling appears to be dingy patches of plaster "
            "stuck between old oak rafters that have turned black with "
            "age.\n");

  /* Make it so you cannot godmother out of here */
  add_property("no godmother", 1);

  add_exit( "foyer", PATH + "foyer", "door" );
  add_alias( "southwest", "foyer");

  add_exit("one", PATH + "combat_room1", "door");
  add_exit("two", PATH+"combat_room2", "door");
  add_exit("three", PATH+"combat_room3", "door");

  modify_exit("one", ({"function", "check_room", "closed", 1,
                         "look", "It is the door to training room one.\n"}));
  modify_exit("two", ({"function", "check_room", "closed", 1,
                         "look", "It is the door to training room two.\n"}));
  modify_exit("three", ({"function", "check_room", "closed", 1,
                         "look", "It is the door to training room three.\n"}));
  
}

void reset() {
  if(!sign) {
    sign=add_sign("This is a sign stuck to the wall.  It contains "
                  "instructions on how to use this room.\n",
                  "Welcome to the newbie combat area!  If you would like "
                  "to practise your combat skills just ask Greg.  To do "
                  "this say \"can I practise please\".",
                  0, "sign", "general");
  }
}
void dest_me() {
  if(sign)
    sign->dest_me();
  
  if(instructor)
    instructor->dest_me();
  ::dest_me();
}

void init() {
  call_out("get_instructor", 0);
}

void get_instructor() {
  if(!instructor)
    instructor = load_object(PATH +"greg");
  
  if(instructor && environment(instructor) != this_object()) {
    instructor->move( this_object(), "$N marches in from another room.");
  }
}/*get_instructor*/

int check_room(string str, object ob, string special_mess) {
  int room;

  switch(str) {
  case "one":
    room = 0;
    break;
  case "two":
    room = 1;
    break;
  case "three":
    room = 2;
  }

  if(ob == instructor)
    return 1;
  
  if(!instructor || instructor->query_using(room) != ob) {
    tell_object(ob, "The door slams closed in your face.\n");
    modify_exit("one", ({ "closed", 1}));
    return notify_fail("");
  }

  return 1;
}
