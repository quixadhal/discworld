/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: ptest_ers.c,v 1.1 1998/01/06 05:25:41 ceres Exp $
 * $Log: ptest_ers.c,v $
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
#include <player.h>

#define LOGIN "/secure/login"

inherit "/cmds/base";

int main_menu( string cmd, string name, string reason ) {
   string str;

   switch ( cmd[ 0 ] ) {
      case 'L' :
      case 'l' :
/*         this_player()->set_finish_func( "main_menu", this_object() ); */
         str = PLAYTESTER_HAND->query_show_list();
         str += "\n\nSee: 'ptesters H' for help.\n";
         this_player()->more_string(str, "Playtester");
         return 1;
      case 'A' :
      case 'a' :
         if ( !name ) {
            return 0;
         }
         if ( !LOGIN->test_user( name ) ) {
            printf( capitalize( name ) +" does not exist.\n" );
            break;
         }
         if (!PLAYTESTER_HAND->valid_playtester( name ) ) {
            printf( capitalize( name ) + " cannot be made a play tester "
                    "because: " + PLAYTESTER_HAND->reason_invalid_playtester(name) +
                    ".\n");
            break;
         }
         if ( (int)PLAYTESTER_HAND->add_playtester( name ) ) {
            printf( capitalize( name ) +" is now a playtester.\n" );
         } else {
            printf( capitalize( name ) +" is already a playtester.\n" );
         }
         break;
      case 'S' :
      case 's' :
         if ( !name ) {
            return 0;
         }
         if ( !PLAYTESTER_HAND->query_playtester( name ) ) {
            printf( capitalize( name ) +" is not yet a playtester.\n" );
         } else {
            if ( (int)PLAYTESTER_HAND->add_senior_playtester( name ) ) {
               printf( capitalize( name ) +" is now a senior playtester.\n" );
            } else {
               printf( capitalize( name ) +" is already a senior playtester.\n" );
            }
         }
         break;
      case 'R' :
      case 'r' :
         if ( !name ) {
            return 0;
         }
         if ( !reason) {
            notify_fail("You need to supply a reason when deleting someone.\n");
            return 0;
         }
         if ( !LOGIN->test_user( name ) ) {
            printf( capitalize( name ) +" does not exist.\n" );
            break;
         }
         if ( (int)PLAYTESTER_HAND->remove_playtester( name, reason ) ) {
            printf( capitalize( name ) +" is no longer a playtester.\n" );
         } else {
            printf( capitalize( name ) +" is not a playtester.\n" );
         }
         break;
      default :
      case 'H' :
      case 'h' :
      case '?' :
         printf( "\nPlaytesters editor commands:\n\n"
               "Q        : quit the playtester editor\n"
               "L        : list playtesters\n"
               "A <name> : add a new playtester\n"
               "S <name> : promote someone to senior playtester\n"
               "R <name> : remove an existing playtester\n"
               "H        : get this list of commands\n\n" );
         break;
   }
   /* printf( "Choose one of Q, L, A, S, R or H (for help) : " );
    input_to( "main_menu" ); */
   return 1;
} /* main_menu() */

mixed *query_patterns() {
   return ({ "", (: main_menu("L", 0, 0) :),
             "{l|h|?}", (: main_menu($4[0], 0, 0) :),
             "{a|s|h|r|?} <word>", (: main_menu($4[0], $4[1], 0) :), 
             "r <word> <string>", (: main_menu("r", $4[0], $4[1]) :) });
} /* query_patterns() */
