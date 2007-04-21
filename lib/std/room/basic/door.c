/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: door.c,v 1.28 2003/05/22 17:57:31 ceres Exp $
 *
 * Basic door object used by rooms. Typically there is no need to use any of
 * the methods in this object since they are accessible through modify_exit.
 *
 * @author pinkfish
 *
 */
#include <door.h>
#include <room.h>
#include <parse_command.h>
#include <dirs.h>

#define BEVERB ({ " is ", " are " })
#define PRONOUN ({ "it", "them" })
#define CLOSED        2
#define CLOSED_LOCKED 3

inherit "/std/basic/enchant";
inherit "/std/basic/hide_invis";
inherit "/std/basic/property";
inherit "/std/basic/close_lock";
inherit "/std/basic/trap";
inherit "/std/basic/id_match";

private int how_many;
private int one_way;
// pk only pick, no one can pick.
private string _lock_owner;
private string shrt;
private string lng;
private string my_id;
private string other_id;
private string *name;
private string *adjs;
private string *plu;
private string dest;
private string _door_name;
private object key;
private object my_room;

/** 
 * @ignore yes
 */
void create() {
   hide_invis::create();
   property::create();
   close_lock::create();
   name = ({ "door" });
   lng = "It's just a door.\n";
   shrt = "door";
   adjs = ({ });
   plu = ({ });
   add_property( "determinate", "the " );
   set_unlocked();
} /* create() */ 

/**
 * This method returns how many doors are stored in this particular
 * door object.  This is used for double doors and things like that
 * to make the plurals work correctly.
 * @return an integer value representing the number of doors.
 * @see set_how_many() 
 */
int query_how_many() { 
    return how_many; 
} /* query_how_many() */ 

/** 
 * This method sets how many doors there are so that plurals
 * and everything are set correctly.  It also flags the door
 * with the "group object" property.
 * @param number the number of doors this door should represent.
 * @see query_how_many()
 */
void set_how_many( int number ) {
  // make the doors act as plural if there's more than one of them
  if(number)
    add_property("group object", 1);
  else
    remove_property("group object");
  how_many = number;
} /* set_how_many() */ 

/**
 * This method is used to determine if a door has been flagged as being
 * 'one way' or not. 
 * @return an integer value which returns 1 if the door is one way,
 * 0 if it is not. 
 * @see set_one_way()
 */
int query_one_way() { 
    return one_way; 
} /* query_one_way() */ 

/**
 * This method is used to set whether or not the door is one way.
 * A one way door won't trigger a "broken door" message when there's
 * no exit leading back.
 * @param number pass 1 if the door is one way, or 0 if it isn't.
 */ 
void set_one_way( int number ) { 
    one_way = number; 
} /* set_one_way() */ 

/**
 * This method returns the short of the door.  This is normally a string
 * like "east door".
 * @return a string containing the short for the door.
 * @see set_short() 
 */ 
string query_short() {  
    return shrt; 
} /* query_short() */

/** 
 * This method sets the short for the door.  As well as setting the short,
 * it calculates the name, adjectives and plurals for the door.  
 * @param words the short to set for the door object. 
 * @see query_short() 
 */ 
void set_short( string words ) {
   string *bits;
   shrt = words;
   bits = explode( words, " " );

   // Now split apart any hyphenated words. 
   // And add them to the original. 

   bits += implode( map( bits, (: explode( $1, "-" ) :) ), (: $1 + $2 :) );
   bits = uniq_array( bits );

   name += ({ bits[ sizeof( bits ) - 1 ] });
   adjs += bits[ 0 .. sizeof( bits ) - 2 ];
   plu += ({ pluralize( bits[ sizeof( bits ) - 1 ] ) });
} /* set_short() */

/** 
 * This returns the name of the door.  Note that this is different to the
 * actual 'name' of the door.  It is normally an exciting and original
 * value of 'door'. 
 * @return a string representing the door name.  
 */ 
string query_door_name() { 
    return _door_name; 
} /* query_door_name() */ 

/** 
 * This method sets the name of the door.  Note that this is different
 * to the actual visible name or short of the door.
 * @param word the new door name
 * @see query_door_name()
 * @see set_short()
 */ 
void set_door_name( string word ) { 
    _door_name = word; 
} /* set_door_name() */ 

/** 
 * This method sets the owner of the lock.   This value is used in
 * pick_lock() to determine whether or not various PK checks etc
 * are passed.
 * @param owner the name of the player who owns the lock.
 */ 
void set_lock_owner(string owner) { 
    _lock_owner = owner; 
} /* set_lock_owner() */ 

/** 
 * This method is used to determine which player owns the lock
 * on this door.
 * @return a string containing the name of the player who owns the
 * lock.
 * @see set_lock_owner() 
 */ 
