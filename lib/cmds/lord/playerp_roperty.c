/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: playerp_roperty.c,v 1.2 2003/03/21 01:57:25 ceres Exp $
 * $Log: playerp_roperty.c,v $
 * Revision 1.2  2003/03/21 01:57:25  ceres
 * Modified to use player handler
 *
 * Revision 1.1  1998/01/06 05:29:21  ceres
 * Initial revision
 * 
*/
#include <parser.h>
#include <player_handler.h>

inherit "/cmds/base";

mapping map_prop;

/*
 * This will add a property to a player.
 */
mixed cmd(string str) {
   string name;
   string value;
   string player;
   mixed *args;

   if (!str) {
      return notify_fail("You must give a property...\n");
   }

   if (sscanf(str, "%s \"%s\" %s", player, name, value) == 3) {
      seteuid("Root");
      /* Restore them... */
      unguarded( (: restore_object("/players/" + $(player[0..0]) +
                                   "/" + $(player)) :));
      if (PLAYER_HANDLER->test_user(player)) {
         args = PARSER->parse_args(value);
         if (args && sizeof(args[0]) > 0) {
            map_prop[name] = args[0][0];
            unguarded( (: write_file("/players/" + $(player[0..0]) +
                               "/" + $(player) + ".o",
                       "map_prop "+save_variable(map_prop)+"\n") :));
            write(sprintf("Added property %s to %s; value = %O\n",
                          name, player, args[0][0]));
         } else {
            write("No useful args...\n");
         }
      } else {
         write("Unable to find player "+player+".\n");
      }
   } else {
      write("Syntax: player_add_property <name> \"<prop>\" <value>\n");
   }

   return 1;
} /* cmd() */
