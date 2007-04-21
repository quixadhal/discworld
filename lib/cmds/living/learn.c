/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: learn.c,v 1.19 2003/01/31 06:27:31 pinkfish Exp $
 * 
 */
#include <skills.h>
#include <tune.h>
#include <cmds/teach.h>
#include <command.h>

inherit "/cmds/base";

#define TP this_player()
mixed cmd(string str, object *obs);

mixed cmd(string str, object *obs) {
  object *diff_lvl;
  object *no_xp;
  object *not_offer;
  object *ok;
  object *moved;
  object ob;
  int time;
  string skill;
  string cmd;
  string cmd_ob;
  string *bits;
  string levelstr;
  class teaching_skill womble;

  if (TP->query_property("dead")) {
    notify_fail("Your dead.  Your mind slips off the task and goes for "
                "a wander down memory lane.\n");
    return 0;
  }

  skill = str;

  /* Figure out the skill thingy. */
  bits = explode(implode(explode(skill, " "), "."), ".");
  skill = SKILL_OB->query_skill(bits);
  if (!skill) {
    /* Ok, not a skill...  Check to see if it is a command. */
    ok = ({ });
    cmd = str;
    if (sizeof(CMD_D->GetPaths(cmd)) &&
        CMD_D->GetPaths(cmd)[0]) {
       cmd_ob = CMD_D->GetPaths(cmd)[0] + "/" + cmd;
       foreach (ob in obs) {
          bits = TP->query_respond_command(TEACH_COMMAND_TYPE, ob);
          if (pointerp(bits) && member_array(cmd, bits) != -1) {
             cmd_ob->teach_command(ob, this_player());
             ok += ({ ob });
             bits -= ({ cmd });
             if (sizeof(bits)) {
                TP->add_respond_command(TEACH_COMMAND_TYPE, ob, bits);
             } else {
                TP->remove_respond_command(TEACH_COMMAND_TYPE, ob);
             }
             event(environment(this_player()), "teach", "command", ob, this_player());
             break;
          }
       }
       if (sizeof(ok)) {
          add_succeeded_mess("$N learn$s " + cmd + " from $I.\n", ok);
          return 1;
       }
       add_failed_mess("The skill \"" + cmd + "\" is invalid, or $I "
                       "is not trying to teach you the \"" + cmd + "\" "
                       "command.\n");
    } else {
       add_failed_mess("The skill '" + cmd + "' is invalid.\n");
    }
    return 0;
  }

  moved = ok = not_offer = no_xp = diff_lvl = ({ });
  foreach (ob in obs) {
    womble = TP->query_respond_command(TEACH_SKILL_TYPE, ob);
    if (!womble) {
      not_offer += ({ ob });
      continue;
    }
    if (skill != womble->skill) {
      not_offer += ({ ob });
      continue;
    }
    if ((int)TP->query_xp() < womble->xp ||
        !womble->xp) {
      no_xp += ({ ob });
      continue;
    }
    if (TP->query_skill(womble->skill) != womble->lvl) {
      diff_lvl += ({ ob });
      TP->remove_respond_command(TEACH_SKILL_TYPE, womble->teacher);
      continue;
    }
    if (sizeof(ok)) {
      continue;
    }
    if(womble->num == 1)
      levelstr = "level";
    else
      levelstr = "levels";

    if (environment(this_player()) != womble->room) {
       moved += ({ ob });
       TP->remove_respond_command(TEACH_SKILL_TYPE, womble->teacher);
       continue;
    }
    
    if (ob != TP) {
      write(ob->short()+" starts to teach you " + womble->num + " " +
            levelstr + " of " + womble->skill + " for " + womble->xp +
            " xp.\n");
      tell_object(ob, "You start to teach " + TP->short() +
                  " " + womble->num + " " + levelstr +
                  " of " + womble->skill +
                  " for " + womble->xp + " xp.\n");
      say(ob->short()+" starts to teach "+TP->short()+" some skills.\n",
          ({ TP, ob }));
      ok += ({ ob });
    } else {
      write("You start to teach yourself " + womble->num + " " +
            levelstr + " in "+
            womble->skill + " for " + womble->xp + " xp.\n");
      say(TP->short()+" starts to teach " + TP->query_objective() +
          "self some skills.\n", ({ TP }));
    }
    /*
     * Ok...  We now change the time on both people and wait...
     * After they have both finished everything is hunky womble.  If one
     * types stop, we dont teach them anything...
     */
    time = womble->xp / TIME_DIV;
    if(time > 1000) {
      time = 1000;
    }
    
    TP->adjust_time_left(-time);
    womble->teacher = ob;
    womble->taught = TP;
    TP->set_interupt_command("stop_teaching_skills", file_name(this_object()),
                             womble);

    if (TP != ob) {
      ob->adjust_time_left(-time);
      ob->set_interupt_command("stop_teaching_skills", TP, womble);
    }
  }
  if (sizeof(not_offer)) {
    if(sizeof(not_offer) == 1) {
      if(not_offer[0] != this_player())
        write(capitalize(query_multiple_short(not_offer)) +
              " is not offering to teach you "+ skill+".\n");
      else
        write(capitalize(query_multiple_short(not_offer)) +
              " are not offering to teach yourself "+ skill+".\n");
    } else
        write(capitalize(query_multiple_short(not_offer)) +
              " are not offering to teach you "+ skill+".\n");
  }
  if (sizeof(moved)) {
    write("You appear to have moved away from the excellent teaching "
          "environment selected by "+
          query_multiple_short(moved)+".\n");
  }
  if (sizeof(no_xp)) {
    write("You do not have enough xp to learn "+skill+" from "+
          query_multiple_short(no_xp)+".\n");
  }
  if (sizeof(diff_lvl)) {
    write("You were a different level in "+skill+" when "+
          query_multiple_short(diff_lvl)+" offered to teach you.\n");
  }
  if (sizeof(ok)) {
    say(TP->short()+" learns some skills from "+query_multiple_short(ok)+
        ".\n", ok);
  }
  return 1;
} /* cmd() */

