/*
 * Rabbit
 *
 * by pinkfish
 */

#include <language.h>

inherit LANGUAGE_INHERIT;

void setup() {
   set_long( "This is the local language of Rabbit.\n" );
   set_name( "Rabbitian" );
   set_start_text_bit( "Some carefully constructed characters "
                       "that might mean:\n" );
} /* setup() */

mixed do_garble_text( string text ) {
    return ({ "whats", "up", "doc", "carrot" })[random(4)];
} /* do_garble_text() */


string do_garble( string text, string text_type )  {
    return ({ "whats", "up", "doc", "carrot" })[random(4)];
} /* do_garble() */
