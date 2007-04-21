/**
 * This is to handle a place where players can sell a number of neato
 * craft items they have made up.  It will be extended to deal with
 * books as well.
 * <p>
 * It keeps track of who sold each item and how much they will
 * receive when they pay for it.  Then tracks the royalties so
 * they can come back and collect them.
 * <p>
 * This will be run as a handler object so that npcs and rooms can
 * both run as this sort of shop.
 * <p>
 * NB: This object is not saved using save_object.  You need to set the
 * save and load functions for this system to work.
 * @author Pinkfish
 * @started Thu Feb  5 15:39:57 CST 1998
 */

#include <obj_parser.h>
#include <money.h>
#include <move_failures.h>
#include <player.h>
#include <shops/craft_shop.h>

/**
 * This keeps track of the object information itself.
 * @element cost the cost of the object when it is sold
 * @element cap_owner the capitalised owner of the object
 * @element short the short describtions of the object
 * @element ob_num the number of the object
 */
class craft_object {
   int cost;
   string cap_owner;
   int ob_num;
   string category;
   string short;
}

/**
 * This class is the main sellable list.
 */
class craft_sellable {
   /**
    * The list of objects, it is indexed on the owners name.
    */
   mapping objects;
}

/**
 * This is the class with information about the category in it.
 */
class craft_category {
   int state;
   string description;
   string information;
   string* voted;
   int yes;
   int no;
   int abstain;
   int timeout;
}

#define SELL_OBJECT_NAME_PROP "sell name"
#define SELL_OBJECT_OWNER_PROP "sell owner"
#define SELL_OBJECT_CLASS_PROP "sell class"
#define SELL_OBJECT_ID_PROP "sell id"

/*
 * This will index on the name tag and will have an object, an array of owners
 * and
 * a number left count as the value in an array.  The object is
 * a dummy object to match on.  The real object will be created from
 * the save information.  
 */
private nosave mapping _sellables;
/*
 * This is the contained containingt the objects to sell.  We keep them in
 * a container so that we can do find_match matches.
 */
private nosave object _sell_list;
/* The royalties to be payed to each player. */
private nosave mapping _royalties;
/* The current id to use for the save files... */
private nosave int _current_save_num;
/* The categories we are using, the default is none. */
private nosave mapping _categories;
private nosave int _category_callout;
/* If not set then we add a name on by default if it doesn't exist. */
private nosave int _category_dont_use_name;


/* The functions to save and load the files... */
private nosave function _save_function;
private nosave function _load_function;
private nosave function _category_function;
private nosave int *_current_ids;
private nosave int _has_loaded;

void load_it();
void save_it();
private void update_sellable(string name);
class craft_object create_craft_object(object ob,
                                       string owner,
                                       int cost,
                                       string category);
void adjust_royalty(string player, int amt);
class craft_sellable create_craft_sellable();
void adjust_royalty(string player, int amt);
class craft_object query_class_of_shop_object(object ob);
object find_shop_object(class craft_object frog);
object *query_sell_list_obs();
string query_id_of_shop_object(object ob);
void remove_shop_id(string id);
private void setup_timeout_call();
object* query_items_with_shop_id(string id);

void create() {
   /* Create a nice container to put our sell list in. */
   _sell_list = clone_object("/std/container");
   _current_save_num = 1;
   _current_ids = ({ });
   _categories = ([ ]);
} /* create() */

/**
 * This method is called by the controled object onto here to control
 * if the owners name is added to shop objects.  If this is set to 1
 * then the owners name will  not be added by default.
 * @param flag the flag to set
 */
void set_dont_use_name(int flag) {
   _category_dont_use_name = flag;
} /* set_dont_use_name() */

/**
 * This method is used to determine the status of the flag which controls
 * adding the owners name to shop objects by default.
 * If this is set to 1
 * then the owners name will  not be added by default.
 * @return 1 if the object owners name is not used, 0 if it is
 */
int query_dont_use_name() {
   return _category_dont_use_name;
} /* query_dont_use_name() */

/**
 * This method creates a save file for the specified objects autoloading
 * capability.  If the number to write to is non-null then it will
 * write to that object.
 * @param ob the object to get an autoload number for
 * @param fixed_num the file number to write to
 * @return the auto load number
 * @see save_it()
 */
protected int create_auto_load_file(mixed ob, int fixed_num) {
   mixed *auto_load;
   mixed tmp;

   if (objectp(ob)) {
      if (this_player()) {
         catch(auto_load = this_player()->create_auto_load(({ ob })));
      }
      if (!auto_load) {
         catch(auto_load = PLAYER_OB->create_auto_load(({ ob })));
      }
      if (!auto_load) {
         // Make it error on the last one...
         auto_load = AUTO_LOAD_OB->create_auto_load(({ ob }));
      }
   } else if (pointerp(ob)) {
      auto_load = ob;
   } else {
      printf("Error!  Dammit!\n");
   }
   if (!fixed_num) {
      do {
         tmp = evaluate(_load_function,
                        CRAFT_SHOP_DATA_SAVE_FILE,
                        "" + _current_save_num);
         if (tmp) {
            _current_save_num++;
         }
      } while (tmp);
      fixed_num = _current_save_num;
   }
   evaluate(_save_function,
            CRAFT_SHOP_DATA_SAVE_FILE,
            auto_load,
            "" + fixed_num);
   return fixed_num;
} /* create_auto_load_file() */

