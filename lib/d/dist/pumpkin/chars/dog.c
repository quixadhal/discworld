/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: am_dog.c,v 1.3 2000/01/09 00:52:03 rue Exp $
 *
 * $Log: am_dog.c,v $
 * Revision 1.3  2000/01/09 00:52:03  rue
 * Added gender
 *
 * Revision 1.2  1999/04/11 21:50:06  ranma
 * typo.
 *
 * Revision 1.1  1998/02/08 08:44:56  terano
 * Initial revision
 *
*/
/* -*- LPC -*-
 */
inherit "/obj/monster";

#include "path.h"

string colour, type;

/* predefs */
void check_for_hidden();
void test_rabid_attack();
void test_urinate();
void pat_me( object per );
void sic_em( object per, string mess );

string query_colour() { return colour; }

void setup() {
   colour = ({ "light brown", "black", "grey", "mottled", "white", "patched",
         "dark brown", "black and white", "fluorescent yellow" })[ random( 9 ) ];
  set_name("dog");
  set_race("dog");
  set_gender(1 + random(2));
  set_level( 1 + random( 1 + random( 3 ) ) );
  add_adjective("hairy");
  add_adjective(explode(colour, " "));
} /* setup() */

void set_type(string type) {
   set_short( type +" "+ colour +" dog" );
   set_main_plural( type +" "+ colour +" dogs" );
   add_adjective( type );
   if ( type == "small" ) {
      set_long("The small "+colour+" dog is very hairy.  It looks a "
                 "bit sad and lost in the streets of Pumpkin.  "
                 "You are sure it would prefer a cat flap and a nice "
                 "hot meal every night, or is that you?\n");
   } else {
      add_skill_level( "fighting", random( 10 + random( 30 ) ) );
      add_skill_level( "other.health", random( 10 + random( 30 ) ) );
      set_main_plural("large "+colour+" dogs");
      add_adjective("large");
      set_long("The large "+colour+" dog confidently walks the streets, "
                 "insolently not moving aside for the warriors and heroes "
                 "stalking the streets.\n");
   }
  /* Make them bark at hiding people */
   add_enter_commands( "#check_for_hidden" );

   if ( !random( 3 ) )
     add_effect( "/std/effects/disease/flea_infestation", 10000 );

  if (!random(6)) {
    /* A rabid dog... */
    set_wimpy(10);
    load_chat( 10, ({
       1, "@froth",
       1, "@roll eye",
       1, "@growl",
       1, "#test_rabid_attack",
    }) );
    load_a_chat( 10, ({
       1, "@froth",
       1, "@roll eye",
       1, "@growl" }) );
    add_respond_to_with( ({ ({ "@pat", "@stroke", "@hug" }),
                             query_name() }), "bite $hname$");
  } else {
    set_wimpy( 70 );
    load_chat( 5, ({
       1, (type=="small"?":whines pathetically.":"@growl"),
       1, (type=="small"?"@stare hungrily at $lname":
                             "@stare int at $lname"),
       1, (type=="small"?"@bite $lname ankles":"@stare rud at $lname"),
       1, "#test_urinate",
    }) );
    load_a_chat( 50, ({
       1, "@bite $aname ?",
       1, "@growl",
       1, "@bark",
    }) );
    add_respond_to_with( ({ ({ "@pat", "@stroke", "@hug" }),
                                query_name() }), "#pat_me" );
    add_respond_to_with( ({ "@say", ({ "get", "sic" }), }), "#sic_em" );
  }

  add_effect("/std/effects/npc/eat_edible", 0);

  add_effect("/std/effects/npc/i_died", ({ HOSPITAL, "regen_after_death" }));
  //add_triggered_action("regen", "death", HOSPITAL, "regen_after_death");
  add_property("monster_type", type);
  add_property("animal type", type);
} /* setup() */

void bark_at_hidden(object per) {
  mapping hide_invis;
 
  if (per) {
    hide_invis = per->query_hide_invis();
    if (environment(per) == environment() &&
        hide_invis["hiding"]) {
      do_command("bark loudly");
      if (!random(4))
        do_command("bark loudly at "+
                 per->query_name());
      else
        do_command("bark loudly");
      call_out("bark_at_hidden", 2, per);
    }
  }
} /* bark_at_hidden() */

