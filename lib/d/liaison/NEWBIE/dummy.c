inherit "/obj/monster";

#include <armoury.h>
#include <weapon.h>
#include <tasks.h>
#include <combat.h>

int told_attack, told_parry, ability;
string *misc;
object trainer, trainee;

#define DODGE "fighting.combat.dodging.melee"
#define PARRY "fighting.combat.parry.melee"

#define PERSON_DODGE_FAILURE "As the dummy spins around, an arm swings towards you and hits you before you can dodge out of the way!\n"
#define ROOM_DODGE_FAILURE " isn't fast enough to dodge a spinning arm!  Ouch!\n"
#define PERSON_DODGE_SUCESS "As the dummy spins around, an arm swings towards you but you see it in time and dodge it.\n"
#define ROOM_DODGE_SUCESS " dodges the arm of the wooden training dummy.\n"
#define PERSON_PARRY_FAILURE "As the dummy spins around, an arm swings towards you but you are unable to block it!\n"
#define ROOM_PARRY_FAILURE " isn't fast enough to avoid the training dummy! Ouch!\n"
#define PERSON_PARRY_SUCCESS "As the dummy spins around, an arm swings towards you but you see it in time and block it.\n"
#define ROOM_PARRY_SUCCESS " parries the training dummy.\n"

#define MISC_MESSAGES ({ "Generally, heavier weapons are harder to attack and "\
  "parry with but their attacks are harder to parry.", \
  "Being burdened in combat, by carrying lots of things or wearing lots of " \
  "armour, reduces your effectiveness.  This is especially true for dodging.", \
  "When injured it is harder to fight, so watch your health closely.", \
  "Holding a two-handed weapon in one hand can be useful when using a shield "\
  "but is hard to do unless you have high strength.", \
  "You can focus on specific parts of your opponents body to take advantage " \
  "of weaknesses in their armour but this is harder to do than attacking " \
  "normally."})
#define DIFF 15
#define MOD 10

int assault_dummy();
mixed *callback(int stage, class attack att, mixed data);
void failed_defense( object );
void failed_attack( object );
void tell_misc( object );
void spin_arm( object, object );

object query_trainer() {
  return trainer;
}

object reset_trainer() {
  trainer = 0;
}

object set_trainer( object thing ) {
  return trainer = thing;
}

object query_trainee() {
  return trainee;
}

object reset_trainee() {
  trainee = 0;
}

object set_trainee( object thing ) {
  return trainee = thing;
}

int query_skill_bonus( string words ) {
  return 10;
}

void setup() {
  set_name( "dummy" );
  add_adjective( ({ "wood", "wooden", "training" }) );
  //  add_property( "determinate", "A" );
  set_short( "training dummy" );
  set_long( "A fighters dummy.  It is looking pretty worn from having "+
    "all the stuffing knocked out of it all day.\n");
  basic_setup( "human", "warrior", 10 );
  set_gender( 0 );
  add_property( "run away", -1 );
  set_dex(6);
  set_str(6);
  set_con(18);
  set_int(6);
  set_wis(6);
  remove_ac( "sharp" );
  remove_ac( "pierce" );
  remove_ac( "blunt" );
  remove_ac( "unarmed" );
  add_skill_level( "other.health", 1000 );
  add_combat_action( 100, "spin_arm", (: spin_arm :) );

  ARMOURY->request_item( "dirty rags", 30 )->move( this_object() );

} /* setup() */

// Don't let the dummy attack;
int attack_this_time() {
  return 0;
}

void init() {
  if( this_player() ) {
    this_player()->add_command( "kill", this_object(), "{dummy}",
      (: assault_dummy() :) );
    this_player()->add_command( "attack", this_object(), "{dummy}",
      (: assault_dummy() :) );
  }
}/*init*/

int assault_dummy() {
  object *weapons, weapon;
  int i;

  if( trainer->query_fighting() ) {
    write( "You beat at and tear up " + this_object()->the_short() +
      " for a while until you get bored.\n" );
    say( this_player()->query_short() + " hacks at " +
      this_object()->the_short() + " for a while.\n", this_player() );
  }

  if( trainee && !interactive( trainee ) )
    reset_trainee();

  if( trainee ) {
    if( trainee->query_fighting() ) {
      if( trainee == this_player() ) {
        tell_object( this_player(), "You are already using " +
          this_object()->the_short() + ".\n" );
        return notify_fail( "" );
      }
      tell_object( this_player(), trainee->one_short() + " is currently "
        "using " + this_object()->the_short() + ".\n" );
      return notify_fail( "" );
    }
  }

  weapon = 0;
  ability = 0;
  told_attack = 0;
  told_parry = 0;
  misc = copy(MISC_MESSAGES);
  weapons = (object *)this_player()->query_weapons();

  if( sizeof( weapons ) > 1 ) {
    trainer->stop_them( previous_object(), this_object() );
    trainer->do_command( "say Just use one weapon, I get confused if you use "
      "more than one!" );
    return 1;
  }

  if( sizeof( weapons ) && !weapon )
    weapon = weapons[ 0 ];

  this_player()->attack_ob( this_object() );
  write( "You attack " + this_object()->the_short() + ".\n" );
  say( this_player()->the_short() + " attacks " + this_object()->a_short() +
    ".\n", this_player() );
  trainee = this_player();

  for( i = 0; i < sizeof( ( class combat_special )this_object()->
    query_specials() ); i++ ) {
      this_object()->remove_special( ( class combat_special )this_object()->
        query_specials()[i]->id );
  }

  this_object()->register_special( T_DEFENSIVE,
                                   E_AFTER_ATTACK,
                                   ({ this_object(), "callback" }),
                                   ({ 0, 0, weapon }) );
  return 1;
}/*attack_dummy*/

