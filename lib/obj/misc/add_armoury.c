#include "weapon.h"
#include "armoury.h"
inherit "/std/object";

#define CHECK if (!str || str == "") {\
                write("Aborted.\n");\
                do_next(); \
                return 1;\
              }

int do_next();
int do_edit_wc();
int do_wc_remove();
int do_wc_change();
int edit_array();
int edit_ac();

int weapon,
    in_add,
    up_to;
string add_id, array_name;
string thing_name, end_command;
mixed *data_array,
      tmp,
      *desc_array;

void setup() {
  reset_drop();
  set_name("adder");
  set_short("armoury adder");
  add_adjective("armoury");
  set_main_plural("armoury adders");
  add_plural("adders");
  set_weight(40);
  desc_array = ({
"What name do you want your object to be called in the armoury?\n"+
"Id : ", "get_id", 0,
"Do you want to an object different to the standard weapon or armour?\n"+
"Object path : ", "get_object_name", 0,
"What name do you want to give your object (NB: used for iding)?\n"+
"Name : ", "get_name", 0,
"Please add any aliases you want.  Return on a blank line to end.\n"+
"Alias : ", "get_alias", "edit_alias",
"Please add any plurals you want.  Return on a blank line to end.\n"+
"Plural : ", "get_plural", "edit_plural",
"Please add any adjectives you want for your object.\n"+
"Adjective : ", "get_adjective", "edit_adjective",
"What is the short description of the object?\n"+
"Short : ", "get_short", 0,
"Just press return on the next line.  Input to it is ignored.\n"+
"What is the long description of the object (Entering ed)?\n",
"get_long", 0,
"How much does your object weigh ? ",
"get_weight", 0,
"How does your object cost (in brass coins) ? ",
"get_value", 0,
"What is the maximum condition of your object?\n"+
"Condition : ", "get_cond", 0,
"What is the damage chance for it ? ",
"get_chance", 0,
({ "Ok now for specific stuff.\nWhat type is your armour ? ",
   "Ok now for the weapon specific stuff.\nWhat skill is used with "+
   "your weapon ? " }), ({ "get_type", "get_skill" }), ({ 0, 0, }),
({ "Dexterity minus for using it ? ",
   "Enter the weapon attacks.\nWhat is the name of the attack ? ", }),
({ "get_dex_minus", "get_wc", }), ({ 0, "edit_wc", }),
({ "Now for the ac itself.  How to add..."+
"<name> <type> <num>\n"+
"where name is just a reference name, "+
"type is the protection type, ie sharp, blunt, pierce "+
"and num is of the form 100, 10+100, 10+3d5 "+
"The ac should be out of 100, 100 being the max for the class of armour\n"+
"Armour class : ",
"What attacks don't damage the weapon ? " }),
({ "get_ac", "get_undamaged", }), ({ "do_edit_ac", "edit_undamaged" }),
({ "Attack types the armour does not get damaged from\n"+
"Type : ",
"err", }),
({ "get_immune", "get_bing", }), ({ "edit_immune", 0 }),
});
}

void init() {
::init();
add_action("add", "add");
add_action("edit", "edit");
}


int add(string str) {
if (str != "weapon" && str != "armour") {
notify_fail("Usage: add <armour|weapon>\n");
return 0;
}
weapon = (str == "weapon");
if (weapon)
data_array = allocate(WEAP_A_SIZE + 1);
else
data_array = allocate(ARM_A_SIZE + 1);
in_add = 1;
up_to = -1;
/* will put the new improved method of handling it in here. */
do_next();
return 1;
}

int edit(string str) {
if (str != "weapon" && str != "armour") {
notify_fail("Usage: edit <armour|weapon>\n");
return 0;
}
weapon = (str == "weapon");
write("What id of armour do you wish to edit ? ");
input_to("get_id");
in_add = 0;
data_array = 0;
return 1;
}

int print_menu() {
string menu;
menu =  "a : Id\nb : Object Name\nc : Name\nd : Alias\ne : Plural\n"+
  "f : Adjective\ng : Short\nh : Long\ni : Weight\n"+
  "j : Value\nk : Condidtion\nl : Chance\n";
if (weapon)
menu += "m : Skill\nn : Attacks\no : Undamaging\n";
else
menu += "m : Type\nn : Dex Minus\no : Ac\np : Immune\n";
printf("%#*-s\n", this_player()->query_cols(), menu+"Quit\nSave\nMenu\n");
}

