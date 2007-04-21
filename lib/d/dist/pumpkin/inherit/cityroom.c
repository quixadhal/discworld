/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: cityroom.c,v 1.3 1999/10/12 20:07:31 ceres Exp $
 *
 * $Log: cityroom.c,v $
 * Revision 1.3  1999/10/12 20:07:31  ceres
 * Missed an unneeded callout
 *
 * Revision 1.2  1999/10/04 22:49:46  ceres
 * Modified to seriously simplify the guttering code. (Basically deleted most of it :)
 *
 * Revision 1.1  1998/02/11 04:13:54  terano
 * Initial revision
 *
*/
#include <armoury.h>
#include <hospital.h>
#include <move_failures.h>
#include <weather.h>
#include "path.h"

#define CREVICE ({ "a crevice", "a pothole", "a gutter" })[ random( 3 ) ]
#define LOG_FILE LOG +"city_log"

inherit "/std/room/outside";

int *crowd;
string type, *froms;
object *monsters;
int last_found;

int query_cityroom() { return 1; }
int *query_crowd() { return crowd; }
object *query_monsters() { return monsters; }

void announce_entry( object thing ) {
  string word;
  if ( !froms ) {
    froms = ({ });
    foreach ( word in query_direc() ) {
      if ( member_array( word, ({ "north", "south", "east", "west",
                                    "northeast", "southwest", "southeast",
                                    "northwest" }) ) == -1 )
        continue;
      if ( call_door( word, "query_closed" ) )
        continue;
      froms += ({ "the "+ word });
    }
  }  
  if ( !sizeof( froms ) )
    tell_room( this_object(), "$a_short:"+ file_name( thing ) +
               "$ $V$0=arrives,arrive$V$ from elsewhere.\n" );
  else
    tell_room( this_object(), "$a_short:"+ file_name( thing ) +
               "$ $V$0=arrives,arrive$V$ from "+
               froms[ random( sizeof( froms ) ) ] +".\n" );
} /* announce_entry() */

void make_citymonster( int number ) {
  if(!(int)HOSPITAL->ok_to_clone()) {
    HOSPITAL->add_empty( this_object() );
    return;
  }
  
  if ( !monsters[ number ] ) {
    monsters[ number ] = (object)( HOSPITAL )->get_monster( type );
    monsters[ number ]->add_property( "start location", this_object() );
    this_object()->do_zoning( monsters[ number ] );
    monsters[ number ]->move( this_object() );
    announce_entry( monsters[ number ] );
  }
  number++;
  if ( number >= sizeof( monsters ) )
    return;
  
  if((int)HOSPITAL->ok_to_clone())
    call_out( "make_citymonster", 5 + random( 5 ), number );
} /* make_citymonster() */

void set_monster( mixed number, string word ) {
  if ( intp( number ) ) {
    crowd = ({ number });
  } else {
    crowd = number;
  }
  number = crowd[ 0 ];
  crowd += ({ number });
  if ( number > 3 ) {
    number = 3;
  }
  monsters = allocate( random( 1 + number ) );
  type = word;
} /* set_monster() */

void create() {
  crowd = ({ 0, 0 });
  type = "unknown";
  monsters = ({ });
  do_setup++;
  ::create();
  do_setup--;
  add_property( "room size", 20 );
  add_property( "room zone", ({ "Pumpkin" }) );
  add_extra_look( this_object() );
  if ( !do_setup ) {
    this_object()->setup();
    this_object()->reset();
  }
} /* create() */

void reset() {
  if ( sizeof( monsters ) ) {
    call_out( "make_citymonster", 5 + random( 5 ), 0 );
  }
} /* reset() */

void dest_me() {
  ::dest_me();
} /* dest_me() */

int clean_up( int parent ) {
  if ( base_name( this_object() )[0..<3] == __FILE__) {
    return 0;
  }
  return ::clean_up( 0 );
} /* clean_up() */

mixed *stats() {
  return ::stats() + ({
    ({ "crowd", array_to_string( crowd[ 0 .. <2 ] ) }),
      });
} /* stats() */

