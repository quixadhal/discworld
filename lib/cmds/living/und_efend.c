/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: unp_rotect.c,v 1.6 1999/04/13 01:15:25 pinkfish Exp $
 * 
*/
inherit "/cmds/base";

#define TP this_player()

mixed cmd(object *things) {
  object *defendees;
  object ob;
  object *fail;

  if(!things) {
    things = ({ });
  }
  things -= ({ TP });

  if ( !sizeof( things ) ) {
    add_failed_mess("You cannot undefend yourself!\n");
    return 0;
  }

  defendees = ({ });
  fail = ({ });

  foreach ( ob in things) {
    if ( ob->remove_defender( TP ) ) {
      defendees += ({ ob });
    } else  {
       fail += ({ ob });
    }
  }
  
  if ( sizeof( fail ) ) {
    add_failed_mess( "You are not defending " + query_multiple_short( fail ) +
                     ".\n" );
  }
  if (sizeof(defendees)) {
     add_succeeded_mess("$N stop$s defending $I.\n", defendees);
     return 1;
  } else {
     return 0;
  }
} /* cmd() */

mixed *query_patterns() {
   return ({ "<indirect:living>", (: cmd($1) :) }); 
} /* query_patterns() */
