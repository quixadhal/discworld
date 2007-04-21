/*
 * A guide for new home owners.
 */
inherit "/std/book_dir";

void setup() {
  set_name("book");
  set_short("brilliant vermillion book");
  add_adjective(({"brilliant", "vermillion"}));
  add_alias("guide");
  set_long("This is a brilliant vermillion coloured book with lots of "
           "pictures of money on the front cover.\n");
  set_read_mess("                   DIY\n\n"
                "          A guide for shop owners.\n");
  set_main_plural("brilliant vermillion books");
  set_weight(10);
  set_value(10);
  
  set_book_language( "general");
  set_book_dir("/save/books/shop_owner/page");

  set_open_page(0);
  set_ignore_saved_pages(1);
}

int do_tear() {
  return 0;
}

int query_binding_force() {
  return 100;
}
