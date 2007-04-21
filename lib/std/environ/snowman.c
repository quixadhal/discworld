/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: snowman.c,v 1.5 2003/01/30 16:38:28 taffyd Exp $
 * $Log: snowman.c,v $
 * Revision 1.5  2003/01/30 16:38:28  taffyd
 * Exciting bug fixes!
 *
 * Revision 1.4  2001/06/01 20:55:39  ceres
 * Can't remember
 *
 * Revision 1.3  1999/12/31 03:22:49  pinkfish
 * Fix up a typo.
 *
 * Revision 1.2  1999/05/15 12:58:49  rywfol
 * Uncapitalized (is that a word?) the short()
 *
 * Revision 1.1  1998/01/06 04:23:53  ceres
 * Initial revision
 * 
*/
#include <move_failures.h>

inherit "/std/object";

private int _call_out_handle; 

/** @ignore yes */ 
void setup() {
    reset_get();
    if ( clonep() ) { 
        _call_out_handle = call_out("die",400);
    }
} /* setup() */ 

/** 
 * This method sets the type of the snowman.  
 * @param type the type of the snowman, ie, snowman, snowwoman, snowwomble.
 * snowfrog, etc.
 */ 
void set_type(string type) {
    set_name(type);
    set_short(type);
    set_long("A wonderful " + type + " with glowing eyes made of coal and twigs "
      "for ears.\n");
    add_extra_look( this_object() ); 
} /* set_type() */ 

/** @ignore yes */ 
string extra_look( object ob ) {
    int time_left;

    if ( !_call_out_handle ) {
        return "";
    }

    time_left = find_call_out( _call_out_handle );

    debug_printf( "time left is %d\n", time_left );

    if ( time_left > 300 || time_left < 0 ) {
        return "";
    }

    return "It is " + ({ "mostly", "half", "slightly" })[time_left/100] + 
        " melted.\n";
} /* extra_look() */ 

/** @ignore yes */ 
void die() {
    string noun;

    switch( environment( this_object() )->query_property( "location" ) ) { 
    case "inside":
        noun = "floor";
        break;
    case "outside":
        noun = "ground";
        break;
    case "underwater":
        noun = "seabed";
        break;
    default:
        noun = "floor";
        break;
    }

    tell_room( environment( this_object() ), 
      the_short() + " $V$0=melts,melt$V$ into the " + noun + ".\n");

    if ( this_object()->move( "/room/rubbish" ) != MOVE_OK ) { 
        dest_me(); 
    }
} /* die() */
