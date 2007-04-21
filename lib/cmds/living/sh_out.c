/*  -*- LPC -*-  */
/*
 * $Locker: pinkfish $
 * $Id: sh_out.c,v 1.29 2002/08/03 23:16:49 danbala Exp pinkfish $
 */
#include <player.h>
#include <drinks.h>
#include <language.h>
#include <cmds/options.h>
#include "/d/am/path.h"

inherit "/cmds/base";
inherit "/cmds/speech";

#define TP this_player()
#define BEEP sprintf("%c",7)

#define NO_QUIT_INVENTORY 1
#define NO_QUIT_COMBAT    2
#define BROADCASTER "/obj/handlers/broadcaster"

int query_no_quit();
string query_shout_word_type(string str);
void my_mess(string fish, string erk);

/* ok... the shout cost is charged for every 5 letters, plus a bonus
 * one for the start charge..  SO a shout of yes will cost 1 social
 * point... where as a shout of lots a letters will cost lots
 */
mixed cmd(string str) {
  string s1, s2, s;
  object g;
  string cur_lang;
  object lag;
  int tim, cost;

  if(!str || str == "")
    return notify_fail("Syntax : shout <text>\n");

  if (!environment(this_player()))
    return notify_fail( "You appear to be in limbo...\n" );

  if(TP->query_property("dead") == 1)
    return notify_fail("You don't have any lungs, how can you shout?\n");

  if (TP->check_earmuffs("shout"))
    return notify_fail("Why shout when you can't hear people "
                       "shout back?\n");

  cur_lang = TP->query_current_language();

  if (!LANGUAGE_HAND->query_language_spoken(cur_lang))
    return notify_fail(capitalize(cur_lang)+" is not a spoken language.\n");

  if (!LANGUAGE_HAND->query_language_distance(cur_lang))
    return notify_fail(capitalize(cur_lang)+" is not able to spoken "
                       "at a distance.\n");

  if (TP->query_invis())
    return notify_fail("Invisible people cannot shout, they cannot "
                       "see their arm in front of their face.  Shouting is "
                       "out of the question.\n");

  // Added by Obilix 9/4/97
  if (TP->query_property("gagged"))
    return notify_fail("You have been gagged!  You will not be able "
                       "to shout again until the gag is removed.  Perhaps "
                       "you should talk to a creator about this.\n");

  if (TP->query_property("recently shouted")) {
    return notify_fail("You have shouted very recently, perhaps you "
                       "should give your lungs a small break and try again "
                       "in a couple seconds.\n");
  }

  if(TP->query_property("player") == 1 && !TP->query_creator()) {
    cost = SHOUT_COST * ((strlen(str) / 5) + 1);
    // Capitals and punctuation cost extra.
    cost += sizeof(filter(explode(str, ""), (: $1 <= "Z" :))) / 2;

    if(TP->adjust_sp(-cost) < 0)
      return notify_fail(NO_POWER);

    TP->adjust_max_sp(- (cost / 5));
  }

  if (!interactive(TP)) {
    str = TP->convert_message( str );
    str = TP->fit_message( str );
  }

  s1 = query_shout_word_type(str);

  if (s1 != "yell")
    s = "shouts" + s1;
  else
    s = s1 + "s";

  if(TP->query_property(SHORTHAND_PROP))
    str = fix_shorthand(str);

  if(TP->query_volume( D_ALCOHOL))
    str = drunk_speech( str );
  TP->remove_hide_invis("hiding");

  if (s1 != "yell") {
    if (cur_lang != "common") s1 += " in " + capitalize(cur_lang);
    my_mess("You shout" + s1 + ": ", str);
  } else {
    if (cur_lang != "common") s1 += " in " + capitalize(cur_lang);
    my_mess("You " + s1 + ": ", str);
  }

  s2 =  " " + lower_case(str);

  /* Who zapped the old 8 code? */
  /* Damn it used to be quite clever... grrr.  could have least #ifdefed it
   * - pf */

  /* Remove this and face the wrath of Bel-Shamharoth. */
  /* And yes, this is in the books....                 */

  if (sscanf(" "+s2, "%s eight%s", s1, s1)==2 ||
      sscanf(s2, "%s8%s", s1, s1)==2) {

    s1 = replace(str, ({ "8", "", " eight", "", " ", "" }));

    switch (query_no_quit()) {
    case NO_QUIT_INVENTORY:
      return notify_fail("You only just arrived.  You are far too "
                         "busy strapping on equipment to shout like that.\n");
    case NO_QUIT_COMBAT:
      return notify_fail("You are too busy fighting to shout "
                         "like that.\n");
    default:
      if (s1 == "") {
        /* Was just a string of 8's and spaces.... */
        write("Oops...\n");
        call_out( "summon_bel_shamharoth", 10, TP );
      }
      else {
        g = (object)TP->query_guild_ob();
        if (random(100) < 40 ||
            (g && (string)g->query_name() == "wizards"))
          write("Oops...\n");
        call_out( "summon_bel_shamharoth", 10, TP );
      }
    }
  }


  /* Check for lag shouts... */
  /* This isn't in the books, but I don't care  */
  lag = load_object(SHORT + "short20")->lag_for_shout();
  if(lag && sscanf(" "+lower_case(s2), "%*s lag%*s") == 2) {

    //        catch(lag = LAG_ROOM->lag_for_shout());

    tim = TP->query_property("lag shout time");

    /* Make it time out so it does not clutter up stuff */
    TP->add_property("lag shout time", time(), 300);

    // that'll teach them! Ceres
    if((tim + 300 > time()) && lag != TP) {

      switch (query_no_quit()) {
      case NO_QUIT_INVENTORY:
        lag->init_command("hug "+ TP->query_name() +
                          " Welcome to " + mud_name() + "", 0);
        return notify_fail("The air is suddenly squeezed out "
                           "of you.\n");

      case NO_QUIT_COMBAT:
        lag->init_command("wedgie " + TP->query_name(), 0);
        return notify_fail("You are rudely interrupted "
                           "mid-shout.  Fortunately you are not distracted "
                           "from the fight.\n");

      default:
        call_out( "summon_bel_shamharoth", 10, TP, 1);
      }
    }
  }

  if(environment(this_player()) &&
     environment(this_player())->query_property("location") == "inside")
    tim = 10;
  else
    tim = 3;
  
  user_event( this_player(), "person_shout",
              (string)this_player()->a_short() +" "+ s,
              str, cur_lang,
              (int *)environment( this_player() )->query_co_ord(),
              ( 100 * ( (int)this_player()->query_con() +
                        (int)this_player()->query_str() ) ) / tim );
  /*
    TP->comm_event(users(), "person_shout", "$a_short:"+ file_name(TP) +
    "$ "+ s, str, cur_lang);
  */
  environment(TP)->event_person_shout( 0, (string)TP->a_short() +" "+ s,
                                       str + "%^RESET%^", cur_lang );

  BROADCASTER->npc_shout_event(TP, (string)TP->a_short() +" "+ s,
                               str + "%^RESET%^", cur_lang,
                               (int *)environment(TP)->query_co_ord(),
                               (100 * ((int)TP->query_con() +
                                       (int)TP->query_str() ) ) / 3);

  if (TP->query_property("player") == 1 && !TP->query_creator()) {
    TP->add_property("recently shouted", 1, 2);
  }

  return 1;
}

