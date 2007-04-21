/**
 * This room will handle craft shop like things, where you sell stuff to
 * the store and it will sell it onto other players after taking
 * a margin.  This can be used for potions, pottery, swords, anything!
 * This has a few small hooks in here to handle categories in the
 * sell process.  Most of the category handling code is dealt with in
 * the category based craft shop.
 * @author Pinkfish
 * @started Thu Feb  5 15:39:57 CST 1998
 */
#include <money.h>
#include <shops/craft_shop.h>

inherit "/std/shops/inherit/open_shop";

private nosave object _controller;
private nosave string _save_dir_name;
private nosave function _allowed_to_sell;
private nosave function _when_sold_func;
private nosave int _max_sell_per_person;
private nosave int _cut;
private nosave int _minimum_cost;
private nosave function _extra_sell_check;
private nosave int _always_ask_price;
private nosave int _use_name;
private nosave int _no_royalty_commands;

int do_sell(object *obs);
void set_controller(string name);
int do_buy(object *obs, int force);
int do_delete(string id);
int do_browse(object *obs);
int do_change(object *obs, string str);
int do_change_name(object *obs, string str);
void set_allowed_to_sell(function f);
void set_minimum_cost(int i);
protected void sell_name(string name, object *sellable);
protected void sell_choice(string choice, object *sellable, string *names);
protected void determine_cost(string cost, string name, object *sellable);
protected void confirm_sale(string answer, int value, string name,
                            object *sellable, string category);
protected void confirm_sell_name_choice(string answer, string name,
                                        object *sellable);
protected void complete_sale(int value, string name, object *sellable, string category);
protected void do_confirm_sale(int value, string name, object *sellable);
void set_cut(int value);
int query_cut();
int check_open(object player, string command);
mixed query_property(string name);
string the_short();
string query_craft_shop_save_dir();

void create() {
   /* Always return true. */
   if (!_allowed_to_sell) {
      set_allowed_to_sell((: 1 :));
   }
   /* The default cut is 10% */
   if (!_cut) {
      set_cut(10);
   }
   /* Make it really 12 am pence. */
   if (!_minimum_cost) {
      set_minimum_cost(12 * 4);
   }
   if (!_controller && _save_dir_name) {
      set_controller("/std/shops/controllers/craft_shop_controller");
      _controller->load_it();
   }
} /* create() */

/**
 * This attempts to figure out how much the person is telling us the
 * item costs.
 * @param str the string to get the value of
 */
int value_from_string(string str, string place) {
   return MONEY_HAND->value_from_string(str, place);
} /* value_from_string() */

/**
 * This method handles the saving of the file to the disk.
 * @param type the type of file to save
 * @param value the value to save to it
 * @param fname the file name is optional and only used for data files
 */
protected void do_save_file(int type, mixed value, string fname) {
   string tmp, file;
   string dir;
   
   switch (type) {
   case CRAFT_SHOP_MAIN_SAVE_FILE :
     debug_printf("attempting to write main");
#ifdef USE_RAMDISK
     tmp = replace_string(query_craft_shop_save_dir(), "/save/",
                          "/save/ramdisk/");
     file = tmp + ".o";
#endif
     if(!file) {
       file = query_craft_shop_save_dir() + ".o";
     }
     
     debug_printf("writing main %s", file);
     tmp = save_variable(value);
     if (file_size(query_craft_shop_save_dir() + ".o") != -1) {
       unguarded( (: rename(query_craft_shop_save_dir() + ".o",
                            query_craft_shop_save_dir() + ".o.bak") :));
     }
     if(unguarded( (: write_file($(file), $(tmp), 1) :) )) {
       unguarded( (: rm($(file) + ".bak") :));
     } else {
       unguarded( (: rename($(file) + ".bak", $(file)) :));
     }
     break;
   case CRAFT_SHOP_DATA_SAVE_FILE :
#ifdef USE_RAMDISK
     //file = replace_string(query_craft_shop_save_dir() + "/" + fname, "/save/",
     //                     "/save/ramdisk/");
     dir = replace_string(query_craft_shop_save_dir(), "/save/", "/save/ramdisk/");
     if (file_size(dir) == -2) {
        file = dir + "/" + fname;
     }
#endif
     if(!file) {
       file = query_craft_shop_save_dir() + "/" + fname;
     }

     debug_printf("writing data %s", file);

     tmp = save_variable(value);
     if (!unguarded( (: write_file($(file), $(tmp), 1) :))) {
        debug_printf("failed to write data to %s", file);
     }
     break;
   case CRAFT_SHOP_REMOVE_DATA_SAVE_FILE :
#ifdef USE_RAMDISK
     file = replace_string(query_craft_shop_save_dir() + "/" + fname, "/save/",
                          "/save/ramdisk/");
     unguarded( (: rm($(file)) :) );
#endif     
     unguarded((: rm(query_craft_shop_save_dir() + "/" + $(fname)) :));

     break;
   default :
     printf("Unknown file type to write in %O, of %O\n", __FILE__, type);
     break;
   }
} /* do_save_file() */

