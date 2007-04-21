inherit "/std/object";
#include <language.h>

//int do_read();

object our_pub;

void create() {
  ::create();
  set_name("menu");
  set_long("This is a menu.\n");
  reset_get();
}

void set_pub(object pub) { 
  our_pub = pub; 
  add_read_mess(our_pub->read(), "printed letters", our_pub->query_language());
}

object query_pub() { return our_pub; }

void init() {
  this_player()->add_command( "list", this_object(), "" );
}

string query_readable_message(object player) {
   string lang;
   string mess;

   mess = our_pub->read();
   lang = our_pub->query_language();

   mess = LANGUAGES->garble_text(lang, mess, this_object(), player);
   return mess;
} /* query_reable_message() */

int do_list() {
  write(query_readable_message(this_player()));
  add_succeeded_mess(({ "", "$N read$s $D.\n" }));
  return 1;
}

