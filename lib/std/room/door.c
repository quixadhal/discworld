#include "parse_command.h"
#include "room.h"
#include "door.h"
#define ROOM_HAND "/obj/handlers/room_handler"

string lng,
       shrt,
       my_id,  /* The id of the door used on the room... */
       *adjs,
       *plu,
       *name,
       key_prop,
       other_id;
mixed  *unlock_func,
       *open_func;
object my_room,
       dest;
int locked,
    open;

int do_unlock(object *obs);

void create() {
  adjs = ({ });
  plu = ({ "doors" });
  name = ({ "door" });
  key_prop = "generic key";
  lng = "";
}

void set_key_prop(string p) { key_prop = p; }
string query_key_prop() { return key_prop; }

void set_other_id(string s) { other_id = s; }
string query_other_id() { return other_id; }

void set_open_func(mixed *bing) { open_func = bing; }
mixed *query_open_func() { return open_func; }

void set_unlock_func(mixed *bing) { unlock_func = bing; }
mixed *query_unlock_func() { return unlock_func; }

void set_locked(int i) { locked = i; }
int query_locked() { return locked; }

void set_open(int i) { open = i; }
int query_open() { return open; }

void init() {
  this_player()->add_command("open", this_object(), "%D");
  this_player()->add_command("close", this_object(), "%D");
  this_player()->add_command("lock", this_object(), "%D %p %I");
  this_player()->add_command("unlock", this_object(), "%D %p %I");
}

string short() {
  return shrt;
}

string pretty_short() { return short(); }

string query_plural() { return pluralize(shrt); }
string pretty_plural() { return query_plural(); }

string long() {
  string ret;

  if (open)
    ret = "The door is open.\n";
  else {
    ret = "The door is closed.\n";
    if (locked)
      ret += "The door is locked.\n";
  }
  return lng+ret;
}

void set_long(string s) { lng = s; }
string query_long() { return lng; }

string query_short() { return shrt; }
void set_short(string str) { 
  string *bits;

  bits = explode(str, " ");
  adjs += bits[0..sizeof(bits)-2];
  name += ({ bits[sizeof(bits)-1] });
  shrt = str;
}

int drop() { return 1; }
int get() { return 1; }

void setup_door(string nam, object mine, object his, mixed *bing) {
  string *bits, s;

  my_id = nam;
  my_room = mine;
  dest = his;
  if (nam[0..5] == "enter ")
    shrt = nam[6..1000]+" door";
  else
    shrt = nam+" door";
  bits = explode(nam, " ");
  name += ({ (s=bits[sizeof(bits)-1]) });
  plu += ({ pluralize(s) });
  adjs += bits;
  open = bing[D_OPEN];
  locked = bing[D_LOCKED];
}

void add_alias(string nam) {
  string *bits, s;

  bits = explode(nam, " ");
  name += ({ (s=bits[sizeof(bits)-1]) });
  plu += ({ pluralize(s) });
  adjs += bits;
}

int do_open() {
  if (open)
    return 0;
  if (locked)
    if (!do_unlock(all_inventory(this_player())))
      return 0;
  if (open_func) {
    if (!call_other(open_func[0], open_func[1], 1))
      return 0;
  }
/* Ok...  now open it... */
  my_room->modify_exit(my_id, ({ "open", 1 }));
  if (!other_id) { /* Force it... */
    ROOM_HAND->check_door( ({ my_room, my_id,  dest }));
    if (!other_id)
      return 0;
  }
  dest->modify_exit(other_id, ({ "open", 1 }));
  dest->tell_door(other_id, this_player()->query_cap_name()+
                            " opens the %D.\n");
  return 1;
}

int do_close() {
  if (!open)
    return 0;
/* Ok...  now open it... */
  if (open_func) {
    if (!call_other(open_func[0], open_func[1], 0))
      return 0;
  }
  my_room->modify_exit(my_id, ({ "open", 0 }));
  if (!other_id) { /* Force it... */
    ROOM_HAND->check_door( ({ my_room, my_id,  dest }));
    if (!other_id) /* errr...  we should die around here... */
      return 0;
  }
  dest->modify_exit(other_id, ({ "open", 0 }));
  dest->tell_door(other_id, this_player()->query_cap_name()+
                            " closes the %D.\n", this_player());
  return 1;
}

