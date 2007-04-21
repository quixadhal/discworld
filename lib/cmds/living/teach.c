/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: teach.c,v 1.48 2003/02/25 17:20:10 trilogy Exp $
 */
#include <skills.h>
#include <tune.h>
#include <command.h>
#include <cmds/teach.h>
#include <playtesters.h>
#include <player.h>

inherit "/cmds/base";

//#define DEBUG 1
#define DEBUGGER "ceres"

/** If this is defined it enforces a maximum level to teach others. */
#undef ENFORCE_MAX_TEACH_LEVEL

int command_teach( object *obs, string comm, object teacher );
int spell_teach(object *obs, string spell, object teacher);
int teach_skill(object *obs, string str, object teacher);
int cmd_int(string str, object *obs, object teacher);
int check_ignoring(object ignorer, object ignoree);

int query_auto_teaching(object teacher, object learner) {
   return teacher->query_property(TEACH_COMMAND_AUTO_PROPERTY);
} /* query_auto_teaching() */

int cmd(string str, object *obs, object player) {
  int ret;
  object ob;
  object* ear;

  if (this_player()->query_property(PASSED_OUT_PROP)) {
     add_failed_mess("You cannot teach while passed out.\n");
     return 0;
  }

  if (this_player()->query_fighting()) {
    add_failed_mess("You cannot teach or learn anything while you are "
                    "fighting!\n");
    return 0;
  }

  if (this_player()->check_earmuffs("teach")) {
    add_failed_mess("You have teaching earmuffed.\n");
    return 0;
  }

  if (player) {
    ear = filter(obs, (: userp($1) && !interactive($1) :));
    if (sizeof(ear)) {
      obs -= ear;
      add_failed_mess("$C$$I does not have enough responsiveness to teach "
                      "you.\n", ear);
      if (!obs) {
        return 0;
      }
    }

    ear = filter(obs, (: $1->check_earmuffs("teach") :));
    if (sizeof(ear)) {
      obs -= ear;
      if (sizeof(ear) == 1) {
        add_failed_mess("$I has " + ear[0]->query_possessive() + " earmuffs "
                        "on.\n", ear);
      } else {
        add_failed_mess("$I have their earmuffs on.\n", ear);
      }
      if (!obs) {
        return 0;
      }
    }

    ear = filter(obs, (: check_ignoring($1, this_player()) :));
    if (sizeof(ear)) {
      obs -= ear;
      add_failed_mess("$I is ignoring you, or you are ignoring them.\n",
                      ear);
      if (!obs) {
        return 0;
      }
    }

    ear = filter(obs, (: $1->query_property(PASSED_OUT_PROP) :));
    if (sizeof(ear)) {
      obs -= ear;
      add_failed_mess("$I is passed out and looks rather worse for wear.\n",
                      ear);
      if (!obs) {
        return 0;
      }
    }

    ear = filter(obs, (: $1->query_fighting() :));
    if (sizeof(ear)) {
      obs -= ear;
      add_failed_mess("$I is currently beating stuff up, please wait "
                      "for them to finish.\n", ear);
      if (!obs) {
        return 0;
      }
    }

    obs = filter(obs, (: !$1->query_creator() :) );
    if (!sizeof(obs)) {
      add_failed_mess("None of those people are allowed to teach you.\n");
      return 0;
    }

    obs = filter(obs, (: query_auto_teaching($1, this_player()) :));
    if (!sizeof(obs)) {
      add_failed_mess("None of those people have auto teaching turned "
                      "on.\n");
      return 0;
    }
    obs = filter(obs, (: $1->query_time_left() > 0 :));
    if (!sizeof(obs)) {
      add_failed_mess("All the people you are trying to learn from appear "
                      "to be busy.\n");
      return 0;
    }

    foreach (ob in obs) {
      ret |= cmd_int(str, ({ this_player() }), ob);
    }
    return ret;
  }

  return cmd_int(str, obs, this_player());
} /* cmd() */


int cmd_int(string str, object *obs, object teacher) {
  if (teacher->query_property("dead")) {
    add_failed_mess("You wave your arms around, and your lips move but "
                       "they can't hear what you are saying.\n");
    return 0;
  }
   
  obs = filter(obs,  (: !$1->query_property("dead") :));
  if (!sizeof(obs)) {
    add_failed_mess("You must teach someone, preferably living.\n");
    return 0;
  }

  if (sizeof(obs) > 1) {
    add_failed_mess("You can only teach one person at a time.\n");
    return 0;
  }
  
  if (!command_teach(obs, str, teacher) &&
      !spell_teach(obs, str, teacher) &&
      !teach_skill(obs, str, teacher))
  {
    return 0;
  }

  return 1;
} /* cmd() */


