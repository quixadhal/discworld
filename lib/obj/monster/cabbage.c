inherit "/obj/monster";

#define SCATHE "/cmds/guild-race/magic/scathe"
#define LANCE "/obj/spells/mental_lance"

#include <config.h>
#ifndef __DISTRIBUTION_LIB__
#include <special_day.h>
#endif
#include <move_failures.h>
#include <armoury.h>

object query_owner();
void check_env();
void zap_cabbage();
string long_desc();

int eaten;
string owner_name, owner_short, language;
object owner;



void setup() {
  set_name( "cabbage" );
  set_short( "cabbage" );
  set_long( "A small cute looking cabbage.\n" );
  set_race( "cabbage" );
  set_level( 1 );

  add_respond_to_with( ({ ({ "@hug", "@snuggle", "@cuddle", "@nestle",
    "@snuffle", "@smooch", "@fondle", "@huggle", "@squeeze", "@wuffle"
    "@hop", "@lick", "@snog", "@fluff", "@nuzzle" }), "you" }), 
    "#respond_nice");
    
  add_respond_to_with( ({ ({ "@skick", "@spunch", "@sburn", "@jump", }),
    "you" }), "#respond_nasty");

#ifndef __DISTRIBUTION_LIB__
  if (SPECIAL_DAY_HANDLER->query_special_day() == CABBAGE_DAY) {
    add_property( "player", 1 ); // Can't be buried.
    set_get();
  }
#endif
} /* setup() */


void init() {
  ::init();
  this_player()->add_command("eat", this_object(), "<direct:object'cabbage'>" );    
  call_out( "check_env", 3 );
} /* init() */


void set_owner( object thing ) {
  if (!objectp( thing ) )
    return;
  set_long( (: long_desc :) );
  owner = thing;
  owner_name  = owner->query_name();
  owner_short = owner->query_short();
  language = owner->query_default_language();
  
  add_language( language );
  set_language( language );
  load_chat(30, ({
    1, "@roll up",
    1, "@purr",
    1, ":$V$0=ruffles its,ruffle their$V$ fronds.",
    1, ":$V$0=looks,look$V$ green.",
    3, "#soul_owner",
    1, "@roll over",
    1, "#check_env" }) );
    
  check_env();
} /* set_owner() */


string long_desc() {
  if ( owner && environment( owner ) == environment() ) {
    return "This small, cute-looking cabbage stares adoringly at " 
      + owner->the_short() + ".\n";
  }
  return "This small, cute-looking cabbage is looking off into the "
    "distance.  It is obviously hopelessly lost.\n";
} /* long_desc() */
        
        
void respond_nice( object souler ) {
  if ( !souler->query_visible(this_object()) )
    return;
  if ( owner && souler == owner) {
    do_command("nestle " + souler->query_name() );
  } else {
    do_command("peer curiously " + souler->query_name() );
  }
} /* respond_nice() */


void respond_nasty( object souler ) {
  if ( !souler->query_visible(this_object()) )
    return;
  if ( owner && souler == owner) {
    do_command("gasp");
    do_command("cry");
  } else {
    do_command( "sburn " + souler->query_name() + " dreams");
  }
} /* respond_nasty() */


int do_eat() {
  if ( this_player() != owner || eaten) {
    this_player()->add_failed_mess( this_object(), "You cannot eat this "
      "cabbage!\n" );
  return -1;
  }
  
  if( this_player()->query_property("dead") ) {
    this_player()->add_failed_mess( this_object(), "You are dead.  You "
      "cannot eat.\n" );
  }
    
#ifndef __DISTRIBUTION_LIB__
    if (SPECIAL_DAY_HANDLER->query_special_day() == CABBAGE_DAY) {
      this_player()->add_failed_mess(this_object(), "This is *my* day, "
      "how can you even think of eating me, your faithful cabbage!\n" );
      return -1;
    }
#endif
    
  this_player()->add_succeeded_mess( this_object(), 
    ({ "The cabbage lovingly and "
       "adoringly purrs quietly as you viciously rip off its fronds and "
       "eat it.  It dies looking wonderingly into your face.\n", 
       "$N $V $D.\n" }) );
    this_player()->add_effect("/std/effects/object/cabbage", 150);
    call_out("zap_cabbage", 2 );
    
    return 1;
} /* do_eat() */


void zap_cabbage() {        
  if ( environment() ) {
    all_inventory()->move( environment() );
  }
  if ( owner ) {
    do_command( "unfollow " + owner_name );
  }
  eaten = 1;
  owner = 0;
  owner_name = 0;
  owner_short = 0;
  move( "/room/rubbish" );
} /* zap_cabbage() */


int attack_by(object ob) {
  if( ( !query_owner() || ob == query_owner() ) && 
        base_name( previous_object() ) == LANCE )
    call_out( "coleslaw", 2 );
    
  ob->stop_fight(this_object());
  tell_object(ob, "The cabbage is a terrible pacifist.\n");
} /* attack_by() */

int attack_ob(object ob) {
  ob->stop_fight(this_object());
  tell_object(ob, "The cabbage is a terrible pacifist.\n");
} /* attack_by() */

int adjust_hp(int number, object attacker) {
  return 0;
} /* adjust_hp() */


void soul_owner() {
  string *souls = ({ "snuggle ", "nestle ", "stare dreamily ", "snuffle " });
  if(!owner)
    return;
  
  if ( environment() != environment( owner ) ) {
    do_command( "sniffle sadly" );
    return;
  } 
  if ( !owner->query_visible(this_object()) )
    return;
  
  do_command( souls[random(sizeof(souls))] + owner_name );
} /* soul_owner() */


void check_env() {
  object my_env, owner_env, *caterpillars;
  
  if ( !owner_name )
    return;

  my_env = environment();
    if ( !my_env ) {
        return;
    }
    
  if ( owner_name && !owner ) {
    do_command( "cry" );
    do_command( "'Woe is me! Where art thou, " + owner_short + "?" );
    call_out("zap_cabbage", 1);
    return;
  }
    
  owner_env = environment( owner );
  if ( !owner_env ) {
    return;
  }

  if ( file_name( my_env ) == "/room/rubbish" ) {
    return;
  }
    
  if ( this_object()->query_property( "dead" ) ) {
    return;
  }

  if (my_env == owner) {
    return ;
  }

  if (owner_env == my_env) {
    return ;
  }

  if (owner_env->query_mirror_room() == my_env) {
    return ;
  }
    
  caterpillars = match_objects_for_existence( "caterpillar", ({ owner_env }) );
    
  if ( my_env != owner_env && !sizeof(caterpillars)) {
    move( owner_env, "$N appear$s with a squelch.",
      "$N disappear$s with a squelch." );
    do_command( "follow " + owner_name );
  }

} /* check_env() */


object query_owner() { 
    return owner; 
} /* query_owner() */


/*
 * On Cabbage Day, we need to be moved out of a player's inventory.
 */
int query_theft_command() {
    return -1;
} /* query_theft_command() */


void coleslaw() {
  object ob;

  tell_room( environment(), the_short() + " explodes in a shower of "
      "fronds.\n" );
  ob = ARMOURY->request_object("coleslaw", 100);
  if (ob) {
     ob->move( environment() );
  }
  zap_cabbage();
} /* coleslaw() */
