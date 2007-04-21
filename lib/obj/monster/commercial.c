inherit "/obj/monster";
#include <money.h>

private object _sell_to;
private string* _furniture;
private string _commercial_type;

int do_list(int force);

void setup() {
   set_name("spulluf");
   set_gender(1 + random(2) );
   set_long("A nice happy person with a nice smile.\n");
   add_property("demon", 1);
   set_race( "human" );
   set_level( 1 );

   add_help_file("spulluf");

   add_respond_to_with(({ "@say", "browse" }), "#do_browse");
   add_respond_to_with(({ "@say", "buy" }), "#do_buy");
   _furniture = ({
        "/obj/furnitures/commercial/smithy",
        "/obj/furnitures/commercial/money_changer",
        "/obj/furnitures/commercial/blackjack",
        "/obj/furnitures/commercial/small_shop",
        "/obj/furnitures/commercial/pickler",
        "/obj/furnitures/commercial/blackboard",
        "/obj/furnitures/commercial/newspaper_box",
        "/obj/furnitures/commercial/newspaper_archive_box",
        "/obj/furnitures/commercial/bank_master",
        "/obj/furnitures/commercial/bank_franchise_small",
        "/obj/furnitures/commercial/postal",
        "/obj/furnitures/commercial/poll_data",
      });
} /* setup() */

string query_money_place() {
   return environment()->query_property("place");
} /* query_moneY_place() */

void set_commercial_type(string commercial_type) {
   _commercial_type = commercial_type;
} /* set_commercial_type() */

void do_start_speil(object sellto) {
   setup_nationality(sellto->query_nationality(), sellto->query_nationality_region());

   _sell_to = sellto;
   queue_command("sayto " + file_name(_sell_to) +
                 " Hello!  So you are looking at some commercial "
                 "furniture?");
   //do_list(1);
} /* do_list() */

int do_list(int force) {
   string furn;
   string place;
   int num;
   int size;
   object ob;

   if (!_sell_to) {
      call_out("leave_room", 5);
      return 0;
   }
   if (!force && this_player() != _sell_to) {
      queue_command("sayto " + file_name(this_player()) +
                 " I am dealing with " + _sell_to->the_short() +
                 " right now.");
      return 1;
   }

   queue_command("sayto " + file_name(_sell_to) +
                 " I have the following items available.");
   num = 0;
   place = query_money_place();
   foreach (furn in _furniture) {
      if (!_commercial_type ||
          furn->query_commercial_type() == _commercial_type) {
         queue_command("sayto " + file_name(_sell_to) +
                    sprintf(" %c) %s costs %s (%d square yards)",
                            num + 'A', furn->the_short(),
                            MONEY_HAND->money_value_string(furn->query_value(), place),
                            furn->query_commercial_size()));
         num++;
      }
   }

   foreach (ob in all_inventory(environment())) {
      size += ob->query_commercial_size();
   }
   queue_command("sayto " + file_name(_sell_to) +
                  " The size of your shop is " +
                  environment()->query_property_size() +
                  " square yards and you have " +
                  (environment()->query_property_size() - size) + 
                  " square yards of empty space.");
   return 1;
} /* do_list() */

string strip_spaces(string str) {
   while (strlen(str) && str[0] == ' ') {
      str = str[1..];
   }
   while (strlen(str) && str[<1] == ' ') {
      str = str[0..<2];
   }
   return str;
} /* strip_spaces() */

void leave_room() {
   move("/room/rubbish", "$N shuffles away.");
} /* leave_room() */

