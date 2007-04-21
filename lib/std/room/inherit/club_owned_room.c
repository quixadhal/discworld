/**
 * This is the same as an owned room, but it is controlled by a club
 * instead.
 * @author Pinkfish
 * @started 16th of May
 */
inherit "/std/room/inherit/owned_room";

private nosave string _club_controller;
private nosave string _club_owned_id;

/** @ignore yes */
string query_owner() {
   return _club_controller->query_owner_of(_club_owned_id);
} /* query_owner() */

/**
 * This method sets the club controller of the room.
 * @param club_controller the club controller
 */
void set_club_controller(string club_controller) {
   _club_controller = club_controller;
} /* set_club_controller() */

/**
 * This method returns the club controller for the club.
 * @return the club controller
 */
string query_club_controller() {
   return _club_controller;
} /* query_club_controller() */

/**
 * This method sets the owned id to use for this room.  The id is then
 * used to query the club controller to see who owns the room.
 * @param id the id to set the room to use
 * @see query_owned_id()
 */
void set_owned_id(string id) {
   _club_owned_id = id;
} /* set_owned_id() */

/**
 * This method sets the owned id to use for this room.  The id is then
 * used to query the club controller to see who owns the room.
 * @param id the id to set the room to use
 * @see query_owned_id()
 */
string query_owned_id() {
   return _club_owned_id;
} /* set_owned_id() */
