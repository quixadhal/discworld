/*  -*- LPC -*-  */
/*
 * $Locker: ceres $
 * $Id: t_ell.c,v 1.64 2003/02/15 20:09:03 pinkfish Exp ceres $
 * 
*/
#include <drinks.h>
#include <player.h>
#include <language.h>

#define TP this_player()

#define TELL_REPLY_LOCK_ID "lock fluff"

/* Behaviour when we encounter ear muffs in player */
#define CUT_THOUGH_PROPERTY "cut earmuffed tells"
#define FAIL_DONT_ASK 0
#define ASK 1
#define CUT_DONT_ASK 2

inherit "/cmds/base";
inherit "/cmds/speech";


/* This function handles tells from cres to players who
 * have tells ear muffed. */
void delayed_tell(string yesno,
                  object * recicipients,
                  string message);


#ifdef USE_SMILEYS
string *two_smileys = ({ ":)", ":(", ":P", ":p", ":b", ";)",
                           ";(", ";P", ";p", ";b", "=)", "=("
                           });

string *three_smileys = ({ ":-)", ":-(", ":-P", ":-p", ":-b",
                             ";-)", ";-(", ";-P", ";-p", ";-b", ":o)", ":o(",
                             ":oP", ":op", ":ob", ";o)", ";o(", ";oP", ";op",
                             ";ob", "=-)", "=-(", "=o)", "=o("
                             });
#endif

