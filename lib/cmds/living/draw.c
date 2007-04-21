/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: draw.c,v 1.11 2002/12/03 09:04:29 dogbolter Exp $
 *
 *
 */

#include <move_failures.h>
#include <obj_parser.h>

#define DEFAULT -1
#define LEFT 0
#define RIGHT 1

inherit "/cmds/base";

int cmd( string weapon_str, object * scabbards, string hand_str ) {
   object scabbard, * weapons, weapon;
   class obj_match match;
   string *limbs;
   string *hands;
   int *what_in;
   int targeted, result;
   int hand;

   match = new( class obj_match );
   targeted = scabbards ? 1 : 0;

   debug_printf( "targeted = %d\nscabbards = %O\n", targeted, scabbards );

   if( !scabbards ) {
      scabbards = filter( all_inventory( this_player() ),
        (: $1->query_scabbard() :) );

      if( !sizeof(scabbards) ) {
         add_failed_mess( "Can't find any scabbards in your inventory.\n" );
         return 0;
      }

      foreach( scabbard in scabbards ) {
         match = (class obj_match)match_objects_in_environments( weapon_str, scabbard,
           0, this_player() );
         result = match->result;
         if( result == OBJ_PARSER_SUCCESS ) {
            weapons = match->objects;
            break;
         } else if( result == OBJ_PARSER_AMBIGUOUS ) {
            add_failed_mess( "Ambiguous reference to \"" + weapon_str
              + "\" in at least one scabbard.\n" );
         }
      }
      if( !sizeof(weapons) ) {
         add_failed_mess( "Cannot find " + weapon_str + " in any "
           "scabbard in your inventory.\n" );
         return 0;
      }
   } else {
      if( sizeof(scabbards) > 1 ) {
         add_failed_mess( "You can only draw from one scabbard at a "
           "time.\n" );
         return -1;
      }
      scabbard = scabbards[0];

      if( !scabbard->query_scabbard() ) {
         add_failed_mess( scabbard->the_short() + " is not a scabbard.\n" );
         return 0;
      }

      if ( environment(scabbard) != this_player() ) {
         add_failed_mess( "You can only draw from a scabbard in your "
           "inventory.\n" );
         return -1;
      }

      match = (class obj_match)match_objects_in_environments( weapon_str, scabbard,
        0, this_player() );
      if( match->result == OBJ_PARSER_SUCCESS ) {
         weapons = match->objects;
      } else {
         add_failed_mess( "Cannot find " + weapon_str + " in " +
           scabbard->the_short() + ".\n" );
         return -1;
      }
   }
   weapon = weapons[0];

   if ( scabbard->query_closed() ) {
      add_failed_mess( "You cannot draw " + weapon->the_short()
        + " from " + scabbard->the_short() + "; it is closed.\n" );
      return targeted ? -1 : 0;
   }

   if( !scabbard->test_type_ok( weapon ) ) {
      add_failed_mess( "You cannot draw " + weapon->the_short()
        + " from " + scabbard->the_short() + ".\n" );
      return targeted ? -1 : 0;
   }

   if ( weapon->move( this_player() ) != MOVE_OK ) {
      add_failed_mess( "You cannot draw " + weapon->the_short()
        + " from " + scabbard->the_short() + ".\n" );
      return targeted ? -1 : 0;
   }

   if (!hand_str)  hand = -1;
   else hand = member_array(hand_str, this_player()->query_limbs());
   if ( !sizeof( weapon->hold_item( this_player(), hand ) ) ) {
      weapon->move( scabbard );
      add_failed_mess( "You cannot hold " + weapon->the_short() + ".\n" );
      return targeted ? -1 : 0;
   }

   if (!hand_str)  {
      limbs = this_player()->query_limbs();
      what_in = find_member(weapon, this_player()->query_holding());
      hands = allocate(sizeof(what_in));
      for (hand = 0; hand < sizeof(what_in); hand++)
         hands[hand] = limbs[what_in[hand]];
      hand_str = query_multiple_short(hands);
   }

   this_player()->add_succeeded_mess(scabbard, "$N $V $I from $D into $p " +
                                     hand_str + ".\n",
     ({ weapon }) );
   return 1;
}

mixed * query_patterns() {
   string limbs;

   limbs = "{" + implode(this_player()->query_limbs(), "|") + "}";
   return ({
      "<string'weapon'>",
        (: cmd( $4[0], 0, 0 ) :),
      "<string'weapon'> from <indirect:object:me'scabbard'>",
        (: cmd( $4[0], $1, 0 ) :),
      "<string'weapon'> into [my] " + limbs,
        (: cmd($4[0], 0, $4[1]) :),
      "<string'weapon'> from <indirect:object:me'scabbard'> into [my] " + limbs,
        (: cmd( $4[0], $1, $4[2]) :),
   });
}
