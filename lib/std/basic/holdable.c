/**
 * A nice file to allow you to setup an object as being holdable.  To deal
 * with this inheritable you only need to define the method held_this_item
 * if you wish to do anything when the item is held.
 * @author Pinkfish
 * @started Wed Feb  2 15:38:10 PST 2000
 */
#include <move_failures.h>

private int _my_limb;
private nosave int _no_limbs;
private object _holder;

int held_this_item(int held, object holder, mixed arg);
void set_no_limbs(int num);

void create() {
   _no_limbs = 1;
} /* create() */

/** @ignore yes */
mixed query_static_auto_load() {
  return ([ "no limbs" : _no_limbs
          ]);
} /* query_static_auto_load() */

/** @ignore yes */
void init_static_arg( mapping arg ) {
   if (!arg) {
      return ;
   }
   if (arg["no limbs"]) {
      set_no_limbs(arg["no limbs"]);
   }
} /* init_static_arg() */

/** @ignore yes */
mixed query_dynamic_auto_load() {
  return ([ "limb"       : (_holder ? _my_limb : -1),
            "limbs used" : (_holder ?
              sizeof(find_member(this_object(),
                                 _holder->query_holding())) : 0)
          ]);
} /* query_dynamic_auto_load() */

/** @ignore yes */
void init_dynamic_arg( mapping arg, mixed new_arg ) {
   if (!arg) {
      return ;
   }
   if (arg["limb"] != -1) {
      call_out( function( int pos, mapping arg, mixed new_arg ) {
         if ( environment() ) {
            environment()->set_hold(this_object(), pos,
                                    arg["limbs used"]);
         }
         held_this_item(2, environment(), new_arg);
      }, 0, arg["limb"], arg, new_arg);
   }
} /* init_dynamic_arg() */

/**
 * This method is called by the inheritable when the object is change from
 * being held to unheld, or vica versa.  This should be used to turn on 
 * and off light sources and so on.  The method will also be called with 
 * a 2 if the object was held and we are doing an initialisation sequence.
 * @param held 1 if the item is held, 0 if not, 2 if held on init
 * @param holder this is person already holding it when removed, new holder when held
 * @param arg an argument use in the autoloading, passed into init_dynamic_arg
 * @return 1 if the hold/remove was successful
 */
int held_this_item(int held, object holder, mixed arg) {
   return 1;
} /* held_this_item() */

/**
 * This method is called from the living object when we are held or
 * unheld.
 * @param ob the person holding us
 * @param limb the limb we are being held in
 */
int set_holder(object ob, int limb) {

   /* No!! We -unhold- items by setting ob to 0! */ 

   if (ob && environment() != ob) {
      // Only allow us to hold things in our environment.
      return 0;
   }
   this_object()->remove_hide_invis( "concealed" );
   if ( !ob ||
        ob != _holder) {
      if (!held_this_item(0, _holder, 0)) {
         return 0;
      }
   }
   if (ob) {
      if (!held_this_item(1, ob, 0)) {
         return 0;
      }
   }

   _holder = ob;
   _my_limb = limb;
   return 1;
} /* set_holder() */

/**
 * This method returns the current limb the object is being
 * held in.
 * @return the number limb the object is held in
 */
int query_my_limb() { return _my_limb; }

/**
 * This method returns the number of limbs that the object uses.
 * @return the number of limbs the object uses when held
 */
int query_no_limbs() { return _no_limbs; }

/**
 * This method sets the number of limbs the object uses when held.
 * @param num the number of limbs
 */
void set_no_limbs(int num) {
   if (num < 0) {
      return ;
   }
   _no_limbs = num;
} /* set_no_limbs() */

/**
 * This method should be called by the upper level moves.  The result
 * needs to be check, if it is not MOVE_OK then the move should be
 * stopped.
 * @param ob the object to move
 */
int move( mixed ob) {
   if ( ob != _holder && _holder ) {
      if ( !sizeof( _holder->set_unhold( this_object() ) ) ) {
         return MOVE_NO_UNHOLD;
      }
   }
   return MOVE_OK;
} /* move() */

/**
 * This method returns the person who is holding the object.
 * @return the person holding the object
 * @ignore yes
 */
object query_wielded() { return _holder; }

/**
 * This method returns the person who is holding the object.
 * @return the person holding the object
 */
object query_holder() { return _holder; }

/**
 * This method deals with removing ourselves as a holder when dested.
 * @ignore yes
 */
void dest_me() {
   if (_holder) {
      _holder->set_unhold(this_object());
   }
} /* dest_me() */

/** 
 *
 * Attempts to free up enough limbs for this_object to be held.  If
 * items are dropped to make this possible, who is notified.  If 
 * enough limbs are freed, attempts to hold this_object.  If the position
 * is undefined (ie: not specified) then it will try and choose a limb
 * by itself.
 * <p>
 * The return numbers are the limb numbers that were used by holding the
 * item.
 * <p>
 * hold_item: As designed by Tannah!
 * @param who The living object trying to hold this one.
 * @param pos the position to hold the item in
 * @return the an (int *) if successful or ({ }) if not enough limbs are freed
 */
varargs int *hold_item( object who, int pos ) {
   object ob;
   object *held_things;
   object *dropped_things = ({ });
   int num_limbs;

  /* If a particular position is specified, put that position at
   * the front of the queueueue, continue from it to the end, then
   * start at the beginning */

  if( !undefinedp( pos ) && pos != -1 ) {
    /* is the position number valid? */
    if( pos < 0 || pos >= sizeof( who->query_holding() ) )
      return ({});
    /* is there already something in that hand that can't be put down? */
    ob = who->query_holding()[ pos ];
    if( ob ) {
      if( !sizeof( who->set_unhold( ob ) ) )
        return ({});
      else dropped_things += ({ ob });
    }
    /* all is well...continue on */
    held_things = who->query_holding()[pos..] + 
                  who->query_holding()[0..pos-1];
  }

  /* Otherwise use the standard order */
  else {
    held_things = who->query_holding();
    pos = -1;
  }

  if (pos == -1)  num_limbs = this_object()->query_no_limbs();
  else num_limbs = 1;

  tell_creator( who, "FL: %d, NL: %d, ob: %O\n", who->query_free_limbs(),
      num_limbs, this_object() );
  tell_creator( who, "Dropped things: %O\n", dropped_things );      

  /* Try to put down enough things to hold this object */
  for( int i = 0; who->query_free_limbs() < 
      num_limbs && i < sizeof( held_things ); i++ ) {
    tell_creator( who, "freeing limbs\n" );
    ob = held_things[i]; 

    //We stopped holding something
    if( ob && sizeof( who->set_unhold( ob ) ) )  
      dropped_things += ({ ob });
  }
   
  /* Tell the player what he's put down: */
  if ( sizeof( dropped_things ) )
    tell_object( who, "You put down " + query_multiple_short( 
        dropped_things ) + ".\n" );

  /* Can he hold the item now? */
  if( who->query_free_limbs() >= num_limbs ) {
     //Excellent!
     return who->set_hold( this_object(), pos, num_limbs );
  }

   tell_object( who, "You can't free up enough limbs to use "+
       this_object()->a_short() + ".\n" );
   return ({ });  // Nope, he can't.
} /* hold_item() */