/**
 * This method handles the reading of the file to the disk.
 * @param type the type of file to read
 * @param fname the file name is optional and only used for data files
 * @return the value read
 */
protected string do_read_file(int type, string fname) {
   string tmp, file;

   switch (type) {
   case CRAFT_SHOP_MAIN_SAVE_FILE :
#ifdef USE_RAMDISK
     tmp = replace_string(query_craft_shop_save_dir(), "/save/",
                          "/save/ramdisk/");
     if(unguarded((: file_size($(tmp) + ".o") :)) > 0) {
       file = tmp + ".o";
     }
#endif
     if(!file) {
       tmp = query_craft_shop_save_dir();
       if(unguarded((: file_size($(tmp) + ".o") :)) > 0) {
         file = tmp + ".o";
       }
       // Check for the backup.
       if(unguarded((: file_size($(tmp) + ".o.bak") :)) > 0) {
         file = tmp + ".o.bak";
       }
     }

     // No file exists yet.
     if (!file) {
        debug_printf("No file found at %s or %s\n", tmp + ".o", tmp + ".o.bak");
        return 0;
     }
     
     tmp = unguarded( (: read_file($(file)) :) ); 
     if(tmp)
       return restore_variable(tmp);
     return 0;
     
   case CRAFT_SHOP_DATA_SAVE_FILE :
#ifdef USE_RAMDISK
     tmp = replace_string(query_craft_shop_save_dir() + "/" + fname,
                           "/save/", "/save/ramdisk/");
     if(unguarded((: file_size($(tmp)) :)) > 0)
       file = tmp;
#endif
     if(!file)
       file = query_craft_shop_save_dir() + "/" + fname;

     debug_printf("reading %s", file);
     tmp = unguarded( (: read_file($(file)) :) );
     if (tmp) {
       return restore_variable(tmp);
     }
     return 0;
   default :
     printf("Unknown file type to read in %O, of %O\n", __FILE__, type);
     break;
   }
} /* do_read_file() */

/**
 * This method allows you to cause the shop to load itself up.
 */
void reload_shop() {
   set_controller("/std/shops/controllers/craft_shop_controller");
   _controller->load_it();
} /* reload_shop() */

/**
 * This method sets the controller object for this shop.
 * @param controller the controller name as a string
 * @see query_controller()
 */
void set_controller(string name) {
   if (_controller) {
      _controller->dest_me();
   }
   _controller = clone_object(name);
   _controller->set_save_function((: do_save_file($1, $2, $3) :));
   _controller->set_load_function((: do_read_file($1, $2) :) );
} /* set_controller() */

/**
 * This method returns the current value of the controller.
 * @return the current controller
 * @see set_controller()
 */
object query_controller() {
   return _controller;
} /* query_controller() */

/**
 * This method sets the save file associated with the controller.  The save
 * file should actually be a directory, the name of the directory plus a
 * .o will be used for the save files.  The directory must exist or weird
 * errors will occur.
 * This should always be set in the setup function of the room.
 * @param fname the save file
 * @see set_controller()
 * @see query_save_dir()
 */
void set_save_dir(string fname) {
   _save_dir_name = fname;
} /* set_save_dir() */

/**
 * This method returns the file name of the save file.
 * @return the file name of the save file
 * @see set_save_dir()
 */
string query_craft_shop_save_dir() {
   return _save_dir_name;
} /* query_craft_shop_save_dir() */

/**
 * This method sets the function to use to check to see if the object
 * is able to be sold here or not.  The function will be called with
 * one parameter, that is the object being check for buyability.
 * @param func the function to check objects with
 * @see query_allowed_to_sell()
 */
void set_allowed_to_sell(function func) {
   _allowed_to_sell = func;
} /* set_allowed_to_sell() */

/**
 * This method returns the function used to check to see if an object
 * is able to be sold here or not.
 * @return func the function to use for checking
 * @see set_allowed_to_sell()
 */
function query_allowed_to_sell() {
   return _allowed_to_sell;
} /* query_allowed_to_sell() */

/**
 * This method sets the maxium allowed number of objects to be sold
 * by each person into the shop.
 * @param num the maximum number to be sold
 * @see query_max_sell_per_person()
 */
void set_max_sell_per_person(int num) {
   _max_sell_per_person = num;
} /* set_max_sell_per_person() */

/**
 * This methods returns the maximum number of allowed objects to be
 * sold by each person into the shop.
 * @return the maximum number to be sold
 * @see set_max_sell_per_person()
 */
int query_max_sell_per_person() {
   return _max_sell_per_person;
} /* query_max_sell_per_person() */

/**
 * This method sets the flag which makes the shop always ask for a price
 * when it attempts to save something.
 * @param ask_price the flag
 */
void set_always_ask_price(int ask_price) {
   _always_ask_price = ask_price;
} /* set_always_ask_price() */

/**
 * This method returns the flag on this object that tells us if the shop
 * will always ask for a price
 * @return the flag
 */
int query_always_ask_price() {
   return _always_ask_price;
} /* query_always_ask_price() */

/**
 * This method sets the flag which makes the shop use the name of the
 * item as the sell type always.
 * @param flag 1 to make it force the use name ability
 */
