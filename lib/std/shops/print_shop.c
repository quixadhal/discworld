/**
 * A printing workshop so that players can create their own books runs.
 * <P>
 * This can be divided up into two rooms, an office (where you pay for
 * the printing to be done) and a collection room (where you do the collection).
 * To enable this behaviour the collection room needs to know about the
 * office.  The office stores all the information, so you call
 * set_office_room(room_path); on the collection room and
 * set_no_collection() on the office.
 *
 * @author Pinkfish
 * @started Thu Jan 29 17:55:47 CST 1998
 * @see set_office_room()
 * @see set_no_collection();
 */

#include <language.h>
#include <money.h>
#include <book_handler.h>

inherit "/std/room/basic_room";


#define COPYRIGHT_NUMBER 3

nosave mixed *discount_schedule;
mapping collectables;
int current_save_number;
nosave int letter_cost;
nosave int time_to_collect;
nosave int time_per_copy;
nosave string save_dir;
nosave string office_room;
nosave int no_collection;
nosave function open_func;
nosave string default_language;
nosave mapping translation_costs;
nosave string print_shop_name;

#define SCHEDULE_NUM 1
#define SCHEDULE_PERCENTAGE 0

#define COLLECT_TIME 0
#define COLLECT_NUM  1
#define COLLECT_OB_NAME 2
#define COLLECT_LANGUAGE 3
#define COLLECT_AUTO_LOAD_NUM 4
#define COLLECT_ARR_SIZE 5

#define BINDING_COST 600
// This is the cost for every 10 letters.
#define DEFAULT_LETTER_COST 1
#define DEFAULT_TIME_TO_COLLECT (8*60*60)
#define DEFAULT_TIME_PER_COPY (20*60)

/* 100 copies.  Wheee... */
#define MAX_NUMBER 30

int price_object(object ob, int num, string language);
protected int do_print_copies(object *obs, int num, string language);
protected int do_print(object *obs, string language);
protected int do_claim(object *obs);
protected int do_collect();
void do_print_run(object ob, int number, int cost, string language);
void print_pricing(object ob, string language);
void remove_print_run(string name, mixed *info);
mixed *query_print_run(string name);
void add_print_run(string name, object ob, int number, string language);
void load_it();
void save_it();
string query_print_shop_name();
string query_static_auto_load_info(int num);
mapping query_dynamic_auto_load_info(int num);
protected int add_auto_load_info(string static, string dynamic);
private object create_fly_leaf(object book, string language);
private void remove_auto_load_info(int num);

void create() {
   letter_cost = DEFAULT_LETTER_COST;
   time_to_collect = DEFAULT_TIME_TO_COLLECT;
   time_per_copy = DEFAULT_TIME_PER_COPY;
   collectables = ([ ]);
   discount_schedule = ({ ({ 100, 5 }),
                          ({ 95, 10 }),
                          ({ 90, 20 }),
                          ({ 80, 101 }) });
   default_language = "common";
   translation_costs = ([ ]);
   print_shop_name = "Rabbits happy little printing press";
   ::create();
   load_it();
   add_help_file("print_shop");
} /* setup() */

/** @ignore yes */
void init() {
   string languages;

   if (!office_room) {
      add_command("print", "<indirect:object>",
                   (: do_print($1, default_language) :));
      add_command("print", "<number> [copies] of <indirect:object>",
                      (: do_print_copies($1, $4[0], default_language) :) );
      add_command("claim", "copyright on <indirect:object>", (: do_claim :));
      if (sizeof(translation_costs)) {
         /* If there are some languages we can translate to. */
         languages = implode(keys(translation_costs) + ({ default_language }), "|");
         add_command("print", "<indirect:object> in {" + languages + "}",
                   (: do_print($1, $4[1]) :));
         add_command("print", "<number> [copies] of <indirect:object> in "
                              "{" + languages + "}",
                         (: do_print_copies($1, $4[0], $4[2]) :) );
      }
   }
   if (!no_collection) {
      add_command("collect", "", (: do_collect :));
   }
} /* init() */

/**
 * This method checks to make sure that the player is the correct holder
 * of the copyright and that the book is still copyright.
 * @param book the book to check
 * @return 1 if it is copyright
 */
protected int is_copyright(object book) {
  if (BOOK_HANDLER->check_book_same(book, book->query_book_num())) {
     return 1;
  }
  return 0;
} /* is_copyright() */

