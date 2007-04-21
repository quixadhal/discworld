#include <money.h>
#include <skills.h>
#include <tune.h>
#include <config.h>

private int cost_div;
private int cost_mult;
private string our_guild;
private string start_pos;
private mapping spells;
private mapping commands;
private object teaching_person;

void check_spells_commands(string *sk, int lvl);
int do_cost_primaries();
int do_cost_all(int brief);
int do_cost_to(string skill, int level);
int do_cost_by(string skill, int level);
int do_cost(string skill);

int do_advance(string skill);
int do_advance_to(string skill, int num);
int do_advance_by(string skill, int num);

void create()
{
   cost_div = COST_DIV;
   cost_mult = 100;
   spells = ([ ]);
   commands = ([ ]);
   this_object()->add_help_file("guild");
} /* create() */


void init()
{
   this_player()->add_command("advance", this_object(),
                              "<string>", (: do_advance($4[0]) :));
   this_player()->add_command("advance", this_object(),
                              "<string> to <number>",
                              (: do_advance_to($4[0], $4[1]) :));
   this_player()->add_command("advance", this_object(),
                              "<string> by <number>",
                              (: do_advance_by($4[0], $4[1]) :));
   this_player()->add_command("join", this_object(), "");
   this_player()->add_command("info", this_object(), "");
   this_player()->add_command("cost", this_object(),
                              "primaries", (: do_cost_primaries() :));
   this_player()->add_command("cost", this_object(),
                              "all", (: do_cost_all(0) :));
   this_player()->add_command("cost", this_object(),
                              "all brief", (: do_cost_all(1) :));
   this_player()->add_command("cost", this_object(),
                              "<string>", (: do_cost($4[0]) :));
   this_player()->add_command("cost", this_object(),
                              "<string> to <number>",
                              (: do_cost_to($4[0], $4[1]) :));
   this_player()->add_command("cost", this_object(), "<string> by <number>",
                              (: do_cost_by($4[0], $4[1]) :));
   if ((string) this_player()->query_guild_ob() == our_guild) {
      if (stringp(start_pos) && !clonep(this_object())) {
         this_player()->add_start(file_name(this_object()), start_pos);
      }
   }
} /* init() */


varargs void set_guild(string word1, string word2)
{
   our_guild = word1;
   start_pos = word2;
} /* set_guild() */


void set_cost_div(int number)
{
   cost_div = number;
/*
 * Points of interest:
 *    cost_div -> zero      gives cost_mult = 10
 *    cost_div = COST_DIV   gives cost_mult = 100
 *    cost_div -> infinity  gives cost_mult = 1000
 */
   if (!cost_div)               /* stands in for infinity */
      cost_mult = 1000;
   else
      cost_mult = 10 + (990 * cost_div) / (10 * COST_DIV + cost_div);
} /* set_cost_div() */


int query_skill_cost(string skill, int offset)
{
   int i;
   int total;
   string *next;

   next = (string *) SKILL_OB->query_immediate_children(skill);
   if (!sizeof(next)) {
      if (!this_player()->query_guild_ob())
         total = DEFAULT_COST;
      else
         total = (int) our_guild->query_skill_cost(skill);
      total *= (STD_COST * cost_mult) / 500;
      total *= ((int) this_player()->query_skill(skill) + offset) /
         LEVEL_DIV + 1;
      // this is the new, increased skill cost.
      total = to_int(total * exp((this_player()->query_skill(skill) +
                                  offset) / 150.0));

      return total;
   }
   for (i = 0; i < sizeof(next); i++)
      total += query_skill_cost(next[i], offset);
   return total;

} /* query_skill_cost() */


