/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: label.c,v 1.9 2003/04/10 20:59:35 ceres Exp $
 * $Log: label.c,v $
 * Revision 1.9  2003/04/10 20:59:35  ceres
 * Tried a second time
 *
 * Revision 1.7  1999/05/25 20:39:40  pinkfish
 * Fix up the label code to correctly work really this time. :)
 *
 * Revision 1.6  1999/05/24 20:59:04  pinkfish
 * Fix up the label effect.
 *
 * Revision 1.5  1999/05/16 03:50:47  pinkfish
 * Reformat it a little.
 *
 * Revision 1.4  1998/07/28 13:38:00  wodan
 * fixed it for real now, i hope.
 *
 * Revision 1.3  1998/07/24 14:17:57  wodan
 * fixed a memory leak.
 *
 * Revision 1.2  1998/01/11 17:03:41  pinkfish
 * fixed_up_the_extra_look_stuff,_tomake_them_ignored.
 *
 * Revision 1.1  1998/01/06 04:19:27  ceres
 * Initial revision
 * 
*/
/**
 * This is the effect skelton docs.  This effect
 * has a classification of "object.label".
 * <p>
 * The arguement is the storage container.  It is either the object itself
 * (when the player is running) or the mapping of the dynamic loading
 * of the container (when the player logs on/off).
 * @classification object.label
 * @see help::effects
 */
#include <effect.h>

/** @ignore yes */
int query_indefinite() { return 1; }

/**
 * Create the storage container for the labels.
 * @return the storage container
 */
object create_store(mapping map,
                    object player) {
   object store;

   store = clone_object( "/std/container" );
   if ( objectp( player ) ) {
      store->set_player( player );
   }
   if ( mapp( map ) ) {
      store->init_dynamic_arg( map );
   }
   return store;
} /* create_store() */

/** @ignore yes */
string query_classification() { return "object.label"; }

/** @ignore yes */
mixed* beginning( object thing, object label, int id ) {
   object store;

   store = clone_object( "/std/container" );
   label->move( store );
   thing->add_extra_look( file_name(this_object()) );
   return ({ store, store->query_dynamic_auto_load() });
} /* beginning() */

/** @ignore yes */
mixed* merge_effect( object thing, mixed* store, object label, int id ) {
   if (!store) {
      store = ({ 0, ([ ]) });
   }

   if (mapp(store)) {
      store = ({ 0, store });
   }

   if ( !store[0] ) {
      store[0] = create_store(store[1], environment(thing));
   }
   label->move( store[0] );
   store[1] = store[0]->query_dynamic_auto_load();
   return store;
} /* merge_effect() */

/** @ignore yes */
void end( object thing, mixed* store, int id ) {
  int i;
  
  if (arrayp(store)) {
    for(i=0; i<sizeof(store); i++)
      if(objectp(store[i]))
         store[i]->dest_me();
  } else if(objectp(store))
    store->dest_me();
  
   thing->remove_extra_look( this_object() );
} /* end() */

/** @ignore yes */
mixed* restart( object thing, mixed stuff, int id ) {
   object player;
   object store;
   mapping map;
   int enum;

   if (mapp(stuff)) {
      map = stuff;
   } else if (stuff) {
      map = stuff[1];
   } else {
      map = ([ ]);
   }
   player = environment( thing );
   thing->add_extra_look( file_name(this_object()) );
   store = create_store(map, environment(thing));
   enum = thing->sid_to_enum(id);
   thing->set_arg_of(enum, ({ store, map }));
} /* restart() */

/** @ignore yes */
string extra_look( object thing ) {
   int *enums;
   object store;
   object *labels;
   mixed *bing;

   enums = (int *)thing->effects_matching( "object.label" );
   if ( !sizeof( enums ) ) {
      return "";
   }
   bing = thing->arg_of( enums[ 0 ] );
   if (pointerp(bing)) {
      store = bing[0];
      if (!store) {
         store = create_store(bing[1], environment(thing));
         thing->set_arg_of(enums[0], ({ store, bing[1] }));
      }
   }
   //store = (object)thing->arg_of( enums[ 0 ] )[0];
   if ( !objectp( store ) ) {
      return "";
   }
   labels = (object *)store->find_inv_match( "" );
   if ( !sizeof( labels ) ) {
      return "";
   }
   return capitalize( (string)thing->query_pronoun() ) +" has "+
         query_multiple_short( labels ) +" stuck on "+
         (string)thing->query_objective() +".\n";
} /* extra_look() */
