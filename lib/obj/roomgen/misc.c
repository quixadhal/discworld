inherit "obj/roomgen/support";
 
string short, long, zone, inher_file, *new_long;
int light_level, *coord;
 
#define MISC_HELP_FILE "/doc/roomgen/misc"
#define INHERIT_HELP "/doc/roomgen/inherit_help"
#define SUGGESTED_LIGHT "/doc/roomgen/suggested_light"
#define ZONE_HELP_FILE "/doc/roomgen/zone_help"
 
void misc_menu();
 
void init_misc_data() {
  inher_file = "std/room";
  short = "somewhere";
  long = "\"You are in a very boring room.\\n\"";
  zone = "somewhere";
  light_level = 60;
  coord = ({ 0, 0, 0 });
}
 
string get_room_inherit() {
  return "inherit \"" + inher_file + "\";\n";
}

void set_room_inherit() {
  output("Inherit what file (? for help) : ");
  input_to("set_room_inherit2");
}

void set_room_inherit2(string str) {
  if(str == "") {
    misc_menu();
    return;
  }
  if(str == "?") {
    cat(INHERIT_HELP);
    input_to("set_room_inherit2");
    return;
  }
  inher_file = str;
  misc_menu();
}
 
void set_short_desc() {
  output("Enter short description : ");
  input_to("set_short_desc2");
}
 
void set_short_desc2(string str) {
  if(str != "") {
    short = str;
    output("Short description set to " + str + "\n");
  }
  misc_menu();
}
 
void set_long_desc() {
  output("Enter long description.  ** to end and ~q to abort.\n] ");
  new_long = ({ });
  input_to("set_long_desc2");
}
 
void set_long_desc2(string str) {
  if(str == "~q") {
    misc_menu();
    return;
  }
  if(str == "**") {
    new_long[sizeof(new_long) - 1] += " + \"\\n\"\n";
    long = implode(new_long, " +\n");
    output("Long description now reads:\n");
    output(long);
    misc_menu();
    return;
  }
  if(sizeof(new_long)) {
    new_long += ({"\" " + str + "\"" });
  }
  else {
    new_long += ({ "\"" + str + "\"" });
  }
  output("] ");
  input_to("set_long_desc2");
}
 
void set_light_level() {
  output("What light level for the room (? for suggested) : ");
  input_to("set_light_level2");
}
 
void set_light_level2(string str) {
  if(str == "?") {
    cat(SUGGESTED_LIGHT);
    set_light_level();
    return;
  }
  if(!sscanf(str, "%d", light_level)) {
    output("Enter a single number please.\n");
    set_light_level();
    return;
  }
  misc_menu();
}
 
void set_room_zone() {
  output("Enter zone for the room (? for help) : ");
  input_to("set_room_zone2");
}
 
void set_room_zone2(string str) {
  if(str == "?") {
    cat(ZONE_HELP_FILE);
    set_room_zone();
    return;
  }
  if(str != "") {
    zone = str;
  }
  misc_menu();
}
 
void set_room_coord() {
  output("Please enter the room coords '%d %d %d' : ");
  input_to("set_room_coord2");
}
 
void set_room_coord2(string str) {
  if(!sscanf(str, "%d %d %d", coord[0], coord[1], coord[2])) {
    output("Please use the format 1 2 3 using spaces to seperate numbers\n");
    set_room_coord();
    return;
  }
  misc_menu();
}
 
void misc_menu() {
  output("Please select from the following:\n"+
         "   1. short description\n"+
         "   2. long description\n"+
         "   3. light level\n"+
         "   4. room zone\n"+
         "   5. room coordinate\n"+
         "   6. file to inherit\n"+
         "   help - help on miscellaneous functions\n"+
         "   end - return to main menu\n"+
         "Selection : ");
  input_to("handle_misc_menu");
}
 
void handle_misc_menu(string str) {
  switch(str) {
    case "1":
      set_short_desc();
      return;
    case "2":
      set_long_desc();
      return;
    case "3":
      set_light_level();
      return;
    case "4":
      set_room_zone();
      return;
    case "5":
      set_room_coord();
      return;
    case "6":
      set_room_inherit();
      return;
    case "help":
      cat(MISC_HELP_FILE);
      get_key("misc_menu");
      return;
    case "end":
      this_object()->main_menu();
      return;
    case "":
      misc_menu();
      return;
    default:
      output("Invalid Choice\nSelection: ");
      input_to("handle_misc_menu");
  }
}
 
string dump_misc() {
  string str;
 
  str = "";
  str += "  set_short(\"" + short + "\");\n";
  str += "  set_long(\n" + long + ");\n";
  str += "  set_light(" + light_level + ");\n";
  str += "  set_zone(\"" + zone + "\");\n";
  if(coord[0] || coord[1] || coord[2]) {
    str += "  set_coord("+coord[0]+", "+coord[1]+", "+coord[2]+");\n";
  }
  return str;
}
 
void parse_misc(string str) {
  string temp1, temp2, temp3;
  int tempint1, tempint2, tempint3;

  if(sscanf(str, "%svoid setup() {%s", temp1, temp2) == 1) {
    str = temp2;
  }
  if(sscanf(str, "%sset_long(\n%s);%s", temp1, temp2, temp3) == 3) {
    long = temp2;
  }
  if(sscanf(str, "%sset_short(\"%s\");%s", temp1, temp2, temp3) == 3) {
    short = temp2;
  }
  if(sscanf(str, "%sset_light(%d);%s", temp1, tempint1, temp2) == 3) {
    light_level = tempint1;
  }
  if(sscanf(str, "%sset_coord(%d, %d, %d);%s", temp1, tempint1, tempint2,
            tempint3, temp2) == 5) {
    coord[0] = tempint1;
    coord[1] = tempint2;
    coord[2] = tempint3;
  }
  if(sscanf(str, "%sset_zone(\"%s\");%s", temp1, temp2, temp3) == 3) {
    zone = temp2;
  }
  if(sscanf(str, "%sinherit \"%s\";%s", temp1, temp2, temp3) == 3) {
    inher_file = temp2;
  }
}

void review_misc() {
  write("Short : " + short + "\n");
  write("Long :\n" + long + "\n\n");
  write("Zone : " + zone + "\n");
  write("File : " + inher_file + "\n");
  write("Light : " + light_level + "\n");
  write("Coord : (" + coord[0] + ", " + coord[1] + ", " + coord[2] + ")\n");
}