int do_advance_internal(string skill, int to, int by)
{
   string *bits;
   string skill_name;
   string outer_name;
   string place;
   int lvl;
   int max_lvl;
   int tmp;
   int total_xp;
   int total_cost;
   int tmp_cost;
   int p_val;
   int i, j;
   int round_value;

   if ((string) this_player()->query_guild_ob() != our_guild) {
      add_failed_mess("You cannot advance here!  Go to your own guild.\n");
      return 0;
   }
   place = this_object()->query_property("place");
   if (!place)
      place = "default";
   bits = explode(implode(explode(skill, " "), "."), ".");
   if (!(skill_name = (string) SKILL_OB->query_skill(bits))) {
      add_failed_mess("The skill " + skill + " does not exist.\n");
      return 0;
   }

   bits = SKILL_OB->query_inactive_skills();
   for(j=0; j<sizeof(bits); j++)
     if(strsrch(skill, bits[j]) != -1)
       return add_failed_mess("The skill " + skill + " is inactive at "
                              "present and cannot be taught.\n");

   lvl = (int) this_player()->query_skill(skill_name);
   if (!by && to)
      by = to - lvl;
   if (by <= 0) {
      if (by == 0) {
         add_failed_mess("You are already at level " + lvl + " at " +
                         skill_name + ".\n");
      } else {
         add_failed_mess("You cannot regress below your current "
                         "skill level!\n");
      }
      return 0;
   }
   if (SKILL_OB->query_only_leaf(skill_name)) {
      if (sizeof((mixed *)SKILL_OB->query_immediate_children(skill_name))) {
         add_failed_mess("You can only advance the outer skills "
                         "(eg: other.movement.riding.horse), "
                         "not the internal "
                         "skills (eg: other.movement.riding, or other).\n");
         return 0;
      }
   } else {
      bits = explode(skill_name, ".");
      if (sizeof(bits) > 1) {
         outer_name = implode(bits[0..<2], ".");
         if (!SKILL_OB->is_skill_ignored(outer_name) &&
             ((sizeof(bits) - 1) * 5) >
             (int) this_player()->query_skill(outer_name)) {
            add_failed_mess("You are not at a high enough level in " +
                            outer_name +
                            " to advance " + skill_name + " yet.\n");
            return 0;
         }
      }
   }

   if (!SKILL_OB->query_allowed_to_teach(skill_name)) {
      add_failed_mess("You cannot learn '" + skill_name +
                      "' from your guild.\n");
      return 0;
   }

   max_lvl = (int) our_guild->query_skill_max_level(skill_name);
   if (lvl >= max_lvl) {
      add_failed_mess("You cannot advance in " + skill_name +
                      " any further here.\n");
      return 0;
   }

   p_val = (int) this_player()->query_value_in(place);
   if (place != "default") {
      p_val += (int) this_player()->query_value_in("default");
   }

   round_value = MONEY_HAND->query_smallest_value_in(place);
   total_cost = 0;

   for (i = 0; i < by; i++) {
      tmp = query_skill_cost(skill_name, i);

      if ((int) this_player()->query_xp() < tmp + total_xp) {
         if (!i) {
            add_failed_mess("Your lack of experience prevents you improving "
                            + skill_name + ".\n");
            return 0;
         } else {
            write("Your lack of experience prevents you taking all the "
                  "advancement you requested in " + skill_name + ".  You are "
                  "only trained up to level " + (i + lvl) + ".\n");
         }
         break;
      }
      if ((lvl + i) >= max_lvl) {
         write("Your guild hasn't the resources to train you as far as " +
               "you requested in " + skill_name +
               ".  It trains you to level " + max_lvl + ".\n");
         break;
      }
      if (cost_div) {
         tmp_cost = ( total_xp + tmp ) / cost_div;
         tmp_cost -= (tmp_cost % round_value);
         if ( tmp_cost > p_val ) {
            if (!i) {
               add_failed_mess
                  ("Due to your lack of money, the guild cannot train " +
                   "you in " + skill_name + ": it would have cost you " +
                   MONEY_HAND->money_value_string( tmp_cost, place ) + ".\n" );
               return 0;
            } else {
               write("Due to your sad lack of money the guild cannot train " +
                     "you in " + skill_name + " to level " + (by + lvl) +
                     ".  It trains you to level " + (i + lvl) + " instead.\n");
            }
            break;
         }
      }
      total_xp += tmp;
      total_cost = tmp_cost;
   }

   if (!total_xp) {
      return add_failed_mess("Oops, something has gone wrong.\n");
   }

   this_player()->adjust_xp(-total_xp);
   if (total_cost) {
      this_player()->pay_money((mixed *) MONEY_HAND->
                               create_money_array(total_cost, place), place);
   }
   this_player()->add_skill_level(skill_name, i, total_xp);
   this_player()->reset_all();
   this_player()->dest_hide_shadow();
   event(this_object(), "guild_advance", bits, lvl, lvl + i);
   check_spells_commands(bits, lvl + i);
   write("You advance your skill in " + skill_name + " from " + lvl + " to " +
         (lvl + i) + " for " + total_xp + " xp" + (total_cost ? " and " +
                                                   (string) MONEY_HAND->
                                                   money_value_string
                                                   (total_cost,
                                                    place) : "") + ".\n");
   say((string) this_player()->one_short() + " $V$0=advances,advance$V$ " +
       "$V$0=" + this_player()->query_possessive() + ",their$V$ skills.\n");
   return 1;
} /* do_advance_internal() */

