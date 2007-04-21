/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: hospital.c,v 1.20 2000/01/30 06:26:43 terano Exp $
 *
*/

#include <armoury.h>
#include <hospital.h>
#include <living.h>
#include <map.h>
#include <money.h>
#include <move_failures.h>
#include <route.h>
#include <wander.h>
#include "path.h"

#define MAX_HEAVIES 10
#define DAY 8 * 60 * 60
#define INTERVAL 20 * 60                 /* a divisor of DAY */
#define MUD_TO_REAL 3
#define BLOCK 5
#define MAX_EMPTIES 12
#define MAX_MOVERS 60
#define SAVE_FILE SAVE +"hospital"

inherit "/std/room/basic_room";

nosave int alchemists, update, *alignments;
mapping uniques;
nosave int *al_data;
nosave string *city;
nosave object *empties;
nosave mapping blockages;
nosave mixed *movers;

// used for checking numbers of npcs
nosave int am_npcs, last_check;
int ok_to_clone();

void get_weapon(object ob, string *items);
void get_armour(object ob, string *items);
void get_jewellery(object ob, string *items);

void setup() {
   set_keep_room_loaded(1);
   update = time();
   alignments = allocate( 50 );
   uniques = ([ ]);
   if ( file_size( SAVE_FILE +".o" ) > 0 ) {
      unguarded( (: restore_object, SAVE_FILE :) );
   }
   alchemists++;
   unguarded( (: save_object, SAVE_FILE :) );
   city = ({
      "child", "rodent", "cockroach", "dog"
   });
   empties = ({ });
   blockages = ([ ]);
   movers = allocate( MAX_MOVERS );
   call_out( "check_movers", 10 );
   call_out( "housekeeping", INTERVAL );
} /* setup() */


int *query_al_data() { return al_data; }

mapping query_uniques() { return uniques; }

object *query_empties() { return empties; }

mapping query_blockages() { return blockages; }

int query_blockage( string this, string other, int number ) {
   if ( undefinedp( blockages[ this ] ) ) {
      if ( random( 100 ) >= BLOCK )
         number = -1;
      blockages[ this ] = blockages[ other ] = number;
   }
   return blockages[ this ];
} /* query_blockage() */

mixed *query_movers() { return movers; }

void housekeeping() {
   object thing;
   update = time();
   foreach( thing in users() ) {
/* ignore creators */
      if ( thing->query_creator() )
         continue;
/* sample alignments */
      alignments[ random( sizeof( alignments ) ) ] =
            (int)thing->query_al();
   }
   al_data = 0;
   unguarded( (: save_object, SAVE_FILE :) );
   call_out( "housekeeping", INTERVAL );
} /* housekeeping() */

int pick_al() {
   int one, *stats;
   if ( !al_data ) {
      al_data = allocate( 3 );
      stats = allocate( 4 );
      foreach( one in alignments ) {
         stats[ 0 ] += one;
         stats[ 1 ] += one * one;
         if ( one > stats[ 2 ] )
            stats[ 2 ] = one;
         if ( one < stats[ 3 ] )
            stats[ 3 ] = one;
      }
      stats[ 0 ] /= sizeof( alignments );
      stats[ 1 ] /= sizeof( alignments );
      stats[ 1 ] -= stats[ 0 ] * stats[ 0 ];
      stats[ 1 ] /= 10;
      if ( stats[ 1 ] < 50 )
         stats[ 1 ] = 50;
      stats[ 2 ] -= stats[ 3 ];
      if ( stats[ 2 ] < 2 )
         stats[ 2 ] = 2;
      al_data[ 0 ] = stats[ 0 ] - stats[ 2 ] / 2;
      al_data[ 1 ] = ( stats[ 2 ] * stats[ 2 ] ) / ( 12 * stats[ 1 ] );
      if ( al_data[ 1 ] < 5 )
         al_data[ 1 ] = 5;
      al_data[ 2 ] = ( 12 * stats[ 1 ] ) / stats[ 2 ];
      if ( al_data[ 2 ] < 25 )
         al_data[ 2 ] = 25;
   }
   return al_data[ 0 ] + roll_MdN( al_data[ 1 ], al_data[ 2 ] );
} /* pick_al() */

