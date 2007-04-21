/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: auto_load.c,v 1.22 2003/07/15 13:09:06 taffyd Exp $
 */
/**
 * Handles the automatic loading and saving of objects into the
 * players inventories on log in/out.
 * @author Pinkfish
 * @change Ember 1992 sometime
 * Make it use greco and all the other exciting missing item receipt stuff
 */
#include <move_failures.h>
#include <virtual.h>
#include <player.h>

#define AUTO_STR_LENGTH 3

#define AUTO_LOAD_TYPE 0
#define AUTO_LOAD_NAME 1
#define AUTO_LOAD_DATA 2

/* Stuff for the data elements themselves */
#define AUTO_LOAD_STATIC_ARG  0
#define AUTO_LOAD_DYNAMIC_ARG 1

private mixed auto_load;
private nosave int _no_calls;
private nosave function _finished;

private void auto_clone_alt( object thing, mixed *load_info,
                     object dest, object tell_pl, function move_f, string name);
private object *int_auto_load(int type, string name, mixed *load_info,
                              object dest, object tell_pl, int now,
                              function move_f);
private void move_to_destination(object thing, object dest, object tell_pl,
                                 function move_f);

/** @ignore yes */
string *fragile_auto_str_ob(object ob) {
   /* this function was added by Ember 30-Oct-93 */
   /* it is a copy of auto_str_ob below, with all the catches taken out */
   /* it is intended for the newer style of saving/loading with receipts */
   mixed static_arg;
   mixed dynamic_arg;
   string fname;

   static_arg = ob->query_static_auto_load();
   dynamic_arg = ob->query_dynamic_auto_load();
   if (!static_arg && !dynamic_arg) {
      return ({ });
   }
   if ( sscanf( file_name( ob ), "%s#%*d", fname) == 2 ) {
      return  ({ 1, fname, ({ static_arg, dynamic_arg }) });
   } else {
      return ({ 0, file_name(ob), ({ static_arg, dynamic_arg }) });
   }
} /* fragile_auto_str_ob() */

/**
 * This method figures out the auto load string for the given object.
 *  The returned array contains 3 elements.  The first element is a
 * flag which tells if the object is a clone or the original object.
 * The second element is the name of the file and the third is the array
 * containing the saved information.  The third array contains three
 * elements.  The first is the static auto load information, the
 * second is the dynamic autoload information and the third is the shadow
 * auto load information.  The shadow autoload information is reduant these days
 * effects should be used instead.
 * @param ob the object to get the auto load information for
 * @return the array containing the auto load information
 * @see help::effects
 */
private string *auto_str_ob(object ob) {
   string static_arg;
   string dynamic_arg;
   string catch_static;
   string catch_dynamic;
   string fname;
   int value;
   string ob_name;
   mixed* tmp;

   catch_static = catch(static_arg = (string)ob->query_static_auto_load());
   catch_dynamic = catch(dynamic_arg = (string)ob->query_dynamic_auto_load());
   if (!static_arg && !dynamic_arg) {
      //
      // create receipts?
      //
      if (catch_static || catch_dynamic) {
         catch(value = ob->query_value());
         ob_name = "unknown object";
         catch(ob_name = ob->short(1));
         catch(PLAYER_RECEIPT->set_object(file_name(ob)));
         PLAYER_RECEIPT->set_obname(ob_name);
         PLAYER_RECEIPT->set_value(value);
         catch(PLAYER_RECEIPT->set_static_data(({ static_arg, dynamic_arg })));
         tmp = fragile_auto_str_ob(find_object("/obj/misc/al_receipt"));
         tmp[0] = 1;
         // This is a bit of a hack.  It could easily break if the receipt
         // inheritance changes.
         if (!undefinedp(tmp[2][1]["::"]["cloned by"])) {
           tmp[2][1]["::"]["cloned by"] = "greco";
         }
      } else {
         tmp = ({ });
      }
      return tmp;
   }

   if ( sscanf( file_name( ob ), "%s#%*d", fname ) == 2 ) {
      return  ({ 1, fname, ({ static_arg, dynamic_arg }) });
   } else {
      return ({ 0, file_name( ob ), ({ static_arg, dynamic_arg }) });
   }
} /* auto_str_ob() */

/**
 * Creates the complete auto load array from the array of objects
 * passed into this object.  It returns an array of elements 
 * as specified in auto_str_ob.  The arrays from auto_str_ob() are
 * added together, so every 3 elemnts in the array is a new
 * autoload object.
 * @param obs the objects to create an autoload string for
 * @param into_array place the results directly iunto the auto_load array
 * @return the auto load array
 * @see auto_str_ob()
 */