int do_advance(string skill)
{
   return do_advance_internal(skill, 0, 1);
} /* do_advance() */

int do_advance_by(string skill,
                  int num)
{
   return do_advance_internal(skill, 0, num);
} /* do_advance_by() */

int do_advance_to(string skill,
                  int num)
{
   return do_advance_internal(skill, num, 0);
} /* do_advance_to() */


int do_join(string str)
{
   if ((string) this_player()->query_guild_ob() == our_guild) {
      add_failed_mess("You're already in this guild.\n");
      return 0;
   }
   if (this_player()->query_guild_ob()) {
      add_failed_mess("You cannot join a guild while a member of another.\n");
      return 0;
   }
   write("You will only ever get to join one guild.  Are you sure? ");
   input_to("join2");
   return 1;
} /* do_join() */


int join2(string str)
{

   str = lower_case(str);

   if (str[0] != 'n' && str[0] != 'y') {
      write("I do not understand.  Yes or no? ");
      input_to("join2");
      return 1;
   }
   if (str[0] == 'n') {
      write("Ok, not joining the " + our_guild->query_name() + " guild.\n");
      return 1;
   }
   this_player()->set_guild_ob(our_guild);
   if (stringp(start_pos) && !clonep(environment(this_player())))
      this_player()->add_start(file_name(environment(this_player())),
                               start_pos);
   this_player()->race_guild_commands();
   write("You are now a member of the " + our_guild->query_name() +
         " guild.\n");
   say(this_player()->one_short() + " joins the " + our_guild->query_name() +
       " guild.\n");
   event(this_object(), "guild_join");
   event(users(), "inform",
         this_player()->query_cap_name() + " is now a member " + "of " +
         our_guild->query_name(), "guild");
   return 1;
} /* join2() */


int do_leave(string str)
{
   if (str != "guild") {
      add_failed_mess("Syntax : leave guild\n");
      return 0;
   }
   if ((string) this_player()->query_guild_ob() != our_guild) {
      add_failed_mess("How can you leave a guild if you aren't in it.\n");
      return 0;
   }
   if ((int) this_player()->query_level() > 5) {
      add_failed_mess("You are too high a level to leave this guild.\n");
      return 0;
   }
   write("Are you sure you want to leave the guild (Y/N) : ");
   input_to("confirm_leave");
   return 1;
} /* do_leave() */


void confirm_leave(string str)
{
   str = lower_case(str);
   if (str != "y") {
      write("You are still in the guild.\n");
      return;
   }
   write("Goodbye.\n");
   call_other(our_guild, "leaving_guild");
   this_player()->set_start_pos(CONFIG_START_LOCATION);
   this_player()->set_guild_ob(0);
   this_player()->set_guild_data(0);
   this_player()->guild_commands();
   this_player()->save_me();
} /* confirm_leave() */


int check_primaries(string lpath, string g_o)
{
   int i;
   string dummy,
    *primaries;

   primaries = (string *) g_o->query_skills();
   if (member_array(lpath, primaries) != -1)
      return 1;
   for (i = 0; i < sizeof(primaries); i++)
      if (sscanf(primaries[i], lpath + ".%s", dummy))
         return 1;
   return 0;
} /* check_primaries() */


/*
 * The purpose of this function is to build an array in the same
 * format as what you get from SKILL_OB->query_skills, but only
 * containing the data for the primaries in this guild.
 */
mixed *filter_primaries(mixed *tmp_skills, string skill_path,
                        string *primaries)
{
   int i;
   string test_skill;

   for (i = 0; i < sizeof(tmp_skills); i += SKILL_ARR_SIZE)  {
      test_skill = skill_path + tmp_skills[i];
      if (sizeof(tmp_skills[i + SKILL_BIT]) != 0) /* not a leaf skill */
         tmp_skills[i + SKILL_BIT] =
            filter_primaries(tmp_skills[i + SKILL_BIT], test_skill + ".",
                             primaries);
      if (member_array(test_skill, primaries) == -1  &&
          sizeof(tmp_skills[i + SKILL_BIT]) == 0)
      {
         tmp_skills = tmp_skills[0 .. (i - 1)] +
                      tmp_skills[(i + SKILL_ARR_SIZE) .. ];
         i -= SKILL_ARR_SIZE;
      }
   }
   return tmp_skills;

} /* filter_primaries() */


