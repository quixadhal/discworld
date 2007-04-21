/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: room_handler.c,v 1.54 2003/02/13 23:33:56 ceres Exp $
 */
/**
 * This file contains all the bits needed to handle rooms, plus the code to
 * handle following when moving.
 * @author Pinkfish
 * @revision Deutha Who knows
 * Severely changed, move the follow code in here from the living object
 * @see /std/room/basic_room.c
 */
#include <climate.h>
#include <living.h>
#include <move_failures.h>
#include <player.h>
#include <room.h>
#include <weather.h>
#include <position.h>

#define CHATSIZE 80
#define DOORSIZE 60 

mapping exit_types;
mapping door_types;
mapping opposite;
mixed *chatters, *doors;

void add_door( object thing );

/* ({ mess, obv, size, func }) */
void create() {
   seteuid( (string)"/secure/master"->
           creator_file( file_name( this_object() ) ) );
   exit_types = ([
"standard" :({ 0, 1,   400, 0 }),
"corridor" :({ 0, 1,   250, 0 }),
"plain"    :({ 0, 1, 10000, 0 }), /* very large */
"door"     :({ 0, 1,   300, 0 }),
"stair"    :({ 0, 1,   300, 0 }), /* going up? */
"hidden"   :({ 0, 0,   300, 0 }), /* hidden non door exit */
"secret"   :({ 0, 0,   300, 0 }), /* secret door */
"gate"     :({ 0, 1,   450, 0 }),
"road"     :({ 0, 1,  1300, 0 }),
"path"     :({ 0, 1,   800, 0 }),
"window"   :({ "$N climb$s through a window.\n", 0, 75, 0 }),
]);
   door_types = ([
"door"      : ({ 0, 0, "generic_key", 2, 0, 0, "door" }),
"secret"    : ({ 0, 0, "generic_key", 3, 1, 0, "door" }),
"gate"      : ({ 1, 0, "generic_key", 1, 0, 1, "door" }),
"window"    : ({ 1, 0, "generic_key", 1, 0, 1, "window" }),
]);
   opposite = ([
"north" : ({ 0, "$R$[the ]+south$R$" }),
"south" : ({ 0, "$R$[the ]+north$R$" }),
"east" : ({ 0, "$R$[the ]+west$R$" }),
"west" : ({ 0, "$R$[the ]+east$R$" }),
"northeast" : ({ 0, "$R$[the ]+southwest$R$" }),
"southwest" : ({ 0, "$R$[the ]+northeast$R$" }),
"southeast" : ({ 0, "$R$[the ]+northwest$R$" }),
"northwest" : ({ 0, "$R$[the ]+southeast$R$" }),
"up":({0,"below"}), "down":({0,"above"}),
"out":({0,"inside"}), "in":({0,"outside"}),
"exit":({0,"inside"}), "enter":({0,"outside"}),
"hubward":({0,"rimward"}), "rimward":({0,"hubward"}),
"turnwise":({0,"widdershins"}),
"widdershins":({0,"turnwise"}) ]);
   chatters = allocate( CHATSIZE );
   doors = allocate( DOORSIZE );
   call_out( "housekeeping", 4 );
} /* create() */

/**
 * This method returns the opposite direction to this exit.  This should
 * only be used for printing, since it is not a useful
 * real name.
 * @param dir the direction to get the opposite of
 * @return the opposite direction
 */
string query_opposite_direction(string dir) {
   if (opposite[dir]) {
      return opposite[dir][1];
   }
   return 0;
}

/**
 * This method returns the current list of rooms that are enabled for
 * chatting.
 * @return the current chatters
 */
mixed *query_chatters() { return chatters; }

/**
 * This method returns the current list of doors handled by the room
 * handler.
 * @return the current array of doors
 */
mixed *query_doors() { return doors; }

/**
 * This method adds an exit type to the current list of available exit types.
 * This is used when the room handler is setup to add all the used exit
 * types.
 * @param type the name of the exit type
 * @param message the message to display when going through the exit
 * @param obvious if the exit is obvious or not
 * @param size the size of the exit (used for heigh restrictions)
 * @param func the function to call when using the exit
 * @return 1 if successfuly added, 0 if not
 * @see remove_exit_type()
 */
int add_exit_type(string type, mixed message, mixed obvious,
                            int size, mixed func) {
  if (exit_types[type]) {
    return 0;
  }
  exit_types[type] = ({ message, obvious, size, func });
  return 1;
} /* add_exit_type() */

/**
 * This method remove the named exit from the type list.
 * @param type the name of the exit type to remove
 * @return always returns 1
 * @see add_exit_type()
 */