int cmd(string arg, mixed thing, int silent) {
  class message mess;
  string words,
    word;
  string them_mess,
    extra;
  string *ok_string;
  string *prop;
  object *obs;
  object *net_dead;
  object *multiple_earmuffed;
  object *cannot_see;
  object *ok;
  object *fail;
  object *earmuffed;           /* These people have ear muffed tells */
  object *roleplaying;
  mixed busy;
  
  if (pointerp(thing)) {
    words = arg;
    thing -= ({ 0 });
    if (TP && environment(TP) &&
        function_exists("trap_tell", environment(TP)) &&
        member_array(this_object(), previous_object(-1)) == -1) {
      return (int) environment(TP)->trap_tell(words, thing, 0);
    }
  } else if (!objectp(thing)) {
    if (!arg || sscanf(arg, "%s %s", word, words) != 2) {
      return 0;
    }
    word = lower_case(word);
    word = (string) TP->expand_nickname(word);

    if (sizeof(thing = explode(word, ",")) == 1)
      thing = ({ find_player(word) });
    else {
      thing = map(thing, (: find_player :));
    }

    thing -= ({ 0 });
    if (sizeof(thing) == 0) {
      if (TP->query_creator() && sscanf(word, "%*s@%*s") == 2) {
        "/net/daemon/out_tell"->do_tell(word + " " + words);
        return 1;
      }
      add_failed_mess(capitalize(word) + " is not logged in.\n");
      return 0;
    }
    if (environment(TP)) {
      if (function_exists("trap_tell", environment(TP)) &&
          member_array(this_object(), previous_object(-1)) == -1) {
        return environment(TP)->trap_tell(words, thing, 0);
      }
    }
  } else {
    thing = ({ thing }) - ({ 0 });
    words = arg;
  }

  /* Don't allow someone with tells ear muffed to use tell. */
  if (TP->check_earmuffs("tell")) {
    if (TP->query_earmuffs() == PLAYER_ALLOW_FRIENDS_EARMUFF) {
      fail = filter(thing, (: !TP->is_friend($1->query_name()) &&
                            $1 != this_player() :));
      if (sizeof(fail)) {
        add_failed_mess("One of the people ($I) you "
                        "are trying to tell is not a "
                        "friend (and you have tells earmuffed).\n",
                        fail );
        return -1;
      }
    } else {
      add_failed_mess("You have tells ear muffed.\n");
      /* return 0 gave us two messages to player. */
      return -1;
    }
  }

  if(TP->query_role_playing()) {
    add_failed_mess("You cannot send tells as you are currently "
        "role playing.\n");
    return -1;
  }
  
  mess = build_message(words, thing, "tell");
  switch(mess->status) {
  case NOT_DISTANCE:
    return add_failed_mess("The language " + capitalize(mess->language) +
                           " is not able to used over a distance.\n");
  case NOT_SPOKEN:
    return add_failed_mess(capitalize(mess->language) +
                           " is not a spoken language.\n");
  case NO_MESSAGE:
    return add_failed_mess("No message given.\n");
  }
  obs = thing - ({ this_player() });

  if (!sizeof(obs)) {
    add_failed_mess("Talking to yourself again.  I don't know.\n");
    return 0;
  }

  if (sizeof(obs) > 20) {
    add_failed_mess("You can only tell up to 20 people a message.\n");
    return 0;
  }

  fail = this_player()->query_ignoring(obs);
  if (sizeof(fail)) {
    write("You are currently ignoring " +
          query_multiple_short(fail, 0, 1, 0, 0) + ", so " +
          (sizeof(fail) > 1 ? " they" : fail[0]->query_pronoun()) +
          " couldn't reply anyway.\n");
    obs -= fail;
    if (!sizeof(obs)) {
      return 1;
    }
  }

  fail = this_player()->query_ignored_by(obs);
  if (sizeof(fail)) {
    if (!this_player()->query_creator()) {
      write("You are currently being ignored by " +
            query_multiple_short(fail, 0, 1, 0, 0) + ", so "
            "you cannot tell them anything.\n");
      obs -= fail;
      if (!sizeof(obs)) {
        return 1;
      }
    } else {
      write("Warning!  " + query_multiple_short(fail, 0, 1, 0, 0) +
            " have you on ignore.\n");
    }
  }

  if (!TP->query_creator() && (TP->adjust_sp(-TELL_COST) < 0)) {
    return notify_fail(NO_POWER);
  }

  net_dead = ({ });
  multiple_earmuffed = ({ });
  earmuffed = ({ });
  ok = ({ });
  roleplaying = ({ });
  cannot_see = ({ });

  if (sizeof(obs) > 1 && this_player()->check_earmuffs("multiple-tell")) {
    add_failed_mess("You cannot do a multiple-tell if you have them "
                    "earmuffed yourself.\n");
    return 0;
  }

  foreach(thing in obs) {
    if (sizeof(obs) > 1 && thing->check_earmuffs("multiple-tell")) {
      multiple_earmuffed += ({ thing });
    } else if (thing->check_earmuffs("tell")) {
      if (TP->query_creator() &&
          TP->query_property(CUT_THOUGH_PROPERTY) == CUT_DONT_ASK) {
        /* A cre that doesn't care about ear muffs, so we warn the
         * cre and player, but don't add to the ear muffed array. */
        write("You cut through the ear muffs of " +
              thing->the_short() + ".\n");
        tell_object(thing, TP->the_short()
                    + " cuts through your ear muffs.\n");
        /* Add to array of people who get the message. */
        ok += ({ thing });
      } else {
        /* Ok, someone cares, so we add to earmuffed array. */
        earmuffed += ({ thing });
      }
    } else if(thing->query_role_playing()) {
      if (TP->query_creator() &&
          TP->query_property(CUT_THOUGH_PROPERTY) == CUT_DONT_ASK) {
        /* A cre that doesn't care about ear muffs, so we warn the
         * cre and player, but don't add to the ear muffed array. */
        write("You cut through the roleplaying mode of " +
              thing->the_short() + ".\n");
        tell_object(thing, TP->the_short()
                    + " cuts through your roleplaying mode.\n");
        /* Add to array of people who get the message. */
        ok += ({ thing });
      } else {
        /* Ok, someone cares, so we add to earmuffed array. */
        roleplaying += ({ thing });
      }
    } else if (!this_player()->query_invis() ||
               reference_allowed(this_player(), thing)) {
      /* All is hoopy. */
      ok += ({ thing });
    } else {
      cannot_see += ({ thing });
      ok += ({ thing });
    }
  }

  ok_string = map(ok, (: $1->query_name() :)) +
    ({ this_player()->query_name() });

  switch (mess->type) {
  case "exclaim":
    extra = " to";
    break;
  default:
    extra = "";
    break;
  }

  foreach(thing in ok) {
    them_mess = query_multiple_short(map(ok- ({ thing }),
                                         (: $1->short(0, 0) :)) +
                                     ({ "you"  }), 0, 1, 0, 0);
    if ( interactive( this_player() ) ) {
      thing->event_person_tell(TP, capitalize((string) TP->short(0, 0)) +
                               " " + mess->emote + mess->type + "s" + extra +
                               " " + them_mess + ": ",
                               mess->text);
    }
    else {
      thing->event_person_tell(TP, capitalize( TP->the_short()) +
                               " " + mess->emote + mess->type + "s" + extra +
                               " " + them_mess + ": ",
                               mess->text);
    }
    // Lasts for 15 minutes.
    prop = thing->query_tell_reply_list();
    if (prop && prop[0] == TELL_REPLY_LOCK_ID) {
      // Refresh if one of the locked people tells us.
      if (member_array(TP->query_name(), prop) != -1) {
        thing->set_tell_reply_list(prop);
      }
    } else {
      thing->set_tell_reply_list(ok_string);
    }
    if (!interactive(thing) && userp(thing)) {
      net_dead += ({ thing });
    }
  }

  if (!silent) {
    if (sizeof(net_dead)) {
      write("Warning: " +
            query_multiple_short(map(net_dead, (: $1->query_cap_name() :)), 0,
                                 1, 0, 0) + " " +
            (sizeof(net_dead) > 1 ? "are" : "is") + " net dead.\n");
    }
    if (sizeof(multiple_earmuffed)) {
      write(capitalize(query_multiple_short(multiple_earmuffed, 0, 1, 0, 0)) +
            (sizeof(multiple_earmuffed) > 1 ? " have " : " has ") +
            "multiple tells ear muffed.\n");
    }
    if (sizeof(earmuffed)) {
      write(capitalize(query_multiple_short(earmuffed, 0, 1, 0, 0)) +
            (sizeof(earmuffed) > 1 ? " have " : " has ") +
            "tells ear muffed.\n");
      if (TP->query_creator() &&
          TP->query_property(CUT_THOUGH_PROPERTY) != FAIL_DONT_ASK) {
        write("Cut through ear muffs? (y/n) ");
        input_to((: delayed_tell :), earmuffed, words);
      }
    }
    if (sizeof(roleplaying)) {
      write(capitalize(query_multiple_short(roleplaying, 0, 1, 0, 0)) +
            (sizeof(roleplaying) > 1 ? " are " : " is ") +
            "role playing.\n");
      if (TP->query_creator() &&
          TP->query_property(CUT_THOUGH_PROPERTY) != FAIL_DONT_ASK) {
        write("Cut through ear muffs? (y/n) ");
        input_to((: delayed_tell :), earmuffed, words);
      }
    }
    busy = this_player()->query_busy();
    if (busy == 1) {
      write("Warning! You have your busy flag on.\n");
    } else if (pointerp(busy) && sizeof(ok - busy)) {
      write("Warning! You are currently set as busy with " +
            query_multiple_short(busy, 0, 1, 0, 0) + ".\n");
    }

    if (sizeof(cannot_see)) {
      write("Warning! " + query_multiple_short(cannot_see, 0, 1, 0, 0) +
            " cannot see you and will not be able to respond.\n");
    }

    if (sizeof(ok)) {
      them_mess =
        TP->convert_message(query_multiple_short(map(ok, (:
$1->short(0,0) :)), 0, 1, 1, 0));
      my_mess("You " + mess->emote + mess->type + extra + " " +
              them_mess + ": ", mess->text);
      TP->add_tell_history("You " + mess->emote + mess->type + extra +
                           " " + them_mess + ": ", mess->text);
    } else if (TP->query_creator() && !sizeof(earmuffed)) {
      /* We don't want this message *and* the one about cutting
       * * through ear muffs, so if the player is a cre and has
       * * already gotten the (y/n) stuff, we skip this. */
      add_failed_mess("It seems no one was listening.\n");
      /* return 0 gave two messages. */
      return -1;
    }
  }
  TP->adjust_time_left(-5);
  return 1;
}