private string rec_cost(mixed *arr, string path, int depth, string guild,
                        int brief)
{
   /* use lots of variables...so we only work things out minimum times */
   int i;
   int depth_gap;
   int ndots;
   int lvl;
   int max_lvl;
   int only_leaf;
   int tmp;
   string str;
   string sub_str;
   string lpath;

   str = "";
   depth_gap = (depth - 1) * 2;
   ndots = 19 - depth_gap;

   for (i = 0; i < sizeof(arr); i += SKILL_ARR_SIZE) {
      lpath = path + arr[i];
      lvl = this_player()->query_skill(lpath);
      if (!guild) {
         max_lvl = 5;
      } else {
         max_lvl = guild->query_skill_max_level(lpath);
      }

      only_leaf = SKILL_OB->query_only_leaf(lpath);

      /* If this is a "leaf-only" skill and it's not bottom level... */
      if (only_leaf  &&  sizeof(arr[i + SKILL_BIT]) > 0)  {
         sub_str = rec_cost(arr[i + SKILL_BIT], lpath + ".", depth + 1,
                            guild, brief);
         /* And if there are subskills to print under this skill... */
         if (sub_str != "")  {
            /*
             * Print a row of dots for this skill, then print the
             * subskill info and skip to the next skill
             */
            str += sprintf("%*'| 's%-*'.'s\n", depth_gap, "",
                           ndots + 18, arr[i]) + sub_str;
            continue;
         }
      }
      /* If we can actually learn this skill from the guild... */
      else if (SKILL_OB->query_allowed_to_teach(lpath))  {
         /* If we're over the guild max in this skill... */
         if (lvl >= max_lvl)  {
            /* And we're in brief mode... */
            if (brief)  {
               sub_str = rec_cost(arr[i + SKILL_BIT], lpath + ".", depth + 1,
                                  guild, brief);
               /* And if there are subskills to print under this skill... */
               if (sub_str != "")  {
                  /*
                   * Print a row of dots for this skill, then print the
                   * subskill info and skip to the next skill
                   */
                  str += sprintf("%*'| 's%-*'.'s\n", depth_gap, "",
                                 ndots + 18, arr[i]) + sub_str;
                  continue;
               }
            }
            /* But if we're not in brief mode, print the 'mastered' thingy */
            else str += sprintf("%*'| 's%-*'.'s%4d/%3d  mastered\n", depth_gap,
                                "", ndots, arr[i], lvl, max_lvl);
         }
         /* Otherwise, we're not over the max level, so print the cost info */
         else if (SKILL_OB->query_allowed_to_teach(lpath))  {
            tmp = query_skill_cost(lpath, 0);
            str += sprintf("%*'| 's%-*'.'s%4d/%3d %6d xp\n", depth_gap, "",
                           ndots, arr[i], lvl, max_lvl, tmp);
         }
      }

      /*
       *If we're capable of learning subskills for this skill, or if this
       * is a leaf-only skill, then go down one more level
       */
      if (lvl >= depth * 5  ||  only_leaf) {
         reset_eval_cost();
         str += rec_cost(arr[i + SKILL_BIT], lpath + ".", depth + 1, guild,
                         brief);
      }
   }
   return str;

} /* rec_cost() */


int do_cost_all(int brief)
{
   mixed *arr;
   string list;

   if ((string) this_player()->query_guild_ob() != our_guild) {
      add_failed_mess("You are not a member of this guild.\n");
      return 0;
   }
   arr = (mixed *) SKILL_OB->query_skills();
   if (!arr) {
      add_failed_mess("Hmmm, we appear to have an error here.\n");
      return 0;
   }
   list = sprintf("%-*'='s\n", (int) this_player()->query_cols(),
                  "======SKILLS=======Cur/Max==For Next");
   list += sprintf("%#*-s\n", (int) this_player()->query_cols() + 1,
                   rec_cost(arr, "", 1, our_guild, brief));
   list += sprintf("%*'='|s\n", (int) this_player()->query_cols(),
                   "> You have " + (int) this_player()->query_xp() +
                   " points to spend <");
   this_player()->more_string(list, "All", 1);
   return 1;

} /* do_cost_all() */


