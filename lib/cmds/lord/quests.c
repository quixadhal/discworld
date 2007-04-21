/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: quests.c,v 1.6 2000/07/29 15:40:43 laurana Exp $
 * $Log: quests.c,v $
 * Revision 1.6  2000/07/29 15:40:43  laurana
 * fixed my help
 *
 * Revision 1.4  2000/07/24 13:29:54  taffyd
 * Tightened use of protected/private.
 *
 * Revision 1.3  2000/07/24 13:27:41  taffyd
 * Fixed up the quests command so you can't get info unless you're allowed(tm).
 *
 * Revision 1.2  1999/05/20 01:07:54  ceres
 * Added support for active/inactive flag
 *
 * Revision 1.1  1998/01/06 05:29:21  ceres
 * Initial revision
 * 
*/
#include <quest_handler.h>

#define TEXTS_DIR "/save/quests/"

inherit "/cmds/base";

private string *names;
private mapping makers;

protected void create() {
   makers = ([ ]);
} /* create() */

private void print_info( int i ) {
   string text;
   text = read_file( TEXTS_DIR + replace( names[ i ], " ", "_" ) +".txt" );
   if ( !text )
      text = "There is no text file for this quest.\n";
   printf( "\nQuest %d of %d  \"%s\", level %d\n"
           "   title: %s\n   story: %s\n"
           "   Active: %s\n"
           "   completed %d times, last by %s\n\n"
           "%s\n",
           i + 1, sizeof( names ), names[ i ],
           (int)QUEST_HANDLER->query_quest_level( names[ i ] ),
           (string)QUEST_HANDLER->query_quest_title( names[ i ] ),
           (string)QUEST_HANDLER->query_quest_story( names[ i ] ),
           (int)QUEST_HANDLER->query_quest_status(names[i]) ? "Yes" : "No",
           (int)QUEST_HANDLER->query_quest_times( names[ i ] ),
           capitalize( (string)QUEST_HANDLER->query_quest_done( names[ i ] ) ),
           text );
} /* print_info() */


private void change_status( int i ) {
  
  switch(QUEST_HANDLER->change_quest_status(names[i])) {
  case 1:
    printf("Quest %s set to active.\n", names[i]);
    break;
  case 0:
    printf("Quest %s set to inactive.\n", names[i]);
    break;
  case -1:
    printf("No such quest: %s.\n", names[i]);
    break;
  }
}