void set_use_name(int flag) {
   _use_name = flag;
} /* set_always_ask_price() */

/**
 * This method returns the flag for the use name of this shop.
 * @return 1 if the name is used, 0 if not
 */
int query_use_name() {
   return _use_name;
} /* query_always_ask_price() */

/**
 * This method will return the maximum price this item is allowed to
 * be sold for.  This will return 0 if there is no maximum price
 * @return 0 if there is no maximum price, or maximum price
 */
int query_maximum_sale_value(string person, object *obs) {
   return 0;
} /* query_maximum_sale_value() */

/**
 * This method returns any extra information about prices that the
 * shop wants to display before entering a cost.  This can be
 * overridden in higher inherits to control this more effectively.
 * @return an extra string to print
 */
string query_extra_price_information(string person, object *obs) {
   return "";
} /* query_extra_price_information() */

/**
 * @ignore yes
 * Sets this room as a shop, to interface with the shoplift command.
 * @return always returns 1
 */
int query_shop() {
   return 1;
} /* query_shop() */

/**
 * @ignore yes
 * This method returns the list of things that can be shoplifted from this
 * shop based on the input list.
 */
object *query_shop_lift_items(string str, object player) {
   object *stuff;
   object *pk_ok;
   object ob;

   stuff = match_objects_for_existence(str, ({ _controller->query_sell_list() }));
   pk_ok = ({ });
   foreach (ob in stuff) {
      if (!pk_check(_controller->query_owner_of_shop_object(ob),
                      player, 1)) {
         pk_ok += ({ ob });
      }
   }
   if (sizeof(pk_ok) != sizeof(stuff)) {
      player->add_failed_mess(previous_object(),
                      "You cannot shoplift $I since you cannot "
                      "shoplift from non-player killers.\n", stuff);
   }
   return pk_ok;
} /* query_shop_lift_items() */

/**
 * This method is called when something is bought and tells us how much
 * has been spent.
 * @param value of the the items sold
 * @param obs the objects bought
 * @param player who bought them
 * @param sellers the people whose items were bought
 * @param names the names of the items bought
 * @param cats the categories of the items bought
 * @param values the values of each item
 */
void inform_of_buy(int value, object *obs, object player, string *sellers,
                   string *names, string *cats, int *values) {
} /* inform_of_buy() */

/**
 * @ignore yes
 * This place is always marked as 'no steal' for now.
 */
int query_no_steal() {
   return 1;
} /* query_no_steal() */

/**
 * This allows the system to override this for player run shops so the
 * owner can discount peoples items.
 * @param ob the object to test
 * @return 1 if they can modify the item, 0 if not
 */
int is_able_to_change(object ob) {
   return _controller->query_owner_of_shop_object(ob) ==
       this_player()->query_name();
} /* is_able_change_ob() */

/**
 * This method checks to see if this item is allowed to be sold in this shop.
 * It assumes it is called from a add_command() method and sets up fail
 * messages accordingly.
 * @param obs the objects to check
 * @param name the name of the person doing the selling
 * @param sellable_names the names of the items to sell
 */
int is_allowed_to_sell(object* obs, string name, string *sellable_names) {
   if ((sizeof(sellable_names)+sizeof(obs)) > query_max_sell_per_person() &&
       query_max_sell_per_person()) {
      add_failed_mess("Cannot sell $I, since you already have " +
                      number_as_string(sizeof(sellable_names)) +
                      " items listed with a maximum of " +
                      number_as_string(query_max_sell_per_person()) +
                      ".\n", obs);
      return 0;
   }
   return 1;
} /* is_allowed_to_sell() */

/** @ignore yes */
void init() {
   string storeroom;

   add_command("sell", "<indirect:object:me>");
   //add_command("sell", "<indirect:object:me> as <string'item id'>",
   //             (: do_sell_id($0, $4[1]) :));
   add_command("list", "");
   if (query_controller()) {
      storeroom = file_name(_controller->query_sell_list());
      add_command("buy", "<indirect:object:" + storeroom + ">",
                  (: do_buy($1, 0) :));
      add_command("browse", "<indirect:object:" + storeroom + ">",
                  (: do_browse($1) :));
      add_command("change", "price of <indirect:object:" + storeroom +
                            "> to <string>", (: do_change($1, $4[1]) :));
      add_command("change", "name of <indirect:object:" + storeroom +
                            "> to <string>", (: do_change_name($1, $4[1]) :));
   }
   if (!_no_royalty_commands) {
      add_command("collect", "royalties");
      add_command("royalties", "");
   }
   add_command("delete", "<string'sell id'>",
               (: do_delete($4[0]) :));
} /* init() */

/**
 * This method tries to sell an object as a name that already exists in
 * the shop.
 * @param obs the objects to sell
 * @param id the id to sell them as
 * @see do_sell()
 * @see do_list()
 * @see do_buy()
 */
