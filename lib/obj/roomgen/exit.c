inherit "/obj/roomgen/support";
 
string *room_exits, *room_exit_dests, *room_exit_types;
static string exit_dir, exit_dest, exit_type;
static int pos;
 
#define EXIT_HELP "/doc/roomgen/exit_help"
#define DIR_HELP "/doc/roomgen/dir_help"
#define DEST_HELP "/doc/roomgen/dest_help"
#define EXIT_TYPE_HELP "/doc/roomgen/exit_type_help"
 
void exit_menu();
 
void init_exit_data() {
  room_exits = ({ });
  room_exit_dests = ({ });
  room_exit_types = ({ });
}
 
void add_exit() {
  output("Which direction will the exit leave (? for help): ");
  input_to("add_exit2");
}
 
void add_exit2(string str) {
  if(str == "") {
    exit_menu();
  }
  if(str == "?") {
    cat(DIR_HELP);
    add_exit();
    return;
  }
  if(member_array(str, room_exits) != -1) {
    output("That exit already exists.  Please choose another direction or "+
           "delete the exit that currently exists.\n");
    exit_menu();
  }
  exit_dir = str;
  output("To what room will the exit lead (? for help) : ");
  input_to("add_exit3");
}
 
void add_exit3(string str) {
  if(str == "") {
    exit_menu();
  }
  if(str == "?") {
    cat(DEST_HELP);
    output("To what room will the exit lead (? for help) : ");
    input_to("add_exit3");
    return;
  }
  exit_dest = str;
  output("What type will the exit be (? for possibilities) : ");
  input_to("add_exit4");
}
 
void add_exit4(string str) {
  if(str == "") {
    exit_menu();
  }
  if(str == "?") {
    cat(EXIT_TYPE_HELP);
    output("What type will the exit be (? for possibilities) : ");
    input_to("add_exit4");
    return;
  }
  room_exits += ({ exit_dir });
  room_exit_dests += ({ exit_dest });
  room_exit_types += ({ str });
  output("exit added.\n\n");
  exit_menu();
}
 
void list_exits() {
  if(sizeof(room_exits)) {
    int loop;
 
    output("Exit Direction, Exit Destination, Exit Type\n");
    for(loop = 0; loop < sizeof(room_exits); loop++) {
      output(room_exits[loop] + ", " + room_exit_dests[loop] + ", " +
            room_exit_types[loop] + "\n");
    }
  }
  else {
    output("No exits defined.\n");
  }
}

void list_room_exits() {
  list_exits();
  get_key("exit_menu");
}
 
void edit_exit_menu() {
  if(!sizeof(room_exits)) {
    output("No exits!\n");
    exit_menu();
    return;
  }
  output("edit which exit : ");
  input_to("edit_exit_menu2");
}
 
void edit_exit_menu3();

void edit_exit_menu2(string str) {
  if((pos = member_array(str, room_exits)) == -1) {
    output("No such exit!\n");
    exit_menu();
    return;
  }
  edit_exit_menu3();
}
 
void edit_exit_menu3() {
  output("Edit which field?\n"+
         "   1. direction\n"+
         "   2. destination\n"+
         "   3. type\n"+
         "   end - back to exit menu\n"+
         "Selection : ");
  input_to("edit_exit_menu4");
}
 
void edit_exit_menu4(string str) {
 
  switch(str) {
    case "1":
      output("new direction : ");
      input_to("get_new_dir");
      return;
    case "2":
      output("new destination : ");
      input_to("get_new_dest");
      return;
    case "3":
      output("new type : ");
      input_to("get_new_type");
      return;
    case "end":
      exit_menu();
      return;
    default:
      output("Invalid choice\nSelection\n");
      edit_exit_menu3();
      return;
  }
}
 
void get_new_dir(string str) {
  string old;
 
  if(str == "") {
    edit_exit_menu3();
  }
  old = room_exits[pos];
  room_exits[pos] = str;
  exit_dir = str;
  output("exit direction modified from " + old + " " + str + "\n");
  edit_exit_menu3();
}
 
