inherit "/std/room";

#define GO_AWAY "/d/am/am/mendeddrum"

#ifdef PLAYER
#define SONG_PLAYER "/w/pinkfish/std/song_player"
static object cd_player;
#endif

string *root_list, *visitors;
int locked;

void reset() {
#ifdef PLAYER
  if (!cd_player) {
    cd_player->clone_object(SONG_PLAYER);
    cd_player->set_name("player");
    cd_player->add_adjective("cd");
    cd_player->set_short("CD player");
    cd_player->set_long(
"A small black slim line CD player.  It has in small gold "+
"leaf letters \"To Khaos with love from Pinkfish\" written "+
"above the controls.\n");
    cd_player->reset_get();
    cd_player->move(this_object());
  }
#endif
}

void setup() {
  set_short("fireplace room");
  set_long("The room is dominated by a large cheerfully burning "+
           "fireplace on one wall.  Scattered around the floor "+
           "in abandon are litteraly hundreds of cushions.  Over "+
           "on the wall opposite the fire is a CD player and a "+
           "rack of CD's.\n");
  set_light(100);
  add_exit("east", "/w/pinkfish/workroom", "door");
  add_item("fireplace", "A nice brightly burning fire place, "+
                        "carved above it is the message "+
                        "\"Pinkfish loves Khaos\", the words "+
                        "almost seem to be glowing with a light "+
                        "of their own.\n");
  root_list = ({ "pinkfish", "khaos", "signe", "drgoon" });
  visitors = ({ });
}

void init() {
  string victim;

  ::init();
  say((victim=(string)this_player()->query_cap_name())+" warps time.\n");
  if (member_array(lower_case(victim), root_list) != -1) {
    add_action("lock_it", "lock");
    add_action("list_vis", "list");
    add_action("unlock_it", "unlock");
    add_action("trans", "trans");
    add_action("add_vis", "add");
    add_action("remove_vis", "remove");
  }
}

void event_enter(object ob, object mess, string from) {
  string victim;

  if (!interactive(ob))
    return ;
  if (!locked)
    return ;
  victim = (string)ob->query_name();
  if (member_array(victim, root_list) == -1 &&
      member_array(victim, visitors) == -1) {
    write("Sorry, the room you tried to enter appears to be locked.\n");
    ob->move(from, 0, capitalize(victim)+" leaves the room in a hurry.\n");
    return ;
  }
}

int lock_it() {
  if (member_array(this_player()->query_name(), root_list) == -1)
    return 0;
  write("Room is now locked!\n");
  locked = 1;
  return 1;
}

int unlock_it() {
  if (member_array(this_player()->query_name(), root_list) == -1)
    return 0;
  write("Room is now unlocked.\n");
  locked = 0;
  return 1;
}

int add_vis(string name) {
  object ob;

  if (member_array(this_player()->query_name(), root_list) == -1)
    return 0;
  notify_fail("Syntax: "+query_verb()+" <person>\n");
  if (!name)
    return 0;
  if (!(ob = find_player(name)))
    return 0;
  write("Adding "+name+" to the visitors list.\n");
  if (member_array(name, visitors) == -1)
    visitors += ({ name });
  return 1;
}

int remove_vis(string name) {
  int i;
  object ob;

  if (member_array(this_player()->query_name(), root_list) == -1)
    return 0;
  notify_fail("Syntax: "+query_verb()+" <person>\n");
  if (!name)
    return 0;
  if ((i=member_array(name, visitors)) == -1)
    return 0;
  visitors = delete(visitors, i, 1);
  if ((ob = find_player(name)) && environment(ob) == this_object()) {
    tell_object(ob, "Oh dear, you appear to be not wanted.\n");
    ob->move(GO_AWAY,capitalize(name)+" appears looking miffed.\n",
                     capitalize(name)+" disapears unwanted and unneeded.\n");
  }
  write("Ok removed "+name+" from the visitors list.\n");
  return 1;
}

int list_vis() {
  write("Root members\n");
  printf("%#-*s\n", this_player()->query_cols(), implode(root_list, "\n"));
  write("Visitors\n");
  printf("%#-*s\n", this_player()->query_cols(), implode(visitors, "\n"));
  return 1;
}

int trans(string str) {
  object ob;

  notify_fail("Syntax: "+query_verb()+" <player>\n");
  if (!str)
    return 0;
  if (!(ob=find_player(str)))
    return 0;
  tell_object(ob, this_player()->query_cap_name()+
 " requests your presence... Now!\nYou think you suroundings have changed\n");
  if (member_array(str, root_list) == -1 &&
      member_array(str, visitors) == -1)
    visitors += ({ str });
  ob->move(this_object(), capitalize(str)+" vanishes with a splash of water.\n",
                          capitalize(str)+" appears from the fireplace.\n");
  return 1;
}

void event_exit(object ob, string mess, object to) {
  if (interactive(ob))
    say(ob->query_cap_name()+" folds up on "+ob->query_objective()+"self.\n");
}
