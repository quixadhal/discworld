/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: health.c,v 1.14 2003/07/08 05:57:33 pinkfish Exp $
 * $Log: health.c,v $
 * Revision 1.14  2003/07/08 05:57:33  pinkfish
 * Fix up to make sure people don't go out hiding.
 *
 * Revision 1.13  2003/02/21 03:45:22  pinkfish
 * Fix up an error in the logic.
 *
 * Revision 1.12  2003/02/21 03:21:48  pinkfish
 * Add in a 'wounded' setting.
 *
 * Revision 1.11  2000/09/01 01:59:04  presto
 * Fixed up mistake with my replace()...needed ({ }) around the strings
 *
 * Revision 1.10  2000/08/16 01:15:10  presto
 * Added coloring
 *
 * Revision 1.9  2000/08/12 14:13:51  presto
 *  Forcibly unlocked by terano
 *
 * Revision 1.8  2000/07/16 02:27:54  presto
 * Fixed up the messages a bit, so that when you health yourself, it looks right
 *
 * Revision 1.7  2000/07/15 06:16:15  pinkfish
 * Remove the 5 restriction.
 *
 * Revision 1.6  2000/07/13 00:30:28  pinkfish
 * Make it tell someone their health is being c hecked out and also limit it
 * to 5 people.
 *
 * Revision 1.5  2000/07/13 00:29:09  pinkfish
 * Stop people getting the health of everyone at once.
 *
 * Revision 1.4  1999/10/25 23:06:09  pinkfish
 * Some fixes to make things look better.
 *
 * Revision 1.3  1999/05/17 21:15:39  pinkfish
 * Fix it up to work with offler.
 *
 * Revision 1.2  1998/09/27 20:35:00  ceres
 * Fixed dark messages
 *
 * Revision 1.1  1998/01/06 05:29:43  ceres
 * Initial revision
 * 
*/
// Health command by Ceres

inherit "cmds/base";

string *level_colors = ({ "", "%^BOLD%^%^RED%^", "%^RED%^", "%^YELLOW%^",
                          "%^CYAN%^", "" });
mixed cmd(object *who, int wounded) {
   object person;
   string health;
   int level;
   int found;

   if(this_player()->check_dark((int)environment(this_player())->
                                 query_light())) {
      add_failed_mess("You cannot see well enough to examine someone's "
                      "health.\n");
      return 0;
   }
    
   foreach(person in who) {
      health = person->health_string(1, ref level);
      if (this_player() == person) {
         health = replace(health, ({ "is ", "are ", "appears ", "appear " }));
      }
      if (health) {
         if (!wounded || level != 5) {
            write(level_colors[level] + "$C$" + person->one_short(1) + " " +
                  level_colors[level] + health + ".\n%^RESET%^");
            found++;
         }
      } else {
         write(person->one_short(1) + " is so unhealthy they have forgotten "
               "how healthy they are.\n");
      }
      if (person != this_player())
         tell_object(person, this_player()->the_short(1) + " appears to be "
                  "checking out all your wounds.\n");
   }

   if (wounded && !found) {
      add_failed_mess("Unable to find any wounded people.\n");
      return 0;
   }

   say(this_player()->one_short(1) + " studies " +
       query_multiple_short(who) + ".\n");
   return 1;
} /* cmd() */

mixed *query_patterns() {
  return ({ "<indirect:living>", (: cmd($1, 0) :),
            "wounded <indirect:living>", (: cmd($1, 1) :) });
} /* query_patterns() */