int do_sell_id(object* obs, string id, int force) {
   object *sellable;
   object *cannot;
   object *worn;
   string name;
   string *sellable_names;

   // Check to make sure the shop is open.
   if (!check_open(this_player(), "sell")) {
      return 0;
   }

   sellable = filter(obs, query_allowed_to_sell());
   if (!sizeof(sellable)) {
      add_failed_mess("This shop does not deal in $I.\n", obs);
      return 0;
   }

   worn = this_player()->query_holding() + this_player()->query_armours();
   cannot = filter(sellable, (: member_array($1, $2) != -1 :), worn);
   if (sizeof(cannot)) {
       sellable -= cannot;
       tell_object(this_player(), "You decide not to sell " +
         query_multiple_short(cannot, "the") + ", because "
         "you are wearing or holding $V$0=it,them$V$.\n");
      if (!sizeof(sellable))  return 0;
   }

   name = this_player()->query_name();
   if (!is_allowed_to_sell(sellable, name, sellable_names)) {
      return 0;
   }


   if (query_cut() != 100) {
      write("Selling an item to this shop does not automatically give you the "
            "money.  You will get a cut of " + (100 - query_cut()) +
            "% of the sale price once the item has been sold by the shop.\n");   }

   //
   // Ok, find the id and fill in all the needed bits.
   //

} /* do_sell_id() */

/**
 * This method sells an object into the craft shop.
 * @param obs the objects to sell
 * @return 1 on success, 0 on failure
 * @see do_list()
 * @see do_buy()
 * @see do_sell_id()
 */
int do_sell(object *obs) {
   object *sellable;
   object *cannot;
   object *worn;
   string name;
   string *sellable_names;
   int i;
   int cost;
   string place;

   // Check to make sure the shop is open.
   if (!check_open(this_player(), "sell")) {
      return 0;
   }

   sellable = filter(obs, query_allowed_to_sell());
   if (!sizeof(sellable)) {
      add_failed_mess("This shop does not deal in $I.\n", obs);
      return 0;
   }

   worn = this_player()->query_holding() + this_player()->query_armours();
   cannot = filter(sellable, (: member_array($1, $2) != -1 :), worn);
   if (sizeof(cannot)) {
       sellable -= cannot;
       tell_object(this_player(), "You decide not to sell " +
         query_multiple_short(cannot, "the") + ", because "
         "you are wearing or holding $V$0=it,them$V$.\n");
      if (!sizeof(sellable))  return 0;
   }

   name = this_player()->query_name();
   if (!is_allowed_to_sell(sellable, name, sellable_names)) {
      return 0;
   }


   if (query_cut() != 100) {
      write("Selling an item to this shop does not automatically give you the "
            "money.  You will get a cut of " + (100 - query_cut()) +
            "% of the sale price once the item has been sold by the shop.\n");
   }

   if (_use_name) {
      // We use the name of the item.
      sell_name(query_multiple_short(sellable, 0, 1), sellable);
      add_succeeded_mess(({ "", "$N starts to sell $I to the shop.\n" }),
                         sellable);
      return 1;
   }

   obs->set_no_join();
   sellable_names = _controller->query_owner_sellables(name);
   sellable_names = uniq_array(sellable_names);
   if (!sizeof(sellable_names)) {
      write("You are not currently selling any items.  What item name "
            "would you like to list " + query_multiple_short(sellable) +
            " under?  (Blank line to quit)\nList name: ");
      input_to((: sell_name :), 0, sellable);
   } else {
      place = query_property("place");
      if (!place) {
         place = "default";
      }
      write("You currently have the following listed for sale:\n");
      for (i = 0; i < sizeof(sellable_names); i++) {
         cost = _controller->query_list_object_cost(sellable_names[i],
                                   this_player()->query_name());
         write(sprintf("  %c) %s (%s)\n", 'a' + i, sellable_names[i],
                       MONEY_HAND->money_value_string(cost, place)));
      }
      write("Select the letter of the choice to add to the item, or type "
            "'create' to create a new listing name.\n"
            "Your choice ([a-" + sprintf("%c", 'a' + sizeof(sellable_names) -1) +
            "],create) ? ");
      input_to((: sell_choice :), 0, sellable, sellable_names);
   }
   add_succeeded_mess(({ "", "$N starts to sell $I to the shop.\n" }),
                      sellable);
   return 1;
} /* do_sell() */


/**
 * This method makes sure a name is not too long and contains
 * no colour codes.
 * @param name the name to list the object as
 * @return 0 if name is unacceptable, 1 if it's OK
 */
protected int check_sell_name( string name ) {

   if (strlen(name) > CRAFT_SHOP_MAX_NAME_LENGTH) {
      write("Sorry, that name is too long.  It can be a maximum of " +
            CRAFT_SHOP_MAX_NAME_LENGTH + " characters.\n");
      return 0;
   }

   if (strsrch(name, "%^") != -1) {
      write("Sorry, you cannot use colour sequences in names.\n");
      return 0;
   }

   // Name was OK
   return 1;

} // check_sell_name()

/**
 * This method determines the name to list the object under.  This is
 * called when a new name is used.
 * @param name the name to list the object as
 * @param sellable the list of sellable objects
 */
