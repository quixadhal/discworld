/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: shut.c,v 1.8 2003/05/05 07:00:55 ceres Exp $
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
**  Bashed for our great mud by pinkfish.
**
**  Changed to use quit_alt on Fri Dec  6 10:35:04 WST 1996
**    -- Pinkfish.
*/

#include <network.h>

#define OFFLER_SHADOW "/std/shadows/misc/offler_shadow"

#undef CONVERTER
#undef AUTO_RESURRECT

inherit "/std/container";

int time_of_crash, time_of_last_shout;

void setup() {
/* This stops wizzes from cloning armageddon. He only needs to be loaded */
  if (file_name(this_object()) != "/obj/shut") {
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
  reset_get();
} /* setup() */

#define ishout(str) user_event("say", "%^BOLD%^%^RED%^Offler the Crocodile God shouts: "+str+"%^RESET%^\n", 0)

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
      ishout( "Game weboot in "+ time_to_crash +" thecondth." );
      return;
   }
   if (time_to_crash < 60 && time_to_crash % 10 < 2) {
      ishout( "Game ith webooting in "+ time_to_crash +
            " thecondth.  Go away!" );
      return;
   }
   if (time_to_crash % 60 > 5)
      return;
   time_to_crash /= 60;
   if (time_to_crash == time_of_last_shout)
      return;
   time_of_last_shout = time_to_crash;
   if (time_to_crash > 30) {
      /* Give them at least 30 minutes warning... */
      return ;
   }
   if(time_to_crash == 1) {
      ishout( "Game ith webooting in one minute.  Quit now!" );
      return;
   }
   if (time_to_crash < 10 || !(time_to_crash % 10)) {
      ishout( "Game ith webooting in "+ time_to_crash +" minuteth." );
      return;
   }
} /* heart_beat() */

void shut(int minutes, string reason) {
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
#ifdef CONVERTER
   if (!CONVERTER->ok_to_shut()) {
      dest_me();
      return;
   }
#endif
   players = users();
   for (i=0;i<sizeof(players);i++) {
     if ( !players[ i ] )
       continue;
     if (players[i]->query_creator())
     tell_object(players[i], "%^BOLD%^%^RED%^Offler the Crocodile God wishes "
                 "you to know:  The mud is rebooting in "+
                 number_as_string( minutes ) + " minutes.%^RESET%^\n" );
   }
   set_long("He is firmly concentrating on counting.\n");
   if (time_of_crash)
     write( "There was already a reboot scheduled, set for "+
           (time_of_crash - time()) + " seconds.\n");

   move("/d/am/buildings/drum/mendeddrum",
      "Offler the Crocodile God appears in a puff of smoke.",
      "Offler the Crocodile God appears in a puff of smoke.");

   time_of_crash = time() + minutes*60;
   time_of_last_shout = minutes;
   set_heart_beat(1);
#ifdef AUTO_RESURRECT  
   call_out( "check_ghosts", 60 );
#endif
   
   if(file_name(previous_object()) == "/obj/handlers/garbage")
     log_file("REBOOT", "Shutdown requested automatically at "+ctime(time())+
              "\n");
   else if(this_player())
     log_file("REBOOT", "Shutdown at "+ctime(time())+" requested by " +
              this_player()->query_cap_name()+" for " + reason + "\n");
   else
     log_file("REBOOT", "Shutdown at "+ctime(time())+" requested by " +
              file_name(previous_object()) + " for " + reason + "\n");

} /* shut() */

string long(string str, int dark) {
  ::long(str, dark);
  if ( time_of_crash && this_player() && this_player()->query_creator() )
     return ::long(str,dark)+
            "Game reboot will be in " + ( time_of_crash - time() ) +
            " seconds.\n";
  return ::long(str,dark);
} /* long() */

void end_it_all() {
   int i;
   object *obs;
   
   ishout( "Thutdown now!" );
   obs = users();
   for (i=0;i<sizeof(obs);i++) {
      call_out("force_quit", i, obs[i]);
   }
   call_out("blue", 10);
} /* end_it_all() */

void force_quit( object ob ) {
   if ( !objectp( ob ) )
      return;
   ob->stop_all_fight();
   if(ob->query_login_ob()) {
     ob->dest_me();
     return;
   }
   if ( !environment( ob ) )
     ob->move( "/d/am/buildings/drum/mendeddrum" );
   /* Change it to use the quit_alt code. */
   if(environment(ob) &&
      file_name( environment( ob ) ) != "/room/departures" ) {
     ob->quit_alt();
   }
   /* If the quit_alt doesn't work.  Force a hard quit */
   if (environment(ob) && 
       file_name( environment( ob ) ) != "/room/departures" ) {
     ob->quit();
   }
} /* force_quit() */

/*
 * Check to see that everyone has been quit.  If we have taken
 * longer than 4 minutes to quit everyone, then we shutdown
 * anyway. -- Pinkfish
 */
void blue() {
  if (sizeof(users()) > 0 &&
      time() - time_of_crash < 240)
    call_out("blue", 10);
  else {
    SERVICES_D->eventShutdown(1);
    shutdown(0);
  }
} /* blue() */

int query_time_to_crash() {
   if ( !time_of_crash ) {
      call_out( "dest_me", 1 );
      return 9999;
   }
   set_heart_beat( 1 );
   return time_of_crash - time();
} /* query_time_to_crash() */

int query_unambushable() { return 1; }

void do_ambushed() {
   write( "Bad mistake...\n" );
   this_player()->run_away();
} /* do_ambushed() */

#ifdef AUTO_RESURRECT
void person_died( string word, int number ) {
   object thing;
   thing = find_player( word );
   if ( !thing )
      return;
   switch ( number ) {
      case 0 :
         break;
      case 1 :
         tell_object( thing, "$I$5=Offler tells you exclaiming: Well, "+
               "weally!  Thilly mortal, of all the timeth to die!\n" );
         break;
      case 2 :
         tell_object( thing, "$I$5=Offler tells you: Don't wowwy, I'll "+
               "welieve the chap with the thythe of you thith time.\n" );
         break;
      case 3 :
         thing->remove_ghost();
         clone_object( OFFLER_SHADOW )->setup_shadow( thing );
         break;
      default :
         tell_object( thing, "$I$5=Offler tells you exclaiming: And behave "+
               "yourthelf until I'm done, for goodneth' thake!\n" );
         return;
   }
   call_out( "person_died", 3, word, number + 1 );
} /* person_died() */

void person_dead( object thing, int number ) {
   if ( !thing )
      return;
   if ( thing->query_property( "noregen" ) ) {
      call_out( "person_dead", 3, thing, number );
      return;
   }
   switch ( number ) {
      case 0 :
         tell_object( thing, "$I$5=Offler tells you exclaiming: Well, "+
               "weally!  Can't have you wunning awound detheathed!\n" );
         break;
      case 1 :
         thing->remove_ghost();
         clone_object( OFFLER_SHADOW )->setup_shadow( thing );
         break;
      default :
         tell_object( thing, "$I$5=Offler tells you exclaiming: Now behave "+
               "yourthelf until I'm done, for goodneth' thake!\n" );
         return;
   }
   call_out( "person_dead", 3, thing, number + 1 );
} /* person_dead() */

void check_ghosts() {
   object thing;
   foreach( thing in users() ) {
      
      if ( !thing ) {
          continue;
      }
      if ( !thing->query_property( "dead" ) ) { 
         continue;
      }
      if ( (int)thing->query_deaths() > (int)thing->query_max_deaths() ) {
         continue;
      }
      call_out( "person_dead", 3, thing, 0 );
   }
} /* check_ghosts() */

#endif
