inherit "obj/roomgen/support";

string *room_monsters;
 
init_monster_data() {
  room_monsters = ({ });
}
 
void monster_menu() {
  write("   1. add monster to the room\n");
  write("   2. list monsters in the room\n");
  write("   3. edit monster in the room\n");
  write("   4. delete monster in the room\n");
  write("   help - help on monsters\n");
  write("Selection :");
  input_to("handle_monster_menu");
}
 
void handle_monster_menu(string str) {
}
 
string monster_vars() {
  if(sizeof(room_monsters)) {
    return "object *monsters";
  }
  else {
    return "";
  }
}
 
string dump_monsters() {
  return "";
}
 
int num_monsters() {
   return sizeof(room_monsters);
}

void parse_monster(string str) {
}

void review_monster() {
}
