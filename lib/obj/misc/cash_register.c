#include <money.h>

inherit "/std/container";

object shop;

object query_shop() { return shop; }

void set_shop( object thing ) { shop = thing; }

string long( string word, int dark ) {
   int i;
   string ret, *coins;
   object cash;
   ret = query_long();
   cash = present( MONEY_ALIAS, this_object() );
   if ( !cash )
      return ret +"There is no money in the register.\n";
   coins = (string *)cash->half_short( 1 );
   if ( !sizeof( coins ) )
      return ret +"There is no money in the register.\n";
   ret += "The register contains:\n";
   for ( i = 0; i < sizeof( coins ); i++ )
      ret += "      "+ coins[ i ] +"\n";
   return ret;
} /* long() */

int test_add( object thing, int flag ) {
   if ( !thing->id( MONEY_ALIAS ) )
      return 0;
   if ( shop )
      shop->save_register();
   return 1;
} /* test_add() */

int test_remove( object thing, int flag ) {
   if ( shop )
      shop->save_register();
   return 1;
} /* test_remove() */