/**
 * This method creates a real object from the save file number.
 * @param num the save file number
 * @return the nice shiny new object
 * @see create_auto_load_file()
 */
protected object create_real_auto_load_object(int num, object player) {
   mixed *auto_load;
   object *obs;

   auto_load = evaluate(_load_function,
                        CRAFT_SHOP_DATA_SAVE_FILE,
                        "" + num);
   if (userp(player)) {
      obs = player->load_auto_load_to_array(auto_load, player);
   } else if (this_player()) {
      obs = this_player()->load_auto_load_to_array(auto_load, player);
   } else {
      obs = PLAYER_OB->load_auto_load_to_array(auto_load, player);
   }
   if (sizeof(obs)) {
      return obs[0];
   }
   return 0; // clone_object("/std/object");
} /* create_object_from_auto_load() */

/**
 * This method removes the auto load stuff after it is no longer needed.
 * @param num the file number to remove
 * @see create_object_from_auto_load()
 */
protected void remove_auto_load_file(int num) {
   evaluate(_save_function,
            CRAFT_SHOP_REMOVE_DATA_SAVE_FILE,
            0,
            "" + num);
} /* remove_auto_load_file() */

/**
 * This method adds a selable into the current sellable array.
 * @param name the name the object willb elisted under
 * @param owner the owner of the object being sold
 * @param ob the objects being sold
 * @see add_sell_list_object()
 * @see change_name_of_object()
 */
protected void add_to_sellables(string name,
                                string owner,
                                class craft_object *ob) {
   class craft_sellable craft_sell;

   if (_sellables[name]) {
      craft_sell = (class craft_sellable)_sellables[name];
   } else {
      craft_sell = create_craft_sellable();
      _sellables[name] = craft_sell;
   }

   /*
    * This seemingly weird way of handling the craft objects is so we have
    * an easily returnable handle to the class which we can pass to the
    * dummy shop objects.
    */
   if (!craft_sell->objects[owner]) {
      craft_sell->objects[owner] = ({ });
   }
   craft_sell->objects[owner] += ob;
   
   update_sellable(name);
} /* add_to_selllist() */

/**
 * This will allow us to add an object into the inventory of the
 * craft shop.  If the name already exists in the inventory we
 * just increment the number left by the number passed in here.
 * If the number left goes below 0 then the item is remove
 * from the inventory.
 * @param ob the object to add
 * @param name the tag to use for it
 * @param num the number to add
 * @param category the category to add the item to
 * @param dont_update_cost allows the same name to have multiple prices
 * @see remove_list_object()
 * @see add_sell_list_name()
 */
int add_list_object(object ob,
                    string name,
                    int cost,
                    string owner,
                    string category,
                    int dont_update_cost) {
   class craft_object craft_ob;
   string cap_owner;

   if (!objectp(ob) || !stringp(name) || !intp(cost)) {
      return 0;
   }

   cap_owner = owner;
   owner = lower_case(owner);
   add_to_sellables(name,
                    owner,
                    ({ create_craft_object(ob, cap_owner, cost, category) }));
   if (!dont_update_cost) {
      foreach (craft_ob in ((class craft_sellable)_sellables[name])->objects[owner]) {
         craft_ob->cost = cost;
         craft_ob->category = category;
      }
   }
   save_it();
   return 1;
} /* add_list_object() */

/**
 * This method removes a single object from the current object list.
 * @param name the name of object to remove
 * @param owner the owner of the object to remove
 * @param ob the craft object pointer itself we want removed
 * @see add_list_object()
 */
void remove_list_object(string name, string owner, class craft_object ob) {
   int i;
   object us;
   class craft_object *data;

   if (_sellables[name] &&
       ((class craft_sellable)_sellables[name])->objects[owner]) {
      data = ((class craft_sellable)_sellables[name])->objects[owner];
      for (i = 0; i < sizeof(data); i++) {
         if (data[i] == ob) {
            break;
         }
      }
      //i = member_array(ob, (class craft_object)_sellables[name]->objects[owner]);
      if (i < sizeof(data)) {
         ((class craft_sellable)_sellables[name])->objects[owner] -= ({ ob });
         if (!sizeof(((class craft_sellable)_sellables[name])->objects[owner])) {
            map_delete(((class craft_sellable)_sellables[name])->objects, owner);
            if (!sizeof(((class craft_sellable)_sellables[name])->objects)) {
               map_delete(_sellables, name);
               us = find_shop_object(ob);
               remove_shop_id(query_id_of_shop_object(us));
            }
         }
         save_it();
         remove_auto_load_file(ob->ob_num);
         us = find_shop_object(ob);
         us->dest_me();
      } else {
         printf("Unable to find the object to remove? %O, %O\n", name, owner);
      }
   }
} /* remove_list_object() */

/**
 * This method turns a craft_object class into an actual object.
 * @param craft the craft object to create
 * @return the newly formed craft object
 * @see create_all_real_objects()
 */
object create_real_object(object player, class craft_object craft) {
   return create_real_auto_load_object(craft->ob_num, player);
} /* create_real_object() */

