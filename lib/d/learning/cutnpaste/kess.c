/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: kess.c,v 1.2 2000/07/02 18:39:00 sasquatch Exp $
 *
 * $Log: kess.c,v $
 * Revision 1.2  2000/07/02 18:39:00  sasquatch
 * Fixed her up so that she can use all of her rituals, calm, paralysis, fear
 * etc.  Also gave her rage as an option in her combat options.
 * In this change, I made her hauberk a holy amulet, needed for a few rituals.
 *
 * Revision 1.1  1998/01/22 13:49:06  gerbil
 * Initial revision
 *
*/
#define CREATOR "Ceres"
/*
 * This is Kess, the Archcardinal of Pishe. Another attempt by me to
 * create an interesting female npc.
 *
 * Kess has several interesting features:
 *   She joins fights
 *   She cuts hearts from corpses and eats them
 *   She uses rituals and her yellow rod
 *   She runs off it people hit her for too much damage in one go
*/
#include <armoury.h>
inherit "/obj/monster";

object rod;

// This function is called when Kess is loaded and does all her basic setup.
void setup() {
  object chain;

  // Give her a name, short and long
  set_name("kess");
  set_short("Kess of Sek");
  set_main_plural("Kesses of Sek");
  set_long( "This is Kess, an Archcardinal of Sek.  She is a young woman, "+
           "and beautiful in a twisted kind of way.  She seems fairly "+
           "innocuous until you notice her eyes and see the vicious look in "+
           "them.\n" );
  // by setting these adjective words any of them can be used in conjunction
  // with her name to identify her.
  add_adjective(({"archcardinal", "of", "sek"}));

  // other names that can be used to identify kess
  add_alias(({"priestess", "archcardinal", "cardinal"}));

  add_property("determinate","");   // dont put a the or a in front of her name
  add_property("unique", 1);        // mark her as unique
  set_gender("female");             // shes female

  // the race, class and level must be in the following order
  set_race("human");
  set_class("priest");
  set_deity("sek");
  set_level(260);

  // height and weight are optional, but used here
  set_height(140);
  set_weight(1000);
  set_al(2000);         // shes a priestess of sek so make her very bad

  // shes a pretty special npc so lets mess with her stats
  adjust_str(2);
  adjust_con(2);
  adjust_wis(5);
  adjust_int(-6);
  adjust_dex(-3);

  // making her a level 260 priest will have given her a bunch of skills
  // but here we make a few tweaks
  add_skill_level( "faith.items.rod", 250, 0 );
  add_skill_level( "fighting.combat.melee.blunt", 200, 0 );
  add_skill_level( "fighting.combat.parry.held", 200, 0 );

  // Give her the 'kick' command
  add_known_command("kick");

  // Give her some rituals. (spell name, spell file, function to call)
  add_spell( "paralysis", "/obj/rituals/paralysis",
            "cast_spell" );
  add_spell( "fear", "/obj/rituals/fear",
            "cast_spell" );
  add_spell("cure medium wounds", "/obj/rituals/curemedium",
            "cast_spell" );
  add_spell("calm", "/obj/rituals/calm",
            "cast_spell" );   
  add_spell( "rage", "/obj/rituals/rage",
            "cast_spell" );                 

  // This makes kess join in fights which are going on in the room shes in
  set_join_fights( "Blood-lust glows in Kess' eyes as she dives "+
                   "into the melee.\n" );
  set_join_fight_type(1);

  // Now give her some equipment
  chain = ARMOURY->request_armour("chainmail", 100);
  chain->add_ac("magic", "magic", 60);
  chain->add_ac("fire", "fire" , 60);
  chain->add_ac("cold", "cold", 60);
  chain->move(this_object());
  chain->add_property("holy amulet", "sek");
  chain->add_property("sek", 1);

  ARMOURY->request_armour("blood red cloak", 100)->move(this_object());
  ARMOURY->request_armour("rope belt", 100)->move(this_object());
  ARMOURY->request_armour("soft leather shoes", 100)->move(this_object());

  // This creates a cleric rod. Calling whichone(6) makes it a yellow rod
  rod = ARMOURY->request_weapon( "cleric rod", 100 );
  rod->whichone(6);           // make the rod a yellow one
  rod->set_enchant(8);        // give it a little enchantment (radiance)
  rod->move(this_object());   // move it to kess

  // her holy symbol which she needs for her rituals
  clone_object("/obj/faith/sek_symbol.arm")->move(this_object());

  init_equip();   // make her hold and wear her equipment

  // request a dagger from the armoury and give it to her all in one go.
  // the second parameter in request_weapon is it's condition (100%)
  ((object)ARMOURY->request_weapon("dagger", 100))->move(this_object());

  // setup her combat tactics
  do_command("tactics response parry");
  do_command("tactics parry both");
  do_command("tactics attitude offensive");

  // normal chats
  load_chat(20, ({2, ":smiles viciously.",
                  1, "@glare $lname$",
                  1, "@smirk",
                }));

  // chats given while in combat
  load_a_chat(20, ({1, "'Do you know who I am?",
                    1, "'Die unbeliever!",
                    1, "'You fight me, Sek will rip your heart out!"}) );

  set_virtual_move(1);           // allow her to move virtually
  set_move_after(30, 120);       // move after 30+random(120) seconds
  add_move_zone("Ankh-Morpork"); // move around AM 
  add_move_zone("sek");          // and the temple of sek

  // this calls the function get_heart() whenever she enters a room
  // see later for more details
  add_enter_commands("#get_heart");

  // Combat actions are called during combat by the combat system.  It is an
  // easy way to have an npc react suitably or do things while fighting.
  // The format is: likelihood, name, action.
  // You'll find maintain_self, fighting_stuff and reequip are functions
  // further down.
  add_combat_action(100, "maintenance", ({ "maintain_self" }) );
  add_combat_action(70, "fighting_stuff", ({ "fighting_stuff" }) );
  add_combat_action(50, "reequip", ({ "reequip" }));
}