mixed query_lock_owner() { 
    return _lock_owner; 
} /* query_lock_owner() */ 

string query_long() { return lng; }
void set_long( string word ) {lng = word;}

string query_my_id() { return my_id; }
void set_my_id( string word ) { my_id = word; }

string query_other_id() { return other_id; }
void set_other_id( string word ) { other_id = word; }

string *query_name() { return name; }
string *query_adjs() { return adjs; }
string *query_plu() { return plu; }

string query_dest() { return dest; }
void set_dest( string word ) { dest = word; }

object query_my_room() { return my_room; }

/** 
 * @ignore yes
 */ 
void init() {
   add_command( "knock", "on <direct:object>" );
   close_lock::init();
} /* init() */ 

string query_determinate() { return "the "; }
string short() { return query_short(); }
string a_short() { return "$a_short:"+ file_name( this_object() ) +"$"; }
string one_short() { return "$one_short:"+ file_name( this_object() ) +"$"; }
string poss_short() { return "$poss_short:"+ file_name( this_object() ) +"$"; }
string the_short() { return "$the_short:"+ file_name( this_object() ) +"$"; }
string pretty_short() { return query_short(); }
string query_plural() { return pluralize( query_short() ); }
string pretty_plural() { return pluralize( query_short() ); }

string long() {
  return lng + ( ( this_player()->query_see_octarine() ) ? enchant_string() :
                 "" ) + long_status();
}

int drop() { return 1; }
int get() { return 1; }

/** 
 * This method is called by /std/room/basic_room.c in query_door()
 * to create the door. 
 * @param word the name of the door being created
 * @param mine the object that owns the door
 * @param his the destination for the door
 * @param args dest_other information from door.  This is mainly
 * the modify_exit() parameters.
 * @param type the type of the door, eg, door, window, etc. 
 */ 
void setup_door( string word, object mine, string his, mixed *args,
                 string type) {
   string *bits;

   my_id = word;
   my_room = mine;
   dest = his;

   switch(type) {
   case "window":
     if(word != "window")
       shrt = word + " window";
     else
       shrt = word;
     
     lng = "It's just the " + shrt + ".\n";
     name = ({ shrt, "window" });
     plu = ({ "windows" });
     break;
     
   default:
     if ( word[ 0 .. 5 ] == "enter " ) {
       if(SHORTEN[word[6..100]])
         name += ({ SHORTEN[word[6..100]] });
       shrt = word[ 6 .. 100 ] +" door";
       lng = "It's just the " + shrt + ".\n";
       name += ({ shrt });
       if(!args[D_SECRET ])
         plu = ({ "doors" });
     } else {
       if(SHORTEN[word])
         name += ({ SHORTEN[word] });
       shrt = word +" door";
       lng = "It's just the " + shrt + ".\n";
       name += ({ shrt });
       if(!args[D_SECRET ])
         plu = ({ "doors" });
     }
     break;
   }

   bits = explode( word, " " );
   name += ({ bits[ sizeof( bits ) - 1 ] });
   adjs +=  bits;
   plu += ({ pluralize( bits[ sizeof( bits ) - 1 ] ) });

   (!args[ D_CLOSED ]) ? set_closed() : set_open();
   (args[ D_LOCKED ]) ? set_locked() : set_unlocked();

   set_key(args[ D_KEY ] );
   set_difficulty(args[ D_DIFFICULTY ] );
   if ( args[ D_SECRET ] )
      add_hide_invis( "secret", 0, 200, 0 );

   if(sizeof(args) >= D_TRANSPARENT && args[D_TRANSPARENT])
     set_transparent();

   set_door_name(args[D_NAME]);
}

void add_alias( string word ) {
   string *bits;

   bits = explode( word, " " );
   name += ({ bits[ sizeof( bits ) - 1 ] });
   adjs +=  bits[ 0 .. sizeof( bits ) - 2 ];
   plu += ({ pluralize( bits[ sizeof( bits ) - 1 ] ) });
} /* add_alias() */

/** @ignore yes
 * I believe this function is used to forcibly load & setup the other side
 * of a door. I'm not entirely sure if all of this is needed but debugging
 * it is tricky. -- Ceres
 */
int force_other() {
  string door_name, direc;
  
  if(!find_object(dest)) {
    if(!load_object(dest))
      return 0;
    dest->query_door( my_room, _door_name );
  }

  // No ID then we need to find our ID
  if(!other_id) {
    door_name = my_room->call_door(my_id, "query_door_name");
    direc = dest->query_door(my_room, door_name);
    my_room->modify_exit( my_id, ({ "other", direc }) );
  }
  
  // Still no id? Then we're in trouble.
  if(!other_id)
    return 0;
  
  return 1;
}