/**
 * This method creates real objects for all the passed in dummy objects.
 * Remember to dest the objects after you are finished with them.
 * @param obs the dummy objects to find real ones of
 * @return the real object values
 * @see create_real_object()
 * @example
 * real_obs = create_real_objects(this_player(), obs);
 * foreach (ob in real_obs) {
 *    ret += ob->the_short + ":\n" + ob->long() + "\n";
 * }
 */
object *create_all_real_objects(object player, object *obs) {
   object *ret;
   object ob;
   class craft_object craft;
   object new_ob;

   ret = ({ });
   foreach (ob in obs) {
      craft = query_class_of_shop_object(ob);
      new_ob = create_real_object(player, craft);
      if (!new_ob) {
         ret->move("/room/rubbish");
         return ({ });
      }
      ret += ({ new_ob });
   }
   return ret;
} /* create_all_real_objects() */

/**
 * This method creates a craft object from the input actual object.
 * @param ob the real input object
 * @param owner the owner of the object
 * @param cost the cost of the object
 * @param category the category of the object
 * @return a nice craft object
 */
class craft_object create_craft_object(object ob,
                                       string owner,
                                       int cost,
                                       string category) {
   class craft_object craft;
   int num;

   craft = new (class craft_object);
   num = create_auto_load_file(ob, 0);
   craft->ob_num = num;
   craft->cost = cost;
   craft->cap_owner = owner;
   craft->category = category;
   craft->short = ob->query_short();
   return craft;
} /* create_craft_object() */

/**
 * This method creates a sellable class.
 * @return a new sellable class
 */
class craft_sellable create_craft_sellable() {
   class craft_sellable craft;

   craft = new(class craft_sellable);
   craft->objects = ([ ]);
   return craft;
} /* create_craft_sellable() */

/**
 * This method returns the owner associated with the shop
 * object.
 * @param ob the shop object to query the owner of
 * @return the owner of the shop object
 */
string query_owner_of_shop_object(object ob) {
   return ob->query_property(SELL_OBJECT_OWNER_PROP);
} /* query_owner_of_shop_object() */

/**
 * This method returns the list name associated with the shop
 * object.
 * @param ob the shop object to query the list name of
 * @return the owner of the shop object
 */
string query_name_of_shop_object(object ob) {
   return ob->query_property(SELL_OBJECT_NAME_PROP);
} /* query_name_of_shop_object() */

/**
 * This method returns the id associated with the shop object.
 * This is the one letter id used for buying selling etc.
 * @param ob the shop object to query the id of
 * @return the id of the shop object
 */
string query_id_of_shop_object(object ob) {
   return ob->query_property(SELL_OBJECT_ID_PROP);
} /* query_id_of_shop_object() */

/**
 * This method sets the current id of the shop objects.
 * @param obs the objects to set the id for
 * @param id the new id for them all
 */
void set_id_of_shop_objects(object *obs, string id) {
   obs->add_property(SELL_OBJECT_ID_PROP, id);
   obs->add_alias(id);
} /* set_id_of_shop_objects() */

/**
 * This method returns the category of the speficied shop object.
 * @param ob the object to find the category of
 * @return the category of the object
 */
string query_category_of_shop_object(object ob) {
   class craft_object craft;

   craft = query_class_of_shop_object(ob);
   if (craft) {
      return craft->category;
   }
   return 0;
} /* query_category_of_shop_object() */

/**
 * This method returns the possible short descriptions of the shop objects.
 * @param ob the object to find the shorts of
 * @return the short descriptions
 */
string query_short_of_shop_object(object ob) {
   class craft_object craft;

   craft = query_class_of_shop_object(ob);
   if (craft) {
      if (sizeof(craft) == 5) {
         return craft->short;
      }
      return "womble";
   }
   return 0;
} /* query_shorts_of_shop_object() */

/**
 * This method updates the internal representation of the specified
 * craft object.  It replaces the saved auto load info with the new
 * stuff.
 * @param craft the craft object containing the info to replace
 * @param replacement_ob the new object
 */
void update_craft_object(object craft_ob, object replacement_ob) {
   class craft_object craft;

   craft = query_class_of_shop_object(craft_ob);
   if (craft) {
      create_auto_load_file(replacement_ob, craft->ob_num);
   }
} /* update_craft_object() */

/**
 * This method attempts to figure out what the next id would
 * be for the object.  It will allocate this id as well as returning.
 * @return the next id for the shop
 * @see remove_shop_id()
 * @see update_sellables()
 */
string query_next_shop_id() {
   int num;

   num = 0;
   while (member_array(num, _current_ids) != -1) {
      num++;
      // Make it skip an
      if (num == 13) {
         num++;
      }
   }
   _current_ids += ({ num });
   return sprintf("%c%c", (num / 26) + 'a',
                          (num % 26) + 'a');
} /* query_next_shop_id() */

/**
 * This method removes an id when that book has been taken from the
 * shop.
 * @param id the id to remove
 * @see query_next_shop_id()
 */
void remove_shop_id(string id) {
   int real_id;

   real_id = (id[0] * 26) - 'a' + id[1] - 'a';
   _current_ids -= ({ real_id });
} /* remove_shop_id() */

