/*
 * My new and wonderous new room.c
 * It will never work as the frog says to the chicken
 */
inherit "/std/basic/light";
inherit "/std/basic/property";
inherit "/std/basic/cute_look";
inherit "/std/basic/extra_look";
inherit "/std/basic/desc";
#include "room.h"
 
#define ROOM_HAND "/obj/handlers/room_handler"
#define STD_ORDERS ({\
"north", ({ -1, 0, 0 }), "south", ({ 1, 0, 0 }), "east", ({ 0, -1, 0 }),\
"west", ({ 0, 1, 0 }), "up", ({ 0, 0, -1 }), "down", ({ 0, 0, 1 }),\
"northeast", ({ -1, -1, 0 }), "northwest", ({ -1, 1, 0 }),\
"southwest", ({ 1, 1, 0 }), "southeast", ({ 1, -1, 0 }), })

static mapping items;
static string exit_string,
       room_zone,
       dark_mess,
       *dest_direc,
       *aliases;
static mixed  *dest_other;
static int *co_ord;
static object *destables, *tempory_bits;
 
int test_add(object ob, int flag) { return 1; }
int test_remove(object ob, int flag) { return 1; }
int add_weight() { return 1; }
 
int *query_co_ord() { return co_ord; }
void set_co_ord(int *new) { co_ord = new; }
 
string query_dark_mess() { return dark_mess; }
void set_dark_mess(string str) { dark_mess = str; }
 
void create() {
  dest_other = ({ });
  dest_direc = ({ });
  items = ([ ]);
  aliases = ({ });
  destables = ({ });
  tempory_bits = ({ });
  room_zone = "nowhere";
  seteuid((string)"/secure/master"->creator_file(file_name(this_object())));
  set_dark_mess("It is too dark to see");
  property::create();
  extra_look::create();
  add_property("location", "inside");
  this_object()->setup();
  this_object()->reset();
}
 
string expand_alias(string str);

string short(int dark) {
  if (dark)
    return query_dark_mess();
  return ::short();
}
 
int id(string str) {
  int i;

  return 0;
  str = expand_alias(str);
  return items[str];
}
 
string expand_alias(string str) {
  int i;
 
  if (!aliases)
    return str;
 
  if ((i=member_array(str,aliases))==-1)
    return str;
 
  if (i%2)
    return aliases[i-1];
  return str;
}
 
/*
 * Yes folks. This creates that really stupid obvious exits message you keep
 * getting. Hope you really hate it.
 * specialy changed to handle stupid things. If you can figure it out I
 * will shake your hand. Look for docs coming soon to a universe near you.
 */
string query_dirs_string() {
  string ret, *dirs;
  int no, i, nostore;
 
  if (!dest_direc || sizeof(dest_direc)==0)
    dest_direc = ({ });
  dirs = ({ });
  for (i=0;i<sizeof(dest_other);i+=2)
    if (dest_other[i+1][ROOM_OBV]) {
      no += 1;
      dirs += ({ dest_other[i] });
    } else if (stringp(dest_other[i+1][ROOM_OBV])) {
      nostore = 1;
      if (call_other(this_object(),dest_other[i+1][ROOM_OBV]))
        dirs += ({ dest_other[i] });
    } else if (pointerp(dest_other[i+1][ROOM_OBV])) {
      nostore = 1;
      if (call_other(dest_other[i+1][ROOM_OBV][0],dest_other[i+1][ROOM_OBV][1]))
        dirs += ({ dest_other[i] });
    }
  if (sizeof(dirs)==0) {
    if (nostore)
      return "%^GREEN%^There are no obvious exits.%^RESET%^";
    exit_string = "%^GREEN%^There are no obvious exits.%^RESET%^";
    return exit_string;
  }
  if (sizeof(dirs)==1) {
    if (nostore)
      return "%^GREEN%^There is one obvious exit: "+dirs[0]+".%^RESET%^";
    exit_string = "%^GREEN%^There is one obvious exit: "+dirs[0]+".%^RESET%^";
    return exit_string;
  }
  ret = " and "+dirs[sizeof(dirs)-1]+".";
  dirs = delete(dirs,sizeof(dirs)-1, 1);
  if (nostore)
    return "%^GREEN%^There are "+query_num(sizeof(dirs)+1, 0)+
           " obvious exits : "+implode(dirs,", ")+ret+"%^RESET%^";
  exit_string = "%^GREEN%^There are "+query_num(sizeof(dirs)+1, 0)+
                " obvious exits : "+implode(dirs,", ")+ret+"%^RESET%^";
  return exit_string;
}
 