/**
 * This method returns the current copyright holder of the object.
 * @param book the book to get the copyright of
 * @return the name of the copyright holder, 0 if none
 */
protected string copyright_holder(object book) {
   return BOOK_HANDLER->query_book_owner(book->query_book_num());
} /* copyright_holder() */

/**
 * This method sets up the copyright for the objects.  This will check for
 * existing copyright status and then check with the book handler to see
 * if it still fits the criteria.  If it does not then it will add itself
 * as a new book.
 * @param obs the objects to claim copyright on
 */
protected int do_claim(object *obs) {
   object ob;
   object *already_copyright;
   object *new_copyright;
   object *cannot_copyright;
   string name;
   string place;
   int value;
   int player_money;

   already_copyright = ({ });
   new_copyright = ({ });
   cannot_copyright = ({ });
   name = this_player()->query_name();
   foreach (ob in obs) {
      if (!ob->query_book()) {
         cannot_copyright += ({ ob });
      } else if (is_copyright(ob)) {
         already_copyright += ({ ob });
      } else {
         new_copyright += ({ ob });
      }
   }

   if (!sizeof(new_copyright)) {
      if (sizeof(cannot_copyright)) {
         add_failed_mess("You cannot copyright $I.\n", cannot_copyright);
      }
      if (sizeof(already_copyright) == 1) {
         add_failed_mess("$I is copyrighted.\n",
                         already_copyright);
      } else {
         add_failed_mess("$I are copyrighted.\n",
                         already_copyright);
      }
      return 0;
   }

   place = query_property( "place" );
   if ( !place || ( place == "" ) ) {
      place = "default";
   }

   /* Otherwise we will try for the new copyright value on these guys. */
   value = 0;
   foreach (ob in new_copyright) {
      value += price_object(ob, COPYRIGHT_NUMBER, default_language);
   }

   player_money = (int)this_player()->query_value_in( place );
   if ( place != "default" ) {
      player_money += (int)this_player()->query_value_in( "default" );
   }

   write("It will cost you " + MONEY_HAND->money_value_string( value, place ) +
         " to make " + query_multiple_short(new_copyright) + 
         " copyright.\n");
   if (player_money > value) {
      write("Do you wish to continue? ");
      input_to("check_copyright", 0, new_copyright, value);
   } else {
      write("Although you do not currently have enough money.\n");
   }
   add_succeeded_mess( ({ "", "$N enquires about copyrighting $I.\n" }),
                       new_copyright);
   return 1;
} /* do_claim() */

/** @ignore yes */
protected void check_copyright(string str, object *new_copyright, int value) {
   int new_num;
   object ob;
   string place;

   if (!strlen(str) || (str[0] != 'y' && str[0] != 'Y')) {
      write("Ok, never mind.  See you again soon.\n");
      return ;
   }

   /* Make them pay for it. */
   place = query_property( "place" );
   if ( !place || ( place == "" ) ) {
      place = "default";
   }
   this_player()->pay_money( (mixed *)MONEY_HAND->create_money_array(
               value, place ), place );

   /* Add the books to the book handler. */
   foreach (ob in new_copyright) {
      new_num = BOOK_HANDLER->add_book(ob, 0, this_player()->query_name());
      ob->set_book_num(new_num);
      ob->add_property(BH_COPYRIGHT, this_player()->query_name());
   }
   write("Ok, added " + query_multiple_short(new_copyright) +
         " as a copyright book.\n");
} /* check_copyright() */

/**
 * This is when the player comes back to collect the book they have
 * printed.
 * @return 1 on success, 0 on failure
 */
