/*
 * My new and wonderous new room.c
 * It will never work as the frog says to the chicken
* Modified by Godot on 3/6/93 to fix punctuation in do_search message.
 */
inherit "/std/basic/light";
inherit "/std/basic/property";
inherit "/std/basic/cute_look";
inherit "/std/basic/extra_look";
inherit "/std/basic/desc";
#include "room.h"
#include "door.h"
 
#define ROOM_HAND "/obj/handlers/room_handler"
#define STD_ORDERS ({\
"north", ({ -1, 0, 0 }), "south", ({ 1, 0, 0 }), "east", ({ 0, -1, 0 }),\
"west", ({ 0, 1, 0 }), "up", ({ 0, 0, -1 }), "down", ({ 0, 0, 1 }),\
"northeast", ({ -1, -1, 0 }), "northwest", ({ -1, 1, 0 }),\
"southwest", ({ 1, 1, 0 }), "southeast", ({ 1, -1, 0 }), })
#define SHORTEN ([\
"north":"n", "south":"s", "west":"w", "east":"e", "northeast":"ne",\
"northwest":"nw", "southeast":"se", "southwest":"sw", "up":"u",\
"down":"d" ])

static mapping items,
               door_control;
static string exit_string,
       short_exit_string,
       room_zone,
       dark_mess,
       *dest_direc,
       *aliases;
static mixed  *dest_other;
static int *co_ord, room_size;
static object *destables,
              *hidden_objects;

string query_short_exit_string();
 
int test_add(object ob, int flag) { return 1; }
int test_remove(object ob, int flag) { return 1; }
int add_weight(int n) { return 1; }
 
int *query_co_ord() { return co_ord; }
void set_co_ord(int *new) { co_ord = new; }
 
string query_dark_mess() { return dark_mess; }
void set_dark_mess(string str) { dark_mess = str; }

void set_room_size(int siz) { room_size = siz; }
int query_room_size() { return room_size; }
 
void create() {
  dest_other = ({ });
  dest_direc = ({ });
  items = ([ ]);
  aliases = ({ });
  destables = ({ });
  hidden_objects = ({ });
  door_control = ([ ]);
  room_zone = "nowhere";
  seteuid((string)"/secure/master"->creator_file(file_name(this_object())));
  set_dark_mess("It is too dark to see");
  if (!room_size) room_size = 5;
  property::create();
  extra_look::create();
  add_property("determinate", "");
  add_property("location", "inside");
  this_object()->setup();
  this_object()->reset();
} /* create() */
 
string expand_alias(string str);

string short(int dark) {
  if (dark)
    return ::short(1);
  if(query_verb() == "glance")
    return ::short(0) + query_short_exit_string();
  else
    return ::short(0);
} /* short() */
 
int id(string str) {
  int i;

  return 0;
  str = expand_alias(str);
  return items[str];
} /* id() */
 
string expand_alias(string str) {
  int i;
 
  if (!aliases)
    return str;
 
  if ((i=member_array(str,aliases))==-1)
    return str;
 
  if (i%2)
    return aliases[i-1];
  return str;
} /* expand_alias() */
 
/*
 * Yes folks. This creates that really stupid obvious exits message you keep
 * getting. Hope you really hate it.
 * specialy changed to handle stupid things. If you can figure it out I
 * will shake your hand. Look for docs coming soon to a universe near you.
 */
string query_dirs_string() {
  string ret, *dirs;
  int no, i, nostore;
  mixed tmp;
 
  if (!dest_direc || sizeof(dest_direc)==0)
    dest_direc = ({ });
  dirs = ({ });
  for (i=0;i<sizeof(dest_other);i+=2)
    if (intp(tmp = dest_other[i+1][ROOM_OBV]) && tmp) {
      no += 1;
      dirs += ({ dest_other[i] });
    } else if (stringp(tmp)) {
      nostore = 1;
      if (call_other(this_object(), tmp, dest_other[i]))
        dirs += ({ dest_other[i] });
    } else if (pointerp(tmp)) {
      nostore = 1;
      if (call_other(tmp[0], tmp[1], dest_other[i]))
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
} /* query_dirs_string() */

/*
 * Yes folks. This creates that really stupid obvious exits message you keep
 * getting. Hope you really hate it.
 * specialy changed to handle stupid things. If you can figure it out I
 * will shake your hand. Look for docs coming soon to a universe near you.
 */
string query_short_exit_string() {
  string ret, *dirs;
  int no, i, nostore, add;
  mixed tmp;

  if (short_exit_string) 
    return short_exit_string;
  if (!dest_direc || sizeof(dest_direc)==0)
    dest_direc = ({ });
  dirs = ({ });
  for (i=0;i<sizeof(dest_other);i+=2) {
    add = 0;
    if (intp(tmp = dest_other[i+1][ROOM_OBV]) && tmp) {
      no += 1;
      add = 1;
    } else if (stringp(tmp)) {
      nostore = 1;
      add = (int)call_other(this_object(), tmp, dest_other[i]);
    } else if (pointerp(dest_other[i+1][ROOM_OBV])) {
      nostore = 1;
      add = (int)call_other(tmp[0], tmp[1], dest_other[i]);
    }
    if (!add)
      continue;
    if ((ret=SHORTEN[dest_other[i]]))
      dirs += ({ ret });
    else
      dirs += ({ dest_other[i] });
  }
  if (sizeof(dirs)==0) {
    if (nostore)
      return " %^GREEN%^[none]%^RESET%^";
    return short_exit_string = " %^GREEN%^[none]%^RESET%^";
  }
  if (nostore)
    return " %^GREEN%^["+implode(dirs,",")+"]%^RESET%^";
  return short_exit_string  = " %^GREEN%^["+implode(dirs,",")+"]%^RESET%^";
} /* query_short_exit_string() */
 
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
} /* long() */
 