// The following functions are combat actions

// if kess isn't holding her rod, then get the rod and hold it.
void reequip(object thing1, object thing2) {
  if(this_object()->query_holding()[0] != rod) {
    do_command( "get yellow rod" );
    do_command( "hold yellow rod" );
  }
}

// Perform a variety of actions to make her combat more interesting
void fighting_stuff(object thing1, object thing2) {
  if(thing2 != this_object())
    return;

  // don't do something every round.
  switch(random(6)) {
  case 5:
    do_command("cast rage on "+thing1->query_name());
    break;       
  case 4:
    do_command("cast fear on "+thing1->query_name());
    break;
  case 3:
    do_command("cast paralysis on "+thing1->query_name());
    break;
  case 2:
    do_command("use rod to cast dustdevil");
    break;
  case 1:
    do_command("use rod to cast barrier");
    break;
  default:
    do_command("concentrate on "+thing1->query_name());
    do_command("kick "+thing1->query_name());
    break;
  }
}

// healing actions 
void maintain_self( object thing1, object thing2 ) {
   if ( thing2 != this_object() )
      return;

   // we increase her guild points if they need it. Otherwise she'll
   // just runout.
   if ( query_gp() < 200 )
      adjust_gp( 200 );

   // the following commands are performed when she reaches a percentage
   // of her maximum hit points
   if ( query_hp() < query_max_hp() / 4 )
     do_command( "use rod to cast sanctuary" );
   
   if ( query_hp() < query_max_hp() / 2 )
     do_command( "cast calm on "+thing1->query_name());

   if ( query_hp() < query_max_hp() )
     do_command( "cast cure medium wounds on kess" );

   
}

// This function is called when someone or something dies in the room.
void event_death(object thing) {
  if(thing != this_object()) {      // it it wasn't us who died
    call_out("get_heart", 3);       // call the get_heart function in 3 seconds
  }
}

// This function is called either when someone dies (see above) or when
// kess enters a room (see the setup function)
void get_heart() {
  object corpse;

  // search the room we're in for objects which respond to the name
  // 'corpse' if we find one then do the commands listed
  if(find_match("corpse", environment()) != ({})) {
    do_command("hold dagger");
    do_command("get heart from corpse");
    do_command("eat heart");
    do_command("hold rod");
  }
}

// Every npc has an adjust_hp() function. It is what is used to reduce
// and replace an npcs hit points. We are 'overloading' it here. Because
// we define adjust_hp() this is the one that will get called.
int adjust_hp(int hp, object attacker) {
  // first call adjust_hp() in our parent object since we still want the
  // normal adjust_hp() things to happen.
  ::adjust_hp(hp, attacker);

  // If we took more than 300 hits in a single go then...
  if(hp < -300) {
    // init_command() makes the command be performed in about 2 seconds
    init_command("cast cure medium wounds on kess");
    // we might also choose to run away. we have to do it as a callout
    // otherwise kess will runoff in the middle of an ambush.
    if(!random(3))
      call_out("eek", 0);
  }
}

// Lose everyone, then run away. run_away() is a standard npc function.
void eek() {
  do_command("lose all");
  run_away();
}
