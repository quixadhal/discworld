/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: bob.c,v 1.3 2000/06/09 06:47:35 pinkfish Exp $
 * 
*/
#include <tasks.h>
#include <weapon.h>

#define QUEST_MAX 570
#define DIFFICULTY 100
//#define DEBUG 1

inherit "/std/effect_shadow";

string *data;

void set_data( string *words ) { data = words; }

mapping special_attack( object target ) {
    int damage;
    int skill;
    int difficulty;
    object *args;

    args = arg();

    if((object)environment(target) != (object)environment(player)) {
        tell_object(player, "Where did "+target->short()+" go?\n");
        remove_this_effect();
        return ([ ]);
    }

    if (!sizeof(match_objects_for_existence("bucket", ({ this_player() }))) &&
        !sizeof(match_objects_for_existence("apple", ({ this_player() })))) {
       add_failed_mess("You must have an apple and a bucket to use bob.\n");
       return 0;
    }

    difficulty += target->query_str() * 10;

    skill = (int)player->query_skill_bonus("fighting.combat.melee."+ data[1]);

    damage = 4 + sqrt( damage ) / 2;
    damage = 2 * damage + 8 * (random(damage)) +
             2 * damage + 8 * (random(damage));

    switch( (int)TASKER->perform_task(player, "fighting.combat.melee."+ data[1],
        difficulty, TM_COMMAND)) {
    case AWARD :
        tell_object( player, "%^YELLOW%^"+ replace( ({ "You feel that "+
              "your skill with a bucket has increased.", "You feel "+
              "more able to use a bucket.", "You seem to be a step "+
              "closer to mastering a bucket." })[ random( 3 ) ],
            "$weapon$", ( args[ 1 ] == player ? "unarmed combat" :
              (string)args[ 1 ]->a_short() ) ) +"%^RESET%^\n" );
    case SUCCEED :
        tell_object( player, "You grab hold of " +
          (string)target->the_short() + "'s head and shove it down "
          "into the bucket.\nYou scream out 'Bob for this!'\n"
          "You try and drown " + target->the_short() + ".\n");
        tell_room( environment(player),
          player->the_short() + " grabs hold of " +
          (string)target->the_short() + "'s head and shoves it down "
          "into the bucket.\n" + player->the_short() +
          " scream out 'Bob for this!'\n" +
          player->the_short() + " tries to drown " + target->the_short() +
          ".\n", ({ player, }));
        set_arg(0);
        break;
    default:
        tell_object( player, "You manage to botch your use of a bucket.\n");
        remove_this_effect();
        return 0;
    }

#ifdef DEBUG
    tell_creator("ceres", sprintf("Crush: player: %s, weapon: %s (%d), Diff: %d, Dam: %d Ski: %d\n",
        player->query_name(),
        args[ 1 ]->query_name(),
        args[1]->query_enchant(),
        difficulty, damage, skill));
#endif

    remove_this_effect();
    return ([ args[ 1 ] : ({ damage }) + data ]);
} /* special_attack() */

int tasking_done() { return 1; }

void event_death(object killed, object * others, object killer, string rmess,
                      string kmess) {
  object *args;

  player->event_death(killed, others, killer, rmess, kmess);
  args = arg();
  if ( killed == args[ 0 ] ) remove_this_effect();
}