int check_ignoring(object ignorer, object ignoree) {
  return (ignorer->query_property("ignoring") &&
          member_array(ignoree->query_name(),
                       ignorer->query_property("ignoring")) != -1);
} /* check_ignoring() */


int command_teach(object *obs, string comm, object teacher) {
  string cmd_ob;
  object *succ;
  object *no_go;
  object *me_low;
  mixed *him_low;
  object *know_already;
  object *ear_muffed;
  object *ignored;
  object *ignoring;
  object ob;
  string *tmp_stuff;
  int i;
  class command cmd = new(class command, verb : comm);
  mixed tmp;

  if (member_array(comm, teacher->query_known_commands()) == -1 ||
      !CMD_D->HandleStars(cmd))
  {
    if (CMD_D->HandleStars(cmd)) {
      if (teacher == this_player()) {
        add_failed_mess("You do not know the command " + comm + ".\n");
      } else {
        add_failed_mess(teacher->the_short(1) +
                        " does not know the command " + comm + ".\n");
      }
    }
    return 0;
  }
  obs -= ({ teacher });
  if (!sizeof(obs)) {
    add_failed_mess("You cannot teach yourself a command.\n");
    return 0;
  }
  if (teacher->check_earmuffs("teach")) {
    if (teacher == this_player()) {
      add_failed_mess("You have teaching earmuffed.\n");
    } else {
      add_failed_mess(teacher->the_short(1) + " has teaching earmuffed.\n");
    }
    return 0;
  }
  cmd_ob = CMD_D->GetPaths(cmd->verb)[0] + "/" + cmd->verb;
  succ = ({ });
  ear_muffed = ({ });
  know_already = ({ });
  no_go = ({ });
  me_low = ({ });
  him_low = ({ });
  ignored = ({ });
  if (teacher->query_property("ignoring")) {
    ignoring = filter(obs, (: check_ignoring($(teacher), $1) :));
    obs -= ignoring;
  }
  foreach(ob in obs) {
    if (!living(ob)) {
      continue;
    }
    if (teacher == this_player()) {
      tmp = ob;
    } else {
      tmp = "you";
    }
    if (member_array(comm, ob->query_known_commands()) != -1) {
      know_already += ({ tmp });
    } else if (ob->check_earmuffs("teach")) {
      ear_muffed += ({ tmp });
    } else if (check_ignoring(ob, teacher)) {
      ignored += ({ tmp });
    } else {
      switch (cmd_ob->can_teach_command(teacher, ob)) {
        case 1:
          succ += ({ ob });
          break;

        case 0 :
          no_go += ({ tmp });
          break;
        case -1 :
          me_low += ({ tmp });
          break;

        case -2 :
          him_low += ({ tmp });
          break;
      }
    }
  }
  if (!succ || !sizeof(succ)) {
    if (teacher != this_player()) {
      tell_object(teacher, this_player()->the_short() + " tried to "
                  "learn " + comm + " from you automatically, "
                  "but failed.\n");
    }

    if (sizeof(no_go) > 0) {
      add_failed_mess(teacher->the_short() + " cannot teach " + comm +
                      " to $I.\n", no_go);
    }
    if (sizeof(me_low)) {
      if (teacher == this_player()) {
        add_failed_mess(teacher->the_short() +
                        " are too low a level to teach " + comm +
                        " to $I.\n", me_low);
      } else {
        add_failed_mess(teacher->the_short() +
                        " is too low a level to teach " + comm +
                        " to $I.\n", me_low);
      }
    }
    if (sizeof(ear_muffed) > 0) {
      add_failed_mess("$I has teaching events earmuffed, you are unable "
                      "to teach them.\n", ear_muffed);
    }
    if (sizeof(him_low) > 0) {
      add_failed_mess("$I " +
                      ((sizeof(him_low) == 1  &&
                        him_low[0] != this_player()) ? "is" : "are") +
                      " too low a level to learn " + comm + ".\n", him_low);
    }
    if (sizeof(know_already)) {
      add_failed_mess("$I already know" +
                      (sizeof(know_already) > 1  ||
                       teacher != this_player()? "" : "s") +
                      " the command " + comm + ".\n", know_already);
    }
    if (sizeof(ignoring)) {
      if (this_player() == teacher) {
        add_failed_mess("You are ignoring $I.\n", ignoring);
      } else {
        add_failed_mess(teacher->the_short(1) + " is ignoring $I.\n",
                        ignoring);
      }
    }
    if (sizeof(ignored)) {
      add_failed_mess("You are being ignored by $I.\n", ignored);
    }
    return 0;
  }
  write(teacher->the_short() + " offer" +
        (this_player() == teacher ? "" : "s") + " to teach " + comm +
        " to " + query_multiple_short(succ) + ".\n");
  if (teacher == this_player()) {
    if (sizeof(no_go)) {
      write("You cannot teach " + comm + " to " +
            query_multiple_short(no_go, "the", 0, 1 ) + ".\n");
    }
    if (sizeof(ear_muffed) > 0) {
      write(capitalize(query_multiple_short(ear_muffed, "the", 0, 1)) +
            " has teaching events earmuffed, you are unable "
            "to teach them.\n");
    }
    if (sizeof(me_low) > 0) {
      write("You are too low a level to teach " + comm + " to " +
            query_multiple_short(me_low, "the", 0, 1) + ".\n");
    }
    if (sizeof(him_low) > 0) {
      write(capitalize(query_multiple_short(him_low, "the", 0, 1)) +
            ((sizeof(him_low) == 1) ? " is" : " are") +" too low a "
            "level to learn " + comm + ".\n");
    }
  } else {
    tell_object(teacher,
                "You automatically offer to teach " + comm + " to " +
                query_multiple_short(succ) + ".\n");
  }
  say(capitalize((string)teacher->short()) + " teaches something to " +
      query_multiple_short(succ) + ".\n", succ + ({ teacher }));

  for (i = 0; i < sizeof(succ); i++) {
    tell_object(succ[i], capitalize(teacher->the_short()) +
                " offers to teach " +
                query_multiple_short(delete(succ, i, 1) +
                ({ "you" }) ) + " the command \"" + comm + "\".\n"
                "Type \"learn " + comm + " from " +
                teacher->query_name() + "\" to learn the command.\n");
    tmp_stuff = succ[i]->query_respond_command(TEACH_COMMAND_TYPE, teacher);
    if (!tmp_stuff) {
      tmp_stuff = ({ comm });
    } else if (member_array(comm, tmp_stuff) == -1) {
      tmp_stuff += ({ comm });
    }
    succ[i]->add_respond_command(TEACH_COMMAND_TYPE, teacher, tmp_stuff);
  }
  add_succeeded_mess("");
  return 1;
} /* command_teach() */


