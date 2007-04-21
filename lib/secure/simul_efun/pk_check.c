/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: pk_check.c,v 1.17 2003/05/15 21:30:23 ceres Exp $
 */
/**
 * Contains some simul_efuns.
 * @author Pinkfish
 */
#include <player.h>
#include <login.h>
#include <player_handler.h>

#define PLAYTESTING

/**
 * This method checks to see if both of the objects are able to do a
 * pk action on each other.  This will return 1 if they cannot
 * complete the action, ie: either of them are not player killers.
 * It will take an object or a string as the first parameter and attempt
 * to check for pkness, if the off_line flag is set it will even check for
 * players that are not even logged on.
 * <p>
 * If an object is passed into this method, it is first checked with the
 * method query_owner_pk_check(), if this returns a non-zero value it is
 * used as the person who owns the object.  This allows objects to be
 * pk checked when they are not players.
 * @param thing1 first object to check
 * @param thing2 second object to check
 * @param off_line allow the check to occur if they are not logged on
 * @return 1 if they cannot attack each other
 */
int pk_check( mixed thing1, mixed thing2, int off_line ) {
   int pk1;
   int pk2;
   object ob;

   //If there is only one creator or test character involved, fail always.
   //ie: When there are 0, or 2 go right ahead! -- Terano [Approved by Wodan!]
   if(objectp(thing1) && objectp(thing2) &&
      interactive(thing1) && interactive(thing2) &&
      sizeof( filter( ({ thing1, thing2 }), 
                      (: $1->query_creator() || 
                       $1->query_property("test character") :) )) == 1) {
     this_object()->debug_printf( "PK check between %s and %s failed!\n", 
                                  thing1->query_name(), 
                                  thing2->query_name() );
     return 1;
   }

   //
   // Check to see if this some sort of object that has a pk owner.
   //
   if (objectp(thing1) &&
       !interactive(thing1) &&
       thing1->query_owner_pk_check()) {
       thing1 = thing1->query_owner_pk_check();
   }

   /* Get the pk value for thing1. */
   if ( !objectp( thing1 ) ) {
      ob = this_object()->find_player(thing1);
      if (ob) {
         thing1 = ob;
      }
   }
   if (objectp(thing1) && userp(thing1)) {
      pk1 = thing1->query_player_killer() &&
            interactive(thing1);
   } else if (stringp(thing1)) {
      if (off_line && stringp(thing1)) {
         if (PLAYER_HANDLER->test_user(thing1)) {
           pk1 = PLAYER_HANDLER->test_player_killer(thing1);
         } else {
            return 0;
         }
      } else {
         return 0;
      }
   } else {
      return 0;
   }

   //
   // Check to see if this some sort of object that has a pk owner.
   //
   if (objectp(thing2) &&
       !interactive(thing2) &&
       thing2->query_owner_pk_check()) {
       thing2 = thing2->query_owner_pk_check();
   }

   /* Get the pk values for thign2. */
   if ( !objectp( thing2 ) ) {
      ob = this_object()->find_player(thing2);
      if (ob) {
         thing2 = ob;
      }
   }
   if (objectp(thing2) && userp(thing2)) {
      pk2 = thing2->query_player_killer() &&
            interactive(thing2);
   } else if (stringp(thing2)) {
      if (off_line && stringp(thing2)) {
         if (PLAYER_HANDLER->test_user(thing2)) {
            pk2 = PLAYER_HANDLER->test_player_killer(thing2);
         } else {
            return 0;
         }
      } else {
         return 0;
      }
   } else {
      return 0;
   }

   /* Do the actual checks. */
   return !pk1 || !pk2;
} /* pk_check() */

/**
 * This method checks to see if both of the objects are able to assist each
 * other. This will return 1 if they cannot complete the action and 0
 * They are not able to assist each other if one of them is not PK and
 * the other is and the PK is in combat with other players.
 * <p>
 * If an object is passed into this method, it is first checked with the
 * method query_owner_pk_check(), if this returns a non-zero value it is
 * used as the person who owns the object.  This allows objects to be
 * pk checked when they are not players.
 * @param assister thing doing the assisting
 * @param assistee thing being assisted
 * @return 1 if they cannot attack each other
 */
int pk_assist(mixed assister, mixed assistee, int off_line ) {
  object ob;
  
  if(objectp(assistee) && !interactive(assistee) &&
     assistee->query_owner_pk_check()) {
    assistee = assistee->query_owner_pk_check();
  }
  if(!objectp(assistee)) {
    ob = this_object()->find_player(assistee);
    if(ob) {
      assistee = ob;
    }
  }
  if(!objectp(assistee) || !assistee->query_player_killer())
    return 0;
  
  if(objectp(assister) &&
     !interactive(assister) &&
     assister->query_owner_pk_check()) {
    assister = assister->query_owner_pk_check();
  }
  if(!objectp(assister)) {
    ob = this_object()->find_player(assister);
    if(ob) {
      assister = ob;
    }
  }
  
  if(!objectp(assister) || !userp(assister) || assister->query_player_killer())
    return 0;
  
  // If the assistee is a PK & the assister isn't and the assistee is fighting
  // one or more PKs then they cannot be assisted by the assister.
  return sizeof(filter(assistee->query_attacker_list(),
                       (: $1->query_player_killer() :))) > 0;
}
