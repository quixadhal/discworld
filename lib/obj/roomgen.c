/* object to simplify the creation of normal rooms on the mud */
 
inherit "std/object";
inherit "obj/roomgen/monster";
inherit "obj/roomgen/armour";
inherit "obj/roomgen/weapon";
inherit "obj/roomgen/object";
inherit "obj/roomgen/item";
inherit "obj/roomgen/alias";
inherit "obj/roomgen/exit";
inherit "obj/roomgen/misc";
 
#define MAIN_HELP "/doc/roomgen/main_help"
#define SAVE_DIR "/save/roomgen/"
 
string file_name, temp_file_name, extra_code;
static string tmp_file_name;
 
void main_menu();

void setup() {
  set_short("room generator");
  set_long(
"This is a room generator designed to simplify the process of creating "+
"simple rooms, or skeletons for more complex rooms.  Version 1.00\n"+
"Usage : make <room name> [recover]\n");
  add_adjective("room");
  set_name("generator");
  add_alias("roomgen");
  add_alias("generator");
  set_main_plural("roomgens");
  add_plural("room generators");
  add_plural("generators");
}
 
void init() {
  add_action("make_room", "make");
}
 
void init_data() {
  init_monster_data();
  init_armour_data();
  init_weapon_data();
  init_object_data();
  init_item_data();
  init_alias_data();
  init_exit_data();
  init_misc_data();
  extra_code = "";
}
 
void get_file_data(string fname) {
  string str, temp;

  str = read_file(fname);
  parse_misc(str);
  parse_exit(str);
  parse_item(str);
  parse_alias(str);
  parse_object(str);
  parse_weapon(str);
  parse_armour(str);
  parse_monster(str);
  sscanf(str, "%s/* PLEASE ADD ALL ADITIONAL CODE AFTER THIS LINE */\n%s",
        temp, extra_code);
}

int make_room(string str) {
  string temp, *path;

  if(!str || str == "") {
    notify_fail("Usage : make <filename> [recover]\n");
    return 0;
  }
  seteuid(geteuid(this_player()));
  init_data();
  if(sscanf(str, "%s/%s", temp)) {
    string *names;
 
    names = explode(str, "/");
    temp_file_name = SAVE_DIR + this_player()->query_name() + "." +
                     names[sizeof(names) - 1];
  }
  else {
    temp_file_name = SAVE_DIR + this_player()->query_name() + "." + str;
  }
  if(sscanf(str, "%s recover", file_name) == 1) {
    temp_file_name = SAVE_DIR + this_player()->query_name() +
                     "." + file_name;
    if(restore_object(temp_file_name)) {
      write("recovered " + file_name + "\n");
      main_menu();
      return 1;
    }
    else {
      notify_fail("no temporary saved file to recover.\n");
      return 0;
    }
  }
  str = (string)this_player()->get_path(str);
  if(!sscanf(str, "%s.c", temp)) {
    str += ".c";
  }
  file_name = str;
  path = explode(file_name, "/");
  set_path(implode(path[0..sizeof(path)-2], "/"));
  if(file_size(file_name) == -2) {
    notify_fail(file_name + " is a directory.\n");
    return 0;
  }
  if(file_size(file_name) != -1) {
    write("Reading data from existing file : " + file_name + ".\n");
    get_file_data(file_name);
  }
  main_menu();
  return 1;
}
 
void review() {
  review_misc();
  review_exit();
  review_item();
  review_alias();
  review_object();
  review_weapon();
  review_armour();
  review_monster();
  get_key("main_menu");
}

void generate_file(string fname) {
  string str;
 
  str = "";
  str += get_room_inherit();
  str += "\n#include \"path.h\"\n";
  str += object_vars();
  str += weapon_vars();
  str += armour_vars();
  str += monster_vars();
  if(num_monsters()) {
    str += "make_monsters() {\n" + dump_monsters() + "}\n\n";
  }
  if(num_objects()) {
    str += "make_objects() {\n" + dump_objects() + "}\n\n";
  }
  if(num_weapons()) {
    str += "make_weapons() {\n" + dump_weapons() + "}\n\n";
  }
  if(num_armours()) {
    str += "make_armours() {\n" + dump_armours() + "}\n\n";
  }
  if(num_armours() || num_weapons() || num_monsters() || num_objects()) {
    str += "void reset() {\n";
    if(num_objects()) {
      str += "  make_objects();\n";
    }
    if(num_monsters()) {
      str += "  make_monsters();\n";
    }
    if(num_weapons()) {
      str += "  make_weapons();\n";
    }
    if(num_armours()) {
      str += "  make_armours();\n";
    }
    str += "}\n\n";
  }
  str += "void setup() {\n";
  str += dump_misc();
  str += dump_exits();
  str += dump_items();
  str += dump_aliases();
  str += "}\n\n";
  str += "/* PLEASE ADD ALL ADITIONAL CODE AFTER THIS LINE */\n";
  str += extra_code;
  write("writing file : " + fname + "\n");
  if(file_size(fname) >= 0) {
    rm(fname);
  }
  write_file(fname, str);
  rm(temp_file_name);
}
 
void main_menu() {
  write("saving temporary file : " + temp_file_name + "\n");
  save_object(temp_file_name);
  write("Please select from the following commands:\n"+
         "   1.  misc\n"+
         "   2.  exits\n"+
         "   3.  items\n"+
         "   4.  aliases\n"+
         "   5.  objects\n"+
         "   6.  monsters\n"+
         "   edit - drop into ed\n"+
         "   show - review room and contents\n"+
         "   init - initialise\n"+
         "   help - help on how to use the room generator\n"+
         "   save - save room and exit\n"+
         "   exit - exit without saving\n"+
         "Selection : ");
  input_to("handle_main_menu");
}
 
void handle_main_menu(string str) {
 
  switch(str) {
    case "help":
      cat(MAIN_HELP);
      get_key("main_menu");
      return;
    case "save":
      generate_file(file_name);
      rm(temp_file_name);
      return;
    case "exit":
      rm(temp_file_name);
      write("Ok.\n");
      return;
    case "1":
      misc_menu();
      return;
    case "2":
      exit_menu();
      return;
    case "3":
      item_menu();
      return;
    case "4":
      alias_menu();
      return;
    case "5":
      object_menu();
      return;
    case "6":
      monster_menu();
      return;
    case "show":
      review();
      return;
    case "init":
      init_data();
      return;
    case "edit":
      tmp_file_name = "/tmp/rgen-"+(string)this_player()->query_name();
      generate_file(tmp_file_name);
      ed(tmp_file_name, "finish_ed");
      return;
    case "":
      main_menu();
      return;
    default:
      write("Selection : ");
      input_to("handle_main_menu");
      return;
  }
}
 
string get_name(string str) {
  return "this_object()";
}

void finish_ed() {
  init_data();
  get_file_data(tmp_file_name);
  rm(tmp_file_name);
  main_menu();
}

