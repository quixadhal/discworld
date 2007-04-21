/*  -*- LPC -*-  */
/*
 * $Id: start_position.c,v 1.14 2003/07/16 19:04:48 pinkfish Exp $
 */

/**
 * This file contains all the start position related code.  It sets up
 * and controls the start location of the player.  This was split off from
 * the main player code by Pinkfish in April 1998.
 * @see /global/player.c
 * @author Pinkfish
 */
inherit "/std/basic/virtual_quit_control";

#include <playerinfo.h>
#include <config.h>

#define MULTIPLAYER "/obj/handlers/multiplayer"

private int  *saved_co_ords;
private string last_pos;
private string *starts;

protected void inform_entered_game();
int query_creator();
string query_name();
string query_cap_name();
string the_short();
varargs int move(string pos, string messin, string messout);
string query_nationality_start_location();

void create() {
   starts = ({ });
} /* create() */

/**
 * This method returns the co-ordinates of the room in which the player
 * last saved.
 * @see check_last_pos()
 * @see move_to_start()
 * @see query_start_pos()
 * @return the last saved co-ordinates
 */
int *query_saved_co_ords() { return saved_co_ords; }

/**
 * This method sets the current start location.
 * @param last_pos the start location
 */
protected void set_last_pos(string pos) {
  if(pos[0..4] != "/room")
    last_pos = pos;
} /* set_last_pos() */

/**
 * This method returns the last saved position of the player.
 * @return the last saved position
 */
string query_last_pos() {
   return last_pos;
}

/**
 * This method returns the starting location of the player.  This the
 * location they will start at if the saved position does not work
 * for some reason.
 * @return the start position to use
 * @see query_starts()
 * @see add_start()
 * @see remove_start()
 * @see reset_starts()
 * @see set_start_location()
 */
string query_start_pos() {
   string start_pos;

   if ( !sizeof( starts ) ) {
      start_pos = query_nationality_start_location();
   } else {
      start_pos = starts[ 0 ];
   }
   if ( !find_object( start_pos ) ) {
      if ( catch( call_other( start_pos, "??" ) ) ) {
         start_pos = query_nationality_start_location();
      }
   }
   return start_pos;
} /* query_start_pos() */

/**
 * This method returns the current array of possible start positions on
 * the player.
 * @return the array of possible start locations
 * @see query_start_pos()
 * @see add_start()
 * @see remove_start()
 * @see reset_starts()
 * @see set_start_location()
 */
string *query_starts() {
   return starts;
}

/**
 * This method resets the start locations back to an empty array.
 * @see query_start_pos()
 * @see add_start()
 * @see remove_start()
 * @see query_starts()
 * @see set_start_location()
 */
void reset_starts() { starts = ({ }); }

/**
 * This method will add a new start location to the player.  The
 * description will be used in the starts command.
 * @param start_file the file name of the start location
 * @param start_desc the description of the start location
 * @see query_start_pos()
 * @see reset_starts() 
 * @see remove_start()
 * @see query_starts()
 * @see set_start_location()
 * @example
 * // NB: This should be done with a define :)
 * player->add_start("/d/am/am/mendeddrum", "the mended drum");
 */
void add_start( string start_file, string start_desc ) {
   if ( !starts ) {
      starts = ({ });
   }
   if ( member_array( start_file, starts ) != -1 ) {
      return;
   }
   starts += ({ start_file, start_desc });
} /* add_start() */

/**
 * This method will remove a start location from the player.
 * @param start_file the file name of the location to remove
 * @see query_start_pos()
 * @see reset_starts()
 * @see add_start()
 * @see query_starts()
 * @see set_start_location()
 * @example
 * // NB: This should be done with a define :)
 * player->remove_start("/d/am/am/mendeddrum");
 */
void remove_start( string start_file ) {
   int i;

   if ( !starts ) {
      return;
   }
   i = member_array( start_file, starts );
   if ( i == -1 ) {
      return;
   }
   starts = delete( starts, i, 2 );
} /* remove_start() */

/**
 * This method sets the start location of the player.
 * @param start_file the start location to set
 * @see query_start_pos()
 * @see reset_starts()
 * @see remove_start()
 * @see query_starts()
 * @see add_start()
 * @example
 * // NB: This should be done with a define :)
 * player->set_start_location("/d/am/am/mendeddrum");
 */
