/**
 * The todo list handler thingy.  A small command to deal with nice little
 * todo lists.<p>
 *
 * NB: This assumes that the creator has a ~/save directory.
 *
 * @author Pinkfish
 * @started Thu Mar 25 21:15:16 PST 1999
 */
inherit "/cmds/base";

class todo_item {
   string type;
   int id;
   int priority;
   string description;
   int time_added;
   int time_to_finish;
}

#define TODO_PRIVATE 0x8000
#define TODO_PRIORITY_MASK (~TODO_PRIVATE)

private mapping todo_lists;
private mapping changed;
private int save_callout_id;

void create() {
   ::create();
   todo_lists = ([ ]);
   changed = ([ ]);
   seteuid("Root");
} /* create() */

private class todo_item *query_todo_list(string player) {
   string data;
   class todo_item *stuff;
   class todo_item *new_stuff;
   class todo_item item;
   class todo_item new_item;

   if (todo_lists[player]) {
      return todo_lists[player];
   }

   /* Read it from the disk... */
   data = read_file("/w/" + player + "/save/todo_list.o");
   if (!data) {
      return ({ });
   }
   stuff = restore_variable(data);
   if (!stuff) {
      return ({ });
   }

   /* Old type, upgrade it. */
   if (sizeof(stuff) &&
       sizeof(stuff[0]) != 6) {
      new_stuff = ({ });
      foreach (item in stuff) {
         new_item = new(class todo_item);
         new_item->priority = item->priority;
         new_item->type = item->type;
         new_item->description = item->description;
         new_item->id = item->id;
         new_item->time_added = time();
         new_item->time_to_finish = time();
         new_stuff += ({ new_item });
      }
      stuff = new_stuff;
   }
   return stuff;
} /* query_todo_list() */

private void save_todo_lists() {
   int times;
   string player;
   string data;

   save_callout_id = 0;
   foreach (player, times in changed) {
      data = save_variable(todo_lists[player]);
      write_file("/w/" + player + "/save/todo_list.o", data, 1);
   }
   changed = ([ ]);
   todo_lists = ([ ]);
} /* save_todo_lists() */

private void set_todo_list(string player, class todo_item *data) {
   todo_lists[player] = data;
   changed[player] = time();
   if (!save_callout_id) {
      save_callout_id = call_out((: save_todo_lists :), 120);
   }
} /* set_todo_list() */

private int query_next_id(string player) {
   class todo_item *data;
   class todo_item item;
   int id;

   data = query_todo_list(player);
   id = 1;
   foreach (item in data) {
      if (item->id >= id) {
         id = item->id + 1;
      }
   }
   return id;
} /* query_next_id() */

string query_priority_name(int priority) {
   string extra;

   extra = " ";
   if (priority & TODO_PRIVATE) {
      extra = "P";
   }
   switch (priority & TODO_PRIORITY_MASK) {
      case 0 :
         return "VL" + extra;
      case 1 :
         return " L" + extra;
      case 2 :
         return " H" + extra;
      case 3 :
         return "VH" + extra;
      case 4 :
         return " U" + extra;
      default :
         return " F" + extra;
   }
} /* query_priority_name() */

int query_priority_number(string priority) {
   int mask;

   priority = lower_case(priority);
   mask = 0;
   if (priority[<1] == 'p') {
      mask = TODO_PRIVATE;
      priority = priority[0..<2];
   }
   switch (priority) {
      case "vl" :
         return 0 | mask;
      case "l" :
         return 1 | mask;
      case "h" :
         return 2 | mask;
      case "vh" :
         return 3 | mask;
      case "u" :
         return 4 | mask;
   }
   return -1;
} /* query_priority_number() */

string query_todo_line(class todo_item item, int brief, int cols) {
  if(brief) {
  return sprintf("%s%-=*s\n",
                  query_priority_name(item->priority),
                  cols - 3, 
                  item->description);
  }
  
  return sprintf("#%3d %s (%s) Added at %s\n    %-=*s\n",
                  item->id,
                  query_priority_name(item->priority),
                  item->type,
                  ctime(item->time_added),
                  cols - 4, 
                  item->description);
} /* query_todo_line() */

