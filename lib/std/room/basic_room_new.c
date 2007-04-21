/*  RCS stuff...  -*- LPC -*-
 * $Locker: pinkfish $
 *
 * $Id: basic_room.c,v 1.10 2001/01/14 03:28:59 ceres Exp pinkfish $
 *
 */

/**
 * The standard room inheritable.  This contains all the stuff needed to
 * construct a standard room.
 * @author Pinkfish
 * @see /std/room/outside.c
 */

#include <armoury.h>
#include <dirs.h>
#include <door.h>
#include <move_failures.h>
#include <room.h>
#include <position.h>
#include <situations.h>
#include <twilight.h>
#include <nroff.h>

inherit "/std/basic/cute_look";
inherit "/std/basic/desc";
inherit "/std/basic/extra_look";
inherit "/std/basic/light";
inherit "/std/basic/property";
inherit "/std/basic/export_inventory";
inherit "/std/basic/help_files";

nosave int do_setup;
nosave int *co_ord;
nosave int co_ord_calculated;
nosave string long_exit;
nosave string short_exit;
nosave string theft_handler;
nosave string *aliases;
nosave string *exits;

nosave object item;
nosave object chatter;
nosave object sitchanger;
nosave object effects;
nosave object linker;
nosave object terrain;
nosave object wall; 
nosave object *hidden_objects;
private nosave object* _use_internal_objects;
nosave mapping door_control;
nosave mixed *dest_other;
nosave int enchant_time;
nosave int background_enchant;
nosave float dynamic_enchant;
private nosave int last_visited;

// This is the day/night stuff.
nosave string *variablelongs;
nosave mixed variablechats;
nosave mixed variableitems;
nosave int is_day = -1;

varargs int add_item( mixed shorts, mixed desc, int no_plural );
int remove_item( string word );
varargs void room_chat( mixed *args, object chatobj );
void set_not_replaceable(int replace);
int query_not_replaceable();
void set_night_long( string str );

protected void create() {
   object *npcs;
   object virt_room;
   string *inhs;

   desc::create();
   extra_look::create();
   property::create();
   export_inventory::create();
   light::create();
   set_can_export_inventory();
   set_ignore_livings_in_inventory( 0 );
   aliases = ({ });
   exits = ({ });
   hidden_objects = ({ });
   _use_internal_objects = ({ });
   door_control = ([ ]);
   dest_other = ({ });
   seteuid( (string)"/secure/master"->creator_file( file_name(
           this_object() ) ) );
   add_property( "location", "inside" );
   add_property( "here", "on the floor" );
   if ( !do_setup ) {
      this_object()->setup();
      this_object()->reset();
   }
   if ( find_object( "/obj/handlers/map" ) ) {
      catch( "/obj/handlers/map"->check_map( file_name( this_object() ) ) );
   }
   if( (virt_room = find_object("/room/virtual") ) ){
     npcs = ({}) + all_inventory(virt_room);
     catch(npcs->real_room(file_name()));
   }

   // Check to see if the room is replaceable...
   inhs = inherit_list(this_object());
   if (sizeof(inhs) == 1) {
      if (replaceable(this_object(), ({ "setup" }))) {
         // Do it after a delay so that setup() has definately finished.
         call_out(function(string fname) {
           if (!query_not_replaceable()) {
             replace_program(fname);
           }
         }, 0, inhs[0]);
      }
   }
} /* create() */

/**
 * Returns 1 to indicate that this object is a room.
 * @return 1 to indicate that this is a room
 */
int query_is_room()
{
  return 1;
}

/**
 * Returns the current enchantment level of the room.  The enchanment
 * level controls things like what happens when you flip coins and
 * some special messages which give wizards some idea about magic
 * levels.
 * @return the current enchantment
 * @see set_enchant()
 */
int query_enchant() {
  int enchant_level =  to_int( floor( 0.5 + dynamic_enchant *
                        exp( -0.693 *
                             ( time() - enchant_time ) /
                             ENCHANT_HALF)) +
                 background_enchant );
  if ( enchant_level > 5000 ) {
    return 5000;
  }

  return enchant_level;
} /* query_enchant() */

/**
 * Sets the current enchantment level of the room.  The enchanment
 * level controls things like what happens when you flip coins and
 * some special messages which give wizards some idea about magic
 * levels.
 * When called from the room itself, it sets a background level of 
 * enchantment that don't decay, when called from another object it
 * sets the current enchantment which then decays towards the background 
 * level.
 * @param number the new enchantment level to set
 * @see query_enchant()
 */
int set_enchant(int number) {
  if (number < 0) {
    number = 0;
  }

  if (previous_object()==this_object()) { 
    /* setting up the background enchantment */
    background_enchant = (float) number;
    dynamic_enchant = 0.0;
    enchant_time = time();
  } else {
    /* by design, this can result in negative dynamic enchantment
     * it is definitely possible to suck out all magic in an area,
     * but it'll seep back in if the background level is higher.
     */
    dynamic_enchant = number - background_enchant;
    enchant_time=time();
  }
  return number;
} /* set_enchant() */

/**
 * Adds number to the current enchantment level of the room.
 * The enchanment level controls things like what happens when you
 * flip coins and some special messages which give wizards some 
 * idea about magic levels.
 * @param number the new enchantment level to set
 * @see query_enchant()
 */
int add_enchant( int number ) {
  dynamic_enchant = dynamic_enchant * exp( -0.693 * 
                                           ( time() - enchant_time ) /
                                           ENCHANT_HALF ) + number;
  enchant_time = time();
  return floor( 0.5 + dynamic_enchant ) + background_enchant;
} /* add_enchant() */
  
/**
 * This method sets the background enchantment of the room.
 * @return the theft handler of the room
 * @see set_enchant()
 */
void set_background_enchant( int number ) { 
  background_enchant = number;
} /* set_background_enchant() */

/**
 * This method sets the current dynamic enchantment of the room.
 * @return the theft handler of the room
 * @see set_enchant()
 */
void set_dynamic_enchant( float number ) { 
  dynamic_enchant = number;
  enchant_time = time();
} /* set_dynamic_enchant() */

/**
 * This method returns the background enchantment of the room.
 * @return the theft handler of the room
 * @see query_enchant()
 */
int query_background_enchant() { 
  return background_enchant;
} /* query_background_enchant() */

/**
 * This method returns the current dynamic enchantment of the room.
 * @return the theft handler of the room
 * @see query_enchant()
 */
float query_dynamic_enchant() { 
  dynamic_enchant = dynamic_enchant * exp( -0.693 * 
                                           ( time() - enchant_time ) /
                                           ENCHANT_HALF );
  enchant_time = time();
  return dynamic_enchant;
} /* set_dynamic_enchant() */

/**
 * Returns the current co-ordinates of the room.  The co-ordinates are
 * 3d, ({ x, y, z }).  So an array with three elements.
 * @return the current co-ordinates
 * @see set_co_ord()
 */
int *query_co_ord() {
  if ( pointerp( co_ord ) ) {
    return copy(co_ord);
  }
  return 0;
} /* query_co_ord() */

/**
 * Sets the current co-ordinates of the room.  The co-ordinates are
 * 3d, ({ x, y, z }).  So an array with three elements.
 * @param new_co_ord the new co-ordinates for the room.
 * @see query_co_ord()
 * @see query_co_ord_calculated()
 */
void set_co_ord( int *new_co_ord ) {
  if ( !pointerp( new_co_ord ) ) {
    write( "Warning: Co-ordinate must be an array.\n" );
    return;
  }
  if ( sizeof( new_co_ord ) != 3 ) {
      write( "Warning: The co-ordinate must have three elements.\n" );
      return;
  }
  co_ord = new_co_ord;
  if ( previous_object() == this_object() ) {
    co_ord_calculated = 0;
  } else {
    co_ord_calculated = 1;
  }
} /* set_co_ord() */

/**
 * This tells us if the co-ordinates were set or if they were calculated.
 * If they were set with set_co_ord then the value of this will be 0
 * otherwise it will be 1.
 * @return 1 if it is calculated, 0 if it is not
 * @see query_co_ord()
 * @see set_co_ord()
 */
int query_co_ord_calculated() { return co_ord_calculated; }

/**
 * This returns the long exit string.  This is calculated when it is
 * first needed by the calc_long_exit function.
 * @return the long exit string
 * @see calc_long_exit()
 * @see long()
 */
string query_long_exit() { return long_exit; }

/**
 * This method creates the long exit description used in the room long
 * descriptions.
 * @see query_long_exit()
 */
void calc_long_exit() {
  int i, add;
  string *words;
  mixed tmp;
  
  words = ({ });
  for ( i = 0; i < sizeof( dest_other ); i += 2 ) {
    tmp = dest_other[ i + 1 ][ ROOM_OBV ];
    if ( !tmp ) {
      continue;
    }
    if ( intp( tmp ) && tmp ) {
      add = 1;
    }
    if ( stringp( tmp ) ) {
      add = (int)call_other( this_object(), tmp, dest_other[ i ] );
    }
    if ( pointerp( tmp ) ) {
      add = (int)call_other( tmp[ 0 ], tmp[ 1 ], dest_other[ i ] );
    }
    if ( add ) {
      if(dest_other[i+1][ROOM_REL]) {
        words += ({ "$R$-"+ dest_other[ i ] +"$R$" });
      } else {
        words += ({ dest_other[i] });
      }
    }
  }
  switch ( sizeof( words ) ) {
  case 0 :
    long_exit = "There are no obvious exits.";
    break;
  case 1 :
    long_exit = "There is one obvious exit: "+ words[ 0 ];
    break;
  default :
    long_exit = "There are "+ query_num( sizeof( words ), 0 ) +
      " obvious exits: "+ query_multiple_short( words ) +
      ".";
  }
} /* calc_long_exit() */

/**
 * This method returns the current theft handler for the room.
 * @return the theft handler of the room
 * @see set_theft_handler()
 */
string query_theft_handler() { return theft_handler; }

/**
 * This method sets the current theft handler for the room.
 * @param word the new theft handler for the room
 * @see query_theft_handler()
 */
void set_theft_handler( string word ) { theft_handler = word; }

/**
 * This method returns the current exit aliases for the room.
 * @return the exit aliases of the room
 * @see add_alias()
 * @see remove_alias()
 */
string *query_aliases() { return copy( aliases ); }

/**
 * This method adds an exit alias to the room.
 * Aliases are convenient extra forms that can be attached to certain
 * exits. In the above functions, the variable names is either a string
 * or an array of strings and is, respectively, the alias or aliases
 * for the direction passed in word. Since, sometimes, the same alias
 * could be used for more than one exit, remove_alias() requires both
 * alias(es) and direction in order to remove the correct alias(es). 
 * @param names the exit names to alias
 * @param word the name to alias them too
 * @see query_aliases()
 * @see remove_alias()
 * @example
 * add_exit( "north", PATH +"dining_hall", "corridor" );
 * add_alias( ({ "enter", "enter hall", "enter dining hall" }), "north" );
 * @example
 * add_exit( "board carriage", PATH +"carriage", "door" );
 * add_alias( "board", "board carriage" );
 */