int remove_exit_type(string type) {
   map_delete(exit_types, type);
   return 1;
} /* remove_exit_type() */

/**
 * This method returns information about the door type, the door has
 * extra information associated with it than the standard exit type.
 * This function does a double job of trying to find the corresponding
 * door on the other side of the room.
 * @param type the type of the door
 * @param direc the direction the door points
 * @param dest the destination of the door
 * @return the door type array of information
 */
mixed *query_door_type(string type, string direc, string dest) {
   if (!door_types[type]) {
      return 0;
   }
   /* If there isnt a door on the other side.  We don't join. */
   call_out( "check_door", 1, ({ previous_object(), direc }) );
   return door_types[type];
} /* query_door_type() */

/**
 * This method checks to see if the door exists or not.
 * It is passed in the room we are going from and the direction the
 * exit faces in the array.<br>
 * <pre>({ room_from, direction })</pre><br>
 * This is the function which generates those door xx not found messages.
 * @param args the arguements passed into the function
 * @see query_door_type()
 */
void check_door( mixed args ) {
   string direc, dest;
   string door_name;
   
   if ( !args[ 0 ] ) {
      return;
   }
   args[ 0 ]->set_destination( args[ 1 ] );
   dest = (string)args[ 0 ]->query_destination( args[ 1 ] );
   if ( !dest ) {
      tell_room( args[ 0 ], "Error: "+ args[ 1 ] +
            " is no longer an exit.\n" );
      return;
   }
   if ( !find_object( dest ) ) {
      return;
   }

   door_name = args[0]->call_door(args[1], "query_door_name");
   direc = (string)dest->query_door( args[ 0 ], door_name );

   if ( !direc && 
       !args[ 0 ]->call_door( args[ 1 ], "query_one_way" ) ) {
      tell_room( args[ 0 ], "Error: "+ dest +
                "does not have a door coming back here.\n" );
      return;
   }
   args[ 0 ]->modify_exit( args[ 1 ], ({ "other", direc }) );

   /*
    * This makes sure that whatever the states of the two sides, they'll
    * both end up the same.  Think about it...
    */
   args[ 0 ]->modify_exit( args[ 1 ], ({
      "closed", (int)dest->call_door( direc, "query_closed" ),
      "locked", (int)dest->call_door( direc, "query_locked" ) }) );
   if ( !args[ 0 ]->call_door( args[ 1 ], "query_closed" ) &&
       ( (string)args[ 0 ]->query_property( "location" ) == "outside" ) ) {
      add_door( (object)args[ 0 ]->query_door_control( args[ 1 ], door_name ) );
   }
} /* check_door() */

/**
 * This method returns the information associated with the exit type.
 * @param type the exit type to query
 * @param dir the direction the type information is for
 * @return a huge amount of info as specified above
 */
