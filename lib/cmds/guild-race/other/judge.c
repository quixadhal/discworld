#include <tasks.h>
#include <weapon.h>

// Remove this to disable logging to /log/BAD_WEAPONS
// and creator informs when judging
#undef DEBUG

#define COST 25
#define SKILL "other.evaluating.weapons"
#define QUEST_AVE 140
#define QUEST_MAX 300
#define LEARN 5
#define TEACH 50

inherit "/cmds/guild_base";

nosave string *_attack_types = ({ "blunt", "sharp", "pierce" });
nosave string *attacks;

void create() {
    ::create();
    set_nroff_file("judge");
    set_command_name("judge");
    add_teach_skill(SKILL, 50, 5);
} /* create() */

varargs int *calc_rating( object weapon, object player ) {
    int i, size;
    int ave = 0;
    int max = 0;
    int *damage;
    mixed *data;

    data = (mixed *)weapon->query_attack_data();

    if ( !( size = sizeof( data ) ) )
        return ({ 0, 0 });

    for ( i = 0; i < size; i += W_ARRAY_SIZE ) {
      if ( member_array( data[ i + W_SKILL ], _attack_types ) == -1 )
        continue;
      damage = data[ i + W_DAMAGE ];
      ave += ( data[ i + W_CHANCE ] * ( damage[ F_FIXED ] +
            ( damage[ F_NUM ] * ( 1 + damage[ F_DIE ] ) ) / 2 ) ) / 100;
      if(damage[ F_FIXED ] + damage[ F_NUM ] * damage[ F_DIE ] > max)
        max = damage[ F_FIXED ] + damage[ F_NUM ] * damage[ F_DIE ];
      debug_printf("[%s - ave: %d max: %d]\n", data[ i + W_TYPE],
                   ( data[ i + W_CHANCE ] * ( damage[ F_FIXED ] +
            ( damage[ F_NUM ] * ( 1 + damage[ F_DIE ] ) ) / 2 ) ) / 100,
                   damage[ F_FIXED ] + damage[ F_NUM ] * damage[ F_DIE ]);
    }

    debug_printf("[%s - ave: %d max: %d]\n", weapon->short(), ave, max );
    return ({ ave, max });
} /* calc_rating() */

void judge_against( object player, object first, object second ) {
  int grain, rating, rating_1, rating_2, difficulty, margin, *data_1,
    *data_2;

  data_1 = calc_rating( first, player );
  data_2 = calc_rating( second, player );
  difficulty = data_1[ 0 ] - data_2[ 0 ];

    if ( difficulty > 0 )
        difficulty = QUEST_AVE - difficulty;
    else
        difficulty = QUEST_AVE + difficulty;

    margin = ( difficulty + ( difficulty * ( data_1[ 1 ] + data_2[ 1 ] ) ) /
        ( 2 * QUEST_MAX ) ) / 2;

    switch ( TASKER->perform_task(this_player(), SKILL, difficulty - 25,
        TM_COMMAND) ) {

        case AWARD:
            tell_object( player,
                "%^YELLOW%^"+ replace( ({ "As you scrutinise $weapons$, you "
                "recognise an aspect of judging that you didn't use before.",
                "You realise something else that you can use to better judge "
                "$weapons$.", "As you inspect $weapons$, you feel a sense "
                "of surety in your judgement." })[ random( 3 ) ], "$weapons$",
                query_multiple_short( ({ first, second }), "the" ) ) +
                "%^RESET%^\n" );

        case SUCCEED:
            break;

        default:
            tell_object( player,
                "You examine "+ query_multiple_short( ({ first, second }),
                    "the" ) + " closely, but you're not sure which is "
                    "better.\n" );
            return;
    }
    grain = sqrt( (int)this_player()->query_skill_bonus( SKILL ) );

    if ( grain > 12 )
        grain = 12;

    rating_1 = QUEST_AVE * data_1[ 1 ] + QUEST_MAX * data_1[ 0 ];
    rating_1 = ( grain * rating_1 + QUEST_AVE * QUEST_MAX ) /
         ( 2 * QUEST_AVE * QUEST_MAX );
    rating_2 = QUEST_AVE * data_2[ 1 ] + QUEST_MAX * data_2[ 0 ];
    rating_2 = ( grain * rating_2 + QUEST_AVE * QUEST_MAX ) /
         ( 2 * QUEST_AVE * QUEST_MAX );
    rating = rating_1 - rating_2;

    if ( rating > 0 )
        rating++;
    else
        rating--;

    rating = 6 + rating / 2;

    if ( rating > 12 )
        rating = 12;

    if ( rating < 0 )
        rating = 0;

    if ( rating == 6 ) {
        tell_object( player, "You think that " +
            query_multiple_short( ({ first, second }), "the" ) +
            " are exactly the same.\n" );
        return;
    }

    tell_object( player, "You think that " + first->the_short() +" is "+ ({
         "amazingly worse", "extremely worse", "very much worse",
         "much worse", "worse", "a bit worse", "!", "a bit better",
         "better", "much better", "very much better",
         "extremely better", "amazingly better" })[ rating ] +" than "+
         second->the_short() +".\n" );
} /* judge_against() */

