inherit "/std/object";
#include "skills.h"
#include "commands.h"
#include "tune.h"

string *commands;
mixed spells;
int heal_loop;
string start_pos;

void create() {
  commands = ({ });
  spells = ({ });
  start_pos = "/d/am/am/mendeddrum";
  ::create();
}

void set_start_pos(string str) {
  start_pos = str;
}

string query_start_pos() { return start_pos; }

int add_command(string name, int add_if_player) {
  if (member_array(name, commands) != -1)
    return 0;
  commands += ({ name, ({ add_if_player }) });
  return 1;
}

int remove_command(string name) {
  int i;

  if ((i = member_array(name, commands)) == -1)
    return 0;
  commands = delete(commands, i, 1);
  return 1;
}

int do_command(string name, mixed param) {
  mixed *tmp;

  if (member_array(name, commands))
    return 0;
  tmp = (mixed *)COMMAND_SERVER->query_command(name);
  return (int)call_other(tmp[0], tmp[1], param);
}

int add_spell(string name, object ob, string func_name, int add_if_player) {
  int i;

  if ((i = member_array(name, spells)) != -1)
    return 0;
  spells += ({ name, ({ ob, func_name, add_if_player }), });
  return 1;
}

int remove_spell(string name) {
  int i;

  if ((i= member_array(name, spells)) == -1)
    return 0;
  spells = delete(spells, i, 2);
  return 1;
}

int cast_spell(string name, mixed bing) {
  int i;

  if ((i=member_array(name, spells)) == -1)
    return 0;
  return (int)call_other(spells[i+1][0], spells[i+1][1], bing);
}

mixed query_spells() { return spells; }

string *query_commands() { return commands; }

/*
 * The next two functions should be redefined on any guild item you write...
 */
int query_skill_cost(string skill) {
  return 10;
}

int query_skill_max_level(string skill) {
  return 10;
}

/*
 * This function allows you to give specific bonuses on certain
 * skills.  Useful if you are a specialty guild of some sort,
 * or you wish to make people able to specialise in a certain
 * sort of weapon/spell.
 */
/* removed into /std/living/skills */
#if 0
int query_skill_bonus(int lvl, string skill) {
  string stat_bonus;
  int bonus, gbon, stat, i, lvl_bon;

  stat_bonus = (string)SKILL_OB->query_skill_stat(skill);
  for (i=0;i<strlen(stat_bonus);i++) {
    switch(stat_bonus[i]) {
      case 'C' : stat = (int)previous_object()->query_con();
                 break;
      case 'S' : stat = (int)previous_object()->query_str();
                 break;
      case 'D' : stat = (int)previous_object()->query_dex();
                 break;
      case 'W' : stat = (int)previous_object()->query_wis();
                 break;
      case 'I' : stat = (int)previous_object()->query_int();
                 break;
    }
    bonus += (stat - 13) * 3;
  }
  if (lvl > 60)
    lvl_bon = 170+((lvl-60)>>1);
  else if (lvl > 40)
    lvl_bon = 150+(lvl-40);
  else if (lvl > 20)
    lvl_bon = 100+(((lvl-20)*5)>>1);
  else
    lvl_bon = lvl*5;
  if (strlen(stat_bonus))
    return lvl_bon + (bonus*lvl_bon/(strlen(stat_bonus)*60));
  return lvl_bon;
}
#endif

string query_title(object player) {
  int lvl;

  return "Level "+lvl+" in guild";
}

/*
 * cant think of anything I want to do here at the moment...
 * This is where you would set up any shadows you want on the player
 * object, or just do what you will..
 */
void start_player(object pl) {
  int i;
  if (!spells)
    spells = ({ });

  for (i=0;i<sizeof(spells);i+=2)
    if (spells[i+1][2] || !interactive(pl))
      pl->add_spell(spells[i], spells[i+1][0], spells[i+1][1]);
  if (!pointerp(commands))
    commands = ({ });
  for (i=0;i<sizeof(commands);i+=2)
    if (commands[i+1][0] || !interactive(pl))
      pl->add_known_command(commands[i]);
}

void leave_guild() {
  int i;

/*
  for (i=0;i<sizeof(spells);i+=2)
    this_player()->remove_spell(spells[i]);
 */
}

/*
 * Guess what this is called when the player quits...
 * set up anything stupid you want to do on here...
 */
void player_quit() {
}

/*
 * this is called every time the player saves his character
 */
void player_save() {
}

/*
 * Decied to make being drunk not count for anything...
 */
void player_heart_beat(int intox) {
  int bon;

  bon = (int)this_player()->query_con()*5;
/*
  if (intox) {
    if (bon < 40)
      this_player()->adjust_hp(4);
    else
      this_player()->adjust_hp(bon/10);
  } else {
 */
    if (bon < 40)
      this_player()->adjust_hp(1);
    else
      this_player()->adjust_hp(bon/40);
/*
  }
 */
  bon = (int)this_player()->query_int()*5;
/*
  if (intox) {
    if (bon < 40)
      this_player()->adjust_gp(4);
    else
      this_player()->adjust_gp(bon/10);
  } else {
 */
    if (bon < 40)
      this_player()->adjust_gp(1);
    else
      this_player()->adjust_gp(bon/40);
/*
  }
 */
}

void set_level(object ob, int lvl, string class) {
  call_out("set_new_level", 0, ({ ob, lvl, class }) );
}

void set_new_level(mixed *arr) {
  object ob;
  int lvl;

  ob = arr[0];
  lvl = arr[1];
  ob->add_skill_level("fighting", lvl);
  ob->add_skill_level("other", lvl);
  ob->add_skill_level("magic", lvl);
  ob->add_skill_level("faith", lvl);
  ob->add_skill_level("occult", lvl);
  ob->set_int(13);
  ob->set_dex(13);
  ob->set_wis(13);
  ob->set_con(13);
  ob->set_str(13);
  ob->set_hp(100000);
  ob->set_gp(100000);
  ob->reset_all();
/*
  ob->add_attack("standard", 0, 100, 40+lvl*10, 0, 0, "blunt");
 */
}

/* returns the character level...
 */
int query_level(object ob) {
  return (int)ob->query_skill("other");
}
