/*  -*- LPC -*-  */
/*      lantern.c
        
        This file was constructed by Godot (Marshall Buhl).  If you
        make any changes to it, please leave comments in the code
        detailing the changes and let him know about them.  This file
        is not the original, so it is important that he be informed.
        If not, he may unknowingly throw away your changes by
        uploading an new version.
 
        This is a modification of the standard torch.
 
    Change log:
 
        11/26/92 - Created by Godot.
        11/28/92 - Modified by Godot to add prototype for init_arg().
        2 May 93 - Modified by Pinkfish to use failed_mess and succeeded_mess.
        13 Apr 95 - Fixed by Ceres for bug reports
        15 Okt 96 - Mostly rewritten by Turrican to make it a bit nicer.
         5 Jun 97 - Rewritten for the fuel_handler
        August 2001 - Siel: Placed the code in an inherit named
                      /obj/handlers/non_refuelling_lantern.c and kept the 
                      descriptions here only.
*/

inherit "/obj/lantern";

void setup() {
   fuel_left = 3600;
   is_lit    = 0;
   
   set_name( "lantern" );
   set_main_plural( "lanterns");
   set_short( "lantern" );
   add_help_file("torch");
   
   set_long(function() {
     string str;
     
     str = "This is a camping lantern, which smells of lamp oil.\n";
     if ( is_lit ) {
       return str + "The wick flickers inside spreading a dull "
         "glow.\n";
     }
     str += "The lantern is not lit, and ";
     if ( !fuel_left )
       return str + "is out of oil.\n";
     return str + "has some oil left in it.\n";
   } );
   set_weight( 60 );
   set_value( 150 );
}

