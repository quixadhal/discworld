/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: inv_entory.c,v 1.4 2001/03/11 00:17:11 ceres Exp $
 * $Log: inv_entory.c,v $
 * Revision 1.4  2001/03/11 00:17:11  ceres
 * Moved burden to the start of the inventory.
 *
 * Revision 1.3  2001/03/09 22:22:40  pinkfish
 * Add in a burden message.
 *
 * Revision 1.2  1998/04/14 02:44:58  pinkfish
 * Changed to use add_command.
 *
 * Revision 1.1  1998/01/06 05:29:43  ceres
 * Initial revision
 * 
*/
inherit "/cmds/base";

mixed cmd()
{
   if (this_player()->query_property("dead")) {
      object *obs;

      write("You are just a disembodied spirit.  "
            "You have mist; what else do you want?\n");
      obs = all_inventory(this_player());
      obs = filter_array(obs, (: $1->query_property("dead usable") :));
      if (sizeof(obs)) {
         write("Hang on!  You appear to have " +
               query_multiple_short(obs) + ".  Amazing.\n");
      }
      return 1;
   }
   write(sprintf("You are %s by:\n", this_player()->burden_string()) + 
	 this_player()->query_living_contents(1));

   if (this_player()->query_auto_loading()) {
      write("\n%^YELLOW%^Please note that your inventory is still being "
            "generated.%^RESET%^\n");
   }
   return 1;
}                               /* cmd() */

mixed *query_patterns()
{
   return ({ "", (: cmd() :) });
}                               /* query_patterns() */