int do_cost_primaries()
{
   mixed *arr;
   string list;
   string *primaries;

   if ((string) this_player()->query_guild_ob() != our_guild) {
      add_failed_mess("You are not a member of this guild.\n");
      return 0;
   }
   arr = copy((mixed *)SKILL_OB->query_skills());
   if (!arr) {
      add_failed_mess("Hmmm, we appear to have an error here.\n");
      return 0;
   }

   primaries = our_guild->query_skills();
   arr = filter_primaries(arr, "", primaries);

   list = sprintf("%-*'='s\n", (int) this_player()->query_cols() - 1,
                  "======SKILLS=======Cur/Max==For Next");
   list += sprintf("%#*-s\n", (int) this_player()->query_cols(),
                   rec_cost(arr, "", 1, our_guild, 0));
   list += sprintf("%*'='|s\n", (int) this_player()->query_cols() - 1,
                   "> You have " + (int) this_player()->query_xp() +
                   " points to spend <");
   this_player()->more_string(list, "Primaries", 1);
   return 1;

} /* do_cost_primaries() */


int do_cost_internal(string skill, int to, int by)  {
   string *bits;
   string skill_name;
   string outer_name;
   string place;
   string cost_str;
   int lvl;
   int max_lvl;
   int i;
   int tmp;
   int cost;

   if ((string) this_player()->query_guild_ob() != our_guild) {
      add_failed_mess("You are not a member of this guild.\n");
      return 0;
   }

   bits = explode(implode(explode(skill, " "), "."), ".");
   if (!(skill_name = (string) SKILL_OB->query_skill(bits))) {
      add_failed_mess("There is no such skill as " + skill + ".\n");
      return 0;
   }

   lvl = (int) this_player()->query_skill(skill_name);
   if (!by && to)
      by = to - lvl;
   if (by == 0) {
      add_failed_mess
         ("It won't cost you anything to stay at the same level!\n");
      return 0;
   }
   if (by < 0) {
      add_failed_mess("We can't give refund xp for regressing skills!\n");
      return 0;
   }

   bits = explode(skill_name, ".");
   if (sizeof(bits) > 1  &&  !SKILL_OB->query_only_leaf(skill_name)) {
      outer_name = implode(bits[0 .. <2], ".");
      if (!SKILL_OB->is_skill_ignored(outer_name) &&
          ((sizeof(bits) - 1) * 5) > this_player()->query_skill(outer_name))
      {
         add_failed_mess("You are not at a high enough level in " +
                         outer_name + " to advance " + skill_name +
                         " yet.\n");
         return 0;
      }
   }

   if (!SKILL_OB->query_allowed_to_teach(skill_name)) {
      add_failed_mess("You cannot learn '" + skill_name +
                      "' from your guild.\n");
      return 0;
   }

   max_lvl = (int) our_guild->query_skill_max_level(skill_name);
   if (lvl >= max_lvl) {
      add_failed_mess("Your guild prevents you even considering advancing " +
                      skill_name + " above level " + max_lvl + ".\n");
      return 0;
   }

   for (i = 0; i < by; i++) {
      if ((lvl + i) >= max_lvl) {
         write("Your guild prevents you considering all the advancement you "
               + "requested.  You can only advance up to level " + max_lvl +
               " in " + skill_name + " here.\n");
         break;
      }
      tmp += query_skill_cost(skill_name, i);

   }

   // Now work out the monetary cost if any.
   place = this_object()->query_property("place");
   if (!place)
      place = "default";
   cost_str = "";
   if (cost_div) {
      cost = tmp / cost_div;
      cost += cost % MONEY_HAND->query_smallest_value_in(place);
      if (cost)
         cost_str = " and " + MONEY_HAND->money_value_string(cost, place);
   }

   printf("It would cost you %d xp%s to raise %s from level %d to %d.\n",
          tmp, cost_str, skill_name, lvl, (lvl + i));
   return 1;

} /* do_cost_internal() */