protected int do_collect() {
   object new_ob;
   object page;
   object fly_leaf;
   int i;
   int j;
   int number;
   int no_printed;
   string ob_name;
   mixed static_arg;
   mixed dynamic_arg;
   string language;
   mixed *info;
   mixed *stuff;
   mixed *collectables;
   mixed room;
   int auto_load_num;

   if (open_func) {
      if (!evaluate(open_func)) {
         return 0;
      }
   }

   if (office_room) {
      room = office_room;
   } else {
      room = this_object();
   }
   collectables = room->query_print_run(this_player()->query_name());
   if (!collectables) {
      add_failed_mess("No books available for collection.\n");
      return 0;
   }

   /* Ok, now see if they have waited long enough. */
   no_printed = 0;
   foreach (info in collectables) {
      if (time() < info[COLLECT_TIME]) {
         add_failed_mess("You need to wait until " +
                         amtime(info[COLLECT_TIME]) +
                         " to collect your book.\n");
         continue;
      }
   
      /* Ok, they are here.  Whoo.  Lets do it. */
      auto_load_num = info[COLLECT_AUTO_LOAD_NUM];
      static_arg = room->query_static_auto_load_info(auto_load_num);
      dynamic_arg = room->query_dynamic_auto_load_info(auto_load_num);
      ob_name = info[COLLECT_OB_NAME];
      number = info[COLLECT_NUM];
      language = info[COLLECT_LANGUAGE];

      /* Remove the run now... */
      if (office_room) {
         office_room->remove_print_run(this_player()->query_name(), info);
      } else {
         remove_print_run(this_player()->query_name(), info);
      }

      /* First create ourselves a book. */
      new_ob = clone_object(ob_name);
      if (static_arg) {
         new_ob->init_static_arg(static_arg);
      }
      if (dynamic_arg) {
         new_ob->init_dynamic_arg(dynamic_arg);
      }

      /* Close the book first. */
      new_ob->set_open_page(0);

      /* Add in the fly leaf and fix the languages. */
      for (j = 0; j < new_ob->query_num_pages(); j++) {
         if (j > 0) {
            page = new_ob->query_selected_page(j);
         } else {
            new_ob->set_open_page(0);
            page = new_ob;
         }

         /* See if we have a fly leaf... */
         if (j == 1) {
             fly_leaf = create_fly_leaf(new_ob, language);
             if (page->query_property(BH_IGNORE_PAGE)) {
                /* Replace it with our one... */
                new_ob->replace_page_with(fly_leaf, j);
             } else {
                /* Add a new page. */
                new_ob->add_page_after(fly_leaf, j - 1);
             }
             page = new_ob->query_selected_page(j + 1);
         }

         /* Fix up the language for all the messages. */
         stuff = page->query_read_mess();
         foreach (info in stuff) {
            if (info[READ_LANG] != language) {
               /*
                * Hopefully this allows the evil array changing stuff
                * to occur...
                */
               info[READ_LANG] = language;
            }
         }
         page->set_read_mess(stuff);
      }
   
      static_arg = new_ob->query_static_auto_load();
      dynamic_arg = new_ob->query_dynamic_auto_load();
      /* Kill the sample one. */
      new_ob->dest_me();

      for (i = 0; i < number; i++) {
         new_ob = clone_object(ob_name);
         new_ob->set_player(this_player());
         if (static_arg) {
            new_ob->init_static_arg(static_arg, this_player());
         }
         if (dynamic_arg) {
            new_ob->init_dynamic_arg(dynamic_arg, this_player());
         }
   
         new_ob->move(this_object());
      }

      if (number > 1) {
         write("Ok, your " + query_num(number, 1000) + " copies of " +
               new_ob->the_short() +
               " have been placed on the floor here.\n");
      } else {
         write("Ok, your copy of " + new_ob->the_short() +
               " has been placed on the floor here.\n");
      }
      no_printed++;
   }
   return no_printed;
} /* do_collect() */

private object create_fly_leaf(object book, string language) {
    object page;

    page = book->create_default_page_object();
    if (is_copyright(book)) {
       page->add_read_mess("\n\nPrinted " + amtime(time()) +
                             " by " + query_print_shop_name() +
                             "\n\nCopyright " + 
                             this_player()->query_name(), 0,
                             language);
    } else {
       page->add_read_mess("\n\nPrinted " + amtime(time()) +
                             " by " + query_print_shop_name() +
                             "\n", 0,
                             language);
    }
    page->add_property(BH_IGNORE_PAGE, 1);
    return page;
} /* create_fly_leaf() */

/**
 * Checks to see if the object has any magic writing on it.
 * @return 1 if there is magic writing, 0 if not.
 * @see find_book_or_paper()
 * @see /obj/handlers/language->query_language_magic()
 */
int check_for_magic_writing(object ob) {
   mixed *stuff;
   int i;

   stuff = ob->query_read_mess();
   for (i = 0; i < sizeof(stuff); i++) {
      if (LANGUAGE_HAND->query_language_magic(stuff[i][READ_LANG])) {
         return 1;
      }
   }
   return 0;
} /* check_for_magic_writing() */