protected void sell_name(string name, object *sellable) {
   if (name == "" || name == 0) {
      write("Ok, cancelling the sale of " + query_multiple_short(sellable) +
            ".\n");
      return ;
   }

   if ( !check_sell_name( name ) ) {
      input_to((: sell_name :), 0, sellable);
      write("What name would you like to list " +
            query_multiple_short(sellable) + " under? ");
      return;
   }

   write("You wish to sell " + query_multiple_short(sellable) +
         " as '" + name + "' (y/n)? ");
   input_to((: confirm_sell_name_choice :), 0, name, sellable);

} /* sell_name() */


/**
 * This method is used to confirm the choice of name to use when
 * selling the object.
 * @param answer the confirmation choice
 * @param name the name to sell it as
 * @param sellable the list of objects to sell
 */
protected void confirm_sell_name_choice(string answer, string name,
                                        object *sellable) {
   answer = lower_case(answer);
   if (strlen(answer) > 0 && ( answer[0] == 'n' || answer[0] == 'q' ) ) {
      write("Ok, cancelling the sale of " + query_multiple_short(sellable) +
            ".\n");
      return ;
   }
   if (!strlen(answer) || answer[0] != 'y') {
      write("Unknown selection.\n");
      write("You wish to sell " + query_multiple_short(sellable) +
            " as '" + name + "' (y/n)? ");
      input_to( (: confirm_sell_name_choice :), 0, name, sellable);
      return ;
   }
   write(query_extra_price_information(this_player()->query_name(), sellable));
   write("How much money do you wish to sell the item '" + name + "' for? ");
   input_to((: determine_cost :), 0, name, sellable);
} /* confirm_sell_name_choice() */

/**
 * This method is used when there is a list of object already listed.  It
 * will check to see if the choice entered is valid and then use that
 * name when selling the object if it is.
 * @param choice the selected choice
 * @param sellable the array of sellable objects
 * @param names the names of choices for name
 */
protected void sell_choice(string answer, object *sellables, string *names) {
   int cost;
   string place;
   int pos;

   if (sizeof(names) >= 17) {
      if (answer == "" || answer[0..1] == "qu") {
         write("Ok, cancelling the sale of " + query_multiple_short(sellables) +
             ".\n");
         return 0;
      }
   } else if (answer == "" || answer[0] == 'q') {
      write("Ok, cancelling the sale of " + query_multiple_short(sellables) +
            ".\n");
      return ;
   }
   answer = lower_case(answer);
   if (answer == "create") {
      write("Please enter the name you wish to list " +
            query_multiple_short(sellables) + " as.\nList name: ");
      input_to((: sell_name :), 0, sellables);
      return ;
   }
   if (strlen(answer) > 1 ||
       (answer[0] < 'a' || answer[0] >= 'a' + sizeof(names))) {
      write(sprintf("Your response must be between 'a' and '%c' or "
                    "'create'.\nChoice? ", 'a' + sizeof(names) - 1));
      input_to((: sell_choice :), 0, sellables, names);
      return ;
   }
   pos = answer[0] - 'a';
   cost = _controller->query_list_object_cost(names[pos],
                                              this_player()->query_name());
   if (!cost || _always_ask_price) {
      /* Hmm, the cost is not working, we need to set a cost as well. */
      write("How much money do you wish to list the item '" + names[pos] +
            "' for? ");
      write(query_extra_price_information(this_player()->query_name(),
                                          sellables));
      input_to((: determine_cost :), 0, names[pos], sellables);
   } else {
      place = query_property("place");
      if (!place) {
         place = "default";
      }
      do_confirm_sale(cost, names[pos], sellables);
   }
} /* sell_choice() */

/**
 * This method figured out the cost of the item based on the value they
 * type in.
 * @param cost the cost of the item
 * @param name the name of the item
 * @param sellable the array of objects to sell
 */
protected void determine_cost(string cost, string name, object *sellable) {
   int max;
   int value;
   string place;

   place = query_property("place");
   if (!place) {
      place = "default";
   }
   value = value_from_string(cost, place);
   if (value < _minimum_cost) {
      if (!strlen(cost) ||
          lower_case(cost)[0] == 'q') {
         write("Aborting.\n");
         return ;
      }
      write("You must list your item as costing more than " +
            MONEY_HAND->money_value_string(_minimum_cost, place) + ".\n");
      write(query_extra_price_information(this_player()->query_name(),
                                          sellable));
      write("What do you wish to list " + name + " for? ");
      input_to("determine_cost", 0, name, sellable);
      return ;
   }

   max = query_maximum_sale_value(this_player()->query_name(), sellable);
   if (value > max && max) {
      write("You must list your item as costing less than " +
            MONEY_HAND->money_value_string(max, place) + ".\n");
      write(query_extra_price_information(this_player()->query_name(),
                                          sellable));
      write("What do you wish to list " + name + " for? ");
      input_to("determine_cost", 0, name, sellable);
      return ;
   }

   do_confirm_sale(value, name, sellable);
} /* determine_cost() */

