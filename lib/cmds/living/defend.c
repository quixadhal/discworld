/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: protect.c,v 1.3 1999/04/13 01:01:44 pinkfish Exp $
 * 
*/
inherit "/cmds/base";

#define TP this_player()

mixed cmd(object *things) {
  int i;
  object *defendees;

  things -= ({ TP });

  if ( TP->query_property( "dead" ) ) {
    add_failed_mess("You cannot provide much defense as a ghost.\n" );
    return 0;
  }
  
  if ( !sizeof( things ) ) {
     add_failed_mess("You cannot defend yourself!\n");
     return 0;
  }

  if(TP->query_combat_response() != "parry") {
    add_failed_mess("Dodging out of the way will not help " +
                    query_multiple_short(things) + ".\n");
    return 0;
  }
                    
  defendees = ({ });
  for ( i = 0; i < sizeof( things ); i++ ) {
    if ( things[ i ]->add_defender( TP ) ) {
      defendees += ({ things[ i ] });
      things = delete( things, i, 1 );
      i--;
    }
  }

  if ( !sizeof( defendees ) ) {
    add_failed_mess("You cannot defend $I.\n", things);
    return 0;
  }

  if ( sizeof( things ) ) {
    write( "You cannot defend "+ query_multiple_short( things ) +".\n" );
  }
  add_succeeded_mess(({ "You will defend $I.\n",
                        "$N moves to defend $I.\n" }),
                  defendees);

  return 1;
} /* cmd() */

int show_defenders() {
   object *defend;
   object womble;

   defend = this_player()->query_defenders();
   if (sizeof(defend)) {
      write("You are being defended by " + query_multiple_short(defend) +
            ".\n");
   } else {
      write("You are not being defended by anyone.\n");
   }
   defend = ({ });
   foreach (womble in all_inventory(environment(this_player()))) {
      if (living(womble) && womble != this_player()) {
         if (member_array(this_player(), womble->query_defenders()) != -1) {
            defend += ({ womble });
         }
      }
   }
   if (sizeof(defend)) {
      write("Of the people in this room, you are defending " +
            query_multiple_short(defend) + ".\n");
   } else {
      write("You are not defending anyone in this room.\n");
   }
   return 1;
}

mixed *query_patterns() {
   return ({ "<indirect:living>",
             (: cmd($1) :),
             "",
             (: show_defenders() :) });
} /* query_patterns() */