int sort_function(class todo_item bing,
                  class todo_item bing2) {
   if ((bing->priority & TODO_PRIORITY_MASK) == (bing2->priority & TODO_PRIORITY_MASK)) {
      if (bing->type == bing2->type) {
         return bing->id - bing2->id;
      }
      return strcmp(bing->type, bing2->type);
   }
   return (bing2->priority & TODO_PRIORITY_MASK) - (bing->priority & TODO_PRIORITY_MASK);
} /* sort_function() */

/**
 * Sort it in priority order, then in order of type, then id.
 */
class todo_item *sort_todo_list(class todo_item *data) {
   return sort_array(data, (: sort_function :));
} /* sort_list() */

string query_todo_list_string(class todo_item *items, int brief) {
   class todo_item bing;
   string data;

   data = "";
   if (sizeof(items)) {
      foreach (bing in items) {
         data += query_todo_line(bing, brief, this_player()->query_cols());
      }
   }
   return data;
} /* query_todo_list_string() */

private int list_todo(int brief) {
   class todo_item* items;
   string data;

   items = query_todo_list(this_player()->query_name());
   if (sizeof(items)) {
     data = query_todo_list_string(items, brief);
   } else {
      data = "No items in your todo list.\n";
   }
   write("$P$Todo list$P$" + data);
   add_succeeded_mess("");
   return 1;
} /* list_todo() */

private int list_todo_creator(string player) {
   class todo_item* items;
   string data;

   items = query_todo_list(player);
   if (sizeof(items)) {
      items = filter(items, (: !($1->priority & TODO_PRIVATE) :));
      data = query_todo_list_string(items, 0);
   } else {
      data = "No items in " + capitalize(player) + "'s todo list.\n";
   }
   write("$P$Todo list$P$" + data);
   add_succeeded_mess("");
   return 1;
} /* list_todo() */

private int list_todo_priority(int priority) {
   class todo_item *items;
   string data;

   items = query_todo_list(this_player()->query_name());
   items = filter(items, (: ($1->priority & TODO_PRIORITY_MASK) == $(priority) :) );
   if (sizeof(items)) {
      data = query_todo_list_string(items, 0);
   } else {
      data = "No items in your todo list with the specified priority.\n";
   }
   write("$P$Todo list$P$" + data);
   add_succeeded_mess("");
   return 1;
} /* list_todo_priority() */

private int list_todo_type(string type) {
   class todo_item *items;
   string data;

   type = lower_case(type);
   items = query_todo_list(this_player()->query_name());
   items = filter(items, (: $1->type == $(type) :) );
   if (sizeof(items)) {
      data = query_todo_list_string(items, 0);
   } else {
      data = "No items in your todo list with a type of " + type + ".\n";
   }
   write("$P$Todo list$P$" + data);
   add_succeeded_mess("");
   return 1;
} /* list_todo_type() */

private int add_item(string priority, string type, string description) {
   int priority_num;
   class todo_item frog;
   class todo_item *data;

   priority_num = query_priority_number(priority);
   if (priority_num == -1) {
      add_failed_mess("Unknown priority '" + priority +
                      "', only VL, L, H, VH and U allowed.\n");
      return 0;
   }

   if (strlen(type) > 10) {
      add_failed_mess("The type cannot be more than 10 characters long.\n");
      return 0;
   }

   /* Check to make sure the directories exist and stuff */
   if (file_size("/w/" + this_player()->query_name() + "/save") != -2) {
      add_failed_mess("There is no save directory in your home directory.  "
                      "This is needed for operation of this command.\n");
      return 0;
   }

   frog = new(class todo_item);
   frog->priority = priority_num;
   frog->description = description;
   frog->type = lower_case(type);
   frog->id = query_next_id(this_player()->query_name());
   frog->time_added = time();
   // default to 1 week finish time.
   frog->time_to_finish = time() + (60 * 60 * 24) * 7;

   data = query_todo_list(this_player()->query_name());
   data += ({ frog });
   data = sort_todo_list(data);
   set_todo_list(this_player()->query_name(), data);
   add_succeeded_mess(({ "You added a new item #" + frog->id +
                         " to your todo list.\n", "" }));
   return 1;
} /* add_item() */