/**
 * The method to call when we stop teaching skills.  THis will stop the
 * stuff being taught if the stop is successful, and only teach partial
 * amounts if we are not finished yet.
 * @param left the amount of time left
 * @param bing the data associated with the command
 */
void stop_teaching_skills(int time_left, class teaching_skill womble,
                          object us, object interupter, string cmd) {
   object ob;
   mixed *stuff;
   string levelstr;
   
   if (time_left > 0) {
      /* Someone did a stop!  Naughty frogs! */
      if (womble->teacher == us) {
         /* Only print the messages once. */
         if (womble->teacher == womble->taught) {
            say(womble->taught->short() + " stops teaching " +
                womble->taught->query_objective() + "self some "
                "skills.\n");
         } else if (us == interupter) {
            ob = womble->teacher;
            tell_object(ob, womble->taught->short() + " interupts your "
                            "training.\n");
         } else {
            tell_object(us, interupter->short() + " interupts your "
                            "training.\n");
         }
         say(interupter->short() + " stops teaching some skills to " +
             womble->taught->short() + ".\n",
             ({ us, interupter }));
      }

      /* Interupt the other person. */
      if (us == womble->teacher) {
         ob = womble->taught;
      } else {
         ob = womble->teacher;
      }
      if (ob) {
         stuff = ob->query_interupt_command();
         if (stuff) {
            if (stuff[0] == "stop_teaching_skills") {
               ob->interupt_command(us);
            }
         }
      }
      us->adjust_time_left(-((int)womble->taught->query_time_left()));
      us->set_interupt_command(0);

      return ;
   }
  
   if (interupter != us) {
      /* First make sure we dont get the level twice... */
      return ;
   }

   if (environment(womble->taught) != environment(womble->teacher) ||
       environment(womble->taught) != womble->room) {
      tell_object(us, "You appear to have left your excellent "
                      "teaching environment, so the skill teaching failed.\n");
      tell_object(womble->teacher, "You appear to have left your excellent "
                      "teaching environment, so the skill teaching failed.\n");
      return;
   }

   // additional test added by ceres coz people are getting put into negative
   // xp by getting taught twice somehow.
   if(womble->taught->query_xp() < womble->xp) {
      write("Something has gone wrong. :(\n");
      return;
   }
   /* Ok...  We did it!  Finished! */
   if (womble->taught != womble->teacher) {
      womble->teacher->adjust_xp(womble->xp / 10);
   }
   womble->taught->adjust_xp(-womble->xp);

   if(womble->num == 1)
     levelstr = "level";
   else
     levelstr = "levels";
   
   if(womble->taught->add_skill_level(womble->skill, womble->num, womble->xp))
     {
       
       if (us != womble->teacher) {
         tell_object(us, "You finish learning " + womble->num +
                     " " + levelstr + " of "
                     + womble->skill + " from " + womble->teacher->short() +
                     ".\n");
         tell_object(womble->teacher, womble->taught->short() + " finishes " +
                     "learning " + womble->num + " " + levelstr + " of "
                     + womble->skill + " from you.\n");
         say(us->short() + " finishes learning some skills "+
             "from "+ womble->teacher->short()+".\n",
             ({ us, womble->teacher }));
       } else {
         tell_object(us, "You finish teaching yourself " + womble->num +
                     " " + levelstr + " of " +
                     womble->skill + ".\n");
         say(us->short() + " finishes learning some skills "
             "from " + us->query_objective() + "self.\n",
             ({ us, womble->teacher }));
       }
     } else {
       tell_object(womble->taught, "Something went wrong learning " +
                   womble->skill + " from " + womble->teacher->query_short() +
                   ".\n");
     }
   womble->taught->remove_respond_command(TEACH_SKILL_TYPE, womble->teacher);
} /* stop_teaching_skills() */

mixed *query_patterns() {
   return ({ "<string'skill/command'> from <indirect:living>",
             (: cmd($4[0], $1) :) });
} /* query_pattern() */
