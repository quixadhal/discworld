#include "path.h"
 
inherit "std/guild";
 
#define OUTSIDE "d/am/am/smallgod2"
#define ARMOURY "obj/handlers/armoury"
 
object board,
       priest,
       mace;
mixed *spells;
 
void do_raise(object ob);
 
void setup() {
  object note;
  set_guild("/std/guilds/priest");
  set_light(60);
  set_co_ord(({ -150, -205, 0 }));
  set_short("Temple of Small Gods");
  set_long(
    "This is a very empty temple.  There is a note floating beside " +
    "one of the walls.\n");
  add_item("note", "You come to the conclusion that it can be read.\n");
  add_item("wall", "There is a note floating in front of it.\n");
  add_item("walls", "They tend to be vertical and flat in nature.\n");
  add_exit("east", OUTSIDE, "gate");
  add_exit("north", ROOM+"priestshop", "standard");
  set_zone("temple of small gods");
  note = clone_object("/std/object");
  note->set_name("note");
  note->reset_get();
  note->set_long("A small not at all puce note floating in front of "+
                 "the wall.\n");
  note->set_read_mess(
    "Type \"advance\" to advance your levels, \"join\" to join up, " +
    "\"info\" to get information about the guild,  \"leave\" to " +
    "leave the guild,  \"cost\" to see how much all the skills " +
    "cost to advance and \"join\" to join up.\n");
  hidden_objects += ({ note });
  add_spell("raise", ({ "faith", "spells", "curing" }), 0);
  add_spell("cure light wounds", ({ "faith", "spells", "curing" }), 0);
  add_spell("calm", ({ "faith", "spells", "defensive" }), 5);
  add_spell("cure serious wounds", ({ "faith", "spells", "curing" }), 10);
  add_spell("cure disease", ({ "faith", "spells", "curing" }), 18);
  add_spell("cure critical wounds", ({ "faith", "spells", "curing" }), 20);
  add_spell("remove curse", ({ "faith", "spells", "curing" }), 25);
  add_spell("restore", ({ "faith", "spells", "curing" }), 40);
  add_spell("ressurect", ({ "faith", "spells", "curing" }), 70);
  add_spell("heal", ({ "faith", "spells", "curing" }), 100);
  add_spell("extra life", ({ "faith", "spells", "curing" }), 120);
 
   add_property("place of power",1);   /* added by [als] 20/10/92 ... */
} /* setup() */
 
int do_join(string str) {
  if (this_player()->query_property("killed harry")) {
    notify_fail("Thought you could get away with killing harry hey?\n");
    return 0;
  }
  return ::do_join(str);
} /* do_join() */
 
void reset() {
  object ob;
 
  if (!priest) {
    priest = clone_object("/obj/monster");
    priest->set_name("harry");
    priest->add_alias("priest");
    priest->set_long(
"A old wise looking priest.  He has long flowing robes and his sharp eyes "+
"watch you carefully.  He looks friendly enough, and he likes to "+
"help people by raising their souls from the dead - but he also "+
"has a LONG memory of people who have wronged him in a past life.\n");
    priest->set_race("human");
    priest->set_gender("male");
    ob = (object)ARMOURY->request_weapon("mace",100);
   ob->add_property("harry's mace", 1);
    if (!mace) {
      ob->set_enchant(50);
      ob->add_attack("bing", "blunt", 30, 50, 0, 0, "magic");
      ob->set_wield_func("harry_wield", file_name(this_object()));
      mace = ob;
    }
    priest->set_class("priest");
    priest->set_level(200);
    priest->add_skill_level("fighting",200);
    ob->move(priest);
    priest->add_ac("bing", "blunt", ({ 30, 30 }));
    priest->add_ac("bing2", "sharp", ({ 30, 30 }));
    priest->adjust_wis(10);
    priest->adjust_bonus_wis(5);
    priest->adjust_con(5);
    priest->adjust_str(3);
    priest->init_equip();
    priest->move(this_object());
    set_teaching_person(priest);
  }
  if (board) return;
  board = clone_object("obj/misc/board");
  board->set_datafile("clericguild");
  board->move(this_object());
} /* reset() */
 
void dest_me() {
  if (board) board->dest_me();
  if (mace && environment(mace) == priest) mace->dest_me();
  if (priest) priest->dest_me();
  ::dest_me();
} /* dest_me() */
 
void init() {
  ::init();
  if (this_player()->query_property("killed harry")) {
    if (priest) priest->init_command("glare "+this_player()->query_name());
  }
  if (this_player()->query_property("dead") && priest &&
      !sizeof((object *)priest->query_attacker_list()))
    call_out("do_raise", 2, this_player());
} /* init() */
 
int read(string str) {
  if (str != "note") return 0;
  say((string)this_player()->query_cap_name() +
    " reads the note.\n", this_player());
  write("Type \"advance\" to advance your levels, \"join\" to join up,\n" +
    "\"info\" to get information about the guild,  \"leave\" to\n" +
    "leave the guild,  \"cost\" to see how much all the skills\n" +
    "cost to advance and \"join\" to join up.\n\n");
  return 1;
} /* raise() */
 
