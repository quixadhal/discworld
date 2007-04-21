
/*  This is the plant object thingy.  This is the quite the easy part.   */

inherit "/std/object";

string plant_name, plant_type, plant_desc;

void set_plant(string name) { plant_name = name; add_alias(name);  }
void set_plant_type(string type) { plant_type = type; }
void set_plant_desc(string desc) { plant_desc = desc; }
string query_plant() { return plant_name; }
string query_plant_type() { return plant_type; }

string get_plant_short() {
  if (plant_name && plant_type)
    return plant_type;
  return "plant";
}

string get_plant_long() {
  if (plant_desc)
    return plant_desc;
  return "A lovely plant thingy.\n";
}

void setup() {
  set_name("plant");
  set_short((: get_plant_short() :));
  set_long((: get_plant_long() :));
  add_property("plant",1);
}


mapping query_dynamic_auto_load() {
   return ([
      "::" : ::query_dynamic_auto_load(),
      "plant_name" : plant_name,
      "plant_type" : plant_type,
      "plant_desc" : plant_desc,
   ]);
} /* query_dynamic_auto_load() */


void init_dynamic_arg( mapping map, object bing ) {
   if ( map["::"] )
      ::init_dynamic_arg( map["::"], bing );
   if ( !undefinedp( map["plant_name"] ) ) {
      plant_name = map["plant_name"];
   }
   if( !undefinedp( map["plant_type"] ) ) {
      plant_type = map["plant_type"];
   }
   if( !undefinedp( map["plant_desc"] ) ) {
      plant_desc = map["plant_desc"];
   }
} /* init_dynamic_arg() */