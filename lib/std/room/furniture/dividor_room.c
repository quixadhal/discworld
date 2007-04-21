inherit "/std/shops/commercial";

private string _main_room;

/**
 * This method sets the location of the main room.
 * @param room the location of the main room
 */
void set_main_room(string room) {
   _main_room = room;
}

/**
 * This method returns the location of the main room.
 * @return the main room of the shop
 */
string query_main_room() {
   return _main_room;
}

/**
 * @ignore yes
 */
string query_parcel_post() {
   return _main_room->query_parcel_post();
}