string long(string str, int dark) {
  int i;
  string ret,s;
 
  if (dark)
    return query_dark_mess()+".\n";
  if (!str) {
    ret = "";
    ret += query_long();
    s = calc_extra_look();
    if (s && s!="")
      ret += s;
    if (!exit_string)
      s = query_dirs_string();
    else
      s = exit_string;
    ret += s + "\n";
    return ret+query_contents("");
  }
  str = expand_alias(str);
  return items[str];
}
 
void calc_co_ord() {
  int *old_co_ord, i, j, k;
 
  for (i=0;i<sizeof(dest_other) && !co_ord;i+=2)
    if (find_object(dest_other[i+1][ROOM_DEST]))
    if (!catch((old_co_ord = (int *)dest_other[i+1][ROOM_DEST]->query_co_ord())))
      if (old_co_ord) {
        co_ord = old_co_ord + ({ });
        if ((j=member_array(dest_other[i], STD_ORDERS)) != -1)
          for (k=0;k<3;k++)
            co_ord[k] += STD_ORDERS[j+1][k];
      }
}
 
void init() {
  int i, j;
  mapping done;

  if (!dest_direc)
    return 0;

  done = ([ ]); 
  for (i=0;i<sizeof(dest_direc);i++) {
    if (!done[dest_direc[i]])
      add_action("do_exit_command", dest_direc[i]);
    done[dest_direc[i]] = 1;
    if ((j=member_array(dest_direc[i], aliases)) != -1) {
      string *al;
      al = aliases;
      
      do {
        if (!(j%2)) {
         if (!done[al[j+1]]) {
          add_action("do_exit_command", al[j+1]);
          done[al[j+1]] = 1;
         }
        } else
          j++;
        al = al[j+2..sizeof(al)];
      } while ((j=member_array(dest_direc[i], al)) != -1);
    }
  }
  add_action("do_search", "search");
  if (!pointerp(co_ord))
    calc_co_ord();
  for (i=0;i<sizeof(tempory_bits);i++)
    if (tempory_bits[i])
      tempory_bits[i]->init();
    else {
      tempory_bits = delete(tempory_bits, i, 1);
      i--;
    }
}
 
/*
 * not particularly useful.
 * But here just the same, great for debugging purposes.
 */
string *query_direc() { return dest_direc; }
 
/*
 * It IS useful to get directions+where they go to
 * BTW this is not a frog.
 */
mixed *query_dest_dir() {
  int i;
  string *retval;
 
  retval = ({ });
  for (i=0;i<sizeof(dest_other);i+=2)
    retval += ({ dest_other[i], dest_other[i+1][ROOM_DEST] });
  return  retval;
}
 
mixed *query_dest_other() { return dest_other; }
 
void set_zone(string str) {
  room_zone = str;
}
 
string query_zone() {
  return room_zone;
}

/*
 * this function puts the directions into the thingy list
 * I am sure you know what I mean 
 */
string expand_direc(string str) {
  string s1,s2;
 
  if (sscanf(str,"%s %s",s1,s2)==2)
    return s1;
  return str;
}
 
/* ok.... I am going to rewrite the add_exit, modify_exit and
 * remove_exit code.... I hope you dont mind.
 */