/**
 * This method searches the current sell list to see if the object
 * passed corresponds to any of the existing things in our
 * sell list.
 * @param ob the object to check
 * @return the craft_object class pointed to by the object
 * @see add_object()
 * @see remove_object()
 * @see create_real_object()
 */
class craft_object query_class_of_shop_object(object ob) {
   return ob->query_property(SELL_OBJECT_CLASS_PROP);
} /* query_class_of_shop_object() */

/**
 * This method figured out what the real object is for the class.
 * @param frog the class to find the object for
 * @return the dummy object for the class
 */
object find_shop_object(class craft_object frog) {
   object ob;

   foreach (ob in query_sell_list_obs()) {
      if (query_class_of_shop_object(ob) == frog) {
         return ob;
      }
   }
   return 0;
} /* find_shop_object() */

/**
 * This method changes the value of the shop object.
 * @param ob the shop object
 * @param value the new value
 */
void change_value_of_shop_object(object ob, int value) {
   string owner;
   string name;
   class craft_object wombat;
   //object frog;
   //int old_cost;
   object* obs;

   obs = query_items_with_shop_id(query_id_of_shop_object(ob));

   foreach (ob in obs) {
      owner = query_owner_of_shop_object(ob);
      name = query_name_of_shop_object(ob);
      wombat = query_class_of_shop_object(ob);
      wombat->cost = value;
      ob->set_value(value);
      //old_cost = wombat->cost;
      

/*
      foreach (wombat in ((class craft_sellable)_sellables[name])->objects[owner]) {
         if (wombat->cost == old_cost) {
            wombat->cost = value;
            frog = find_shop_object(wombat);
            frog->set_value(value);
         }
      }
 */
   }
   save_it();
} /* change_value_of_shop_object() */

/**
 * This method changes the name of the shop object.
 * @param ob the shop object to change
 * @param new_name the new name of the shop object
 * @param dont_update_cost allows the names to merge and not update costs
 * @return 1 on success, 0 on failure
 */
int change_name_of_shop_object(object ob, string new_name,
                               int dont_update_cost) {
   string owner;
   string name;
   object* obs;
   string* names;
   class craft_object wombat;
   int new_cost;
   class craft_object* fluff;
   int i;

   obs = query_items_with_shop_id(query_id_of_shop_object(ob));

   names = ({ });
   foreach (ob in obs) {
      owner = query_owner_of_shop_object(ob);
      name = query_name_of_shop_object(ob);
      wombat = query_class_of_shop_object(ob);

      if (!_sellables[name]) {
tell_creator("pinkfish", "The name %O does not exist.\n", name);
         return 0;
      }

      if (!wombat) {
tell_creator("pinkfish", "Unable to find the class.\n");
         return 0;
      }

      if (!dont_update_cost &&
          _sellables[new_name] &&
          (((class craft_sellable)_sellables[new_name])->objects[owner])) {
         new_cost = (((class craft_sellable)_sellables[new_name])->objects[owner])[0]->cost;
         wombat->cost = new_cost;
      }
      //
      // First, add this thingy to the new spot.
      //
      //add_to_sellables(new_name, owner, ({ ob }));
      if (!_sellables[new_name]) {
         _sellables[new_name] = create_craft_sellable();
      }
      if (!(((class craft_sellable)_sellables[new_name])->objects[owner])) {
         (((class craft_sellable)_sellables[new_name])->objects[owner]) = ({ });
      }
      (((class craft_sellable)_sellables[new_name])->objects[owner]) += ({ wombat });

      //
      // Next, remove it from where it came from.
      //
      fluff = ((class craft_sellable)_sellables[name])->objects[owner];
      for (i = 0; i < sizeof(fluff); i++) {
         if (fluff[i] == wombat) {
            fluff = fluff[0..i-1] + fluff[i+1..];
         }
      }
      if (sizeof(fluff)) {
         ((class craft_sellable)_sellables[name])->objects[owner] = fluff;
      } else {
         map_delete(((class craft_sellable)_sellables[name])->objects, owner);
      }
      if (!sizeof(((class craft_sellable)_sellables[name])->objects)) {
         map_delete(_sellables, name);
      }
      names |= ({ name });
   }

   if (sizeof(names)) {
      update_sellable(name);
      update_sellable(new_name);
      save_it();
      return 1;
   }
   return 0;
} /* change_name_of_shop_object() */

/**
 * This method changes the category of the shop object.
 * @param ob the shop object to change
 * @param new_category the new category of the shop object
 */
int change_category_of_shop_object(object ob, string new_category) {
   string owner;
   string name;
   class craft_object bing;
   object* obs;
   int do_save;

   obs = query_items_with_shop_id(query_id_of_shop_object(ob));
   foreach (ob in obs) {
      owner = query_owner_of_shop_object(ob);
      name = query_name_of_shop_object(ob);
      if (!_sellables[name] ||
          !((class craft_sellable)_sellables[name])->objects[owner]) {
         return 0;
      }
      foreach (bing in ((class craft_sellable)_sellables[name])->objects[owner]) {
         bing->category = new_category;
         do_save = 1;
      }
   }
   if (do_save) {
      save_it();
   }
   return 1;
} /* change_name_of_shop_object() */

