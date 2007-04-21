/**
 * A binding shop, where people can come and get books bound.
 * @author Pinkfish
 * @started Fri Apr  7 13:19:40 PDT 2000
 */
inherit "/std/room/basic_room";

#include <money.h>
#include <language.h>

private int _cost_per_book;
private int _cost_per_page;

void create() {
   _cost_per_book = 4000;
   _cost_per_page = 400;
} /* create() */

void init() {
   add_command("bind", "<object:indirect:me'pages'> into book");
} /* init() */

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
 * This method binds up a book from a bunch of pages.
 * @param obs the pages to bind up
 * @return 1 on success, 0 on failure
 */
int do_bind(object *obs) {
   object *magic;
   object *paper;
   object *fail;
   object ob;
   object book;
   string place;
   int cost;

   foreach (ob in obs) {
      if (ob->query_paper()) {
         if (check_for_magic_writing(ob)) {
            magic += ({ ob });
         } else {
            paper += ({ ob });
         }
      }
   }
   if (sizeof(fail) || sizeof(magic)) {
      if (sizeof(magic)) {
         add_failed_mess("Cannot bind up $I with magic stuff into "
                         "a book.\n", magic);
      }
      if (sizeof(fail)) {
         add_failed_mess("$I are not pieces of paper and cannot be "
                         "bound into a book.\n", fail);
      }
      return 0;
   }

   place = query_property("place");
   if (!place) {
      place = "default";
   }

   cost = _cost_per_book + _cost_per_page * sizeof(paper);

   //
   // Check to see if they have enough money.
   //
   if (this_player()->query_value(place) > cost) {
      add_failed_mess("You do not have enough money, you need " +
                      MONEY_HAND->money_value_string(cost, place) +
                      " to bind $I into a book.\n", paper);
      return 0;

   }

   //
   // Ok, do the binding.
   //
   book = clone_object("/std/book");
   foreach (ob in paper) {
      book->add_page_after(ob, book->query_num_pages() + 1);
   }

   book->move(this_player());

   this_player()->pay_money( MONEY_HAND->create_money_array(cost, place),
                             place);
   return 1;
} /* do_bind() */
