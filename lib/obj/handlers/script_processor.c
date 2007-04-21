/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: script_processor.c,v 1.1 1998/01/06 05:03:33 ceres Exp $
 * $Log: script_processor.c,v $
 * Revision 1.1  1998/01/06 05:03:33  ceres
 * Initial revision
 * 
*/
#define RATIO 20

inherit "/std/object";

string stage;
mapping actors;

void setup() {
   actors = ([ ]);
} /* setup() */

string query_stage() { return stage; }

void set_stage( string word ) { stage = word; }

mapping query_actors() { return actors; }

string query_actor( string word ) { return actors[ word ]; }

void set_actor( string name, string path ) { actors[ name ] = path; }

int perform_line( string script, int line ) {
   int length;
   string action, name, text;
   object actor;
   length = file_length( script );
   if ( length < 1 ) {
      write( "Length of file \""+ script +"\" is "+ length +".\n" );
      return -1;
   }
   if ( length < line )
      return -1;
   if ( !stage || ( stage == "" ) )
      return -2;
   text = read_file( script, line, 1 );
   if ( sscanf( text, "%s=%s\n", name, action ) == 2 ) {
      if ( !actors[ name ] )
         return -3;
      actor = find_object( actors[ name ] );
      if ( !actor )
         return -4;
      actor->do_command( action );
   } else {
      actor = find_object( stage );
      if ( !actor )
         return -4;
      tell_room( actor, text );
   }
   if ( length == line )
      return -5;
   return strlen( text ) / RATIO;
} /* perform_line() */

void perform_script( string script, int line ) {
   int next;
   next = perform_line( script, line );
   if ( next < 0 ) {
      if ( find_object( stage ) )
         stage->script_ended( next );
      return;
   }
   call_out( "perform_script", 2 + next, script, line + 1 );
} /* perform_script() */

int start_script( string script ) {
   if ( find_call_out( "perform_script" ) != -1 )
      return 0;
   call_out( "perform_script", 3, script, 1 );
   return 1;
} /* start_script() */
