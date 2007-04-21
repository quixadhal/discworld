/* 
* @main
* A nice little effect so that witches who plummet out of the sky
* when their brooms break in midflight will leave a crater in the
* ground where they fall.  
*
* add_effect is called in the crash site; the name of the victim
* is passed to the effect.  The arg of the effect is an array of
* strings, each of which is the name of someone who's landed in
* that particular crash site.
*
* @classification room.crater.broom
* @see help::effects
*/

#include <effect.h>

/** @ignore */
string query_classification() { return "room.crater.broom"; }

/** @ignore */
string *beginning( object room, string name ) {
  
  room->add_item( ({ name + " shaped crater", "crater" }),
      "There is a large crater in the ground, as if " + 
      capitalize( name ) + " happened here recently, from a "
      "very great height indeed." );

  room->add_extra_look( this_object() );

  return ({ name });

} /* beginning() */

/** @ignore */
string *merge_effect( object room, mixed old_names, string name ) {

  // Don't add a new crater if this witch has already left one here.
  if( member_array( name, old_names ) != -1 ) {
    return old_names;
  }

  room->add_item( ({ name + " shaped crater", "crater" }),
      "There is a large crater in the ground, as if " + 
      capitalize( name ) + " happened here recently, "
      "from a very great height indeed." );

  return old_names += ({ name });

} /* merge_effect() */


/** @ignore */
void end( object room, mixed names ) {
  string name;
   
  /* remove the items that were added for each crash */
  foreach( name in names )  {
    room->remove_item( name + " shaped crater" );
  }

  room->remove_extra_look( this_object() );

} /* end() */

/** @ignore */
int query_indefinite() { return 1; }  /* don't end effect */

string extra_look( object room ) {
   int     *enums;          /* Effect numbers */
   string  *shapes;         /* Who made the crater */

   /* Find out the effect number we have */
   enums = room->effects_matching( query_classification() );

   /* Make sure we have something to show them... */
   if( !sizeof( enums ) )
     return "";

   /* Check the args.  We should only have one effect, since we
    * take care of merging */
   shapes = room->arg_of( enums[0] );
   if( !sizeof( shapes ) )
     return "";

   if( sizeof( shapes ) == 1 )
     return "There is " + add_a( capitalize( shapes[0] ) ) +
         " shaped crater in the ground.\n";
              
   else {
     return "There are " + query_multiple_short( map( shapes, (: capitalize(
                $1 ) :) ) ) + " shaped craters in the ground.\n";
   }
} /* extra_look() */