int do_next() {
  up_to ++;
  if (!data_array)
    if (in_add) {
      write("Serious error.\n");
      return 1;
    } else {
      if (weapon)
        data_array = (mixed *)ARMOURY->query_weapon(add_id);
      else
        data_array = (mixed *)ARMOURY->query_armour(add_id);
      if (!data_array) {
        write("Invalid armour/weapon name "+add_id+"\n");
        return 1;
      }
    }
  if (up_to >= sizeof(data_array))
    in_add = 0;
  if (!in_add) {
    print_menu();
    write("Selection : ");
    input_to("do_choice");
    return 1;
  }
  if (!pointerp(desc_array[up_to*3])) {
    write(desc_array[up_to*3]);
    if (data_array[up_to])
      printf("Is currently set to : %O\n", data_array[up_to]);
    input_to(desc_array[up_to*3+1]);
    return 1;
  } else {
    if (data_array[up_to])
      printf("Is currently set to : %O\n", data_array[up_to]);
    write(desc_array[up_to*3][weapon]);
    input_to(desc_array[up_to*3+1][weapon]);
    return 1;
  }
}

int do_choice(string str) {
  int i;

  str = lower_case(str);
  if (str == "menu") {
    print_menu();
    write("Selection : ");
    input_to("do_choice");
    return 1;
  }
  if (str == "quit") {
    write("Quiting.\n");
    return 1;
  }
  if (str == "save") {
    write("Saving.\n");
    if (weapon)
      ARMOURY->add_weapon(add_id, data_array);
    else
      ARMOURY->add_armour(add_id, data_array);
    return 1;
  }
  if (str[0] < 'a' || str[0] > 'p' || (weapon && str[0] > 'o')) {
    write("Invalid choice, try agian. (Menu to redraw menu)\n");
    write("Selection : ");
    input_to("do_choice");
    return 1;
  }
  i = str[0] - 'a';
  if (pointerp(desc_array[i*3])) {
    if (data_array[i])
      printf("Is currently set to : %O\n", data_array[i]);
    write(desc_array[i*3][weapon]);
    if (!desc_array[i*3+2][weapon])
      input_to(desc_array[i*3+1][weapon]);
    else
      call_other(this_object(), desc_array[i*3+2][weapon]);
  } else {
    if (data_array[i])
      printf("Is currently set to : %O\n", data_array[i]);
    write(desc_array[i*3]);
    if (!desc_array[i*3+2])
      input_to(desc_array[i*3+1]);
    else
      call_other(this_object(), desc_array[i*3+2]);
  }
  return 1;
}

int get_id(string str) {
  CHECK
  add_id = str;
  do_next();
  return 1;
}

int get_name(string str) {
  CHECK
  data_array[W_NAME] = str;
  do_next();
  return 1;
}

int get_object_name(string str) {
  CHECK
  data_array[W_OBJ] = str;
  do_next();
  return 1;
}

int get_short(string str) {
  CHECK
  data_array[W_SHORT] = str;
  do_next();
  return 1;
}

int get_long() {
  if (!data_array[W_LONG])
    data_array[W_LONG] = "";
  seteuid("tmp");
  rm("/tmp/long-"+this_player()->query_name());
  write_file("/tmp/long-"+this_player()->query_name(),
               sprintf("%-=*s", this_player()->query_cols(), data_array[W_LONG]));
  ed("/tmp/long-"+this_player()->query_name(), "end_long");
  return 1;
}

int end_long() {

  data_array[W_LONG] = read_file("/tmp/long-"+this_player()->query_name());
  data_array[W_LONG] = implode(explode(data_array[W_LONG],"\n")," ")+"\n";
  rm("/tmp/long-"+this_player()->query_name());
  seteuid("Room");
  do_next();
  return 1;
}

int get_alias(string str) {
  if (!tmp)
    tmp = ({ });
  if (!str || str == "") {
    data_array[W_ALIAS] = tmp;
    tmp = 0;
    do_next();
    return 1;
  }
  tmp += ({ str });
  write("Alias "+(sizeof(tmp)+1)+" : ");
  input_to("get_alias");
  return 1;
}