void main_menu( string word ) {
   int number, which;
   string name;
   mixed *args;
   
   which = makers[ this_player() ][ 0 ];
   args = makers[ this_player() ][ 1 ];
   if ( !word || ( word == "" ) )
      word = " ";
   switch ( word[ 0 ] ) {
      case 'Q' :
      case 'q' :
         return;
      case 'N' :
      case 'n' :
         if ( which > sizeof( names ) - 2 ) {
            printf( "No more quests.\n" );
            which = sizeof( names ) - 1;
            break;
         }
         which++;
         print_info( which );
         break;
      case 'A':
      case 'a':
         change_status(which);
         break;
      case 'P' :
      case 'p' :
         if ( which < 1 ) {
            printf( "No previous quests.\n" );
            which = 0;
            break;
         }
         which--;
         print_info( which );
         break;
      case 'G' :
      case 'g' :
         if ( strlen( word ) < 3 ) {
            printf( "Syntax: G <number>\n" );
            break;
         }
         if ( sscanf( word[ 2 .. ], "%d", number ) != 1 ) {
            printf( "Syntax: G <number>\n" );
            break;
         }
         if ( ( number < 1 ) || ( number > sizeof( names ) ) ) {
            printf( "Number must be between 1 and %d.\n", sizeof( names ) );
            break;
         }
         which = number - 1;
         print_info( which );
         break;
      case 'C' :
      case 'c' :
         if ( strlen( word ) < 3 ) {
            printf( "Syntax: C <quest>\n" );
            break;
         }
         if ( args[ 0 ] < 1 ) {
            printf( "No quest level set.\n" );
            break;
         }
         if ( !stringp( args[ 1 ] ) ) {
            printf( "No quest title set.\n" );
            break;
         }
         if ( !stringp( args[ 2 ] ) ) {
            printf( "No quest story set.\n" );
            break;
         }
         word = word[ 2 .. ];
         if ( member_array( word, names ) != -1 ) {
            printf( "There is already a quest \""+ word +"\".\n" );
            break;
         }
         QUEST_HANDLER->add_quest( word, args[ 0 ], args[ 1 ], args[ 2 ] );
         names += ({ word });
         args = allocate( 3 );
         printf( "Quest \""+ word +"\" added.\n" );
         which = sizeof( names ) - 1;
         print_info( which );
         break;
      case 'L' :
      case 'l' :
         if ( strlen( word ) < 3 ) {
            printf( "Syntax: L <level>\n" );
            break;
         }
         if ( sscanf( word[ 2 .. ], "%d", number ) != 1 ) {
            printf( "Syntax: L <level>\n" );
            break;
         }
         if ( number < 1 ) {
            printf( "Level must be at least 1.\n" );
            break;
         }
         args[ 0 ] = number;
         printf( "Quest level set to "+ number +".\n" );
         break;
      case 'T' :
      case 't' :
         if ( strlen( word ) < 3 ) {
            printf( "Syntax: T <title>\n" );
            break;
         }
         word = word[ 2 .. ];
         if ( member_array( word, (string)QUEST_HANDLER->query_quest_titles() )
               != -1 ) {
            printf( "There is already a quest that gives the title \""+
                  word +"\".\n" );
            break;
         }
         args[ 1 ] = word;
         printf( "Quest title set to \""+ word +"\".\n" );
         break;
      case 'S' :
      case 's' :
         if ( strlen( word ) < 3 ) {
            printf( "Syntax: S <story>\n" );
            break;
         }
         word = word[ 2 .. ];
         if ( member_array( word, (string)QUEST_HANDLER->query_quest_stories() )
               != -1 ) {
            printf( "There is already a quest that has the story \""+
                  word +"\".\n" );
            break;
         }
         args[ 2 ] = word;
         printf( "Quest story set to \""+ word +"\".\n" );
         break;
      case 'R' :
      case 'r' :
         printf( "\nQuest data so far:\n" );
         if ( args[ 0 ] < 1 )
            word = "not set";
         else
            word = sprintf( "%d", args[ 0 ] );
         printf( "   level: %s\n", word );
         if ( !stringp( args[ 1 ] ) )
            word = "not set";
         else
            word = args[ 1 ];
         printf( "   title: %s\n", word );
         if ( !stringp( args[ 2 ] ) )
            word = "not set";
         else
            word = args[ 2 ];
         printf( "   story: %s\n\n", word );
         break;
      case 'E' :
      case 'e' :
         printf( "Editing the text file...\n"
               "The log will show that you edited this file; if you "
               "ruin it,\nyou will be hunted down and eviscerated.\n" );
         this_player()->do_edit( read_file( TEXTS_DIR +
               replace( names[ which ], " ", "_" ) +".txt" ),
               "end_edit" );
         return;
      case 'F' :
      case 'f' :
         if ( strlen( word ) < 3 ) {
            printf( "Syntax: F <word>\n" );
            break;
         }
         word = word[ 2 .. ];
         number = 0;
         foreach ( name in ( names[ which + 1 .. ] + names[ 0 .. which ] ) ) {
            which = ( which + 1 ) % sizeof( names );
            if ( strsrch( name, word ) != -1 ) {
               print_info( which );
               number = 1;
               break;
            }
            if ( strsrch( lower_case( (string)QUEST_HANDLER->
                  query_quest_title( name ) ), word ) != -1 ) {
               print_info( which );
               number = 1;
               break;
            }
            if ( strsrch( lower_case( (string)QUEST_HANDLER->
                  query_quest_story( name ) ), word ) != -1 ) {
               print_info( which );
               number = 1;
               break;
            }
         }
         if ( !number )
            printf( "\""+ word +"\" not found.\n" );
         break;
      case 'H' :
      case 'h' :
         printf( "\nQuest editor commands:\n\n"
               "Q          : quit the quest editor\n"
               "N          : go to the next quest\n"
               "P          : go to the previous quest\n"
               "G <number> : go to a particular quest by number\n"
               "A          : toggle a quests active/inactive flag\n"
               "C <quest>  : create a new quest\n"
               "L <level>  : set a new quest's level\n"
               "T <title>  : set a new quest's title\n"
               "S <story>  : set a new quest's story\n"
               "R          : review the new quest's data\n"
               "E          : edit the quest's text file\n"
               "F <word>   : find the next quest using that word\n"
               "H          : get this list of commands\n\n"
               "To create a new quest, set the level, title and \n"
               "story first, then use C to create the quest with \n"
               "the title that creators will see.\n\n"
               "No command will reprint the current quest.\n\n" );
         break;
      default :
         print_info( which );
   }
   makers[ this_player() ] = ({ which, args });
   printf( "Choose one of Q, N, P, G, C, L, T, S, R, E, F or H (for help) : " );
   input_to( "main_menu" );
} /* main_menu() */

void end_edit( string text ) {
   string name;
   if ( !text || ( text == "" ) ) {
      printf( "Aborted.\n" );
      main_menu( "Z" );
      return;
   }
   if ( text[ <1 ] != '\n' )
      text += "\n";
   name = names[ makers[ this_player() ][ 0 ] ];
   log_file( "QUESTS", ctime( time() ) +" "+
         (string)this_player()->query_name() +
         " changed text file for "+ name +"\n" );
   printf( "Writing file...\n" );
   unguarded( (: write_file, TEXTS_DIR + replace( name, " ", "_" ) +
         ".txt", text, 1 :) );
   main_menu( "Z" );
} /* end_edit() */

private int cmd() {
   names = (string *)QUEST_HANDLER->query_quest_names();
   makers[ this_player() ] = ({ 0, allocate( 3 ) });
   main_menu( "G 1" );
   return 1;
} /* cmd() */

/** 
 * @ignore yes
 */
public mixed *query_patterns() {
    return ({ "", (: cmd() :) });
} /* query_patterns() */
