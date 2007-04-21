#include <housing.h>

inherit "/cmds/base";

#include <armoury.h>

int request(string item, int condition, string area) {
  object ob;

  ob = ARMOURY->request_item(item, condition, area);
  if(!ob) 
    return notify_fail(item + " not found.\n");

  ob->move(this_player());
  this_player()->add_succeeded_mess(ob, "$N $V $D\n", ({ }));
  log_file ("REQUEST", "%s: %s requested %s.\n",
    ctime (time()), this_player()->query_name(), item);
  return 1;
}

int list(string type, string filter) {
  mapping stuff;

  stuff = ARMOURY->query_items(type, filter);
  if(!sizeof(stuff))
     return this_player()->add_failed_mess(this_object(),
                                           "No such list.\n", ({ }));
  

  printf("Available " + type +" objects are:\n%-*#s\n",
         (int)this_player()->query_cols(),
         implode(keys(stuff), "\n"));
  this_player()->add_succeeded_mess(this_object(), "", ({ }));
  return 1;
}

int path(string item, int condition, string area) {
  object ob;
  string res;
  
  ob = ARMOURY->request_item(item, condition, area);
  if(!ob) 
    return notify_fail("");
  
  if (ob->query_property("virtual name")) {
    res = "Virtual file for " + ob->the_short() +
      ":\n   " + ob->query_property("virtual name") + "\n";
  } else {
    res = "Path of " + ob->the_short() + ":\n   " + base_name(ob) + "\n";
  }
  ob->move("/room/rubbish");
  this_player()->add_succeeded_mess(ob, res, ({ }));
  return 1;
}

mixed *query_patterns() {
  return ({ "list <word'" + implode(ARMOURY->query_types(), "|") + "'>",
              (: list($4[0], "") :),
"list <word'" + implode(ARMOURY->query_types(), "|") + "'> <string'filter'>",
              (: list($4[0], $4[1]) :),             
              "<word'area'> <string'item'>", (: request($4[1], 100, $4[0]) :),
              "<word'area'> <string'item'> <number'condition'>", (: request($4
[1], $4[2], $4[0]) :),
              "<string'item'>", (: request($4[0], 100, "") :),
              "<string'item'> <number'condition'>", (: request($4[0], $4
[1], "") :),
              "path <string'item'>", (: path($4[0], 100, "") :),
              "path <word'area'> <string'item'>", (: path($4[1], 100, $4
[0]) :),
        });
} /* query_patterns() */