int spell_teach(object *obs, string spell, object teacher) {
  object *succ, *no_go, *me_low, *him_low, ob;
  object *ignoring;
  object *ignored;
  object *ear_muffed;
  string ret;
  string is_are;
  mapping spells;
  mixed tmp;
   
  spells = teacher->query_spells();
 
  if (!spells[spell]) {
    return 0;
  }

  if (teacher == this_player()) {
    is_are = "are";
  } else {
    is_are = "is";
  }
 
  if (teacher->check_earmuffs("teach")) {
    add_failed_mess("You have teaching earmuffed.\n");
    return 0;
  }
  ear_muffed = succ = no_go = me_low = him_low = ({ });
 
  ignored = ({ });
  if (teacher->query_property("ignoring")) {
    ignoring = filter(obs, (: check_ignoring($(teacher), $1) :));
    obs -= ignoring;
  }
  foreach(ob in obs) {
    if (living(ob)) {
      if (ob->check_earmuffs("teach")) {
        ear_muffed += ({ ob });
        continue;
      } else if (check_ignoring(ob, teacher)) {
        ignored += ({ ob });
      } else {
        if (teacher == this_player()) {
          tmp = ob;
        } else {
          tmp = "you";
        }
        switch (spells[spell][0]->teach(ob, spell)) {
          case 1:
            succ += ({ ob });
            break;

          case 0:
            no_go += ({ ob });
            break;

          case -1:
            me_low += ({ ob });
            break;

          case -2:
            him_low += ({ ob });
            break;
        }
      }
    }
  }
  if (sizeof(succ) == 0) {
    if (teacher != this_player()) {
      tell_object(teacher, this_player()->the_short() + " tried to "
                  "learn " + spell +
                  " from you automatically, but failed.\n");
    }
    ret = "";
    if (sizeof(no_go) > 0) {
      add_failed_mess(teacher->the_short() +
                      " cannot teach " + spell + " to $I.\n", no_go);
    }
    if (sizeof(me_low) > 0) {
      add_failed_mess(teacher->the_short() + 
                      " " + is_are + 
                      " too low a level to teach $I " + spell + ".\n",
                      me_low);
    }
    if (sizeof(him_low) > 0) {
      add_failed_mess("$I " + is_are + " too low a level to learn " +
                       spell + ".\n", him_low);
    }
    if (sizeof(ignoring)) {
       add_failed_mess("You are ignoring $I.\n", ignoring);
    }
    if (sizeof(ignored) > 0) {
       add_failed_mess("You are being ignored by $I.\n", ignored);
    }
    if (sizeof(ignored) > 0) {
       add_failed_mess("$I has teaching earmuffed.\n", ignored);
    }
    return 0;
   }
   if (teacher == this_player()) {
     write(teacher->the_short() +
           " successfuly teach " + query_multiple_short(succ) + " " +
           spell + ".\n");
     if (sizeof(no_go) > 0) {
       write("You cannot teach " + spell + " to " +
             query_multiple_short(no_go, "the", 0, 1) + ".\n");
     }
     if (sizeof(me_low) > 0) {
       write("You are too low a level to teach " +
             query_multiple_short(me_low, "the", 0, 1) + " " + spell +
             ".\n");
     }
     if (sizeof(him_low) > 0) {
       write(capitalize(query_multiple_short(him_low, 0, 1)) +
             (sizeof(him_low) == 1 ? " is " : " are ") +
             "too low a level to learn " + spell + ".\n");
     }
     if (sizeof(ignoring) > 0) {
       add_failed_mess("You are ignoring " +
                       query_multiple_short(ignoring, "the", 0, 1) +
                       ".\n");
     }
     if (sizeof(ignored) > 0) {
       add_failed_mess("You are being ignored by " +
                       query_multiple_short(ignored, "the", 0, 1) + ".\n");
     }
   } else {
     write(teacher->the_short() +
           " successfuly teaches " + query_multiple_short(succ) + " " +
           spell + ".\n");
   }
   return 1;
} /* spell_teach() */
     