int reply_cmd(string mess) {
  string *rep;
  object *obs;

  rep = this_player()->query_tell_reply_list();
  if (!rep) {
    add_failed_mess
      ("No one has told you anything in the last 15 minutes.\n");
    return 0;
  }

  obs = map(rep, (: find_living($1) :)) - ({ 0 });
  if (!sizeof(obs)) {
    add_failed_mess("None of " + query_multiple_short(rep, 0, 1, 0, 0) +
                    " are currenttly online.\n");
    return 0;
  }

  this_player()->set_tell_reply_list(rep);
  return cmd(mess, obs, 0);
}

int replylock_cmd(string names) {
  string *rest;
  string *rep;
  rep = uniq_array(explode(names, ","));
  rep = map(rep, (: this_player()->expand_nickname($1) :));
  rest = filter(rep, (: $1 && find_player($1) && interactive(find_player($1)) :));

  if(!sizeof(rest)) {
    add_failed_mess("None of " + query_multiple_short(rep, 0, 1, 0, 0) +
                    " are logged in.  You cannot lock onto someone who is "
                    "not here.\n");
    return 0;
  }

  this_player()->set_tell_reply_list(({ TELL_REPLY_LOCK_ID }) + rest);
  add_succeeded_mess(({ "You lock in " +
                          query_multiple_short(rest, 0, 1, 0, 0) +
                          " in your reply list.\n", "" }));
  return 1;
}

