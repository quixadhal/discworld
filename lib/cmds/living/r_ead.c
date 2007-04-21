/*  -*- LPC -*-  */
/*
 * $Locker: ceres $
 * $Id: r_ead.c,v 1.19 2003/01/30 07:12:19 pinkfish Exp ceres $
 * $Log: r_ead.c,v $
 * Revision 1.19  2003/01/30 07:12:19  pinkfish
 * Add in a nerw line on the end.
 *
 * Revision 1.18  2001/09/08 01:42:48  ceres
 * Fixed bright/dark mixup
 *
 * Revision 1.17  2001/03/07 21:38:18  pinkfish
 * Make the bright/dark messages correct.
 *
 * Revision 1.16  2000/01/03 10:12:16  taffyd
 * Fixed typo and changed the success array so names would merge together. (three gold rings)
 *
 * Revision 1.15  1999/12/16 23:07:28  pinkfish
 * Stop people reading stuff in other peoples inventories.
 *
 * Revision 1.14  1999/08/19 01:21:53  pinkfish
 * Fix up this so it doesn't give weird messages for reading yourself.
 *
 * Revision 1.13  1999/08/19 01:09:09  pinkfish
 * Change it so you cannot read in the dark.
 *
 * Revision 1.12  1998/11/23 06:33:09  pinkfish
 * Really fixed the error.
 *
 * Revision 1.11  1998/11/23 06:20:10  pinkfish
 * Fix up a bug with add_items() and read.
 *
 * Revision 1.10  1998/09/15 21:30:01  pinkfish
 * Fix up read to say: reads rather than rs.
 *
 * Revision 1.9  1998/09/15 00:35:51  pinkfish
 * Make the read short stuff work if the function doesn't exist.
 *
 * Revision 1.8  1998/09/11 08:07:07  pinkfish
 * Fix up the sillyness with the read failures.
 *
 * Revision 1.7  1998/09/11 07:50:59  pinkfish
 * Fix this up to do the error messages correctly.
 *
 * Revision 1.6  1998/09/11 06:45:32  pinkfish
 * Stop it printing that silly 0.
 *
 * Revision 1.5  1998/09/11 05:56:24  pinkfish
 * Fixed up the read command to be better done.  Following real oo paradim
 * ideas.
 *
 * Revision 1.3  1998/08/16 03:09:47  tape
 * Minor typo fix when reading living things.
 *
 * Revision 1.2  1998/01/31 20:11:55  pinkfish
 * Allow the items on things to be readadable again.
 *
 * Revision 1.1  1998/01/06 05:28:43  ceres
 * Initial revision
 * 
*/
#include <language.h>

inherit "/cmds/base";

varargs string create_text( object thing, int label );

mixed cmd( object *things ) {
   string gendstr;
   string text;
   string read;
   string reader;
   string *others;
   string r_short;
   object thing;
   mapping hide_invis;
   int dark;
 
   dark = this_player()->check_dark(environment(this_player())->query_light());
   if (dark < 0) {
      add_failed_mess("It is too dark to read $I.\n", things);
      return 0;
   }
   if (dark > 0) {
      add_failed_mess("It is too bright to read $I.\n", things);
      return 0;
   }

   reader = "";
   others = ({ });
   foreach( thing in things ) {
      if (!is_in_me_or_environment(thing, this_player())) {
          add_failed_mess("You cannot read $I.\n", ({ thing }));
          continue;
      }

      text = thing->query_readable_message(this_player());
      read = thing->query_read_short();
      if (!read) {
         read = "$name$";
      }
      if ( !text) {
        /* This oculd be a tricky item obvject... */
        if (living(thing)) {
            gendstr = thing->query_objective();
        } else {
           gendstr = "it";
        }
        r_short = thing->query_read_short();
        if (!r_short) {
           r_short = "$name$";
        }
        if (!thing->command_control("read")) {
            if (thing != this_player()) {
               reader += "$C$"+ replace_string(r_short,
                                        "$name$",
                                        thing->a_short()) +
                     " does not have anything written on " +
                     gendstr + ".\n";
            } else {
               reader += "You cannot read yourself.\n";
            }
         } else {
            others += ({ replace( read, "$name$", (string)thing->a_short() ) });
            reader += "";
         }
         continue;
      }
      reader += "You read "+ replace_string( read, "$name$",
            (string)thing->the_short() ) +":\n"+ text + "\n";
      if ( read == "$name$" ) {
          others += ({ thing });
      }
      else {     
          others += ({ replace( read, "$name$", (string)thing->a_short() ) });
      }
   }
   /* Print success and failure messages. */
   if ( !sizeof( others ) ) {
      add_failed_mess(reader);
      return 0;
   }
   write( "$P$Read$P$"+ reader );

   hide_invis = this_player()->query_hide_invis();
   if(!hide_invis["hiding"]) {
      add_succeeded_mess(({ "", "$N read$s $I.\n" }), others);
   }
   return 1;
} /* cmd() */

mixed *query_patterns() {
   return ({ "<indirect:object>", (: cmd($1) :) });
} /* query_patterns() */