void add_alias( mixed names, string word ) {
  string name;

  if ( !aliases ) {
    aliases = ({ });
  }
  if ( pointerp( names ) ) {
    foreach ( name in names ) {
      add_alias( name, word );
    }
    return;
  }
  aliases += ({ word, names });
  if ( find_call_out( "calc_exits" ) == -1 ) {
    call_out( "calc_exits", 1 );
  }
} /* add_alias() */

/**
 * This method removes the exit aliases from the room.
 * Aliases are convenient extra forms that can be attached to certain
 * exits. In the above functions, the variable names is either a string
 * or an array of strings and is, respectively, the alias or aliases
 * for the direction passed in word. Since, sometimes, the same alias
 * could be used for more than one exit, remove_alias() requires both
 * alias(es) and direction in order to remove the correct alias(es). 
 * @param names the names to remove
 * @param word what they were aliased to
 * @see add_alias()
 * @see query_aliases()
 * @example
 * remove_exit( "board carriage" );
 * remove_alias( "board", "board carriage" );
 */
void remove_alias( mixed names, string word ) {
  int i;
  string name;

  if ( !aliases ) {
    return;
  }
  if ( pointerp( names ) ) {
    foreach ( name in names ) {
      remove_alias( name, word );
    }
    return;
  }
  for ( i = sizeof( aliases ) - 2; i >= -1; i -= 2 ) {
    if ( ( aliases[ i ] == word ) && ( aliases[ i + 1 ] == names ) ) {
      aliases = delete( aliases, i, 2 );  
    }
  }
} /* remove_alias() */

/**
 * This returns the current array of exits.
 * @return the exits array
 * @see add_exit()
 * @see remove_exit()
 * @see modify_exit()
 */
string *query_exits() { return copy(exits); }

/**
 * This method removes all the current exits in the room.
 * @see add_exit()
 * @see remove_exit()
 * @see modify_exit()
 */
void reset_exits() { exits = ({ }); }

/**
 * This method returns the current item object.
 * @return the current item object
 * @see add_item()
 */
object query_item() { return item; }

/**
 * This method returns the current chatter object.
 * @return the chatter object
 * @see add_room_chat()
 */
object query_chatter() { return chatter; }

/**
 * This method returns the current situation changer object.
 * @return the situation changer object
 * @see add_situation()
 * @see automate_situation()
 * @see change_situation
 */
object query_situation_changer() { return sitchanger; }

/**
 * This method returns the current effects object.
 * @return the effects object
 * @see add_effect()
 */
object query_effects() { return effects; }

/**
 * This method returns the current linker object.
 * @return the linker object
 */
object query_linker() { return linker; }

/**
 * This method returns the current terrain object.
 * @return the terrain object
 * @see add_room_chat()
 */
object query_terrain() { return terrain; }

/**
 * This method returns the current wall object.
 * @return the wall object
 * @see add_room_chat()
 */
object query_wall() { return wall; }

/**
 * This returns the current array of hidden objects.  The hidden objects
 * are used to allow things to not actually be in the room description
 * but be able to be manipulated by commands.
 * @see add_hidden_object()
 * @see remove_hidden_object()
 * @return the array of hidden objects
 */
object *query_hidden_objects() { return hidden_objects + ({ }); }

/**
 * This puts a hidden object into a room. A hidden object is an object that
 * exists in the room as far as all the find_match calls go. So, for look at's
 * and so on, but does not actually exist in the room so it does not show up
 * in the inventory when the player does a look. This is the method used for
 * putting signs and doors into rooms, that actually have shorts and you can
 * do things to, but do not show up in the inventory. The function init is also
 * called on these objects when init is called in the room. The only thing you
 * cannot put in your init function is an add_action. You can however define
 * up bunches of add_commands...
 * <p>
 * If this sounds complicated. Think of it as an object that IS in the room, but
 * you cannot see it. 
 * <p>
 * A word of warning here, the init() function will *not* be called on all
 * the players when the object is added as hidden.  This means that the
 * commands on it will not be available until the player re-enters the room.
 * You could get around this by moving everyone out of the room and
 * then back in again.
 * @example
 * #include <room.h>
 * sign = clone_object(PATH + SIGN);
 * add_hidden_object(sign);
 * @example
 * // Add a hidden object that has actions we want players to be able to
 * // use.
 * add_hidden_object(fluffy_container);
 * players = filter(all_inventory(), (: living($1) :));
 * players->move(ROOM_VOID);
 * // This forces init() to be recalled.  (This is realtivatively icky
 * // way of doing it, but the driver does not give us many alternatives).
 * players->move(this_object());
 * @see query_hidden_object()
 * @see remove_hidden_object()
 * @param thing the hidden object to add
 * @return 1 if successful, 0 on a failure
 */
int add_hidden_object( object thing ) {
  if ( member_array( thing, hidden_objects ) != -1 ) {
    return 0;
  }
  hidden_objects += ({ thing });
  return 1;
} /* add_hidden_object() */

/**
 * This method removes a hidden object.
 * @param thing the hidden object to remove
 * @return 1 on success, 0 on failure
 * @see add_hidden_object()
 * @see query_hidden_objects()
 */
int remove_hidden_object( object thing ) {
  int i;

  i = member_array( thing, hidden_objects );
  if ( i == -1 ) {
    return 0;
  }
  hidden_objects = hidden_objects[0..i - 1] + hidden_objects[i + 1..];
  return 1;
} /* remove_hidden_object() */

/**
 * This method adds an object whose interior bits want to be able to export
 * commands.  You can use this for tables an so on, so that stuff on a table
 * can still be used.
 * @param thing the thing whose inventory bits are to be exported
 */
void add_use_internal_object(object thing) {
   _use_internal_objects |= ({ thing });
} /* add_use_internal_objects() */

/**
 * This method removes an object whose interor bits want to export.
 * @param thing the object to remove
 */
void remove_use_internal_object(object thing) {
   _use_internal_objects -= ({ thing });
} /* remove_use_internal_object() */

/**
 * This method returns all the current use internal objects available.
 * @return the list of use internal objects here
 */
object* query_use_internal_objects() {
   return _use_internal_objects;
} /* query_use_internal_objects() */

/**
 * This returns the information about the door in the specified direction.
 * @param direc the direction to query the door in
 * @return the door control information
 * @see modify_exit()
 */
varargs mixed query_door_control( string direc ) {
   if ( !stringp( direc ) ) {
      return copy( door_control );
   }
   return door_control[ direc ];
} /* query_door_control() */

/**
 * This returns information about the exits in the room.  This is the
 * information set by modify_exit().  The values from this are probably
 * not very useful for normal coding.
 * @see modify_exit()
 * @see query_dest_dir()
 */
varargs mixed *query_dest_other( string direc ) {
  int i;

  if ( !stringp( direc ) ) {
    return copy( dest_other );
  }
  i = member_array( direc, dest_other );
  if ( i == -1 ) {
    return 0;
  }
  return copy( dest_other[ i + 1 ] );
} /* query_dest_other() */

/**
 * Returns an array containing just the destinations and directions used to
 * get there. This is useful for monster or whatever that you want to scan a
 * room for exits to leave out of. The array is of the format. ({ direction1,
 * destination1, direction2, destination2, ... })
 * <p>
 * The thing passed in is used as the basis for the relative directions
 * if it is an object.  If it is not an object then this is ignored
 * altogether.
 * @see query_dest_other()
 * @see add_exit()
 * @param thing used to get the relative directions according to thing
 * @return the array of direction, destination pairs
 */
varargs string *query_dest_dir( object thing ) {
  int i;
  string *ret;
  
  ret = ({ });
  for ( i = sizeof( dest_other ) - 2; i > -1; i -= 2 ) {
    if ( !dest_other[ i + 1 ][ ROOM_REL ] || !objectp( thing ) ) {
      ret += ({ dest_other[ i ], dest_other[ i + 1 ][ ROOM_DEST ] });
    } else {
      ret += ({ (string)thing->find_rel( dest_other[ i ] ),
                  dest_other[ i + 1 ][ ROOM_DEST ] });
    }
  }
  return ret;
} /* query_dest_dir() */

/**
 * This method just returns all the directions available to leave from
 * the room.
 * <p>
 * The thing passed in is used as the basis for the relative directions
 * if it is an object.  If it is not an object then this is ignored
 * altogether.
 * <p>
 * <b>Strawberries</b>
 * <p>
 * Starting from a above,<br>
 * Working slowly down under.<br>
 * Sliding up the sides<br>
 * Eating a meal, fresh cream and syrup.<br>
 * <p>
 * Round and round, and round again<br>
 * Grining micheviously<br>
 * One tongue at play<br>
 * Firm and hard, fresh strawberries today.
 *
 * @see query_dest_other()
 * @see add_exit()
 * @param thing used to get the relative directions according to thing
 * @return the array of directions
 */
varargs string *query_direc( object thing ) {
  int i;
  string *ret;
  
  ret = ({ });
  for ( i = sizeof( dest_other ) - 2; i > -1; i -= 2 ) {
    if ( !dest_other[ i + 1 ][ ROOM_REL ] || !objectp( thing ) ) {
      ret += ({ dest_other[ i ] });
    } else {
      ret += ({ (string)thing->find_rel( dest_other[ i ] ) });
    }
  }
  return ret;
} /* query_direc() */

/**
 * This method returns the destination room for an exit.
 * @param exit the exit name
 * @return the path of the destination room, or ROOM_VOID on error
 * @see query_dest_dir()
 * @example
 * #include <room.h>
 * string dest;
 *
 * dest = room->query_destination("south");
 * if (dest == ROOM_VOID) {
 *    do_error();
 * } else {
 *    do_move("south");
 * }
 */
string query_destination( string exit ) {
   int i;

   i = member_array( exit, dest_other );
   if ( ( i < 0 ) && objectp( this_player() ) )
      i = member_array( (string)this_player()->reorient_rel( exit ),
            dest_other );
   if ( i < 0 )
      return ROOM_VOID;
   return dest_other[ i + 1 ][ ROOM_DEST ];
} /* query_destination() */

/** @ignore yes */
int test_add( object thing, int flag ) { return 1; }

/** @ignore yes */
int test_remove( object thing, int flag, mixed dest ) { return 1; }

/** @ignore yes */
int add_weight( int number ) { return 1; }

/** @ignore yes */
int query_no_writing() { return 1; }

/** @ignore yes */
int query_decay() { return 10; }

/**
 * This method sets the default attack speed for the room.
 * This defaults to 15.
 * @return the default attack speed
 */
int attack_speed() { return 15; }

/**
 * This is the message to print instead of the room description when the
 * room is dark.  It defaults to the message "It's dark in here isn't it?".
 * @return the dark message
 * @see set_dark_mess()
 * @see long()
 * @see query_bright_mess()
 */
string query_dark_mess() {
  mixed dark_mess;

  if ( !stringp( dark_mess = query_property( "dark mess" ) ) ) {
    return "It's dark here, isn't it?";
  }
  return dark_mess;
} /* query_dark_mess() */