int make_unique( string word ) {
   if ( uniques[ word ] > time() )
      return 0;
/* Don't let them reappear for three hours. */
    uniques[ word ] = time() + 1 * 60 * 60;
   unguarded( (: save_object, SAVE_FILE :) );
   return 1;
} /* make_unique() */

void add_mover( object thing ) {
   int number;

   /* Fact Gathering Done by Terano */
   /* This generates WAY too much log traffic... the log wraps more then
      once an hour - Turrican.
   log_file("HOSPITAL_DATA", "Hospital: add_mover called with %O.\n", thing );
   */

   number = MAX_MOVERS / 2 + random( MAX_MOVERS / 2 );
   if ( !pointerp( movers[ number ] ) )
      movers[ number ] = ({ thing });
   else
      movers[ number ] += ({ thing });
} /* add_mover() */

object get_monster( string type ) {
   object thing;
   object ob;

   switch( type ) {
/* First the zones that there are: */
      case "city" :
      case "pumpkin" :
         thing = get_monster( city[ random( sizeof( city ) ) ] );
         thing->add_property( "monster type", type +":"+
               (string)thing->query_property( "monster type" ) );
         thing->add_move_zone( "Pumpkin" );
         add_mover( thing );
         return thing;
/* Now the specific types of NPC: */
      case "dog":
         thing = clone_object( CHARS + "dog" );
         if ( random( 4 ) )
            thing->set_type( "small" );
         else
            thing->set_type( "large" );
         thing->add_effect( "/std/effects/npc/i_died",
               ({ HOSPITAL, "regen_after_death" }));
         thing->add_effect( "/std/effects/npc/eat_edible" );
         thing->add_effect( "/std/effects/npc/savage_corpse" );
         thing->add_property( "monster type", type );
         thing->add_property("animal type", type);
         return thing;
      case "child":
         thing = clone_object( CHARS + "child_human" );
         return thing;
/* And now for all the nasty old ones... */
    case "cityguard":
      ob = clone_object(MONSTER);
      ob->add_property("monster type", type);
      ob->set_name("guard");
      ob->add_adjective("city");
      ob->set_main_plural( "city guards" );
      ob->set_short("city guard");
      ob->set_long("This is a city guard. He is supposed to protect the " +
        "city from undesirables. Sadly, Pumpkin isn't very fussy.\n");
      ob->set_race("human");
      ob->set_gender("male");
      ob->set_guild("fighter");
      ob->set_level(8);
      ob->set_al( pick_al() / 5 );
      ob->adjust_money( 5 + random( 10 ), "Pumpkin pence" );
      ob->load_chat(10, ({
          3, "@grumble bitterly",
          1, "@daydream",
          1, "@sigh heavily"
          }) );
      ob->load_a_chat(50, ({
          1, "'Ouch!",
          1, "'Come on! I don't need this.",
          1, "'Be on your way!",
          }) );
      ARMOURY->request_weapon("long sword", 40+random(40))->move(ob);
      if(random(2))
        ARMOURY->request_armour("hard leather cap", 70 + random(30))->move(ob);
      ob->init_equip();
      return ob;
    case "mercenary":
      ob = clone_object(MONSTER);
      ob->set_name("mercenary");
      ob->set_main_plural( "mercenaries" );
      ob->add_plural( "mercenaries" );
      ob->add_adjective("tough");
      ob->set_race("human");
      ob->set_class("fighter");
      ob->set_level( 50 + roll_MdN( 5, 50 ) );
      ob->set_al( pick_al() );
      ob->set_gender(random(2) + 1);
      ob->set_long("A tough battle scarred mercenary.  There are better "
                   "ways of filling in an afternoon than messing with the "
                   "likes of "+ob->query_objective()+".\n");
      ob->adjust_money( 5 + random( 50 ), "Ankh-Morpork pence" );
      ob->adjust_money( 1 + random( 5 ), "Ankh-Morpork dollar" );
      ob->load_chat(3, ({
        1, "'Anyone you don't like?",
        1, "'I'll do anything if the price is right?",
        1, "'Don't get me angry.  You won't like me when I'm angry."
      }));
      ob->load_a_chat(30, ({
        1, "'I'll show you who's boss.",
        1, "'You think you're so tough.",
        1, "The crowd runs in terror as the mercenary kills someone by "+
           "accident."
      }));
      get_weapon(ob, ({"bastard sword", "long sword", "morning star",
       "two-handed axe"}));
      get_armour(ob, ({"chainmail", "splintmail", "ringmail"}));
      ob->add_property("monster type", type);
      ob->init_equip();
      return ob;
    case "rodent":
      ob = clone_object(MONSTER);
      ob->set_name("rat");
      ob->set_short("rat");
      ob->set_race("rat");
      ob->set_level( random( 1 + random( 3 ) ) );
      ob->add_alias("city rat");
      ob->set_main_plural("rats");
      ob->add_adjective("dirty");
      ob->add_adjective("city");
      ob->set_long("This is a large rat.  It appears to have done well "+
       "living in the city.\n"+
       "The pollution must not affect it too much.\n" );
      ob->load_chat( 5, ({
         1, ":squeaks.",
         1, ":sniffs the air, whiskers twitching.",
         1, ":checks for dwarves."
      }) );
      ob->set_wimpy( 10 );
      ob->load_a_chat( 50, ({
         1, ":squeals in pain.",
         1, ":twitches fearfully."
      }) );
      ob->add_effect("/std/effects/npc/i_died",
                     ({ HOSPITAL, "regen_after_death" }));
      ob->add_effect( "/std/effects/npc/eat_edible" );
      ob->add_effect( "/std/effects/npc/savage_corpse" );
      ob->add_property("monster type", type);
      ob->add_property("animal type", type);
      return ob;
    case "cockroach":
      ob = clone_object(MONSTER);
      ob->set_name("cockroach");
      ob->set_short("cockroach");
      ob->set_race("cockroach");
      ob->set_level( random( 1 + random( 3 ) ) );
      ob->set_main_plural("cockroaches");
      ob->add_adjective("dirty");
      ob->add_alias("roach");
      ob->set_long( "This is a huge dirty great big cockroach, the size of "
             "your hand at least.  It waves its feelers at you and looks "
             "horrible and black.  Not only can cockroaches survive fires, "
             "plagues, earthquakes and miscellaneous acts of supernatural "
             "beings, they can survive in Ankh-Morpork!\n" );
      ob->load_chat( 5, ({
         10, ":scuttles around a bit.",
         10, ":cleans its feelers.",
         10, ":rushes around in circles.",
         1,  ":jerks off in a strange direction.",
      }) );
      ob->set_wimpy( 30 );
      ob->load_a_chat( 50, ({
         1, ":oozes horrible white stuff.",
         1, ":makes a sort of scraping noise."
      }) );
      ob->add_effect("/std/effects/npc/i_died", 
                      ({ HOSPITAL, "regen_after_death" }));
      ob->add_property("monster type", type);
      ob->add_property("animal type", type);
      return ob;
    default:
      ob = clone_object(MONSTER);
      ob->set_name("failure");
      ob->set_short("failure");
      ob->set_long( "Please inform a creator about this object.\n"+
          "Type is "+ type +" from "+ file_name( previous_object() ) +".\n" );
      ob->add_property( "monster type", "failure:"+ type );
      return ob;
  }
} /* get_monster() */