/**
 * Checks to see if the book contains a language we cannot translate.
 * @param ob the book to check for languages
 * @return 1 if there is a language we cannot deal with
 * @see check_magic_writing()
 * @see find_book_or_paper()
 * @see set_default_language()
 * @see add_translation_cost()
 */
int check_for_untranslatable_language(object ob) {
   mixed *stuff;
   int i;
   string lang;

   stuff = ob->query_read_mess();
   for (i = 0; i < sizeof(stuff); i++) {
      lang = stuff[i][READ_LANG];
      if (lang != default_language &&
          !translation_costs[lang]) {
         return 1;
      }
   }
   return 0;
} /* check_for_untranslatable_language() */

/**
 * This method finds a single book or piece of paper to work on.  It will
 * return 0 if multiple objects are found, or none are found.  It will
 * set the error message appropriately.
 * @param obs the objects to check for a book or paper
 * @return the book or paper found
 * @see check_magic_writing()
 */
object find_book_or_paper(object *obs) {
   object *paper;
   object *books;
   object *magic;
   object *torn_out;
   object *nothing;
   object ob;
   int page;
   int old_page;
   string owner;

   paper = ({ });
   books = ({ });
   magic = ({ });
   torn_out = ({ });
   nothing = ({ });
   foreach (ob in obs) {
      owner = copyright_holder(ob);
      if (is_copyright(ob) && owner != this_player()->query_name()) {
         add_failed_mess("$I has been copyrighted by " + owner +
                         " and cannot be copied.\n", ({ ob }));
      } else if (ob->query_book()) {
         if (ob->query_spell_book()) {
            magic += ({ ob });
         } else {
            /* Check to see if it has any magic writting... */
            old_page = ob->query_open_page();
            for (page = 0; page <= ob->query_num_pages(); page++) {
               ob->set_open_page(page);
               if (ob->is_current_page_torn_out()) {
                  torn_out += ({ ob });
                  break;
               }
               if (check_for_magic_writing(ob)) {
                  magic += ({ ob });
                  break;
               }
            }
            ob->set_open_page(old_page);
            if (page == ob->query_num_pages() + 1) {
               books += ({ ob });
            }
         }
      } else if (ob->query_paper()) {
         if (check_for_magic_writing(ob)) {
            magic += ({ ob });
         } else {
            paper += ({ ob });
         }
      } else {
         nothing += ({ ob });
      }
   }

   if (sizeof(magic)) {
      add_failed_mess("$I have magic writing in them and cannot be copied.\n",
                      magic);
   }

   if (sizeof(torn_out)) {
      add_failed_mess("$I have pages torn out and cannot be copied.\n",
                      torn_out);
   }

   if (!sizeof(books) && !sizeof(paper)) {
      if (sizeof(nothing)) {
         add_failed_mess("Unable to print $I as they are not a book or paper.\n", nothing);
      }
      return 0;
   }

   if (sizeof(books + paper) > 1) {
      add_failed_mess("Can only do print one book or piece of paper at a time.  "
                  "Please be more specific and choose from " +
                  query_multiple_short(books + paper) + ".\n");
      return 0;
   }

   if (sizeof(books) > 0) {
      return books[0];
   }
   return paper[0];
} /* find_book_or_paper() */

/**
 * This method will be called by the print add_command.  It will print out
 * the pricing schedule and how long the item will take to make.
 * @param obs the objects which have been matched
 * @param language the language to print the book in
 * @return 1 on success, 0 on failure
 * @see do_print_copies()
 */
int do_print(object *obs, string language) {
   object ob;

   if (open_func) {
      if (!evaluate(open_func, obs)) {
         return 0;
      }
   }

   /* Ok, now we have our book or bit of paper... */
   ob = find_book_or_paper(obs);
   if (!ob) {
      return 0;
   }

   print_pricing(ob, language);
   write("Use: print <number> of <book>\n");
   add_succeeded_mess(({ "",
                       "$N enquires about the cost of printing a book.\n" }));
   return 1;
} /* do_print() */

/**
 * This method will be called when the player tries to print a certain
 * number of copies of a book.
 * @param obs the objects which have been matched
 * @param number the number of prints to do
 * @param language the language to print the book in
 * @return 1 on success, 0 on failure
 * @see do_print()
 */