#define CANNOT 0
#define TOO_LOW 1
#define ONLY_LEAF 2
#define TOO_HIGH 3
#define CANNOT_TEACH 4

int teach_skill(object *obs, string str, object teacher) {
  int num;
  int lvl;
  int my_lvl;
  int lvl_up;
  int j;
  int cost;
  int total;
  int total2;
  int sk;
  int depth;
  object *ok;
  object *too_little;
  mixed  *too_low;
  object *cannot_teach;
  object *only_leaf;
  object *ignored;
  object *ignoring;
  object *ear_muffed;
  object *too_high;
  object ob;
  string skill;
  string skill_start;
  mixed *bits;
  class teaching_skill frog;
  float k;
  mixed tmp;

  num = 1;
  if (sscanf(str, "%d levels of %s", num, skill) != 2) {
    if (sscanf(str, "%d level of %s", num, skill) != 2) {
      if (sscanf(str, "%d %s", num, skill) != 2) {
        num = 1;
        skill = str;
      }
    }
  }
  if (num < 1) {
    add_failed_mess("You cannot teach a negative or zero number of "
                    "levels.\n");
    return 0;
  }
  if (num > 50) {
    add_failed_mess("You cannot teach more than 50 levels at a time.\n");
    return 0;
  }
  if (teacher->check_earmuffs("teach")) {
    add_failed_mess("You have teaching earmuffed.\n");
    return 0;
  }
  /* Make sure its a valid skill */
  skill_start = skill;
  bits = explode(implode(explode(skill, " "), "."), ".") - ({ "" });
  if (!bits || !(skill = (string)SKILL_OB->query_skill(bits))) {
    if (member_array(skill_start, teacher->query_known_commands()) != -1) {
      return 0;
    }
    add_failed_mess("The skill " + implode(bits, ".") + " is invalid.\n");
    return 0;
  }
  
  bits = explode(skill, ".");
  /*
   * We don't do the teaching here.  Figure out how much xp it will cost
   * to go up the levels, and inform the person we are teaching...
   */
  my_lvl = teacher->query_skill_bonus(skill, 1);
  ear_muffed = ({ });
  too_high = ({ });
  only_leaf = ({ });
  too_low = ({ });
  ok = ({ });
  too_little = ({ });
  cannot_teach = ({ });
  ignored = ({ });
  if (teacher->query_property("ignoring")) {
    ignoring = filter(obs, (: check_ignoring($(teacher), $1) :));
    obs -= ignoring;
  }

  foreach (ob in obs) {
    /*
     * First make sure the skill can be taught.  There are only restrictions
     * (currently) on teaching of languages.  You cannot teach yourself
     * levels in a language.  You have to go and research them.
     */
    if (teacher == this_player()) {
      tmp = ob;
    } else {
      tmp = "you";
    }
    if (!interactive(ob)) {
      cannot_teach += ({ tmp });
      continue;
    }
    if (!SKILL_OB->query_allowed_to_teach(skill)) {
      cannot_teach += ({ tmp });
      continue;
    }
    if (ob->check_earmuffs("teach")) {
      ear_muffed += ({ tmp });
      continue;
    }
    if (check_ignoring(ob, teacher)) {
      ignored += ({ tmp });
      continue;
    }
    if (SKILL_OB->query_only_leaf(skill) &&
        sizeof((mixed *)SKILL_OB->query_immediate_children(skill)))
    {
      only_leaf += ({ tmp });
      continue;
    }
    lvl = (int)ob->query_skill(skill);

    if (ob != teacher) {
      if ((int)ob->calc_bonus(lvl + num, skill, 1) > my_lvl) {
        add_failed_mess(teacher->the_short() +
                        " cannot teach $I, since their bonus is " +
                        (teacher == this_player() ? "higher" : "lower") +
                        " than yours.\n", ({ tmp }));
        continue;
      }
#ifdef ENFORCE_MAX_TEACH_LEVEL
      /*
       * Check to see if your level, or the level being taught too is
       * too high.
       */
      if (lvl + num > SKILL_MAX_TEACH) {
        too_high += ({ tmp });
        continue;
      }
#endif
    }

    if (sizeof(bits) > 1) {
      lvl_up = ob->query_skill(implode(bits[0 .. sizeof(bits) - 2], "."));
    } else {
      lvl_up = lvl;
    }

    depth = SKILL_OB->query_skill_depth(bits);
    if (!SKILL_OB->query_only_leaf(skill) && depth * 5 > lvl_up) {
      too_low += ({ tmp });
      continue;
    }

    total = 0;
    total2 = 0;

    foreach (sk in SKILL_OB->query_all_children(skill)) {
      if (SKILL_OB->query_immediate_children(sk) != ({ })) {
        continue;
      }
      my_lvl = teacher->query_skill_bonus(sk, 1);
      lvl = (int)ob->query_skill(sk);
      cost = DEFAULT_COST;
      cost *= STD_COST / 5;

      if (!my_lvl) {
        my_lvl = 1;
      }

      for (j = 0; j < num; j++) {
        k = 0.5 * (int)ob->calc_bonus(lvl + j, sk, 1 ) / my_lvl + 1.0;
        total2 += 500 + to_int(cost * (lvl+j) * exp((lvl + j) / 500.0) * k);
        if (total > total2) {
          num = j;
          break;
        } else {
          total = total2;
        }
      }

#ifdef DEBUG
      tell_object(find_player(DEBUGGER),
                  sprintf("%s:%s j:%d, lvl: %d, mylvl: %d, newbonus: %d, "
                          "total: %d\n",
                          this_player()->query_name(),
                          ob->query_name(),
                          j, lvl,
                          my_lvl,
                          (int)ob->calc_bonus(lvl + j, sk, 1),
                          lvl,
                          total2));
#endif

      if (!total) {
        total = cost;
      }
    }

    if (total > (int)ob->query_xp()) {
      if (teacher == this_player()) {
        add_failed_mess("It would have cost " + total + " xp to teach " +
                        num + " level" + (num > 1 ? "s" : "") + " of " +
                        skill + " to $I.\n", ({ ob }));
      } else {
        add_failed_mess("It would have cost " + total + " xp to teach " +
                        num + " level" + (num > 1 ? "s" : "") + " of " +
                        skill + " to you from " + teacher->the_short() +
                        ".\n");
      }
      too_little += ({ tmp });
      continue;
    }

    if (ob != teacher) {
      if (teacher == this_player()) {
        tell_object(ob, teacher->the_short() + " offers to teach "
                    "you " + num + " level" + (num > 1 ? "s" : "") +
                    " of " + skill + " for " + total + " xp.\nUse "
                    "\"learn\" to learn the skill.\n");
        write("You offer to teach $M$$the_short:" + file_name(ob) +
              "$$M$ " + num + " level" + (num > 1 ? "s" : "") + " of " +
              skill + " for " + total + " xp.\n");
      } else {
        tell_object(ob, teacher->the_short() + " offers to teach "
                    "you " + num + " level" + (num > 1 ? "s" : "") +
                    " of " + skill + " for "+total+" xp.\nUse \"learn\" "
                    "to learn the skill.\n");
        tell_object(teacher,
                    "You offer to teach $M$$the_short:" + file_name(ob) +
                    "$$M$ " + num + " level" + (num > 1 ? "s" : "") +
                    " of " + skill + " for " + total + " xp.\n");
      }
    } else {
      write("You can teach yourself " + num + " level" +
            (num > 1 ? "s" : "") + " of " + skill + " for " + total +
            " xp.\nUse \"learn\" to learn the skill.\n");
      write("You offer to teach $M$$the_short:" + file_name(ob) +
            "$$M$ " + num + " level" + (num > 1 ? "s" : "") + " of " +
            skill + " for " + total + " xp.\n");
    }
    ok += ({ tmp });
    frog = new(class teaching_skill);
    frog->num = num;
    frog->lvl = ob->query_skill(skill);
    frog->skill = skill;
    frog->xp = total;
    frog->room = environment(this_player());
    ob->add_respond_command(TEACH_SKILL_TYPE, teacher, frog);
  }
  if (sizeof(ok) == 0) {
    if (teacher != this_player()) {
      tell_object(teacher,
                  this_player()->the_short() + " tried to learn " + skill +
                  " from you automatically but failed.\n");
    }

    if (sizeof(only_leaf) > 0) {
      add_failed_mess("You cannot teach the skill " + skill +
                      ", as it is only possible to teach leaf skills in "
                      "this skill tree.\n");
    }

    if (sizeof(ear_muffed) > 0) {
      add_failed_mess("You cannot teach any levels of " + skill +
                      " to $I, they have teaching earmuffed and cannot "
                      "hear you.\n", ear_muffed);
    }

    if (sizeof(cannot_teach) > 0) {
      add_failed_mess("You cannot teach any levels of " + skill +
                      "; you need to look for alternative methods of "
                      "advancement.\n");
    }

    if (sizeof(too_low) > 0) {
      add_failed_mess(query_multiple_short(too_low, "the", 0, 1) +
                      ((sizeof(too_low) > 1    ||
                        too_low[0] == teacher  ||
                        teacher != this_player()) ? " are " : " is ") +
                      "not at a high enough level in the outer skills to "
                      "learn " + num + " levels of " + skill + ".   See "
                      "'help skills' for more details.\n");
    }

    if (sizeof(too_high) > 0) {
      add_failed_mess(query_multiple_short(too_low, "the", 0, 1) +
                      ((sizeof(too_high) > 1     ||
                        teacher != this_player() ||
                        too_low[0] == teacher) ? " are " : " is ") +
                      "too high a level to learn " + num + " levels of " +
                      skill + ", they must be less than " +
                      SKILL_MAX_TEACH + " to learn from someone else.\n");
    }

    if (sizeof(ignoring) > 0) {
      add_failed_mess("You are ignoring $I.\n", ignoring);
    }

    if (sizeof(ignored) > 0) {
      add_failed_mess("You are being ignored by $I.\n", ignored);
    }
  }

  return sizeof(ok);
} /* teach_skill() */

mixed *query_patterns() {
   return ({
      "<string:'skill|n levels of skill|command'> to <indirect:living>",
                   (: cmd($4[0], $1, 0) :),
      "me <string:'skill|n levels of skill|command'> from <indirect:living>",
                   (: cmd($4[0], $1, this_player()) :)});
} /* query_patterns() */