void calc_co_ord() {
  int *old_co_ord, i, j, k;
 
  for (i=0;i<sizeof(dest_other) && !co_ord;i+=2)
    if (find_object(dest_other[i+1][ROOM_DEST]))
    if (!catch((old_co_ord = (int *)dest_other[i+1][ROOM_DEST]->query_co_ord())))
      if (old_co_ord) {
        co_ord = old_co_ord + ({ });
        if ((j=member_array(dest_other[i], STD_ORDERS)) != -1)
          for (k=0;k<3;k++)
            co_ord[k] += STD_ORDERS[j+1][k]*room_size;
      }
} /* calc_co_ord() */
 
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
  for (i=0;i<sizeof(hidden_objects);i++)
    if (objectp(hidden_objects[i]) && hidden_objects[i])
      hidden_objects[i]->init();
    else {
      hidden_objects = delete(hidden_objects, i, 1);
      i--;
    }
  if (items["The object"])
    items["The object"]->init();
} /* init() */
 
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
} /* query_dest_dir() */
 
mixed *query_dest_other() { return dest_other; }
 
void set_zone(string str) {
  room_zone = str;
} /* set_zone() */
 
string query_zone() {
  return room_zone;
} /* query_zone() */

/*
 * this function puts the directions into the thingy list
 * I am sure you know what I mean 
 */
string expand_direc(string str) {
  string s1,s2;
 
  if (sscanf(str,"%s %s",s1,s2)==2)
    return s1;
  return str;
} /* expand_direc() */
 
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
  stuff = ({ dest }) + ROOM_HAND->query_exit_type(type, direc);
  dest_other += ({ direc, stuff });
  dest_direc += ({ expand_direc(direc) });
  exit_string = 0;
  short_exit_string = 0;
  if ((stuff = (mixed)ROOM_HAND->query_door_type(type, direc, dest))) {
       /* need that to cache the new exit message */
    door_control[direc] = ({ clone_object(DOOR_OBJECT) });
    door_control[direc][0]->setup_door(direc, this_object(), dest, stuff);
    hidden_objects += ({ door_control[direc][0] });
/* We cheat and add the door exit to the door_control array */
    door_control[dest] = direc;
  }
  return 1;
} /* add_exit() */
 