int do_print_copies(object *obs, int number, string language) {
   object ob;
   int cost;
   int player_money;
   string place;

   if (open_func) {
      if (!evaluate(open_func, obs)) {
         return 0;
      }
   }

   if (number < 1) {
      add_failed_mess("Must print at least one copy.\n", obs);
      return 0;
   }

   if (number > MAX_NUMBER) {
      add_failed_mess("Cannot print more than " + MAX_NUMBER +
                      " copies.\n", obs);
      return 0;
   }

   place = query_property( "place" );
   if ( !place || ( place == "" ) ) {
      place = "default";
   }
 
   ob = find_book_or_paper(obs);
   if (!ob) {
      return 0;
   }

   cost = price_object(ob, number, language);

   /* Check to see if they can afford it. */
   player_money = (int)this_player()->query_value_in( place );
   if ( place != "default" ) {
      player_money += (int)this_player()->query_value_in( "default" );
   }

   if (player_money < cost) {
      add_failed_mess("This will cost you " + 
                       (string)MONEY_HAND->money_value_string(
                      cost, place ) + " and you do not have enough "
                  "money to afford it.\n", ({ ob }));
      return 0;
   }

   if (is_copyright(ob)) {
      write("NOTE:  You are printing a copyright document.\n");
   } else {
      write("NOTE:  Please use 'claim copyright' to claim copyright "
            "on a document.\n");
   }

   add_succeeded_mess(({ "This will cost you " +
                       (string)MONEY_HAND->money_value_string(
                      cost, place ) +
                       " to do, do you wish to continue? ",
                       "$N starts to do some printing.\n" }));

   input_to("check_cost", 0, ob, number, cost, language);

   return 1;
} /* do_print_copies() */

/** @ignore yes */
void check_cost(string str, object ob, int number, int cost, string language) {
   if (strlen(str) < 1 || (str[0] != 'y' && str[0] != 'Y')) {
      write("Ok, don't worry.  See you again.\n");
      return ;
   }
   do_print_run(ob, number, cost, language);
} /* check_cost() */

/**
 * This method does the actual print run.  It creates all the objects
 * and makes the player pay for them all.
 * @param ob the object to print
 * @param number the number to print
 * @param cost the cost of the object
 * @param the language to translate things to
 * @see do_print_copies()
 */
void do_print_run(object ob, int number, int cost, string language) {
   string place;
   int player_money;
   mixed *stuff;

   /* Does the print run...  First we make the players pay! */
   place = query_property( "place" );
   if ( !place || ( place == "" ) ) {
      place = "default";
   }

   /* Check to see if they can afford it. */
   player_money = (int)this_player()->query_value_in( place );
   if ( place != "default" ) {
      player_money += (int)this_player()->query_value_in( "default" );
   }

   if (player_money < cost) {
      write("You do not seem to have enough money any more.\n");
      return ;
   }

   /* Get them to give us the money first :) */
   this_player()->pay_money( (mixed *)MONEY_HAND->create_money_array(
               cost, place ), place );

   add_print_run(this_player()->query_name(),
                 ob,
                 number,
                 language);

   stuff = query_print_run(this_player()->query_name());
   if (no_collection) {
      write("The copies will be ready at " +
            amtime(stuff[<1][COLLECT_TIME]) + " for 'collect'ion at the "
            "collection office.\n");
   } else {
      write("The copies will be ready at " +
            amtime(stuff[<1][COLLECT_TIME]) + " for 'collect'ion.\n");
   }
} /* do_print_run() */

/**
 * Figure out how much this object will cost to make.
 * @param ob the object to price
 * @param num the number of them to print
 * @return the cost of the item
 */
int price_object(object ob, int num, string to_language) {
   int i;
   int num_letters;
   int multiplier;
   int cost;
   mixed *stuff;

   if (num <= 0 || !ob) {
     return 0;
   }

   /* If it is a book, make sure we count the title... */
   ob->set_open_page(0);
   stuff = ob->query_read_mess();
   for (i = 0; i < sizeof(stuff); i++) {
      /* Figure out how many letters we have. */
      num_letters += strlen(stuff[i][READ_MESS]);
      if (stuff[i][READ_LANG] != default_language) {
         cost = translation_costs[stuff[i][READ_LANG]];
         if (!cost) {
            cost = 1000;
         }
         num_letters = (num_letters * cost) / 100;
      }
      if (to_language != default_language) {
         cost = translation_costs[to_language];
         if (!cost) {
            cost = 1000;
         }
         num_letters = (num_letters * cost) / 100;
      }
   }

   cost = ob->query_value() + (num_letters / 10) * letter_cost;

   if (ob->query_book()) {
      for (i = 1; i < ob->query_num_pages(); i++) {
         ob->set_open_page(i + 1);
         if (ob != ob->query_current_page()) {
            cost += price_object(ob->query_current_page(), 1, to_language);
         }
      }
      /* Plus cost for binding and stuff... */
      cost += BINDING_COST;
   }

   /* Ok, find the costing stuff... */
   multiplier = 200;
   for (i = 0; i < sizeof(discount_schedule); i++) {
      if (num < discount_schedule[i][SCHEDULE_NUM]) {
         multiplier = discount_schedule[i][SCHEDULE_PERCENTAGE];
      }
   }

   return (cost * num * multiplier) / 100;
} /* price_object() */

