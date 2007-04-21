/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: simple_disease.c,v 1.4 2002/05/11 16:11:58 tannah Exp $
 * $Log: simple_disease.c,v $
 * Revision 1.4  2002/05/11 16:11:58  tannah
 * Fixed id to be the actual enum rather than an effect id, which
 * in turn fixed the common colds.
 *
 * Revision 1.3  1999/03/05 07:22:45  ceres
 * Modified only to log during debyug
 *
 * Revision 1.2  1999/02/25 05:20:06  ceres
 * Modified to prevent communication in the void.
 *
 * Revision 1.1  1998/01/06 04:08:27  ceres
 * Initial revision
 * 
*/
/**
 * This is the effect skelton docs.  This effect
 * has a classification of "disease.hypothermia".
 * <p>
 * Describe the arguments in here.
 * <p>
 * This effect has a shadow associated with it.
 * @classification disease.hypothermia
 * @see help::effects
 */
/* Basic file meant to be inherited by all diseases.  -*- LPC -*-
 * Very much a work in progress
 * If you change this file, remember to update
 * /doc/creator/files/std.effects.disease.basic_disease as well
 */
#include <effect.h>
#include <disease.h>
inherit BASIC_DISEASE;

int cure_rate, infection_chance;
string shadow_ob;
function infection_check;

void set_cure_rate( int i ) { cure_rate = i; }
int query_cure_rate() { return cure_rate; }

void set_infection_chance( int i ) { infection_chance = i; }
int query_infection_chance() { return infection_chance; }

void set_infection_check( function f ) { infection_check = f; }

void set_shadow_ob( string str ) { shadow_ob = str; }
/** @ignore yes */
string query_shadow_ob() { return shadow_ob; }

void create() {
   do_setup++;
   ::create();
   do_setup--;
   shadow_ob = 0;
   infection_chance = 0;
   infection_check = 0;
   cure_rate = 0;
   if ( !do_setup )
      this_object()->setup();
} /* create() */

/* the effects stuff starts here */

/** @ignore yes */
int beginning( object person, int arg, int id ) {
  id = person->sid_to_enum( id );
   arg = this_object()->disease_start( person, arg, id );
   if ( !arg ) {
      person->submit_ee( 0, 0, EE_REMOVE );
      return 0;
   }
   person->submit_ee( "disease_tick", ( arg > 60 ) ? 60 : arg, EE_ONCE );
   return arg;
} /* beginning() */

/* when reinfected, get the worse of the two */
/** @ignore yes */
int merge_effect( object person, int old_arg, int new_arg, int id ) {
   if ( old_arg < new_arg ) {
      old_arg = new_arg;
      this_object()->disease_worse( person, old_arg, id );
   }
   return old_arg;
} /* merge_effect() */

/** @ignore yes */
void end( object person, int arg, int id ) {
/** @ignore yes */
   this_object()->disease_end( person, arg, id );
} /* end() */

void disease_tick( object person, int arg, int id ) {
   object *things;
   int i;

   /* this can be removed later */
   if ( arg > 100000 )
      arg = 10000;
   arg -= ( cure_rate*( (int)person->query_con() + 7 ) ) / 20 ;
   person->set_arg_of( (int)person->sid_to_enum( id ), arg );
   if ( arg < 0) {
      person->submit_ee( 0, 0, EE_REMOVE );
      return;
   }
   if (person && environment(person) &&
       file_name(environment(person)) != "/room/void" &&
       file_name(environment(person)) != "/room/rubbish" &&
       infection_chance > 0 &&
       (random( 100 ) < infection_chance) ) {

     if ( functionp( infection_check ) ) {
       things = filter_array( all_inventory( environment( person ) ),
                             infection_check ) - ({ person }); 
     } else {
       things = filter_array( all_inventory( environment( person ) ),
                  /* Something can catch diseases if it's alive, and */
                  (: living( $1 ) &&
                   /* is an NPC that is not unique */
                   ( ( !userp( $1 ) && !$1->query_property( "unique" ) ) ||
                   /* or is a player and is not dead. */
                   userp( $1 ) ) && !$1->query_property( "dead" ) :) )
            - ({ person });
     }

     i = sizeof( things );
     if ( i ) {
       i = random( i );
       things[ i ]->add_effect( file_name( this_object() ), arg );
#ifdef DEBUG
       log_file( "INFECTION", "%s, %s infected with %s by %s\n",
                ctime( time() ),
                things[ i ]->query_name(),
                this_object()->query_name(),
                person->query_name() );
#endif       
     }
   }
   person->submit_ee( "disease_tick", ( arg > 60 ) ? 60 : arg, EE_ONCE );
   /* If it has no heart beat, don't waste processor time in case
    * disease_action would have caused it to take damage and start the heatbeat
    */
   if ( !query_heart_beat( person ) )
      return;
   this_object()->disease_action( person, arg, id );
} /* disease_tick() */

int test_remove( object, int, int, int ) {
#ifdef DEBUG
  log_file( "OLD_CURE", file_name( previous_object() ) +" "+
            file_name( this_object() ) +"\n" );
#endif  
} /* test_remove() */