void get_armour(object ob, string *items) {
  ARMOURY->request_armour(items[random(sizeof(items))], 50+random(50))->
    move(ob);
} /* get_armour() */

void get_jewellery(object ob, string *items) {
  ARMOURY->request_armour(items[random(sizeof(items))], 20+random(80))->
    move(ob);
} /* get_jewellery() */

void get_weapon(object ob, string *items) {
  ARMOURY->request_weapon(items[random(sizeof(items))], 50+random(50))->
    move(ob);
} /* get_weapon() */

void regen_after_death(object player) {
  object ob, dest;
  string nam;

  if (!player)
    return ;
  nam = (string)player->query_property("monster type");
  dest = (object)player->query_property("start location");
  if ( !nam )
    return;
  if ( !dest )
    return;
  if(!ok_to_clone())
    return;
  
  ob = get_monster( explode( nam, ":" )[ 0 ] );
  dest->add_monster(player, ob);
  call_out("do_move", 10, ({ ob, dest }) );
} /* regen_after_death() */

void do_move(mixed *junk) {
  junk[0]->move(junk[1]);
} /* do_move() */

string change_to_name(object ob) {
  return implode((string *)ob->query_adjectives(), " ")+
         (string)ob->query_name();
} /* change_to_name() */

void do_run(object ob) {
  if (ob)
    ob -> run_away();
} /* do_run() */