int edit_alias(string str) {
  thing_name = "aliases";
  end_command = "end_aliases";
  tmp = data_array[W_ALIAS];
  edit_array();
  return 1;
}

int end_aliases() {
  if (tmp)
    data_array[A_ATYPE] = tmp;
  tmp = 0;
  do_next();
  return 1;
}

int get_plural(string str) {
  if (!tmp)
    tmp = ({ });
  if (!str || str == "") {
    data_array[W_PLURALS] = tmp;
    tmp = 0;
    do_next();
    return 1;
  }
  tmp += ({ str });
  write("Plural "+(sizeof(tmp)+1)+" : ");
  input_to("get_plural");
  return 1;
}

int edit_plural(string str) {
  thing_name = "aliases";
  end_command = "end_plural";
  tmp = data_array[W_PLURALS];
  edit_array();
  return 1;
}

int end_plural() {
  if (tmp)
    data_array[W_PLURALS] = tmp;
  tmp = 0;
  do_next();
  return 1;
}

int get_adjective(string str) {
  if (!tmp)
    tmp = ({ });
  if (!str || str == "") {
    data_array[W_ADJ] = tmp;
    tmp = 0;
    do_next();
    return 1;
  }
  tmp += ({ str });
  write("Adjective "+(sizeof(tmp)+1)+" : ");
  input_to("get_adjective");
  return 1;
}

int edit_adjective(string str) {
  thing_name = "aliases";
  end_command = "end_adjectives";
  tmp = data_array[W_ADJ];
  edit_array();
  return 1;
}

int end_adjectives() {
  if (tmp)
    data_array[W_ADJ] = tmp;
  tmp = 0;
  do_next();
  return 1;
}

int get_value(string str) {
  int num;

  sscanf(str, "%d", num);
  data_array[W_VALUE] = num;
  do_next();
  return 1;
}

int get_weight(string str) {
  CHECK
  sscanf(str, "%d", data_array[W_WEIGHT]);
  do_next();
  return 1;
}

int get_cond(string str) {
  CHECK
  sscanf(str, "%d", data_array[W_COND]);
  do_next();
  return 1;
}

int get_chance(string str) {
  CHECK
  sscanf(str, "%d", data_array[W_DAM_CHANCE]);
  do_next();
  return 1;
}


int get_immune(string str) {
  if (!tmp)
    tmp = ({ });
  if (!str || str == "") {
    data_array[A_IMMUNE] = tmp;
    tmp = 0;
    do_next();
    return 1;
  }
  write("Immune "+sizeof(tmp)+" : ");
  input_to("get_immune");
  return 1;
}

int edit_immune(string str) {
  end_command = "end_immune";
  tmp = data_array[A_IMMUNE];
  edit_array();
  return 1;
}

int end_immune() {
  if (tmp)
    data_array[A_IMMUNE] = tmp;
  tmp = 0;
  do_next();
  return 1;
}

mixed get_val(string str) {
  int fixed, rand, die;

  if (sscanf(str, "%d+%dd%d", fixed, rand, die) == 3)
    return ({ fixed, rand, die });
  if (sscanf(str, "%d+%d", fixed, rand) == 2)
    if (rand)
      return ({ fixed, rand });
    else
      return ({ fixed });
  sscanf(str, "%d", rand);
  return rand;
}

int get_dex_minus(string str) {
  sscanf(str, "%d", data_array[A_DEX_MINUS]);
  do_next();
  return 1;
}

int get_type(string str) {
  CHECK
  data_array[A_ATYPE] = str;
  do_next();
  return 1;
}

int i;

int get_undamaged(string str) {
  if (!tmp)
    tmp = ({ });

  if (str == "") {
    data_array[W_UNDAMAGE] = tmp;
    tmp = 0;
    do_next();
    return 1;
  }
  tmp += ({ str });
  write("Undamaged by "+sizeof(tmp)+" : ");
  input_to("get_undamaged");
  return 1;
 }

int edit_undamaged() {
  thing_name = "undamaged";
  tmp = 0;
  end_command = "end_undamaged";
  edit_array();
  return 1;
}

int end_command() {
  if (tmp)
    data_array[W_UNDAMAGE] = tmp;
  tmp = 0;
  do_next();
  return 1;
}

