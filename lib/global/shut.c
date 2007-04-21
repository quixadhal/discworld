/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: shut.c,v 1.2 2000/06/15 03:23:08 pinkfish Exp $
 * $Log: shut.c,v $
 * Revision 1.2  2000/06/15 03:23:08  pinkfish
 * Change to use the config file.
 *
 * Revision 1.1  1998/01/06 04:54:05  ceres
 * Initial revision
 * 
*/
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
** will then handle a nice shutdown.
**      Gordon
**  Bashed for our great mud by pinkfish.  Mangled, wombled and
**  badgered a lot since then.
*/

inherit "/std/container";
#include <config.h>

int time_of_crash;

void setup() {
/* This stops wizzes from cloning armageddon. He only needs to be loaded */
  if ( file_name( this_object() ) != "/global/shut" ) {
    write("This object can't be cloned.\n");
    dest_me();
    return;
    }
  set_name("offler");
  set_living_name("offler");
  add_adjective( ({ "offler", "the", "crocodile" }));
  add_alias("god");
  enable_commands();
  set_short( "Offler" );
  add_property( "determinate", "" );
  set_long("The Crocodile God.  He looks big, green, and, well "+
           "like this really.\n");
  add_alias("shut");
  move(CONFIG_START_LOCATION,
      "Offler the Crocodile God appears in a puff of smoke.",
      "Offler the Crocodile God appears in a puff of smoke.");
  reset_get();
}

#define ishout(str) shout("Offler the Crocodile God shouts: "+str+"\n")

void ashout( string words ) {
  int i;
  object *people;
  people = users();
  for ( i = 0; i < sizeof( people ); i++ )
    tell_object( people[ i ], "Offler the Crocodile God shouts: "+
        words +"\n" );
} /* ashout() */

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
    ashout( "Game reboot in "+ time_to_crash +" seconds." );
    return;
  }
  if (time_to_crash < 60 && time_to_crash % 10 < 2) {
    ashout( "Game is rebooting in "+ time_to_crash +" seconds." );
    return;
  }
  if (time_to_crash % 60 > 1) return;
  time_to_crash /= 60;
  if (time_to_crash > 30) {
/* Give them at least 30 minutes warning... */
    return ;
  }
  if(time_to_crash == 1)
  {
    ashout( "Game is rebooting in one minute." );
    return;
  }
  if (time_to_crash < 10 || !(time_to_crash % 10)) {
    ishout( "Game is rebooting in "+ time_to_crash +" minutes." );
    return;
  }
} /* heart_beat() */

void shut(int minutes) {
  object *players;
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
  players = users();
  for (i=0;i<sizeof(players);i++)
    if (players[i]->query_creator())
      tell_object(players[i], "Offler the Crocodile God wishes you to know:\n"+
          "The mud is rebooting in "+ number_as_string( minutes ) +
          " minutes.\n" );
  set_long("He is firmly concentrating on counting.\n");
  if (time_of_crash)
    write( "There was already a reboot scheduled, set for "+
      (time_of_crash - time()) + " seconds.\n");
  time_of_crash = time() + minutes*60;
  set_heart_beat(1);
} /* shut() */


string long(string str, int dark) {
  ::long();
  if (time_of_crash && this_player()->query_wizard())
     return ::long(str,dark)+
            "Game reboot will be in " + ( time_of_crash - time() ) +
            " seconds.\n";
  return ::long(str,dark);
} /* long() */

void end_it_all() {
  int i;
  object *obs;

  ishout("Shutdown now!");
  obs = users();
  for (i=0;i<sizeof(obs);i++)
    call_out("force_quit", i, obs[i]);
  call_out("blue", 10);
} /* end_it_all() */

void force_quit(object ob) {
  if (!objectp(ob))
    return ;
/* modified to use the departure lounge for everyone. */
  if (!ob->query_property("quiting frog"))
    ob->quit_alt();
} /* force_quit() */

/*
 * Check to see that everyone has been quit.  If we have taken
 * longer than 2 minutes to quit everyone, then we shutdown
 * anyway. -- Pinkfish
 */
void blue() {
  if (sizeof(users()) > 0 &&
      time() - time_of_crash < 120)
    call_out("blue", 10);
  else
    shutdown(0);
} /* blue() */

int query_time_to_crash() {
  if ( !time_of_crash ) {
    call_out( "dest_me", 1 );
    return 9999;
  }
  return time_of_crash - time();
} /* query_time_to_crash() */
