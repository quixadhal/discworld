/**
 * This room will handle craft shop like things, where you sell stuff to
 * the store and it will sell it onto other players after taking
 * a margin.  This can be used for potions, pottery, swords, anything!
 * @author Pinkfish
 * @started Thu Feb  5 15:39:57 CST 1998
 */
#include <money.h>
#include <shops/craft_shop.h>

inherit "/std/shops/inherit/craft_shop";

private nosave function _cat_extra_sell_check;
private string _default_category;

#define DAY (24 * 60 * 60)

//
// Predefinitions.
//
protected void setup_category(function func,
                              int value,
                              string name,
                              object* sellables);
protected void choose_category(string category,
                              function func,
                              int value,
                              string name,
                              object* sellables,
                              string* categories);
int do_list_category(string category);
int do_change_category(object* obs, string category);

void create() {
   ::set_extra_sell_check( (: setup_category :) );
   if (!_default_category) {
      _default_category = "General";
   }
   ::create();
} /* create() */

void init() {
   string storeroom;

   ::init();

   if (query_controller()) {
      storeroom = file_name(query_controller()->query_sell_list());
      add_command("change", "category of <indirect:object:" + storeroom +
                            "> to <string'category'>",
                  (: do_change_category($1, $4[1]) :));
   }
   add_command("list", "<string'category'>", (: do_list_category($4[0]) :));
} /* init() */

/**
 * This method adds a category into the accept list immediately.  This should
 * really only be used for initialy setting up the shop.
 * @param category the name of the category
 */
void add_shop_category(string category) {
   query_controller()->add_category_accepted(category);
} /* add_shop_category() */

/**
 * This method sets the description of the category.
 * @param category the name of the category
 * @param description the new description
 */
void set_category_description(string category, string description) {
   query_controller()->set_category_description(category, description);
} /* set_category_description() */

/**
 * This method sets the description of the category.
 * @param category the name of the category
 * @param info the new information
 */
void set_category_information(string category, string information) {
   query_controller()->set_category_information(category, information);
} /* set_category_information() */

/**
 * This method forcibly removes a category from the accepted list.
 * @param category the category to remove
 */
void remove_shop_category(string category) {
   query_controller()->remove_category_accepted(category);
} /* remove_shop_category() */

private void setup_default_category() {
   if (!query_controller()->is_valid_category(_default_category) &&
       _default_category) {
      add_shop_category(_default_category);
   }
} /* setup_default_category() */

/**
 * This method returns the current default category for the shop.
 * @return the default category
 */
string query_default_category() {
   return _default_category;
} /* query_default_category() */

/** @ignore yes */
void set_controller(string name) {
   ::set_controller(name);
   setup_default_category();
} /* set_controller() */

/**
 * You should run this when you change the system to the category
 * based system.  It will go through and set anything without
 * a category to have a category.
 */
void update_categories() {
   object *obs;
   object ob;

   //
   // Zip through an update all of the existing things to be in the
   // default category.
   //
   obs = query_controller()->query_sell_list_obs();
   foreach (ob in obs) {
      if (!query_controller()->query_category_of_shop_object(ob)) {
         query_controller()->change_category_of_shop_object(ob, _default_category);
      }
   }
} /* set_controller() */

/**
 * This correctly cases the name.
 * @return the correctly cases name, or 0 for failure
 */
string query_real_category_name(string in_cat) {
   string* categories;
   string cat;

   categories = query_controller()->query_categories();
   foreach (cat in categories) {
      if (lower_case(cat)[0..strlen(in_cat)-1] == lower_case(in_cat)) {
          return cat;
      }
   }
   return 0;
} /* query_real_category_name() */

/**
 * This method sets a category on the sellables.  It is called from
 * within the craft shop code itself.
 * @param func the function to call on exit
 * @param value the current value of the item
 * @param name the name of object
 * @param sellables the items for sale
 */