void event_death(object ob) {
  int i;
  object *obs;
 
  if (ob == priest) {
    obs = (object *)priest->query_attacker_list();
    for (i=0;i<sizeof(obs);i++) {
      obs[i]->add_property("killed harry", time());
/* Chuck em out! */
      if ((string)obs[i]->query_guild_ob() == our_guild) {
        tell_object(obs[i], "Your god renounces you for killing his top "+
                            "priest.  You have been forcibly ejected "+
                            "from the priest guild.\n");
        obs[i]->set_guild_ob(0);
      }
    }
  }
} /* event_death() */
 
void do_raise(object ob) {
 
  if (ob->query_property("killed harry")) {
    tell_object(ob, "Harry pointedly ignores you.\n");
    say("Harry pointedly ignores "+ob->query_cap_name()+".\n");
    return ;
  }
  if (ob->query_property("noregen")) {
    tell_object(ob, "Death tells you: HOLD ON, I'M NOT FINISHED " +
      "WITH YOU YET.\n");
    return;
  }
  tell_object(ob, "Harry summons the gods for you.\n");
  say("Harry summons the gods to raise " +
    (string)ob->query_cap_name() + " from the dead.\n", ob);
  ob->remove_ghost();
  return;
} /* do_raise() */
 
void check_spells_commands(string *path, int lvl) {
  int i, j, not_this_one;
  mixed *spell_arr, spell_num;
 
  if (!teaching_person || member_array(this_player(), 
      (object *)teaching_person->query_attacker_list()) != -1)
    return ;
  
  spell_arr = (mixed *)our_guild->query_spells();
 
  for (i=0;i<sizeof(spells);i+=3) {
    j = 0;
    not_this_one = 0;
    
    while (j < sizeof(spells[i+1]) && j < sizeof(path)) {
       if (spells[i+1][j] != path[j]) { not_this_one = 1; break; }
       j++;
   }
    
    if (not_this_one) continue;
 
    if (lvl < spells[i+2]) continue;
    spell_num = member_array(spells[i], spell_arr);
    if (spell_num < 0) continue;
    if (this_player()->query_spell(spells[i])) continue;
    tell_room(this_object(), "Harry instructs " + this_player()->short() +
       " on the finer details of casting " + spells[i] + ".\n", this_player());
    tell_object(this_player(), "Harry instructs you in how to cast " +
       spells[i] + ".\n");
    (void)this_player()->add_spell(spells[i], spell_arr[spell_num + 1][0], 
       spell_arr[spell_num + 1][1]);
  }
 
/* forget commands for now */
#if 0
  for (i=0;i<sizeof(commands);i+=3)
    for (j=0;j<sizeof(commands[i+1]);j++)
      if (j >= sizeof(path)) {  /* got through the skill path ok */
        if (commands[i+2] <= lvl)
           teaching_person->init_command("teach "+commands[i]+" to "+
                                         this_player()->query_name());
      } else
        if (commands[i+1][j] != path[j])
          break;
#endif
} /* check_spell_commands() */
 
void event_fight_in_progress(object ob, object ob2) {
if (ob == priest) {
  call_out("check_weap", 0, ob2);
  } else if (ob2 == priest)
    call_out("check_weap", 0, ob);
  if (!random(100)) {
    call_out("do_calm", 0);
  }
} /* event_fight_in_progress() */
 
void check_weap(object ob) {
  object weap;
 
  weap = (object)ob->query_weapon();
  if (weap && (string)weap->query_name() == "mace" && (int)weap->query_enchant() == 50) {
    tell_room(this_object(), "You hear a sudden clap of thunder and a bolt "+
             "of something flashes through the room disintergrating "+
            ob->query_cap_name()+"'s mace.\n", ob);
    weap->dest_me();
    tell_object(ob, "A bolt of pure something blasts through the sky "+
                    "and disintergrates your mace.  You hear a tinkling "+
                    "voice say something about killing high priests with "+
                    "the weapon on their god.\n");
  }
} /* check_weap() */
 
void do_calm() {
  int i;
  object *obs2;
  int j;
  object *obs;
 
  tell_room("An overpowering sense of calmness fills the room like "+
           "tacky smoke.\n");
  obs = all_inventory(this_object());
  for (i=0;i<sizeof(obs);i++) {
    obs2 = (object *)obs[i]->query_attacker_list();
    for (j=0;j<sizeof(obs2);j++)
     obs[i]->stop_fight(obs2[i]);
  }
} /* do_calm() */

int harry_wield(object ob) {
/* Let em wield it for a bit... */
  call_out("delay_wield", 0, previous_object(), ob);
  return 1;
} /* harry_wield() */

int delay_wield(object mace, object pl) {
  if (!pl) return 1;
  if (pl->query_al() > 100 ||
      pl->query_property("killed harry")) {
    tell_room(environment(pl), "The mace suddenly glows with a light and burns "+
        pl->query_cap_name()+" hands.\n", ({ pl }));
    tell_object(pl, "The mace glows and you suddenly drop it.\n");
     mace->move(environment(pl));
    pl->adjust_hp(-200);
    return 0;
  }
  return 1;
} /* harry_wield() */