int do_open() {
  if(!::do_open())
    return 0;

  if(!force_other() && !one_way) {
    set_closed();
    return 0;
  }

  /*
   * If the door is now open, it's outdoors and this is not a move (i.e. the
   * room handler is not involved), the wind could blow it shut.
   */
  if(((string)my_room->query_property( "location" ) == "outside") &&
     (member_array(find_object(ROOM_HANDLER), previous_object(-1)) == -1))
    ROOM_HANDLER->add_door( this_object() );
  
  if(!one_way && find_object(dest)) {
    dest->modify_exit(other_id, ({ "closed", 0 }));
    if(this_player())
      dest->tell_door(other_id, (string)this_player()->a_short() +
                      " opens the $D.\n", 0);
    else
      dest->tell_door(other_id, "Someone opens the $D.\n", 0);
  }
  
  return 1;
}

int do_close() {
  if(!::do_close())
    return 0;

  if(!force_other() && !one_way) {
    set_open();
    return 0;
  }

  if(!one_way && find_object(dest)) {
    if(query_autolock()) {
      dest->modify_exit( other_id, ({ "closed", 1, "locked", 1 }) );
      dest->tell_door( other_id, "Someone closes the $D which "
                       "lock$s as $p close$s.\n", 0 );
    } else {
      dest->modify_exit( other_id, ({ "closed", 1 }) );
      dest->tell_door( other_id, "Someone closes the $D.\n", 0 );
    }
  }
  return 1;
}

int do_lock(object *obs) {
  if(!::do_lock(obs))
    return 0;

  if(!force_other() && !one_way) {
    set_unlocked();
    return 0;
  }

  if(!one_way && find_object(dest)) {
    dest->modify_exit(other_id, ({ "locked", 1 }));
    dest->tell_door(other_id, "Someone locks the $D.\n", 0);
  }
  
  return 1;
}

int do_unlock(object *obs) {
  if(!::do_unlock(obs))
    return 0;

  if(!force_other() && !one_way) {
    set_locked();
    return 0;
  }

  if(!one_way && find_object(dest)) {
    dest->modify_exit(other_id, ({ "locked", 0 }));
    dest->tell_door(other_id, "Someone unlocks the $D.\n", 0);
  }
  
  return 1;
}

int do_knock() {
  if(this_player()->query_property("dead"))
    return 0;
  
  if ( !other_id && !one_way ) {
    ROOM_HANDLER->check_door( ({ my_room, my_id }) );
    if ( !other_id )
      return 0;
  }
  if ( one_way )
    return 1;

  if(query_transparent())
    dest->tell_door(other_id, this_player()->a_short() +
                    " knocks on the $D.\n", 0);
  else
    dest->tell_door( other_id, "Someone knocks on the $D.\n", 0 );
  event(find_object(dest), "door_knock", this_player(), other_id);
  return 1;
}

int pick_unlock(object thing) {
  if (stringp(_lock_owner)) {
     // Check and see if they are pk, get the owner from our room
     // or the other room.
     if (pk_check(_lock_owner, thing, 1)) {
        return 0;
     }
  }

  if(!::pick_unlock(thing)) {
    return 0;
  }

  if(!force_other() && !one_way ) {
    set_locked();
    return 0;
  }

  if(!one_way && find_object(dest)) {
    dest->modify_exit( other_id, ({ "locked", 0 }) );
    dest->tell_door( other_id, "Someone unlocks the $D.\n", 0 );
  }
  return 1;
}

int pick_lock( object thing ) {
  if (stringp(_lock_owner)) {
     // Check and see if they are pk, get the owner from our room
     // or the other room.
     if (pk_check(_lock_owner, thing, 1)) {
        return 0;
     }
  }

  if(!::pick_lock(thing))
    return 0;
  
  if(!force_other() && !one_way ) {
    set_unlocked();
    return 0;
  }
  
  if(!one_way && find_object(dest)) {
    dest->modify_exit( other_id, ({ "locked", 1 }) );
    dest->tell_door( other_id, "Someone locks the $D.\n", 0 );
  }
  return 1;
}

string *parse_command_id_list() {
  return (this_player() ? name + ({ this_player()->find_rel(my_id) }) : name);
}

string *parse_command_adjectiv_id_list() { return adjs; }
string *parse_command_plural_id_list() { return plu; }