int do_cost(string skill)
{
   mixed *arr;
   int i;
   int depth;
   string list;
   string exp_skill;
   string *sub_skills;
   int only_leaf;

   if ((string) this_player()->query_guild_ob() != our_guild) {
      add_failed_mess("You are not a member of this guild.\n");
      return 0;
   }
   arr = (mixed *) SKILL_OB->query_skills();
   if (!arr) {
      add_failed_mess("Hmmm, we appear to have an error here.\n");
      return 0;
   }

   exp_skill =
      SKILL_OB->query_skill(explode(implode(explode(skill, " "), "."), "."));
   if (!exp_skill)  {
      add_failed_mess("There is no such skill as " + skill + ".\n");
      return 0;
   }

   if (SKILL_OB->query_allowed_to_teach(exp_skill))  {
      i = member_array(exp_skill, arr);
      only_leaf = SKILL_OB->query_only_leaf(exp_skill);
      if (only_leaf)  {
         sub_skills = explode(exp_skill, ".");
         foreach (skill in sub_skills[0 .. < 2])  {
            i = member_array(skill, arr);
            arr = arr[i + 3];
         }
         i = member_array(sub_skills[<1], arr);
         depth = sizeof(sub_skills) + 1;
      }
      else depth = 2;
      if (sizeof(arr[i + 3]) > 0)  {
         list = sprintf("%-*'='s\n", (int) this_player()->query_cols() - 1,
                        "======SKILLS=======Cur/Max==For Next") +
                sprintf("%#*-s\n", (int) this_player()->query_cols(),
                         rec_cost(arr[i + SKILL_BIT], exp_skill + ".", depth,
                         our_guild, 0)) +
                sprintf("%*'='|s\n", (int) this_player()->query_cols() - 1,
                         "> You have " + (int) this_player()->query_xp() +
                         " points to spend <");
         this_player()->more_string(list, capitalize(exp_skill), 1);
         return 1;
      }
   }
   return do_cost_internal(exp_skill, 0, 1);

} /* do_cost() */


int do_cost_to(string skill, int num)
{
   return do_cost_internal(skill, num, 0);
} /* do_cost_to() */

int do_cost_by(string skill, int num)
{
   return do_cost_internal(skill, 0, num);
} /* do_cost_to() */


int do_info()
{
   mixed *arr;
   string *primaries;

   write(our_guild->long() +
         "\nThis guild's primary skills and costs are:\n");
   arr = copy((mixed *)SKILL_OB->query_skills());
   if (!arr) {
      add_failed_mess("Hmmm, we appear to have an error here.\n");
      return 0;
   }
   primaries = our_guild->query_skills();
   arr = filter_primaries(arr, "", primaries);
   printf("%-*'='s\n", this_player()->query_cols() - 1,
          "======SKILLS=======Cur/Max==For Next");
   printf("%#*-s\n", this_player()->query_cols(),
          rec_cost(arr, "", 1, our_guild, 0));
   printf("%*'='|s\n", this_player()->query_cols() - 1, "> You have " +
          this_player()->query_xp() + " points to spend <");
   return 1;

} /* do_info() */


void set_teaching_person(object ob)
{
   teaching_person = ob;
}

object query_teaching_person()
{
   return teaching_person;
}


void add_spell(string name, string *blue, int lvl)
{
   string path;

   path = implode(blue, ".");
   if (!spells[path])
      spells[path] = ({ name, lvl });
   else
      spells[path] += ({ name, lvl });
} /* add_spell() */


void add_command(string name, string *blue, int lvl)
{
   string path;

   path = implode(blue, ".");

   if (!commands)
      commands = ([ ]);
   if (!commands[path])
      commands[path] = ({ name, lvl });
   else
      commands[path] += ({ name, lvl });

} /* add_command() */


void check_spells_commands(string *sk, int lvl)
{
   string *bits;
   int val;

   int i;
   int j;

   if (!teaching_person)
      return;
   bits = keys(spells);
   for (i = 0; i < sizeof(bits); i++) {
      val = this_player()->query_skill(bits[i]);
      for (j = 0; j < sizeof(spells[bits[i]]); j += 2)
         if ((spells[bits[i]][j + 1] < val) &&
             !(this_player()->query_spell(spells[bits[i]][j])))
            teaching_person->init_command("teach " + spells[bits[i]][j] +
                                          " to " +
                                          this_player()->query_name());
   }
   bits = keys(commands);
   for (i = 0; i < sizeof(bits); i++) {
      val = this_player()->query_skill(bits[i]);
      for (j = 0; j < sizeof(commands[bits[i]]); j += 2)
         if ((commands[bits[i]][j + 1] < val) &&
             !(this_player()->query_known_command(commands[bits[i]][j])))
            teaching_person->init_command("teach " + commands[bits[i]][j] +
                                          " to " +
                                          this_player()->query_name());
   }
} /* check_spell_commands() */


mapping query_spells()
{
   return spells + ([ ]);
}

mapping query_commands()
{
   return commands + ([ ]);
}

string query_start_pos()
{
   return start_pos;
}

string query_our_guild()
{
   return our_guild;
}