/**
 * This method removes the real data behind the specified shop object.
 * The object passed in must be one of the dummy objects used for
 * listing the shops inventory.
 * @param ob the object to remove
 * @see sell_objects()
 * @see query_class_of_shop_object()
 */
void remove_shop_object(object ob) {
   string name;
   string owner;

   owner = query_owner_of_shop_object(ob);
   name = query_name_of_shop_object(ob);
   remove_list_object(name, owner, query_class_of_shop_object(ob));
} /* remove_shop_object() */

/**
 * This method finds the objects which matched the passed in pattern.
 * Note, this returns the dummy pointer objects not the actual
 * objects, they need to be changed into real objects before they
 * are really sold.
 * @param str the pattern to match the objects on
 * @return the matching objects
 * @see query_class_of_shop_object()
 * @see value_of_objects()
 * @see sell_objects()
 */
class obj_match find_matching_objects(string str) {
   class obj_match obs;

   obs = (class obj_match)match_objects_in_environments(str, ({ _sell_list }));
   return obs;
} /* find_matching_objects() */

/**
 * This method determines the price of all the shop objects in the
 * array.
 * @return the price of the shop objects
 * @param obs the objects to price
 * @see find_matching_objects()
 * @see query_class_of_shop_object()
 * @see sell_objects()
 */
int value_of_objects(object *obs) {
   object ob;
   class craft_object craft;
   int value;

   foreach (ob in obs) {
      craft = query_class_of_shop_object(ob);
      if (craft) {
         value += craft->cost;
      }
   }
   return value;
} /* value_of_shop_objects() */

/**
 * This method sells the specified shop objects to the player.  This
 * will make the payments to the player.  The objects will all attempt
 * to be moved into the player.  The cut must be between 0 and 100.
 * <p>
 * The royalties from selling the object will be placed into the
 * correct place by the function, so they can be picked up at a 
 * later date.  The 'cut' will be taken out of the sale.
 * @param obs the objects to sell
 * @param place the place to sell the objects in
 * @param player the player to sell the objects too
 * @param cut the cut the manager takes from the sale (percentage)
 * @param force forces the object to be bought even if it is broken
 * @return the array of sold objects
 * @see value_of_objects()
 * @see find_matching_objects()
 * @see adjust_royalty()
 */
object *sell_objects(object *obs, string place, object player, int cut,
                     int force, int ref total_value) {
   object ob;
   object this_ob;
   object *sold;
   class craft_object craft;

   sold = ({ });
   if (cut < 0) {
      cut = 0;
   }
   if (cut > 100) {
      cut = 100;
   }
   total_value = 0;
   foreach (ob in obs) {
      craft = query_class_of_shop_object(ob);
      if (craft) {
         if (player->query_value_in(place) >= craft->cost) {
            // Create the object.
            this_ob = create_real_object(player, craft);
            if (force && !this_ob) {
               // Make a dummy object to 'sell'
               this_ob = clone_object("/std/object");
               this_ob->set_name("broken");
               this_ob->set_long("A poor broken shop object.");
               this_ob->set_weight(1);
               // Drop it into the rubbish a little while later.
               call_out((: $1->move("/room/rubbish") :), 10, this_ob);
            }
            debug_printf("Made %O %O\n", this_ob, force);
            if (this_ob && this_ob->move(player) == MOVE_OK) {
               sold += ({ this_ob });
               total_value += craft->cost;
               // Make the player pay for it.
               player->pay_money(
                       MONEY_HAND->create_money_array(craft->cost, place),
                       place);
               // Adjust the players current royalties.
               adjust_royalty(query_owner_of_shop_object(ob),
                              craft->cost - (craft->cost * cut) / 100);
               // Remove the object from our inventory.
               remove_shop_object(ob);
            }
         }
      }
   }
   return sold;
} /* sell_objects() */


/**
 * This method deletes the specified list entry.  The player
 * owner will not be payed.
 * @param id The list entry to delete
 */
void delete_objects( string id ) {
   object ob, *obs;

   obs = all_inventory( _sell_list );

   obs = filter( obs,
         (: $1->query_property( SELL_OBJECT_ID_PROP ) == $(id) :) );
   
   foreach( ob in obs ) {
      remove_shop_object( ob );
   }
   
} /* delete_objects() */

/**
 * This method will buy the specified objects and place them into our
 * current potential sell list.  This will destroy the objects after
 * it has been added into the sell list correctly.
 * @param obs the object to sell
 * @param name the name of the object to sell
 * @param cost the cost of the object being sold
 * @param owner the person to who the object belongs
 * @param category the category of the object
 * @param dont_update_costs allows the system to handle objects with the same
 * name and different costs
 * @see sell_objects()
 */
object *buy_objects(object *obs,
                    string name,
                    int cost,
                    string owner,
                    string category,
                    int dont_update_costs) {
   object ob;
   object *bought;

   bought = ({ });
   foreach (ob in obs) {
      if (add_list_object(ob, name, cost, owner, category, dont_update_costs)) {
         ob->move("/room/rubbish");
         bought += ({ ob });
      }
   }
   return bought;
} /* buy_objects() */

/**
 * This method creates a dummy object for use when selling items.  This
 * allows use to use alias names for the potions when buying/selling
 * and not the real name of the container.  When bought these names
 * will be added to the thingy as aliases.
 * @param owner the owner of the object
 * @param name the sell list name of the object
 * @return a dummy object to place into the sell list
 */