/**
 * This method sets the dark message associated with the room.
 * @param word the new dark message
 * @see query_dark_mess()
 * @see long()
 */
void set_dark_mess( string word ) {
    add_property( "dark mess", word );
} /* set_dark_mess() */

/**
 * This method returns the message to use when it is too bright to see in
 * the room.  It defaults to: "It's too bright to see anything!".
 * @return the message to print when it is too bright
 * @see query_dark_mess()
 * @see long()
 * @see set_bright_mess()
 */
string query_bright_mess() {
  mixed bright_mess;
 
  if ( !stringp( bright_mess = query_property( "bright mess" ) ) ) {
    return "It's too bright to see anything!";
  }
  return bright_mess;
} /* query_bright_mess() */

/**
 * This method sets the bright message associated with the room.
 * @param word the new bright message
 * @see query_bright_mess()
 * @see long()
 */
void set_bright_mess( string word ) { add_property( "bright mess", word ); }

/**
 * This method queries the size of the room.  The default size of a room
 * is 10x10x10.  A room can be any rectangular size, this method will return
 * an array of three elements if the room is a non-cube.  If it returns
 * a single number then the room is assumed to be cubic.
 * <p>
 * ({ north-south size, east-west size, up-down size })
 * <p>
 * The sizes are all radii's so they are half the actual width of the room.
 * @return the size of the room
 * @see set_room_size()
 * @see query_room_size_array()
 */
mixed query_room_size() {
   mixed room_size;

   room_size = query_property( "room size" );
   if ( !room_size ) {
      return 10;
   }
   return room_size;
} /* query_room_size() */

/**
 * This method returns the size of the room as a three element array always.
 * <p>
 * ({ north-south size, east-west size, up-down size })
 * <p>
 * The sizes are all radii's so they are half the actual width of the room.
 * @return the size of the room as a three element array
 * @see query_room_size()
 * @see set_room_size()
 */
int *query_room_size_array() {
   mixed room_size;

   room_size = query_room_size();
   if ( pointerp( room_size ) ) {
      return room_size;
   }
   return ({ room_size, room_size, room_size });
} /* query_room_size_array() */

/**
 * This method sets the rooms principle radii.  If the parameter isa single
 * number then the room is assumed to be cubic and dimension applies in
 * all directions.  If the input is a three element array then the elements
 * apply to all the directions.<br>
 * ({ north-south size, east-west size, up-down size })
 * <p>
 * The sizes are all radii's so they are half the actual width of the room.
 * @param number the new size of the room
 * @see query_room_size()
 * @see query_room_size_array()
 */
void set_room_size( mixed number ) {
   if ( intp( number ) ) {
      add_property( "room size", number );
      return;
   }
   if ( pointerp( number ) ) {
      if ( sizeof( number ) == 3 ) {
         add_property( "room size", number );
         return;
      }
   }
   write( "Room size must be an integer or an array of three integers.\n" );
} /* set_room_size() */

/** @ignore yes */
int id( string word ) { return 0; }

string expand_alias( string word ) {
   int i;

   if ( !aliases || !sizeof( aliases ) ) {
      return word;
   }
   i = member_array( word, aliases );
   if ( i == -1 ) {
      return word;
   }
   if ( i % 2 ) {
      return aliases[ i - 1 ];
   }
   return word;
} /* expand_alias() */

/**
 * This method returns the exit string used when in brief mode.
 * @return the brief exit string
 * @see calc_exit_string()
 */
string calc_short_exit_string() {
   int i, add;
   string *words;
   mixed tmp;

   words = ({ });
   for ( i = 0; i < sizeof( dest_other ); i += 2 ) {
      tmp = dest_other[ i + 1 ][ ROOM_OBV ];
      if ( !tmp ) {
         continue;
      }
      if ( intp( tmp ) && tmp ) {
         add = 1;
      }
      if ( stringp( tmp ) ) {
         add = (int)call_other( this_object(), tmp, dest_other[ i ] );
      }
      if ( pointerp( tmp ) ) {
         add = (int)call_other( tmp[ 0 ], tmp[ 1 ], dest_other[ i ] );
      }
      if ( add ) {
         if ( tmp = SHORTEN[ dest_other[ i ] ] ) { // is there a short form?
            if (dest_other[i+1][ROOM_REL]) {// is the exit relative
               words += ({ "$r$-"+tmp+"$r$" });
            } else {
               words += ({ tmp });
            }
         } else {                // no short form
            if (dest_other[i+1][ROOM_REL]) { // is the exit relative?
               words += ({ "$r$-"+dest_other[ i ]+"$r$" });
            } else {
               words += ({ dest_other[i] });
            }
         }
      }
   }
   if(!sizeof(words)) {
     return " [none]";
   }
   return " ["+ implode( words, "," ) +"]";
} /* calc_short_exit_string() */

/**
 * This method returns the short exit string.  The short exit string is the
 * string used in 'brief' mode of a players look.
 * @return the short exit string
 * @see calc_short_exit_string()
 * @see query_exit_string()
 */
string query_short_exit_string() {
   string tmp;
  
   if(short_exit) {
      return this_player()->colour_event("exits", "%^GREEN%^") +
     short_exit + "%^RESET%^";
   }
   
   tmp = calc_short_exit_string();
   if (!query_property("no exit cache")) {
      short_exit = tmp;
   }
   return this_player()->colour_event("exits", "%^GREEN%^") +
     tmp + "%^RESET%^";
} /* query_short_exit_string() */

/**
 * @ignore yes
 */
string enchant_string() {
   string words;
   
   words = (string)this_object()->query_property( "octarine_mess" );
   if ( words ) {
      return words +"\n";
   }
   switch ( query_enchant() ) {
      case 0 .. 49 :
         return "";
      case 50 .. 149 :
         return "There is the residual taste of magic in this place.\n";
      case 150 .. 299 :
         return "This place has seen some use of magic.\n";
      case 300 .. 499 :
         return "A considerable amount of magic has been used here.\n";
      case 500 .. 749 :
         return "A very large quantity of magic has been manipulated here.\n";
      case 750 .. 1000 :
         return "You can feel the Dungeon Dimensions trying to push in.\n";
      case 1001 .. 1500 :
         return "Little sparks flash in from the Dungeon Dimensions.\n";
      case 1501 .. 2000 :
         return "Apparations of things with lots of tentacles seem to be "
                "on the edge of your vision.\n";
      default :
         return "So much magic has been expended here that the area is in "+
               "danger of dumping itself into the Dungeon Dimensions.\n";
   }
} /* enchant_string() */

/** @ignore yes */
string long( string word, int dark ) {
   string ret;

   if ( !long_exit ) {
      calc_long_exit();
   }
   if ( dark ) {
      if ( dark < 0 ) {
         ret = this_object()->query_dark_mess() +"\n";
      } else {
         ret = this_object()->query_bright_mess() +"\n";
      }
      if ( query_property( "location" ) == "outside" ) {
         ret += "$weather$";
      }
      if ( ( dark == 1 ) || ( dark == -1 ) ) {
         ret = "$C$"+ a_short() +".  "+ ret +
           this_player()->colour_event("exits", "%^GREEN%^") +long_exit
           +"%^RESET%^\n";
         if ( query_contents( "" ) != "" ) {
            ret += this_player()->colour_event("inventory", "") + "Some objects you can't make out are here.%^RESET%^\n";
         }
      }
   } else {
      if ( query_property( "location" ) == "outside" ) {
         ret = "$long$";
      } else {
         ret = query_long();
      }

      if(!ret) {
         ret = "Erk, this room seems to be broken.\n";
      }
      
      word = calc_extra_look();
      if ( stringp( word ) && ( word != "" ) ) {
         ret += word;
      }
      if ( this_player()->query_see_octarine() ) {
         ret += enchant_string();
      }
      if ( query_property( "location" ) == "outside" ) {
         ret += "$weather$";
      }
      ret += this_player()->colour_event("exits", "%^GREEN%^") +
        long_exit +"%^RESET%^\n"+ query_contents( "" );
   }
   if ( query_property( "no exit cache" ) )  {
      long_exit = 0;
   }
   return ret;
} /* long() */

/** @ignore yes */
string pretty_short( object thing ) {
   int dark;

   if ( thing ) {
      dark = (int)thing->check_dark( (int)this_object()->query_light() );
   }
   return ::short( dark );
} /* pretty_short() */

/**
 * This method calculates the co-ordinates of this room.  The co-ordinates
 * are based on the surrounding rooms co-ordinates, if one of those rooms
 * are loaded.
 * @see query_co_ord()
 * @see modify_exit()
 */
void calc_co_ord() {
  int i, j, k, shift, *delta, *other_co_ord;
  string other;
   
  for ( i = sizeof( dest_other ) - 2; ( i > -1 ) && !co_ord; i -= 2 ) {
    /* if destination isn't loaded, skip it */
    other = dest_other[ i + 1 ][ ROOM_DEST ];
    if ( !find_object( other ) ) {
      continue;
    }
    /* if destination has no coordinates, skip it */
    other_co_ord = (int *)other->query_co_ord();
    if ( !other_co_ord ) {
      continue;
    }

    /* if exit had a delta defined, use that */
    j = -1;
    if ( delta = dest_other[ i + 1 ][ ROOM_DELTA ] ) {
      co_ord = copy( other_co_ord );
      if (pointerp(delta)) {
        k = 3;
        while ( k-- ) {
          /*
           * it's -= to make delta here the offset from this
           * room to the destination 
           */
          co_ord[ k ] -= delta[ k ];
        }
        continue;
      } else {
        j = member_array(delta, STD_ORDERS);
      }
    }

    /* if exit isn't a direction, skip it */
    if (j == -1) {
      j = member_array( dest_other[ i ], STD_ORDERS );
      if ( j == -1 ) {
        continue;
      }
    }

    co_ord = copy( other_co_ord );
    delta = query_room_size_array() + (int *)other->query_room_size_array();
    for ( k = 0; k < 3; k++ ) {
      co_ord[ k ] += STD_ORDERS[ j + 1 ][ k ] *
        ( delta[ k ] + delta[ k + 3 ] );
    }
    if ( ( j < 16 ) && dest_other[ i + 1 ][ ROOM_GRADE ] ) {
      /* one of the lateral directions */
      switch ( j ) {
      case 0 .. 1 :
        shift = delta[ 0 ] + delta[ 3 ];
        break;
      case 2 .. 3 :
        shift = delta[ 1 ] + delta[ 4 ];
        break;
      default :
        shift = delta[ 0 ] + delta[ 1 ] + delta[ 3 ] + delta[ 4 ];
      }
      co_ord[ 2 ] -= ( dest_other[ i + 1 ][ ROOM_GRADE ] * shift ) / 100;
      }
    co_ord_calculated = 1;
  }
} /* calc_co_ord() */

/**
 * This method calculates all the exit strings to be used for this room.
 */