/**
 * This method will print out the pricing schedule for the object.
 * @param ob the object to get a pricing schedule of.
 */
void print_pricing(object ob, string language) {
   int i;
   int num;
   string place;
   int cost;

   place = query_property( "place" );
   if ( !place || ( place == "" ) ) {
      place = "default";
   }

   /*
    * This is the cost for one of them...  We do discounts if they
    * print a bunch.
    */
   write("The cost for the different numbers of " + ob->the_short() + " is:\n");
   write("  1 for " + (string)MONEY_HAND->money_value_string(
                    price_object(ob, 1, language), place ) +
                    " and will be ready at " +
                    amtime(time_to_collect + time_per_copy + time()) + ".\n");;
   for (i = 0; i < sizeof(discount_schedule); i++) {
      num = discount_schedule[i][SCHEDULE_NUM];
      if (num < MAX_NUMBER) {
         write(sprintf("%3d for %s and will be ready at %s.\n", num,
                       (string)MONEY_HAND->money_value_string(
                       price_object(ob, num, language), place ),
                       amtime(time_to_collect + time_per_copy * num + time()) ));
      }
   }

   if (sizeof(translation_costs)) {
      write("You can translate a book to or from " +
            query_multiple_short(keys(translation_costs) + ({ default_language })) +
            ", the cost of doing the translation is defined in the "
            "following table.");
      if (sizeof(translation_costs) > 2) {
         write("  Translating between two of these languages "
               "is also possible, but it will cost the amount to translate "
               "from the language to " + default_language + " and back again.");
      }
      write("\n");
      foreach (language, cost in translation_costs) {
         write(sprintf("   %3d%% %s", cost, language));
      }
      write("\n");
   }
} /* print_pricing() */

/**
 * This method returns the discount schedule.  There is a default discount
 * schedule setup for printing shops.  The discount schedule tells how much
 * of a discount you get for certain numbers of a run.  It returns
 * an array of the form:
 * <pre>
 * ({ ({ numn1, dis1 }), ({ num2, dis2}), ... })
 * </pre>
 * Where each number is the discount amount for printing less than that
 * number of items.  The array must be sorted from the lowest number to
 * the highest number.
 * @return the discount schedule
 * @see set_discount_schedule()
 */
mixed *query_discount_schedule() {
   return discount_schedule;
} /* query_discount_schedule() */

/**
 * Sets the discount schedule for this object.  See the query_discount_schedule
 * help for the format of the array.
 * @param value the new discount schedule.
 * @see query_discount_schedule()
 */
void set_discount_schedule(mixed *value) {
   discount_schedule = value;
} /* set_discount_schedule() */

/**
 * This queries the cost per letter.
 * @return the cost per letter
 * @see set_letter_cost()
 * @see print_cost()
 */
int query_letter_cost() {
   return letter_cost;
} /* query_letter_cost() */

/**
 * This queries the cost per letter.
 * @param value the new cost per letter
 * @see query_letter_cost()
 * @see print_cost()
 */
void set_letter_cost(int value) {
   letter_cost = value;
} /* query_letter_cost() */

/**
 * This adds an object into the current set to be collected.  This handles
 * figuring out the nice number to use for the static and dynamic save
 * numbers.
 * @param name the name of the person adding the run
 * @param ob the object being added
 * @param cost the cost of the object
 * @param number the number to be printer
 * @see do_collect()
 * @see query_print_run()
 */