int do_browse(string str) {
   mapping options;
   mapping values;
   string option_name;
   string thing;
   string* bits;
   string place;
   int cost;
   int pos;
   string* furns;

   if (!_sell_to) {
      call_out("leave_room", 5);
      return 0;
   }
   place = query_money_place();
   if (this_player() != _sell_to) {
      queue_command("sayto " + file_name(this_player()) +
                 " I am dealing with " + _sell_to->the_short() +
                 " right now.");
      return 1;
   }

   str = lower_case(str);
   pos = str[0] - 'a';
   if (_commercial_type) {
      furns = filter(_furniture, (: $1->query_commercial_type() == _commercial_type :));
   } else {
      furns = _furniture;
   }
   if (strlen(str) > 1 || pos < 0 || pos >= sizeof(furns)) {
      queue_command("sayto " + file_name(_sell_to) +
                    " Sorry, I do not understand which thing "
                    "you want to browse.\n");
      return 1;
   }

   queue_command("sayto " + file_name(_sell_to) + " " +
                 furns[pos]->the_short() + " costs " +
                 MONEY_HAND->money_value_string(furns[pos]->query_value(), place) +
                 " and takes up " + furns[pos]->query_commercial_size() +
                 " square yards.");
   if (furns[pos]->query_weekly_upkeep_cost()) {
      queue_command("sayto " + file_name(_sell_to) +
                    " Costs " +
                    MONEY_HAND->money_value_string(furns[pos]->query_weekly_upkeep_cost(), place) +
                    " per week in upkeep.");
   }
 

   queue_command("sayto " + file_name(_sell_to) +
                 " " +
                 furns[pos]->long()[0..<2]);
   thing = furns[pos]->query_commercial_information();
   if (thing) {
      queue_command("sayto " + file_name(_sell_to) +
                    " " + thing);
   }
   options = furns[pos]->query_commercial_options();
   if (options) {
      queue_command("sayto " + file_name(_sell_to) +
                    " You have the following options for " +
                    furns[pos]->the_short());
      foreach (option_name, values in options) {
         bits = ({ });
         foreach (thing, cost in values) {
            bits += ({ "'" + thing + "' costs " +
                       MONEY_HAND->money_value_string(cost, place) });
         }
         queue_command("sayto " + file_name(_sell_to) +
                       " The options for '" + option_name  + "' are " +
                       query_multiple_short(bits));
     
      }
   }
   return 1;
} /* do_browse() */

int do_buy(string str, string options) {
   string place;
   string* bits;
   string name;
   string value;
   string thing;
   string option_name;
   string my_option;
   int pos;
   int size;
   int cost;
   int total_cost;
   int bad_option;
   int found;
   mapping option_types;
   object ob;
   string* furns;
   mapping values;
   mapping option_values;

   if (!_sell_to) {
      call_out("leave_room", 5);
      return 0;
   }
   place = query_money_place();
   if (this_player() != _sell_to) {
      queue_command("sayto " + file_name(this_player()) +
                 " I am dealing with " + _sell_to->the_short() +
                 " right now.");
      return 0;
   }

   str = lower_case(str);
   pos = str[0] - 'a';
   if (_commercial_type) {
      furns = filter(_furniture, (: $1->query_commercial_type() == _commercial_type :));
   } else {
      furns = _furniture;
   }
   if (strlen(str) > 1 || pos < 0 || pos >= sizeof(furns)) {
      queue_command("sayto " + file_name(_sell_to) +
                    " Sorry, I do not understand which thing "
                    "you want to buy.\n");
      return 1;
   }

   if (furns[pos]->disallow_sale_here(environment())) {
      queue_command("sayto " + file_name(_sell_to) +
                    " Sorry, this room disallows the purchase of that item.");
      return 0;
   }

   bits = explode(options, ",");
   option_types = ([ ]);
   foreach (thing in bits) {
      if (sscanf(thing, "%s=%s", name, value) == 2) {
         name = strip_spaces(name);
         value = strip_spaces(value);
         option_types[lower_case(name)] = lower_case(value);
      }
   }

   foreach (ob in all_inventory(environment())) {
      size += ob->query_commercial_size();
   }

   if (size + furns[pos]->query_commercial_size() >
       environment()->query_property_size()) {
      queue_command("sayto " + file_name(_sell_to) +
                    " The item " + furns[pos]->the_short() +
                    " is too big to fit into " + environment()->the_short());
      return 0;
   }

   //
   // Check to make sure all the options are set correctly.
   //
   option_values = furns[pos]->query_commercial_options();
   if (option_values) {
      foreach (option_name, values in option_values) {
         my_option = option_types[lower_case(option_name)];
         if (!my_option) {
            queue_command("sayto " + file_name(_sell_to) +
                    " The option '" + option_name + "' needs to be "
                    "specified.");
            bad_option = 1;
         } else {
            foreach (thing, cost in values) {
               if (lower_case(thing) == my_option) {
                  // Excelent.
                  total_cost += cost;
                  found = 1;
                  option_types[lower_case(option_name)] = thing;
               }
            }
            if (!found) {
               queue_command("sayto " + file_name(_sell_to) +
                       " The option " + option_name + " has options "
                       "of " + query_multiple_short(keys(values)) + ".");
               bad_option = 1;
            }
         }
      }
   }

   if (bad_option) {
      return 0;
   }

   total_cost += furns[pos]->query_value();
   if (this_player()->query_value_in(place) < total_cost) {
      queue_command("sayto " + file_name(_sell_to) +
              " You do not have the " +
              MONEY_HAND->money_value_string(total_cost, place) +
              " to buy " + furns[pos]->the_short() + ".");
      return 0;
   }

   this_player()->pay_money(MONEY_HAND->create_money_array(total_cost, place),
                            place);
   ob = clone_object(furns[pos]);
   foreach (name, thing in option_types) {
      ob->set_commercial_option(name, thing, environment());
   }
   ob->move(environment());
   return 1;
} /* do_buy() */