private void confirm_sale_question(int value,
                                   string name,
                                   object *sellables,
                                   string category) {
   string place;

   place = query_property("place");
   if (!place) {
      place = "default";
   }
   write("Confirming your sale of " + query_multiple_short(sellables) +
         " as '" + name +
         "' " + (category?"in category " + category + " ":"") +
         "for " + MONEY_HAND->money_value_string(value, place) + " (y/n)? ");
   input_to((: confirm_sale :), 0, value, name, sellables, category);
} /* confirm_sale_question() */

/**
 * This method will ask the person to confirm the sale.  It will check
 * the extra_sell_check function and then call the extra sale thing.
 * @param value the value of the thing being sold
 * @param name the name the thing is sold for
 * @param sellables the things to sell
 */
protected void do_confirm_sale(int value, string name, object *sellables) {
   if (_extra_sell_check) {
      evaluate(_extra_sell_check,
               (: confirm_sale_question :),
               value, name, sellables);
   } else {
      confirm_sale_question(value, name, sellables, 0);
   }
} /* do_confirm_sale() */

/**
 * This method confirms the sale after the cost and name have been
 * determined.
 * @param answer the answer to confirm with
 * @param value the value of the object
 * @param name the name of the object
 * @param sellable the list of sellable objects
 * @param category the category of the object, 0 if none
 */
protected void confirm_sale(string answer, int value, string name,
                            object *sellable,
                            string category) {
   if (answer == "" || answer[0] == 'q' || answer[0] == 'Q' ||
       answer[0] == 'n' || answer[0] == 'N') {
      write("Ok, aborting sale of " + query_multiple_short(sellable) + ".\n");
      return 0;
   }
   if (answer[0] != 'y' && answer[0] != 'Y') {
      write("Please answer yes or no.\n");
      confirm_sale_question(value, name, sellable, category);
      return 0;
   }
   complete_sale(value, name, sellable, category);
} /* confirm_sale() */

/**
 * This method is called to complete the sale completely.  It is split
 * up into a second function to allow the extra sell stuff
 * to work neatly.
 * @param value the value of the objects to sell
 * @param name the list name
 * @param sellable the list of objects to sell
 * @param category the category of the object, 0 if none
 */
protected void complete_sale(int value, string name, object *sellable,
                             string category) {
   object *bought;
   string place;
   object ob;

   /* First run them through the when sold function. */
   if (_when_sold_func) {
      foreach (ob in sellable) {
         evaluate(_when_sold_func, ob);
      }
   }
   bought = _controller->buy_objects(sellable, name, value,
                                   this_player()->query_cap_name(), category);
   place = query_property("place");
   if (!place) {
      place = "default";
   }
   if (sizeof(bought)) {
      write("Sold " + query_multiple_short(bought) + ", listed as '" +
            name + "' " + (category?"in category " + category + " ":"") +
            "for " + MONEY_HAND->money_value_string(value, place) +
            ".\nYou will need to come back later to pick up your royalties "
            "for the sale.\n");
      say(this_player()->query_cap_name() + " sold " +
          query_multiple_short(bought) + " to " + the_short() + ".\n");
   } else {
      write("Unable to sell " + query_multiple_short(sellable) + ".\n");
   }
} /* confirm_sale() */

/**
 * This method will list the current set of objects which are
 * available to buy.
 * @return 1 on success, 0 on failure
 * @see do_sell()
 * @see do_buy()
 */
int do_list() {
   string place;
   object *obs;
   object ob;
   mixed *stuff;
   string ret;

   // Check to make sure the shop is open.
   if (!check_open(this_player(), "list")) {
      return 0;
   }

   /* Do a list of the wombles... */
   obs = _controller->query_sell_list_obs();
   if (!sizeof(obs)) {
      add_failed_mess("The shop is empty.\n", ({ }));
      return 0;
   }

   place = query_property("place");
   if (!place) {
      place = "default";
   }

   stuff = unique_array(obs, (: _controller->query_id_of_shop_object($1) :) );
   stuff = sort_array(stuff, (:
                strcmp(_controller->query_id_of_shop_object($1[0]),
                       _controller->query_id_of_shop_object($2[0])) :) );
   ret = "";
   foreach (obs in stuff) {
      ob = obs[0];
      ret += "$I$9=$C$" + _controller->query_id_of_shop_object(ob) +
            ") $C$" + ob->short() + " for " +
            MONEY_HAND->money_value_string(ob->query_value(), place) + "; " +
            query_num(sizeof(obs)) + " left.\n";
   }
   write("$P$Shop list$P$" + ret);
   add_succeeded_mess( ({ "", "$N browses through the inventory.\n" }) );
   return 1;
} /* do_list() */

/**
 * This method will attempt to buy an object from the shops inventory.
 * @param str the string to match an object with
 * @return 1 on success, 0 on failure
 * @see do_sell()
 * @see do_list()
 */