string *create_auto_load(object *obs, int into_array) {
  int i;
  string *tmp;
  string *al_tmp;

  if (into_array) {
    auto_load = ({ });
  } else {
    tmp = ({ });
  }
  /*
   * For some reason inventories are regenerated backwards, if we go from the
   *  the bottom of this array, so go from the top.
   */
  for (i = sizeof(obs) - 1; i >= 0; i--) {
    reset_eval_cost();
    if (!objectp(obs[i])) {
       continue;
    }
    if (!catch(al_tmp = auto_str_ob(obs[i]))) {
      if (into_array) {
         auto_load += al_tmp;
      } else {
         tmp += al_tmp;
      }
    }
  }
  return tmp;
} /* create_auto_load() */

/**
 * The new method of handling auto loading of objects.  This determines
 * if the object failed to load and gives the player a recipt if it does
 * not.
 * <p>
 * The finished variable should <b>only</b> be set in outer
 * level calls to this.  If you set it inside containers when they call
 * this it will cause errors.  This will only be called from within
 * the player object itself.
 * @param auto_string the values specifing the objects to auto load
 * @param dest the destination to put the objects in
 * @param tell who to tell about errors
 * @param finished the function to call when the auto loading has completed
 * @see create_auto_load()
 * @see load_auto_load()
 */
void load_auto_load_alt( mixed *auto_string, object dest,
                                   object tell_pl, function finished) {
   int i;

   reset_eval_cost();
   if ( !auto_string || !sizeof( auto_string ) ) {
      //tell_object(this_object(), "%^RED%^Warning!  Auto string is empty!%^RESET%^\n");
      return;
   }

   if ( !pointerp( auto_string ) ) {
      //tell_object(this_object(), "%^RED%^Warning!  Auto string is not an array!%^RESET%^\n");
      return;
   }

   /*
    * This is to try to handle the slight over-weight problems that people
    *  close to capacity may have due to mergers, e.g. money.
    */
   if ( ( dest == this_object() ) &&
        ( tell_pl == this_object() ) &&
        !this_object()->query_no_check() ) {
      this_object()->set_no_check( 1 );
      this_object()->set_max_weight( 100 + this_object()->query_max_weight() );
   }

   _finished = finished;
   reset_eval_cost();
   for ( i = 0; i < sizeof( auto_string ); i += AUTO_STR_LENGTH) {
      _no_calls++;
      call_out( (: int_auto_load :), i / 6,
                                     auto_string[ i + AUTO_LOAD_TYPE ],
                                     auto_string[ i + AUTO_LOAD_NAME ],
                                     auto_string[ i + AUTO_LOAD_DATA ],
                                     dest, tell_pl, 0, (: $1->move($2) :));
   }
} /* load_auto_load_alt() */

/**
 * This method creates all the objects and sets them up now.  This will
 * not move any of the objects anywhere...  Relying on the calling
 * code to handle this.
 * @param auto_string the values specifing the objects to auto load
 * @param dest the destination to put the objects in
 * @param tell who to tell about errors
 * @param finished the function to call when the auto loading has completed
 * @see create_auto_load()
 * @see load_auto_load()
 */
object *load_auto_load_to_array( mixed *auto_string, object tell_pl ) {
   object *obs;
   int i;

   obs = ({ });
   if (!tell_pl) {
      tell_pl = this_player();
   }
   if (!tell_pl) {
      tell_pl = this_object();
   }
   for ( i = 0; i < sizeof( auto_string ); i += AUTO_STR_LENGTH) {
      _no_calls++;
      /* Set a null function so it always appears to succeed. */
      obs += int_auto_load(auto_string[ i + AUTO_LOAD_TYPE ],
                           auto_string[ i + AUTO_LOAD_NAME ],
                           auto_string[ i + AUTO_LOAD_DATA ],
                           0, tell_pl, 1, (: MOVE_OK :));
   }
   return obs - ({ 0 });
} /* load_auto_load_to_array() */

/**
 * This method loads all the objects from the array and places them
 * into the inventory of the specified object.
 * @param auto_string the values specifing the objects to auto load
 * @param dest the destination to put the objects in
 * @param tell who to tell about errors
 * @param move_f the function to call to move the object to it's destination
 * @see create_auto_load()
 * @see load_auto_load()
 */
void load_auto_load_to_inventory(mixed *auto_string, object dest,
                                 object tell_pl, function move_f) {
   int i;

   if (!tell_pl) {
      tell_pl = this_player();
   }
   if (!tell_pl) {
      tell_pl = this_object();
   }
   for ( i = 0; i < sizeof( auto_string ); i += AUTO_STR_LENGTH) {
      _no_calls++;
      int_auto_load(auto_string[ i + AUTO_LOAD_TYPE ],
                    auto_string[ i + AUTO_LOAD_NAME ],
                    auto_string[ i + AUTO_LOAD_DATA ],
                    dest, tell_pl, 0, move_f);
   }
} /* load_auto_load_to_inventory() */