int get_ac(string str) {
  string s1, s2, s3;
  mixed frog;
  mixed ac;

  if (!tmp)
    tmp = ({ });
  if (!str || str == "") {
    data_array[A_ARMOURS] = tmp;
    if (!sizeof(tmp))
      write("Warning must have an ac...(Well.. not must, but it might "+
            "be useful.\n");
    i = 0;
    if (!i)
      do_next();
    else
      return edit_ac();
    tmp = 0;
    return 1;
  }
  if (sscanf(str, "%s %s %s", s1, s2, s3) != 3 || s1 == "" || s2 == "") {
    write("You have entered incorrect data.\n");
    write("Ac : ");
    input_to("get_ac");
    return 1;
  }
  if (!(frog = get_val(s3))) {
    write("You have entered incorrect ac.\n");
    write("Ac : ");
    input_to("get_ac");
    return 1;
  }
  tmp += ({ s1, ({ s2, frog }) });
  write("Ac "+(sizeof(tmp)/2+1)+" : ");
  input_to("get_ac");
  return 1;
}

/* sigh... I am going to really hate this! */
int edit_ac() {
  int i;
  write("\n");

  for (i=0;i<sizeof(tmp);i+=2)
    printf("%2d Name: %-15s Type: %-15s Ac: %s\n", i/2, tmp[i],
              tmp[i+1][0], "/std/armour_logic"->calc_string(tmp[i+1][1]));
  write("Do you want to (A)dd, (R)emove or (C)hange an armour class,\n"+
        "(S)ave or (Q)uit. ? \n");
  input_to("edit_ac_main");
  return 1;
}
int do_edit_ac() {
  tmp = data_array[A_ARMOURS];
  edit_ac();
  return 1;
}


int edit_ac_rem() {
  write("Which ac to delete (string or number) ? ");
  input_to("edit_ac_rem2");
  return 1;
}

int edit_ac_rem2(string str) {
  int i;

  if (!str || str == "")
    return edit_ac();
  if (sscanf(str, "%d", i) == 1)
    if (i>=0 && i*2 < sizeof(tmp)) {
      write("Deleted "+tmp[i*2]+"\n");
      tmp = delete(tmp, i*2, 2);
    } else
      write("Invalid Index.\n");
  else
    if ((i=member_array(str, tmp)) == -1)
      write(str+" not in ac array.\n");
    else {
      write("Deleted "+tmp[i*2]+"\n");
      tmp = delete(tmp, i, 2);
    }
  edit_ac_rem();
  return 1;
}

int edit_ac_change() {
  write("(This changes the ac not the name...)\n"+
        "Which ac to change (number or string) ? ");
  input_to("edit_ac_change2");
  return 1;
}

int edit_ac_change2(string str) {
  if (!str || str == "")
    return edit_ac();
  if (sscanf(str, "%d", i) == 1)
    if (i>=0 && i*2 < sizeof(tmp)) {
      i *= 2;
      write("Ok changeing "+tmp[i]+" to what (<type> <ac>) ? ");
      input_to("edit_ac_change3");
      return 1;
    } else
     write("Illegal index.\n");
  else
    if ((i = member_array(str, tmp)) == -1)
      write(str+" is not in the ac list.\n");
    else {
      write("Ok changeing "+tmp[i*2]+" to what ? ");
      input_to("edit_ac_change3");
      return 1;
    }
  tmp = 0;
  edit_ac_rem();
  return 1;
}

int edit_ac_change3(string str) {
  string s1,s2;
  mixed frog;

  if (sscanf(str, "%s %s", s1, s2) != 2 || s1 == "") {
     write("Invalid data.\n");
    edit_ac_change();
    return 1;
  }
  if (!(frog = get_val(s2))) {
    write("Invalid data.\n");
    return edit_ac_change();
  }
  write("Ok changed "+tmp[i]+"\n");
  tmp[i+1] = ({ s1, frog });
  edit_ac_change();
  return 1;
}

