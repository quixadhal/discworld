#include "armoury.h"
inherit "/std/room";
#define FILE_NAME "/save/armoury"


mixed weaps, armours;

#define TEST_CALLER \
  if (sscanf(file_name(previous_object()), "/obj/misc/add_armoury#%d", tmp) != 1) { \
    write("No No you tried to add from an invalid object.\n"); \
    return 0; \
  }

void setup() {
  set_light(100);
  weaps = ({ });
  armours = ({ });
  set_short("The Discworld Armoury");
  set_long("You are in the Discworld armoury.  People cannot normally get "+
           "here.  Commands available are: weapons, armours and request.\n");
}

void reset() {
  restore_object(FILE_NAME);
}

int armours(string str) {
  int i;
  string bit;

  bit = "";
  for (i=0;i<sizeof(armours);i+=2)
    bit += armours[i]+"\n";

  printf("Available armours are:\n%-*#s\n", this_player()->query_cols(), bit);
  return 1;
}

int weapons(string str) {
  int i;
  string bit;

  bit = "";
  for (i=0;i<sizeof(weaps);i+=2)
    bit += weaps[i]+"\n";

  printf("Available weapons are:\n%-*#s\n", this_player()->query_cols(), bit);
  return 1;
}
 
void init() {
  ::init();
  add_action("weapons", "weapons");
  add_action("armours", "armours");
  add_action("request", "request");
}

object request_armour(string name, int per);
object request_weapon(string name, int per);

int request(string str) {
  string s1, s2;
  object ob;
  int per;

  notify_fail("Usage: request <armour|weapon> <percentage> <name>\n");
  if (!str)
    return 0;
  str = lower_case(str);
  if (sscanf(str, "%s %d %s", s1, per, s2) != 3)
    return 0;
  if (s1 != "armour" && s1 != "weapon")
    return 0;
  if (s1 == "armour")
    ob = request_armour(s2,per);
  else
    ob = request_weapon(s2,per);
  if (!ob)
    write("Sorry but the "+s1+", "+s2+" does not exist.\n");
  else {
    if (ob->move(this_player())) {
      ob->move(this_object());
      write("You cant carry it so it has been put here.\n");
    } else
      write("Ok created and put in you.\n");
  }
  return 1;
}

mixed request_weapon(string str, int per) {
  object ob;
  int i, j;

  if ((i=member_array(str, weaps)) == -1)
    return 0;

  if (weaps[i+1][W_OBJ] && weaps[i+1][W_OBJ] != "")
    ob = clone_object(weaps[i+1][W_OBJ]);
  if (!ob) { /* just in case..... should log the error somewhere? */
    ob = clone_object("/obj/weapon");
    ob->set_value(weaps[i+1][W_VALUE]);
    ob->set_name(weaps[i+1][W_NAME]);
    ob->add_alias(weaps[i+1][W_ALIAS]);
    ob->add_plural(weaps[i+1][W_PLURALS]);
    ob->add_adjective(weaps[i+1][W_ADJ]);
    ob->set_weight(weaps[i+1][W_WEIGHT]);
    ob->set_short(weaps[i+1][W_SHORT]);
    ob->set_long(weaps[i+1][W_LONG]);
    ob->set_weap_skill(weaps[i+1][W_SKILL]);
    ob->new_weapon(weaps[i+1][W_COND]);
    ob->set_damage_chance(weaps[i+1][W_DAM_CHANCE]);
    for (j=0;j<sizeof(weaps[i+1][W_ATTACKS]);j+=2)
      ob->add_arr_attack(weaps[i+1][W_ATTACKS][j], weaps[i+1][W_ATTACKS][j+1]);
  }
  ob->set_percentage(per);
  return ob;
}

