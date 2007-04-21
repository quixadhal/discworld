/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: unp_rotect.c,v 1.6 1999/04/13 01:15:25 pinkfish Exp $
 * $Log: unp_rotect.c,v $
 * Revision 1.6  1999/04/13 01:15:25  pinkfish
 * Fix this up to handle errors when your not protecting anyone and you
 * type unproctect all.
 *
 * Revision 1.5  1998/09/15 00:42:38  pinkfish
 * Fix up the messages.
 *
 * Revision 1.4  1998/08/25 09:31:34  pinkfish
 * Fix it up to look a little nicer.
 *
 * Revision 1.3  1998/08/22 13:52:18  gototh
 * Was runtiming because things wasn't getting set to obs.
 *
 * Revision 1.2  1998/08/22 05:50:53  pinkfish
 * Changed to use add_command.
 *
 * Revision 1.1  1998/01/06 05:28:43  ceres
 * Initial revision
 * 
*/
inherit "/cmds/base";

#define TP this_player()

mixed cmd(object *things) {
  object *protectees;
  object ob;
  object *fail;

  if(!things) {
    things = ({ });
  }
  things -= ({ TP });

  if ( !sizeof( things ) ) {
    add_failed_mess("You cannot unprotect yourself!\n");
    return 0;
  }

  protectees = ({ });
  fail = ({ });

  foreach ( ob in things) {
    if ( ob->remove_protector( TP ) ) {
      protectees += ({ ob });
    } else  {
       fail += ({ ob });
    }
  }
  
  if ( sizeof( fail ) ) {
    add_failed_mess( "You are not protecting " + query_multiple_short( fail ) +
                     ".\n" );
  }
  if (sizeof(protectees)) {
     add_succeeded_mess("$N stop$s protecting $I.\n", protectees);
     return 1;
  } else {
     return 0;
  }
} /* cmd() */

mixed *query_patterns() {
   return ({ "<indirect:living>", (: cmd($1) :) }); 
} /* query_patterns() */