int edit_ac_main(string str) {
  str = lower_case(str);
  if (str == "")
    return do_next();
  switch(str[0]) {
    case 'a' :
      i = 1;
      get_ac("flurgle");
      return 1;
    case 'r' :
      edit_ac_rem();
      return 1;
    case 'c' :
      edit_ac_change();
      return 1;
    case 'q' :
      tmp = 0;
      return do_next();
    case 's' :
      data_array[A_ARMOURS] = tmp;
      tmp = 0;
      return do_next();
  }
  edit_ac();
}

int edit_array() {
  int i;

  for (i=0;i<sizeof(tmp);i++)
    printf("%2d %s\n", i, tmp[i]);
  write("Do you want to (A)dd, (R)emove, (C)hange, (Q)uit or (S)ave ? ");
  input_to("get_thingy");
  return 1;
}

int do_add_thing() {
  write("What "+thing_name+" do you wish to add ? ");
  input_to("get_add_thing");
  return 1;
}

int get_add_thing(string str) {
  if (!str || str == "")
    return edit_array();
  tmp += ({ str });
  write("Ok added "+str+" to "+thing_name+" list.\n");
  do_add_thing();
  return 1;
}

int do_remove_thing() {
  write("Which "+thing_name+" do you wish to remove (number or string) ? ");
  input_to("get_remove_thing");
  return 1;
}

int get_remove_thing(string str) {
  int i;

  if (!str || str == "")
    return edit_array();
  if (sscanf(str, "%d", i) == 1)
    if (i>=0 && i<sizeof(tmp)) {
      write("Removeing "+tmp[i]+"\n");
      tmp = delete(tmp,i,1);
    } else
      write("Invalid array index. (PS this in not a LPC error)\n");
  else
    if ((i=member_array(str,tmp)) == -1)
      write(str+" not a member of "+thing_name+" list.\n");
    else {
      write("Removeing "+tmp[i]+"\n");
      tmp = delete(tmp, i, 1);
    }
  do_remove_thing();
  return 1;
}

int do_change_thing() {
  write("Which "+thing_name+" do you wish to change ? ");
  input_to("get_change_thing1");
  return 1;
}

int get_change_thing1(string str) {
  if (!str || str == "")
    return edit_array();
  if (sscanf(str, "%d", i) == 1)
    if (i>=0 && i < sizeof(tmp)) {
      write("Ok changeing "+tmp[i]+" what do you want to change it too ? ");
      input_to("get_change_thing2");
      return 1;
    } else
      write("Invalid Index, (Frog?)\n");
  else
    if ((i=member_array(str, tmp)) == -1)
      write(str+" is not a member of "+thing_name+" list.\n");
    else {
      write("Ok changeing "+tmp[i]+" what do you want to change it too ? ");
      input_to("get_change_thing2");
      return 1;
    }
  do_change_thing();
  return 1;
}

int get_change_thing2(string str) {
  if (!str || str == "")
    return do_change_thing();
  write("Ok changed "+tmp[i]+" to "+str+".\n");
  tmp[i] = str;
  do_change_thing();
  i = 0;
  return 1;
}

int get_thingy(string str) {
  str = lower_case(str);
  if (str != "")
    switch(str[0]) {
      case 'a' :
        do_add_thing();
        return 1;
      case 'r' :
        do_remove_thing();
        return 1;
     case 'c' :
       do_change_thing();
       return 1;
     case 'q' :
       tmp = 0;
       call_other(this_object(), end_command);
       return 1;
    case 's' :
      call_other(this_object(), end_command);
      return 1;
   }
  write("Ivalid selection\n");
  edit_array();
  return 1;
}

/* ok and here goes the weapon specific stuff.
 * I think I am going to be sick... hang on a tic.
 */

int get_skill(string str) {
  CHECK

  data_array[W_SKILL] = str;
  do_next();
  return 1;
}

int get_wc(string str) {
  if (!tmp)
    tmp = ({ });

  if (!str || str == "") {
    write("Ok finished adding weapon classes.\n");
    if (!sizeof(tmp))
      write("WARNING: It might be an idea to add an attack.\n");
    data_array[W_ATTACKS] = tmp;
    if (in_add == 2) {
      in_add = 0;
      do_edit_wc();
      return 1;
    }
    tmp = 0;
    do_next();
    return 1;
  }
  i = sizeof(tmp);
  tmp += ({ str, allocate(W_ARRAY_SIZE) });
  write("What is the flag value (for a desc of this read the docs) ? ");
  input_to("get_wc_flag");
  return 1;
}