void calc_exits() {
   int i, j;
   string exit, word, *tmp_al;

   exits = ({ });
   for ( i = sizeof( dest_other ) - 2; i > -1; i -= 2 ) {
      exit = dest_other[ i ];
      if ( member_array( exit, exits ) == -1 ) {
         exits += ({ exit });
         word = SHORTEN[ exit ];
         if ( stringp( word ) ) {
            exits += ({ word });
         }
      }
      tmp_al = aliases;
      j = member_array( exit, tmp_al );
      while ( j != -1 ) {
         if ( j % 2 ) {
            j--;
         } else {
            word = tmp_al[ j + 1 ];
            if ( member_array( word, exits ) == -1 ) {
               exits += ({ word });
            }
         }
         tmp_al = delete( tmp_al, j, 2 );
         j = member_array( exit, tmp_al );
      }
   }
} /* calc_exits() */

/** @ignore yes */
void init() {
  object ob;
  int i;
  
  /* This is some experimental XP stuff for exploration.
   *
  if(this_player() &&
     !this_player()->query_queued_commands() &&
     ((!last_visited && uptime() > 1800 + random(3600)) ||
      (random(time() - last_visited) > 900))) {
    this_player()->adjust_xp(random(random(500)), 0);
  }
  */

  
  // Has the day/night changed and do we care?
  if(is_day != -1 && ((WEATHER_HANDLER->query_day() > 0) != is_day)) {
    is_day = (1 - is_day);
    
    // Do longs;
    if(variablelongs && strlen(variablelongs[is_day]))
      set_long(variablelongs[is_day]);
    
    // Do items.
    if(variableitems) {
      for(i=0; i<sizeof(variableitems[1-is_day]); i += 2)
        remove_item(variableitems[1-is_day][i]);
      
      for(i=0; i<sizeof(variableitems[is_day]); i += 2)
        add_item(variableitems[is_day][i], variableitems[is_day][i+1]);
    }
    
    // Do chats.
    if(variablechats) 
      room_chat(variablechats[is_day]);
  }

  if(chatter)
    chatter->check_chat();

  if(sitchanger)
    sitchanger->check_situations();

  if(!sizeof(exits))
    calc_exits();

  if(!pointerp(co_ord))
    this_object()->calc_co_ord();

  foreach (ob in hidden_objects) {
    if ( ob && objectp( ob ) ) {
      ob->init();
    } else {
      hidden_objects -= ({ 0, ob });
    }
  }

  if (userp(this_player())) {
    foreach (ob in _use_internal_objects) {
      if (ob && objectp(ob)) {
        _use_internal_objects->find_inv_match("all", this_player())->init();
      } else {
        _use_internal_objects -= ({ 0, ob });
      }
    }
  }

  if(item)
    item->init();
}

/**
 * This method returns the set of move zones for this room.  This is used
 * by npcs to see which rooms they are allowed to move into.
 * @see add_zone()
 * @see /obj/monster->add_move_zone()
 */
string *query_zones() {
  string *zones;

  zones = query_property( "room zone" );
  if ( !zones ) {
    return ({ "nowhere" });
  }
  return zones + ({ });
} /* query_zones() */

/**
 * This method adds a move zone into the current list of movement zones
 * for the room.  The move zones are used by npcs to see which rooms they
 * are allowed to move into.
 * @see set_zone()
 * @see query_zones()
 * @see /obj/monster->add_move_zone()
 */
void add_zone(string zone) {
   string *zones;

   zones = query_property( "room zone" );
   if ( !zones ) {
      zones = ({ zone });
   } else {
      zones += ({ zone });
   }
   add_property( "room zone", zones );
} /* add_zone() */

/**
 * This method adds a move zone into the current list of zones.
 * This method is depreciated, add_zone should be used instead.
 * @see add_zone()
 * @see query_zones()
 */
void set_zone( string zone ) {
   add_zone(zone);
} /* set_zone() */

/**
 * This method determines if there is an exit in the specified direction.
 * @param direc the exit to test for
 * @return 1 if it exists, 0 if it does now
 */
int query_exit( string direc ) {
  return ( member_array( direc, dest_other ) != -1 );
} /* query_exit() */

/**
 * This method adds an exit to the room.   The direction is the direction in
 * which the exit should go.  This is something like "north" or "enter
 * gate".  The destination field is where the player will go when they
 * enter the exit.  The type is a set type that sets a whole bunch of
 * defaults for the room.  The destination can be either a strong
 * or an object.
 * <p>
 * The types are controlled by /obj/handlers/room_handler.c and the current
 * types and what this all means are:
 * <dl>
 * <dd>
 * <dl>
 * <dt>road
 * <dd>Wide road.
 * <dt>path
 * <dd>Narrower path
 * <dt>door
 * <dd>And exit with a door.  Defaults to closed but not locked.
 * <dt>secret
 * <dd>A secret door.  Defaults to closed but not locked.
 * <dt>corridor
 * <dd>A corridor (bing).
 * <dt>hidden
 * <dd>A hidden exit without a door
 * </dl>
 * </dl>
 * <p>
 * The room aliases are used to expand things for exits.  However they
 * don't expand the entire exit name.  They expand it in bits.  For
 * instance, if the exit was "enter live eel", you could
 * add_alias("eel", "live eel"); and add_alias("bing", "enter"); to
 * get both of the bits of the exit.  So "bing eel", "enter eel",
 * "bing live eel" etc would work.
 * @example
 * add_exit("north", PATH + "market2", "road");
 * add_alias("eel", "live eel");
 * add_exit("enter live eel", PATH + "live_eel", "secret");
 * @example
 * object fluffy_room;
 *
 * fluffy_room = clone_object(PATH + "fluffy_room");
 * add_exit("north", fluffy_room, "road");
 * @see modify_exit()
 * @see query_dest_dir()
 * @see remove_exit()
 * @see /obj/handlers/room_handler
 */
int add_exit( string direc, mixed dest, string type ) {
   mixed *stuff;

   if ( !dest_other ) dest_other = ({ });
   if ( member_array( direc, dest_other ) != -1 ) return 0;
   if ( objectp( dest ) )
      dest = file_name( dest );
   if ( dest[ 0 .. 0 ] != "/" )
      dest = "/"+ dest;
   stuff = ({ dest }) + (mixed *)ROOM_HANDLER->query_exit_type( type, direc );
   dest_other += ({ direc, stuff });
   if ( ( stuff = (mixed *)ROOM_HANDLER->query_door_type( type, direc,
                                                         dest ) ) ) {
      door_control[ direc ] = clone_object( DOOR_OBJECT );
      door_control[ direc ]->setup_door( direc, this_object(), dest, stuff,
                                         type);
      hidden_objects += ({ door_control[ direc ] });
      door_control[ dest ] = direc;
   }
   if ( find_call_out( "calc_exits" ) == -1 )
      call_out( "calc_exits", 1 );
   long_exit = 0;
   short_exit = 0;
   return 1;
} /* add_exit() */

/**
 * This method modifies the parameters for the exit.  See the docs in
 * /doc/new/room/modify_exit for more complete information.
 */
int modify_exit( mixed direc, mixed *data ) {
   int i, j, k;
   
   if(pointerp(direc)) {
     for(k = 0; k < sizeof(direc); k++) {
       modify_exit(direc[k], data);
     }
     return 0;
   }
   if ( ( i = member_array( direc, dest_other ) ) == -1 ) {
     return 0;
   }
   for ( j = 0; j < sizeof( data ); j+= 2 ) {
     switch ( lower_case( data[ j ] ) ) {
     case "message" :
       dest_other[ i + 1 ][ ROOM_EXIT ] = data[ j + 1 ];
       break;
     case "exit mess" :
     case "exit_mess" :
       dest_other[ i + 1 ][ ROOM_EXIT ] = data[ j + 1 ];
       break;
     case "move mess" :
       dest_other[ i + 1 ][ ROOM_MESS ] = data[ j + 1 ];
       break;
     case "linker mess" :
       dest_other[ i + 1 ][ ROOM_LINK_MESS ] = data[ j + 1 ];
       break;
     case "obvious" :
       dest_other[ i + 1 ][ ROOM_OBV ] = data[ j + 1 ];
       if ( !intp( data[ j + 1 ] ) )
         add_property( "no exit cache", 1 );
       long_exit = 0;
       short_exit = 0;
       break;
     case "function" :
       dest_other[ i + 1 ][ ROOM_FUNC ] = data[ j + 1 ];
       break;
     case "size" :
       dest_other[ i + 1 ][ ROOM_SIZE ] = data[ j + 1 ];
       break;
     case "upgrade" :
       dest_other[ i + 1 ][ ROOM_GRADE ] = data[ j + 1 ];
       break;
     case "downgrade" :
       dest_other[ i + 1 ][ ROOM_GRADE ] = -data[ j + 1 ];
       break;
     case "enter" :
       dest_other[ i + 1 ][ ROOM_ENTER ] = data[ j + 1 ];
       break;
     case "enter mess" :
     case "enter_mess" :
       if ( sizeof( dest_other[ i + 1 ][ ROOM_ENTER ] ) == 2 )
         dest_other[ i + 1 ][ ROOM_ENTER ] = replace( data[ j + 1 ],
           "$F", dest_other[ i + 1 ][ ROOM_ENTER ][ 1 ] );
       else
         dest_other[ i + 1 ][ ROOM_ENTER ] = data[ j + 1 ];
       break;
     case "dest" :
       dest_other[ i + 1 ][ ROOM_DEST ] = data[ j + 1 ];
       if ( objectp( door_control[ direc ] ) ) {
         door_control[ direc ]->set_dest( data[ j + 1 ] );
         door_control[ data[ j + 1 ] ] = direc;
       }
       break;
     case "relative" :
       dest_other[ i + 1 ][ ROOM_REL ] = data[ j + 1 ];
       break;
     case "look" :
       dest_other[ i + 1][ ROOM_LOOK ] = data[ j + 1 ];
       break;
     case "look func" :
       dest_other[ i + 1][ ROOM_LOOK_FUNC ] = data[ j + 1 ];
       break;
     case "no map" :
       dest_other[ i + 1][ ROOM_NO_MAP ] = data[ j + 1 ];
       break;
     case "delta" :
       dest_other[ i + 1][ ROOM_DELTA ] = data[ j + 1 ];
       break;
     case "closed" :
       if ( objectp( door_control[ direc ] ) ) {
         data[j+1] ? door_control[ direc ]->set_closed() :
           door_control[ direc ]->set_open();
       }
       break;
     case "open" :
       if ( objectp( door_control[ direc ] ) ) {
         data[j+1] ? door_control[ direc ]->set_open() :
           door_control[ direc ]->set_closed();
       }
       break;
     case "transparent" :
       if ( objectp( door_control[ direc ] ) ) {
         data[j+1] ? door_control[ direc ]->set_transparent() :
           door_control[ direc ]->reset_transparent();
       }
       break;
     case "stuck" :
       if ( objectp( door_control[ direc ] ) ) {
         door_control[ direc ]->set_stuck( data[ j + 1 ] );
       }
       break;
     case "locked" :
       if ( objectp( door_control[ direc ] ) ) {
         if(data[j+1]) {
           door_control[ direc ]->set_closed();
           door_control[ direc ]->set_locked();
         } else
           door_control[ direc ]->set_unlocked();
       }
       break;
     case "unlocked" :
       if ( objectp( door_control[ direc ] ) ) {
         data[j+1] ? door_control[ direc ]->set_unlocked() :
           door_control[ direc ]->set_locked();
       }
       break;
     case "autolock":
       if ( objectp( door_control [direc ] ) ) {
         door_control[ direc ]->set_autolock( data[ j + 1 ] );
       }
       break;
     case "key" :
       if ( objectp( door_control[ direc ] ) ) {
         door_control[ direc ]->set_key( data[ j + 1 ] );
       }
       break;
     case "other" :
       if ( objectp( door_control[ direc ] ) ) {
         door_control[ direc ]->set_other_id( data[ j + 1 ] );
       }
       break;
     case "difficulty" :
       if ( objectp( door_control[ direc ] ) ) {
         door_control[ direc ]->set_difficulty( data[ j + 1 ] );
       }
       break;
     case "door long" :
       if ( objectp( door_control[ direc ] ) ) {
         door_control[ direc ]->set_long( data[ j + 1 ] );
       }
       break;
     case "open/close func" :
       if ( objectp( door_control[ direc ] ) ) {
         door_control[direc]->set_open_trap(data[j+1][0], data[j+1][1]);
       }
       break;
     case "lock/unlock func" :
       if ( objectp( door_control[ direc ] ) ) {
         door_control[direc]->set_lock_trap(data[j+1][0], data[j+1][1]);
       }
       break;
     case "door short" :
       if ( objectp( door_control[ direc ] ) ) {
         door_control[ direc ]->set_short( data[ j + 1 ] );
       }
       break;
     case "double doors" :
       if ( objectp( door_control[ direc ] ) ) {
         door_control[ direc ]->set_how_many( data[ j + 1 ] );
       }
       break;
     case "one way" :
       if ( objectp( door_control[ direc ] ) ) {
         door_control[ direc ]->set_one_way( data[ j + 1 ] );
       }
       break;
     case "secret" :
       if ( objectp( door_control[ direc ] ) ) {
         door_control[ direc ]->remove_hide_invis( "secret" );
         if ( data[ j + 1 ] > 0 ) {
           door_control[ direc ]->add_hide_invis( "secret", 0,
                                                  data[ j + 1 ], 0 );
         }
       }
       break;
     case "undoor" :
       if ( objectp( door_control[ direc ] ) ) {
         door_control[ direc ]->go_away();
         hidden_objects -= ({ door_control[ direc ] });
         door_control = m_delete( door_control, direc );
         door_control = m_delete( door_control,
                                  dest_other[ i + 1 ][ ROOM_DEST ] );
         /* Hope that something takes care of the other side. */
       }
       break;
     }
   }
   return 1;
} /* modify_exit() */

