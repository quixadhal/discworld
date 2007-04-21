#include <effect.h>

#define BLIND "/std/effects/religious/blind"
#define CLASS "auriental.punishment.albatross"
#define COMBAT "/std/effects/fighting/combat"

string query_classification(){ return CLASS; }
int query_indefinite(){ return 1; }

void damage_player( object pl, int damage ){
  if( damage > 0 ) 
    damage = damage * -1;
  if( pl->adjust_hp( damage ) <= 0 )
    pl->attack_by( this_object() );
  COMBAT->monitor_points( pl, 1 );
} /* damage_player() */

void pecking( object pl ){
  int damage = roll_MdN( 3, 150 );
  object room = environment( pl );

  if( room->query_property( "location" ) != "outside" ){
    if( pl->expected_tt() )
      pl->submit_ee( 0, ( pl->expected_tt() + 15 + random( 20 ) ), EE_REMOVE );
    tell_object( pl,
      "The albatrosses pace around the area, glaring at you.\n" );
    tell_room( room,
      "Albatrosses pace underfoot, looking quite upset.\n",
      pl );
    return;
  }

  switch( roll_MdN( 2, 3 ) ){
    case 2:
      tell_object( pl,
        "The albatrosses peck at your eyes, blinding you.\n" );
      tell_room( room,
        "A flock of albatrosses peck at " + pl->the_short() + "'s eyes.\n",
        ({ pl }) );
      damage_player( pl, damage * 3 / 2 );
      pl->add_effect( BLIND, damage / 2 );
      break;
    case 3:
      tell_object( pl,
        "A lone albatross dive-bombs you, skewering you with its beak.\n" );
      tell_room( room,
        "A single albatross dives at " + pl->the_short() + ", impaling " +
        pl->query_objective() + " with its beak.\n",
        ({ pl }) );
      damage_player( pl, damage );
      break;
    case 4:
      tell_room( room,
        "The albatrosses circle, watching you with cold, beady eyes.\n" );
      break;
    case 5:
      tell_object( pl,
        "One of the albatrosses swoops low above you, dropping an oyster "
        "shell directly on your head.\n" );
      tell_room( room,
        "A large oyster shell drops from the sky, splitting open on " +
        pl->the_short() + "'s head.\n",
        ({ pl }) );
      damage_player( pl, damage / 2 );
      break;
    default:
      tell_object( pl,
        "Several albatrosses land on your shoulders, spearing their beaks "
        "into you.\n" );
      tell_room( room,
        "Several albatrosses drop onto " + pl->the_short() + "'s shoulders, "
        "skewering " + pl->query_objective() + " with their beaks before "
        "taking off again.\n",
        ({ pl }) );
      damage_player( pl, damage * 2 );
      break;
  }
} /* pecking() */

int beginning( object pl, int args ){
  tell_object( pl,
    "A flock of albatrosses forms above you.  They don't look pleased to see "
    "you at all.\n" );
  tell_room( environment( pl ),
    "A flock of albatrosses circles above " + pl->the_short() + "\n",
    ({ pl }) );
  pl->submit_ee( "pecking", ({ 15, 45 }), EE_CONTINUOUS, pl );
  pl->submit_ee( 0, args, EE_REMOVE );
  pl->add_extra_look( this_object() );
  return args;
} /* beginning() */

int merge_effect( object pl, int old_args, int new_args ){
  pl->submit_ee( 0, new_args, EE_REMOVE );
  return new_args;
} /* merge_effect() */

int restart( object pl, int args ){
  tell_object( pl,
    "The flock of albatrosses returns, apparently not finished with their "
    "attack.\n" );
  tell_room( environment( pl ),
    "A flock of albatrosses forms above " + pl->the_short() + "\n",
    ({ pl }) );
  pl->add_extra_look( this_object() );
  return args;
} /* restart() */

void end( object pl, int args ){
  tell_room( environment( pl ),
    "The albatrosses lose interest and slowly fly away, trying to remember "
    "how to be properly pointless.\n" );
  pl->remove_extra_look( this_object() );
} /* end() */

string extra_look( object pl ){
  if( environment( pl )->query_property( "location" ) != "outside" ){
    return capitalize( pl->query_pronoun() ) +
      " is surrounded by angry albatrosses.\n";
  }
  return capitalize( pl->query_pronoun() ) + 
    " has an angry flock of albatrosses around " + 
    pl->query_objective() + ".\n";
} /* extra_look() */

string query_death_reason(){ 
  return "being pecked to death by albatrosses";
} /* query_death_reason() */