void fight_check(object ob, object ob1) {
  if (ob->query_property(previous_object()->query_name()))
    previous_object()->attack_ob(ob1);
} /* fight_check() */

void do_grin_laugh(object ob) {
  ob->add_respond_to_with(({ "@grin", ob->query_name() }), 
                          "laugh man at $hname$");
} /* do_grin_laugh() */

void add_empty( object thing ) {
   empties -= ({ 0 });
   empties += ({ thing });
   if ( sizeof( empties ) > MAX_EMPTIES )
      empties = shuffle( empties )[ 0 .. MAX_EMPTIES - 1 ];
} /* add_empty() */

void move_monster( object thing ) {
   int i;
   string dest, direc, zone, *movez, *roomz;
   object place;

   /* Fact Gathering Done by Terano */
   /* See above comment - Turrican
   log_file("HOSPITAL_DATA", "Hospital: move_monster called with %O.\n", thing );
   */

   if ( !thing )
      return;
   if ( (int)thing->query_hp() < 0 )
      return;
   if ( thing->query_property( PASSED_OUT ) || thing->query_fighting() ) {
      add_mover( thing );
      return;
   }
   movez = (string *)thing->query_move_zones();
   empties -= ({ 0 });
   if ( sizeof( empties ) && !thing->check_anyone_here() )
      foreach ( place in empties ) {
         roomz = (string *)place->query_zones();
         if ( !sizeof( roomz ) ) {
            empties -= ({ place });
            continue;
         }
         foreach ( zone in roomz ) {
            i = member_array( zone, movez );
            if ( i != -1 )
               break;
         }
         if ( i == -1 )
            continue;
         thing->move( place );
         place->add_monster( 0, thing );
         place->announce_entry( thing );
         empties -= ({ place });
         add_mover( thing );
         return;
      }
   place = environment( thing );
   if ( !place )
      return;
   add_mover( thing );
   foreach ( direc in shuffle( (string *)place->query_direc() ) ) {
      place->set_destination( direc );
      dest = (string)place->query_destination( direc );
      if ( !stringp( dest ) )
         continue;
      if ( !thing->check_anyone_here() && !find_object( dest ) )
         continue;
      if ( find_object( dest ) )
         roomz = (string *)dest->query_zones();
      else
         roomz = (string *)MAP->query_zones( dest );
      if ( !sizeof( roomz ) )
         continue;
      foreach ( zone in roomz ) {
         i = member_array( zone, movez );
         if ( i != -1 )
            break;
      }
      if ( i == -1 )
         continue;
      if ( place->query_relative( direc ) )
         direc = (string)thing->find_rel( direc, 0 );
      if ( thing->do_command( direc ) )
         return;
   }
} /* move_monster() */

void check_movers() {
   int when;
   object thing, *things;

   /* Fact Gathering Done by Terano */


   things = movers[ 0 ];
   movers[ 0 .. MAX_MOVERS - 2 ] = movers[ 1 .. ];
   movers[ MAX_MOVERS - 1 ] = 0;
   call_out( "check_movers", 10 );
   if ( !pointerp( things ) )
      return;
   foreach ( thing in things ) {
      if ( objectp( thing ) ) {
         when += 2;
         call_out( "move_monster", when, thing );
      }
   }
} /* check_movers() */


// This function returns 1 if its ok to clone some more npcs and
// false if not.
int ok_to_clone() {
  if(time() > last_check + 300) {
    last_check = time();
    am_npcs = sizeof( filter( named_livings(),
      (: environment( $1 ) && base_name( environment( $1
       ) )[0..4] == "/d/am" :) ) );
  }
  return ( am_npcs < MAX_AM_LIVING );
}


int *query_npcs() {
  return ({ am_npcs, last_check });
}

