inherit "obj/roomgen/support";

string *room_weapons;
 
void init_weapon_data() {
  room_weapons = ({ });
}
 
void weapon_menu() {
  write("   1. add a weapon to the room\n");
  write("   2. list weapons for the room\n");
  write("   3. edit weapon for the room\n");
  write("   4. delete weapon for the room\n");
  write("   help - help on weapons\n");
  write("Selection :");
  input_to("handle_weapon_menu");
}
 
void handle_weapon_menu(string str) {
}
 
string weapon_vars() {
  if(sizeof(room_weapons)) {
    return "object *weapons";
  }
  else {
    return "";
  }
}
 
string dump_weapons() {
  return "";
}
 
 
int num_weapons() {
  return sizeof(room_weapons);
}

void parse_weapon(string str) {
}

void review_weapon() {
}
