#include "path.h"
inherit "/std/room/basic_room";

object floor, sign;
string log_bing, chair;

void setup() {
  set_short("Meeting room");
  set_long(
"A large oak table dominates this room.  There are some heavy ornate "+
"looking candle sticks in the center of the table.  The table is "+
"sourounded by large expensive looking teak chairs, at one end of the "+
"table are some larger more expensive looking ones.  There is a small "+
"hammer sitting in the middle of the room.  You can see a small sign on "+
"the wall.\n");
  set_light(70);

  add_item("oak table",
           "A large heavy looking oak table.  It was built to last. "+
           "It looks very old, you can tell by all the worn marks "+
           "at the places around the table where people sit.\n");
  add_item("teak chair",
           "The chairs are overly ornate and obviously in a very good "+
           "condition for their age.  The large chairs at the end are "+
           "obviously not well used (you can tell, all the dust).\n");
  add_item("candle stick",
           "The candle sticks are made of silver, however if you try "+
           "and take them, you notice they are attached to the table "+
           "somehow.  The candles seemed to have been carved into the "+
           "shape of a small chicken holding a torch.\n");
  add_item(({ "hammer", "gavel" }),
           "The gavel looks like ones fo those things so common in "+
           "meeting rooms.  You could bang it around a little maybe, "+
           "it might help if you had it though.\n");
  log_bing = 0;
  sign = add_sign("A small sign with not much written on it.\n",
                  "There is no current chair of the meeting.\n");
}

void reset() {
  if (floor)
    return ;
  floor = clone_object("/std/object");
  floor->set_name("floor");
  floor->set_short( "floor" );
  floor->add_property( "determinate", "the " );
  floor->set_long(
"This is the floor of the meeting.  It gives you the right to speak "+
"during the meeting.  Please give this to the next person to speak "+
"after you have finished.\n");
  floor->move(this_object());
}

void init() {
  string str;

  ::init();
/* So I just banged my gavel.
 * You did what?
 * I banged my gavel and did the "order in the court thing"
 */
  str = previous_object()->query_name();
  if (interactive(previous_object()) && (previous_object()->query_lord()
      || str == chair)) {
    add_action("appoint", "appoint");
    add_action("bang", "bang");
    add_action("recover", "recover"); /* recovers the floor in case of
                                       * loss */
    add_action("do_log", "log");
  }
}

int appoint(string str) {
  object ob;

  if (chair && (string)this_player()->query_name() != chair) {
    notify_fail("The chair has already been apointed.\n");
    return 0;
  }
  if (!(ob = find_player(str))) {
    notify_fail("Sorry, cannot find "+str+" to be the chair.\n");
    return 0;
  }
  chair = str;
  say(this_player()->one_short()+" just appointed "+str+" as the "+
        "chair of the meeting.\n", ob);
  write("Ok, "+str+" is now the chair of the meeting.\n");
  tell_object(ob, this_player()->one_short()+
        " just appointed you the chair of the meeting.\n");
  if (environment(ob) == this_object())
    ob->move(this_object());
  sign->set_read_mess("The chair of the meeting is "+str+".\n");
  return 1;
}

int bang(string str) {
  if ((string)this_player()->query_name() != chair) {
    notify_fail("Only the chair can bang the gavel.\n");
    return 0;
  }
  tell_room(this_object(),
            this_player()->one_short()+" bangs the gavel loudly.  STOP "+
            "STOP!\n");
  return 1;
}

int recover() {
  if (!floor)
    reset();
  floor->move(this_player());
  write("Floor recovered.\n");
  say(this_player()->one_short()+" has recovered the floor.\n");
  return 1;
}

int do_log(string str) {
  if (log_bing && !str) {
    write("Stopped logging.\n");
    log_bing = str;
    return 1;
  }
  if (!str) {
    notify_fail("Syntax: log <file_name>\n"+
                "        log by itself to stop logging.\n");
    return 0;
  }
  if (log_bing)
    write("Stopped loggin to "+log_bing+" and started logging to "+str+".\n");
  else
    write("Logging to "+str+".\n");
  log_bing = str;
  return 1;
}

void event_person_say(object ob, string start, string rest) {
  if (log_bing)
    log_file(log_bing, start+rest+"\n");
}

void event_say(object ob, string rest) {
  if (log_bing)
    log_file(log_bing, rest);
}

void event_soul(object ob, string rest) {
  if (log_bing)
    log_file(log_bing, rest);
}

void dest_me() {
  if (floor)
    floor->dest_me();
  ::dest_me();
}
