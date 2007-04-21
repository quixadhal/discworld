/**
 * The main error database handler.  This should be used for all calls
 * into the error system, setting up queries and for making changes to
 * the bugs.
 * @author Pinkfish
 * @started Thu Feb  6 17:28:47 PST 2003
 */
inherit "/obj/handlers/inherit/error_handler";

void create() {
  //seteuid(getuid());
  seteuid("Root");
   ::create();
}

void setup_error_handler(string user, string db, string password) {
   ::setup_error_handler(user, db, password);
}