int get_wc_flag(string str) {
  int j;
  if (sscanf(str, "%d", j) || str == "")
    tmp[i+1][W_FLAG] = j;
  else
    tmp[i+1][W_FLAG] = str;
  write("Ok chance of the attack ? ");
  input_to("get_wc_chance");
  return 1;
}

int get_wc_chance(string str) {
  int j;

  sscanf(str, "%d", j);
  tmp[i+1][W_CHANCE] = j;
  write("What type of damage does it do (ie blunt/sharp/pierce) ? ");
  input_to("get_wc_type");
  return 1;
}

int get_wc_type(string str) {
  tmp[i+1][W_TYPE] = str;
  write("Ok now for the base value (this goes on to hit and dam) "+
        "All these next values have the format of a 100, 100+10, "+
        "or 100+2d5.  The number by itself is the random amount "+
        "the number plus a number is a random amount plus that much "+
        "added on (fixed).\nBase value : ");
  input_to("get_wc_base");
  return 1;
}

int get_wc_base(string str) {
  mixed frog;

  if (!(frog = get_val(str))) {
    write("Invalid data... Try again.\n");
    write("Base value : ");
    input_to("get_wc_base");
    return 1;
  }
  tmp[i+1][W_BASE] = frog;
  write("To hit : ");
  input_to("get_wc_hit");
}

int get_wc_hit(string str) {
  mixed frog;

  if (!(frog = get_val(str))) {
    write("Invalid data... Try again.\n");
    write("To hit : ");
    input_to("get_wc_hit");
    return 1;
  }
  tmp[i+1][W_HIT] = frog;
  write("Damage : ");
  input_to("get_wc_dam");
}

int get_wc_dam(string str) {
  mixed frog;

  if (!(frog = get_val(str))) {
    write("Invalid data... Try again.\n");
    write("Damage : ");
    input_to("get_wc_dam");
    return 1;
  }
  tmp[i+1][W_DAM] = frog;
  write("Ok, added that attack.\n");
  write("Attack "+sizeof(tmp)+" : ");
  i = 0;
  input_to("get_wc");
}

int edit_wc() {
  write("\n");
  tmp = data_array[W_ATTACKS];
  do_edit_wc();
}

int do_edit_wc() {
  int i;

  for (i=0;i<sizeof(tmp);i+=2)
    printf("%2d %15s %15s %s %s %s\n", i/2, tmp[i], tmp[i+1][W_TYPE],
           "/std/weapon_logic"->calc_string(tmp[i+1][W_BASE]),
           "/std/weapon_logic"->calc_string(tmp[i+1][W_HIT]),
           "/std/weapon_logic"->calc_string(tmp[i+1][W_DAM]));
  write("Do you want to (A)dd, (R)emove, (C)hange, (Q)uit or (S)ave ? ");
  input_to("do_wc_choice");
  return 1;
}

int do_wc_choice(string str) {
  str = lower_case(str);
  if (str == "")
    return do_edit_wc();
  switch (str[0]) {
    case 'a' :
      in_add = 2;
      write("What name do you want the attack to be ? ");
      input_to("get_wc");
      return 1;
    case 'r' :
      do_wc_remove();
      return 1;
    case 'c' :
      do_wc_change();
      return 1;
    case 'q' :
      tmp = 0;
      do_next();
      return 1;
    case 's' :
      data_array[W_ATTACKS] = tmp;
      tmp = 0;
      do_next();
      return 1;
   }
   write("Invalid choice.\n");
   do_edit_wc();
}

int do_wc_remove() {
  write("Which attack do you want to remove (number or string) ? ");
  input_to("do_wc_remove2");
  return 1;
}

int do_wc_remove2(string str) {
  int i;

  if (str == "")
    return do_edit_wc();
  if (sscanf(str, "%d", i) == 1)
    if (i>=0 && i*2 <sizeof(tmp)) {
      write("Deleteing "+tmp[i]+".\n");
      tmp = delete(tmp, i*2, 2);
    } else
      write("Invalid index.\n");
  else
    if ((i = member_array(str, tmp)) == -1)
      write("Warning "+str+" not a current attack.\n");
    else {
      write("Deleteing "+tmp[i]+".\n");
      tmp = delete(tmp, i, 2);
    }
  do_wc_remove();
}