/**
 * This method removes the specified exit from the room.
 * @see add_exit()
 * @see modify_exit()
 */
int remove_exit( string direc ) {
  int i;

  if ( !dest_other ) {
    dest_other = ({ });
    return 0;
  }
  i = member_array( direc, dest_other );
  if ( i == -1 )
    return 0;
  if ( door_control[ direc ] ) {
    door_control[ direc ]->dest_me();
    hidden_objects -= ({ door_control[ direc ] });
    door_control = m_delete( door_control, direc );
    door_control = m_delete( door_control, dest_other[ i + 1 ][ ROOM_DEST ] );
  }
  dest_other = delete( dest_other, i, 2 );
  if ( find_call_out( "calc_exits" ) == -1 )
    call_out( "calc_exits", 1 );
   long_exit = 0;
   short_exit = 0;
  return 1;
} /* remove_exit() */

/**
 * This method checks to see if the door is open.
 * @param direc the direction of the door
 * @return -1 on an error, 0 for closed, 1 for open
 * @see modify_exit()
 */
int query_door_open( string direc ) {
  if ( !objectp( door_control[ direc ] ) ) {
    return -1;
  }
  return (int)door_control[ direc ]->query_open();
} /* query_door_open() */

/**
 * This method checks to see if the exit is a relative one.
 * @param direc the direction to check
 * @see modify_exit()
 */
int query_relative( string direc ) {
   int i;

   i = member_array( direc, dest_other );
   if ( i == -1 ) {
      return 0;
   }
   return dest_other[ i + 1 ][ ROOM_REL ];
} /* query_relative() */

string query_look( string direc ) {
   int i;

   i = member_array( direc, dest_other );
   if ( i == -1 )
      return 0;
   if ( !dest_other[ i + 1 ] )
      return 0;
   return (string)evaluate(dest_other[ i + 1 ][ ROOM_LOOK ]);
} /* query_look() */

mixed *query_look_func( string direc ) {
  int i;
  if ( ( i = member_array( direc, dest_other ) ) == -1 ) return 0;
  if( !dest_other[ i + 1 ] ) return 0;
  return dest_other[ i + 1 ][ ROOM_LOOK_FUNC ];
}

/**
 * This method returns the size of the exit.  This is used to check to make
 * sure that people can enter it.
 * @param direc the direction of the exit to check
 * @return the size of the exit
 * @see modify_exit()
 */
int query_size( string direc ) {
  int i;
  if ( ( i = member_array( direc, dest_other ) ) == -1 ) return 0;
  if ( stringp( dest_other[ i + 1 ][ ROOM_SIZE ] ) )
    return (int)call_other( this_object(), dest_other[ i + 1 ][ ROOM_SIZE ] );
  if ( pointerp( dest_other[ i + 1 ][ ROOM_SIZE ] ) )
    return (int)call_other( dest_other[ i + 1 ][ ROOM_SIZE ][ 0 ],
        dest_other[ i + 1 ][ ROOM_SIZE ][ 1 ] );
  return dest_other[ i + 1 ][ ROOM_SIZE ];
} /* query_size() */
 
/** @ignore yes */
void event_magic( object channel, int amount, object caster ) {
   add_enchant( amount / 5 );
} /* event_magic() */

/** @ignore yes */
void event_theft( object command_ob, object thief, object victim,
                  object *stolen ) {
  log_file( "THEFT", "%s: %s stole %s from %s in %s\n",
            ctime( time() ), (string)thief->query_short(),
            implode( (string *)stolen->query_short(), ", " ),
            (string)victim->query_short(), file_name() );
  
  if ( stringp( theft_handler )) {
    if( theft_handler != "none" )
      theft_handler->handle_theft( this_object(), command_ob, thief,
                                   victim, stolen );
  } else
    "/obj/handlers/theft_handler"->handle_theft( this_object(), command_ob,
                                                 thief, victim, stolen );
} /* event_theft() */

/** @ignore yes */
void event_exit(object ob, string message, object to) {
    if(interactive(ob))
    last_visited = time();
} /* event_exit() */

/** @ignore yes */
int query_last_visited() { return last_visited; }

/**
 * This method adds an item description to a room.  This allows you to
 * set up objects which do not as such exist, but can be looked at for
 * instance.  There should be a lot of these in rooms.  The name of
 * the item can be multiple word, and the plural for it is
 * automagicaly added, unless the no_plural flag is set.  If the name
 * is an array all of the elements in the array respond to the
 * description.
 * <p>
 * If the desc is set to an array, you can use this for handling
 * things like read messages and so on too.  Every second element in
 * the array is the description/text to be printed and the other
 * element is the command upon which the text should be printed.  The
 * special command 'long' is used to set the long description.
 * <p>
 * The special type 'position' is used to allow people to use that
 * item to do positions on, like stand, sit, lie etc.
 * <p>
 * This method also allows you to setup add_command patterns.  If the
 * name after the verb is just a string, then the string will be printed
 * when that verb is used.  If it is just a function pointer then the
 * function will be evaluated and the return result printed.  If it
 * is an array, then the first element must be a function pointer and
 * the second optional element is the pattern to use for that method.
 * Multiple patterns and functions may be specified.
 * @param shorts the short description of the item
 * @param desc the description of the item
 * @param no_plural do not automaticaly add a plural for the item
 * @return 1 if successfully added, 0 if not
 * @example
 * add_item("green pot plant", "It is a nasty green pot plant lurking by "
 *                             "the door.\n");
 * @example
 * add_item(({ "telephone", "red phone" }),
 *          "Sitting in the corner is the red phone, it is staring unhappily "
 *          "into space thinking of cupcakes and better times.\n");
 * @example
 * add_item("small book", ({ "long", "A small red book with dots on the
 *                                   "cover.\n",
 *                           "read", "It says 'Rabbit!' in big letters.\n" }) );
 * @example
 * add_item("green leather couch",
 *          ({ "long", "The green leather couch is wonderful , so comfy!  "
 *                     " So... Comfy!\n"m
 *             "position", "the green leather couch" }) );
 * @example
 * add_item("rotating hologram", (: query_current_hologram_string() :));
 * @example
 * add_item("glue stick",
 *          ({ "long", "The glue stick looks sticky, like you could slime "
 *                     "something with it.\n",
 *             "slime", ({ (: do_slime :),
 *                         "<indirect:living> with <direct:object>" }) }) );
 * @see query_item()
 * @see remove_item()
 * @see modify_item()
 */
varargs int add_item( mixed shorts, mixed desc, int no_plural ) {
  if (!desc) {
     // A 0 description is a definate no no.
     printf("Error!  In %O add_item(%O, 0), not added.\n", file_name(),
                                                           shorts);
     return 0;
  }

  if ( !item ) {
     item = clone_object( ITEM_OBJECT );
  }
  item->setup_item( shorts, desc, no_plural );
  return 1;
} /* add_item() */

/**
 * This method will attempt to remove the item defined by the given string.
 * This will remove everything associated with that item, verbs, patterns,
 * everything.
 * @param word the name of the item to remove
 * @return 1 if successful, 0 on a failure
 * @example
 * add_item("frog", "Cute, green and sitting on a lilly pad.  Yes!\n");
 * ...
 * remove_item("frog");
 * @example
 * add_item(({ "big bad chicken", "clucker" }),
 *          "The big bad chicken sits and stares at you.\n");
 * ...
 * remove_item("big bad chicken");
 * @see add_item()
 * @see query_item()
 */
int remove_item( string word ) {
  if ( !item ) {
    return 1;
  }
  return (int)item->remove_item( word );
} /* remove_item() */

/**
 * This method will modify certain bits of the specified item.  This will
 * change only the bits of the pattern that are specified.  If you wish to
 * remove elements a better method would be to remove the item and
 * then readd it.  The format of the new_desc array is the same as in the
 * add_item code.
 * @param word the name of the item to change
 * @param new_desc the bits of the item to change
 * @see remove_item()
 * @see add_item()
 */
int modify_item( string word, mixed new_desc ) {
  if ( !item ) {
    return 0;
  }
  return (int)item->modify_item( word, new_desc );
} /* modify_item() */

/** @ignore yes */
void add_effect( string eff, mixed arg ) {
   effects = clone_object( "/std/shadows/misc/effects" );
   effects->setup_shadow( this_object() );
   effects->add_effect( eff, arg );
} /* add_effect() */

