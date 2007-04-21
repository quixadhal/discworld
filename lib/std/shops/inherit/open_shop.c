/**
 * This is the basic inheritable to make things check for being
 * open.
 * @author Pinkfish
 * @started Mon Oct 23 12:04:18 PDT 2000
 */

private nosave function _open_func;

/**
 * This method sets the open function for the room.
 * @param func the open function
 * @see query_open_function()
 * @see check_open()
 * @example
 * // The pub is only open if the barman is alive.
 * set_open_function( (: ( barman ? 1 : 0 ) :) );
 * @example
 * void setup() {
 *    ...
 *    // Setup the open function.
 *    set_open_function( (: do_open_check :) );
 * } /\* setup() *\/
 *
 * int do_open_check( object player, int type ) {
 *    if( !barman ) {
 *       add_failed_mess( "There appears to be no barman here!\n" );
 *       return 2;
 *    }
 *    if( type <= PUB_DESSERT && !chef ) {
 *       add_failed_mess( "The chef isn't here at the moment.\n" );
 *       return 2;
 *    }
 *    return 0;
 * } /\* do_open_check() *\/
 */
void set_open_function(function func) {
   _open_func = func;
} /* set_open_function() */

/**
 * This method queries the open function for the room/.
 * @return the open function
 */
function query_open_function() {
   return _open_func;
} /* query_open_function() */

/**
 * This method checks to see if the shop is open.
 * @param player the player who we are checking to see if it is open for
 */
int is_open(object player, int type) {
   if (_open_func) {
      return evaluate(_open_func, player, type);
   }
   return 1;
} /* is_open() */


/**
 * @ignore yes
 * This method provides backwards compatibility.
 */
int test_open() {
    return is_open( this_player(), 0 );
} /* test_open() */
