/**
 * This method handles problems where players log out in places which are
 * virtually controlled.  It does the effort to put them back in, but does
 * it in a way that is recursive.  So if they are on a horse in a
 * terrain it will work properly.
 * @author Pinkfish
 * @started Wed Sep 22 18:24:40 PDT 1999
 * @example
 * inherit "/std/basic/virtual_quit_control";
 *
 * void player_quitting(object player, object place) {
 *    string pos;
 *
 *    pos = check_start_pos(environment(place));
 *    // Doesn't need to last long since we save immediately.
 *    player->add_property(START_PLACE, pos, 10);
 *    ::player_quitting(player, environment(place));
 * } /\* player_quitting() *\/
 *
 * // Move the player to the right spot when they login.
 * void event_enter(object player) {
 *    player->move(player->query_property(START_PLACE));
 * } /\* move_player_to_real_location() *\/
 */

/**
 * This method is called on the quit handler when a player quits.
 * @param player the player
 * @param place the place they quit from
 * @return the last position (if that needs to be saved).
 */
protected string find_start_pos(object player, object place) {
   mixed quit_env;
   string last_pos, name;
   mixed test_char; 

   if ( place ) {
      // query_quit_handler() can return a string, or a two-element
      // array ({ last_pos, saved_co_ords })

      //15-06-02 - Terano changed it to pass the player quitting as well.
      quit_env = place->query_quit_handler( player );
      if (arrayp(quit_env)) {
         last_pos = quit_env[0];
      } else {
         last_pos = (string)quit_env;
      }
      if ( last_pos ) {
         last_pos->player_quitting( player, place );
      } else {
         last_pos = file_name( place );
      }
   } else {
      last_pos = player->query_start_pos();
   }

   // Check and make sure they are not somewhere they shouldn't be.
   if (last_pos && last_pos[ 0 .. 2 ] == "/w/" && last_pos != "/w/common.c") {

      // cres and their test chars may start in their own rooms

      test_char = player->query_property( "test character" );

      if ( !stringp( test_char ) ) {
          name = player->query_name();
      }
      else {
          name = test_char;
      }

      if (!sscanf(last_pos, "/w/"+ name +"%*s") ) {
         if (!player->query_creator() ||
              !last_pos->query_valid_save( player->query_name() )) {
            last_pos = player->query_start_pos();
         }
      }
   }

   return last_pos;
} /* find_start_pos() */

/**
 * This method is called on the quit handler when a player quits.
 * @param player the player
 * @param place the place they quit from
 * @return the last position (if that needs to be saved).
 */
protected int * find_start_coord(object player, object place) {
   mixed quit_env;
   string last_pos, name;
   mixed test_char;

   if ( place ) {
      // query_quit_handler() can return a string, or a two-element
      // array ({ last_pos, saved_co_ords })
      quit_env = place->query_quit_handler();
      if (arrayp(quit_env)) {
         return quit_env[1];
      } else {
         last_pos = (string)quit_env;
      }
      if ( !last_pos ) {
         last_pos = file_name( place );
      }
   } else {
      last_pos = player->query_start_pos();
   }

   // Check and make sure they are not somewhere they shouldn't be.
   if (last_pos[ 0 .. 2 ] == "/w/" && last_pos != "/w/common.c") {

      // cres and their test chars may start in their own rooms
      test_char = player->query_property( "test character" );

      if ( !stringp( test_char ) ) {
          name = player->query_name();
      }
      else {
          name = test_char;
      }

      if (!sscanf(last_pos, "/w/"+ name +"%*s") ) {
         if( !( player->query_creator() ||
                player->query_property( "test character" ) ) ||
              !last_pos->query_valid_save( player->query_name() )) {
            last_pos = player->query_start_pos();
         }
      }
   }

   return last_pos->query_co_ord();
} /* find_start_coord() */