/**
 * This method sets up a linkage between the current room and othert
 * rooms.  The linkage broadcasts things like says and enter/exit
 * messages between the rooms.
 * <P>
 * The the dynamic preposition is used when someone enters/exits
 * the room, the static preposition is used when someone says something
 * in the room.  The dynamic proposition defaults to "into" and the
 * static preposition defaults to "in".
 * @param rooms the rooms to link together
 * @param d_prep the dynamic preposition
 * @param s_prep the static preposition
 * @param r_name the name of the room/area
 * @example
 * set_linker( ({ PATH + "room1", PATH + "room2", }),
 *             "into", "in", "fluffy square");
 */
varargs int set_linker( string *rooms, string d_prep, string s_prep,
    string r_name ) {
  if ( linker ) {
    return 0;
  }
  linker = clone_object( LINKER_OBJECT );
  linker->setup_shadow( this_object(), rooms, d_prep, s_prep, r_name );
  return 1;
} /* set_linker() */

int set_terrain( string terrain_name ) {
   if ( terrain ) {
      return 0;
   }
   terrain = clone_object( TERRAIN_OBJECT );
   terrain->setup_shadow( this_object(), terrain_name );
   set_not_replaceable(1);
   return 1;
} /* set_terrain() */

void set_wall( mixed *args ) {
   if ( !wall ) {
      wall = clone_object( WALL_OBJECT );
      wall->setup_shadow( this_object() );
   }
   wall->set_wall( args );
} /* set_wall() */

/**
 * This method sets the default position for the room.  Se the set
 * default position in the living code for a more complete
 * example of this.
 * @param pos the default position
 * @see /ostd/living/living->set_default_position()
 */
void set_default_position(mixed stuff) {
  add_property(DEFAULT_POSITION_PROPERTY, stuff);
} /* set_default_position() */

/**
 * This method returns the current default position asigned to this
 * room.
 * @return the current default position
 */
mixed query_default_position() {
   return query_property(DEFAULT_POSITION_PROPERTY);
} /* query_default_position() */

/**
 * This method tells us if the passed i nposition is
 * allowed in this type of room.
 * @param poss the position to check
 */
int is_allowed_position(string poss) {
  switch (poss) {
    case SITTING :
    case STANDING :
    case KNEELING :
    case LYING :
    case MEDITATING :
    case CROUCHING :
      return 1;
    default :
      return 0;
  }
} /* is_allowed_position() */

/** @ignore yes */
void dest_me() {
   int in_armoury, in_void;
   object thing, *things;

   if ( file_name( this_object() ) == ARMOURY )
      in_armoury = 1;
   if ( file_name( this_object() ) == ROOM_VOID )
      in_void = 1;

   /* If this is not the void, remove contents to /room/rubbish for
      recycling. Move players to the void */
   if ( !in_void ) {
      things = all_inventory( this_object() );
      foreach( thing in things ) {
         if ( userp( thing ) ) {
            thing->move_with_look( ROOM_VOID, "$N fall$s into the void." );
            continue;
         }
         if(thing->cleaning_room())
           continue;
         
         thing->move("/room/rubbish");
      }
   }
   if ( chatter )
      chatter->dest_me();
   if ( sitchanger )
      sitchanger->dest_me();
   if ( effects )
      effects->destruct_shadow( effects );
   if ( linker )
      linker->destruct_shadow( linker );
   if ( terrain )
      terrain->destruct_shadow( terrain );
   if(wall)
     wall->destruct_shadow(wall);
   if ( item )
      item->dest_me();
    
   if(door_control)
     foreach(thing in keys(door_control))
       if(objectp(thing))
         catch(thing->dest_me());

   if ( sizeof( hidden_objects ) )
      foreach( thing in hidden_objects ) {
         // Don't dest thing if it's hidden in multiple rooms
         if ( objectp( thing ) && ( thing->multiple_hidden() == 0 ) )
           catch( thing->dest_me() );
      }
   destruct( this_object() );
} /* dest_me() */

/**
 * This method sets the flag that enables or disables the room being
 * cleaned up.  If they flag is set to 1, then room is never cleaned up.
 * @param flag the room being cleaned up flag
 * @see query_keep_room_loaded()
 */
void set_keep_room_loaded(int flag) {
   add_property(ROOM_KEEP_PROP, flag);
} /* set_keep_room_loaded() */

/**
 * This method returns the status of the keep room loaded flag.  If they
 * flag is non-0 then the room with not be unloaded.
 * @return the status of the keep room loaded flag
 */
int query_keep_room_loaded() {
   return query_property(ROOM_KEEP_PROP);
} /* query_keep_room_loaded() */
 
/** @ignore yes */
int clean_up( int parent ) {
  if (parent) {
    return 0;
  }
  if (query_keep_room_loaded()) {
      return 0;
  }
  call_out("real_clean", 30 + random(120));
  return 1;
} /* clean_up() */

/** @ignore yes */
int real_clean() {
   object thing;

   /*
    * Don't clean up the room if:
    * there's a player in it, there's a unique npc in it and it's been visited
    * by a player in the last hour, it's a slave room or there's a corpse in
    * it.
    */
   foreach ( thing in all_inventory( this_object() ) ) {
     if ( thing->query_property( "player" ) ||
          (thing->query_property( "unique" ) &&
           last_visited > time() - 3600) ||
          thing->query_slave() ||
          thing->query_name() == "corpse") {
         return 0;
      }
   }
   dest_me();
   return 1;
} /* real_clean() */

/** 
 * This method returns all the matchable objects in the room.  This is used
 * by find_match to determine the group of objects to select from.
 * @param words the words to match on
 * @param looker the person doing the pmacthing
 * @return the array of objects to match on
 */
object *find_inv_match( string words, object looker ) {
   object *things;

   things = all_inventory( this_object() );
   if ( pointerp( hidden_objects ) ) {
      things += hidden_objects;
   }

   //
   // Only do these extra checks for players.
   //
   if (looker && userp(looker)) {
      /* Chekced to this filter thing to speed it up. */
      things = filter(things, (: $1 && $1->short(0) &&
                              (!$2 || $1->query_visible($2)) :), looker);
   }

   if ( item ) {
      things += ({ item });
   }
   return things;
} /* find_inv_match() */

/**
 * This method adds a sign into the room.  Any of these elements can
 * be set to 0, except the long description.
 * @param sign_long the long description of the sign
 * @param sign_read_mess the readable message on the sign
 * @param sign_short the short description of the sign
 * @param sign_name the name of the sign
 * @param sign_language the language the sign is written in
 * @return the object for the sign
 */
varargs object add_sign( string sign_long, mixed sign_read_mess, string
    sign_short, mixed sign_name, string sign_language ) {
  object sign;
  string* bits;

  sign = clone_object( "/std/object" );
  if ( !sign_name ) {
     sign_name = "sign";
  }
  if (pointerp(sign_name)) {
    bits = explode(sign_name[0], " ");
    sign->set_name( bits[<1] );
    sign->add_adjective( bits[0..<2]);
    sign->add_alias(sign_name[1..]);
  } else {
    bits = explode(sign_name, " ");
    sign->set_name( bits[<1] );
    sign->add_adjective( bits[0..<2]);
  }
  sign->set_long( sign_long );
  if ( !sign_language ) sign_language = "common";
  sign->set_read_mess( sign_read_mess, sign_language );
  sign->reset_get();
  if ( sign_short && ( sign_short != "" ) ) {
    sign->set_short( sign_short );
    sign->set_main_plural( pluralize( sign_short ) );
    sign->move( this_object() );
    sign->add_property("there", "here");
  } else hidden_objects += ({ sign });
  return sign;
} /* add_sign() */

/** @ignore yes */
void tell_door( string direc, string message, object thing ) {
   if ( objectp( door_control[ direc ] ) )
      door_control[ direc ]->tell_door( message, thing );
} /* tell_door() */

/** @ignore yes */
varargs mixed call_door( string direc, string func, mixed arg1, mixed arg2,
      mixed arg3 ) {
   if ( objectp( door_control[ direc ] ) )
      return (mixed)call_other( door_control[ direc ], func, arg1, arg2,
            arg3 );
} /* call_door() */

/**
 * This method determines if the specified exit is a door or not.
 * @param dest the destination to check for being a door
 * @see query_exit()
 * @see add_exit()
 */
string query_door( mixed dest ) {
   int i;
   string direc;
   mixed bing;

   if ( objectp( dest ) ) {
      dest = file_name( dest );
   }
   if ( !stringp( dest ) ) {
      return 0;
   }
   if ( bing = door_control[ dest ] ) {
      if ( !objectp( bing ) ) {
         direc = bing;
      }
   }
   if ( !direc ) {
      return 0;
   }
   bing = door_control[ direc ];
   if ( objectp( bing ) ) {
      return direc;
   }
   bing = clone_object( DOOR_OBJECT );
   i = member_array( direc, dest_other );
   bing->setup_door( direc, this_object(), dest, dest_other[ i + 1 ] );
   hidden_objects += ({ bing });
   door_control[ direc ] = bing;
   return direc;
} /* query_door() */

/**
 * This method stops all the room chats for the room.  It also removes
 * all the room chats, so if you want to have any more you must
 * add them again.
 * @see room_chat()
 */
void stop_room_chats() {
   if ( chatter ) {
      chatter->dest_me();
   }
} /* stop_room_chats() */

/**
 * Allows the chat interval to be changed.
 * @param min minimum interval between chats (seconds)
 * @param max maximum interval between chats (seconds)
 */
void set_chat_min_max( int min,int  max ) {
  if (chatter) chatter->set_chat_min_max(min,max);
}

/**
 * Adds more chats to the existing set of room chats
 * managed by this chatter object.
 * @param new_chats an array of new chat strings
 * @see remove_room_chats
 * @see query_room_chats
 * @see /std/room/basic/chatter
 */
void add_room_chats( string *new_chats ) {
  if (chatter) chatter->add_room_chats( new_chats );
} /* add_room_chats() */

/**
 * Removes chats from the set of room chats
 * managed by this chatter object.  If there are no chats
 * left the chatter is destructed.
 * @param dead_chats an array of chat strings to remove
 * @see add_room_chats
 * @see query_room_chats
 * @see /std/room/basic/chatter
 */
void remove_room_chats( string *dead_chats ) {
 if (chatter) chatter->remove_room_chats( dead_chats );
} /* remove_room_chats() */

/**
 * Returns the set of room chats
 * managed by the chatter object.  
 * @return pointer to the mixed array of chat args
 * @example
 *  ({ 120, 240, ({ "A frog gimbles the curtains.",
 *                  "A truly revolting smell drifts insidiously "
 *                  "from the rug." }) })
 * @see add_room_chats
 * @see remove_room_chats
 * @see room_chat
 * @see /std/room/basic/chatter
 */
mixed *query_room_chats() { 
  if (chatter) return chatter->query_room_chats();
  return 0;
} /* query_room_chats() */

