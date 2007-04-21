inherit "/std/book_dir";

#define DEAD_PAGES "/save/books/dead/dead_page"

void setup() {
  int i, max;

  seteuid(getuid());
  add_property("dead usable", 1);
  set_short("dark brown book");
  set_name("book");
  add_adjective( ({ "dark", "brown", "dead", "death", "dieing" }) );
  set_long("The book has some writing "
           "burnt into the dark brown leather cover.\n");
  add_read_mess("Introduction to death, or what to "
                "do when your blood runs out", 0, "general", 0 );
  set_book_language("general");
  set_book_dir(DEAD_PAGES);

  set_open_page(0);
} /* setup() */

void recovery_from_death() {
  call_out((: tell_object($1, the_short()+" crumbles and vanishes "
                              "mysteriously.\n") :), 1, previous_object());
  call_out("dest_me", 2);
} /* recovery_from_death() */

mixed query_static_auto_load() { return 0; }
mixed query_dynamic_auto_load() { return 0; }
mixed query_auto_load() { return 0; }

void check_location() {
  if (environment() &&
      !environment()->query_property("dead") &&
      !environment()->query_creator()) {
    say(the_short()+" vanishes suddenly and randomly.\n");
    call_out("dest_me", 1);
  }
} /* check_location() */

varargs int move(object dest, string mess1, string mess2) {
  ::move(dest, mess1, mess2);
  call_out("check_location", 1);
} /* move() */