private void confirm_complete_item(string response, int id) {
   class todo_item *data;
   class todo_item item;

   response = lower_case(response);
   if (!strlen(response) || response[0] != 'y') {
      write("Ok, canceling the completion of the item.\n");
      return ;
   }
   data = query_todo_list(this_player()->query_name());
   foreach (item in data) {
      if (item->id == id) {
         data -= ({ item });
         set_todo_list(this_player()->query_name(), data);
         write("Ok, completed list id #" + id + ".\n");
         return ;
      }
   }
   write("Unable to find the item.  Someone must have run off with it!\n");
   return ;
} /* confirm_complete_list() */

class todo_item find_todo_id(class todo_item *data, int id) {
   class todo_item item;

   foreach (item in data) {
      if (item->id == id) {
         return item;
      }
   }
   return 0;
} /* find_todo_id() */

private int complete_item(int id) {
   class todo_item *data;
   class todo_item item;

   data = query_todo_list(this_player()->query_name());
   item = find_todo_id(data, id);
   if (item) {
      write(query_todo_line(item, 0, this_player()->query_cols()) +
            "Are you sure you wish to complete this todo list item? ");
      input_to((: confirm_complete_item :), 0, id);
      return 1;
   }
   add_failed_mess("Unable to find an item of id #" + id + " to complete.\n");
   return 0;
} /* complete_item() */

private int change_priority(int id, string new_priority) {
   class todo_item *data;
   class todo_item item;
   int priority_num;

   priority_num = query_priority_number(new_priority);
   if (priority_num == -1) {
      add_failed_mess("Unknown priority '" + new_priority +
                      "', only VL, L, H, VH and U allowed.\n");
      return 0;
   }

   data = query_todo_list(this_player()->query_name());
   item = find_todo_id(data, id);
   if (item) {
      item->priority = priority_num;
      add_succeeded_mess(({ "Change the priority of #" + id +
                            " to " + new_priority + ".\n", "" }));
      set_todo_list(this_player()->query_name(), data);
      return 1;
   }
   add_failed_mess("Unable to find an item of id #" + id +
                   " to change the priority of.\n");
   return 0;
} /* change_priority() */

private int change_description(int id, string new_description) {
   class todo_item *data;
   class todo_item item;

   data = query_todo_list(this_player()->query_name());
   item = find_todo_id(data, id);
   if (item) {
      item->description = new_description;
      set_todo_list(this_player()->query_name(), data);
      return 1;
   }
   add_failed_mess("Unable to find an item of id #" + id +
                   " to change the description of.\n");
   return 0;
} /* change_description() */

mixed *query_patterns() {
   return ({
             "", (: list_todo(1) :),
             "list", (: list_todo(0) :),
             "list type <string'type'>", (: list_todo_type($4[0]) :),
             "list urgent", (: list_todo_priority(4) :),
             "list very high", (: list_todo_priority(3) :),
             "list high", (: list_todo_priority(2) :),
             "list low", (: list_todo_priority(1) :),
             "list very low", (: list_todo_priority(0) :),
               "list brief", (: list_todo(1) :), 
             "creator <string'name'>", (: list_todo_creator($4[0]) :),
             "add <word'priority'> <word'type'> <string'description'>",
                   (: add_item($4[0], $4[1], $4[2]) :),
             "complete <number'todo id'>", (: complete_item($4[0]) :),
             "change priority <number'todo it'> <word'priority'>",
                   (: change_priority($4[0], $4[1]) :),
             "change description <number'todo id'> <string'description'>",
                   (: change_description($4[0], $4[1]) :),
           });
} /* query_patterns() */

void dest_me() {
   save_todo_lists();
   ::dest_me();
} /* dest_me() */
