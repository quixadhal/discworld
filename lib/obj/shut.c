/*
** Armageddon 3.0 is largely a rip off of the 2.4.5 version
** that most of us are familiar with. The original author
** left no credits, but the work is recognized.
** This version has a number of improvements, and hopefully,
** a neater finished product.
**
** To use, merely use the command:
**   call shut(##) /obj/shut
** where ## is the number of minutes till shutdown. Armageddon
** will then handle a nice shutdown, including lifts to the shop.
**	Gordon
**  Bashed for our great mud by pinkfish.
*/

inherit "/std/object";
 
int transport_offer, time_of_crash;
 
void setup() {
/* This stops wizzes from cloning armageddon. He only needs to be loaded */
  if (file_name(this_object()) != "/obj/shut") {
    write("This object can't be cloned.\n");
    dest_me();
    return;
    }
  set_name("offler");
  set_living_name("offler");
  add_adjective( ({ "offler", "the", "crockodile" }));
  add_alias("god");
  enable_commands();
  set_short("offler");
  set_long("The crockodile god.  He looks big green and well like this "+
           "really.\n");
  add_alias("shut");
  move("/d/am/am/mendeddrum",
      "Offler the Crockodile God appears in a puff of smoke.\n",
      "Offler the Crockodile God appears in a puff of smoke.\n");
  reset_get();
}
 
#define ishout(str) shout("Offler the Crockodile God shouts: "+str+"\n")
 
void heart_beat() {
int time_to_crash;
  if (!time_of_crash) return;
  time_to_crash = time_of_crash - time();
  if (time_to_crash < 1) {
    call_out("end_it_all",0);
    set_heart_beat(0);
    return;
  }
  if (time_to_crash < 10)  {
    ishout("Game shutdown in "+time_to_crash+" seconds.");
    return;
  }
  if (time_to_crash < 60 && time_to_crash % 10 < 2) {
    ishout("Game is shutting down in "+time_to_crash+" seconds.");
    return;
  }
  if (time_to_crash % 60 > 1) return;
  time_to_crash /= 60;
  if (time_to_crash < 5 && !transport_offer) {
    ishout("Tell me if you want a lift to the shop.");
    transport_offer = 1;
    return;
  }
  if (time_to_crash > 30) {
/* Give them at least 30 minutes warning... */
    return ;
  }
  if (time_to_crash < 10 || !(time_to_crash % 10)) {
    ishout("Game is shutting down in "+time_to_crash+" minutes.");
    return;
  }
} /* heart_beat() */
 
void shut(int minutes) {
int i;
string fname;
 
  if (!intp(minutes)) {
    write("Bad argument\n");
    return;
  }
  fname = file_name(previous_object());
  if (minutes <= 0) {
    write("No time given\n");
    return;
  }
  set_long("He is firmly concentrating on counting.\n");
  if (time_of_crash)
    write("There was already a shutdown in progress, set for "+
      (time_of_crash - time()) + " seconds.\n");
  time_of_crash = time() + minutes*60;
  set_heart_beat(1);
} /* shut() */

void event_person_tell(object ob) {
  call_out("move_me",0,this_player());
} /* event_person_tell() */
 
void catch_tell(string str) {
string dummy;
 
  if (!transport_offer) return;
  if (sscanf(str, "%s tells you: %s", dummy, dummy) != 2)
    return;
/* The call_out avoids arriving in the shop before you tell armageddon
** to take you there. He is good, but no precognisant :)
*/
  call_out("move_me",0,this_player());
} /* catch_tell() */

void move_me(object me) {
  me -> move_player("X", "/d/am/am/filgreeshop");
} /* move_me() */

string long(string str, int dark) {
  ::long();
  if (time_of_crash && this_player()->query_wizard())
     return ::long(str,dark)+
            "Game shutdown will be in " + (time_of_crash - time()) +
            " seconds.\n";
  return ::long(str,dark);
} /* long() */

void end_it_all() {
  int i;
  object *obs;

  ishout("Shutdown now!");
  obs = users();
  for (i=0;i<sizeof(obs);i++)
    call_out("force_quit", 0, obs[i]);
  call_out("blue", 10);
} /* end_it_all() */

void force_quit(object ob) {
  ob->quit();
} /* force_quit() */

void blue() {
  shutdown(0);
} /* blue() */

int query_time_to_crash() { return time_of_crash - time(); }
