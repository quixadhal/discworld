inherit "obj/roomgen/support";
 
string *room_object_names, *room_object_startpos, *room_object_files;
mixed *room_object_funcs;
static string object_name, setup_function_return;
static int pos, func_pos;
 
#define OBJECT_NAME_HELP "/doc/roomgen/object_name_help"
#define OBJECT_FILE_HELP "/doc/roomgen/object_file_help"
#define SETUP_EXAMPLES "/doc/roomgen/setup_examples"
#define SETUP_FUNCTION_HELP "/doc/roomgen/setup_examples"
#define START_HELP "/doc/roomgen/start_help"
#define OBJECT_HELP "/doc/roomgen/object_help"
 
void object_menu();
void setup_function_menu3();
void setup_function_menu5();
void setup_function_menu7();
void list_setup_functions();
void list_objects();
void edit_object3();

void init_object_data() {
  room_object_names = ({ });
  room_object_files = ({ });
  room_object_funcs = ({ });
  room_object_startpos = ({ });
}
 
 
/* Add object code */
 
 
void add_standard_object_setup_funcs() {
  write("not implemented\n");
  input_to("object_menu");
}

void add_object_menu() {
  write("Name of object (? for suggestions) : ");
  input_to("setup_function_menu2");
}
 
void setup_function_menu2(string str) {
  int loop;
 
  if(str == "?") {
    cat(OBJECT_NAME_HELP);
    add_object_menu();
    return;
  }
  if(str == "") {
    object_menu();
    return;
  }
  if((pos = member_array(str, room_object_names)) != -1) {
    write("Object already exists.\n");
    object_menu();
    return;
  }
  object_name = str;
  setup_function_menu3();
}
 
void setup_function_menu3() {
  write("Which object to configure (? for common ones) : ");
  input_to("setup_function_menu4");
}
 
void setup_function_menu4(string str) {
  if(str == "?") {
    cat(OBJECT_FILE_HELP);
    setup_function_menu3();
    return;
  }
  if(str == "") {
    object_menu();
    return;
  }
  room_object_names += ({ object_name });
  room_object_files += ({ str });
  room_object_funcs += ({ ({ }) });
  pos = sizeof(room_object_funcs) - 1;
  setup_function_menu7();
}
 
void setup_function_menu7() {
  write("In what object will this object start (? for help) : ");
  input_to("setup_function_menu8");
}
 
void setup_function_menu8(string str) {
  if(str == "?") {
    cat(START_HELP);
    setup_function_menu7();
    return;
  }
  if(str == "") {
    object_menu();
    return;
  }
  room_object_startpos += ({ str });
  setup_function_return = "object_menu";
  setup_function_menu5();
}
 
void add_setup_function() {
  write("enter function and parameters (? for samples)\n: ");
  input_to("add_setup_function2");
}
 
void add_setup_function2(string str) {
  if(str == "?") {
    cat(SETUP_EXAMPLES);
    add_setup_function();
    return;
  }
  if(str == "") {
    setup_function_menu5();
    return;
  }
  room_object_funcs[func_pos] += ({ str });
  setup_function_menu5();
}
 
void list_setup_functions1() {
  list_setup_functions();
  get_key("setup_function_menu5");
}
 
void list_setup_functions() {
  int loop;
 
  for(loop = 0; loop < sizeof(room_object_funcs[pos]); loop++) {
    write(loop + " :\t" + room_object_funcs[pos][loop] + "\n");
  }
}
 
void edit_setup_function() {
  write("Edit which function (? for list) : ");
  input_to("edit_setup_function2");
}
 
void edit_setup_function2(string str) {
  if(str == "?") {
    list_setup_functions();
  }
  if(str == "") {
    setup_function_menu5();
  }
  if(sscanf(str, "%d", func_pos) >= sizeof(room_object_funcs[pos])) {
    write("Not that many functions.  Enter number of function.\n");
    setup_function_menu5();
    return;
  }
  write("Enter new function (? for suggestions) : ");
  input_to("edit_setup_function3");
}
 
void edit_setup_function3(string str) {
  if(str == "?") {
    cat(SETUP_EXAMPLES);
    write("Enter new function (? for suggestions) : ");
    input_to("edit_setup_function3");
    return;
  }
  if(str == "") {
    setup_function_menu5();
    return;
  }
  room_object_funcs[pos][func_pos] = str;
  setup_function_menu5();
}
 
void delete_setup_function() {
  write("Delete which setup function (? for list) : ");
  input_to("delete_setup_function2");
}
 
void delete_setup_function2(string str) {
  if(str == "?") {
    list_setup_functions();
  }
  if(str == "") {
    setup_function_menu5();
  }
  if(sscanf(str, "%d", func_pos) >= sizeof(room_object_funcs[pos])) {
    write("Not that many functions.  Enter number of function.\n");
    setup_function_menu5();
    return;
  }
  write("Delete " + room_object_funcs[pos][func_pos] +
        "\nAre you sure (Y/N) : ");
  input_to("delete_setup_function3");
}
 
