#include <group_handler.h>
#include <function.h>
#include <player.h>

// Define IN_TESTING below if you want to restrict the usage of
// these commands only to creators, playtesters and test
// characters.

#undef IN_TESTING

// Define IN_STRICT_TESTING in addition to IN_TESTING if
// you wish only creators to be able to use the commands.

#undef IN_STRICT_TESTING

mixed *_patterns;
mapping _sub_commands;

class sub_command {
   // The verb is the key of the _sub_commands mapping.
   string file_name;         // Where the sub-command file is located
   mixed *data;         // This contains alternating elements of
                        // (string)command patterns and (function)
                        // function pointers to the command.
}


#if defined( IN_TESTING ) || defined( IN_STRICT_TESTING )
int allowed_to_use( object user );
#endif

void rehash_group_sub_commands();
void make_patterns_array();
mixed *query_patterns();

int group_command_control( string verb, mixed *indirect_obs,
   string dir_match, string indir_match, mixed *args, string pattern );

int do_help( string on_what );


void create() {
   rehash_group_sub_commands();
   make_patterns_array();
} /* create() */


/* This function reads the GROUP_SUB_CMDS_DIR for files matching
 * GROUP_SUB_CMDS_FILE_WILDCARD, loads them one by one, and extracts
 * information from them if they respond to the group sub command
 * signature function.
 */
void rehash_group_sub_commands() {

   mixed *files;                         // An array of the files in the dir.
   object cmd_object;                    // The active object being used.
   string dir, verb, file, pattern;      // Stuff to be extracted and shuffled
                                         // around.
   
   // flush the sub commands
   _sub_commands = ([ ]);
   
   dir = GROUP_SUB_CMDS_DIR;
   dir += GROUP_SUB_CMDS_FILE_WILDCARD;
   
   // read the group sub-command directory for command files
   files = get_dir( dir );
   
   // if there were no files found, stop
   if( !sizeof( files ) ) {
      return;
   }
   
   // iterate through the files and add data to the mapping
   foreach( file in files ) {

      // remove the file extension
      sscanf( file, "%s.%*s", file );
      
      // load the file or find it if it's already loaded
      cmd_object = load_object( ( GROUP_SUB_CMDS_DIR + file ) );
      
      if( !cmd_object ) {
         // can't be loaded.  Probably broken, so let's skip it.
         continue;
      }

      // if the file isn't a group sub-command file
      if( !cmd_object->query_group_sub_command_amount() ) {
         continue;
      }
      
      // find all verbs the command has declared and
      // iterate through them
      foreach( verb in cmd_object->query_group_sub_command_verbs() ) {

         // if this is an entirely new verb and not just an additional
         // pattern
         if( !_sub_commands[ verb ] ) {         
            // assign a new entry for it
            _sub_commands += ([ verb : new( class sub_command ) ]);
         }
         
         _sub_commands[ verb ]->data = ( mixed * )({ });
         _sub_commands[ verb ]->file_name = ( string )file_name( cmd_object );
         
         // fetch all the patterns for that verb
         foreach( pattern in
            cmd_object->query_group_sub_command_patterns( verb ) ) {
            
            // add the pattern and the corresponding command function to
            // the entry
            _sub_commands[ verb ]->data +=
               ({
               pattern,
               cmd_object->query_group_sub_command_function( verb, pattern )
               });

         }  // foreach pattern
      }  // foreach verb
   } // foreach file

} /* rehash_group_sub_commands() */


int group_command_control( string verb, mixed *indirect_obs,
   string dir_match, string indir_match, mixed *args, string pattern ) {
   
   int count, size;
   string cmd_pattern, mangled_pattern, group;
   function cmd_fun;
   object cmd_object;
   class sub_command info;

#if defined( IN_TESTING ) || defined( IN_STRICT_TESTING )
   if( !allowed_to_use( this_player() ) ) {
      return 0;
   }
#endif

   info = _sub_commands[ verb ];
   
   if( !info ) {
      printf( "ERROR: Command information for \"" + verb + "\""
         " not found.\n" );
      return 0;   
   }
   
   // find the right version of the command (there can be different
   // patterns for the same verb)

   if( pattern != verb ) {
      // means the command takes arguments -- let's get rid
      // of the verb name from the front
      sscanf( pattern, verb + " %s", mangled_pattern );
   }
   else {
      // means the command takes no arguments
      mangled_pattern = "";
   }
   
   size = sizeof( info->data );   

   for( count = 0; count < size; count += 2 ) {
      if( mangled_pattern == info->data[ count ] ) {
         // we now have a match on the verb and the pattern wanted.
         cmd_pattern = info->data[ count ];
         cmd_fun     = info->data[ count + 1 ];
         break;
      }
   }
   
   if( !cmd_pattern || !cmd_fun ) {
      printf( "ERROR: Correct version of \"" + verb + "\" not found.\n" );
      tell_creator( this_player(), "DEBUG: Verb: %s, pattern: %s\n",
         verb, pattern );
      return 0;
   }
   
   if( !cmd_object = load_object( info->file_name ) ) {
      // object's probably broken
      printf( "ERROR: Cannot load command \"" + verb + "\"!\n" );
      tell_creator( this_player(), "DEBUG: File name: %s\n", info->file_name );
      return 0;
   }   

   // if the command function's pointer has been destructed.. This happens
   // when the command object is unloaded after a period of unuse.
   if( functionp( cmd_fun ) & FP_OWNER_DESTED ) {   
      // try to re-acquire the pointer and update the entry as well
      info->data[ count + 1 ] = cmd_fun =
         cmd_object->query_group_sub_command_function( verb, cmd_pattern );         
   }
   
   // if the pointer couldn't be re-acquired
   if( !cmd_fun || !functionp( cmd_fun ) ) {
      printf( "ERROR: Could not find command function for verb " +
         "\"" + verb + "\".\n" );
      return 0;   
   }

   group = this_player()->query_group();
   
   // Check if the command requires this_player() to be a member of
   // a group.
   if( cmd_object->query_membership_required( verb, cmd_pattern ) > 0 ) {
      if( !group ) {
         return notify_fail( "You must be a member of a group in order "
            "to use this command.\n" );
      }
   }
   
   // Check if the command requires the command user to be the leader
   // of the group.
   if( cmd_object->query_leadership_required( verb, cmd_pattern ) > 0 ) {
      if( GROUP->leader_of( group ) != this_player() ) {
         return notify_fail( "Only the leader of a group can use this "
            "command.\n" );
      }
   }
   
   // Call the command function on it and return what it returned
   
   // The parameters for it are identical to a normal add_command(),
   // with the exception of an additional one which designates the
   // command user's current group, if any.
   
   return evaluate( cmd_fun, indirect_obs, dir_match, indir_match,
      args, pattern, group );

} /* group_command_control() */


