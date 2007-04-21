/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: fuel_handler.c,v 1.1 1998/01/06 05:03:33 ceres Exp $
 * $Log: fuel_handler.c,v $
 * Revision 1.1  1998/01/06 05:03:33  ceres
 * Initial revision
 * 
*/
#include <fuel_handler.h>

object *burners;

void create() {
   seteuid( (string)"/secure/master"->
           creator_file( file_name( this_object() ) ) );
   burners = ({ });
   call_out( "consume_fuel", FUEL_TIME );
} /* create() */

/**
 * Adds a burner into the burner array.  The burner has the
 * function concume_fuelk called on it every fuel interval.
 * @param thing the thing which is burning fuel
 */
void add_burner( object thing ) {
   burners += ({ thing });
} /* add_burner() */

/** @ignore yes */
void consume_fuel() {
   burners -= ({ 0 }); /* remove dest'ed consumers */
   catch( burners->consume_fuel() );
   call_out( "consume_fuel", FUEL_TIME );
} /* consume_fuel() */

/**
 * Remove a burner from the burner list.
 * @param thing the burner to remove
 */
void remove_burner( object thing ) {
   burners -= ({ thing });
} /*remove_burner() */

/**
 * Returns the complete list of the burner array.
 * @return an array containing all the burners
 */
object *query_burners() {
   return burners - ({ 0 });
} /* query_burners() */

/** @ignore yes */
mixed *stats() {
  return ({
    ({ "burners" , sizeof( burners ) }),
    ({ "next event", find_call_out( "consume_fuel" ) }),
   });
} /* stats() */