int do_wc_change() {
  write("Which attack do you wish to change (num or string) ? ");
  input_to("do_wc_change1");
  return 1;
}

int do_wc_change2();

int do_wc_change1(string str) {
  if (str == "")
    return do_edit_wc();
  if (sscanf(str, "%d", i) == 1)
    if (i >= 0 && i*2<sizeof(tmp)) {
      i *= 2;
      do_wc_change2();
      return 1;
    } else
     write("Illegal Index.\n");
  else
    if ((i = member_array(str, tmp)) != -1) {
      do_wc_change2();
      return 1;
    } else
      write("Warning "+str+" is not an attack.\n");
  i = 0;
  do_wc_change();
  return 1;
}

int do_wc_change2() {
  printf("Name: %s\nType: %s\nBase: %s Hit %s Dam %s\n", tmp[i],
         tmp[i+1][W_TYPE],
         "/std/weapon_logic"->calc_string(tmp[i+1][W_BASE]),
         "/std/weapon_logic"->calc_string(tmp[i+1][W_HIT]),
         "/std/weapon_logic"->calc_string(tmp[i+1][W_DAM]));
  write("Do you wish to change (N)ame, (T)ype, (B)ase, (H)it, (D)am or "+
        "(Q)uit/(S)ave ? ");
  input_to("do_wc_change3");
  return 1;
}

int do_wc_change3(string str) {
  if (!str)
    return do_wc_change2();
  str = lower_case(str);
  switch (str[0]) {
    case 'n' :
      write("What is the new name ? ");
      input_to("do_wc_change_name");
      return 1;
    case 't' :
      write("What is the new type ? ");
      input_to("do_wc_change_type");
      return 1;
    case 'b' :
      write("What is the new base ? ");
      input_to("do_wc_change_base");
      return 1;
    case 'h' :
      write("What is the new to hit ? ");
      input_to("do_wc_change_hit");
      return 1;
    case 'd' :
      write("What is the new damage value ? ");
      input_to("do_wc_change_dam");
      return 1;
    case 'q' :
    case 's' :
      return do_wc_change();
  }
  write("Invalid entry.\n");
  do_wc_change2();
  return 1;
}

int do_wc_change_name(string str) {
  if (str == "") {
    write("Hmm interesting name.\n");
    return do_wc_change2();
  }
  write("Changed name from "+tmp[i]+" to "+str+".\n");
  tmp[i] = str;
  do_wc_change2();
  return 1;
}

int do_wc_change_type(string str) {
  if (str == "") {
    write("Hmm interesting type.\n");
    return do_wc_change2();
  }
  write("Changed type from "+tmp[i]+" to "+str+".\n");
  tmp[i+1][W_TYPE] = str;
  do_wc_change2();
  return 1;
}

int do_wc_change_hit(string str) {
  mixed frog;

  if (!(frog = get_val(str))) {
    write("Invalid data.\n");
    return do_wc_change2();
  }
  printf("Ok changeing %s to %s.\n",
         "/std/weapon_logic"->calc_string(tmp[i+1][W_HIT]),
         "/std/weapon_logic"->calc_string(frog));
  tmp[i+1][W_HIT] = frog;
  do_wc_change2();
  return 1;
}

int do_wc_change_dam(string str) {
  mixed frog;

  if (!(frog = get_val(str))) {
    write("Invalid data.\n");
    return do_wc_change2();
  }
  printf("Ok changeing %s to %s.\n",
         "/std/weapon_logic"->calc_string(tmp[i+1][W_DAM]),
         "/std/weapon_logic"->calc_string(frog));
  tmp[i+1][W_DAM] = frog;
  do_wc_change2();
  return 1;
}

int do_wc_change_base(string str) {
  mixed frog;

  if (!(frog = get_val(str))) {
    write("Invalid data.\n");
    return do_wc_change2();
  }
  printf("Ok changeing %s to %s.\n",
         "/std/weapon_logic"->calc_string(tmp[i+1][W_BASE]),
         "/std/weapon_logic"->calc_string(frog));
  tmp[i+1][W_BASE] = frog;
  do_wc_change2();
  return 1;
}