void delete_setup_function3(string str) {
  string old;
 
  str = lower_case(str);
  old = room_object_funcs[pos][func_pos];
  if(str[0] != 'y') {
    write(old + " NOT deleted.\n");
    setup_function_menu5();
    return;
  }
  room_object_funcs[pos] = delete(room_object_funcs[pos], func_pos, 1);
  write(old + " deleted.\n");
  setup_function_menu5();
}
 
void setup_function_menu5() {
  write("Object : " + room_object_names[sizeof(room_object_names)-1] + "\n" +
        "File   : " + room_object_files[sizeof(room_object_files)-1] + "\n" +
        "Please select from the following commands:\n"+
        "   1. add standard setup functions\n"+
        "   2. add setup function call\n"+
        "   3. list setup function calls\n"+
        "   4. edit setup function call\n"+
        "   5. delete setup function call\n"+
        "   end - return to add object menu\n"+
        "   help - about setup functions\n"+
        "Selection : ");
  input_to("setup_function_menu6");
}
 
void setup_function_menu6(string str) {
  switch(str) {
    case "1":
      add_standard_object_setup_funcs();
      return;
    case "2":
      add_setup_function();
      return;
    case "3":
      list_setup_functions1();
      return;
    case "4":
      edit_setup_function();
      return;
    case "5":
      delete_setup_function();
      return;
    case "end":
      call_other(this_object(), setup_function_return);
      return;
    case "help":
      cat(SETUP_FUNCTION_HELP);
      get_key("setup_function_menu5");
      return;
    case "":
      setup_function_menu5();
      return;
    default:
      write("Invalid Choice\nSelection : ");
      input_to("setup_function_menu6");
      return;
  }
}
 
 
/* list objects code */
 
 
void list_objects1() {
  list_objects();
  get_key("object_menu");
}
 
void list_objects() {
  int loop1, loop2;
 
  for(loop1 = 0; loop1 < sizeof(room_object_names); loop1++) {
    write("Name  : " + room_object_names[loop1] + "\n" +
          "File  : " + room_object_files[loop1] + "\n" +
          "Start : " + room_object_startpos[loop1] + "\n" +
          "Setup : \n");
    for(loop2 = 0; loop2 < sizeof(room_object_funcs[loop1]); loop2++) {
      write(room_object_funcs[loop1][loop2] + "\n");
    }
    write("\n");
  }
}
 
 
/* edit object code */
 
 
void get_new_name() {
  write("Enter new name for " + room_object_names[pos] + " : ");
  input_to("get_new_name2");
}
 
void get_new_name2(string str) {
  if(str != "") {
    write("Name changed from "+room_object_names[pos]+" to "+str+"\n");
    room_object_names[pos] = str;
  }
  edit_object3();
}
 
void get_new_file() {
  write("Enter new file for " + room_object_names[pos] + " : ");
  input_to("get_new_file2");
}
 
void get_new_file2(string str) {
  if(str != "") {
    write("File changed from "+room_object_files[pos]+" to "+str+"\n");
    room_object_files[pos] = str;
  }
  edit_object3();
}
 
void get_new_startpos() {
  write("Enter new start position (? for help) : ");
  input_to("get_new_startpos2");
}

void get_new_startpos2(string str) {
  if(str != "") {
    write("Start changed from "+room_object_startpos[pos]+" to "+str+"\n");
    room_object_startpos[pos] = str;
  }
  edit_object3();
}
 
void edit_object() {
  write("Edit which object (? for list) : ");
  input_to("edit_object2");
}
 
void edit_object2(string str) {
  int loop;
 
  if(str == "?") {
    for(loop = 0; loop < sizeof(room_object_names); loop++) {
      write(room_object_names[loop] + "\n");
    }
    edit_object();
    return;
  }
  if(str == "") {
    object_menu();
    return;
  }
  if((pos = member_array(str, room_object_names)) == -1) {
    write("No such object.\n");
    object_menu();
  }
  edit_object3();
}
 
void edit_object3() {
  write("Please select from the following:\n"+
        "   1. Change Name\n"+
        "   2. Change File\n"+
        "   3. Change Start Position\n"+
        "   4. Edit Setup Functions\n"+
        "   end - return to Object menu\n"+
        "Selection : ");
  input_to("handle_edit_object");
}
 
