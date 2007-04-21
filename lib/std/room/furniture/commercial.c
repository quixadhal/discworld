/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: commercial.c,v 1.25 2003/06/07 01:37:30 pinkfish Exp $
 *
 *
 */
/**
 * This is a fucniture object for makeing all kinds of commercial
 * furniture.
 * @author Pinkfish
 * @started Sat Jun  2 16:27:27 PDT 2001
 */
#include <move_failures.h>
#include <position.h>
#include <money.h>

inherit "/std/object";
inherit "/std/basic/item_chat";
inherit "/std/room/furniture/inherit/furniture_base";

string query_main_status(int hints);

private nosave int _commercial_size;
private nosave string _commercial_type;
private nosave string _commercial_name;
private nosave string* _use_types;
private nosave string _info;
private int _revenue;
private int _weekly_revenue;
private int _commercial_id;
private int _weekly_upkeep_cost;

#define OBJECT_TAG "commercial inh"

void create() {
   _commercial_size = 100;
   if (!_use_types) {
      _use_types = ({ });
   }
   do_setup++;
   object::create();
   furniture_base::create();
   add_alias( "furniture" );
   add_plural( "furniture" );
   add_property("talisman", "no one");
   set_opaque();
   if ( !query_property( "shop type" ) ) {
      add_property( "shop type", "furniture" );
   }

   do_setup--;
   if ( !do_setup ) {
     this_object()->setup();
   }
   // Don't respond to 'get all'
   set_no_all(1);
   reset_get();
} /* create() */

/**
 * This method sets the weekly upkeep cost for the furniture item.
 */
void set_weekly_upkeep_cost(int cost) {
   _weekly_upkeep_cost = cost;
}

/**
 * This method sets the weekly upkeep cost for the furniture item.
 */
int query_weekly_upkeep_cost() {
   return _weekly_upkeep_cost;
}

/**
 * This method returns the revenue for this item.
 */
int query_revenue() {
   return _revenue;
} /* query_revenue() */

/**
 * This method sets the current revenue of the item.
 * @param revenue the new revenue of the object
 */
void set_revenue(int revenue) {
   _revenue = revenue;
} /* set_revenue() */

/**
 * This method returns the save directory of the outside room.
 * @return the save directory
 */
string query_save_dir() {
   return environment()->query_save_dir();
} /* query_save_dir() */

/**
 * This method returns the owner of the establishment.
 * @return the owener of the establishment
 */
string query_owner() {
  return environment()->query_owner();
} /* query_owner() */

/**
 * This method returns the owner of the establishment.
 * @return the owener of the establishment
 */
string query_language() {
  return environment()->query_language();
} /* query_owner() */

/**
 * This method returns the money place of the object.
 * @return the money place
 */
string query_money_place() {
   if (!environment()) {
      return "default";
   }
   return environment()->query_property("place");
} /* query_money_place() */

/**
 * This method checks to see if the specified person is allowed
 * to use the room or not.
 * @param person the person to check
 */
int is_allowed(string person) {
   return environment()->is_allowed(person);
} /* is_allowed() */

/**
 * This method returns the list of people that are allowed.
 * @return the people that are allowed
 */
string query_allowed() {
   return environment()->query_allowed();
} /* query_allowed() */

/**
 * This method returns the float in the current room.
 * @return the float
 */
int query_float() {
   return environment()->query_royalty(query_owner());
} /* query_float() */

/**
 * This method returns the royalties for the specific person.
 * @param person the person to get the royalty of
 * @return the royalty
 */
int query_royalty(string person) {
   return environment()->query_royalty(person);
} /* query_royalty() */

/**
 * This method adjusts the royalty for the specific person.
 * @param person the person to adjust the royalty of
 * @param amount the amount to adjust it by
 */
int adjust_royalty(string person, int amount) {
   if (person == query_owner()) {
      _revenue += amount;
      _weekly_revenue += amount;
   }
   environment()->adjust_royalty(person, amount);
} /* adjust_royalty() */

/**
 * This method adjusts the float of the current room.
 * @param amt the amount to adjust by
 */
int adjust_float(int amt) {
   adjust_royalty(query_owner(), amt);
} /* adjust_float() */

/**
 * This method checks to see if the place is open for the spefic type of
 * activity.
 * @param type the type of open to check rfor
 * @param person the person to check for
 */
