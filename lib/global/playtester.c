/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: playtester.c,v 1.18 2003/07/21 18:23:15 pinkfish Exp $
 * $Log: playtester.c,v $
 * Revision 1.18  2003/07/21 18:23:15  pinkfish
 * Change the object characters for pts and seniors.
 *
 * Revision 1.17  2003/04/03 19:12:59  ceres
 * Modified to deal with new login object
 *
 * Revision 1.15  2002/08/11 17:49:44  drakkos
 * Added a method to query if a location is a PT area.
 * ((
 *
 * Revision 1.14  2002/08/04 07:45:08  drakkos
 *  Forcibly unlocked by ceres
 *
 * Revision 1.13  2002/07/20 12:54:35  drakkos
 * Updated the senior PT command define.
 *
 * Revision 1.12  2002/07/19 01:45:53  pinkfish
 * Add in stuff to deal with senior pts and the mail command.
 *
 * Revision 1.11  2002/07/19 01:42:04  drakkos
 *  Forcibly unlocked by pinkfish
 *
 * Revision 1.10  2002/07/18 02:27:51  drakkos
 * Added in a forgotten ::event_move_object.
 *
 * Revision 1.9  2002/07/17 23:24:50  drakkos
 * Seperation of room and personal protection into seperate functions... room protection is called by using enable_pt_protection (so that legacy code doesn't break).  Personal protection is called using enable_personal_pt_protection.  Room protection will always over-ride personal protection to allow for creators to safely PT protect their projects without worrying about time delays and such.
 *
 * Revision 1.7  2002/07/12 17:23:27  drakkos
 * Added checks for disabling and enabling protection to make sure it doesn't get enabled twice or disabled twice.
 *
 * Revision 1.6  2002/07/11 23:39:18  pinkfish
 * Make the protection only last five minutes..
 *
 * Revision 1.5  2002/07/11 23:25:28  drakkos
 *  Forcibly unlocked by pinkfish
 *
 * Revision 1.4  2002/06/27 17:47:32  drakkos
 * Added some messages and logging for PT protection, and repressed XP earned when protection is on.
 *
 * Revision 1.3  2002/06/25 19:59:18  pinkfish
 * Fix up some old effect based things so it calls the correct
 * functions.
 *
 * Revision 1.2  2002/06/25 19:51:44  pinkfish
 * Fix up some issues with the protection stuff.
 *
 * Revision 1.1  2002/06/25 18:33:59  pinkfish
 * Initial revision
 *
 * Revision 1.6  2001/06/16 05:09:29  presto
 * Fixed warning.  (Added dummy 'object' argument to query_object_type)
 *
 * Revision 1.5  2001/03/23 23:08:20  ceres
 * Added go_invis param
 *
 * Revision 1.4  1999/02/10 04:14:12  ceres
 * Modified to get creator news from the login handler
 *
 * Revision 1.3  1998/10/30 09:10:15  pinkfish
 * Fix up the name variable to be a method call.
 *
 * Revision 1.2  1998/04/13 11:37:08  pinkfish
 * Removed all the swap stuff fomr the creator object and
 * generly cut it back :)
 *
 * Revision 1.1  1998/01/06 04:54:05  ceres
 * Initial revision
 * 
 */
/* this is the creator player object */

/**
 * This is the playtester player object.  It gives the playtesters the
 * bonus extra commands needed by happy playtesters.
 * @author Pinkfish
 * @see /global/player.c
 * @started Tue Jun 25 11:27:05 PDT 2002
 */
#include <login_handler.h>
#include <command.h>
#include <player.h>
#define PROTECTION_LOG "/log/secure/playtesters/protection_log"
#define MAX_COUNTER 2

class playtester_data {
   int protect;
   int hp;
   string log_file;
   int log_death;
   int log_damage;
   int turn_off;
   int protection_counter;
   int room_protection;
}


inherit "/global/player";

int query_in_pt_arena();

private nosave class playtester_data _pt_data;

int query_pt_area (object ob) {
  string *file;
  
  if (strsrch (base_name (ob), "_pt") != -1) {
    return 1;
  }
  if (strsrch (base_name (ob), "_dev") != -1) {
    return 1;
  }

  file = explode (base_name (ob), "/");
  
  if (file[1] == "playtesters") {
    return 1;
  }
  
  return 0;
}

void create() {
   ::create();
   _pt_data = new(class playtester_data);
}

/** @ignore yes */
void move_player_to_start(string bong, int new_pl, string c_name, string ident, int go_invis) {
  if(!sscanf(file_name(previous_object()), "/secure/login#%*s") &&
     !sscanf(file_name(previous_object()), "/secure/nlogin#%*s"))
    return 0;
#ifndef NEW_DRIVER
  //enable_wizard();
#endif
  ::move_player_to_start(bong, new_pl, c_name, ident, go_invis);
  AddSearchPath(({ DIR_PLAYTESTER_CMDS }));
  if (PLAYTESTER_HAND->query_senior_playtester(query_name())) {
     AddSearchPath(({ DIR_SENIOR_PLAYTESTER_CMDS }));
  }
  if (PLAYTESTER_HAND->query_exec_access (query_name())) {
     AddSearchPath(({ DIR_EXEC_PLAYTESTER_CMDS }));
  }
} /* move_player_to_start() */

/**
 * This method tells us if the object is a playtester or not.
 * @return 1 if the object is a creator, 0 if not.
 */
int query_playtester() { return 1; }

/** @ignore yes */
string query_object_type(object) {
  if (PLAYTESTER_HAND->query_pt_exec(query_name())) {
    return "E";
  }
  if (PLAYTESTER_HAND->query_senior_playtester(query_name())) {
    return "P";
  }
  return "p";
} /* query_object_type() */

/**
 * This method returns the status of the protect flag.  2 means that they have 
 * their room protection enabled, 1 means that they have their personal protection
 * enabled, 0 means that they are not.
 * @return The value of the protect flag
 */
int query_pt_protection() {
  if (_pt_data->room_protection) {
    return 2;
  }
  return _pt_data->protect;
} // query_pt_protection()


/**
 * This method sets the PT as being under room protection.  Room protection does not 
 * expire and lasts until it is switched off.  It also over-rides personal protection.  
 * @return 1 on success, 0 on failure.
 */
 
int enable_pt_protection() {
  if (_pt_data->room_protection != 1) {
   log_file (PROTECTION_LOG, "%s: %s had room PT "
    "protection enabled by %s.\n", ctime(time()), this_player()->query_name(), 
    base_name (environment (this_player())));     
    tell_object (this_player(), "%^BOLD%^You have had your playtester "
      "protection enabled by an external source.  It will last until switched "
      "off by another external source.\n%^RESET%^");
    _pt_data->room_protection = 1;
    return 1;
  }
  
  return 0;
}

/**
 * This method sets the PT as no longer being under room protection.  Room protection does not 
 * expire and lasts until it is switched off.  It also over-rides personal protection.  
 * @return 1 on success, 0 on failure.
 */
int disable_pt_protection() {
  if (_pt_data->room_protection == 1) {
   log_file (PROTECTION_LOG, "%s: %s had room PT "
    "protection disabled by %s.\n", ctime(time()), this_player()->query_name(), 
    base_name (environment (this_player())));   
    tell_object (this_player(), "%^BOLD%^The playtester protection enabled "
      "by an external source has been switched off.\n%^RESET%^");
    _pt_data->room_protection = 0;
    return 1;
  }
  return 0;
}

/**
 * This method sets the protect flag so that adjust_hp and
 * do_death are masked.
 */
int enable_personal_pt_protection() {
  if (query_pt_protection()) {
    return 0;
  }
  
   _pt_data = new(class playtester_data);
   _pt_data->hp = ::query_hp();
   _pt_data->protect = 1;
   _pt_data->log_file = "/d/playtesters/log/pain";
   tell_room (environment (this_player()), this_player()->one_short() 
      + " looks as if " + this_player()->query_pronoun() + " no longer has "
      "any fear of the risks of the real world.\n", this_player());
   log_file (PROTECTION_LOG, "%s: %s enabled PT "
    "protection (%s).\n", ctime(time()), this_player()->query_name(), 
    base_name (environment (this_player())));
   if (_pt_data->turn_off) {
      remove_call_out(_pt_data->turn_off);
   }
   
   if (!query_pt_area (environment (this_player()))) {
      tell_object (this_player(), "%^BOLD%^You feel somewhat more secure, as if "
        "you can no longer be harmed by conventional weapons (This will "
        "only last a maximum of fifteen minutes).%^RESET%^\n");    
     _pt_data->protection_counter = 0;
     _pt_data->turn_off = call_out("disable_personal_pt_protection", 5 * 60);
   }
   else {
      tell_object (this_player(), "%^BOLD%^You feel somewhat more secure, as if "
        "you can no longer be harmed by conventional weapons (This will "
        "last until you switch it off or leave a PT area).%^RESET%^\n");   
  }            
   
   return 1;
} // enable_pt_protection()


/**
 * This method resets the protect flag so that adjust_hp and
 * do_death are not masked, i.e. the player can be hurt in the
 * normal(*) way.
 *
 * (*) Having "normal" ways to hurt people is quite sick...  But it's fnu!
 */
int disable_personal_pt_protection() {
  if (query_pt_protection() != 1) {
    return 0;
  }
    
  if (this_player()) {
    _pt_data->protection_counter = MAX_COUNTER;
  }
  
  if (_pt_data->protection_counter < MAX_COUNTER) {
    tell_object (this_player(), "%^BOLD%^You still have your PT protection "
      "enabled.\n%^RESET%^");
    log_file (PROTECTION_LOG, "%s: %s warned about still enabled PT "
      "protection (%s).\n", ctime(time()), this_player()->query_name(), 
      base_name (environment (this_player())));      
    _pt_data->turn_off = call_out("disable_personal_pt_protection", 5 * 60);   
    _pt_data->protection_counter = _pt_data->protection_counter + 1;
    return 0;
  }
  
   _pt_data->turn_off = 0;
   _pt_data->protect = 0;
   _pt_data->protection_counter = 0;
   log_file (PROTECTION_LOG, "%s: %s disabled PT "
    "protection (%s).\n", ctime(time()), this_player()->query_name(), 
    base_name (environment (this_player())));   
   tell_object (this_player(), "%^BOLD%^You feel less secure in your "
    "indestructability.%^RESET%^\n");
   tell_room (environment (this_player()), this_player()->one_short() 
      + " looks as if the risks of the real world are once again "
      "relevant.\n", this_player());
   return 1;
} // disable_pt_protection()



/**
 * This method sets the fake hitpoints.
 * @param hp The new value of fake hitpoints
 */
void pt_set_hp( int hp ) {
    
   _pt_data->hp = hp;
} // pt_set_hp()

/**
 * This method returns the number of fake hitpoints.
 * @return The number of fake hitpoints
 */
int pt_query_hp() {
   return _pt_data->hp;
} // pt_query_hp()


/** @ignore */
object pt_make_corpse() {
   // Make a corpse in case someone needs it...

   object corpse;

   corpse = clone_object( "/obj/corpse" );
   corpse->set_owner( 0, this_object() );
   corpse->set_ownership( query_name() );
   corpse->set_race_name( query_race() );
   if ( !query_race_ob() ) {
      corpse->set_race_ob( "/std/races/unknown" );
   } else {
      corpse->set_race_ob( query_race_ob() );
   }
   corpse->add_adjective( query_adjectives() );
   corpse->start_decay();

   return corpse;
} // pt_make_corpse()


/**
 * This method replaces the normal do_death() when testing.  It gives
 * players a message about how it would have hurt if it had been for real.
 * @param thing The object that killed us.
 * @return A corpse
 */
object pt_do_death() {
   object corpse, flowers;

   
//   if( _pt_data->log_death && stringp( _pt_data->log_file ) )
   DEATH->death_informer( this_object(), 0, _pt_data->log_file, 1 );
   
   tell_object( this_object(),
         "The Death of Playtesters sneaks up behind you and taps "
         "you on the shoulder.\n"
         "%^CYAN%^The Death of Playtesters says: MY, WE'VE BEEN A BIT "
         "CARELESS, HAVEN'T WE?%^RESET%^\n"
         "The Death of Playtesters coughs.\n"
         "%^CYAN%^The Death of Playtesters says: Sorry about the Voice.  "
         "We have to do that, you know.  Well, you should really be dead "
         "now, but the Playtesters' Union renegotiated the whole death deal "
         "with the powers that be.%^RESET%^\n" );

     
   
   // Restore player to full health
   call_out ("pt_set_hp", 0, query_max_hp());
    
   // Make a corpse and give them flowers
   corpse = pt_make_corpse();
   corpse->move( environment( ) );
   
   flowers = clone_object( "/std/object" );
   flowers->set_name( "flowers" );
   flowers->set_short( "bunch of flowers" );
   flowers->add_alias( "bunch" );
   flowers->add_adjective( ({ "bunch", "of" }) );
   flowers->set_main_plural( "bunches of flowers" );
   flowers->add_plural( ({ "bunches", "bunches of flowers" }) );
   flowers->set_long( "This is a small bunch of nice flowers.\n" );
   flowers->set_weight( 5 + random( 10 ) );
   flowers->set_value( 0 );
   flowers->set_read_mess( "Rest In Peas", "common" );
   flowers->move( corpse );
   
   return corpse;
} // pt_do_death()


/**
 * This method adjusts the fake hitpoints.
 * @param hp The value to add to fake hitpoints
 * @return hp after adjustment
 */
varargs int pt_adjust_hp( int hp, object attacker ) {
       
   _pt_data->hp += hp;

   if( ( hp < 0 || hp > 10 ) && _pt_data->log_damage && stringp( _pt_data->log_file ) )
      log_file( _pt_data->log_file, ctime( time() ) +": "+
                query_name() +" - "+ hp +" hitpoints.\n" );

   // Check if they "die"

   if( _pt_data->hp <= 0 && hp < 0) {
   
      pt_do_death();
   } else if( _pt_data->hp > ::query_max_hp() ) {
      // Make sure they don't get too many hp
      _pt_data->hp = ::query_max_hp();
   }

   // Wimpy
   if( hp < 0 && _pt_data->hp > 0 && attacker && attacker != this_object() ) {
      if( 100 * _pt_data->hp < query_wimpy() * ::query_max_hp() ) {
         run_away();
      }
   }

   return _pt_data->hp;
   
} // pt_adjust_hp()




/**
 * This method sets the log file for logging of PT fake deaths and damage.
 * The default is /d/playtesters/log/pain.
 * @param str The filename of the log file.
 */
void set_pt_log_file( string str ) {
   _pt_data->log_file = str;
} // set_pt_log_file()


/**
 * This method returns the log file for logging of PT fake deaths and damage.
 * The default is /d/playtesters/log/pain.
 * @return The filename of the log file.
 */
string query_pt_log_file() {
   return _pt_data->log_file;
} // query_pt_log_file()


/**
 * This method enables logging of PT fake deaths.
 */
void enable_death_log() {
   _pt_data->log_death = 1;
} // enable_death_log()


/**
 * This method disables logging of PT fake deaths.
 */
void disable_death_log() {
   _pt_data->log_death = 0;
} // disable_death_log()


/**
 * This method returns the status of the LOG_DEATH flag, i.e. whether
 * PT fake deaths are logged or not.
 * @return Status of LOG_DEATH flag
 */
int query_log_death() {
   return _pt_data->log_death;
} // query_log_death()


/**
 * This method enables logging of PT fake damage.
 */
void enable_damage_log() {
   _pt_data->log_damage = 1;
} // enable_damage_log()


/**
 * This method disables logging of PT fake damage.
 */
void disable_damage_log() {
   _pt_data->log_damage = 0;
} // disable_damage_log()


/**
 * This method returns the status of the LOG_DAMAGE flag, i.e. whether
 * PT fake damage is logged or not.
 * @return Status of LOG_DAMAGE flag
 */
int query_log_damage() {
   return _pt_data->log_damage;
} // query_log_damage()


/** @ignore */
object do_death( object thing ) {
   if( query_pt_protection() || query_in_pt_arena())
      return pt_do_death();
   else
      return ::do_death( thing );
} // do_death()


/** @ignore */
varargs int adjust_hp( int hp, object attacker ) {
   // If protected, adjust "fake" hitpoints
   if( query_pt_protection() ) {
     return pt_adjust_hp( hp, attacker );
   } else {
      // Not protected - hurt them
      return ::adjust_hp( hp, attacker );
   }
} // adjust_hp()


/** @ignore */
int query_hp() {
   if( query_pt_protection() )
      return pt_query_hp();
   else
      return ::query_hp();
   
} // query_hp()


/** @ignore */
mixed *stats() {
   return ::stats() + ({
      ({ "PT hitpoints", _pt_data->hp }),
      ({ "PT protection", ( query_pt_protection() ? "On" : "Off" ) })
   });
} /* stats() */

/** @ignore */
string extra_score() {
   return ::extra_score() +
         "Your playtester protection is "+
         ( query_pt_protection() ? "on" : "off" ) +".\n";
} // extra_score()

int query_in_pt_arena() {
  if (!environment(this_object())) {
    return 0;
  }
  if (environment (this_object())->query_pt_arena()) {
    return 1;
  }
  return 0;
}

int advancement_restriction() {
  if (query_pt_protection()) {
    return 1;
  }
  if (query_in_pt_arena()) {
    return 1;
  }
  return 0;
}

/** @ignore yes */
varargs int adjust_xp(int number, int shared) {
  if ( advancement_restriction() && number > 0) {
    return 0;
  }
  return ::adjust_xp(number, shared);
} /* adjust_xp() */

/** @ignore yes */
void event_move_object(mixed from, mixed to) {
  ::event_move_object (from, to);

  if (query_pt_protection()) {
    if (query_pt_area(from) && !query_pt_area (to)) {
       _pt_data->protection_counter = MAX_COUNTER;
  
      if (_pt_data->turn_off) {
        remove_call_out(_pt_data->turn_off);
      }
      tell_object (this_player(), "%^BOLD%^You are now leaving a PT area.  "
        "Your protection is still on.\n%^RESET%^");
      log_file (PROTECTION_LOG, "%s: %s left a playtester area with protection "
        "still active (%s).\n", ctime(time()), this_player()->query_name(), 
        base_name (environment (this_player())));    
     
      _pt_data->turn_off = call_out("disable_pt_protection", 5 * 60);
    }
    else if (!query_pt_area (from) && query_pt_area (to)) {

      tell_object (this_player(), "%^BOLD%^You are now entering a PT area.  "
        "Your protection will not wear off until you leave or switch it "
        "off.\n%^RESET%^");
      if (_pt_data->turn_off) {
        remove_call_out(_pt_data->turn_off);
        _pt_data->protection_counter = 0;
      }        
    }
  }       
  
} /* event_move_object() */

/** @ignore yes */
int query_player_killer() {
  if (query_in_pt_arena()) {
    return 1;
  }
  return ::query_player_killer();
}

int query_contractable() {
  return ::query_player_killer();
}
