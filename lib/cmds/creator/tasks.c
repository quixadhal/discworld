/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: tasks.c,v 1.1 1998/01/06 05:25:41 ceres Exp $
 * $Log: tasks.c,v $
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
#include <skills.h>

#define SAVE_FILES "/save/tasks/"

inherit "/cmds/base";

mapping awards;

void create() { seteuid( "Root" ); }

void main_menu( string word ) {
   int i, j, *numbers;
   string text, *words;
   if ( !word || ( word == "" ) )
      word = " ";
   switch ( word[ 0 ] ) {
      case 'Q' :
      case 'q' :
         return;
      case 'L' :
      case 'l' :
         words = get_dir( SAVE_FILES +"*.o" );
         if ( !sizeof( words ) ) {
            printf( "No tasks have been logged.\n" );
            return;
         }
         for ( i = sizeof( words ) - 1; i > -1; i-- ) {
            words[ i ] = words[ i ][ 0 .. strlen( words[ i ] ) - 3 ];
            if ( sizeof( (string *)SKILL_OB->
                  query_immediate_children( words[ i ] ) ) )
               words[ i ] += sprintf( "%*s", 60 - strlen( words[ i ] ),
                     "[non-primary]" );
         }
         this_player()->set_finish_func( "main_menu", this_object() );
         this_player()->more_string( "Skills with logged tasks are:\n   "+
               implode( words, "\n   " ) +"\n", "Tasks", 1 );
         return;
      case 'C' :
      case 'c' :
         if ( strlen( word ) < 4 ) {
            printf( "Syntax: C <skill>\n" );
            break;
         }
         word = word[ 2 .. ];
         if ( file_size( SAVE_FILES + word +".o" ) < 1 ) {
            printf( "There are no tasks logged for "+ word +".\n" );
            break;
         }
         unguarded( (: restore_object, SAVE_FILES + word :) );
         numbers = allocate( 5 );
         text = "Skill awards made for "+ word +":\n";
         foreach ( i in sort_array( keys( awards ), 1 ) ) {
            j = 0;
            foreach ( word in keys( awards[ i ] ) )
               j += awards[ i ][ word ];
            text += sprintf( "   level%5d awarded by%5d tasks\n", i, j );
            if ( i < 301 )
               switch ( j ) {
                  case 1 .. 4 :
                     numbers[ 0 ]++;
                     break;
                  case 5 .. 9 :
                     numbers[ 1 ]++;
                     break;
                  case 10 .. 19 :
                     numbers[ 2 ]++;
                     break;
                  case 20 .. 49 :
                     numbers[ 3 ]++;
                     break;
                  default :
                     numbers[ 4 ]++;
               }
         }
         text += sprintf( "Coverage at the  1 task level is%4d%%.\n",
               ( numbers[ 0 ] + numbers[ 1 ] + numbers[ 2 ] +
               numbers[ 3 ] + numbers[ 4 ] ) / 3 );
         text += sprintf( "Coverage at the  5 task level is%4d%%.\n",
               ( numbers[ 1 ] + numbers[ 2 ] + numbers[ 3 ] +
               numbers[ 4 ] ) / 3 );
         text += sprintf( "Coverage at the 10 task level is%4d%%.\n",
               ( numbers[ 2 ] + numbers[ 3 ] + numbers[ 4 ] ) / 3 );
         text += sprintf( "Coverage at the 20 task level is%4d%%.\n",
               ( numbers[ 3 ] + numbers[ 4 ] ) / 3 );
         text += sprintf( "Coverage at the 50 task level is%4d%%.\n",
               numbers[ 4 ] / 3 );
         this_player()->set_finish_func( "main_menu", this_object() );
         this_player()->more_string( text, "Tasks", 1 );
         return;
      case 'H' :
      case 'h' :
         printf( "\nTasks editor commands:\n\n"+
               "Q         : quit the quest editor\n"+
               "L         : list tasked skills\n"+
               "C <skill> : examine coverage for <skill>\n"+
               "H         : get this list of commands\n\n" );
         break;
      default :
   }
   printf( "Choose one of Q, L, C or H (for help) : " );
   input_to( "main_menu" );
} /* main_menu() */

int cmd() {
   main_menu( "L" );
   return 1;
} /* cmd() */
