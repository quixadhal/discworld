inherit "/std/basic/property";
inherit "/std/basic/extra_look";
inherit "/std/basic/read_desc";
inherit "/std/basic/misc";
inherit "/std/basic/id";


void set_name(string str) {
  if (name && name != "object") return ;
  name = str;
  if (!short_d)
    short_d = str;
  add_plural(pluralize(str));
}

string create_me;
 
void create() {
  id::create();
  property::create();
  extra_look::create();
  misc::create();
  read_desc::create();
  seteuid((string)"/secure/master"->creator_file(file_name(this_object())));
  create_me = "who knows";
  if(this_player()) { create_me = (string)this_player()->query_name(); }
  this_object()->setup();
}

mixed *stats() {
  return ::stats() + ({
    ({ "name" , name, }),
    ({ "short", short(0), }),
    ({ "plural", query_plural(), }),
    ({ "value", query_value(), }),
    ({ "weight", query_weight(), }),
    ({ "cloned by", create_me, }),
   });
}

int query_alignment() {
  return query_property("alignment");
}

int set_alignment(int al) {
  return add_property("alignment",al);
}

mixed *query_init_data() {
   return
      property::query_init_data() +
      extra_look::query_init_data() +
      read_desc::query_init_data() +
      misc::query_init_data() +
      id::query_init_data();
} /* query_init_data() */

/*
 * This is here till I can delete it...  ie the new system is functioning.
 */
mapping int_query_static_auto_load() {
  return ([ "weight" : weight, "light" : light, "short" : short_d, 
            "long" : long_d, "adjective" : adjectives, "alias" : alias, 
            "plural" : plurals, "weight" : weight, "name" : name, 
            "main plural" : plural_d, "value" : value ]);
} /* query_static_auto_load() */

mapping query_dynamic_auto_load() {
  return ([ "labels" : "/global/player"->create_auto_load(labels),
            "properties" : map_prop,
            "read mess" : read_mess,
            "cloned by" : create_me,
          ]);
} /* query_dynamic_auto_load() */

void init_static_arg(mapping bing) {
  if (!undefinedp(bing["name"]))
    set_name(bing["name"]);
  if (!undefinedp(bing["weight"]))
    set_weight(bing["weight"]);
  if (!undefinedp(bing["light"]))
    set_light(bing["light"]);
  if (!undefinedp(bing["short"]))
    set_short(bing["short"]);
  if (!undefinedp(bing["long"]))
    set_long(bing["long"]);
  if (bing["adjective"])
    set_adjectives(bing["adjective"]);
  if (bing["alias"])
    set_aliases(bing["alias"]);
  if (bing["plural"])
    set_plurals(bing["plural"]);
  if (!undefinedp(bing["main plural"]))
    set_main_plural(bing["main plural"]);
  if (!undefinedp(bing["value"]))
    set_value(bing["value"]);
} /* int_init_static_arg() */

mixed query_static_auto_load() {
  if (file_name(this_object())[0..10] == "/std/object")
    return int_query_static_auto_load();
  return ([ ]);
} /* query_static_auto_load() */

void init_dynamic_arg(mapping args) {
  if (args["labels"])
    "/global/player"->load_auto_load(args["labels"], this_object());
  if (args["properties"])
    map_prop = args["properties"];
  set_read_mess(args["read mess"]);
  create_me = args["cloned by"];
} /* init_dynamic_arg() */

void init_arg(mixed *bing) {
  if (sizeof(bing) < 10)
    return ;
  weight = bing[0];
  light = bing[1];
  short_d = bing[2];
  long_d = bing[3];
  set_adjectives(bing[4]);
  set_aliases(bing[5]);
  set_plurals(bing[6]);
  weight = bing[8];
  set_name(bing[9]);
  map_prop = bing[10];
  value = bing[12];
} /* init_arg() */
