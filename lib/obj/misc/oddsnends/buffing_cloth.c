/*
Davyd
Item
For General Item Stores
*/

inherit "/std/object";

void setup(){
    set_name("cloth");
    set_short("buffing cloth");
    set_main_plural("buffing cloths");
    add_adjective( "buffing" );
    set_long("This is a small, soft cloth that might be used for polishing"
	     " metal objects.\n" );
    set_value(32);
    set_weight(2);
}