void add_print_run(string name, object ob, int number, string language) {
   mixed *arr;
   int num;

   if (!collectables[name]) {
      collectables[name] = ({ });
   }
   arr = allocate(COLLECT_ARR_SIZE);
   arr[COLLECT_TIME] = time() + time_to_collect + time_per_copy * number;
   arr[COLLECT_NUM] = number;
   arr[COLLECT_LANGUAGE] = language;
   arr[COLLECT_OB_NAME] = base_name(ob);
   num = add_auto_load_info(ob->query_static_auto_load(),
                             ob->query_dynamic_auto_load());
   arr[COLLECT_AUTO_LOAD_NUM] = num;
   collectables[name] += ({ arr });
   save_it();
} /* add_print_run() */

/**
 * This method returns the array of documents awaiting collection.  If there
 * are not documents this returns 0.
 * @param name the player to query
 * @return an array containing the player information, or 0 if none
 * @see add_print_run()
 * @see /include/print_shop.h
 */
mixed *query_print_run(string name) {
   return collectables[name];
} /* query_print_run() */

/**
 * This method will remove a players print run.
 * @param name the player whose print run is to be removed.
 * @see query_print_run()
 * @see add_print_run()
 */
void remove_print_run(string name, mixed *info) {
   int index;

   if (collectables[name]) {
      index = member_array(info, collectables[name]);
      if (index != -1) {
         /* Remove the save files... */
         remove_auto_load_info(info[COLLECT_AUTO_LOAD_NUM]);
         collectables[name] = collectables[name][0.. index - 1] +
                              collectables[name][index + 1..];
         if (!sizeof(collectables[name])) {
            map_delete(collectables, name);
         }
         save_it();
      }
   }
} /* remove_print_run() */

/**
 * This method saves the autoload information out to a file and returns
 * the number associated with the file.
 * @param static_arg the static auto load information
 * @param dynamic_arg the dynamic auto load information
 * @return the number associated with the save files
 */
protected int add_auto_load_info(string static, string dynamic) {
   int num;

   num = current_save_number;
   while (unguarded( (: file_size(save_dir + "/" + $(num)) :) ) > 0) {
      num++;
   }
   current_save_number = num + 1;
   unguarded( (: write_file(save_dir + "/" + $(num),
                            save_variable( ({ $(static), $(dynamic) }) ) +
                            "\n") :) );
   return num;
} /* add_auto_load_info() */

/**
 * This method removes the files associated with the auto load information.
 * @param num the auto load number
 */
private void remove_auto_load_info(int num) {
   unguarded( (: rm(save_dir + "/" + $(num) ) :) );
} /* remove_auto_load_info() */

/**
 * This method returns static the autoload information associated with the
 * file.
 * @param num the number associated with the information
 * @return the static auto load string, 0 if none
 */
string query_static_auto_load_info(int num) {
   string *arr;
   string tmp;

   if (unguarded( (: file_size(save_dir + "/" + $(num)) :) ) > 0) {
      tmp = unguarded( (: read_file(save_dir + "/" + $(num)) :) );
      if (tmp) {
         arr = restore_variable(tmp);
         return arr[0];
      }
   }
   return 0;
} /* query_static_auto_load_info() */

/**
 * This method returns dynamic the autoload information associated with the
 * file.
 * @param num the number associated with the information
 * @return the dynamic auto load string, 0 if none
 */
mapping query_dynamic_auto_load_info(int num) {
   mixed arr;
   string tmp;
 
   if (unguarded( (: file_size(save_dir + "/" + $(num)) :) ) > 0) {
      tmp = unguarded( (: read_file(save_dir + "/" + $(num)) :) );
      if (tmp) {
         arr = restore_variable(tmp);
         return arr[1];
      }
   }
   return 0;
} /* query_static_auto_load_info() */

/**
 * Sets the save file name.
 * @param name the name of the save file
 * @see query_save_dir()
 * @see load_it()
 * @see save_it()
 */
void set_save_dir(string name) {
   save_dir = name;
} /* set_save_dir() */

/** 
 * Returns the save file name
 * @return save file name
 * @see set_save_dir()
 * @see load_it()
 * @see save_it()
 */
string query_save_dir() {
   return save_dir;
} /* query_save_dir() */

/**
 * Loads the saved information off the disc.
 * @see set_save_dir()
 * @see save_it()
 */
void load_it() {
   if (save_dir) {
      unguarded((: restore_object(save_dir + "/main") :));
      if (!collectables) {
         collectables = ([ ]);
      }
   }
} /* load_it() */

/**
 * Saves the information to the disc.
 * @see set_save_dir()
 * @see load_it()
 */