int is_open_for(string type, string person) {
   if (!environment()->query_commercial_property()) {
      return 0;
   }
   return environment()->is_open_for(type, person);
} /* is_open_for() */

/**
 * This method sets the size of the item in terms of commericial
 * property.  100 is very large and takes up all of a large
 * commercial property.  For example, a 100 item shop will be
 * size 100.
 * @param size the new size of the item
 */
void set_commercial_size(int size) {
   _commercial_size = size;
} /* set_commercial_size() */

/**
 * This method returns the type of the commericial property.
 * @return the type of the commercial property
 */
string query_commercial_type() {
   return _commercial_type;
} /* query_commercial_type() */

/**
 * This method sets the type of the item in terms of commericial
 * property.
 * @param type the new type of the item
 */
void set_commercial_type(string type) {
   _commercial_type = type;
} /* set_commercial_type() */

/**
 * THis method sets any extra information about the shop to be told
 * when someone does a browse.
 * @param info the infomation
 */
void set_commercial_information(string info) {
   _info = info;
} /* set_commercial_information() */

/**
 * This method returns the extra information about the shoip to be used
 * in the browse.
 * @return the extra inforation
 */
string query_commercial_information() {
   return _info;
} /* query_commercial_information() */

/**
 * This method sets the name of the item for use in finding this specific
 * type of item in a room.
 * @param name the name
 * @see /std/shops/inherit/commercial->find_commercial_items()
 */
void set_commercial_name(string name) {
   _commercial_name = name;
} /* set_commercial_information() */

/**
 * This method returns the name of the item for use in finding this specic
 * type of item in a room.
 * @return the extra inforation
 * @see /std/shops/inherit/commercial->find_commercial_items()
 */
string query_commercial_name() {
   return _commercial_name;
} /* query_commercial_name() */

/**
 * This method returns the size of the commericial property.
 * @return the size of the commercial property
 */
int query_commercial_size() {
   return _commercial_size;
} /* query_commercial_size() */

/**
 * This returns a unique id for the commercial item.  This id will be
 * unique within the room*
 * @return a unique id
 */
int query_commercial_id() {
   return _commercial_id;
} /* query_commercial_id() */

/**
 * This method sets up a new commercial id for the place.
 */
void setup_new_commercial_id() {
   _commercial_id = environment()->find_next_commercial_id();
} /* setup_new_commercial_id() */

/**
 * This method sets the commercial id for the item.
 * @param id the unique id to use
 */
void set_commercial_id(int id) {
   _commercial_id = id;
} /* set_commercial_id() */

/**
 * This method returns the parcel post room defined for the room.
 * @return the parcel post room defined for the room
 */
string query_parcel_post() {
   return environment()->query_parcel_post();
} /* query_parcel_post() */

/**
 * This method sets the types which this item will use for checking.
 * @param types this sets the types for the shop
 */
void set_shop_use_types(string* types) {
   _use_types = types;
} /* set_shop_use_types() */

/**
 * This method returns the types of thingies for the commercial property.
 * @return the shop types
 */
string* query_shop_use_types() {
   return _use_types;
} /* query_shop_use_types() */

/**
 * This resets the weekly statistics for the object.
 */
void reset_weekly_status() {
   _weekly_revenue = 0;
} /* reset_weekly_status() */

/**
 * This method returns the weekly status for this furniture object.
 * @return the weekly status for the this object
 */
string query_weekly_status_string() {
   string ret;
   string place;

   place = query_money_place();
   if (query_weekly_upkeep_cost()) {
      ret = "Weekly Upkeep : " +
            MONEY_HAND->money_value_string(_weekly_upkeep_cost, place) + "\n" +
            "Weekly Revenue: " +
            MONEY_HAND->money_value_string(_weekly_revenue, place) + "\n" +
            "Profit        : " +
            MONEY_HAND->money_value_string(_weekly_revenue - _weekly_upkeep_cost, place) + "\n";
   } else {
      ret = "Weekly Revenue: " +
            MONEY_HAND->money_value_string(_weekly_revenue, place) + "\n";
   }
   return ret;
} /* query_weekly_status_string() */

/**
 * This method checks to see if the furniture can be trashed or not.  This
 * will check to make sure the item can be trashed or not.  The item should
 * be unable to be trashed if it still contains some player data of
 * some sort.
 * @return 1 if it can be trashed, 0 if not
 */
