/*
 *$Id$
 */

/* Remains of a dead golem */

#include <armoury.h>

inherit "/obj/container";

void make_fragments();


void setup() {
    set_name( "rubble" );
    set_short( "heap of clay rubble" );
    add_alias( ({"heap", "clay"}) );
    add_adjective( ({"heap of", "clay"}) );
    set_main_plural( "heaps of clay rubble" );
    add_plural( "heaps" );

    set_weight( 40000 );
    set_max_weight( 50000 );
    set_value( 0 );
    set_stuck( 1 );
    make_fragments();
    add_property( "no recycling", 1 );
} /* void setup() */


void make_fragments() {
    int amount = random( 8 );
    int i;

    for( i = 1; i <= amount; i++ )
        switch( random( 11 ) ) {
           case 0..5:
               ARMOURY->request_item( "golem shard", 100 )->
                   move( this_object() );
               break;
           case 6..7:
               ARMOURY->request_item( "golem finger", 100 )->
                   move( this_object() );
               break;
           case 8..9:
               ARMOURY->request_item( "golem toe", 100 )->
                   move( this_object() );
               break;
           case 10:
               ARMOURY->request_item( "golem eye", 100 )->
                   move( this_object() );
               break;
        }
} /* void make_fragments() */


string long( string word, int dark ) {
    string inv = query_multiple_short( all_inventory( this_object() ) );
    string tmp = "";
    switch( sizeof( all_inventory( this_object() ) ) ) {
        case 0:
           break;
        case 1:
            tmp = "Something that looks like " + inv + " protrudes from it.  ";
            break;
        default:
            tmp = "It looks like " + inv + " are buried in it.  ";
    }
    return "This is a large heap of shattered clay.  " + tmp + "Whatever this "
        "was before, it is destroyed thouroughly and beyond any means of "
        "repair.\n";
} /* string long() */


