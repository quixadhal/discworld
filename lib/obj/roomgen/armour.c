inherit "obj/roomgen/support";

string *room_armours;
 
void init_armour_data() {
  room_armours = ({ });
}
 
void armour_menu() {
  write("   1. add armour to the room\n");
  write("   2. list armours for the room\n");
  write("   3. edit armour for the room\n");
  write("   4. delete armour for the room\n");
  write("   help - help on armours\n");
  write("Selection :");
  input_to("handle_armour_menu");
}
 
void handle_armour_menu(string str) {
}
 
string armour_vars() {
  if(sizeof(room_armours)) {
    return "object *armours";
  }
  else {
    return "";
  }
}
 
string dump_armours() {
  return "";
}
 
int num_armours() {
  return sizeof(room_armours);
}

void parse_armour(string str) {
}

void review_armour() {
}