/**
 * This method sets up the room chats.
 * Room chats are strings which are printed at (semi) random intervals
 * in rooms.  They are used to add atmosphere to a room. A chat will
 * be picked at random from the array of chats with a frequency
 * controlled by the times min and max. ie. one will be picked every n
 * seconds where is varies between min and max seconds. Please don't
 * make the values for min and max too small or the messages just
 * become annoying!
 * <p>
 * The argument to the room_chat method is an array of the format:<br>
 * ({ int min, int max, ({ string *chats }) }).  In place of a chat
 * string you may use "#function_name" where function_name is a
 * function that exists on the room object.
 * <p>
 * Repeated calls to this function overwrite the chats for
 * the default chatter.
 * @param args the room chat arguments
 * @param chatobj chatter object in case the default offends you.
 *                This argument may be omitted in which case you get 
 *                /std/room/basic/chatter.c
 * @example
 * room_chat(({ 120, 240, ({ "A string frog wanders past.",
 *                           "#make_soggy_bread",
 *                           "A trully revolting smell drifts insidiously "
 *                                "from the bakery." }) }) );
 * @see stop_room_chat()
 * @see add_room_chats()
 * @see remove_room_chats()
 * @see set_chat_min_max()
 */
varargs void room_chat( mixed *args, object chatobj ) {
   if ( !pointerp( args[ 2 ] ) ) {
      write( "Error: second argument of room_chat args is not an array.\n" );
      return;
   }
  if ( chatter ) {
    chatter->setup_chatter( this_object(), args );
    return;
  }
  if (objectp(chatobj)) chatter = chatobj;
  else chatter = clone_object( CHATTER_OBJECT );
  chatter->setup_chatter( this_object(), args );
} /* room_chat() */

/**
 * Set a situation changer (in place of the default).
 * If there is no argument you get the default:
 * /std/room/basic/situation_changer.
 * You call this before any other situation related functions.
 * If you create your own changer it should inherit one of
 * /std/room/basic/situation_changer or
 * /std/room/basic/multiroom_situation_changer or
 * otherwsie provide the functionality of those objects.
 * @param changer optional parameter specifying either a path for
 * the changer object or an existing object to use.
 * @see add_situation
 * @see start_situation
 * @see automate_situation
 * @see change_situation
 * @example
 * If you have a special changer object used for more than one room
 * then in setup for those rooms you should have:
 * set_situation_changer(load_object("/w/me/mychanger"));
 * Where /w/me/mychanger inherits 
 * /std/room/basic/multiroom_situation_changer
 */
varargs object set_situation_changer( mixed changer ) {
  if (stringp(changer)) {
    sitchanger = clone_object( changer );  
  } else if (objectp(changer)) {
    sitchanger = changer; 
  } else {
    sitchanger = clone_object( SITUATION_CHANGER_OBJECT );
  }
  return (sitchanger = sitchanger->set_room( this_object() ));
} /* set_situation_changer() */

/**
 * Adds a situation to the room.  These situations can be
 * invoked manually with start_situation or automatically via
 * automate_situation.
 * @param label string or number labelling the situation
 * @param sit a structure (class) containing all the bits
 * of the situation you want to add.  It should be a
 * variable of class situation.  You should include
 * situations.h where this class is defined.
 * Every part is optional.
 * eg. 
 * start_func  function to be called at start of situation 
 *             that might be used to load NPC's or anything
 *             beyond a message.
 * 
 *             The start function is passed the label, 
 *             a do_start_mess flag and the room object.  
 *             If the flag is 1 the situation is starting 
 *             rather than being reloaded.  Thus if 
 *             do_start_mess is 0 then you should avoid
 *             any obvious start messages and make it look
 *             like the situation is already underway.
 *
 * end_func    function to be called an the end of a situation.  
 *             The end function is only
 *             passed the label and the room object.
 *
 * start_mess  message told to the room at start of situation
 *
 * end_mess    message told to the room at end of situation
 *
 * extra_look  extra look string appended to rooms long 
 *             during the situation
 * chat_rate   an array of 2 numbers giving the minimum and
 *             maximum delay between chats.  If this is set
 *             then the chats are not merged with the
 *             existing chats but added independently with
 *             their own chat rates as given.
 *
 * chats       an array of chat strings to be active 
 *             during the situation 
 *
 * add_items   a mixed array of ({ item, item description }) 
 *             pairs to be active during the situation
 * 
 * random_words  sets of words to insert into text to replace 
 *               the special character #n where n is a number.
 *             The form of the array is ({  #1array, #2array, ... })
 *             where #1array = ({ "#1word1","#1word2",... }) etc.
 *             For the duration of the situation one of the strings
 *             in #1array is used to replace all instances of #1
 *             in the extra_look, start_mess, end_mess, chats
 *             and key and the long description part of the add_items.
 *             In a situation compounded of many situations
 *             the same random seed is used for choosing all #1's
 *             for each individual situation for the duration, 
 *             and a different seed for all #2's etc. 
 *
 * @see start_situation
 * @see end_situation
 * @see automate_situation
 * @see change_situation
 * @see add_item
 * @see room_chat
 * @see add_extra_look
 * @see set_situation_changer
 * @see make_situation_seed
 * @see /include/situation.h
 * @see goto learning  search  situation  for working examples
 * @example
 * #include <situations.h>
 *
 * class situation frogs;
 * frogs = new(class situation, 
 *     start_mess: "Water seeps out of the ground to form puddles.",
 *     extra_look: "There are large puddles on the ground here.",
 *     chat_rate: ({ 120,180 }),
 *     chats: ({"A hidden frog croaks quietly.",
 *              "There is a blooping sound." }),
 *     add_items:({ ({"puddle", "The puddles are dark and murky.  " 
 *                    "They will probably dry up given time." }) }) );
 * add_situation( "frogs", frogs );
 *
 * @example
 * add_situation( "ship", new(class situation, 
 *      start_mess: "A #1 ship hoves into view.",
 *      extra_look: "There is a #1 ship forging up the river.",
 *      chats: ({"The #1 ship's sails flap in the breeze.",
 *               "Shouts of sailors carry over to you from the #1 ship." }),
 *      add_items: ({ ({"ship", "The #1 ship, the \"#2\" is a small "
 *                    "sailing vessel that transports cargo up and "
 *                    "down the river."}) }),
 *      random_words: ({ ({ "old","waterlogged","heavily laden" }),
 *                       ({ "Jemima", "Old Sea Dog", "Randy Mermaid" }) })
 *      ) );
 * When the situation is started a random choice (eg. "old") replaces #1 
 * and a name (eg. "Jemima") replaces #2 in the text strings for
 * the duration.  
 * 
 * @example
 * add_situation( "frogs", new(class situation, 
 *      start_mess: "Water seeps out of the ground to form puddles.",
 *      extra_look: "There are large puddles on the ground here.",
 *      chats: ({"A hidden frog croaks quietly.",
 *               "There is a blooping sound." }),
 *      add_items: ({ ({"puddle", "The puddles are dark and murky.  " 
 *                      "They will probably dry up given time." }) }) ));
 * @example
 * This is an empty situation, useful for making pauses in the action.
 * add_situation( "pause", new(class situation) );
 */
void add_situation( mixed label, class situation sit ) {
  if (!sitchanger) {
    sitchanger = clone_object( SITUATION_CHANGER_OBJECT );
    sitchanger->set_room( this_object() );
  }
  sitchanger->add_situation( label, sit );
} /* add_situation() */

/**
 * Makes a seed value for the random part of when 
 * situations turn on and off.  The two ints should be
 * constant for a given room -- eg. the coordinates.
 * If this function is not called the seed is 
 * generated from the file_name of the object.
 * For this reason, rooms that are clones will produce
 * erratic results unless this function is called.
 * @param xval integer to use to make a seed (eg. x coordinate)
 * @param yval integer to use to make a seed (eg. y coordinate)
 */
void make_situation_seed(int xval, int yval) {
  if (sitchanger) sitchanger->set_seed(xval,yval);
} /* make_situation_seed() */

/**
 * Starts a situation previously added to the room.  These situations can be
 * invoked manually with start_situation or automatically via
 * automate_situation.  The call is passed to the situation
 * changer object.  If there isn't one nothing happens. 
 * @param label label for the situation as passed to add_situation
 * @param do_start_mess 0 to supress the start_mess string
 *        This is to fake it that a situation has been 
 *        going for a while when really you just loaded it.
 * @see add_situation
 * @see end_situation
 * @see automate_situation
 * @see change_situation
 */
void start_situation(int label, int do_start_mess) {
  if (sitchanger) 
    sitchanger->start_situation( label, do_start_mess );
} /* start_situation */

/**
 * Ends a situation previously added and started on the room.  
 * These situations can be invoked manually with start_situation 
 * or automatically via automate_situation. 
 * @param label label for the situation
 * @see add_situation
 * @see start_situation
 * @see automate_situation
 * @see change_situation
 */
void end_situation(mixed label) {
  if (sitchanger) sitchanger->end_situation( label );
} /* end_situation() */

/**
 * Starts one or more situations that will end after a
 * specified duration.  You can use an array and make
 * further situations commence when others end.
 * @param label (mixed) label of the situation to start up.  
 * If you pass an array such as ({ "frog1", "frog2" }) for the 
 * label then that set of situations are started one at
 * a time and the total duration is split evenly between them.
 * Label is usually an integer or a string or an array of
 * integers and/or strings.
 * If the string is a list of labels
 * separated by , then multiple situations
 * are started using those labels.
 * @param duration (int) total time (seconds) the overall situation 
 * should last.  You can put an array of durations -- one for each
 * situation if the label lists more than one situation and then
 * the overall time is the sum of the numbers.
 * -1 is a special duration.  It means that the labelled situation goes on
 * forever (and thus nothing after it in the array will ever go).
 * @param words is a list of replacements for #n in the text OR
 * a random number seed to use to choose words from random_words.
 * eg. ({ "#1", "frog", "#2", "honey" }) or 22
 *
 * @return handle useful for halting the changes later.  It is typically
 * a small positive integer.
 * @see shutdown_situation
 * @see add_situation
 * @see automate_situation
 * @see goto learning  search  situation  for working examples
 * @example
 * handle=change_situation( ({ "sew1","sew2" }), ({ 60,60 }) );
 * 
 * if (over) shutdown_situation( handle, ({ "sew1","sew2" }) );
 * @example
 * change_situation( ({ "background,sew1","background,sew2" }), 120 );
 * @example
 * change_situation( ({ "building_falling","rubble" }), ({ 120, -1 }) );
 * @example
 * add_situation( "boat1", 
 *   new(class situation,
 *     start_mess: "A little fishing boat comes up to the pier.  "
 *        "It has the name \"#1\" painted on the side.\n"
 *        "A fisherman jumps off the boat and ties its painter to a post.",
 *     chats: ({"The little boat bobs up and down next to the pier.",
 *              "A fisherman hops off the little boat." }),
 *     chat_rate: ({ 20, 30 }),
 *     extra_look: "There is a little boat here.",
 *     add_items: ({ ({ ({ "#1","little boat" }), 
 *       "There is little fishing boat tied up at the pier here.  "
 *       "The name \"#1\" is painted on the side."   }) }),
 *     end_mess: "The fishermen get back on board and "
 *        "the little boat moves on.",
 *     random_words: ({ ({ "Honey Toad", "Piker", "Bing" }) })
 *   )
 * );
 * 
 * change_situation( "boat1", 120, ({ "#1","Floating Egg" }) );
 */