int add_exit(string direc, mixed dest, string type) {
  mixed *stuff;
 
  if (!dest_other)
    dest_other = ({ });
 
  if (member_array(direc, dest_other) != -1)
    return 0;
 
/* if the type fails... it will return a standard exit style */
  stuff = ({ dest }) + ROOM_HAND->query_exit_type(type);
  dest_other += ({ direc, stuff });
  dest_direc += ({ expand_direc(direc) });
  exit_string = 0;
       /* need that to cache the new exit message */
  return 1;
}
 
int modify_exit(string direc, mixed *data) {
  int i, j;
  if ((i=member_array(direc, dest_other)) == -1)
    return 0;
 
  for (j=0;j<sizeof(data);j+=2)
    switch (data[j]) {
      case "message" :
        dest_other[i+1][ROOM_MESS] = data[j+1];
        break;
      case "obvious" :
        dest_other[i+1][ROOM_OBV] = data[j+1];
        break;
      case "function" :
        dest_other[i+1][ROOM_FUNC] = data[j+1];
        break;
      case "size" :
        dest_other[i+1][ROOM_SIZE] = data[i+1];
        break;
    }
  return 1;
}
 
int remove_exit(string direc) {
  int i;
  if (!dest_other)
    dest_other = ({ });
  if ((i=member_array(direc, dest_other)) == -1)
    return 0;
  dest_other = delete(dest_other, i, 2);
  dest_direc = delete(dest_direc, i/2, 1);
  exit_string = 0;
  return 1;
}

int query_exit(string direc) {
  return (member_array(direc, dest_other) != -1);
}

int query_size(string direc) {
  int i;
 
  if ((i=member_array(direc, dest_other))==-1)
    return 0;
  if (stringp(dest_other[i+1][ROOM_SIZE]))
    return (int)call_other(this_object(), dest_other[i+1][ROOM_SIZE]);
  if (pointerp(dest_other[i+1][ROOM_SIZE]))
    return (int)call_other(dest_other[i+1][ROOM_SIZE][0],
                      dest_other[i+1][ROOM_SIZE][1]);
  return dest_other[i+1][ROOM_SIZE];
}
 
int do_exit_command(string str, string verb, object ob, object foll) {
  int i;
  if (!verb)
    verb = query_verb();
  else
    if (!sscanf(verb, "%s %s", verb, str) !=2)
      str = "";
  if (!ob)
    ob = this_player();
  if ((i=member_array(verb,dest_direc))==-1)
    if ((i=member_array(verb, aliases)) == -1)
      return 0;
    else
      if ((i=member_array(aliases[i-1], dest_direc)) == -1)
        return 0;
 
/* ok must be two command exit */
  if (dest_direc[i] != dest_other[i*2]) {
    string s1,s2;
    mixed zip;
    int j;
 
    sscanf(dest_other[i*2],"%s %s",s1,s2);
    str = expand_alias(str);
    if (s2 != str) {
      zip = dest_direc[i+1..sizeof(dest_direc)];
      while (1)
        if ((j = member_array(verb, zip)) != -1) {
          i += j+1;
          sscanf(dest_other[i*2],"%s %s", s1, s2);
          if (str == s2)
            break;
          zip = zip[j+1..sizeof(zip)];
        } else
          return 0;
    }
  }
  if (dest_other[i*2+1][ROOM_FUNC])
    if (stringp(dest_other[i*2+1][ROOM_FUNC])) {
      if (!call_other(this_object(), dest_other[i*2+1][ROOM_FUNC], str, ob))
        return 0;
    } else {
      if (pointerp(dest_other[i*2+1][ROOM_FUNC]))
        if (!call_other(dest_other[i*2+1][ROOM_FUNC][0],
                        dest_other[i*2+1][ROOM_FUNC][1], ob))
          return 0;
    }
  return (int)ob->move_player(dest_other[i*2], dest_other[i*2+1][ROOM_DEST],
                                   dest_other[i*2+1][ROOM_MESS], foll);
}
 
/* 
 * Ok we have done all the exit junk, now for the item bits and pieces
 * share and enjoy your plastic pal who is fun to be with
 */