protected object create_dummy_object(string owner, string name, int cost,
                                     class craft_object craft) {
   object new_name;
   string *bits;

   new_name = clone_object("/std/object");
   new_name->reset_get();
   bits = explode(lower_case(name), " ");
   
   if (!sizeof (bits)) {
    new_name->set_name("error");
   }
   else {
    new_name->set_name(bits[<1]);
    new_name->add_adjective(bits[0..<2]);
   }
  
   if (member_array(lower_case(owner), bits) == -1) {
      new_name->add_adjective(owner);
   }
   if (!_category_dont_use_name &&
       member_array(lower_case(owner) + "'s", bits) == -1) {
      new_name->add_adjective(lower_case(owner) + "'s");
      new_name->set_short(craft->cap_owner + "'s " + name);
   } else {
      new_name->set_short(capitalize(name));
   }
   new_name->add_property(SELL_OBJECT_NAME_PROP, name);
   new_name->add_property(SELL_OBJECT_OWNER_PROP, owner);
   new_name->add_property(SELL_OBJECT_CLASS_PROP, craft);
   new_name->add_property("determinate", "");
   new_name->set_value(cost);
   return new_name;
} /* create_dummy_object() */

/**
 * This method adds an object into the current sell list.
 * @param ob the object to add
 */
private void add_sell_object(object ob) {
   if (ob->move(_sell_list) != MOVE_OK) {
      printf("Unable to move %O into the sell list (%O).\n", ob, _sell_list);
   }
} /* add_sell_object() */

/**
 * This method returns all the current objects in the sell list.
 * @return all the current objects in the sell list
 */
object *query_sell_list_obs() {
   return all_inventory(_sell_list);
} /* query_sell_list_obs() */

/**
 * This method returns the container associated with the sell list.
 * @return the container for the sell list
 */
object query_sell_list() {
   return _sell_list;
} /* query_sell_list() */

/**
 * This method returns all the objects which match the specified
 * id of the shop object.
 * @param id the shop id to match on
 * @return the objects which match it
 */
object* query_items_with_shop_id(string id) {
   return filter(query_sell_list_obs(), (: query_id_of_shop_object($1) == $2 :),
                 id);
} /* query_items_with_shop_id() */

/**
 * This method updates the specified named set of sellables.  This
 * removes and creates the nessessary dummy objects needed to
 * handle the selling code.  It will also set the ids for objects
 * which need them, creating a new id if nessessary.
 * @param name the name of the sellables list to update
 * @see query_sell_list_obs()
 */
private void update_sellable(string name) {
   string owner;
   class craft_sellable bing;
   mixed *data;
   class craft_object womble;
   class craft_object craft;
   object shop_ob;
   string id;
   mixed *need_ids;
   int cost;
   mapping costs;
   int pos;

   if (_sellables[name]) {
      bing = _sellables[name];
      costs = ([ ]);
      foreach (owner, data in bing->objects) {
         id = 0;
         data = filter(data, (: $1 :));
         bing->objects[owner] = data;
         pos = 0;
         foreach (womble in data) {
            if (sizeof(womble) == 4) {
               craft = new(class craft_object);
               craft->ob_num = womble->ob_num;
               craft->cost = womble->cost;
               craft->cap_owner = womble->cap_owner;
               craft->category = womble->category;
               craft->short = "womble";
               data[pos] = craft;
            }
            pos++;

            shop_ob = find_shop_object(womble);
            if (!shop_ob ||
                query_name_of_shop_object(shop_ob) != name) {
               if (shop_ob) {
                  shop_ob->move("/room/rubbish");
               }
               add_sell_object(create_dummy_object(owner, name,
                                                   womble->cost, womble));
            }
            shop_ob = find_shop_object(womble);
            if (!costs[womble->cost]) {
               costs[womble->cost] = ({ 0, ({ }) });
            }
            if (!costs[womble->cost][0]) {
               id = query_id_of_shop_object(shop_ob);
               if (id) {
                  costs[womble->cost][0] = id;
               }
            }
            if (!costs[womble->cost][0] ||
                query_id_of_shop_object(shop_ob) != costs[womble->cost][0]) {
               costs[womble->cost][1] += ({ shop_ob });
            }
         }
         if (!_category_dont_use_name) {
            foreach (cost, need_ids in costs) {
               if (!need_ids[0]) {
                  need_ids[0] = query_next_shop_id();
               }
               set_id_of_shop_objects(need_ids[1], need_ids[0]);
            }
         }
      }

      if (_category_dont_use_name) {
         foreach (cost, need_ids in costs) {
            if (!need_ids[0]) {
               need_ids[0] = query_next_shop_id();
            }
            set_id_of_shop_objects(need_ids[1], need_ids[0]);
         }
      }
   }
} /* update_sellables() */

/**
 * This method returns the list of sellables that the owner currently
 * has in the shop.  This will potentially return more than one of the
 * same name if the owner has more than one of the same sort of object
 * being sold.
 * @param owner the owner to check
 * @return the array of all the possible sellables
 * @see query_list_object_cost()
 */