void handle_edit_object(string str) {
  switch(str) {
    case "1":
      get_new_name();
      return;
    case "2":
      get_new_file();
      return;
    case "3":
      get_new_startpos();
      return;
    case "4":
      setup_function_return = "edit_object3";
      setup_function_menu5();
      return;
    case "end":
      object_menu();
      return;
    case "":
      edit_object3();
      return;
    default:
      write("Bad Choice\nSelection : ");
      input_to("handle_edit_object");
      return;
  }
}
 
 
/* delete object code */
 
 
void delete_object() {
  write("Delete which object (? for list) : ");
  input_to("delete_object2");
}
 
void delete_object2(string str) {
  int loop;
 
  if(str == "?") {
    for(loop = 0; loop < sizeof(room_object_names); loop++) {
      write(room_object_names[loop] + "\n");
    }
    delete_object();
    return;
  }
  if(str == "") {
    object_menu();
    return;
  }
  if((pos = member_array(str, room_object_names)) == -1) {
    write("No such object.\n");
    object_menu();
    return;
  }
  write("Delete object " + str + ".  Are you sure (Y/N) : ");
  input_to("delete_object3");
}
 
void delete_object3(string str) {
  string old;
 
  str = lower_case(str);
  old = room_object_names[pos];
  if(str[0] != 'y') {
    write(old + " NOT deleted.\n");
    object_menu();
    return;
  }
  room_object_names = delete(room_object_names, pos, 1);
  room_object_files = delete(room_object_files, pos, 1);
  room_object_funcs = delete(room_object_funcs, pos, 1);
  room_object_startpos = delete(room_object_startpos, pos, 1);
  write(old + " deleted.\n");
}
 
/* object menu code */
 
 
void object_menu() {
  write("Please select from the following commands:\n"+
        "   1. add object to the room\n"+
        "   2. list objects of the room\n"+
        "   3. edit object of the room\n"+
        "   4. remove object from the room\n"+
        "   end - return to main menu\n"+
        "   help - help on objects\n"+
        "Selection :");
  input_to("handle_object_menu");
}
 
void handle_object_menu(string str) {
  switch(str) {
    case "1":
      add_object_menu();
      return;
    case "2":
      list_objects1();
      return;
    case "3":
      edit_object();
      return;
    case "4":
      delete_object();
      return;
    case "help":
      cat(OBJECT_HELP);
      get_key("object_menu");
      return;
    case "end":
      this_object()->main_menu();
      return;
    case "":
      object_menu();
      return;
    default:
      write("Invalid Choice\nSelection : ");
      input_to("handle_object_menu");
      return;
  }
}
 
 
/* IO interface code */
 
 
string object_vars() {
  if(sizeof(room_object_names)) {
    return "\nobject *objs;\n\n";
  }
  else {
    return "";
  }
}
 
string dump_objects() {
  int loop1, loop2;
  string str;
 
  loop1 = 0;
  str = "";
  str += "  if(!objs) {\n";
  str += "    objs = allocate(" + sizeof(room_object_names) + ");\n  }\n";
  for(loop1 = 0; loop1 < sizeof(room_object_names); loop1++) {
    str += "/* start of object "+loop1+" - "+room_object_names[loop1]+" */\n";
    str += "  objs[" + loop1 + "] = clone_object(\"" +
           room_object_files[loop1] + "\");\n";
    for(loop2 = 0; loop2 < sizeof(room_object_funcs[loop1]); loop2++) {
      str += "  objs[" + loop1 + "]->" + room_object_funcs[loop1][loop2] +
             ";\n";
    }
    str += "  objs[" + loop1 + "]->move(" +
    this_object()->get_name(room_object_startpos[loop1]) + ");"+
                   " /* " + room_object_startpos[loop1] + " */\n";
    str += "/* end of object " + loop1 + " */\n";
  }
  return str;
}
 
void parse_object(string str) {
  int loop, loop1, tem1;
  string text, start, obj, rest, tem0;
  string *calls;
  string *funcs;

  loop = 0;
  rest = str;
  while(sscanf(rest, "%s/* start of object %d - %s */\n%s"+
                    "/* end of object %d */\n%s", tem0, tem1,
                     object_name, text, tem1, rest) == 6) {

    calls = ({ });
    funcs = explode(text, "\n");
    sscanf(funcs[sizeof(funcs) - 1], "%s/* %s */", tem0, start);
    funcs = delete(funcs, sizeof(funcs) - 1, 1);
    sscanf(funcs[0], "%sclone_object(\"%s\");", tem0, obj);
    funcs = delete(funcs, 0, 1);
    for(loop1 = 0; loop1 < sizeof(funcs); loop1++) {
      string call;

      if(sscanf(funcs[loop1], "  objs["+loop+"]->%s;", call)) {
        calls += ({ call });
      }
    }
    room_object_names += ({ object_name });
    room_object_files += ({ obj });
    room_object_startpos += ({ start });
    room_object_funcs += ({ calls });
  }
}
 
void review_object() {
  list_objects();
}
 
int num_objects() {
  return sizeof(room_object_names);
}
 
