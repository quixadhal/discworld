// Una
// For use in Cwc

inherit "/std/room/furniture/basic";

void setup () {
  
    set_name( "plant" );
    add_adjective( ({ "silk", "potted", "cabbage" }) );
    set_short( "potted silk cabbage plant" );
    add_alias( "cabbage" );
    set_main_plural ( "potted silk cabbage plants" );
    add_property( "determinate", "a " );

    set_long( "Rare and magnificent!  Strange and exotic!  Potted in a "
        "subtley plain pot, this fine silk-leafed brassica has been "
        "created in Sum Dim by none other than Brown Fingers Dibbler, "
        "purveyor of the finest silken forn plants.  Its leaves even look "
        "as though they have been nibbled slightly by a caterpillar, "
        "for the authentic touch.\n" );
    set_value( 35000 );
    set_weight( 150 );
  
    set_allowed_room_verbs(([ "sitting" : "sits", 
                              "standing" : "stands",
                              "lying" : "lies" ]));

} /*setup*/

   