void set_start_location( string start_file ) {
  int i;

  if ( !starts) {
    return ;
  }

  i = member_array(start_file, starts);
  if (i <= 0) {
    return ;
  }
  starts = starts[i..i + 1] + starts[0..i - 1] + starts[i + 2..];
} /* set_start_location() */

/**
 * This method moves the player to the last saved position.  This should
 * only be called in the startup sequence.  If the last location could
 * not be loaded, the the start position will be used.
 * @see query_last_pos()
 * @see quert_start_pos()
 */
protected void move_to_start_pos() {
   if (!last_pos || catch(call_other(last_pos, "??"))) {
      last_pos = query_start_pos();
      saved_co_ords = 0;
   }
   move(last_pos);

   /*
    * Set the old coord if one does not already exist.  Otherwise don't
    * destroy the already existing scheme.
    */
   if ( !last_pos->query_co_ord() && !last_pos->query_property( "no map" ) &&
         saved_co_ords && !query_creator() ) {
      last_pos->set_co_ord( saved_co_ords );
   }

   inform_entered_game();
} /* move_to_start_pos() */

private function query_extra_login_stuff(string start) {
   string ip_str;
   object *obs;
   object mph;
   string alert;
   int num;
   string *dups;

   switch (this_object()->query_invis()) {
      case 3 :
         start += " (trustee invisible)";
         break;
      case 2 :
         start += " (director invisible)";
         break;
      case 1 :
         start += " (invisible)";
         break;
   }

   mph = load_object(MULTIPLAYER);
   obs = filter(users() - ({ this_object() }),
                (: query_ip_number($1) == query_ip_number(this_object()) &&
                   !$1->query_login_ob() :));
   if (sizeof(obs) &&
       sizeof((dups = mph->check_allowed(this_object(), obs)))) {
      ip_str = " (" + query_ip_name(this_object()) + ")";
      ip_str += " Duplicate: " + query_multiple_short(dups);
   }

   if (!catch(num = PLAYERINFO_HANDLER->query_alerts_for(query_name()))) {
      if (num > 0) {
         alert = " %^BOLD%^%^RED%^" + num + " alert";
         if (num > 1) {
            alert += "s";
         }
         alert += "%^RESET%^";
      }
   }

   if (ip_str) {
      if (!alert) {
         alert = "";
      }
      return (: $(start) +
                       ($1->query_creator()?$(ip_str) + $(alert):"") :);
   } else {
      if (alert) {
         return (: $(start) + ($1->query_creator()?$(alert):"") :);
      } else {
         return (: $(start) :);
      }
   }

} /* query_extra_login_stuff() */

/**
 * This method handles informing people that the player has entered the
 * game.
 * @see move_to_start_pos()
 */
protected void inform_entered_game() {
   string logon_str;

   if ("/d/liaison/master"->query_member( query_name() ) ) {
      logon_str = query_cap_name() + " enters "
           "" + mud_name() + " %^YELLOW%^(Liaison)";
   } else {
      logon_str = query_cap_name() + " enters " +
           ( this_object()->query_property( "guest" ) ? "as a guest of " : "" ) +"" + mud_name() + ""+
           ( this_object()->query_property( "new player!" ) ? " (new player)" : "" );
   }
   user_event( this_object(), "inform",
               query_extra_login_stuff(logon_str),
               "logon", this_object());

   say(the_short()+" enters the game.\n", 0);
   last_pos->enter(this_object());
} /* inform_enter_game() */

/**
 * THis method is called to inform people that someone has just reconnected.
 */
void inform_reconnect_game() {
   
   user_event( "inform", 
         query_extra_login_stuff(query_cap_name() +
         " reconnects"), "link-death", this_object() );
} /* inform_reconnect_game() */

/**
 * This method figured out what the last saved position was and sets it
 * on the player.  This basicly sets the last saved postion.
 * @see query_last_pos()
 */
void check_last_pos() {
   last_pos = find_start_pos(this_object(), environment());
   saved_co_ords = find_start_coord(this_object(), environment());
} /* check_last_pos() */