varargs mixed change_situation( mixed label, mixed duration, mixed words ) {
  if (sitchanger) 
    return sitchanger->change_situation(label,duration,words,0);

  return 0;
} /* change_situation() */

/**
 * Automate starting and ending of a situation.
 * These situations can be invoked manually with start_situation.
 * The automated starting and ending is unaffected by the room 
 * unloading.  When the room reloads the situation will be 
 * restarted unless its duration is up.
 * You must include the file situations.h for the definitions
 * of the when masks.  The random seed needs to be set
 * consistently for the situations.  This is probably ok
 * unless your rooms are clones (see make_situation_seed). 
 * @param label (mixed) label of the situation to start 
 *  up.  If you pass an array such as ({ "frog1", "frog2" }) for the 
 * label then that set of situations are started one at
 * a time and the total duration is split evenly between them.
 * Label is usually an integer or a string or an array of
 * integers and/or strings.
 * If the string is a list of labels
 * separated by , then multiple situations
 * are started using those labels.
 * @param duration (int) total time (seconds) the overall situation 
 * should last.  You can put an array of durations -- one for each
 * situation if the label lists more than one situation and then
 * the overall time is the sum of the numbers.
 * -1 is a special duration.  It means that the situaton given that
 * duration is not part of the set but a special background or
 * default situation that occurs all the time except when
 * this automated situation is going.
 * @param when (int) a time of the day mask.  This limits when
 * the situation is allowed to occur.  The mask is composed of
 * the allowed hours in AM time ( 24 hours clock, (1<<hour) and 
 * combined with | (OR) ).   You can just use these
 * predefined masks and ignore how it works:
 *  WHEN_WEE_HOURS, WHEN_EARLY_MORNING, WHEN_LATE_MORNING, WHEN_AFTERNOON
 *  WHEN_EVENING, WHEN_LATENIGHT, WHEN_MIDDAY, WHEN_MORNING, 
 *  WHEN_EARLY_MORNING, WHEN_LATE_MORNING, WHEN_NIGHT, WHEN_DAY
 *  WHEN_ANY_TIME    
 * The masks are defined in /include/situations.h.
 * @param chance  (int) chance in 1000 of starting the situation
 *  This is tested every duration seconds.
 * @param category (optional) if you specify a cateory for the situation
 *                 then no situations with the same category will overlap.
 *                 category would usually be a string eg. "boats".
 * @see add_situation
 * @see start_situation
 * @see end_situation
 * @see situations.h
 * @see set_situation_changer
 * @see make_situation_seed
 * @see goto learning  search  situation  for working examples
 * @example
 * #include <situations.h>
 *
 *  automate_situation( "frog", 300, WHEN_ANY_TIME, 200 );
 *
 * This will automatically start the situation labelled "frog" 
 * at a random time that is any time of the day with a 200/1000
 * chance of it starting per 300 seconds.  It will last for
 * 300 seconds (5 minutes).  
 * @example
 *  automate_situation( ({"frog1,pond","frog2,pond"}), 240, 
 *                        WHEN_EVENING|WHEN_NIGHT, 300 );
 *
 * This will automatically start a situation that is a combination 
 * of "frog1" and "pond" followed by "frog2" and "pond".  They start
 * at a random time but only in the evening or at night.  
 * There will be a 300/1000 chance of 
 * it starting per 240 seconds.  Both the "frog1" and "frog2" 
 * situations will get half the total time (as there are two),  
 * 120 seconds each, for a total duration of 240 seconds (4 minutes).
 * @example
 *  automate_situation( ({"frog1,pond","frog2,pond"}), ({ 80,160 }), 
 *                        WHEN_EVENING|WHEN_NIGHT, 300 );
 * Same as previous example except the durations of the individual
 * parts are set: "frog1,pond" for 80 seconds followed by "frog2,pond"
 * for 160 seconds.  The total time is 80+160.
 */
varargs void automate_situation( mixed label, mixed duration, mixed when, 
    mixed chance, mixed category ) {
  if (sitchanger) 
    sitchanger->automate_situation(label,duration,when,chance,category);
} /* automate_situation() */

/**
 * Shuts down all current and pending situations.  It also turns off the
 * automated situation manager so no more are added.  It does not
 * destruct this object so all the add_situations are still loaded
 * and make be recommenced with automate_situation.  dest_me is
 * the appropriate call to permanently remove all situations.  The
 * call is passed to the situation changer object.  If none exists
 * then nothing happens.  The situation changer is created when
 * an add_situation call is performed. 
 * @see add_situation
 * @see automate_situation
 * @see change_situation
 */
void shutdown_all_situations() {
  if (sitchanger) sitchanger->shutdown_all_situations();
} /* shutdown_all_situations() */

/**
 * Shuts down a situation or set of situations initiated with
 * change_situation based on the call_out handle
 * returned by the call to change_situation.
 * @param callout call_out handle.  If 0 then the last
 * known handle is used.
 * @param label label or array of labels of situations to clean 
 * up with end_situation
 * @param the_room the room
 * @see automate_situation
 * @see change_situation
 */
void shutdown_situation(int call, mixed label) {
  if (sitchanger) sitchanger->shutdown_situation(call,label);
} /* shutdown_situation() */

/**
 * This method checks to see if the program is replaceable.
 * @return 1 if the program is not replaceable
 * @see set_not_replaceable()
 */
int query_not_replaceable() {
   return query_property(ROOM_NOT_REPLACE_PROGRAM_PROP);
} /* query_not_replaceable() */

/**
 * This method sets a property to make the program replaceable.  A program
 * will only be replaced if there is only a setup() function in the room.
 * A reset() or a create() will stop the room from being replace and in
 * fact any other function existing in there will stop it from being
 * replaced as well.
 * @pram replace 1 to make the room not replacable
 * @see query_not_replaceable()
 */
void set_not_replaceable(int replace) {
   add_property(ROOM_NOT_REPLACE_PROGRAM_PROP, replace);
} /* set_not_replaceable() */

/** @ignore yes */
mixed stats() {
   int i;
   mixed *stuff;
   stuff = ({ });
   for ( i = sizeof( dest_other ) - 2; i > -1; i -= 2 )
      stuff += ({
         ({ dest_other[ i ], dest_other[ i + 1 ][ ROOM_DEST ] })
      });
   if ( co_ord )
      stuff += ({
         ({ "co-ord x", co_ord[ 0 ] }),
         ({ "co-ord y", co_ord[ 1 ] }),
         ({ "co-ord z", co_ord[ 2 ] })
      });
   return light::stats() + property::stats() + stuff + ({
      ({ "short", short( 0 ) }),
      ({ "enchantment", query_enchant() }),
      ({ "background enchantment", background_enchant }),
      ({ "dynamic enchantment", dynamic_enchant }),
      ({ "enchantment time", enchant_time }),
      ({ "theft handler", theft_handler }),
   });
} /* stats() */

/**
 * This method sets the long description to display during the day time.
 * @param str the new day long description
 * @see query_day_long()
 * @see set_night_long()
 */
void set_day_long( string str ) {
  if(!variablelongs)
    variablelongs = allocate(2);

  variablelongs[DAY] = str;

  if(is_day == -1)
    is_day = (WEATHER_HANDLER->query_day() > 0);

  if(is_day == DAY)
    set_long(str);
}

/**
 * This method sets up the night long for the room.  This will be the
 * long description displayed at night in the room.
 * @param str the new night long description
 * @see set_day_long()
 * @see query_night_long()
 */
void set_night_long( string str ) {
  if(!variablelongs)
    variablelongs = allocate(2);

  variablelongs[NIGHT] = str;

  if(is_day == -1)
    is_day = (WEATHER_HANDLER->query_day() > 0);
  if(is_day == NIGHT)
    set_long(str);
}

/**
 * This method returns the long description of the room at night.  This is
 * used to print out different strings for rooms during the day and during
 * the night.
 * @return the night long for the room.
 */
string query_night_long() {

   if(variablelongs && strlen(variablelongs[NIGHT]))
     return variablelongs[NIGHT];
   
   return this_object()->query_long();
} /* query_night_long() */

/* Returns the long part of the description. */
private string return_long(mixed desc) {
  int ma;

  if(!pointerp(desc))
    return (string)desc;

  ma = member_array("long", desc);
  if(ma < 0)
    return "Error: No long found.";
  
  return (string)desc[ma+1];
}

/**
 * This method sets up an item which will only be displayed during the
 * day.  All of the standard add_item things are available with this
 * method.
 * @see /std/room->add_item()
 * @see add_night_item()
 */
varargs int add_day_item(mixed shorts, mixed desc, mixed no_plural) {
  string the_item;
  
  if(pointerp(shorts))
    the_item = shorts[0];
  else
    the_item = shorts;

  if(!variableitems)
    variableitems = ({ ({ }), ({ }) });
  
  variableitems[DAY] += ({ the_item, return_long(desc) });

  if(is_day == -1)
    is_day = (WEATHER_HANDLER->query_day() > 0);
  if(is_day == DAY) 
    return add_item(shorts, desc, no_plural);
  
  return 1;
}

/**
 * This method sets up an item which will only be displayed during the
 * night.  All of the standard add_item things are available with this
 * method.
 * @see /std/room->add_item()
 * @see add_day_item()
 */
varargs int add_night_item(mixed shorts, mixed desc, mixed no_plural) {
  string the_item;
  
  if(pointerp(shorts))
    the_item = shorts[0];
  else
    the_item = shorts;

  if(!variableitems)
    variableitems = ({ ({ }), ({ }) });

  variableitems[NIGHT] += ({ the_item, return_long( desc ) });
  
  if(is_day == -1)
    is_day = (WEATHER_HANDLER->query_day() > 0);
  if(is_day == NIGHT) 
    return add_item(shorts, desc, no_plural);

  return 1;
}

/**
 * This method sets up chats for when the room is in the day cycle.
 * @param args the chatter arguements
 * @see room_night_chat()
 * @see /std/room->room_chat()
 */
void room_day_chat(mixed *args) {
  if(!variablechats)
    variablechats = allocate(2);
  
  variablechats[DAY] = args;
  
  if(is_day == -1)
    is_day = (WEATHER_HANDLER->query_day() > 0);
  if(is_day == DAY)
    room_chat(args);
}

/**
 * This method sets up chats for when the room is in the night cycle.
 * @param args the chatter arguements
 * @see room_day_chat()
 * @see /std/room->room_chat()
 */
void room_night_chat( mixed *args ) {
  if(!variablechats)
    variablechats = allocate(2);
  
  variablechats[NIGHT] = args;
  
  if(is_day == -1)
    is_day = (WEATHER_HANDLER->query_day() > 0);
  if(is_day == NIGHT)
    room_chat(args);
}

/** @ignore yes */
string query_help_file_directory() {
  return ROOM_HELP_FILE_DIR;
} /* query_help_file_directory() */
