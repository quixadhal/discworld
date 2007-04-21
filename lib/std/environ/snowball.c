/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: snowball.c,v 1.8 2002/06/06 14:19:35 ohdamn Exp $
 * $Log: snowball.c,v $
 * Revision 1.8  2002/06/06 14:19:35  ohdamn
 * Fixed typo
 *
 * Revision 1.7  2002/06/06 14:17:44  taffyd
 * Fixed some things!
 *
 * Revision 1.6  2001/03/21 13:53:32  taffyd
 * Fixed pattern
 *
 * Revision 1.5  2001/03/13 00:04:52  pinkfish
 * Fix up the message when hitting.
 *
 * Revision 1.4  2000/11/16 06:31:29  ceres
 * Fixed typo
 *
 * Revision 1.3  2000/04/15 01:46:54  pinkfish
 * Make them use skills to determine the hits.
 *
 * Revision 1.2  2000/04/15 01:05:19  pinkfish
 * Make it so that it hits the person on the way up.
 *
 * Revision 1.1  1998/01/06 04:23:53  ceres
 * Initial revision
 * 
*/
#include <tasks.h>
inherit "std/object";

int strength;

void set_strength( int number );

void setup() {
   set_name( "snowball" );
   add_alias( "ball" );
   add_adjective( "snow" );
   set_strength( 99 );
   set_heart_beat( 1 );
} /* setup() */

int query_strength() { return strength; }

void set_strength( int number ) { strength = number; }

string long( string word, int dark ) {
   return "This is a "+ ({
      "very small lump of slush",
      "small and rather soggy lump of snow",
      "small snowball",
      "smallish snowball",
      "vaguely small snowball",
      "decent sized snowball",
      "reasonably sized snowball",
      "snowball for throwing at someone",
      "good snowball for throwing at someone",
      "nice, big snowball, just right for throwing at someone"
   })[ strength / 10 ] +".\n";
} /* long() */

void init() {
    add_command( "throw", "<direct:object> {at|to} <indirect:object>" );
} /* init() */

void heart_beat() {
   strength--;
   if ( strength < 1 ) {
      set_heart_beat( 0 );
      move( "/room/rubbish" );
   }
} /* heart_beat() */

private object find_person(object ob) {
   if (living(ob)) {
      return ob;
   } else {
      while (ob && !living(ob)) {
         ob = environment(ob);
      }

      if (ob) {
         return ob;
      }
   }
   return 0;
} /* find_person() */

int do_throw( object *obs ) {
   object per;
   int chance;

   per = find_person(obs[0]);
   if (per) {
      //
      // Make it harder if you are aiming at a specific bit rather than
      // the person.
      //
      if (per != obs[0]) {
         chance = 0;
      } else {
         chance = 30;
      }
      if (per == this_player()) {
         per->add_effect( "/std/effects/external/snowball", strength );
      } else {
         switch (TASKER->compare_skills(this_player(),
                                        "fighting.combat.range.thrown",
                                        per,
                                        "fighting.combat.dodging.range",
                                        chance,
                                        TM_FREE,
                                        TM_FREE)) {
         case OFFAWARD :
            tell_object(this_player(), "%^YELLOW%^You feel more able to throw "
                                       "snowballs at people.\n%^YELLOW%^");
         case OFFWIN :
            per->add_effect( "/std/effects/external/snowball", strength );
            
            add_succeeded_mess( ({ "$N throw$s $D at $I and hit$s!\n", 
                "$N throw$s $D at $I and hits!\n" }),  ({ obs[0] }) );
            break;
         case DEFAWARD :
            tell_object(this_player(), "%^YELLOW%^You feel more able to dodge "
                                       "snowball thrown by people.\n%^YELLOW%^");
         case DEFWIN :
            add_succeeded_mess( "$N throw$s $D at $I and miss$es!\n",
                                   ({ obs[0] }) );
            break;
         }
      }
   } else {
      add_succeeded_mess( "$N throw$s $D at $I.\n", ({ obs[ 0 ] }) );
   }
   move( "/room/rubbish" );
   return 1;
} /* do_throw() */

mapping query_dynamic_auto_load() {
   return ([
      "::" : ::query_dynamic_auto_load(),
      "strength" : strength
   ]);
} /* query_dynamic_auto_load() */

void init_dynamic_arg( mapping map, object ) {
   ::init_dynamic_arg( map[ "::" ] );
   strength = map[ "strength" ];
} /* init_dynamic_arg() */