string *query_owner_sellables(string owner) {
   class craft_sellable sell;
   mixed *womble;
   string *ret;
   string name;

   ret = ({ });
   foreach (name, sell in _sellables) {
      if (sell->objects[owner]) {
         womble = sell->objects[owner];
         ret += allocate(sizeof(womble), (: $(name) :) );
      }
   }
   return ret;
} /* query_owner_sellables() */

/**
 * This method returns the cost of the specified type of object.
 * @param name the name of the object
 * @param owner the owner of the object
 * @return the cost of the object, 0 if it is not known
 * @see query_owner_sellables()
 */
int query_list_object_cost(string name, string owner) {
   class craft_sellable sell;
   class craft_object wombat;

   owner = lower_case(owner);
   if (_sellables[name]) {
      sell = _sellables[name];
      if (sell->objects[owner]) {
         wombat = sell->objects[owner][0];
         return wombat->cost;
      }
   }
   return 0;
} /* query_list_object_cost() */

/**
 * This method sets the save function on the class.  The save function will
 * be called with two parameters, one is the file name to be saved and
 * the other is the text to be saved to it.
 * @param func the new save function
 */
void set_save_function(function func) {
   _save_function = func;
} /* set_save_function() */

/**
 * This method sets the load function on the class.  The restore function
 * will be called with one parameter and is expected to return a string.
 * The parameter is the file name to load.
 * @param func the new load function
 */
void set_load_function(function func) {
   _load_function = func;
} /* set_load_function() */

/**
 * This method is called when categories complete each of their phases to
 * see if the idea was passed by enough majority or whatever to go onto
 * the next one.
 * @param func the new category function
 */
void set_category_function(function func) {
   _category_function = func;
} /* set_category_function() */

/**
 * This method saves the current state of the system.
 * @see set_save_function()
 * @see load_it()
 */
void save_it() {
   if (_has_loaded) {
      evaluate(_save_function, CRAFT_SHOP_MAIN_SAVE_FILE,
            ({ _sellables, _royalties, _current_save_num, _categories }) );
   }
} /* save_it() */

/**
 * This method loads the current state of the system.
 * @see set_load_function()
 */
void load_it() {
   mixed *map;
   string name;

   _has_loaded = 1;
   map = evaluate(_load_function, CRAFT_SHOP_MAIN_SAVE_FILE);
   if (pointerp(map) && sizeof(map) == 3) {
      _sellables = map[0];
      _royalties = map[1];
      _current_save_num = map[2];
      foreach (name in keys(_sellables)) {
         reset_eval_cost();
         update_sellable(name);
      }
      if (!mapp(_royalties)) {
         _royalties = ([ ]);
      }
   } else if (pointerp(map) && sizeof(map) == 4) {
      _sellables = map[0];
      _royalties = map[1];
      _current_save_num = map[2];
      _categories = map[3];
      // This does need to be run even if there are no categories.  What has
      // no categories anyway?
      if (!mapp(_sellables)) {
         _sellables = ([ ]);
      }
      foreach (name in keys(_sellables)) {
         reset_eval_cost();
         update_sellable(name);
      }
      if (!mapp(_royalties)) {
         _royalties = ([ ]);
      }
   } else {
      _sellables = ([ ]);
      _royalties = ([ ]);
   }
   setup_timeout_call();
} /* load_it() */

/**
 * This method adjusts the current royalties for the player.
 * @param player the name of the player to pay the royalties to
 * @param amt the amount to adjust the value by
 * @see query_royalties()
 * @see pay_out_royalties()
 */
void adjust_royalty(string player, int amt) {
   _royalties[player] += amt;
   save_it();
} /* adjust_royalities() */

/**
 * This method returns the current royalties for the specified person.
 * @param player the player to get the royalties for
 * @return the current royalties
 * @see adjust_royalties()
 * @see pay_out_royalties()
 */
int query_royalty(string player) {
  return _royalties[player];
} /* query_royalties() */

/**
 * This method returns the royalty mapping on the object.  This is only
 * use for debug information.
 * @return the royalties mapping
 */
mapping query_all_royalties() {
   return copy(_royalties);
} /* query_all_royalties() */

/**
 * This method pays out the royalties owed to the specified player.
 * @param player the player to pay out
 * @param place the place in which the royalties are to occur
 * @param amount the amount to pay out, 0 for everything
 * @see query_royalties()
 * @see adjust_royalties()
 */
void pay_out_royalty(string player, string place, int amount) {
   object pl;
 
   pl = find_player(player);
   if (pl && _royalties[player]) {
      if (!amount) {
         amount = _royalties[player];
      }
      pl->adjust_money(MONEY_HAND->create_money_array(amount, place));
      _royalties[player] -= amount;
      if (!_royalties[player]) {
         map_delete(_royalties, player);
      }
      save_it();
   }
} /* pay_out_royalties() */

//
//  All the category handling code goes after this point.
//

/**
 * This method checks to make sure that the category is valid.  This
 * means a category that has been properly accepted.
 * @param category the category to check
 */
int is_valid_category(string category) {
   if (_categories[category] &&
       (_categories[category]->state == CRAFT_CATEGORY_STATE_ACCEPTED ||
        _categories[category]->state == CRAFT_CATEGORY_STATE_DELETEING)) {
      return 1;
   }
   return 0;
} /* is_valid_category() */

