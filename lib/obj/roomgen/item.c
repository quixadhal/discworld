inherit "obj/roomgen/support";
 
string *room_items, *room_item_descs;
string *new_item_desc;
static int pos;
 
#define ITEM_HELP "/doc/roomgen/item_help"
#define ITEM_EXAMPLE "/doc/roomgen/item_example"
 
void init_item_data() {
  room_items = ({ });
  room_item_descs = ({ });
}
 
void item_menu();
 
 
/* Adding item to room code */
 
 
void add_item() {
  output("What will the item be (? for help) : ");
  input_to("add_item2");
}
 

void add_item_description() {
  output("Enter the description of the item ** to end ~q to abort.\n");
  new_item_desc = ({ });
  write("] ");
  input_to("add_item3");
}


void add_item2(string str) {
  if(str == "") {
    item_menu();
  }
  if(str == "?") {
    cat(ITEM_EXAMPLE);
    add_item();
    return;
  }
  if(member_array(str, room_items) != -1) {
    output("That item already exists.  Please choose another direction or "+
           "delete the item that currently exists.\n");
    item_menu();
  }
  room_items += ({ str });
  pos = sizeof(room_items) - 1;
  room_item_descs += ({ "No description" });
  add_item_description();
}

void add_item3(string str) {
  if(str == "~q") {
    output("Item description for " + room_items[pos] + " now reads :\n");
    output(room_item_descs[pos]);
    item_menu();
    return;
  }
  if(str == "**") {
    if(!sizeof(new_item_desc)) {
      add_item3("~q");
      return;
    }
    new_item_desc[sizeof(new_item_desc) - 1] += " + \"\\n\"\n";
    room_item_descs[pos] = implode(new_item_desc, " +\n");
    output("Item description for " + room_items[pos] + " now reads :\n");
    output(room_item_descs[pos]);
    item_menu();
    return;
  }
  if(sizeof(new_item_desc)) {
    new_item_desc += ({ "\" " + str + "\"" });
  }
  else {
    new_item_desc += ({ "\"" + str + "\"" });
  }
  write("] ");
  input_to("add_item3");
}


/* Listing item code */
 
 
void list_items() {
  if(sizeof(room_items)) {
    int loop;
 
    output("item, Meaning\n");
    for(loop = 0; loop < sizeof(room_items); loop++) {
      output(room_items[loop] + ", " + room_item_descs[loop] + "\n");
    }
  }
  else {
    output("No items defined.\n");
  }
}

void list_room_items() {
  list_items();
  get_key("item_menu");
}
 
 
/* Editing item code */
 
 
void edit_item_menu() {
  if(!sizeof(room_items)) {
    output("No items!\n");
    item_menu();
    return;
  }
  output("edit which item : ");
  input_to("edit_item_menu2");
}
 
void edit_item_menu3();
 
void edit_item_menu2(string str) {
  if((pos = member_array(str, room_items)) == -1) {
    output("No such item!\n");
    item_menu();
    return;
  }
  edit_item_menu3();
}
 
void edit_item_menu3() {
  output("Edit which field?\n"+
         "   1. item\n"+
         "   2. description\n"+
         "   end - back to item menu\n"+
         "   help - help on items\n"+
         "Selection : ");
  input_to("edit_item_menu4");
}
 
void edit_item_menu4(string str) {
 
  switch(str) {
    case "1":
      output("new item : ");
      input_to("get_new_item");
      return;
    case "2":
      add_item_description();
      return;
    case "end":
      item_menu();
      return;
    case "help":
      cat(ITEM_EXAMPLE);
      get_key("edit_item_menu3");
      return;
    case "":
      edit_item_menu3();
      return;
    default:
      output("Bad Selection\n");
      edit_item_menu3();
      return;
  }
}
 
void get_new_item(string str) {
  string old;
 
  if(str == "") {
    edit_item_menu3();
  }
  old = room_items[pos];
  room_items[pos] = str;
  output("item modified from " + old + " " + str + "\n");
  edit_item_menu3();
}
 
 
/* delete item code */
 
 
void delete_item() {
  output("Delete which item : ");
  input_to("delete_item2");
}
 
void delete_item2(string str) {
  if(str == "") {
    item_menu();
    return;
  }
  if((pos = member_array(str, room_items)) == -1) {
    output("No such item!\n");
    item_menu();
    return;
  }
  output("Delete item " + str + ".  Are you sure? (Y/N) :");
  input_to("confirm_delete_item");
}
 
void confirm_delete_item(string str) {
  string rm_name;
 
  if(lower_case(str)[0] != 'y') {
    output("item not deleted!\n");
    item_menu();
  }
  rm_name = room_items[pos];
  room_items = delete(room_items, pos, 1);
  room_item_descs = delete(room_item_descs, pos, 1);
  output("Deleted item " + rm_name + "\n");
  item_menu();
}
 
 
void item_menu() {
  output("   1. add an item for the room\n"+
         "   2. list items for the room\n"+
         "   3. edit item for the room\n"+
         "   4. delete item for the room\n"+
         "   end - return to main menu\n"+
         "   help - help on items\n"+
         "Selection : ");
  input_to("handle_item_menu");
}
 
void handle_item_menu(string str) {
  switch(str) {
    case "1":
      add_item();
      return;
    case "2":
      list_room_items();
      return;
    case "3":
      edit_item_menu();
      return;
    case "4":
      delete_item();
      return;
    case "end":
      this_object()->main_menu();
      return;
    case "help":
      cat(ITEM_HELP);
      get_key("item_menu");
      return;
    case "":
      item_menu();
      return;
    default:
      output("Invalid Choice.\nSelection : ");
      input_to("handle_item_menu");
  }
}
 
string dump_items() {
  string str;
  int loop;
 
  str = "";
  for(loop = 0; loop < sizeof(room_items); loop++) {
    str += "  add_item(\"" +
           room_items[loop] + "\",\n " +
           room_item_descs[loop] + ");\n";
  }
  return str;
}
 
void parse_item(string str) {
  string temp1, temp2, temp3, temp4;

  if(sscanf(str, "%svoid setup() {%s", temp1, temp2) == 2) {
    str = temp2;
  }
  else {
    return;
  }
  while(sscanf(str, "%sadd_item(\"%s\",\n%s);\n%s", temp1, temp2,
                    temp3, temp4) == 4) {
    str = temp4;
    if(!str) {
      str = "";
    }
    room_items += ({ temp2 });
    room_item_descs += ({ temp3 });
  }
}

void review_item() {
}