private object create_auto_load_object( string name, 
                                        mixed *load_info,  object dest,
                                        object tell_pl, function move_f) {
   object thing;

   catch(name = (string)CLONER->other_file( name ));
   if ( !catch( thing = (object)CLONER->clone( name ) ) ) {
      if ( thing ) {
         if ( base_name( thing ) == name ) {
            auto_clone_alt(thing, load_info, dest, tell_pl, move_f, name);
         } else {
            move_to_destination(thing, dest, tell_pl, move_f);
            //catch(thing->move( dest ));
         }
      } else {
  tell_object( tell_pl, "%^RED%^Could not clone "+ name +
         ".%^RESET%^\n" );
  thing = clone_object( PLAYER_RECEIPT );
  thing->set_object( name );
  thing->set_static_save( load_info );
  //thing->move( dest );
  move_to_destination(thing, dest, tell_pl, move_f);
      }
   } else {
      tell_object( tell_pl, "%^RED%^Error in loading "+ name +
       ".%^RESET%^\n" );
      thing = clone_object( PLAYER_RECEIPT );
      thing->set_object( name );
      thing->set_static_save( load_info );
      //thing->move( dest );
      move_to_destination(thing, dest, tell_pl, move_f);
   }
   return thing;
} /* create_auto_load_object() */

private object *int_auto_load(int type, string name, mixed *load_info,
                              object dest, object tell_pl, int now,
                              function move_f) {
   object thing;
   object new_thing;
   mixed *stuff;
   int reduce;

   _no_calls--;
   if ( type ) {
      //
      // Do this first so we don't leak receipts.
      //
      thing = create_auto_load_object(name, load_info, dest, tell_pl, move_f);
      if ( name == PLAYER_RECEIPT) {
         /* Try and reload the receipt */
         stuff = thing->query_static_save();
         if (stuff) {
            new_thing = create_auto_load_object(name, load_info, dest,
                                         tell_pl, move_f);
            if (new_thing) {
               thing->dest_me();
               thing = new_thing;
            }
         }
      }
   } else {
      if ( !find_object( name ) ) {
         if ( !catch( load_object( name ) ) ) {
            thing = find_object( name );
            if ( thing ) {
               auto_clone_alt(thing, load_info, dest, tell_pl, move_f, name);
            } else {
               tell_object( tell_pl, "%^RED%^Could not load "+ name +
          ".%^RESET%^\n" );
            }
         } else {
            tell_object( tell_pl, "%^RED%^Error in loading "+ name +
       ".%^RESET%^\n" );
         }
      } else {
         tell_object( tell_pl, "%^RED%^Object "+ name +
          " exists.%^RESET%^\n" );
      }
   }

   /* Check to see if we have finished */
   if (_no_calls == 0 && _finished) {
      /*
       * NB: This has to be run at the end of the setup...  Because a
       * container may attempt to reload some stuff.
       */
      evaluate(_finished, this_object());
      _finished = 0;

      this_object()->set_no_check( 0 );

      /*
       * This is to handle stuffing about with weights so players don't
       * loose stuff due to things like merging objects.
       */
      reduce = (int)this_object()->query_max_weight() -
             (int)this_object()->query_loc_weight();
      if ( reduce > 1 ) {
         if ( reduce > 100 ) {
            reduce = 100;
         }
         this_object()->set_max_weight( (int)this_object()->query_max_weight() -
                                 reduce );
      }
      this_object()->calc_burden();
   }
   if (thing) {
      return ({ thing });
   }
   return ({ });
} /* int_auto_load() */

/** @ignore yes */
private string ident( object thing ) {
   mixed word;

   catch(word = (string)thing->query_short());
   if ( stringp( word ) ) {
      return "\""+ word +"\"";
   }
   return file_name( thing );
} /* ident() */

/**
 * This moves the cloned object into the correct location and
 * loads up the arguments.  The arguments array contains three elements
 * the first is the object being cloned, the second is the auto load
 * string and the third is the destination to move the object too.  This is
 * used by the alternate system of loading objects.
 * @param arg the arg array
 * @see load_auto_load_alt()
 * @see create_auto_load()
 * @see auto_clone()
 */