/**
 * Add a request for a new category.
 * @param name the name of the category
 * @param player the person who suggested it
 * @param timeout the initial timeout for the nomination period
 * @return 1 if successful added, 0 if not
 */
int add_category_request(string name,
                         object player,
                         int timeout) {
   if (!_categories[name]) {
      _categories[name] = new(class craft_category);
      _categories[name]->state = CRAFT_CATEGORY_STATE_NOMINATING;
      _categories[name]->voted = ({ player->query_name() });
      _categories[name]->description = 0;
      _categories[name]->yes = 0;
      _categories[name]->no = 0;
      _categories[name]->timeout = time() + timeout;
      save_it();
      return 1;
   }
   return 0;
} /* add_category_request() */

/**
 * This method removes a category immediately.
 * @param name the name of the category
 */
int remove_category_accepted(string category) {
   if (_categories[category]) {
      map_delete(_categories, category);
      save_it();
      return 1;
   }
   return 0;
} /* remove_category_accepted() */

/**
 * This method adds a category and sets it as already being accepted.
 * @param name the name of the category to add
 */
int add_category_accepted(string name) {
   if (!_categories[name]) {
      _categories[name] = new(class craft_category);
      _categories[name]->state = CRAFT_CATEGORY_STATE_ACCEPTED;
      _categories[name]->voted = ({ });
      _categories[name]->description = 0;
      _categories[name]->yes = 0;
      _categories[name]->no = 0;
      _categories[name]->timeout = 0;
      save_it();
      return 1;
   }
   return 0;
} /* add_category_accepted() */

/**
 * This method sets the description of the category.
 * @param cat the category to set the description of
 * @param desc the description of the category
 */
void set_category_description(string cat, string desc) {
   if (_categories[cat]) {
      _categories[cat]->description = desc;
      save_it();
   }
} /* set_category_description() */

/**
 * This method returns the category description.
 * @param cat the cateogryt to find the description for
 * @return 0 for no description, the main description otherwise
 */
string query_category_description(string cat) {
   if (_categories[cat]) {
      if (stringp(_categories[cat]->description)) {
         return _categories[cat]->description;
      }
   }
   return 0;
} /* query_category_description() */

/**
 * This method sets the information of the category.
 * @param cat the category to set the information of
 * @param info the information of the category
 */
void set_category_information(string cat, string info) {
   if (_categories[cat]) {
      _categories[cat]->information = info;
      save_it();
   }
} /* set_category_information() */

/**
 * This method returns the category information.
 * @param cat the cateogryt to find the information for
 * @return 0 for no information, the main information otherwise
 */
string query_category_information(string cat) {
   if (_categories[cat]) {
      if (stringp(_categories[cat]->information)) {
         return _categories[cat]->information;
      }
   }
} /* query_category_information() */

/**
 * This method returns the category associated with this name/category
 * set.
 */
string query_category_of(string name, string owner) {
   if (_sellables[name] &&
       sizeof( ((class craft_sellable)_sellables[name])->objects[owner])) {
      return ((class craft_sellable)_sellables[name])->objects[owner][0]->category;
   }
   return 0;
} /* query_category_of() */

/**
 * This method checks to see if the player has voted (or nominated) for a
 * specified category already.
 * @param name the name of the category
 * @param player the player being checked
 * @return 1 if they have voted/nominated 0 if they have not
 */
int has_voted_for_category(string name,
                           object player) {
   return _categories[name] &&
       member_array(player->query_name(), _categories[name]->voted) != -1;
} /* has_player_voted() */

/**
 * This method returns the time the current category has left before it
 * times out.
 * @param name the name of the cateogyr
 * @return the time it timesout
 */
int query_category_timeout(string name) {
   if (_categories[name]) {
      return _categories[name]->timeout;
   }
   return CRAFT_CATEGORY_ERROR;
} /* query_category_timeout() */

/**
 * This method allows the player to vote for the specified category.
 * @param name the name of the category
 * @param player the player voting
 * @param choice their choice
 * @return 1 if the vote was successful, 0 if not
 */
int vote_for_category(string name,
                      object player,
                      int choice) {
   if (_categories[name]) {
      if (!has_voted_for_category(name, player)) {
         if (choice == CRAFT_CATEGORY_CHOICE_YES ||
             choice == CRAFT_CATEGORY_CHOICE_NO ||
             choice == CRAFT_CATEGORY_CHOICE_ABSTAIN) {
            switch (choice) {
            case CRAFT_CATEGORY_CHOICE_YES :
               _categories[name]->yes++;
               break;
            case CRAFT_CATEGORY_CHOICE_NO :
               _categories[name]->no++;
               break;
            case CRAFT_CATEGORY_CHOICE_ABSTAIN :
               _categories[name]->abstain++;
               break;
            }
            _categories[name]->voted += ({ player->query_name() });
            save_it();
         }
      }
   }
   return 0;
} /* vote_for_category() */

/**
 * This method returns the list of categories for the current shop.
 * @return the list of categories
 */
string* query_categories() {
   return filter(keys(_categories), (: is_valid_category($1) :));
} /* query_categories() */

/**
 * This method is used to go through the categories and see which ones
 * need to timeout.
 */
private void do_timeout_categories() {
   string name;
   class craft_category cat;
   int timeout;