int do_buy(object *obs, int force) {
   int value;
   string place;
   int player_money;
   object *bought;
   string *sellers;
   string *names;
   string *cats;
   int *values;

   // Check to make sure the shop is open.
   if (!check_open(this_player(), "buy")) {
      return 0;
   }

   place = query_property("place");
   if (!place) {
      place = "default";
   }
   player_money = this_player()->query_value_in(place);
   value = _controller->value_of_objects(obs);
   if (value > player_money) {
      add_failed_mess("You cannot buy $I as "+
                      ( sizeof( obs ) > 1 ? "they cost ":"it costs " )+
                      MONEY_HAND->money_value_string(value, place) +
                      " and you do not have that much money.\n", obs );
      return 0;
   }

   sellers = map(obs, (: _controller->query_owner_of_shop_object($1) :));
   names = map(obs, (: _controller->query_name_of_shop_object($1) :));
   cats = map(obs, (: _controller->query_category_of_shop_object($1) :));
   values = map(obs, (: $1->query_value() :));

   bought = _controller->sell_objects(obs, place, this_player(),
                                     query_cut(), force, ref value);
   if (!sizeof(bought)) {
      if (sizeof(bought) > 1) {
         add_failed_mess("Cannot move any of $I into your inventory, nothing "
                         "bought.\n", obs);
      } else {
         add_failed_mess("Unable to find any objects to buy, the shop "
                         "seems to have some stock problems.\n");
      }
      return 0;
   }
   inform_of_buy(value, bought, this_player(), sellers, names, cats, values);
   add_succeeded_mess(({ "You buy $I for " +
                       MONEY_HAND->money_value_string(value, place) + ".\n",
                         "$N buys $I.\n" }), bought);
   return 1;
} /* do_buy() */


/**
 * This method will delete a list entry without the owner getting paid.
 * @param str the string to match an object with
 * @return 1 on success, 0 on failure
 */
int do_delete(string id ) {

   if( !this_player()->query_director() ) {
      add_failed_mess( "This command is reserved for directors.\n", ({ }) );
      return 0;
   }

   if( !id || id == "" ) {
      add_failed_mess( "You have to choose a valid sell id.\n", ({ }) );
      return 0;
   }

   _controller->delete_objects(id);

   add_succeeded_mess( "$N $V some things.\n", ({ }) );

   return 1;
} /* do_delete() */


/**
 * This function browses through the current selected list of items.
 * @param obs the items to browse through
 */
int do_browse(object *obs) {
   object *real_obs;
   object ob;
   string mess;
   string read;

   // Check to make sure the shop is open.
   if (!check_open(this_player(), "browse")) {
      return 0;
   }

   if (sizeof(obs) > 10) {
      add_failed_mess("You cannot browse more than 10 things at once.\n");
      return 0;
   }

   real_obs = _controller->create_all_real_objects(this_player(), obs);
   mess = "$P$Browse$P$";
   foreach (ob in real_obs) {
      mess += ob->the_short() + ":\n" + ob->long();
      read = ob->query_readable_message();
      if (read) {
         mess += "You read " +
                 replace(ob->query_read_short(this_player()), "$name$",
                         ob->a_short()) + ":\n" +
                 ob->query_readable_message();
      }
   }
   real_obs->move("/room/rubbish");
   add_succeeded_mess(({ mess, "$N $V $I.\n" }), obs);
   return 1;
} /* do_browse() */

/**
 * This function changes the list price of an object.
 * @param obs the object to change the list price of
 * @param change the value to change the list price to
 */
int do_change(object *obs, string change) {
   object *frog;
   int value;
   string place;

   // Check to make sure the shop is open.
   if (!check_open(this_player(), "change")) {
      return 0;
   }

   frog = filter(obs, (: $1->short() == $2->short() :), obs[0]);
   if (sizeof(frog) != sizeof(obs)) {
      add_failed_mess("You cannot change the value of more than one "
                      "type of object at once.\n", obs);
      return 0;
   }

   if (!is_able_to_change(obs[0])) {
      add_failed_mess("You do not own $I.\n", obs);
      return 0;
   }

   place = query_property("place");
   if (!place) {
      place = "default";
   }

   value = value_from_string(change, place);
   if (value < _minimum_cost) {
      add_failed_mess("You cannot set $I to a value less than " +
                 MONEY_HAND->money_value_string(_minimum_cost, place) + ".\n",
                 obs);
      return 0;
   }

   _controller->change_value_of_shop_object(obs[0], value);
   add_succeeded_mess( ({ "You change the value of $I to " +
         MONEY_HAND->money_value_string(value, place) + ".\n",
         "$N changes the value of $I.\n" }), obs[0..0]);
   return 1;
} /* do_change() */

/**
 * This function changes the list name of an object.
 * @param obs the object to change the list name of
 * @param new_name the value to change the list name to
 */
int do_change_name(object *obs, string new_name) {
   object *frog;

   // Check to make sure the shop is open.
   if (!check_open(this_player(), "change")) {
      return 0;
   }

   frog = filter(obs, (: $1->short() == $2->short() :), obs[0]);
   if (sizeof(frog) != sizeof(obs)) {
      add_failed_mess("You cannot change the value of more than one "
                      "type of object at once.\n", obs);
      return 0;
   }

   if (!is_able_to_change(obs[0])) {
      add_failed_mess("You do not own $I.\n", obs);
      return 0;
   }

   if( !check_sell_name( new_name ) ) {
      add_failed_mess( "Try another name.\n", ({ }) );
      return 0;
   }

   if (_controller->change_name_of_shop_object(obs[0], new_name,
                               _controller->query_dont_use_name())) {
      add_succeeded_mess(({ "You change the name of $I to " +
                               new_name + ".\n",
                               "$N changes the name of $I.\n" }),
                         ({ obs[0] }) );
   } else {
      add_failed_mess("The name " + new_name + " is already in use.\n", obs);
   }

   return 1;
} /* do_change_name() */

