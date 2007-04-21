/* $Id: which.c,v 1.3 2001/10/23 17:57:56 taffyd Exp $ */

#include <command.h>
#include <soul.h>
#include <user_parser.h>

inherit "/cmds/base";

int cmd(string str) {
  int i;
  mixed *junk, *coms;
  class command cmd;
  string file, *files;

  files = ({ });
  junk = actions_defined(this_player(), 0, 12);
  for (i = 0; i < sizeof(junk); i += 2) {
    if (junk[i] == str) {
      file = function_exists((string)junk[i+1][1], (object)junk[i+1][0]);
      if (!file) {
        file = base_name((object)junk[i+1][0]);
      }
      files += ({ file });
    }
  }
  cmd = new(class command, verb : str);
  if (CMD_D->HandleStars(cmd) &&
      sizeof((coms = (mixed *)CMD_D->GetPaths(cmd->verb) &
              (mixed *)this_player()->GetSearchPath()))) {
    files += map(coms, (: $1 + "/" + $(cmd->verb) :));
  }

  coms = this_player()->query_parse_command_objects(str);
  if (sizeof(coms)) {
    files += map(coms, (: base_name($1) :));
  }
  if ((coms = SOUL_OBJECT->query_soul_command(str))) {
    files += ({ SOUL_OBJECT });
  }
  if (!sizeof(files)) {
    return notify_fail("Command " + str + " not found.\n");
  }

  printf("List of files defining '%s', in search order:\n%s", str,
         implode(files, (: $1 + "    " + $2 + "\n" :), ""));
  return 1;
} /* cmd() */ 

int command_list() {
    mapping objects;
    mapping data = ([ ]); 
    string *dkeys;
    string command;
    int maxlen = 8; 
    int width = 0;
    string txt;
    
    // Get a list of all the objects defined on the player.
    objects = this_player()->query_p_objects(); 

    // Data is in the following format:
    //
    // "verb" : ({ objects }),
    //
    // So combine them together.        

    foreach( object ob, string *commands in objects ) {
        foreach( command in commands ) { 
            if ( data[ command ] ) { 
                data[ command ] += ({ ob }); 
            }
            else {
                data[ command ] = ({ ob });
            }

            if ( strlen( command ) > maxlen ) {
                maxlen = strlen( command );
            }
        }
    }

    width = this_player()->query_cols() - maxlen;
    
    // Get the keys, and start building the text. 

    dkeys = sort_array( keys( data ), 1 ); 
   
    txt = sprintf( "$P$Command list$P$%*-s %*-=s\n", maxlen, "Command", 
        width, "Defined On" );

    foreach( command in dkeys ) { 
        // #define LIST_FORMAT "   %12-s %*-=s\n"

        txt += sprintf( "%*-s %*-=s\n", maxlen, 
            command, width, 
            implode( map( data[ command ], (: file_name :) ), ", " ) );
    }

    // Finally display the text. 

    tell_object( this_player(), txt );
    
    return 1;
} /* command_list() */ 

mixed *query_patterns() {
  return ({ 
      "", (: command_list :), 
      "<word'command'>", (: cmd($4[0]) :) });
} /* query_patterns() */ 
