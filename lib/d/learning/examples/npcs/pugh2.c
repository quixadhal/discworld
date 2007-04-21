
/* Essentially, exactly the same data as in room1, but kept
*in a different file, for ease of mulitple use.
*/


inherit "/obj/monster";


void setup() {

    set_name( "pugh" );
    set_short( "Pugh" );
    add_property("determinate", "");
    add_alias( ({"npc", "warrior", "man"}) );
    set_long("This is Pugh.  He exists in a separate file, so can easily be "
    "loaded in a variety of locations at once.  Essentially, he is identical"
    " to Pugh in the other room.\n"); 
    basic_setup ("human", "warrior", 10);
    set_gender( 1 );

} /* ends setup() */