object query_parse_id( mixed *arr ) {
   if (arr[P_THING] == 0) {
      return this_object();
   }
   if (arr[P_THING] < 0) { /* specific object case */
      arr[P_THING]++;
      if (arr[P_THING] != 0) {
         return 0;
      }
      arr[P_THING] = -10321;
      return this_object();
   }
   arr[P_THING]--; /* lots of objects case */
   if (arr[P_THING] != 0) {
      return this_object();
   }
   arr[P_THING] = -10101;
   return this_object();
}

mixed parse_match_object( string* input,
                          object viewer,
                          class obj_match_context context )
{
   if (input[<1] == "all") {
      return 0;
   }
   return ::parse_match_object(input, viewer, context);
} /* parse_match_object() */

void dest_me() {
  //  tell_creator("ceres", "Door being destructed: %O %O %O\n",
  //my_room, call_stack(1), call_stack(2));
  destruct( this_object() );
}
void dwep() { destruct( this_object() ); }
void go_away() { call_out("dest_me", 0); }
int move() { return 1; }

int moving_open( object thing ) {
  if(!::do_open())
    return 0;

  if(!force_other() && !one_way) {
    set_closed();
    return 0;
  }
  
  // This is now handled in the sneaking shadow.
  //thing->remove_hide_invis("sneaking");
  
  if(!one_way && find_object(dest)) {
    dest->modify_exit(other_id, ({ "closed", 0 }));
    dest->tell_door(other_id, (string)thing->a_short() + " opens the $D.\n", 0);
  }

  tell_object( thing, "You open the "+ shrt +".\n" );
  tell_room( my_room, (string)thing->the_short() +
             " opens the "+ shrt +".\n", thing );
  return 1;
}

int moving_close( object thing ) {
  if(!::do_close())
    return 0;
  
  if(!force_other() && !one_way) {
    set_open();
    return 0;
  }

  if(!one_way && find_object(dest))
    if(query_autolock()) {
      dest->modify_exit( other_id, ({ "closed", 1, "locked", 1 }) );
      dest->tell_door(other_id, (string)thing->the_short() +
                      " closes the $D which lock$s as $p close$s.\n", thing );
    } else {
      dest->modify_exit( other_id, ({ "closed", 1 }) );
      if ( thing->short() )
        dest->tell_door( other_id, capitalize( (string)thing->the_short() ) +
                         " closes the $D.\n", thing );
    }

  if(query_autolock()) {
    tell_object( thing, "You close the "+
                 (string)dest->call_door( other_id, "query_short" ) +
                 " which locks as it closes.\n" );
    tell_room( my_room, (string)thing->the_short() +
               " closes the "+ shrt +" which locks as it closes.\n", thing );
  } else {
    tell_object( thing, "You close the "+
                 (string)dest->call_door( other_id, "query_short" ) +".\n" );
    tell_room( my_room, (string)thing->the_short() +
               " closes the "+ shrt +".\n", thing );
  }

  return 1;
}

int moving_unlock(object thing) {
  if(!::do_unlock(all_inventory(thing)))
    return 0;

  if(!one_way && find_object(dest))
    dest->modify_exit(other_id, ({ "locked", 0 }));

  tell_object(thing, "You unlock the "+ shrt +".\n");
  if (thing->short())
    tell_room( my_room, capitalize( (string)thing->the_short() ) +
               " unlocks the "+ shrt +".\n", thing );
  return 1;
}

int moving_lock(object thing) {
  if(!::do_lock(all_inventory(thing)))
    return 0;

  if(!one_way && find_object(dest))
    dest->modify_exit(other_id, ({ "locked", 1 }));
  
  tell_object( thing, "You lock the "+
               (string)dest->call_door( other_id, "query_short" ) +".\n" );
  if (thing->short())
    dest->tell_door( other_id, capitalize( (string)thing->the_short() ) +
                     " locks the $D.\n", thing );
  return 1;
}

void tell_door(string words, object thing) {
  tell_room(my_room, replace( words, ({
    "$D", shrt, "$p", PRONOUN[how_many], "$s", ({ "s", "" })[how_many] })),
            ({ thing }));
}

/** @ignore yes */ 
mixed *stats() {
  return property::stats() + close_lock::stats() +
         ({ ({ "lock owner", _lock_owner }),
            ({ "other id", other_id }),
            ({ "dest", dest }) });
} /* stats() */ 

// Mask long status from close_lock to accurated describe double doors.
string long_status() {
   string str;
   // If it is stuck in one state don't bother telling us the state.
   if (!query_stuck()) {
      if(query_how_many() > 0)
      {
        str = "They are";
      } 
      else 
      {
        str = "It is";
      }
      switch (query_closed_locked_status()) {
         case CLOSED_LOCKED :
            return str + " closed and locked.\n";
         case CLOSED :
            return str + " closed.\n";
         default :
            return str + " open.\n";
      }
   } else {
      return "";
   }
}
