/* 
   Inheritable for melting food
   by Carmine, August 2001
*/

/**
 *
 * <BR>
 * This is an inheritable for frozen food which melts, 
 * based on the temperature of the room environment in 
 * which it finds itself. 
 * <P>
 * It performs just as normal food, apart from the following:
 * <P>
 * - It cannot be cured or dried
 * <P>
 * - It does not decay
 * <P>
 * - It cannot be forced to behave as a liquid
 * <P>
 * - When applied or eaten, it will cause a brief decrease
 * in temperature of the user.
 * <P>
 * It will melt into a liquid, which can be set using 
 * set_melt_result().  This is default at water, and can be 
 * queried using query_melt_result().
 * <P>
 * It's melt level can be checked by query_melt_level().  This
 * starts at a value of ten times the original weight of the 
 * meltable food.
 * <P>
 * It's rate of melting adjusts itself regularly, based on the
 * temperature.  This can be checked with query_melt_rate(), and
 * set temporarily with set_melt_rate.
 * <P>
 * The level of melting is shown in the long and short of the object.
 * These are created using query_short_rotten_adjective and 
 * query_long_decay_level, and are based on the percentage of 
 * object left.
 * 
 * @author Carmine
 * 
 */
 

#include <bits_controller.h>
#include <weather.h>

inherit "/obj/food";

#define CHILL -5        /* By how much eating or applying this will cool you */
#define CHILL_TIME 10  /* For how long eating or applying this will cool you */

#define MELT_RATE_DEFAULT 6  /* The default melt rate, initially corresponding
                                to 60 seconds for something of weight 1 to 
                                melt away to nothing in a 20 deg. C 
                                environment */
                                
#define WATER "/obj/food/beverages/water.food"  /* default melt liquid */

/* This is a lookup table which holds the melt rates dependent on the 
   environment's temperature.  It has values for 1 to 50 C, after which 
   the melt rate is 1 (fast).  Below 1, no melt takes place */
#define _LOOKUP_RATE ({ 60, 57, 54, 51, 48, 42, 36, 30, 27, 24, \
                        20, 18, 16, 14, 12, 10,  9,  8,  7,  6, \
                         6,  6,  6,  5,  5,  5,  4,  4,  4,  3, \
                         3,  3,  3,  3,  3,  3,  3,  3,  3,  2, \
                         2,  2,  2,  2,  2,  2,  2,  2,  2,  1  })


/* GLOBAL VARIABLES */
private int _melt_level;   /* this represents the amount left to melt */

private int _original_melt_level;   /* this stores the original melt level 
                                       at setup */
private int _melt_rate;    /* this is the number of seconds between the melt
                              counter counting up by 1 - it is the number of
                              seconds between liquid being produced by the 
                              melting object
                           */
                           
private int _melt_counter; /* this counts up to 10, and decreases the weight
                              of this object by 1 every time it reaches 10, 
                              whereupon it resets to 0 */
                              
private int _callout_id;   /* the id of the melting call out */

private string _melt_result; /* the path of the liquid result of melting */


/* FUNCTION PROTOTYPES */
void create();
void set_melt_result( string result );
string query_melt_result();
void set_melt_rate( int rate );
int query_melt_rate();
int query_melt_level();
private void setup_callout();
protected void do_melt();

/**
 * @ignore yes
 */
void create() {
   do_setup++;
   ::create();
   do_setup--;
   _melt_rate = MELT_RATE_DEFAULT;
   _melt_counter = 0;
   set_melt_result( WATER );
   set_decay_speed( 1 );
   setup_callout();
   if ( !do_setup ) {
      this_object()->setup();
   }
   _melt_level = ( this_object()->query_weight() ) * 10;
   _original_melt_level = _melt_level;
}/* create() */


/**
 * This function sets the liquid that the melting food melts into.
 * The default value for this is water.
 *
 * @param result The path of the liquid generated when this food melts
 *
 * @see query_melt_result()
 */
void set_melt_result( string result ) {
   _melt_result = result;
} /* set_melt_result */


/**
 * This function returns the liquid that the melting food melts into.
 * The default value for this is water.
 *
 * @return The path of the liquid generated when this food melts.
 *
 * @see set_melt_result()
 */
string query_melt_result() {
   return _melt_result;
} /* query_melt_result */


/**
 * This function sets the current melt rate for the liquid.  This will
 * have no ostensible affect on the melting of the liquid, as the
 * melt rate is recalculated regularly.
 *
 * @param rate The new rate of melting
 *
 * @see query_melt_rate()
 *
 */
void set_melt_rate( int rate ) {
   _melt_rate = rate;
} /* set_melt_rate */


/**
 * This function returns the current rate of melting of the frozen food.
 *
 * @return The current melt rate of the food.
 *
 * @see set_melt_rate()
 *
 */
int query_melt_rate() {
   return _melt_rate;
} /* query_melt_rate */


/**
 * This function returns the current melt level of the food.  This value
 * starts at ten times the original weight of the object, and decreases
 * in time, as the food melts.
 *
 * @return The current melt level of the food.
 */