private void auto_clone_alt( object thing, mixed *load_info,
                     object dest, object tell_pl, function move_f, string name) {
   string ob_path;

   /*
    * The setting up the object was previously done after this...
    * Not sure why?  Moved it to here...  Oh!  I have an idea.  The
    * object which fail might not end up being moved into the right place
    * or somewhere the player can see at all.  Changed the movement stuff
    * to also try and move into the telling players environment if we
    * run out of options.
    *
    * This is a moot problem now however, since with the inventory
    * generation one the contents is generated slowly after a while
    * anyway...
    */
   reset_eval_cost();
   if ( load_info[ AUTO_LOAD_STATIC_ARG ] ) {
      catch( thing->init_static_arg( load_info[ AUTO_LOAD_STATIC_ARG ],
                                     tell_pl ) );
   }
   catch( thing->set_player( tell_pl ) );

   if ( load_info[ AUTO_LOAD_DYNAMIC_ARG ] ) {
      catch( thing->init_dynamic_arg( load_info[ AUTO_LOAD_DYNAMIC_ARG ],
                                      tell_pl ));
   }

   //
   // We check to see if the virtual object exists or not.
   //
   ob_path = thing->query_property(VIRTUAL_NAME_PROP);
   if (ob_path) {
      ob_path = CLONER->other_file( ob_path );
      if (file_size(ob_path) < 0) {
         // Turn it into a receipt.
         thing->dest_me();
         thing = clone_object( PLAYER_RECEIPT );
         thing->set_object( name );
         thing->set_static_save( load_info );
         thing->set_virtobname( ob_path );
      } else {
         thing->add_property(VIRTUAL_NAME_PROP, ob_path);
      }
   }

   /*
    * This gets rid of illegal objects that don't have their own file
    * and gives the player a replacement of some kind
    */
   ob_path = CLONER->illegal_thing(base_name(thing), thing->query_short());
   if(ob_path) {
     thing->dest_me();
     thing = clone_object( ob_path );
   }

   move_to_destination(thing, dest, tell_pl, move_f);
} /* auto_clone_alt() */

private void move_to_destination(object thing, object dest, object tell_pl,
                                 function move_f) {
   object place;
   int ret_val;
   string catch_str;

   /* Move it into its correct location. */
   place = dest;
   ret_val = MOVE_OK - 1;
   while ( place && ret_val != MOVE_OK) {
      catch_str = catch( ret_val =  evaluate(move_f, thing, place ) );
      if(catch_str) {
        tell_object(tell_pl, "%^RED%^Error moving object: " + catch_str + 
        ".%^RESET%^\n");
        ret_val = MOVE_OK - 1;
      }
      
      if (ret_val != MOVE_OK) {
         if ( environment( place ) ) {
            tell_object( tell_pl, "%^RED%^Cannot move "+ ident( thing ) +
       " into "+
       ident( place ) +" (move returned " +
                         catch_str + "); attempting to move it into "+
                  ident( environment( place ) ) +".%^RESET%^\n" );
            place = environment( place );
/*
 * I don't think this case is needed.  I think it just
 * adds confusion to the issue.  Who thinks it just adds confusiong
 * to the issue, was this me? - pf
 */
         } else if ( ( place != environment(tell_pl) ) &&
                     environment( tell_pl ) ) {
            tell_object( tell_pl, "%^RED%^Cannot move "+ ident( thing ) +
       " into "+
       ident( place ) +" (move returned " +
                         ret_val + "); attempting to move it into "
       "environment of " + tell_pl->query_name() +
       ".%^RESET%^\n" );
            place = environment( tell_pl );
         } else  if (place != find_object("/room/broken")) {
            tell_object( tell_pl, "%^RED%^Cannot move "+ ident( thing ) +
       " into "+
       ident( place ) +" (move returned " + ret_val +
                         "); moving it to the room for broken objects "+
                         "--- please ask a creator for help.%^RESET%^\n" );
            load_object("/room/broken");
            place = find_object( "/room/broken" );
         } else {
            // Error!  Error!
            tell_object( tell_pl, "%^RED%^Cannot move "+ ident( thing ) +
       " into "+
       " the room for broken objects "+
       "--- This is a serious errror!  Please tell a "
       "creator immediately.%^RESET%^\n");
            place = 0;
         }
         /* Only use the strange move function for the first move. */
         move_f = (: $1->move($2) :);
      }
   }
} /* auto_clone_alt() */

/**
 * This method tells us if the player is in the inventory regeneration
 * phase still.
 * @return 1 if the inventory is being regenerated, 0 otherwise.
 */
int query_auto_loading() {
   return _no_calls > 0;
} /* query_auto_load_over() */

/**
 * This method retrusn the auto load string which is used to
 * regenerate the players inventory.
 * @return the array of the auto load string
 * @see set_auto_load_string()
 */
protected mixed query_auto_load_string() {
   return auto_load;
} /* query_auto_load_string() */

/**
 * This method sets the current auto load string to the specified value.
 * @see query_auto_load_string()
 */
protected void set_auto_load_string(mixed str) {
   auto_load = str;
} /* set_auto_load_string() */