void save_it() {
   if (save_dir) {
      unguarded((: save_object(save_dir + "/main") :));
   }
} /* save_it() */

/**
 * Sets the office room to which this collection room is associated.  If
 * this is set on a room, you cannot setup print jobs in the room and this
 * room specified here is queried for the information about the jobs to
 * collect.
 * @param room the new office room
 * @see query_office_room()
 * @see set_no_collection()
 */
void set_office_room(string room) {
   office_room = room;
} /* set_office_room() */

/**
 * Returns the current office room associated with this room.
 * @return the current office room
 * @see set_office_room()
 * @see query_no_collection()
 */
string query_office_room() {
   return office_room;
} /* query_office_room() */

/**
 * This sets the no collection flag.  It makes it so this room cannot be
 * used for collections.  If this is set then there should be an associated
 * collection room used to pick up the books from.  This room should
 * use the set_office_room() function to set the office room to use
 * for the collections.
 * @param collect the new value of no_collections flag
 * @see query_no_collection()
 * @see set_office_room()
 */
void set_no_collection(int collect) {
   no_collection = collect;
} /* set_no_collection() */

/**
 * This returns the no collection flag used by the room.
 * @return the no collection flag
 * @see set_no_collection()
 * @see query_office_room()
 */
int query_no_collection() {
   return no_collection;
} /* query_no_collection() */

/**
 * This method sets the function to evaluate to check to see if the shop
 * is open or not.  This should set any error messages which are needed.
 * The objects are passed in as an argument in the do_print functions
 * and nothing is passed in for the collection check.
 * @param func the open function
 * @see query_open_func()
 * @see do_print()
 * @see do_print_copies()
 * @see do_collect()
 */
void set_open_func(function func) {
   open_func = func;
} /* set_open_func() */

/**
 * This method returns the current function which is used to check for
 * the openness of the shop.
 * @see set_open_func()
 * @return the current open function
 */
function query_open_func() {
   return open_func;
} /* query_open_func() */

/**
 * This method sets the default language to use in the print shop.
 * Anything written in this language will cost nothing extra to
 * duplicate.
 * @param lang the new default language
 * @see query_default_language()
 * @see add_translation_cost()
 * @see check_for_untranslatable_language()
 */
void set_default_language(string lang) {
   default_language = lang;
} /* set_default_language() */

/**
 * This method will return the current default language for the
 * print shop.
 * @return the current default language
 * @see set_default_language()
 * @see add_translation_cost()
 * @see check_for_untranslatable_language()
 */
string query_default_language() {
   return default_language;
} /* query_default_language() */

/**
 * This method will add the cost to translate from one language to
 * another.  The default is translating to the common language and
 * this cost will be used in both directions.  ie: the cost of translating
 * from djelian to common is the same as translating from common to
 * djelian.  If translating between two languages that are not the
 * default language, say the default is common and we want to translate
 * from djelian to wombat, then the cost from djelian->common and then
 * common->wombat will be used.
 * @param lang the language to set the translation cost of
 * @param cost the cost is in percentage, 200 means twice the price
 * @see set_default_language()
 * @see remove_translation_cost()
 * @see query_all_translation_costs()
 */
void add_translation_cost(string lang, int cost) {
   translation_costs[lang] = cost;
} /* add_translation_cost() */

/**
 * This method will remove the translation cost for a specific language.
 * @param lang the language to remove the translation cost of
 * @see set_default_language()
 * @see add_translation_cost()
 * @see query_all_translation_costs()
 */
void remove_translation_cost(string lang) {
   map_delete(translation_costs, lang);
} /* remove_translation_cost() */

/**
 * This method will return a mapping of all the translation costs.
 * The keys are the languages and the values are the costs in terms
 * of percentages.
 * @return the current translation cost mapping
 * @see add_translation_cost()
 * @see remove_translation_cost()
 */
mapping query_all_translantion_costs() {
   return translation_costs;
} /* query_all_translation_costs() */

/**
 * This method sets the name of the print shop which will be used on
 * books when they are printed.  This is only needed in the collection
 * room, this is the name that is placed on the fly leaf.
 * @param name the name of the print shop
 */
void set_print_shop_name(string name) {
   print_shop_name = name;
} /* set_print_shop_name() */

/**
 * This method will return the current name of the print shop.
 * @return the name of the print shop
 */
string query_print_shop_name() {
   return print_shop_name;
} /* query_print_shop_name() */