/**
 * This method collects any royalties the current player is owed.
 * @return 1 on succes, 0 on failure
 */
int do_collect() {
   string name;
   string place;
   int value;

   // Check to make sure the shop is open.
   if (!check_open(this_player(), "collect")) {
      return 0;
   }

   name = this_player()->query_name();
   value = _controller->query_royalty(name);
   if (!value) {
      add_failed_mess("You have no money owed to you here.\n");
      return 0;
   }

   if (value < 0) {
      add_failed_mess("You owe money to " + the_short() + ".\n");
      return 0;
   }


   place = query_property("place");
   if (!place) {
      place = "default";
   }
   _controller->pay_out_royalty(name, place);
   log_file(__FILE__ + ".log",
            name + " collects " +
                      MONEY_HAND->money_value_string(value, place) +
                      " in royalties.\n");
   add_succeeded_mess(({ "You collect " +
                      MONEY_HAND->money_value_string(value, place) +
                      " in royalties.\n",
                      "$N collects some money in royalties.\n" }));
   return 1;
} /* do_collect() */

/**
 * This method shows any royalties that you might be able to collect.
 * @return 1 on succes, 0 on failure
 */
int do_royalties() {
   string name;
   string place;
   int value;

   // Check to make sure the shop is open.
   if (!check_open(this_player(), "royalties")) {
      return 0;
   }

   name = this_player()->query_name();
   value = _controller->query_royalty(name);
   if (!value) {
      add_failed_mess("You have no money owed to you here.\n");
      return 0;
   }

   if (value < 0) {
      add_failed_mess("You owe money to " + the_short() + ".\n");
      return 0;
   }

   place = query_property("place");
   if (!place) {
      place = "default";
   }
   add_succeeded_mess(({ "You have " +
                      MONEY_HAND->money_value_string(value, place) +
                      " in royalties currently.\n",
                      "$N checks $p royalties.\n" }));
   return 1;
} /* do_royalites() */

/**
 * This method sets the cut for the object.
 * @param new_cut the new cut of the object
 * @see query_cut()
 */
void set_cut(int new_cut) {
   _cut = new_cut;
} /* set_cut() */

/**
 * This method queries the current cut defined for this shop.
 * @return the current cut for the shop
 * @see set_cut()
 */
int query_cut() {
   return _cut;
} /* query_cut() */

/**
 * This method sets the minimum allowed cost for the item in the
 * shop inventory.
 * @param cost the minimum cost
 * @see query_minimum_cost()
 */
void set_minimum_cost(int cost) {
   _minimum_cost = cost;
} /* set_minimum_cost() */

/**
 * This method returns the minimum allowed cost for an item in the
 * shop inventory.
 * @see set_minimum_cost()
 */
int query_minimum_cost() {
   return _minimum_cost;
} /* query_minimum_cost() */

/**
 * This method sets the function to be called on all the objects when
 * they are sold.  For instance this can be used to close all books
 * etc.
 * @param func the function to use
 * @example
 * // Close the book when it is sold.
 * set_when_sold_function( (: $1->set_open_page(0) :) );
 */
void set_when_sold_function(function func) {
   _when_sold_func = func;
} /* set_when_sold_function() */

/**
 * This method returns the when sold function.
 * @return the when sold function
 * @see set_when_sold_function()
 */
function query_when_sold_function() {
   return _when_sold_func;
} /* query_when_sold_function() */

/**
 * This method checks to see if the shop is open or not.  It will setup the
 * default fail message if this is what is needed.
 * @param player
 * @param command the command to check for
 * @see set_open_function()
 */
int check_open(object player, string type) {
   if (!is_open(player, 0)) {
      add_failed_mess("The shop is not open.\n");
      return 0;
   }
   return 1;
} /* check_open() */

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
   _extra_sell_check = func;
} /* set_extra_sell_check() */

/**
 * This method returns the extra sell check function.
 * @return the extra sell check function
 * @see set_extra_sell_check()
 */
function query_extra_sell_check() {
   return _extra_sell_check;
} /* query_extra_sell_check() */

/**
 * This method sets the flag that removes the royalty commands from the
 * shop, if the royalty stuff is be handled somewhere else.
 * @param flag the flag to set
 */
void set_no_royalty_commands(int flag) {
   _no_royalty_commands = flag;
} /* set_no_royalty_commands() */

/**
 * This method returns the flagt the removes the royalty commands from the
 * shop.
 * @return the royalty command flag
 */
int query_no_royalty_commands() {
   return _no_royalty_commands;
} /* query_no_royalty_commands() */

void dest_me() {
  if(_controller) {
     _controller->dest_me();
  }
} /* dest_me() */