int query_melt_level() {
   return _melt_level;
} /* query_melt_level */


/**
 * @ignore yes
 */
private void setup_callout() {
   if( _callout_id ) {
      remove_call_out( _callout_id );
   }
   
   _callout_id = call_out( "do_melt", _melt_rate );
} /* setup_callout */


/**
 * @ignore yes
 */
protected void do_melt() {
   object liquid;
   object room, ob;
   int environment_temp, current_weight;

   /* finds the final container of this object - ie
      the room that the object is within, whether it's
      nested in many containers or not */   
   ob = environment( this_object() );
   
   if( !ob ) {
      return;
   }
   
   room = ob;
   while( ob = environment( ob ) ) {
      room = ob;
   }

   /* sets environment_temp to the temperature of the room, 
      in degrees centigrade */   
   if( room->query_property( "location" ) == "outside" ) {
      environment_temp = WEATHER->query_temperature( room );
   }
   else if( room->query_name() == "clone_on_demand-store" ) {
      environment_temp = 0; // Don't melt if you're in a shop's inventory
   }
   else {
      environment_temp = 20 + room->query_property( "warmth" );
   }
      
   /* if temperature is less than 0, no melt. */
   if( environment_temp > 1 ) {
      /* setup the melt rate dependent on environment temp: hotter 
         environment means faster melting call out */
      if( environment_temp > 50 ) {
         _melt_rate = 1;
      }
      else {
         _melt_rate = _LOOKUP_RATE[ environment_temp ];
      }
      
      /* decrease melt level counter and increase melt counter */
      _melt_level = _melt_level - 1;
      _melt_counter = _melt_counter + 1;
   
      /* if the counter has reached 10 yet, make some liquid and 
         remove some weight */
      if ( _melt_counter >= 10 ) {
         liquid = clone_object( _melt_result );
         liquid->set_amount( 20 );
         liquid->move( environment( this_object() ) );
      
         current_weight = this_object()->query_weight();
         if( current_weight <= 1 ) {
            this_object()->move( "/room/rubbish" );
         }
         else {
            this_object()->set_weight( current_weight - 1 );
         }
         _melt_counter = 0;
      }   
   }
   
   _callout_id = call_out( "do_melt", _melt_rate );
} /* do_melt */


/**
 * This function returns the adjective appended to the food's
 * short description, describing how melted it is.
 *
 * @return The adjectives added to the food's short describing its
 * melt status.
 * @see query_long_decay_level()
 */
string query_short_rotten_adjective() {
   string ret;
   int bing;
   
   bing = 100 - ( ( 100 * _melt_level ) / _original_melt_level );

   ret = "";

   switch ( bing ) {
   case 0..1:
      break;
   case 2..20:
      ret = "slightly melted " + ret;
      break;
   case 21..45:
      ret = "partially melted " + ret;
      break;
   case 46..55:
      ret = "half melted " + ret;
      break;
   case 56..80:
      ret = "mostly melted " + ret;
      break;
   case 81..100:
      ret = "almost completely melted " + ret;
      break;
   }

   return ret; 
} /* query_short_rotten_adjective */


/**
 * This function provides words for the food's long description, 
 * showing how melted the object is.
 * 
 * @return The words appended to the food's long, showing its melt status.
 *
 * @see query_short_rotten_adjective()
*/
string query_long_decay_level() {
   string ret;
   int bing;
   
   bing = 100 - ( ( 100 * _melt_level ) / _original_melt_level );

   ret = "";

   switch ( bing ) {
   case 0..1:
     if( query_collective() && query_amount() > 1 )
        ret += "They have not yet begun to melt.\n";
     else
        ret += "It has not yet begun to melt.\n";
     break;
   case 2..20:
      if( query_collective() && query_amount() > 1 )
         ret += "They have melted slightly.\n";
      else
         ret += "It has melted slightly.\n";
      break;
   case 21..45:
      if( query_collective() && query_amount() > 1 )
         ret += "They have partially melted away.\n";
      else
         ret += "It has partially melted away.\n";
      break;
   case 46..55:
      if( query_collective() && query_amount() > 1 )
         ret += "They are half melted away.\n";
      else
         ret += "It is half melted away.\n";
      break;
   case 56..80:
      if( query_collective() && query_amount() > 1 )
         ret += "They have mostly melted away.\n";
      else
         ret += "It has mostly melted away.\n";
      break;
   case 81..100:
      if( query_collective() && query_amount() > 1 )
         ret += "They have almost complete melted away.\n";
      else
         ret += "It has almost completely melted away.\n";
      break;
   }

    return ret; 
} /* query_long_decay_level */


/*
   Make the person chilly for a while when they eat
   the food.  Then inherits normal eat stuff.
*/
/**
 * @ignore yes
 */
varargs int do_eat( int no_mess ) {
   this_player()->add_property( "warmth", CHILL, CHILL_TIME );
   return( ::do_eat( no_mess ) );
} /* do_eat() */


/*
   Make the victim chilly for a while when they apply
   the food.  Then inherits normal apply stuff.
*/
/**
 * @ignore yes
 */
