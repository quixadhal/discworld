inherit "std/room";
#define AUTOMATIC 1
 
#include "pub.h"
#include "money.h"
#include "drinks.h"
 
mapping menu_items, menu_aliases;
string menu_header;
int display_headings;
object menu_object;
 
void create() {
  menu_items = ([ ]);
  menu_aliases = ([ ]);
  set_short("Pub");
  set_long(
"You are in a pub.  You can buy drinks here.\n"+
"There is a menu here that you can read.\n");
  set_light(60);
  menu_header = "The menu reads:";
  display_headings = 1;
  ::create();
  menu_object = clone_object("/obj/misc/menu");
  menu_object->set_pub(this_object());
  hidden_objects += ({ menu_object });
}
 
void init() {
  ::init();
  add_action("buy", "buy");
  add_action("order", "order");
}
 
void add_menu_item(string name, string type, int cost, int heal, int volume,
                   int intox, string drmess, string othmess) {
  int num_type;

  menu_items[name] = ({ type, cost, heal, volume, intox,
                              drmess, othmess });
}
 
void add_menu_items(mixed *drinks) {
  int loop;
 
  for(loop=0; loop<sizeof(drinks); loop++) {
    if(sizeof(drinks[loop]) == 5) {
      mixed *dr;
 
      dr = drinks[loop];
      add_menu_item(dr[0], dr[1], dr[2], dr[3], dr[4], dr[5], dr[6], dr[7]);
    }
  }
}
 
mapping query_menu_items() {
  return menu_items;
}
 
int remove_menu_item(string name) {
  int temp;

  if (!menu_items[name])
    return 0;
  menu_items = m_delete(menu_items, name); 
  return 1;
}
 
string string_menu(string *items) {
  int loop;
  string str;

  str = ""; 
  for(loop = 0; loop < sizeof(items); loop++)
    str += sprintf("    %-20s %s\n", items[loop],
             MONEY_HAND->money_value_string(menu_items[items[loop]][DR_COST]));
  return str;
}
 
void cost_sort(string *list) {
  int loop1, loop2;
  mixed *temp;
  string temp1;
 
  for(loop1 = 0; loop1 < sizeof(list) - 1; loop1++) {
    for(loop2 = 0; loop2 < loop1 - 1; loop2++) {
      if(menu_items[list[loop2]][DR_COST] >
         menu_items[list[loop2+1]][DR_COST]) {
        temp = menu_items[list[loop2]];
        menu_items[list[loop2]] = menu_items[list[loop2+1]];
        menu_items[list[loop2+1]] = temp;
        temp1 = list[loop2];
        list[loop2] = list[loop2+1];
        list[loop2+1] = temp1;
      }
    }
  }
}
 
string read() {
  int loop, temp;
  string *fpos;
  string str, ret, *stuff;
 
  fpos = ({ });
  ret = "\n" + menu_header + "\n";
  stuff = m_indices(menu_items);
  for(loop=0;loop<sizeof(stuff);loop++) {
    if(menu_items[stuff[loop]][DR_TYPE] == FOOD) {
      fpos += ({ stuff[loop] });
    }
  }
  if(sizeof(fpos) > 0) {
    if(display_headings) {
      str = "Meals\n";
    }
    cost_sort(fpos);
    str += string_menu(fpos);
    ret += sprintf("%-=*s\n", (int)this_player()->query_cols(), str);
  }
  fpos = ({ });
  for(loop=0;loop<sizeof(stuff);loop++) {
    if(menu_items[stuff[loop]][DR_TYPE] == SOFTDRINK) {
      fpos += ({stuff[loop]});
    }
  }
  if(sizeof(fpos) > 0) {
    if(display_headings) {
      str = "Drinks\n";
    }
    cost_sort(fpos);
    str += string_menu(fpos);
    ret += sprintf("%-=*s\n", (int)this_player()->query_cols(), str);
  }
  fpos = ({ });
  for(loop=0;loop<sizeof(stuff);loop++) {
    if(menu_items[stuff[loop]][DR_TYPE] == ALCOHOL) {
      fpos += ({stuff[loop]});
    }
  }
  if(sizeof(fpos) > 0) {
    if(display_headings) {
      str = "Alcoholic Beverages\n";
    }
    cost_sort(fpos);
    str += string_menu(fpos);
    ret += sprintf("%-=*s\n", (int)this_player()->query_cols(), str);
  }
  return ret;
}
 
void set_menu_header(string header) {
  menu_header = header;
}
 
string query_menu_header() {
  return menu_header;
}
 
void set_display_headings(int value) {
  display_headings = value;
}
 
int query_display_headings() {
  return display_headings;
}
 
void add_menu_alias(string alias, string actual) {
  menu_aliases[alias] = actual;
}
 
mapping query_menu_aliases() {
  return menu_aliases;
}
 
int remove_menu_alias(string alias) {
  menu_aliases = m_delete(menu_aliases, alias);
}
 
int buy(string str) {
  int temp, volume, intox, heal, error, type;
  mixed *cost;
  string drmess, othmes;
  object food;

  if (menu_aliases[str])
    str = menu_aliases[str];
  if(!menu_items[str]) {
    notify_fail("Sorry, "+str+" is not on the menu.\n");
    return 0;
  }
  heal = menu_items[str][DR_HEAL];
  cost = menu_items[str][DR_COST];
  switch (menu_items[str][DR_TYPE]) {
    case FOOD : type = D_FOOD;
                break;
    case ALCOHOL : type =D_ALCOHOL;
                break;
    case SOFTDRINK : type = D_FOOD;
                break;
  }
  volume = menu_items[str][DR_VOLUME];
  intox = menu_items[str][DR_INTOX];
  drmess = menu_items[str][DR_DRMESS];
  othmes = menu_items[str][DR_OTHMES];
  if(this_player()->query_value() < menu_items[str][DR_COST]) {
    notify_fail("You are too poor to afford that.\n");
    return 0;
  }
/*
  if(this_player()->query_free_volume(type) < volume) {
    write("You are full.\n");
/*    this_player()->full_mess(DR_TYPE); */
/*    return 1;
  }
  if(this_player()->query_free_intox() < intox) {
    write("You are sooo drunk that you cant put the drink to your mouth.\n");
 */
/*    this_player()->intox_mess(); */
/*
    return 1;
  }
 */
#ifdef AUTOMATIC
  this_player()->pay_money(({ "brass", cost }));
  this_player()->adjust_volume(type, volume);
  this_player()->adjust_hp(heal);
  this_player()->adjust_intox(intox);
  write("You "+drmess+".\n");
  say(this_player()->query_cap_name()+" "+othmes+".\n");
#else
  food = clone_object("obj/food");
  food->set_type(type);
  food->set_name(str);
  food->set_short(str);
  food->set_heal(heal);
  food->set_intox(intox);
  food->set_volume(volume);
  food->set_weight(5);
  food->set_consume_write(drmess);
  food->set_consume_say(othmes);
  error = (int)food->move(this_player());
  if(error) {
    write("You cannot carry that much.\n");
    food->dest_me();
    return 1;
  }
  this_player()->pay_money(cost);
#endif
  return 1;
}

void dest_me() {
  if (menu_object)
    menu_object->dest_me();
  ::dest_me();
}

void event_fight_in_progress() {
  mixed *dest;
object ob;
  int i;

  dest = query_dest_dir();
  for (i=1;i<sizeof(dest);i+=2)
    if (objectp(dest[i]))
      event(dest[i], "pub_brawl", this_object());
    else
      if(ob=find_object(dest[i]))
        event(ob,"pub_brawl", this_object());
}