void get_new_dest(string str) {
  string old;
 
  if(str == "") {
    edit_exit_menu3();
  }
  old = room_exit_dests[pos];
  room_exit_dests[pos] = str;
  output("exit destination modified from " + old + " " + str + "\n");
  edit_exit_menu3();
}
 
void get_new_type(string str) {
  string old;
 
  if(str == "") {
    edit_exit_menu3();
  }
  old = room_exit_types[pos];
  room_exit_types[pos] = str;
  exit_dir = str;
  output("exit type modified from " + old + " " + str + "\n");
  edit_exit_menu3();
}
 
void delete_exit() {
  output("Delete which exit : ");
  input_to("delete_exit2");
}
 
void delete_exit2(string str) {
  if(str == "") {
    exit_menu();
    return;
  }
  if((pos = member_array(str, room_exits)) == -1) {
    output("No such exit!\n");
    exit_menu();
    return;
  }
  output("Delete exit " + str + ".  Are you sure? (Y/N) : ");
  input_to("confirm_delete_exit");
}
 
void confirm_delete_exit(string str) {
  string rm_name;

  if(lower_case(str)[0] != 'y') {
    output("Exit not deleted!\n");
    exit_menu();
  }
  rm_name = room_exits[pos];
  room_exits = delete(room_exits, pos, 1);
  room_exit_dests = delete(room_exit_dests, pos, 1);
  room_exit_types = delete(room_exit_types, pos, 1);
  output("Deleted exit " + rm_name + "\n");
  exit_menu();
}
 
void exit_menu() {
  output("Please select from the following commands:\n"+
         "   1. add exit to room description\n"+
         "   2. list exits in room\n"+
         "   3. edit exit in room\n"+
         "   4. delete exit from room\n"+
         "   end - back to main menu\n"+
         "   help - help on exits\n"+
         "Selection : ");
  input_to("handle_exit_menu");
}
 
void handle_exit_menu(string str) {
 
  switch(str) {
    case "1":
      add_exit();
      return;
    case "2":
      list_room_exits();
      return;
    case "3":
      edit_exit_menu();
      return;
    case "4":
      delete_exit();
      return;
    case "end":
      this_object()->main_menu();
      return;
   case "help":
      cat(EXIT_HELP);
      get_key("exit_menu");
      return;
    case "":
      exit_menu();
      return;
    default:
      output("Invalid choice\nSelection : ");
      input_to("handle_exit_menu");
  }
}
 
string dump_exits() {
  string str;
  int loop;
 
  str = "";
  for(loop = 0; loop < sizeof(room_exits); loop++) {
    if(room_exit_dests[loop][0] == '/') {
      str += "  add_exit(\"" +
             room_exits[loop] + "\", \"" +
             room_exit_dests[loop] + "\", \"" +
             room_exit_types[loop] + "\");\n";
    }
    else {
      str += "  add_exit(\"" +
             room_exits[loop] + "\", ROOM+\"" +
             room_exit_dests[loop] + "\", \"" +
             room_exit_types[loop] + "\");\n";
    }
  }
  return str;
}
 
void parse_exit(string str) {
  string temp1, temp2, temp3, temp4, temp5, temp6;
  int tempint1, tempint2, tempint3;

  if(sscanf(str, "%svoid setup() {%s", temp1, temp2) == 2) {
    str = temp2;
  }
  while(sscanf(str, "%sadd_exit(\"%s\", %s, \"%s\");\n%s", temp1, temp2,
             temp3, temp4, temp5) == 5) {
    str = temp5;
    if(!str) {
      str = "";
    }
    if(sscanf(temp3, "ROOM+\"%s\"", temp6) ||
       sscanf(temp3, "\"%s\"", temp6)) {
      room_exits += ({ temp2 }); 
      room_exit_dests += ({ temp6 });
      room_exit_types += ({ temp4 }); 
    }
  }
}

void review_exit() {
  list_exits();
}