int attack_by(object ob) {
  if(!sizeof(query_specials()))
    call_out("attack_dummy", 0);
  return ::attack_by(ob);
}

mixed *callback( int stage, class attack att, mixed data ) {
  int *rating, number, damage, last_damage;
  object dumdum, newbie, *weapons, weapon;
  string what, skill;
  mixed *attacks;
  
  newbie = att->attacker;
  dumdum = att->opponent;

  /*
   * The data array is the list of information specific to a particular
   * special for a particular player.
   *
   * data[0] == Times tried
   * data[1] == Damage
   * data[2] == Weapon
   */

  weapons = (object *)this_player()->query_weapons();

  if( sizeof( weapons ) > 1 ) {
    trainer->stop_them( newbie, dumdum );
    trainer->do_command( "say Just use one weapon, I get confused if you use "
      "more than one!" );
    debug_printf( "More than one weapon, removing special.\n" );
    return({ R_CONTINUE | R_REMOVE_ME, att, data });
  }

  if( sizeof( weapons ) && !weapon )
    weapon = weapons[ 0 ];

  if( weapon != data[2] ) {
    trainer->stop_them( newbie, dumdum );
    trainer->do_command( "say You've changed weapons!" );
    debug_printf( "Changed weapons, removing special.\n" );
    return({ R_CONTINUE | R_REMOVE_ME, att, data });
  }

  last_damage = data[1];
  damage = att->damage;
  data[1] = damage;

  if( !last_damage) {
    failed_attack(newbie);
  }
  if(last_damage || !random(4)) {
    // This hands out skill advances very generously up to lvl 5 then it
    // stops.
    if( weapon ) {
      attacks = weapon->query_attack_data();
      skill = attacks[random( sizeof( attacks ) / W_ARRAY_SIZE ) *
        W_ARRAY_SIZE + W_SKILL];
    } else {
      skill = "unarmed";
    }
    if( newbie->query_skill( "fighting.combat.melee." + skill ) < 5 &&
       !random( 4 * ( newbie->query_skill( "fighting.combat.melee." +
       skill ) ) ) &&
       newbie->add_skill_level( "fighting.combat.melee." + skill, 1, 1 ) ) {
      if( skill == "unarmed" )
         tell_object( newbie, "%^YELLOW%^You feel you've learned something "
          "about unarmed combat.%^RESET%^\n" );
       tell_object( newbie, "%^YELLOW%^You feel you've learned something about "
         "using " + weapon->a_short() + ".%^RESET%^\n" );
    }
  }

  // Check if we have used up all our chances.
  if( data[0]++ > 15 ) {
    number = ( 2 * (ability + data[0]) ) / ( 2 * data[0] );
    if( weapon )
      what = convert_message( ( string )weapon->the_short() );
    else
      what = "whatever it was you were using";
    trainer->stop_them( newbie, dumdum );
    
    trainer->do_command( "think" );
    switch ( number ) {
    case 0..1 :
      if( skill == "unarmed" )
        trainer->do_command( "say Well, you're rather hopeless." );
      else {
        if( strsrch( what, "knife" ) == -1 && strsrch( what, "dagger" ) == -1)
          trainer->do_command( "say Well, you're rather hopeless with " + what +
            ", really.  A lighter, simpler weapon may help." );
        else
          trainer->do_command( "say Well, you're rather hopeless with " + what +
            ", really, but with practice you'll get better.");
      }  
      break;
    case 2 .. 3 :
      if( skill == "unarmed" )
        trainer->do_command( "say Hmmm... You have some skill." );
      trainer->do_command( "say Hmmm... You have some skill with " + what +
        ", at any rate, as you practice you'll get better." );
      break;
    case 4 .. 5 :
      if( skill == "unarmed" )
        trainer->do_command( "say You're getting there.  Keep training and "
          "you'll be pretty good soon." );
      trainer->do_command( "say You're getting there.  Keep training with " +
        what + " and you'll be pretty good with it soon." );
      break;
    default :
      if( skill == "unarmed" )
        trainer->do_command( "say I'd say you've more or less mastered unarmed "
          "combat.  Well done!" );
      trainer->do_command( "say I'd say you've more or less mastered " + what +
        ".  Well done!" );
    }
    trainee = 0;
    return({ R_CONTINUE | R_REMOVE_ME, att, data });
  }

  tell_misc(newbie);
  if( !damage) {
    return ({ R_CONTINUE, att, data });
  }
  

  if( skill == "unarmed" ) {
    rating = (int *)"/cmds/guild-race/other/judge"->calc_rating( newbie );
  } else {
    rating = (int *)"/cmds/guild-race/other/judge"->calc_rating( weapon );
  }
  // Ratings for unarmed.  I'm making them up!
  if( !rating[0] )
    rating [0] = random( 10 ) + 30;
  if( !rating[1] )
    rating [1] = random( 80 ) + 60;

  number = ( ( 40 * damage ) / rating[ 0 ] +
             ( 20 * damage ) / rating[ 1 ] ) / 5;
  debug_printf("%d", number);
  ability += number;
  if (number > 14)
    number = 14;
    
  trainer->do_command( "say That's " + ({ "an atrocious", "an extremely poor",
    "a very poor", "a rather poor", "a poor", "a pretty poor", "quite a poor",
    "a reasonable", "quite a good", "a pretty good", "a good", "a rather "
    "good", "a very good", "an extremely good", "an excellent" })
    [ number ] + " attack." );

  return ({ R_CONTINUE, att, data });
}

