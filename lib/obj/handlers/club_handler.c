/**
 * This is the handler for all things clubby, a club being a group of
 * players.  Each club must have a unique name.  It also handles elections
 * for various club positions.
 * @author Pinkfish
 * @started Sun Sep 27 03:35:42 EDT 1998
 */

inherit "/obj/handlers/inherit/club_inherit";

void create() {
   ::create();

   // Check the clubs immediately.
	 // No, lets not, lets leave it for 30 minutes so things have stabilised
	 // a bit and our load is under control.
   //setup_call_check_clubs(1800, 1);
   setup_call_check_clubs(0, 1);
} /* create() */

/** @ignore yes */
void dest_me() {
   save_cache();
   destruct(this_object());
} /* dest_me() */
