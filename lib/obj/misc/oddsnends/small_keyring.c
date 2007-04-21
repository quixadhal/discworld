/*
Davyd
Item
For General Item Stores
*/

inherit "/obj/misc/keyring";

void setup(){
    set_name("keyring");
    set_short("small keyring");
    set_main_plural("small keyrings");
    add_adjective( "small" );
    set_long("This is a small keyring, suitable for holding a few "
     "small keys.\n" );
    set_weight(1);
    set_max_weight( 10 );
    set_value( 200 );
}