protected void setup_category(function func,
                              int value,
                              string name,
                              object* sellables) {
   string* categories;
   string cat;
   int i;

   //
   // Check to see if this name/owner choice has a category already.
   //
   cat = query_controller()->query_category_of(name,
                                               this_player()->query_name());
   if (cat) {
      evaluate(func, value, name, sellables, cat);
   } else {
      categories = query_controller()->query_categories();
      if (sizeof(categories) == 1) {
         write("There is only category, putting it into the default " +
               categories[0] + ".\n");
         evaluate(func, value, name, sellables, categories[0]);
      } else {
         write("Categories:\n");
         for (i = 0; i < sizeof(categories); i++) {
            write(sprintf("%c) %s\n", i + 'A', categories[i]));
         }
         write("Which category do you wish to put '" + name + "' into? ");
         input_to((: choose_category :),
                  0,
                  func,
                  value,
                  name,
                  sellables,
                  categories);
      }
   }
} /* setup_category() */

private void complete_cat_sale(int value,
                               string name,
                               object* sellables,
                               string category,
                               function func) {
   evaluate(func, value, name, sellables, category);
} /* complete_cat_sale() */

/**
 * This method chooses which category to set the item in.
 * @param category the category to choose
 * @param func the function to call on exit
 * @param value the current value of the item
 * @param name the name of object
 * @param sellables the items for sale
 */
protected void choose_category(string category,
                              function func,
                              int value,
                              string name,
                              object* sellables,
                              string* categories) {
   string real_category;

   if (!strlen(category)) {
      write("Ok, stopping the sell.\n");
      return ;
   }

   if (strlen(category) == 1) {
      category = lower_case(category);
      if (category[0] < 'a' ||
          category[0] >= 'a' + sizeof(categories)) {
         write("Sorry, " + category + " is out of range.  Try again.\n");
         write("Which category do you wish to put '" + name + "' into? ");
         input_to((: choose_category :),
                  0,
                  func,
                  value,
                  name,
                  sellables,
                  categories); 
         return ;
      } else {
         real_category = categories[category[0] - 'a'];
      }
   } else {
      real_category = query_real_category_name(category);
      if (!real_category) {
         write("Sorry, " + category + " is not a category.\n");
         write("Which category do you wish to put '" + name + "' into? ");
         input_to((: choose_category :),
                  0,
                  func,
                  value,
                  name,
                  sellables,
                  categories);
         return ;
      }
   }
   if (!real_category) {
      setup_category(func, value, name, sellables);
   } else {
      if (_cat_extra_sell_check) {
         evaluate(_cat_extra_sell_check,
                  (: complete_cat_sale :),
                  value, name, sellables, real_category, func);
      } else {
         complete_cat_sale(value, name, sellables, real_category, func);
      }
   }
} /* choose_category() */

/**
 * This method lists a specific category of items.
 * @param category the category to list
 */
int do_list_category(string category) {
   string place;
   string info;
   object *obs;
   object ob;
   mixed *morestuff;
   string ret;
   string list;

   // Check to make sure the shop is open.
   if (!check_open(this_player(), "list")) {
      return 0;
   }

   list = query_real_category_name(category);

   if (!list ||
       !query_controller()->is_valid_category(list)) {
      add_failed_mess("The category " + category + " does not exist.\n");
      return 0;
   }

   info = query_controller()->query_category_information(list);
   /* Do a list of the wombles... */
   obs = query_controller()->query_sell_list_obs();
   if (!sizeof(obs)) {
      add_failed_mess("The shop is empty.\n", ({ }));
      return 0;
   }

   place = query_property("place");
   if (!place) {
      place = "default";
   }

   obs = filter(obs, (: query_controller()->query_category_of_shop_object($1)  == $2:), list );
   ret = "";
   ret += list + ":\n";
   if (info) {
      ret += "$I$3=" + info + "\n";
   }
   morestuff = unique_array(obs, (: query_controller()->query_id_of_shop_object($1) :));
   morestuff = sort_array(morestuff, (:
                strcmp(query_controller()->query_id_of_shop_object($1[0]),
                       query_controller()->query_id_of_shop_object($2[0])) :) );
   foreach (obs in morestuff) {
      ob = obs[0];
      ret += "$I$9=$C$   " + query_controller()->query_id_of_shop_object(ob) +
            ") $C$" + ob->short() + " for " +
            MONEY_HAND->money_value_string(ob->query_value(), place) + "; " +
            query_num(sizeof(obs)) + " left.\n";
   }
   write("$P$Shop list" + list + "$P$" + ret);
   add_succeeded_mess( ({ "", "$N browses through the inventory.\n" }) );
   return 1;
} /* do_list_category() */

