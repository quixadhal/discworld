/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: special_attack.c,v 1.7 2000/03/28 21:18:50 ceres Exp $
 * $Log: special_attack.c,v $
 * Revision 1.7  2000/03/28 21:18:50  ceres
 * Can't remember
 *
 * Revision 1.6  2000/03/01 20:09:02  ceres
 * Added debugging informs
 *
 * Revision 1.5  1999/10/28 02:10:10  ceres
 * God knows
 *
 * Revision 1.4  1998/09/07 01:51:42  ceres
 * Minor tweaks.
 *
 * Revision 1.3  1998/03/28 06:17:18  ceres
 * modified advance rate
 *
 * Revision 1.2  1998/01/07 21:53:35  sin
 * Fixed_the_code_to_detect_whether_the_weapon_is_appropriate.
 *
 * Revision 1.1  1998/01/06 04:36:31  ceres
 * Initial revision
 * 
*/
#include <tasks.h>

#define QUEST_MAX 570
#define INFORM

inherit "/std/effect_shadow";

string *data;

void set_data( string *words ) { data = words; }

mapping special_attack( object target ) {
   int i, damage, skill;
   object *args;
   mixed *attacks;
   args = arg();
   if (!args) {
      return 0;
   }
   if ( !data || ( (string)player->query_combat_attitude() != "offensive" ) ||
         ( target != args[ 0 ] ) || !args[ 1 ] ) {
      tell_object( player, "You lose the moment!\n" );
      set_arg( 0 );
      remove_this_effect();
      return ([ ]);
   }
   if ( ( (object)args[ 1 ]->query_wielded() != player ) &&
         ( args[ 1 ] != player ) ) {
      tell_object( player, "What did you do with "+
            (string)args[ 1 ]->the_short() +"?\n" );
      set_arg( 0 );
      remove_this_effect();
      return ([ ]);
   }
   if((object)environment(target) != (object)environment(player)) {
     tell_object(player, "Where did "+target->short()+" go?\n");
     remove_this_effect();
     return ([ ]);
   }
   attacks = (mixed *)args[ 1 ]->weapon_attacks( 100, target );
   if ( !sizeof( attacks ) )
   {
      return 0;
   }
   for ( i = 0; i < sizeof( attacks ); i += 4 ) {
      /* This checks to see if the weapon attack type (attacks[i + 2])
       * is the same as the attack type we need (data[1]), or that
       * the attack type is the same up to the first '-' */
      if (attacks[i + 2] == data[1] ||
            attacks[i + 2][0..sizeof(data[1])] == data[1] + "-")
        damage += attacks[ i ];
   }
   if ( !damage ) {
     tell_object( player, "You manage to botch your use of "+
                  ( args[ 1 ] == player ? "unarmed combat" :
                    (string)args[ 1 ]->the_short() ) +
                  " and barely launch a mediocre attack at "+
                  (string)target->the_short() +".\n" );
#ifdef INFORM
    event(environment(player), "inform", (string)player->query_name() +
          " special manoeuvre failed (no damage)", "combat");
#endif
     return 0;
   }
   
   skill = (int)player->query_skill_bonus( "fighting.combat.melee."+
         data[ 0 ] );
   switch( (int)TASKER->perform_task(player, "fighting.combat.melee."+data[0],
                                     damage, TM_COMMAND) ) {
      case AWARD :
        tell_object( player, "%^YELLOW%^"+ replace( ({ "You feel that "+
               "your skill with $weapon$ has increased.", "You feel "+
               "more able to use $weapon$.", "You seem to be a step "+
               "closer to mastering $weapon$." })[ random( 3 ) ],
               "$weapon$", ( args[ 1 ] == player ? "unarmed combat" :
               (string)args[ 1 ]->a_short() ) ) +"%^RESET%^\n" );
      case SUCCEED :
         tell_object( player, "You launch a powerful attack on "+
               (string)target->the_short() + ( args[ 1 ] == player ? "" :
               " with "+ (string)args[ 1 ]->the_short() ) +".\n" );
         damage = sqrt( ((damage*2/3)) * skill ) + ( damage * skill ) /
           QUEST_MAX;
#ifdef INFORM
    event(environment(player), "inform", (string)player->query_name() +
          " special manoeuvre success, damage: " +damage, "combat");
#endif
         break;
      default :
        damage = 1;
         tell_object( player, "You manage to botch your use of "+
               ( args[ 1 ] == player ? "unarmed combat" :
               (string)args[ 1 ]->the_short() ) +
               " and barely launch a mediocre attack at "+
               (string)target->the_short() +".\n" );
#ifdef INFORM
    event(environment(player), "inform", (string)player->query_name() +
          " special manoeuvre failed", "combat");
#endif
   }
   set_arg( 0 );
   remove_this_effect();
   return ([ args[ 1 ] : ({ damage }) + data ]);
} /* special_attack() */

int tasking_done() { return 1; }