mixed request_armour(string name, int per) {
  int j, i;
  object ob;

  if ((i = member_array(name, armours)) == -1)
    return 0;

  if (armours[i+1][A_OBJ] && armours[i+1][A_OBJ] != "")
    ob = clone_object(armours[i+1][A_OBJ]);
  if (!ob) {
    ob = clone_object("/obj/armour");
    ob->set_value(armours[i+1][A_VALUE]);
    ob->set_name(armours[i+1][A_ANAME]);
    ob->add_alias(armours[i+1][A_ALIAS]);
    ob->add_plural(armours[i+1][A_PLURALS]);
    ob->add_adjective(armours[i+1][A_ADJ]);
    ob->set_weight(armours[i+1][A_WEIGHT]);
    ob->set_short(armours[i+1][A_SHORT]);
    ob->set_long(armours[i+1][A_LONG]);
    ob->set_dex_minus(armours[i+1][A_DEX_MINUS]);
    ob->setup_armour(armours[i+1][A_COND]);
    ob->set_immune(armours[i+1][A_IMMUNE]);
    ob->set_type(armours[i+1][A_ATYPE]);
    ob->set_damage_chance(armours[i+1][A_CHANCE]);
    for (j=0;j<sizeof(armours[i+1][A_ARMOURS]);j+=2)
      ob->add_ac(armours[i+1][A_ARMOURS][j], armours[i+1][A_ARMOURS][j+1][0],
                 armours[i+1][A_ARMOURS][j+1][1]);
  }
  ob->set_percentage(per);
  return ob;
}

int add_weapon(string name, mixed *arr) {
  int i, tmp;
  if ((i=member_array(name, weaps)) != -1)
    return 0;
  TEST_CALLER
  log_file("ARMOURY", this_player()->query_name()+" added weapon "+name+"\n");
  weaps += ({ name, arr });
  save_object(FILE_NAME);
  return 1;
}

int remove_weapon(string name) {
  int i, tmp;

  if ((i=member_array(name, weaps)) == -1)
    return 0;
  TEST_CALLER
  log_file("ARMOURY", this_player()->query_name()+" deleted weapon "+name+"\n");
  weaps = delete(weaps, i, 2);
  save_object(FILE_NAME);
  return 1;
}

int add_armour(string name, mixed *arr) {
  int i, tmp;
  if ((i=member_array(name, armours)) != -1)
    return 0;
  TEST_CALLER
  log_file("ARMOURY", this_player()->query_name()+" added armour "+name+"\n");
  armours += ({ name, arr });
  save_object(FILE_NAME);
  return 1;
}

int remove_armour(string name) {
  int i, tmp;

  if ((i=member_array(name, armours)) == -1)
    return 0;
  TEST_CALLER
  log_file("ARMOURY", this_player()->query_name()+" deleted armour "+name+"\n");
  armours = delete(armours, i, 2);
  save_object(FILE_NAME);
  return 1;
}

mixed query_armours() { return armours + ({ }); }
mixed query_weapons() { return weaps + ({ }); }

dest_me() {
  save_object(FILE_NAME);
  ::dest_me();
}

mixed query_armour(string name) {
  int i;

  if ((i=member_array(name,armours)) == -1)
    return 0;
  return armours[i+1];
}

mixed query_weapon(string name) {
  int i;

  if ((i=member_array(name, weaps)) == -1)
    return 0;
  return weaps[i+1];
}

string nice_array(mixed bing) {
  if (pointerp(bing)) {
    switch (sizeof(bing)) {
      case 1 :
        return "({ "+bing[0]+" })";
      case 2 :
        return "({ "+bing[0]+", "+bing[1]+" })";
      case 3 :
        return "({ "+bing[0]+", "+bing[1]+", "+bing[2]+" })";
    }
  }
  return bing+"";
} /* nice_array() */

string add_quotes(string str) {
  return "\""+str+"\"";
} /* add_quotes() */

/*
 * Creates nice virtual object files from the old armoury format.
 */