int modify_exit(string direc, mixed *data) {
  int i, j;
  if ((i=member_array(direc, dest_other)) == -1)
    return 0;
 
  for (j=0;j<sizeof(data);j+=2)
    switch (lower_case(data[j])) {
      case "message" :
        dest_other[i+1][ROOM_MESS] = data[j+1];
        break;
      case "obvious" :
        if (dest_other[i+1][ROOM_OBV] != data[j+1]) {
          exit_string = 0; /* State changed... */
          short_exit_string = 0;
        }
        dest_other[i+1][ROOM_OBV] = data[j+1];
        break;
      case "function" :
        dest_other[i+1][ROOM_FUNC] = data[j+1];
        break;
      case "size" :
        dest_other[i+1][ROOM_SIZE] = data[j+1];
        break;
      case "enter" :
        dest_other[i+1][ROOM_ENTER] = data[j+1];
        break;
      case "dest" :
        dest_other[i+1][ROOM_DEST] = data[j+1];
        break;
      case "open" :
        if (door_control[direc])
          door_control[direc][0]->set_open(data[j+1]);
        break;
      case "locked" :
        if (door_control[direc])
          door_control[direc][0]->set_locked(data[j+1]);
        break;
      case "key" :
        if (door_control[direc])
          door_control[direc][0]->set_key_prop(data[j+1]);
        break;
      case "other" :
        if (door_control[direc])
          door_control[direc][0]->set_other_id(data[j+1]);
        break;
      case "difficulty" :
        if (door_control[direc])
          door_control[direc][0]->set_difficulty(data[j+1]);
        break;
      case "open desc" :
        if (door_control[direc])
          door_control[direc][0]->set_open_desc(data[j+1]);
        break;
      case "open/close func" :
        if (door_control[direc])
          door_control[direc][0]->set_open_func(data[j+1]);
        break;
      case "lock/unlock func" :
        if (door_control[direc])
          door_control[direc][0]->set_unlock_func(data[j+1]);
        break;
      case "close desc" :
        if (door_control[direc])
          door_control[direc][0]->set_close_desc(data[j+1]);
        break;
      case "door short" :
        if (door_control[direc])
          door_control[direc][0]->set_short(data[j+1]);
        break;
      case "undoor" :
        if (door_control[direc]) {
          door_control[direc][0]->go_away();
          hidden_objects = hidden_objects - ({ door_control[direc][0] });
        }
        door_control = m_delete(door_control, direc);
        break;
    }
  return 1;
} /* modify_exit() */
 
int remove_exit(string direc) {
  int i;
  if (!dest_other)
    dest_other = ({ });
  if ((i=member_array(direc, dest_other)) == -1)
    return 0;
  dest_other = delete(dest_other, i, 2);
  dest_direc = delete(dest_direc, i/2, 1);
  if (door_control[direc]) {
    door_control[direc][0]->go_away();
    hidden_objects = hidden_objects - ({ door_control[direc][0] });
  }
  door_control = m_delete(door_control, direc);
  short_exit_string = 0;
  exit_string = 0;
  return 1;
} /* remove_exit() */

/* added by ember cause it seemed useful, and I couldn't see any other way of */
/* getting this info ... should it be possible for creators to get */
/* hold of the door_control objects? */
/* oh yeah, it's the 15th-feb-93 */
int query_door_open(string direc) {
   if (!pointerp(door_control[direc])) return 0;
   if (!door_control[direc][0]) return 0;
   return (int)door_control[direc][0]->query_open();
}

int query_exit(string direc) {
  return (member_array(direc, dest_other) != -1);
} /* query_exit() */

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
} /* query_size() */
 
int do_exit_command(string str, mixed verb, object ob, object foll) {
  string special_mess, closed;
  int i;
  mixed zip;

  if (!verb)
    verb = query_verb();
  else {
    if (pointerp(verb)) {
      special_mess = verb[1];
      verb = verb[0];
    }
    if (sscanf(verb, "%s %s", verb, str) !=2)
      str = "";
  }
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
/* First check for lockedness of doors etc */
  if (zip = door_control[dest_other[i*2]]) {
    if (zip[0]->query_locked()) /* Locked...  We auto-unlock, if they have the key */
      if (!zip[0]->moveing_unlock(ob))
        return 0;
    if (!zip[0]->query_open()) { /* Closed open it and close it after us. */
      if (!zip[0]->moveing_open(ob))
        return 0;
      closed = zip[0];
    }
  }
  if (dest_other[i*2+1][ROOM_FUNC])
    if (stringp(dest_other[i*2+1][ROOM_FUNC])) {
      if (!call_other(this_object(), dest_other[i*2+1][ROOM_FUNC], str, ob, special_mess))
        return 0;
    } else {
      if (pointerp(dest_other[i*2+1][ROOM_FUNC]))
        if (!call_other(dest_other[i*2+1][ROOM_FUNC][0],
                        dest_other[i*2+1][ROOM_FUNC][1], ob, special_mess))
          return 0;
    }
  if (!special_mess)
    zip = (int)ob->move_player(dest_other[i*2], dest_other[i*2+1][ROOM_DEST],
                                   dest_other[i*2+1][ROOM_MESS], foll,
                                   dest_other[i*2+1][ROOM_ENTER]);
  else
    zip = (int)ob->move_player(dest_other[i*2], dest_other[i*2+1][ROOM_DEST],
                                   special_mess, foll,
                                   dest_other[i*2+1][ROOM_ENTER]);
  if (closed)
    closed->moveing_close(ob);
  return zip;
} /* do_exit_command() */
 
/* 
 * Ok we have done all the exit junk, now for the item bits and pieces
 * share and enjoy your plastic pal who is fun to be with
 */