int replylockremove_cmd() {
  this_player()->set_tell_reply_list(0);
  add_succeeded_mess(({ "You remove any possible reply locks.\n", "" }));
  return 1;
}

mixed *query_patterns() {
  return ({ "<indirect:player> <string'message'>",
              (: cmd($4[1], $1, 0) :),
              "reply <string'message'>", (: reply_cmd($4[0]) :),
              "replylock <string'names'>", (: replylock_cmd($4[0]) :),
              "replylock remove", (: replylockremove_cmd() :),
              "<string>", (: cmd($4[0], 0, 0) :) });
}


/*
 * Tells from cres to ear muffed players.
 * All emoticon and "asks you" or "exclaims" stuff removed.
 */
void delayed_tell(string yesno,
                  object * recicipients,
                  string message) {
  object ob,
    *netdead;
  int disappeared;

  /*
   * Tried to keep us out
   * But nothing stops creators
   * Except alter.net
   */

  /* Did they really want to break through? */
  if (!(yesno == "yes" || yesno == "y")) {
    write("Ok, tell aborted.\n");
    return;
  }

  netdead = ({ });

  foreach(ob in recicipients) {
    if (!ob) {
      disappeared++;
      recicipients -= ({ ob });
    } else {
      if (!interactive(ob))
        netdead += ({ ob });
      /* Tell them even if they are net dead - it'll end up
       * in their tell history..? */
      message = replace(message, "%^", " ");
      ob->event_person_tell(TP, capitalize(TP->query_name()) +
                            " cuts through your ear muffs and tells "
                            + query_multiple_short(recicipients -
                                                   ({ ob }) + ({ "you" }), 0,
                                                   1, 0, 0) + ": ", message);
    }
  }

  if (sizeof(netdead))
    write("Warning: " + query_multiple_short(netdead, 0, 1, 0, 0) +
          (sizeof(netdead) > 1 ? " is" : " are") + " net dead.\n");


  /* There's no way to get their names now, so we just tell the cre
   * how many recicipients disappeared. */
  if (disappeared)
    write("It seems " + disappeared + " person" +
          (disappeared > 1 ? "s have" : " has")
          + " disappeared.\n");

  /* Ooops - no one left to talk to! */
  if (!sizeof(recicipients)) {
    write("No one wanted to stay around long enough "
          "to hear what you had to say.\n");
    return;
  }

  my_mess("You tell " +
          TP->convert_message(query_multiple_short(recicipients, 0, 1, 0, 0))
          + ": ", message);

  TP->add_tell_history("You cut through earmuffs and tell " +
                       TP->convert_message(capitalize(
                       query_multiple_short(recicipients, 0, 1, 0, 0))) +
                       ": ", message);
  return;
}