int add_item(string str,string desc) {
  object ob;
  int i;

  if (!items)
    items = ([ ]);

  ob = items["The object"];
  if (!ob)
    ob = clone_object(ITEM_OBJECT);
  ob->setup_item(str, desc);
/*
  if (items[str])
    ob = items[str];
  else
    ob = clone_object(ITEM_OBJECT);
  items[str] = ob;
 */
  return 1;
}

/* Hmmm.....  At this point remove_item becomes hard... Ignore for now.
int remove_item(string str) {
  int i;
 
  if (!items)
    items = ([ ]);

  if (items[str])
    items[str]->dest_me(); 
  items = m_delete(items, str);
  return 1;
}
*/ 

mapping query_items() { return items; }

/*
 * The alias junk
 */
int add_alias(mixed name,string str) {
  int i;
 
  if (!aliases)
    aliases = ({ });
 
  if (pointerp(name)) {
    for (i=0;i<sizeof(name);i++)
      add_alias(name[i], str);
    return 1;
  }
 
  aliases += ({ str, name });
  return 1;
}
 
int modify_alias(string str,string name) {
  int i;
 
  if (!aliases)
    return 0;
 
  if ((i=member_array(str, aliases))==-1 || !(i%2))
    return 0;
 
  aliases[i+1] = name;
  return 1;
}
 
int remove_alias(string str) {
  int i;
 
  if (!aliases)
    return 0;
 
  if ((i=member_array(str, aliases))==-1 || !(i%2))
    return 0;
 
  aliases = delete(aliases, i, 2);
  return 1;
}
 
int query_no_writing() { return 1; }
 
void dest_me() {
  int i;

  if (!destables)
    destables = ({ });
  if (mappingp(items))
    destables += m_values(items);
  for (i=0;i<sizeof(destables);i++)
    if (destables[i])
      destables[i]->dest_me();
  destruct(this_object());
}
 
void clean_up() {
  mixed *arr;
 
  arr = deep_inventory(this_object());
  if (!sizeof(arr))
    destruct(this_object());
}

object *find_inv_match(string str) {
  return all_inventory(this_object()) + tempory_bits + m_values(items);
}

int do_search(string str) {
  write( ({
"You search around for a while but dont find anything.\n",
"You scrounge around, the ground does look interesting you decide.\n",
"You look carefully at everything, but you find nothing.\n",
"After a while of intensive searching you find nothing.\n" })[random(3)]);
  say(this_player()->query_cap_name()+" searches around the room a bit.\n");
  return 1;
}

/* Cute, usefull add_sign. */
/*
 * add_sign(string long, string read_mess)
 *
 * This file can be inherited and will return a sign that can be used
 * by a room or object in any way it sees fit.  This function was the
 * brainchild of Wyrm - 7 Feb '92
 * If the short is left out.. The sign isnt moved into the room,
 * but it can still be look at and read.
 */

varargs object add_sign (string long, string mess, string short) {
  object sign;

  sign = clone_object("/std/object");
  sign-> set_name("sign");
  if (!short)
    sign-> set_short("sign");
  else
    sign-> set_short(short);
  sign-> set_long(long);
  sign-> set_read_mess(mess);
  sign-> set_main_plural("signs");
  sign-> reset_get();
  if (short)
    sign-> move(this_object());
  destables += ({ sign });
  tempory_bits += ({ sign });
  return sign;
}

mixed stats() {
  mixed *exits, *tmp;
  int i;

  exits = ({ });
  for (i=0;i<sizeof(dest_other);i+=2)
    exits += ({ ({ "Direction", dest_other[i], }),
                ({ "Destination", dest_other[i+1][ROOM_DEST] }) });
  if (co_ord)
    tmp = ({ ({ "co-ord x", co_ord[0] }),
             ({ "co-ord y", co_ord[1] }),
             ({ "co-ord z", co_ord[2] }), });
  else
    tmp = ({ });
  return ({ ({ "short", short(0), }),
            ({ "location", query_property("location"), }),
            ({ "move zone", room_zone }),
         }) + tmp + exits + ::stats();
}

int query_decay() { return 10; }