int do_apply( object * things ) {
   int i;
   if( i = ::do_apply( things ) ) {
      things[ 0 ]->add_property( "warmth", CHILL, CHILL_TIME );
   }
   
   return i;
} /* do_apply() */

/**
 * @ignore yes
 */
int do_rub( object * things ) {
   int i;
   if( i = ::do_rub( things ) ) {
      things[ 0 ]->add_property( "warmth", CHILL, CHILL_TIME );
   }
   return i;
} /* do_rub() */


/**
 * @ignore yes
 */
mixed *stats()
{
   mixed *args;
   args =::stats() + ({ ({ "melt level", _melt_level }),
                        ({ "melt result", _melt_result }),
                        ({ "original melt level", _original_melt_level }),
                        ({ "melt rate", _melt_rate }),
                        ({ "melt counter", _melt_counter }), 
                        ({ "callout id", _callout_id }) });
   return args;
}                               /* stats() */



/*
   This section covers all the loading of global variables
*/
/**
 * @ignore yes
 */
mapping int_query_static_auto_load() {
   return ([ "::" : ::int_query_static_auto_load(),
             "melt_level" : _melt_level,
             "melt_result" : _melt_result,
             "original melt level" : _original_melt_level,
             "melt rate" : _melt_rate,
             "melt counter" : _melt_counter,
             "callout id" : _callout_id ]);
} /* int_query_static_auto_load() */


/**
 * @ignore yes
 */
mapping query_static_auto_load()
{
   if ( ( base_name( this_object() ) != "/obj/food" )
       && !query_continuous() ) {
      return 0;
   }
   return int_query_static_auto_load();
} /* query_static_auto_load() */


/**
 * @ignore yes
 */
void init_static_arg( mapping map )
{
   if ( !mapp( map ) ) {
      return;
   }
   
   if ( map[ "::" ] ) {
      ::init_static_arg( map[ "::" ] );
   }
   _melt_level = map[ "melt_level" ];
   _melt_result = map[ "melt_result" ];
   _original_melt_level = map[ "original melt level" ];
   _melt_rate = map[ "melt rate" ];
   _melt_counter = map[ "melt counter" ];
   _callout_id = map[ "callout id" ];
} /* init_static_arg() */


/**
 * @ignore yes
 */
mapping query_dynamic_auto_load()
{
   return ([ "::" : ::query_dynamic_auto_load(),
             "melt_level" : _melt_level,
             "melt_result" : _melt_result,
             "original melt level" : _original_melt_level,
             "melt rate" : _melt_rate,
             "melt counter" : _melt_counter,
             "callout id" : _callout_id ]);
} /* query_dynamic_auto_load() */


/**
 * @ignore yes
 */
void init_dynamic_arg( mapping map, object )
{
   if ( !mapp( map ) ) {
      return;
   }
   if ( map[ "::" ] ) {
      ::init_dynamic_arg( map[ "::" ] );
   }
   _melt_level = map[ "melt_level" ];
   _melt_result = map[ "melt_result" ];
   _original_melt_level = map[ "original melt level" ];
   _melt_rate = map[ "melt rate" ];
   _melt_counter = map[ "melt counter" ];
   _callout_id = map[ "callout id" ];
} /* init_dynamic_arg() */





/*
   This is all masked stuff from /obj/food.c which stops
   frozen foods being liquids, from being dried, 
   from decaying, from being splashed.
*/


/*
   Frozen things are not liquids
*/
/**
 * @ignore yes
 */
int query_liquid() {
   return 0;
} /* query_liquid() */

/**
 * @ignore yes
 */
void set_liquid() {
} /* set_liquid() */


/*
   do_decay is called by the bits_controller, and, as the 
   frozen item won't decay, does nothing.
*/
/**
 * @ignore yes
 */
void do_decay() {
   return;
} /* do_decay */


/*
   This frozen food doesn't decay - it melts
*/
/**
 * @ignore yes
 */
int query_decays() {
   return 0;
} /* query_decays() */

/**
 * @ignore yes
 */
int query_decay_speed() {
   return 0;
} /* query_decay_speed() */

/**
 * @ignore yes
 */
int query_decay_level() {
   return 0;
} /* query_decay_level() */

/**
 * @ignore yes
 */
void set_decay_speed( int decay ) {
  return( ::set_decay_speed( 1 ) );
} /* set_decay_speed() */


/*
  You cannot cure frozen food.
*/
/**
 * @ignore yes
 */
int do_cure() {
   return 0;
}

/**
 * @ignore yes
 */
int query_cured() { 
   return 0;
}

/**
 * @ignore yes
 */
int do_dry() {
   return 0;
}

/**
 * @ignore yes
 */
int query_dried() { 
   return 0; 
}


/*
   The frozen food won't be rotten.
*/ 
/**
 * @ignore yes
 */
int query_rotten() { 
   return 0;
}


/*
  Can't splash it.
*/
/**
 * @ignore yes
 */
void set_splashable() {
   return;
}