int do_search( string words ) {
  object found;

  if ( query_property( "no scavenging" ) ) {
    return -1;
  }
  if ( this_player()->query_property( "dead" ) ) {
    return -1;
  }
  if ( words ) {
    if ( ( strsrch( words, "ground" ) == -1 ) &&
         ( strsrch( words, "crevice" ) == -1 ) &&
         ( strsrch( words, "pothole" ) == -1 ) &&
         ( strsrch( words, "gutter" ) == -1 ) &&
         ( strsrch( words, "cobble" ) == -1 ) ) {
      return -1;
    }
  }
  switch ( random( 21 ) ) {
  case 0 .. 6 :
    found = clone_object( "/obj/money" );
    if ( random( 80 ) ) {
      found->adjust_money( 1, "Pumpkin cent" );
    } else {
      if ( random( 80 ) ) {
        found->adjust_money( 1, "Pumpkin dollar" );
      } else {
        found->adjust_money( 10, "Pumpkin dollar" );
      }
    }
    break;
  case 7 .. 16 :
    if((time() - last_found) > random(360)) {
      found = (object)ARMOURY->choose_small_item();
      if(objectp(found)) {
        found->set_percentage(5 + random(30));
      } else {
        found = 0;
      }
      last_found = time();
    }
    break;
  case 17 .. 18 :
    if(sizeof(match_objects_for_existence("cockroach", this_object())) > 3) {
      return -1;
    }
    
    found = (object)HOSPITAL->get_monster( "cockroach" );
    found->move( this_object(), "$N scuttle$s out of "+
                 CREVICE +"." );
    return -1;
  case 19 :
    if(sizeof(match_objects_for_existence("rodent", this_object())) > 3) {
      return -1;
    }
    
    found = (object)HOSPITAL->get_monster( "rodent" );
    found->move( this_object(), "$N slink$s out of "+
                 CREVICE +"." );
    return -1;
  default :
    write( "You have a good look at the ground and spotting "
           "something, pick it up.  It's only a lump of rotting "
           "rubbish, though, which you throw away quickly.\n" );
    say( (string)this_player()->a_short() +" looks closely at "
         "the ground.  Seeing something, "+
         (string)this_player()->query_pronoun() +" picks it up, "
         "but finds that it's only a lump of rotting rubbish, so "+
         (string)this_player()->query_pronoun() +" throws it away.\n" );
    this_player()->add_effect( "/std/effects/other/rubbish_smell",
                               roll_MdN( 5, 60 ) );
    return 1;
  }
  if ( !found ) {
    return -1;
  }
  write( "You have a good look at the ground and scavenge "+
         (string)found->a_short() +" from "+ CREVICE +".\n" );
  say( (string)this_player()->a_short() +" looks closely at the ground.\n" );
  if ( (int)found->move( this_player() ) == MOVE_OK ) {
    write( "You pick it up.\n" );
    say( (string)this_player()->query_pronoun() +" picks something up.\n" );
  } else {
    write("Unfortunately you cannot pick it up and it disappears down "+
          CREVICE+".\n");
  }
  
  return 1;
} /* do_search() */

void add_monster( object old_thing, object new_thing ) {
  int i;

  i = member_array( old_thing, monsters );
  if ( i != -1 ) {
    monsters[ i ] = new_thing;
    return;
  }
  i = member_array( 0, monsters );
  if ( i != -1 ) {
    monsters[ i ] = new_thing;
    return;
  }
  monsters += ({ new_thing });
} /* add_monster() */

/* Temporary measure until NPCs clean up the streets themselves. */
void event_dest_me() {
  if ( find_call_out( "check_stuff" ) == -1 ) {
    call_out( "check_stuff", 30 );
  }
} /* event_dest_me() */

void check_stuff() {
  object thing, *things;

  things = all_inventory( this_object() );
  foreach( thing in things ) {
    if ( userp( thing ) ) {
      call_out( "check_stuff", 30 );
      return;
    }
    if ( living( thing ) || thing->query_corpse() ) {
      things -= ({ thing });
      continue;
    }
    if ( thing->do_not_sell() || !thing->query_value() ) {
      thing->move( "/room/rubbish" );
      things -= ({ thing });
      continue;
    }
  }
  if ( sizeof( things ) ) {
    things->move( "/d/am/filigree/general_store" );
  }
} /* check_stuff() */

int query_crowd_now() {
  return crowd[ ( (int)WEATHER->minute_number() *
                  sizeof( crowd ) - 720 ) / 1440 ];
} /* query_crowd_now() */

string extra_look( object thing ) {
  switch ( query_crowd_now() ) {
  case 0 :
    return "";
  case 1 :
    return "With few people about, this seems to be a quiet "
      "part of the city, perfect for getting mugged.\n";
  case 2 :
    return "A number of people pass by as they go about their "
      "lives (or in some cases, existences).\n";
  case 3 :
    return "Quite a few people, some in small groups, walk, "
      "trot and lurch along the street.\n";
  case 4 :
    return "This seems to be a busy part of the city, with "
      "many people hurrying to various destinations.\n";
  case 5 :
    return "All around you, crowds teem as Pumpkin moves "
      "turbulently through its hectic day.\n";
  case 6 :
    return "Crowds of people jostle you constantly, so keep "
      "an eye on your pockets...\n";
  default :
    return "The densely packed crowds make it difficult to "
      "move, and unpleasant to breathe.\n";
  }
} /* extra_look() */