varargs int add_item(string str, string desc, int no_plural) {
  object ob;
  int i;

  if (!items)
    items = ([ ]);

  ob = items["The object"];
  if (!ob) {
    ob = clone_object(ITEM_OBJECT);
    items["The object"] = ob;
  }
  ob->setup_item(str, desc, no_plural);
  return 1;
} /* add_item() */

/*
 * Ok.  Remove item is nasty :(  But....  We will add it.
 * It will remove all things that reference the given desc.
 */
int remove_item(string str) {
  object ob;
 
  if (!items)
    items = ([ ]);

  ob = items["The object"];
  if (!ob)
    return 0;
  return (int)ob->remove_item(str);
} /* remove_item() */

/*
 * Change all descs...  It does a match and goes wimble.
 */
int modify_item(string str, string new_desc) {
  object ob;
 
  if (!items)
    items = ([ ]);

  ob = items["The object"];
  if (!ob)
    return 0;
  return (int)ob->modify_item(str, new_desc);
} /* modify_item() */

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
} /* add_alias() */
 
int modify_alias(string str,string name) {
  int i;
 
  if (!aliases)
    return 0;
 
  if ((i=member_array(str, aliases))==-1 || !(i%2))
    return 0;
 
  aliases[i+1] = name;
  return 1;
} /* modify_alias() */
 
int remove_alias(string str) {
  int i;
 
  if (!aliases)
    return 0;
 
  if ((i=member_array(str, aliases))==-1 || !(i%2))
    return 0;
 
  aliases = delete(aliases, i, 2);
  return 1;
} /* remove_alias() */
 
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
} /* dest_me() */
 
void clean_up() {
  mixed *arr;
  int i;
 
  arr = all_inventory(this_object());
  for (i=0;i<sizeof(arr);i++)
    if (interactive(arr[i]))
      return ;
  arr->dest_me();
  dest_me();
} /* clean_up() */

object *find_inv_match(string str) {
  return all_inventory(this_object()) + hidden_objects + m_values(items);
} /* find_inv_match() */

int do_search(string str) {
  write( ({
"You search around for a while, but don't find anything.\n",
"You scrounge around, the ground does look interesting you decide.\n",
"You look carefully at everything, but you find nothing.\n",
"After a while of intensive searching you find nothing.\n" })[random(3)]);
  say(this_player()->query_cap_name()+" searches around the room a bit.\n");
  event(this_object(), "player_search");
  return 1;
} /* do_search() */

/* Cute, useful add_sign. */
/*
 * add_sign(string long, string read_mess, string short, string name)
 * [short and name are optional]
 * This file can be inherited and will return a sign that can be used
 * by a room or object in any way it sees fit.  This function was the
 * brainchild of Wyrm - 7 Feb '92
 * If the short is left out.. The sign isnt moved into the room,
 * but it can still be looked at and read.
 */

varargs object add_sign(string long, string mess, string short, string sname) {
  object sign;

  sign = clone_object("/std/object");

  if(!sname) sname = "sign";
  sign->set_name(sname);
  sign->set_main_plural(pluralize(sname));
  if (short)
    sign->set_short(short);
  sign->set_long(long);
  sign->set_read_mess(mess);
  sign->reset_get();
  if (short && short != "")
    sign->move(this_object());
  else
    hidden_objects += ({ sign });
  destables += ({ sign });
  return sign;
} /* add_sign() */

mapping query_door_control() { return door_control; }

void tell_door(string dir, string mess) {
  if (!door_control[dir])
    return;
  door_control[dir][0]->tell_door(mess);
} /* tell_door() */

mixed call_door(string dir, string func, mixed arg1, mixed arg2, mixed arg3) {
  if (!door_control[dir])
    return;
  return (mixed)call_other(door_control[dir][0], func, arg1, arg2, arg3);
} /* call_door() */


mixed query_door(mixed dest) {
  mixed bing;

  if ((bing = door_control[dest]))
    if (!pointerp(bing))
      return bing;
  if (objectp(dest))
    if ((bing = door_control[file_name(dest)]))
      if (!pointerp(bing))
        return bing;
  return 0;
} /* query_door() */

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
} /* stats() */

int query_decay() { return 10; }

/* Number of move points used by an attack... */
int attack_speed() { return 15; }

void add_hidden_object(object ob) {
  hidden_objects += ({ ob });
} /* add_hidden_object() */

int remove_hidden_object(object ob)  {
  int i;

  if ((i = member_array(ob, hidden_objects)) == -1)
    return 0;
  hidden_objects = delete(hidden_objects, i, 1);
  return 1;
} /* remove_hidden_object() */

object *query_hidden_objects() { return hidden_objects; }
