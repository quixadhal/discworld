/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: playera_ddproperty.c,v 1.5 2003/03/21 01:58:43 ceres Exp $
 * $Log: playera_ddproperty.c,v $
 * Revision 1.5  2003/03/21 01:58:43  ceres
 * Modified to use player handler
 *
 * Revision 1.4  2002/02/07 11:02:24  drakkos
 * Fixed it up to work with compression.
 *
 * Revision 1.3  2001/06/01 20:58:21  ceres
 * Can't remember
 *
 * Revision 1.2  1999/10/28 02:33:41  ceres
 * tried to make it work
 *
 * Revision 1.1  1998/01/06 05:29:21  ceres
 * Initial revision
 * 
*/
#include <parser.h>
#include <player_handler.h>

inherit "/cmds/base";

string var_name = "map_prop ";
string fname;

/*
 * This will add a property to a player.
 */
mixed cmd(string player, string name, mixed value) {
   mixed *args;
   string file;
   string *bits;
   mapping map_prop;
   int i, found;

   seteuid("Root");

   fname = "/save/players/" + player[0..0] + "/" + player;

   /* Restore them... */
   map_prop = ([ ]);
   if (!PLAYER_HANDLER->test_user(player))
     return notify_fail("Unable to find player "+player+".\n");
   uncompress_file (fname+".o.gz");
   file = unguarded( (: read_file(fname+".o") :));
   if(!file)
     return notify_fail("Error reading player file " + fname + ".\n");
      
   bits = explode(file, "\n");
   map_prop = ([ ]);
   for (i = 0; i < sizeof(bits); i++) {
     if (bits[i][0..strlen(var_name)-1] == var_name) {
       map_prop = restore_variable(bits[i][strlen(var_name) + 0..]);
       found = 1;
       break;
     }
   }
   if(!found || !map_prop)
     return notify_fail("Failed to find property mapping.\n");

   if(stringp(args)) {
     args = PARSER->parse_args(value);
     if(!args || !sizeof(args[0]))
       return notify_fail("No useful args...\n");      

     map_prop[name] = args[0][0];
     write(sprintf("Added property %s to %s; value = %O\n",
                   name, player, args[0][0]));
   } else {
     map_prop[name] = value;
   }
   
   if (i < sizeof(bits)) {
     bits[i] = var_name + save_variable(map_prop);
   } else {
     bits += ({ var_name + save_variable(map_prop) });
   }

         
   unguarded( (: rm(fname + ".old") :) );
   
   unguarded( (: rename(fname+".o", fname+".old") :) );
   
   unguarded( (: write_file(fname + ".o",
                            implode($(bits), "\n") + "\n") :));
   compress_file (fname+".o");
   
   this_player()->add_succeeded_mess (this_object(), "$N manipulate$s " 
    "reality a little.\n", ({ }));
   return 1;
} /* cmd() */

mixed *query_patterns() {
  return ({ "<word> <string:quoted> <string>", (: cmd($4[0], $4[1], $4[2]) :),
            });
} /* query_patterns() */