int can_trash_furniture() {
   return 1;
} /* can_trash_furniture() */

/**
 * This method checks to see if the item is a commercial
 * furniture item.
 * @return 1 if the item is commercial furniture
 */
int query_commercial_furniture() {
   return 1;
} /* query_commercial_furniture() */

/* Furniture is much harder to carry than just its weight, therefore we
 * multiply its weight by 5 to determine if someone can carry it.
 */
/** @ignore yes */
varargs int move( mixed dest, string messin, string messout ) {
   int i;
   int t;
   int f;
   object from;

   if(stringp(dest)) {
      dest = find_object(dest);
   }

   if (!move_check(dest)) {
      write(the_short() + " resists being picked up and shocks you slightly.\n");
      return MOVE_INVALID_DEST;
   }

   if(!dest) {
      return MOVE_INVALID_DEST;
   }

   if(living(dest)) {
      t = (int)this_object()->query_complete_weight() * 5;
      if(!(dest->add_weight(t))) {
         return MOVE_TOO_HEAVY;
      }
      debug_printf("Added %d", t);
   }

   from = environment();
   i = ::move(dest, messin, messout);

   if(i != MOVE_OK) {
      dest->add_weight(-t);
      return i;
   }

   if(from && living(from)) {
      f = (int)this_object()->query_complete_weight() * 5;
      from->add_weight(-f);
   }

   if (from) {
      from->remove_commercial_thing(this_object());
   }
   dest->add_commercial_thing(this_object());
   dest->register_use_types(_use_types);

   return i;
} /* move() */

/** @ignore yes
 * This stops people dragging items away if they aren't allowed to.
 */
int get(mixed dest) {
   if(!move_check(dest)) {
      write("You feel it would be wrong to take " + the_short() + ".\n");
      return 2;
   }
   return ::get(dest);
} /* get() */

/** @ignore yes */
int do_status(int hint) {
   string ret;

   ret = query_main_status(hint);
   write("$P$Status$P$" + ret);
   add_succeeded_mess("$N get$s the status of $D.\n");
   return 1;
} /* do_status() */

/** @ignore yes */
int do_reset() {
   string place;
   int old_revenue;

   place = query_money_place();
   old_revenue = _revenue;
   _revenue = 0;
   add_succeeded_mess("$N clear$s the revenue of " +
                      MONEY_HAND->money_value_string(old_revenue, place) +
                      " on $D.\n");
   return 1;
} /* do_reset() */

void init() {
   ::init();
 
   if (is_allowed(this_player()->query_name())) {
      add_command("status", "<direct:object>", (: do_status, 0 :));
      add_command("status", "hints <direct:object>", (: do_status, 1 :));
      add_command("status", "hint <direct:object>", (: do_status, 1 :));
      add_command("reset", "revenue on <direct:object>", (: do_reset :));
   }
} /* init() */

/** @ignore yes */
mixed query_static_auto_load() {
  if ( base_name(this_object()) + ".c" == __FILE__ ) {
    return int_query_static_auto_load();
  }
  return ([ ]);
} /* query_static_auto_load() */

/** @ignore yes */
mixed query_dynamic_auto_load() {
   mapping junk;

   junk = object::query_dynamic_auto_load();
   if (!junk) {
      junk = ([ ]);
   }
   junk["bonded"] = query_bonded();
   add_auto_load_value(junk, OBJECT_TAG, "revenue", query_revenue());
   add_auto_load_value(junk, OBJECT_TAG, "commercial id", query_commercial_id());
   return junk;
} /* query_dynamic_auto_load() */

/** @ignore yes */
void init_dynamic_arg(mapping map) {
   object::init_dynamic_arg(map);
   if (map) {
      set_bonded(map["bonded"]);
      set_revenue(query_auto_load_value(map, OBJECT_TAG, "revenue"));
      if (query_auto_load_value(map, OBJECT_TAG, "commercial id")) {
         set_commercial_id(query_auto_load_value(map, OBJECT_TAG, "commercial id"));
      }
   }
} /* init_dynamic_arg() */

/** @ingore yes */
string query_help_file_directory() {
   return "/doc/furniture/";
} /* query_help_file_directory() */

/** @ingore yes */
mixed *stats() {
   return ::stats() +
          ({ ({ "revenue", query_revenue() }),
             ({ "bonded", query_bonded() }),
             ({ "commercial id", query_commercial_id() }) });
} /* stats() */