int do_dismiss() {
   if (_sell_to && this_player() != _sell_to) {
      queue_command("sayto " + file_name(this_player()) +
                 " I am dealing with " + _sell_to->the_short() +
                 " right now.");
      return 1;
   }

   queue_command("sayto " + file_name(this_player()) +
                 " Goodbye, see you next time.");
   call_out("leave_room", 5);

   return 1;
} /* do_dismiss() */

int do_trash(object* obs) {
   object* ok;
   object* bad;

   ok = filter(obs, (: $1->query_commercial_furniture() :));
   if (!sizeof(ok)) {
      queue_command("sayto " + file_name(_sell_to) +
                    " None of " + query_multiple_short(obs) + " are "
                    "commercial items, so I cannot remove them.");
      return 0;
   }

   //
   // Trash them.
   //
   bad = filter(ok, (: !$1->can_trash_furniture() :));
   if (sizeof(bad)) {
      this_player()->add_failed_mess(this_object(),
                      "The items $I are unable to be trashed.  Perhaps they "
                      "still have something in them?\n");
      return 0;
   }

   write("Are you sure you want to trash " + query_multiple_short(ok) +
         "; this will get rid of them completely? ");
   input_to("complete_trash", 0, ok);
   this_player()->add_succeeded_mess(this_object(), "", ({ }));
   return 1;
} /* do_trash() */

/** @ignore yes */
void complete_trash(string str, object* obs) {
   str = lower_case(str);
   if (!strlen(str) || str[0] != 'y') {
      write("Aborting trash.\n");
   }

   queue_command("sayto " + file_name(_sell_to) + " Trashing " +
                 query_multiple_short(obs) + " for you.\n");
   obs->move("/room/rubbish", "", the_short() + " points at $N and it rips "
             "apart then vanishes.");
} /* complete_trash() */

void init() {
   ::init();

   this_player()->add_command("trash", this_object(),
                              "<indirect:object> with <direct:object>",
                              (: do_trash :));
   this_player()->add_command("dismiss", this_object(),
                              "<direct:object>",
                              (: do_dismiss() :));
   this_player()->add_command("list", this_object(),
                              "<direct:object>",
                              (: do_list(0) :));
   this_player()->add_command("list", this_object(),
                              "{on|from} <direct:object>",
                              (: do_list(0) :));
   this_player()->add_command("browse", this_object(),
                              "<string'name'> on <direct:object>",
                              (: do_browse($4[0]) :));
   this_player()->add_command("buy", this_object(),
                              "<string'name'> {on|from} <direct:object>",
                              (: do_buy($4[0], "") :));
   this_player()->add_command("buy", this_object(),
                              "<string'name'> with <string'options'> {on|from} <direct:object>",
                              (: do_buy($4[0], $4[1]) :));
} /* init() */