void spin_arm( object newbie, object dumdum ) {
  int diff, which, failed;

  //debug_printf( "specials %O\n", this_player()->query_specials() );

  if( environment( newbie ) != environment( this_object() ) )
     return;

  switch( ( ( class tactics )newbie->query_tactics() )->attitude ) {
  case "offensive":
    diff = DIFF + MOD;
    break;
  case "defensive":
    diff = DIFF - MOD;
    break;
  default:
    diff = DIFF;
  }

  switch( newbie->query_combat_response() ) {
  case "dodge" :
    which = 1;
    break;
  case "parry":
    which = 0;
    break;
  default:
    which = random( 2 );
  }

  // Dodge
  if( which ) {
    switch( TASKER->perform_task( newbie, DODGE, diff, TM_FIXED ) ) {
      case FAIL :
        if( newbie->query_skill( DODGE ) >= 5 ||
           random( 3 * ( newbie->query_skill( DODGE ) ) ) ||
           !newbie->add_skill_level( DODGE, 1, 1 ) ) {
          tell_object( newbie, PERSON_DODGE_FAILURE );
          tell_room( environment( newbie ),newbie->one_short() +
            ROOM_DODGE_FAILURE, ({ newbie }) );
          failed = 1;
          break;
        }
      case AWARD :
        tell_object( newbie, "%^YELLOW%^You feel better at dodging "
          "things.%^RESET%^\n" );
      case SUCCEED :
        tell_object( newbie,PERSON_DODGE_SUCESS );
        tell_room( environment( newbie ), newbie->one_short() +
          ROOM_DODGE_SUCESS, ({ newbie }) );
        break;
    }
  } else {
    // Parry
    switch( TASKER->perform_task( newbie, PARRY, diff, TM_FIXED ) ) {
      case FAIL :
        if( newbie->query_skill( PARRY ) >= 5 ||
           random( 3 * ( newbie->query_skill( PARRY ) ) ) ||
           !newbie->add_skill_level( PARRY, 1, 1 ) ) {
          tell_object( newbie, PERSON_PARRY_FAILURE );
          tell_room( environment( newbie ), newbie->one_short() +
            ROOM_PARRY_FAILURE, ({ newbie }) );
          failed = 1;
          break;
        }
      case AWARD :
        tell_object( newbie, "%^YELLOW%^You feel better at parrying things."
          "%^RESET%^\n" );
      case SUCCEED :
        tell_object( newbie, PERSON_PARRY_SUCCESS );
        tell_room( environment( newbie ), newbie->one_short() +
          ROOM_PARRY_SUCCESS, ({ newbie }) );
        break;
    }
  }

  if( trainer && failed )
    failed_defense(newbie);
  return;
}

void tell_misc(object newbie) {
  int which;

  if(!sizeof(misc) || random(5))
    return;
  
  which = random(sizeof(misc));
  trainer->do_command("say " + misc[which]);
  misc -= ({ misc[which] });
}

void failed_attack( object newbie ) {
  string attitude;

  if(told_attack)
    return;
  
  attitude = (newbie->query_tactics())->attitude;
  if(attitude != "offensive" && attitude != "insane") {
    told_attack = 1;
    trainer->do_command( "say You can improve your chances of hitting your "
                         "opponent by setting your tactics to a more "
                         "offensive attitude.");
  }
}

void failed_defense( object newbie ) {
  string attitude;
  
  if(told_parry)
    return;

  attitude = (newbie->query_tactics())->attitude;

  if(attitude != "defensive" && attitude != "wimp") {
    told_parry = 1;
    trainer->do_command( "say You can improve your chances of defending by "
                         "setting your tactics to a more defensive "
                         "attitude." );
  }
}

void adjust_hp() {
  return this_object()->query_max_hp();
}