string query_shout_word_type(string str) {
  switch (str[<1]) {
  case '!':
    return "yell";
  case '?':
    return " asking";
  default:
    return "";
  }
}

void my_mess(string fish, string erk) {
  if(!interactive(TP))
    return;

  printf("%s%-=*s\n", fish, TP->query_cols() - strlen(fish),
         TP->fix_string(erk));
}

void summon_bel_shamharoth(object who, int lag_shout) {
  object env;

  if (!who || !objectp(who))
    return;

  if (!random(3))
    who->adjust_tmp_con(-2);

  if (!random(3))
    who->adjust_tmp_int(-2);

  if (!random(3))
    who->adjust_tmp_wis(-2);

  if (!random(3))
    who->adjust_tmp_dex(-2);

  env = environment(who);

  if ("/secure/master"->query_lord(geteuid(who))) {
    BROADCASTER->broadcast_event(users(),
                                 (int *)env->query_co_ord(),
                 "The heavens shake with a hideous roar and just as suddenly "
                 "all is quiet.\n$C$"+ who->query_name() + " chuckles in the "
                 "distance.\n",
                                 ( 100 * ( (int)who->query_con() +
                                           (int)who->query_str() ) ) / 3,
                                 1, 0);
    tell_object(who, "Your lordliness saves you from a grizzly "
                "encounter with the Sender of Eight.\n");
  } else {
    BROADCASTER->broadcast_event(users(),
                                 (int *)env->query_co_ord(),
                             "a sudden chill passes through the land as $C$" +
                           who->query_name() + " is carried off screaming "
                           "to the land of shades.\n",
                                 ( 100 * ( (int)who->query_con() +
                                           (int)who->query_str() ) ) / 3,
                                 1, 0);
    tell_object( who, "You think you'd best be careful what you "
                 "shout in the future as shadowy tentacles drag you "
                 "into the ground...\n" );

    if(lag_shout)
      BROADCASTER->broadcast_event(users(),
                                   (int *)env->query_co_ord(),
                                   ( 100 * ( (int)who->query_con() +
                  "The heavens shake with a hideous chuckle and in the "
                  "distance a booming yet apologetic voice says: Oops.\n",
                                             (int)who->query_str() ) ) / 3,
                                   1, 0);

    if(base_name(env) != "/room/departures")
      who->quit();
  }
}

/**
 * check to see if player can quit voluntarily
 * @returns non zero code if the player may not voluntarily quit
 */
int query_no_quit() {

  /* "You cannot quit yet: your inventory is still being generated.\n" */
  if (TP->query_save_inhibit() || TP->query_auto_loading()) {
    return NO_QUIT_INVENTORY;
  }

  /* "You cannot quit while in combat.\n" */
  if (sizeof(filter_array(TP->query_attacker_list(), (: living($1) :)))) {
    return NO_QUIT_COMBAT;
  }

  return 0;
}