int judge_weapon( object player, object weapon ) {
    int grain, rating, margin, *data;
    string res;

    data = calc_rating( weapon, player );
    margin = ( data[ 0 ] + ( data[ 0 ] * data[ 1 ] ) / QUEST_MAX ) / 2;

    switch ( TASKER->perform_task(this_player(), SKILL,  data[ 0 ] + 25,
        TM_COMMAND) ) {

        case AWARD:
            tell_object( player,
                "%^YELLOW%^"+ replace( ({ "As you scrutinise $weapon$, you "
                "recognise an aspect of judging that you didn't use before.",
                "You realise something else that you can use to judge "
                "$weapon$.", "As you inspect $weapon$, you feel a sense of "
                "surety in your judgement." })[ random( 3 ) ], "$weapon$",
                weapon->the_short() ) +"%^RESET%^\n" );

    case SUCCEED:
      break;

    default:
      tell_object( player, "You think that " + weapon->the_short() +
                   " is a good weapon, but you're not sure.\n" );
      return 0;
    }

    grain = sqrt( player->query_skill_bonus( SKILL ) / 2 );

    if ( grain > 14 )
      grain = 14;

    rating = QUEST_AVE * data[ 1 ] + QUEST_MAX * data[ 0 ];
    rating = ( grain * rating + QUEST_AVE * QUEST_MAX ) /
      ( 2 * QUEST_AVE * QUEST_MAX );

    rating += 14 - grain;

    if ( rating > 14 ) {
#ifdef DEBUG
      log_file( "BAD_WEAPON", "%s exceeds weapon specifications, rating is "
                "%d.\n", file_name( weapon ), rating );
#endif
      rating = 14;
      //tell_object( player, "Please inform a creator that " +
      //weapon->the_short() + " exceeds weapon specifications.\n" );
      //return 0;
    }

    // Figure out what special attacks this weapon can do.
    attacks = player->query_known_commands();
    attacks = filter(weapon->query_attack_names(),
                  (: (member_array($1, attacks) != -1) :));
    attacks -= ({ "crush" });
    attacks -= ({ "impale" });
    attacks -= ({ "behead" });

    if(member_array("smash", weapon->query_attack_names()) != -1 &&
       member_array("crush", player->query_known_commands()) != -1)
       attacks += ({ "crush" });
    if(member_array("pierce", weapon->query_attack_names()) != -1 &&
       member_array("impale", player->query_known_commands()) != -1)
       attacks += ({ "impale" });
    if(member_array("slice", weapon->query_attack_names()) != -1 &&
       member_array("behead", player->query_known_commands()) != -1)
       attacks += ({ "behead" });

    res = "You think that "+ weapon->the_short() +" is " + ({
      "an atrocius", "an extremely poor", "a very poor", "a rather poor",
        "a poor", "a pretty poor", "a quite poor", "a reasonable",
        "a quite good", "a pretty good", "a good", "a rather good",
        "a very good", "an extremely good", "an excellent" })[ rating ] +
      " weapon";
    debug_printf("attacks: %O", attacks);
    if(sizeof(attacks))
      res += " that can be used to " + query_multiple_short(attacks, "", 0, 1,
                                                            0);
    res += ".\n";
    tell_object(player, res);
    return rating;
}

int valid_weapon( object player, object ob ) {
  if ( living( ob ) ) {
    if ( ob == player ) {
      add_failed_mess( "You stare at your muscles for a moment, "
                       "and decide that you're more than capable of destroying "
                       "any enemy that decides to interfere with you.\n" );
      return 0;
    }

    add_failed_mess( "You stare at $I for a while, but decide it "
                     "would be better if you considered them instead.\n",
                     ({ ob }) );
    return 0;
  }

  if ( !ob->query_weapon() ) {
    add_failed_mess( "$C$$I is not a weapon.\n", ({ ob }) );
    return 0;
  }

  if(environment(ob) != this_player())
    return add_failed_mess("You can't find $I.\n", ({ ob }));

  return 1;
} /* valid_weapon() */


int cmd( object first, object second ) {
  object *indirect_obs;
  int cost;
  function judge_func;

  if ( !valid_weapon( this_player(), first ) ) {
    return 0;
  }

  cost = COST;

  if ( second ) {
    if ( !valid_weapon( this_player(), second ) ) {
      return 0;
    }

    cost *= 2;

    indirect_obs = ({ first, second });
    judge_func = (: judge_against :);
  } else {
    indirect_obs = ({ first });
    judge_func = (: judge_weapon :);
  }

  if ( this_player()->query_specific_gp( "other" ) < cost ) {
    add_failed_mess( "You can't muster the energy to scrutinise "
                     "$I at the moment.\n", indirect_obs );
    return 0;
  }

  this_player()->adjust_gp( -cost );

  call_out( judge_func, 0, this_player(), first, second );
  add_succeeded_mess( "$N look$s closely at $I.\n", indirect_obs );
  return 1;
} /* cmd() */

mixed *query_patterns() {
  return ({ "<indirect:object'weapon'>", (: cmd( $1[0], 0 ) :),
              "<indirect:object:me'weapon'> against <indirect:object:me'weapon'>",
              (: cmd( $1[0][0], $1[1][0] ) :) });
} /* query_patterns() */