int do_help( string on_what ) {
   
   int top_left;
   string help, message;
   object command;
   
   on_what = lower_case( on_what );

#if defined( IN_TESTING ) || defined( IN_STRICT_TESTING )
   if( !allowed_to_use( this_player() ) ) {
      return 0;
   }
#endif

   if( on_what == "help" ) {
      // "group help help"
      return notify_fail( "To get help on a command, use \"group help <sub-"
         "command>\".  That is, if you wanted to get help on the command "
         "\"group create <name>\", you would type \"group help create\".\n" );
   }
   
   // such a sub-command doesn't exist
   if( !_sub_commands[ on_what ] ) {
      return notify_fail( "There is no sub-command called \"" + on_what +
         "\".\n" );
   }

   command = load_object( _sub_commands[ on_what ]->file_name );
   
   if( !command ) {
      tell_creator( this_player(), "Alleged file name: %s\n",
         _sub_commands[ on_what ]->file_name );
      return notify_fail( "ERROR: Cannot find or load sub-command "
         "\"" + on_what + "\".\n" );
   }
   
   help = command->query_help_string_for( on_what );
   
   if( !help ) {
      return notify_fail( "No help found for sub-command \"" + on_what +
         "\".\n" );
   }
   
   top_left = this_player()->query_cols();
   
   message = sprintf(
      "\n%' '|*s\n"
      "%' '-=*s\n",
      top_left, "Help on sub-command \"" + on_what + "\":\n",
      top_left, help );
   
   this_player()->more_string( message );
   
   return 1;
   
} /* do_help() */


/* This function parses all defined group subcommand classes into
 * a form that the parser understands.  I.e. one that can be
 * returned with query_patterns()
 */
void make_patterns_array() {

   int count, size;
   string verb, pattern;
   class sub_command command;

   // flush the patterns
   _patterns = ({ });

   // Establish a separate command for the help thingie.
   _patterns += ({
      "help <word'sub-command'>", (: do_help( $4[ 0 ] ) :) });
      
   // iterate through the commands   
   foreach( verb, command in _sub_commands ) {

      // get the verb and the pattern from the command
      // and stick them into a pattern entry to the
      // group command controller.
   
      size = sizeof( command->data );
      
      // multiple patterns for one verb?  Certainly possible.
      for( count = 0; count < size; count += 2 ) {

         // these checks are to ensure things' not breaking
         // if a verb doesn't take any arguments

         if( sizeof( command->data[ count ] ) ) {
            pattern = verb + " " + command->data[ count  ];
         }
         else {
            pattern = verb;
         }
      
         _patterns += ({
            pattern, (: group_command_control( $( verb ), $1, $2, $3, $4,
               $5 ) :) });
      }   
   }
} /* make_patterns_array() */


mixed *query_patterns() {
   return _patterns;   
} /* query_patterns() */


#if defined( IN_TESTING ) || defined( IN_STRICT_TESTING )
int allowed_to_use( object user ) {
#endif

#ifdef IN_STICT_TESTING
   if( !user->query_creator() ) {
      tell_object( user, "You are not allowed to use this feature at this "
         "time.\n" );
      return 0;
   }
#endif

#ifdef IN_TESTING
   if( !PLAYTESTER_HAND->query_tester( user ) ) {
      tell_object( user, "You are not allowed to use this feature at this "
         "time.\n" );
      return 0;
   }
#endif

#if defined( IN_TESTING ) || defined( IN_STRICT_TESTING )
   return 1;
} /* allowed_to_use() */
#endif


// Debug.
mapping dump_info() { return _sub_commands; }
mixed *dump_patterns() { return _patterns; }

