
#define GROUP_ARRAY_SIZE      3
#define GROUP_ARRAY_FUNCTION  0
#define GROUP_ARRAY_MEM_REQ   1
#define GROUP_ARRAY_LDR_REQ   2

/**
 * This is the base file for a group sub command.  All group sub commands
 * inherit this file.
 * <p>
 * To create a new sub command that plugs in to the master "group"
 * command, create a new file in the directory where sub commands
 * are stored and have it inherit this file.
 * <p>
 * One file can support several verbs, however it is strongly discouraged
 * unless they are *very* closely tied together.  The file also supports
 * different patterns for the same verb, which is no problem, and in no
 * way discouraged.
 * <p>
 * It is suggested you name the file the same name as the verb(s) it
 * declares.
 *
 * @author Tape
 */

mapping _commands;

/** @ignore */
void create() {
   _commands = ([ ]);
} /* create() */


/**
 * This is the signature function for a group sub command.
 * @return number of sub-commands
 */
int query_group_sub_command_amount() {
   return sizeof( _commands );
} /* query_group_sub_command() */


/**
 * This function declares a new sub command.  If the verb provided already
 * exists, it assumes it is a version of the command that uses a different
 * pattern.  If the verb *and* the pattern already exist, the function
 * will fail.  The cmd_function variable should point to the function
 * which contains the command code.  This function will return 1 or 0
 * depending if the command succeeds and is passed the same arguments
 * as normal commands get.  An additional argument ($6) can be
 * included which returns the group this_player() is a member of
 * (if any).
 *
 * @example add_group_sub_command( "join", "<indirect:object'member of a
 group'>", (: do_join( $1, $6 ) :)
 *
 * @param verb the verb for the command
 * @param pattern the command pattern
 * @param cmd_function the command function
 *
 * @return 1 on success, 0 on failure
 * @see add_command()
 */
int add_group_sub_command( string verb, string pattern,
   function cmd_function ) {

   if( !_commands[ verb ] ) {
      _commands += ([ verb : ([ pattern : ({ cmd_function, 0, 0 }) ]) ]);
      return 1;
   }

   if( _commands[ verb ][ pattern ] ) {
      return 0;
   }
   
   _commands[ verb ] += ([ pattern : ({ cmd_function, 0, 0 }) ]);
   
   return 1;
   
} /* add_group_sub_command() */


/**
 * This function will remove a command from the list of declared sub
 * commands.  If a pattern is provided, it will try to only remove
 * that specific pattern.
 *
 * @param verb the verb to be removed
 * @param pattern the specific pattern to be removed (optional)
 * @return 1 on success, 0 on failure
 */
varargs int remove_group_sub_command( string verb, string pattern ) {
   if( undefinedp( pattern ) ) {      
      if( !_commands[ verb ] ) {
         return 0;
      }
      map_delete( _commands, verb );
      return 1;
   }
   if( !_commands[ verb ] || !_commands[ verb ][ pattern ] ) {
      return 0;
   }
   map_delete( _commands[ verb ], pattern );
   return 1;
} /* remove_group_command() */


/**
 * This function returns all the verbs that have been declared by
 * this object.
 * @return a string array of the verbs
 */
string *query_group_sub_command_verbs() {
   return keys( _commands );
} /* query_group_sub_command_verbs() */


/**
 * This function returns all patterns for a specific verb.
 * @param verb the verb you want patterns for
 * @return a string array of the patterns, 0 if verb doesn't exist
 */
string *query_group_sub_command_patterns( string verb ) {

   if( !_commands[ verb ] ) {
      return 0;
   }
   
   return keys( _commands[ verb ] );

} /* query_group_sub_command_patterns() */


/**
 * This function returns the command function for a specific verb and
 * pattern.  If only the verb is provided, all functions for that
 * verb are returned (in no particular order).
 * @param verb the command verb
 * @param pattern the command pattern (optional)
 * @return array of pointer(s) to the function, 0 if verb/pattern doesn't exist
 */
varargs function *query_group_sub_command_function( string verb, string
   pattern ) {
   
   if( !_commands[ verb ] ) {
      return 0;
   }
   
   if( undefinedp( pattern ) ) {
      return values( _commands[ verb ] );
   }

   if( !_commands[ verb ][ pattern ] ) {
      return 0;
   }
   
   return _commands[ verb ][ pattern ][ GROUP_ARRAY_FUNCTION ];

} /* query_group_sub_command_function() */