int test_key(object ob) {
  return (int)ob->query_property(key_prop);
}

int do_lock(object *obs) {
  int ret;

  if (open)
    return 0;
  if (locked)
    return 0;
  if (unlock_func) {
    if (!(ret = call_other(unlock_func[0], unlock_func[1], 0)))
      return 0;
  }
  if (ret != 2)
    if (!sizeof(obs = filter_array(obs, "test_key", this_object())))
      return 0;
/* Ok...  now open it... */
  if (!other_id) { /* Force it... */
    ROOM_HAND->check_door( ({ my_room, my_id,  dest }));
    notify_fail("The door is already locked.\n");
    if (!other_id)
      return 0;
  }
  my_room->modify_exit(my_id, ({ "locked", 1 }));
  dest->modify_exit(other_id, ({ "locked", 1 }));
  dest->tell_door(other_id, "Someone locks the %D.\n");
  this_player()->add_succeeded(obs[0]);
  return 1;
}

int do_unlock(object *obs) {
  int ret;

  notify_fail("You cannot unlock the door.\n");
  if (!locked)
    return 0;
  if (unlock_func) {
    if (!(ret = call_other(unlock_func[0], unlock_func[1], 1)))
      return 0;
  }
  if (ret != 2)
    if (!sizeof(obs = filter_array(obs, "test_key", this_object())))
      return 0;
/* Ok...  now open it... */
  my_room->modify_exit(my_id, ({ "locked", 0 }));
  if (!other_id) { /* Force it... */
    ROOM_HAND->check_door( ({ my_room, my_id,  dest }));
    if (!other_id)
      return 0;
  }
  dest->modify_exit(other_id, ({ "locked", 0 }));
  dest->tell_door(other_id, "Someone unlocks the %D.\n");
  this_player()->add_succeeded(obs[0]);
  return 1;
}

string *parse_command_id_list() { return name; }
string *parse_command_plural_id_list() { return plu; }
string *parse_command_adjectiv_id_list() { return adjs; }

object query_parse_id(mixed *arr) {
  string *bits;
  mixed stuff;
  int i;

  if (arr[P_THING] == 0) {
    bits = explode(arr[P_STR], " ");
    return this_object();
  }
  if (arr[P_THING] < 0) { /* specific object case */
    arr[P_THING]++;
    if (arr[P_THING] != 0)
      return 0;
    arr[P_THING] = -10321;
    return this_object();
  }
  arr[P_THING]--; /* lots of objects case */
  if (arr[P_THING] != 0)
    return this_object();
  arr[P_THING] = -10786;
  return this_object();
}

void dest_me() {
  destruct(this_object());
  return ;
}

void dwep() {
  destruct(this_object());
  return ;
}

int move() { return 1; }

/*
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
}
 */

int moveing_open(object ob) {
  if (!do_open())
    return 0;
  tell_object(ob, "You open the "+shrt+".\n");
  tell_room(my_room, ob->query_cap_name()+" opens the "+shrt+".\n", ob);
  return 1;
}

int moveing_close(object ob) {
  string oshrt;
  if (!dest->call_door(other_id, "do_close"))
    return 0;
  oshrt = (string)dest->call_door(other_id, "query_short");
  tell_object(ob, "You close the "+oshrt+".\n");
  tell_room(dest, ob->query_cap_name()+" closes the "+oshrt+".\n", ob);
  return 1;
}

int moveing_lock(object ob) {
  string oshrt;

  if (!dest->call_door(other_id, "do_lock", all_inventory(ob)))
    return 0;
  oshrt = (string)dest->call_door(other_id, "query_short");
  tell_object(ob, "You lock the "+oshrt+".\n");
  tell_room(dest, ob->query_cap_name()+" locks the "+oshrt+" door,\n", ob);
  return 1;
}

int moveing_unlock(object ob) {
  if (!do_unlock(all_inventory(ob)))
    return 0;
  tell_object(ob, "You unlock the "+shrt+".\n");
  tell_room(my_room, ob->query_cap_name()+" unlocks the "+shrt+" door,\n", ob);
  return 1;
}

void tell_door(string str) {
  tell_room(my_room, replace(str, "%D", shrt), ({ this_player() }));
}