/**
 * @ignore yes
 */
int do_list() {
   string place;
   object *obs;
   mixed *stuff;
   mixed *morestuff;
   string ret;
   string* categories;
   string cat;
   string desc;
   object ob;

   // Check to make sure the shop is open.
   if (!check_open(this_player(), "list")) {
      return 0;
   }

   /* Do a list of the wombles... */
   obs = query_controller()->query_sell_list_obs();
   if (!sizeof(obs)) {
      add_failed_mess("The shop is empty.\n", ({ }));
      return 0;
   }

   place = query_property("place");
   if (!place) {
      place = "default";
   }

   categories = query_controller()->query_categories();
   stuff = unique_array(obs, (: query_controller()->query_category_of_shop_object($1) :), 12 );
   ret = "";
   foreach (obs in stuff) {
      if (!query_controller()->query_category_of_shop_object(obs[0])) {
         cat = "No category";
         foreach (ob in obs) {
            query_controller()->change_category_of_shop_object(ob, 
                              query_default_category());
         }
         cat = query_default_category();
      } else {
         cat = query_controller()->query_category_of_shop_object(obs[0]);
         categories -= ({ query_controller()->query_category_of_shop_object(obs[0]) });
         desc = query_controller()->query_category_description(cat);
      }
      morestuff = unique_array(obs, (: query_controller()->query_id_of_shop_object($1) :));
      //morestuff = unique_array(obs, (: $1->query_short() :));
      ret += "$I$3=" + cat + ": " + sizeof(morestuff) + 
             " different item" + (sizeof(morestuff)>1?"s":"") + " for sale.\n";
      if (desc) {
         ret += "$I$6=   " + desc + "\n";
      }
   }

   if (sizeof(categories) > 1) {
      ret += "$I$0=The categories " + query_multiple_short(categories) +
             " do not have anything for sale.\n";
   } else if (sizeof(categories)) {
      ret += "$I$0=The category " + query_multiple_short(categories) +
             " does not have anything for sale.\n";
   }

   ret += "\n$I$0=Please list each category for the items in the category.\n";
   write("$P$Shop list$P$" + ret);
   add_succeeded_mess( ({ "", "$N browses through the inventory.\n" }) );
   return 1;
} /* do_list() */

int do_change_category(object *obs, string new_category) {
   object *frog;

   // Check to make sure the shop is open.
   if (!check_open(this_player(), "change")) {
      return 0;
   }

   frog = filter(obs, (: $1->short() == $2->short() :), obs[0]);
   if (sizeof(frog) != sizeof(obs)) {
      add_failed_mess("You cannot change the category of more than one "
                      "type of object at once.\n", obs);
      return 0;
   }

   if (!is_able_to_change(obs[0])) {
      add_failed_mess("You do not own $I.\n", obs);
      return 0;
   }

   new_category = query_real_category_name(new_category);
   if (!new_category ||
       !query_controller()->is_valid_category(new_category)) {
      add_failed_mess("You must choose a category that exists.\n");
      return 0;
   }

   if (query_controller()->change_category_of_shop_object(obs[0], new_category)) {
      add_succeeded_mess(({ "You change the category of $I to " +
                               new_category + ".\n",
                               "$N changes the category of $I.\n" }),
                         ({ obs[0] }) );
   } else {
      add_failed_mess("Unable to change to category " + new_category + ".\n", obs);
   }

   return 1;
} /* do_change_category() */

/**
 * This method sets the extra sell function for the shop.  The extra sell
 * function is called after the sell process is almost finished.  It can
 * check for extra things needed in the sell process, like which page of
 * the book to browse.
 * @param func the extra sell function
 * @see query_extra_sell_check()
 * @see complete_sell()
 */
void set_extra_sell_check(function func) {
   _cat_extra_sell_check = func;
} /* set_extra_sell_check() */

/**
 * This method returns the extra sell check function.
 * @return the extra sell check function
 * @see set_extra_sell_check()
 */
function query_extra_sell_check() {
   return _cat_extra_sell_check;
} /* query_extra_sell_check() */

/**
 * This method sets the default category for the item.  This must be setup
 * for something useful to happen.
 * @param category the default category
 */
void set_default_category(string category) {
  _default_category = category;
} /* set_default_category() */