/**
 * This function sets a flag which requires the player accessing
 * the command to be a member of a group.  This is automatically
 * handled by the master command before the actual command function
 * is called.  If no pattern is specified, then the flag is set
 * for all patterns of that verb.
 *
 * @param verb the command verb
 * @param pattern the command pattern, or 0
 * @return 1 on success, 0 if verb/pattern not found
 */
int set_membership_required( string verb, string pattern, int flag ) {
   
   mixed *dummy;
   string key;
   
   if( !_commands[ verb ] ) {
      return 0;
   }
   
   if( pattern ) {
      if( !_commands[ verb ][ pattern ] ) {
         return 0;
      }
      _commands[ verb ][ pattern ][ GROUP_ARRAY_MEM_REQ ] = flag;
      return 1;
   }
   
   foreach( key, dummy in _commands[ verb ] ) {
      _commands[ verb ][ key ][ GROUP_ARRAY_MEM_REQ ] = flag;
   }

   return 1;
} /* set_membership_required() */


/**
 * This function sets a flag which requires the player accessing
 * the command to be the leader of a group.  This is automatically
 * handled by the master command before the actual command function
 * is called.  If no pattern is specified, then the flag is set
 * for all patterns of that verb.
 *
 * @param verb the command verb
 * @param pattern the command pattern, or 0
 * @return 1 on success, 0 if verb/pattern not found
 */
int set_leadership_required( string verb, string pattern, int flag ) {

   mixed *dummy;
   string key;

   if( !_commands[ verb ] ) {
      return 0;
   }
   
   if( pattern ) {
      if( !_commands[ verb ][ pattern ] ) {
         return 0;
      }
      _commands[ verb ][ pattern ][ GROUP_ARRAY_LDR_REQ ] = flag;
      return 1;
   }
   
   foreach( key, dummy in _commands[ verb ] ) {
      _commands[ verb ][ key ][ GROUP_ARRAY_LDR_REQ ] = flag;
   }

   return 1;
} /* set_leadership_required() */


/**
 * This function returns whether a command user has to be a member
 * of a group in order to use that command.  If no pattern is
 * provided, a flag for a random pattern for that verb is returned.  This
 * is not suggested unless it is certain that all patterns use
 * the same settings, and/or there is only one pattern for that
 * verb.
 *
 * @param verb the command verb
 * @param pattern the pattern for the verb (optional)
 * @return 1 if membership is required, 0 if not, -1 if verb/pattern not found
 */
varargs int query_membership_required( string verb, string pattern ) {
   
   if( !_commands[ verb ] ) {
      return -1;
   }
   
   if( undefinedp( pattern ) ) {
      return _commands[ verb ][ keys( _commands[ verb ] )[ 0 ] ]
         [ GROUP_ARRAY_MEM_REQ ];
   }
   
   if( !_commands[ verb ][ pattern ] ) {
      return -1;
   }
   
   return _commands[ verb ][ pattern ][ GROUP_ARRAY_MEM_REQ ];

} /* query_membership_required() */


/**
 * This function returns whether a command user has to be the leader
 * of a group in order to use that command.  If no pattern is
 * provided, a flag for a random pattern for that verb is returned.  This
 * is not suggested unless it is certain that all patterns use
 * the same settings, and/or there is only one pattern for that
 * verb.
 *
 * @param verb the command verb
 * @param pattern the pattern for the verb (optional)
 * @return 1 if membership is required, 0 if not, -1 if verb/pattern not found
 */
varargs int query_leadership_required( string verb, string pattern ) {
   
   if( !_commands[ verb ] ) {
      return -1;
   }
   
   if( undefinedp( pattern ) ) {
      return _commands[ verb ][ keys( _commands[ verb ] )[ 0 ] ]
         [ GROUP_ARRAY_LDR_REQ ];
   }
   
   if( !_commands[ verb ][ pattern ] ) {
      return -1;
   }
   
   return _commands[ verb ][ pattern ][ GROUP_ARRAY_LDR_REQ ];

} /* query_leadership_required() */