void create_files() {
  int i, j;
  string fname, *arr;

  seteuid("Root");
  for (i=0;i<sizeof(weaps);i+=2) {
    if (weaps[i+1][W_OBJ] && weaps[i+1][W_OBJ] != "")
      continue;
    fname = "/obj/weapons/"+replace(weaps[i], " ", "_")+".wep";
    rm(fname); /* Don't wanna do stuipd womblely thingys */
    write("Createing "+fname+"\n");
    write_file(fname, "::Name::\""+weaps[i+1][W_NAME]+"\"\n");
    write_file(fname, "::Short::\""+weaps[i+1][W_SHORT]+"\"\n");
    arr = map_array(weaps[i+1][W_ADJ], "add_quotes", this_object());
    write_file(fname, "::Adjective::({ "+implode(arr, ", ")+" })\n");
    arr = map_array(weaps[i+1][W_ALIAS], "add_quotes", this_object());
    write_file(fname, "::Alias::({ "+implode(arr, ", ")+" })\n");
    arr = map_array(weaps[i+1][W_PLURALS], "add_quotes", this_object());
    write_file(fname, "::Plural::({ "+implode(arr, ", ")+" })\n");
    write_file(fname, "::Long:: \""+weaps[i+1][W_LONG]+"\n\"\n");
    write_file(fname, "::Weight:: "+weaps[i+1][W_WEIGHT]+"\n");
    write_file(fname, "::Value:: "+weaps[i+1][W_VALUE]+"\n");
    if (stringp(weaps[i+1][W_SKILL])) {
      write_file(fname, "::Weapon Skill:: \""+weaps[i+1][W_SKILL]+"\"\n");
    }
    write_file(fname, "::Setup:: "+weaps[i+1][W_COND]+"\n");
    for (j=0;j<sizeof(weaps[i+1][W_ATTACKS]);j+=2)
      write_file(fname, "::Attack:: \""+weaps[i+1][W_ATTACKS][j]+"\", "+
                                 weaps[i+1][W_ATTACKS][j+1][0]+", "+
                                 weaps[i+1][W_ATTACKS][j+1][1]+", "+
                                 nice_array(weaps[i+1][W_ATTACKS][j+1][2])+", "+
                                 nice_array(weaps[i+1][W_ATTACKS][j+1][3])+", "+
                                 nice_array(weaps[i+1][W_ATTACKS][j+1][4])+", "+
                                 "\""+weaps[i+1][W_ATTACKS][j+1][5]+"\"\n");
    weaps[i+1][W_OBJ] = fname;
  }
  for (i=0;i<sizeof(armours);i+=2) {
    if (armours[i+1][A_OBJ] && armours[i+1][A_OBJ] != "")
      continue;
    fname = "/obj/armours/"+replace(armours[i], " ", "_")+".arm";
    rm(fname); /* Don't wanna do stuipd womblely thingys */
    write("Createing "+fname+"\n");
    write_file(fname, "::Name::\""+armours[i+1][A_ANAME]+"\"\n");
    write_file(fname, "::Short::\""+armours[i+1][A_SHORT]+"\"\n");
    arr = map_array(armours[i+1][A_ADJ], "add_quotes", this_object());
    write_file(fname, "::Adjective::({ "+implode(arr, ", ")+" })\n");
    arr = map_array(armours[i+1][A_ALIAS], "add_quotes", this_object());
    write_file(fname, "::Alias::({ "+implode(arr, ", ")+" })\n");
    arr = map_array(armours[i+1][A_PLURALS], "add_quotes", this_object());
    write_file(fname, "::Plural::({ "+implode(arr, ", ")+" })\n");
    write_file(fname, "::Long:: \""+armours[i+1][A_LONG]+"\n\"\n");
    write_file(fname, "::Weight:: "+armours[i+1][A_WEIGHT]+"\n");
    write_file(fname, "::Value:: "+armours[i+1][A_VALUE]+"\n");
    write_file(fname, "::Setup:: "+armours[i+1][A_COND]+"\n");
    write_file(fname, "# I don't think dex minus is implemented, but leave in "+
                      "for completeness.\n");
    write_file(fname, "::Dex Minus:: "+armours[i+1][A_DEX_MINUS]+"\n");
    arr = map_array(armours[i+1][A_IMMUNE], "add_quotes", this_object());
    write_file(fname, "::Immune::({ "+implode(arr, ", ")+" })\n");
    write_file(fname, "::Type:: \""+armours[i+1][A_ATYPE]+"\"\n");
    write_file(fname, "::Damage Chance:: "+armours[i+1][A_CHANCE]+"\n");
    for (j=0;j<sizeof(armours[i+1][A_ARMOURS]);j+=2)
      write_file(fname, "::Ac:: \""+armours[i+1][A_ARMOURS][j]+"\", "+
                             "\""+armours[i+1][A_ARMOURS][j+1][0]+"\", "+
                             nice_array(armours[i+1][A_ARMOURS][j+1][1])+"\n");
    armours[i+1][A_OBJ] = fname;
  }
  save_object(FILE_NAME);
  seteuid("Room");
} /* create_files() */