void check_for_hidden() {
  object *obs, mine;
  int i;
  mapping hide_invis;
  function fun;

  mine = query_property("following");
  obs = all_inventory(environment(this_object()));
  for (i=0;i<sizeof(obs);i++) {
    hide_invis = query_hide_invis();
    if (living(obs[i]) && hide_invis["hiding"] &&
        obs[i] != mine && obs[i]->query_visible(this_object())) {
      call_out("bark_at_hidden", 0, obs[i]);
      break;
    }
  }
} /* check_for_hidden() */

void test_rabid_attack() {
  int i;
  object *obs;

  obs = filter_array(all_inventory(environment()),
                     (: living($1) && $1 != $2 &&
                        (!$1->query_property("player") ||
                         $1->query_level() > 5) :),
                      this_object());
  if (sizeof(obs))
    attack_ob(obs[0]);
} /* test_rabid_attack() */

void test_urinate() {
  object *obs, *liv, blue, mine;
  int i;
  
  liv = ({ });
  mine = query_property("following");
  obs = all_inventory(environment());

  /* Recheck for hidding people. */
  check_for_hidden();

  /* Track down those living objects */
  for (i=0;i<sizeof(obs);i++) {
    if (living(obs[i]) && obs[i] != mine &&
        obs[i] != this_object()) {
      liv += obs[i..i];
    }
  }

  /* Ok, found someone.  Lets do it! */
  if (sizeof(liv)) {
    blue = liv[random(sizeof(liv))];
    tell_room(environment(),
              the_short() + " urinates on " + blue->the_short() + "'s leg.\n",
              ({ blue }));
    tell_object(blue, capitalize(the_short()) + " urinates on your leg.\n");
    blue->add_effect("/std/effects/other/dog_urinate", 120);
  }
} /* test_urinate() */

void i_like_them(object them) {
  if(!them || environment() != environment(them))
    return ;

  call_out("i_like_them", 60+random(240), them);
  if (query_property("excited")) {
    do_command("bounce excit");
  } else switch (random(5)) {
    case 0 :
      do_command("nuzzle "+them->query_name());
      break;
    case 1 :
      do_command("stare pitifully at "+them->query_name());
      break;
    case 2 :
      do_command("smile "+them->query_name());
      break;
    case 3 :
      do_command("eye "+them->query_name());
      break;
    case 4 :
      do_command("drool "+them->query_name());
      break;
  }
} /* i_like_them() */

void pat_me(object per) {
  object ob;

  ob = query_property("following");
  if (objectp(ob) &&
      environment(ob) == environment()) {
    init_command("nuzzle "+per->query_name());
  } else if (environment() == environment(per) &&
   (query_property("monster type") == "small dog" ||
   !random(5))) {
    init_command("follow "+per->query_name());
    init_command("protect "+per->query_name());
    add_property("following", per);
    call_out("i_like_them", 60+random(240), per);
  } else {
    init_command("ignore "+per->query_name());
  }
} /* pat_me() */

void periodic_excited() {
  if (query_property("excited")) {
    if (query_property("excited") > 1) {
      switch (random(4)) {
        case 0 :
          do_command("growl");
          break;
        case 1 :
          do_command("bark");
          break;
        case 2 :
          do_command("caper");
          break;
        case 3 :
          do_command("bounce excit");
          break;
      }
    } else if (!random(2))
      do_command("bounce excit");
    else
      do_command("caper");
    call_out("periodic_excited", 10+random(10));
  }
} /* periodic_excited() */

void sic_em(object per, string mess) {
   string *bing;
   object mine;
   object *obs;
   int lvl;
   
   bing = explode(replace(mess, "!", ""), " ");
   mine = query_property("following");
   if (bing[0] == "get" && per == mine) {
      obs = match_objects_for_existence(bing[1], environment());
      obs = filter_array(obs, (: living($1) :));
      if ( sizeof( obs ) ) {
	 lvl = query_property( "excited" );
	 add_property( "excited", lvl+1 );
	 call_out( function( object npc ) {
	    int lvl;
	    
	    lvl = npc->query_property("excited");
	    if (lvl > 0)
	       npc->add_property("excited", lvl - 1);
	    else
	       npc->remove_property("excited");
	 }, 120, this_object());
	 if (!lvl) {
	    call_out("periodic_excited", 10+random(10));
	 }
	 switch (lvl) {
	  case 0 :
	    init_command("bounce excit");
	    break;
	  case 1 :
	    init_command("growl "+obs[0]->query_name());
	    break;
	  default :
	    call_out((: attack_ob($2) :), 0, obs[0]);
	    break;
	 }
      }
   }
} /* sic_em() */
