inherit "obj/roomgen/support";
 
string *room_aliases, *room_alias_meanings;
static string ralias;
static int pos;
 
#define ALIAS_HELP "/doc/roomgen/alias_help"
#define ALIAS_EXAMPLE "/doc/roomgen/alias_example"
 
void init_alias_data() {
  room_aliases = ({ });
  room_alias_meanings = ({ });
}
 
void alias_menu();
 
 
/* Adding alias to room code */
 
 
void add_room_alias() {
  output("What will the alias be (? for help) : ");
  input_to("add_alias2");
}
 
void add_alias2(string str) {
  if(str == "") {
    alias_menu();
  }
  if(str == "?") {
    cat(ALIAS_EXAMPLE);
    add_room_alias();
    return;
  }
  if(member_array(str, room_aliases) != -1) {
    output("That alias already exists.  Please choose another direction or "+
           "delete the alias that currently exists.\n");
    alias_menu();
  }
  ralias = str;
  output("What will the alias be for (? for help) : ");
  input_to("add_alias3");
}
 
void add_alias3(string str) {
  if(str == "") {
    alias_menu();
  }
  if(str == "?") {
    cat(ALIAS_EXAMPLE);
    output("What will the alias be for (? for help) : ");
    input_to("add_alias3");
    return;
  }
  room_aliases += ({ ralias });
  room_alias_meanings += ({ str });
  output("alias " + ralias + " added.\n\n");
  alias_menu();
}
 
 
/* Listing alias code */
 
 
void list_aliases() {
  if(sizeof(room_aliases)) {
    int loop;
 
    output("Alias, Meaning\n");
    for(loop = 0; loop < sizeof(room_aliases); loop++) {
      output(room_aliases[loop] + ", " + room_alias_meanings[loop] + "\n");
    }
  }
  else {
    output("No aliases defined.\n");
  }
}

void list_room_aliases() {
  list_aliases();
  get_key("alias_menu");
}
 
 
/* Editing alias code */
 
 
void edit_alias_menu() {
  if(!sizeof(room_aliases)) {
    output("No aliases!\n");
    alias_menu();
    return;
  }
  output("edit which alias : ");
  input_to("edit_alias_menu2");
}
 
void edit_alias_menu3();
 
void edit_alias_menu2(string str) {
  if((pos = member_array(str, room_aliases)) == -1) {
    output("No such alias!\n");
    alias_menu();
    return;
  }
  edit_alias_menu3();
}
 
void edit_alias_menu3() {
  output("Edit which field?\n"+
         "   1. alias\n"+
         "   2. meaning\n"+
         "   end - back to alias menu\n"+
         "   help - help on aliases\n"+
         "Selection : ");
  input_to("edit_alias_menu4");
}
 
void edit_alias_menu4(string str) {
 
  switch(str) {
    case "1":
      output("new alias : ");
      input_to("get_new_alias");
      return;
    case "2":
      output("new meaning : ");
      input_to("get_new_meaning");
      return;
    case "end":
      alias_menu();
      return;
    case "help":
      cat(ALIAS_EXAMPLE);
      get_key("edit_alias_menu3");
      return;
    case "":
      edit_alias_menu3();
      return;
    default:
      output("Bad Selection\n");
      edit_alias_menu3();
      return;
  }
}
 
void get_new_alias(string str) {
  string old;
 
  if(str == "") {
    edit_alias_menu3();
  }
  old = room_aliases[pos];
  room_aliases[pos] = str;
  output("alias modified from " + old + " " + str + "\n");
  edit_alias_menu3();
}
 
void get_new_meaning(string str) {
  string old;
 
  if(str == "") {
    edit_alias_menu3();
  }
  old = room_alias_meanings[pos];
  room_alias_meanings[pos] = str;
  output("alias meaning modified from " + old + " " + str + "\n");
  edit_alias_menu3();
}
 
 
/* delete alias code */
 
 
void delete_room_alias() {
  output("Delete which alias : ");
  input_to("delete_alias2");
}
 
void delete_alias2(string str) {
  if(str == "") {
    alias_menu();
    return;
  }
  if((pos = member_array(str, room_aliases)) == -1) {
    output("No such alias!\n");
    alias_menu();
    return;
  }
  output("Delete alias " + str + ".  Are you sure? (Y/N) :");
  input_to("confirm_delete_alias");
}
 
void confirm_delete_alias(string str) {
  string rm_name;
 
  if(lower_case(str)[0] != 'y') {
    output("Alias not deleted!\n");
    alias_menu();
  }
  rm_name = room_aliases[pos];
  room_aliases = delete(room_aliases, pos, 1);
  room_alias_meanings = delete(room_alias_meanings, pos, 1);
  output("Deleted alias " + rm_name + "\n");
  alias_menu();
}
 
 
void alias_menu() {
  output("   1. add an alias for the room\n"+
         "   2. list aliases for the room\n"+
         "   3. edit alias for the room\n"+
         "   4. delete alias for the room\n"+
         "   help - help on aliases\n"+
         "   end - return to main menu\n"+
         "Selection : ");
  input_to("handle_alias_menu");
}
 
void handle_alias_menu(string str) {
  switch(str) {
    case "1":
      add_room_alias();
      return;
    case "2":
      list_room_aliases();
      return;
    case "3":
      edit_alias_menu();
      return;
    case "4":
      delete_room_alias();
      return;
    case "end":
      this_object()->main_menu();
      return;
    case "help":
      cat(ALIAS_HELP);
      get_key("alias_menu");
      return;
    case "":
      alias_menu();
      return;
    default:
      output("Invalid Choice.\nSelection : ");
      input_to("handle_alias_menu");
  }
}
 
string dump_aliases() {
  string str;
  int loop;
 
  str = "";
  for(loop = 0; loop < sizeof(room_aliases); loop++) {
    str += "  add_alias(\"" +
           room_aliases[loop] + "\", \"" +
           room_alias_meanings[loop] + "\");\n";
  }
  return str;
}
 
void parse_alias(string str) {
  string temp1, temp2, temp3, temp4, temp5, temp6;
  int tempint1, tempint2, tempint3;

  if(sscanf(str, "%svoid setup() {%s", temp1, temp2) == 2) {
    str = temp2;
  }
  else {
    return;
  }
  while(sscanf(str, "%sadd_alias(\"%s\", \"%s\");\n%s", temp1, temp2,
                    temp3, temp4) == 4) {
    str = temp4;
    if(!str) {
      str = "";
    }
    room_aliases += ({ temp2 });
    room_alias_meanings += ({ temp3 });
  }
}

void review_alias() {
  list_aliases();
}
