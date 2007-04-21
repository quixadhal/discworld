/**
 * This is a specialisation of the craft shop inheritable.  It adds in the
 * ability to read specified pages from books.
 * @author Pinkfish
 * @started Thu Sep 10 23:51:01 PDT 1998
 */
#include <shops/craft_shop.h>
inherit CRAFT_SHOP_INHERIT;

#define READ_PAGE_PROPERTY "browse page"

int browse_pages(object *obs);
protected int choose_which_page(function end_func, int value,
                                string name, object *sellables);
private void finish_choose(string page_num, function end_func, int value,
                          string name, object *sellables);

/** @ignore yes */
void init() {
   string storeroom;

   ::init();
   storeroom = file_name(query_controller()->query_sell_list());
   add_command("browse", "page of <indirect:object:" + storeroom + ">",
               (: browse_pages($1) :) );
   set_extra_sell_check((: choose_which_page :));
} /* init() */

/**
 * This is like a normal browse, but it allows us to look at specific pages
 * in the book.
 * @param number the page number to browse
 * @param obs the objects whose pages to browse
 * @return 0 on failure, 1 on success
 */
int browse_pages(object *obs) {
   object *real_obs;
   object ob;
   int number;
   string mess;
   string read;
   object *no_pages;
   object *ok;
   int i;

   // Check to make sure the shop is open.
   if (!check_open(this_player(), "browse")) {
      return 0;
   }

   real_obs = query_controller()->create_all_real_objects(this_player(), obs);
   mess = "$P$Browse page$P$";
   no_pages = ({ });
   ok = ({ });
   for (i = 0; i < sizeof(obs); i++) {
      ob = real_obs[i];
      number = ob->query_property(READ_PAGE_PROPERTY);
      if (undefinedp(number)) {
         number = random(ob->query_num_pages() - 1) + 1;
         ob->add_property(READ_PAGE_PROPERTY, number);
         query_controller()->update_craft_object(obs[i], real_obs[i]);
      }
      if (number > ob->query_num_pages()) {
         no_pages += ({ ob });
      } else {
         ob->set_open_page(number);
         mess += ob->the_short() + ":\n" + ob->long();
         read = ob->query_readable_message();
         if (read) {
            mess += "You read " +
                    replace(ob->query_read_short(this_player()), "$name$",
                            ob->a_short()) + ":\n" +
                    ob->query_readable_message();
         }
         ok += ({ ob });
      }
   }
   if (sizeof(ok)) {
      add_succeeded_mess(({ mess, "$N browses a page of $I.\n" }), ok);
      return 1;
   } else {
      add_failed_mess("There are not enough pages in $I to open to " +
                      " page " + number + ".\n", no_pages);
      return 0;
   }
} /* browse_pages() */

/**
 * This method gets the player to choose a page to allow to browse
 * @param end_func the function to evaluate when finished
 * @param value the value of the sellables
 * @param name the name to sell the object for
 * @param sellables the objects to sell
 */
protected int choose_which_page(function end_func, int value,
                                string name, object *sellables) {
   write("Which page of " + query_multiple_short(sellables) + 
         " would you like to let people browse (0 for the cover only)? ");
   input_to((: finish_choose :), 0, end_func, value, name, sellables);   
} /* choose_which_page() */

/**
 * This finishes the choose page thingy.
 */
private void finish_choose(string page_num, function end_func, int value,
                          string name, object *sellables) {
   int page;
   object *invalid;

   if (page_num == "quit" ||
       page_num == "q") {
      write("Aborting sale.\n");
      return ;
   }
   if (sscanf(page_num, "%d", page) != 1) {
      write("Need to actually enter a number.\n");
      write("Which page of " + query_multiple_short(sellables) + 
           " would you like to let people browse? ");
      input_to((: finish_choose :), 0, end_func, value, name, sellables);
      return ;
   }
   if (page < 0) {
      write("Need to specify a page number greator than zero.\n");
      write("Which page of " + query_multiple_short(sellables) + 
           " would you like to let people browse? ");
      input_to((: finish_choose :), 0, end_func, value, name, sellables);
      return ;      
   }
   invalid = filter(sellables, (: $1->query_num_pages() < $(page) :) );
   if (sizeof(invalid)) {
      write("Page Number " + page + " is larger the number of pages in " +
            query_multiple_short(invalid) + ".\n");
      write("Which page of " + query_multiple_short(sellables) + 
           " would you like to let people browse? ");
      input_to((: finish_choose :), 0, end_func, value, name, sellables);
      return ;      
   }
   sellables->add_property(READ_PAGE_PROPERTY, page);
   sellables->set_open_page(0);
   evaluate(end_func, value, name, sellables);
} /* finish_choose() */
