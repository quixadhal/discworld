/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: kill.c,v 1.18 2003/01/30 08:50:03 pinkfish Exp $
 */
#include <player.h>

inherit "/cmds/base";

int cmd( object* things ) {
   object thing;
   object *fighting;
   
   if ( !environment( this_player() ) ) {
      add_failed_mess( "You are in limbo...\n" );
      return 0;
   }
   if ( this_player()->no_offense() ) {
      add_failed_mess( "You cannot attack anyone at the moment.\n" );
      return 0;
   }

   things -= ({ this_player() });
   if ( !sizeof( things ) ) {
      add_failed_mess( "You shouldn't beat yourself up so much.\n");
      return 0;
   }
   foreach ( thing in things ) {
      if ( !userp( thing ) ) {
         if ( userp( this_player() ) ||
               !thing->query_property( "no attack" ) ) {
            this_player()->attack_ob( thing );
            thing->attack_by(this_player());
         }
         continue;
      }
      if ( !interactive( thing ) ) {
         write( (string)thing->the_short() +" is net-dead.\n" );
         things -= ({ thing });
         continue;
      }
      if ( thing->query_property( "guest" ) ) {
         write( "You cannot attack "+ (string)thing->the_short() +
               " since "+ (string)thing->query_pronoun() +" is a guest.\n" );
         things -= ({ thing });
         continue;
      }
      if ( thing->query_property( "dead" ) ) {
         write( "You cannot attack "+ (string)thing->the_short() +
               " since "+ (string)thing->query_pronoun() +
                " is already dead.\n" );
         things -= ({ thing });
         continue;
      }
      if ( thing->query_auto_loading() ) {
         write( "Be sporting; "+ (string)thing->the_short() +
               " doesn't have "+ (string)thing->query_possessive() +
               " equipment yet.\n" );
         things -= ({ thing });
         continue;
      }
      if ( userp( this_player() ) &&
           !this_player()->query_player_killer() ) {
         write( "You cannot summon the courage to attack "+
               (string)thing->the_short() +".\n" );
         things -= ({ thing });
         continue;
      }
      if ( userp( this_player() ) &&
            !thing->query_player_killer() ) {
         write( "Something tells you that it would be wrong "
               "to attack "+ (string)thing->the_short() +".\n" );
         things -= ({ thing });
         continue;
      }
      call_out( (: this_player()->attack_ob( $(thing) ) :), 0);
      call_out( (: $(thing)->attack_by(this_player()) :), 0);
   }

    if ( sizeof( things ) ) {
      fighting = ({ });
      foreach(thing in things) {
        object *list;

        list = thing->query_attacker_list();
        if (arrayp(list)) {
           fighting += list;
        }
      }
      if(sizeof(fighting))
        fighting = filter(uniq_array(fighting),
                          (: $1 && environment($1) != environment(this_player()) :));
      if(!sizeof(fighting))
        this_player()->add_succeeded_mess(this_object(),
                                          ({ "You prepare to attack " + query_multiple_short(things) + ".\n", "" }), ({ }) );
      else
        this_player()->add_succeeded_mess(this_object(),
					  ({ "You prepare to attack " + query_multiple_short(things) + " who " + (sizeof(things) > 1 ? "are" : "is") + " already hunting " + query_multiple_short(fighting) + ".\n", "" }), ({ }) );
      
        return 1;
    } else {
        add_failed_mess("");
        return 0;
    }
   
} /* cmd() */

mixed *query_patterns() {
   return ({ "<indirect:living:here>", (: cmd($1) :) });
} /* query_patterns() */