mixed *query_exit_type(string type, string dir) {
  mixed s;

   if (!(s = opposite[dir])) {
      s = ({ 0, "elsewhere" });
   }
   if (!exit_types[type]) {
      return exit_types["standard"] + ({ s, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
   }
   return exit_types[ type ] + ({ s, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
} /* query_exit_type() */

/**
 * THis is the code that actually moves the thing around the place.
 * It handles all the weirdness involved with dragging things and other
 * such stuff.
 * @param thing what is being moved
 * @param dir the direction we are going
 * @param dest the destionation room
 * @param exit the exit name
 * @param enter the enter name
 * @param move the string to tell the object when it moves
 * @return 1 on success, 0 on failure
 */
int move_thing( object thing, string dir, string dest, mixed exit,
                mixed enter, string move ) {
  int ret;
  string arrive, leave;
  object dragging;
  
  dragging = (object)thing->query_dragging();

  if (dragging && environment(dragging) != environment(thing)) {
    thing->reset_dragging();
    dragging = 0;
  }

  if ( ( exit != "none" ) || objectp( dragging ) ) {
    if ( stringp( enter ) )
      enter = ({ 1, enter });
    else if ( functionp(enter) )
      enter = ({ 1, evaluate(enter, thing) });
    else if ( !pointerp( enter ) )
      enter = ({ 0, "somewhere" });
    
    switch ( enter[ 0 ] ) {
    case 0 :
      arrive = replace( (string)thing->query_msgin(), 
                        ({"$F", enter[ 1 ], 
                          "$r", thing->query_pronoun() }) );
      break;
    default :
      arrive = enter[ 1 ];
    }

    if ( stringp( exit) )
      leave = exit;
    else if ( functionp( exit ) )
      leave = evaluate(exit, thing);
    else if ( pointerp( exit ) )
      leave = exit[ 0 ];
    else
      leave = (string)thing->query_msgout();

    leave = replace( leave, ({"$T", "$R$-"+ dir +"$R$",
                              "$r", thing->query_pronoun() }) );
  }

  /* Check position... */
  thing->return_to_default_position(1);
  
  if ( arrive || objectp( dragging ) ) {
    thing->remove_hide_invis( "hiding" );
    if ( stringp( arrive ) && objectp( dragging ) ) {
      arrive += "\n$C$"+ (string)thing->query_pronoun() +" drags "+
        (string)dragging->a_short() +" in behind "+
        (string)thing->query_objective() +".";
    }

    if ( stringp( leave ) && objectp( dragging ) ) {
      leave += "\n$C$"+ (string)thing->query_pronoun() +" drags "+
        (string)dragging->the_short() +" away behind "+
        (string)thing->query_objective() +".";
    }

    if ( stringp( move ) ) {
      tell_object( thing, move );
    }

    ret = (int)thing->move( dest, arrive, leave );
    if ( ( ret == MOVE_OK ) && objectp( dragging ) ) {
      if(dragging->move(environment(thing)) == MOVE_OK) {
        tell_object( thing, "You drag "+
                     (string)dragging->the_short() +" behind you.\n" );
        thing->adjust_time_left( -DEFAULT_TIME );
        //dragging->adjust_cond(-100);
      } else {
        tell_object( thing, "You fail to drag "+
                     (string)dragging->the_short() +" behind you.\n" );
      }
    }
  } else {
    if ( stringp( move ) ) {
      tell_object( thing, move );
    }
    ret = (int)thing->move( dest );
  }

  if ( ret == MOVE_OK ) {
    thing->adjust_time_left( -DEFAULT_TIME );
    return 1;
  }

  return 0;
} /* move_thing() */

/** @ignore yes
 * This function performs the door checks for exit_move(). It is called
 * for the object moving and each of its followers.  It returns 1 if
 * the player can move or 0 if not.
 */
int exit_move_door_checks(object thing, mixed closed) {

  closed->force_other();
  
  if(closed->query_open())
    return 1;
  
  if(thing->query_property( "demon" ) || thing->query_property("dead")) {
    tell_object(thing, "You ghost through "+(string)closed->the_short()+
                ".\n");
    return 1;
  }

  // Too small to open the door or unlock it!
  if(thing->query_weight() < 300)
    return 0;
  
  if(closed->query_locked() && !closed->moving_unlock(thing)) {
    // It is locked and invisible...
    if(!closed->query_visible(thing)) {
      return 0;
    }
    
    tell_object(thing, (string)closed->the_short() +
                ({ " is ", " are " })[(int)closed->query_how_many()] +
                "locked.\n");
    return notify_fail("");
  }

  if(!closed->moving_open(thing)) {
    return 0;
  }

  return 1;
}

/** @ignore yes
 * This function performs the function checks for exit_move(). It returns 1 if
 * the player can move or 0 if not.
 */
int exit_move_func_checks(string verb, string special, object thing,
                          mixed func, object place) {
  if(stringp(func))
    return call_other(place, func, verb, thing, special);

  if(functionp(func))
    return evaluate(func, verb, thing, special);

  if(pointerp(func) && sizeof(func) > 1 && func[0] && func[1])
    return call_other(func[0], func[1], verb, thing, special);
  
  return 1;
}

/**
 * This is the main code for moving someone.  The move_thing code above
 * should not be called directly.  This code handlers all the followers
 * and any other things that need to be handled.
 * @param verb the movement verb
 * @param extra extra information
 * @param special special informaiton
 * @param thing the thing to move
 * @return 1 on success, 0 on failure
 */
int exit_move( string verb, string extra, mixed special, object thing ) {
   string leave;
   // Place is where we are right now.
   object place;
   object follower;
   object *okay;
   mixed closed;
   int locked;
   mixed func;
   mixed *dest_other;
   object *all_followers;
   object *tmp_followers;
   object *more_followers;

   /* Find the exit infomation. */
   place = environment( thing );
   verb = (string)place->expand_alias( verb );
   place->set_destination( verb );
   
   /* This checks that the destination is correctly set. */
   dest_other = (mixed *)place->query_dest_other( verb );
   if (!pointerp(dest_other))
      return 0;

   if(thing->cannot_walk( verb, dest_other ))
      return notify_fail( "" );
   
   /* This checks that the door exists if there should be one. */
   closed = (object)place->query_door_control( verb );

   // Force the other side of the door to load. Once this is done make
   // sure our door hasn't been destructed & replaced by another. I know
   // that sounds unlikely but it does happen sometimes.
   if(objectp(closed)) {
     closed->force_other();
     if(!closed)
       closed = (object)place->query_door_control( verb );
   }

   /* This checks to see if this is an NPC trying
    * to go through a "no follow" exit. */
   if (!interactive(thing) && living(thing)
         && sizeof(dest_other) > ROOM_NPC_STOP && dest_other[ROOM_NPC_STOP])
      return 0;

   if (objectp(closed)) {
      locked = closed->query_locked();
   }
   /* If the door is already open we don't need the closed thing coz
      we aren't going to do anything to the door. */
   if(objectp(closed) && closed->query_open()) {
     closed = 0;
   }

   if(objectp(closed) && !exit_move_door_checks(thing, closed)) {
     return 0;
   }
   
   //To allow the room to override exits from a central location.
   if ( place->block_exit_move( verb, thing ) ) {
     return notify_fail( "" );
   }

   /* Check exit functions. */
   func = dest_other[ROOM_FUNC];
   if(func && !thing->query_property( "demon" ) &&
      !exit_move_func_checks(verb, special, thing, func, place))
     return 0;
   
   if ( place->query_relative( verb ) )
     leave = (string)thing->find_rel( verb, 0 );
   else
     leave = verb;

   /* Check height. */
   if((int)thing->query_height() > dest_other[ ROOM_SIZE ] &&
      !(thing->query_crawling() &&
        (int)thing->query_height()/3 <= dest_other[ ROOM_SIZE ])) {
     tell_object( thing, "You are too tall to go that way.\n" );
     return notify_fail( "" );
   }

   /* Now actually move. */
   if ( !stringp( special ) ) {
      special = dest_other[ ROOM_EXIT ];
   }

   if ( !move_thing( thing, verb, dest_other[ ROOM_DEST ], special,
                     dest_other[ ROOM_ENTER ], dest_other[ ROOM_MESS ] ) ) {
     return 0;
   }

   thing->return_to_default_position(1);

   okay = ({ });
   if (place) {
     // Get all the followers of the followers.
     all_followers = thing->query_followers();
     more_followers = all_followers;
     do {
       tmp_followers = ({ });
       foreach (follower in more_followers) {
         //
         // Make sure that we only follow lists of people that are actually
         // here.
         //
         if (follower &&
             environment( follower ) == place ) {
           // Make sure we not end up with duplicates in this array.
           tmp_followers |= follower->query_followers();
         }
       }
       // Make sure we do not get repeated followers.
       more_followers = tmp_followers - all_followers;
       all_followers |= tmp_followers;
     } while (sizeof(more_followers));
     
     // Move all those people following us too!
     foreach ( follower in all_followers) {

       if ( !objectp( follower ) ) {
         thing->remove_follower( follower );
         continue;
       }
       
       /* Make sure they are in the start room and
        * if they are a user they are interactive and
        * the person they are following is visible and
        * they aren't passed out and can walk.
        */
       if(environment(follower) != place ||
          (userp(follower) && !interactive(follower)) ||
          (!thing->query_visible(follower) || (special == "none")) ||
          follower->query_property( PASSED_OUT) ||
          follower->cannot_walk( verb, dest_other ))
         continue;

       // do the door checks
       if(objectp(closed) && !exit_move_door_checks(follower, closed))
         continue;

       // do the function checks.
       if ( func && !follower->query_property( "demon" ) &&
            !exit_move_func_checks(verb, special, follower, func, place))
         continue;

       // do the NPC check.
       if (!interactive(follower) && living(follower)
           && sizeof(dest_other) > ROOM_NPC_STOP && dest_other[ROOM_NPC_STOP])
         continue;

       if ( place->query_relative( verb ) ) {
         leave = (string)follower->find_rel( verb, 0 );
         follower->reorient_rel( leave );
       } else {
         leave = verb;
         follower->reorient_abs( leave );
       }

       if ( (int)follower->query_height() > dest_other[ ROOM_SIZE ] ) {
         tell_object( follower, "You are too tall to follow "+
                      (string)thing->the_short() +" "+ leave +".\n" );
         continue;
       }

       if (function_exists("check_doing_follow", follower) &&
           !follower->check_doing_follow(thing, verb, special)) {
         continue;
       }

       if(move_thing( follower, verb, dest_other[ROOM_DEST], special,
                      dest_other[ROOM_ENTER], dest_other[ROOM_MESS]) &&
          living(follower)) {

         tell_object(follower, "You follow "+ (string)thing->the_short() +
                     " "+ leave +".\n");

         if(follower->query_visible(thing))
           okay += ({ follower });


         follower->return_to_default_position(1);

       } else {
         tell_object( follower, "You fail to follow "+
                      (string)thing->the_short() +" "+ leave +".\n" );
       }
     }
   }

   //
   // Move everyone then do the look.  Fix up problems with followers
   // that have light, like the fireflies and blue lights.
   //
   thing->room_look();
   if(sizeof(okay)) {
     okay->room_look();
     tell_object(thing, query_multiple_short(okay) +
                 " $V$0=follows,follow$V$ you.\n" );
   }

   if(objectp(closed)) {
      closed->moving_close(thing);

      if (locked)
        closed->moving_lock(thing);
   }

   return 1;
}

void housekeeping() {
   call_out( "check_chatters", 1 );
   call_out( "check_doors", 2 );
   call_out( "housekeeping", 4 );
} /* housekeeping() */

void add_chatter(object thing, int number) {
   number /= 4;
   if ( number > CHATSIZE - 1 )
      number = CHATSIZE - 1;
   if ( !pointerp( chatters[ number ] ) )
      chatters[ number ] = ({ thing });
   else
      chatters[ number ] += ({ thing });
} /* add_chatter() */

void check_chatters() {
   object thing, *things;
   things = chatters[ 0 ];
   chatters[ 0 .. <2 ] = chatters[ 1 .. <1 ];
   chatters[ <1 ] = 0;
   if ( !pointerp( things ) )
      return;
   foreach ( thing in things ) {
      if ( objectp( thing ) )
         thing->make_chat();
   }
} /* check_chatters() */

void add_door( object thing ) {
   int number;
   number = random( DOORSIZE );
   if ( !pointerp( doors[ number ] ) )
      doors[ number ] = ({ thing });
   else
      doors[ number ] += ({ thing });
} /* add_door() */

void check_doors() {
   int wind;
   string dest, other, mess;
   object mine, thing, *things;
   things = doors[ 0 ];
   doors[ 0 .. <2 ] = doors[ 1 .. <1 ];
   doors[ <1 ] = 0;
   if ( !pointerp( things ) )
      return;
   foreach ( thing in things ) {
      if ( !objectp( thing ) )
         continue;
      if ( thing->query_closed() )
         continue;
      if(thing->query_stuck())
         continue;
      
      dest = (string)thing->query_dest();
      other = (string)thing->query_other_id();
      mine = (object)thing->query_my_room();
      wind = (int)WEATHER->calc_actual( mine, WINDSP );
      if ( random( 25 ) > wind ) {
         add_door( thing );
         return;
      }
      switch ( wind ) {
       case -1000 .. 20 :
         mess = "blow$s shut in the breeze.\n";
         break;
       case 21 .. 40 :
         mess = "blow$s shut in the wind.\n";
         break;
       default :
         mess = "slam$s shut in the wind.\n";
      }
      if ( find_object( dest ) ) {
         dest->modify_exit( other, ({ "closed", 1 }) );
         if(thing->query_autolock())
           dest->modify_exit(other, ({ "locked", 1 }));
         dest->tell_door( other, "The $D "+ mess, 0 );
      }
      thing->set_closed( 1 );
      if(thing->query_autolock())
        thing->set_locked();
      thing->tell_door( "The $D "+ mess, 0 );
   }
} /* check_doors() */

/* these two are not here because the handler can be saved, but to keep
 * the info over updates.
 */
mapping query_dynamic_auto_load() {
   mapping tmp;
   tmp = ([ "exit_types" : exit_types,
            "door_types" : door_types,
            "opposite" : opposite,
            "chatters" : chatters,
            "doors" : doors,
          ]);
   return tmp;
} /* query_dynamic_auto_load() */

void init_dynamic_arg(mapping maps) {
   if (maps["exit_types"])
      exit_types = maps["exit_types"];
   if (maps["door_types"])
      door_types = maps["door_types"];
   if (maps["opposite"])
      opposite = maps["opposite"];
   if (maps["chatters"])
      chatters = maps["chatters"];
   if (maps["doors"])
      doors = maps["doors"];
} /* init_dynamic_arg() */

mixed *stats() {
   int door_count, chatter_count;
   mixed temp;
   foreach (temp in chatters)
      chatter_count += sizeof( temp );
   foreach (temp in doors)
      door_count += sizeof( temp );
   return ({
      ({ "exit types", sizeof( exit_types ) }),
      ({ "door types", sizeof( door_types ) }),
      ({ "opposites",  sizeof( opposite ) }),
      ({ "chatters",  chatter_count }),
      ({ "doors", door_count }),
   });
}

