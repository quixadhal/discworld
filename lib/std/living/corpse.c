/*
 * This file contains all the code for handling corpses.
 * @author Rhinehold
 */
#include <player.h>


/**
 * This method will allow all sorts of modifications to a corpse once it has
 * been created, based on the weapon or attack used for the killing stroke.
 * @param corpse the corpse object
 * @param weapon the weapon used to inflict the fatal damage
 * @param attack the attack used to inflict the fatal damage
 * @return none
 * @see death_helper()
 * @see make_corpse()
 * @see do_death()
 */
int alter_corpse( object corpse, object weapon, string attack ) {
  object head;
  string *heads;
  string behead_message = "";

  if (!attack) attack = "attack";
  switch (attack) {
    case "behead" :
      heads =
        this_object()->query_race_ob( this_object() )->query_possible_bits( "head" );
      if (sizeof(heads) == 0)
        break;

      head = corpse->make_bit(heads[random(sizeof(heads))] );
      head->move( environment( corpse ) );

      switch(random(6)) {
        case 0 : behead_message = head->a_short() +
          " drops onto the ground and rolls head over, well, head "
          "for a bit before coming to a rest.\n";
           break;

        case 1 : behead_message = head->a_short() +
          " sails through the air in a graceful arc before landing "
          "with a very soggy *squish*.\n";
          break;

        case 2 : behead_message = head->a_short() +
          " quivers a bit as it remains in place, perched atop its owner's "
          "corpse for a moment, then falls to pieces.\n";
          break;

        case 3 : behead_message = head->a_short() +
          " tips off to the left to hang by a small bit of flesh, before "
          "gravity has something to say about the matter and pulls it "
          "completely off the corpse, landing with a small bounce.\n";
          break;

        case 4 : behead_message = head->a_short() +
          " slides off its owner's corpse, upends, and lands upside "
          "down, helping to keep some of the blood from spilling.\n";
          break;

        case 5 : behead_message = head->a_short() +
          " drops onto the ground like a ball, *bouncy* *bouncy*.\n";
          break;

        case 6 :
        case 7 :
        case 8 :
        case 9 :
        default : behead_message = "oops, the mushrooms have turned.\n";
      }
      tell_room( environment( corpse ), behead_message );
      break;

    default :
      break;
  }
  return 1;
} /* alter_corpse */

/**
 * This method will create a corpse for the living object when it
 * eventually dies.  Of old age of course, no one would die of having
 * sword rammed through them
 * @return the created corpse object
 */
object make_corpse() {
   int i;
   object corpse, *armour, *weapons, race;

   race = find_object(this_object()->query_race_ob());
   if(race && function_exists("make_corpse", race))
     return race->make_corpse();

   corpse = clone_object( "/obj/corpse" );
   corpse->set_owner( 0, this_object() );
   corpse->set_ownership( (string)this_object()->query_name() );
   if ( this_object()->query_property( "player" ) ) {
      corpse->add_property( "player", 1 );
   }
   corpse->set_race_name( (string)this_object()->query_race() );
   if ( !this_object()->query_race_ob() ) {
      corpse->set_race_ob( "/std/races/unknown" );
   } else {
      corpse->set_race_ob( (string)this_object()->query_race_ob() );
   }
   corpse->add_adjective( (string *)this_object()->query_adjectives() );
   corpse->add_adjective("dead");
   corpse->remove_adjective("living");
   corpse->start_decay();
   corpse->set_max_weight(this_object()->query_max_weight());
   // Make the length a little longer than the player just in case
   // they were carrying a long weapon.
   corpse->set_length(this_object()->query_height() / 2);
   armour = (object *)this_object()->query_armours();
   this_object()->clear_armours();
   weapons = (object *)this_object()->query_weapons();
   all_inventory( this_object() )->set_tracked_item_status_reason( "DIED" );
   all_inventory( this_object() )->move( corpse );
   armour -= ({ 0 });
   for ( i = 0; i < sizeof( armour ); i++ ) {
      if ( environment( armour[ i ] ) != corpse ) {
         armour = delete( armour, i, 1 );
         i--;
      }
   }
   corpse->set_armours( armour );
   armour->set_worn_by( corpse );
   weapons -= ({ 0 });
   for ( i = 0; i < sizeof( weapons ); i++ ) {
      if ( environment( weapons[ i ] ) != corpse ) {
         weapons = delete( weapons, i, 1 );
         i--;
      }
   }
   corpse->set_holding( weapons );
   weapons->set_holder( corpse );
   return corpse;
} /* make_corpse() */

/**
 * This method deals with any cute messages you want to print when
 * something dies or, cute things you want to do (like not dieng
 * at all).  It handles the alignment shift due to the killing of
 * this npc as well.
 * @param thing the thing which killed us
 * @param death
 * @see make_corpse()
 */
mixed *death_helper(object thing, int death) {
   int shift, my_level, att_wimpy, att_level, xp, txp;
   string *messages;
   object *things, tmp;
   mixed *retval;

   retval = ({ });

   // added by ceres - 4 October 1997
   this_object()->remove_property(PASSED_OUT_PROP);
   this_object()->remove_hide_invis( "hiding" );

#ifdef USE_SURRENDER
   _surrender->refusers = ({ });
   _surrender->to = ({ });
#endif

   // Make sure only living things are left in the attacker list or
   // xp will be decreased for kills with e.g. spells, rituals or
   // special weapons.
   things = filter(this_object()->query_attacker_list(),
                   (:  living($1) && !$1->query_property("dead") :));

   if(environment()) {
     if (death) {
       messages = this_object()->query_property("death messages");
       if(!messages)
         messages = ({ "$K dealt the death blow to $D.\n",
                         "You killed $D.\n", "$D dies.\n" });
     } else {
       messages = this_object()->query_property( "surrender messages" );
       if (!messages)
         messages = ({ "$K accepted the surrender of $D.\n",
                         "You subdued $D.\n", "$D surrenders.\n" });
     }
     event( ({ environment() }) + deep_inventory( environment() ), "death",
            things, thing, (thing ? messages[0] : messages[2]), messages[1]);
   }
   if(sizeof(things)) {
     // Deal with alignment shifts.
     foreach(tmp in things) {
#if DEBUG == 2
       int x, y;
       x = tmp->query_al();
#endif

       shift = (int)this_object()->query_al() ;
       if (!death)
         shift -= (shift / 2);
       tmp->adjust_alignment(shift);

#if DEBUG == 2
       y = tmp->query_al();
       if(interactive(tmp))
         log_file("DEATH_ALIGN", "%s %s adjusted from %d to %d by %s [%d]\n",
                  ctime(time()), tmp->query_name(), x, y,
                  this_object()->query_name(), shift);
#endif

     }
     xp = this_object()->query_death_xp();
     if(xp > 0) {
       shift = 50 + xp / sizeof( things );
       debug_printf("%O Death XP: %O", this_object(), xp);

       // xp given to each player depends on their level & wimpy setting
       my_level = this_object()->query_level();
       foreach(tmp in things) {
         att_wimpy = tmp->query_wimpy();
         att_level = tmp->query_level();
         if(att_level <= my_level)
           txp = shift / 2;
         else
           txp = ((shift/2) * my_level) / att_level;

         if(att_wimpy > 0)      // High wimpy == lower xp.
           txp -= (txp * att_wimpy) / 100;

         tmp->adjust_xp(txp, 1);
       }
       return ({ ({ things }), shift/2 });
     }
   }
   return ({ });
} /* death_helper() */


/**
 * This does the actual death and co ordinates the death into a well
 * ordered feeding frenzy.  This method creates the actual corpse itself.
 * If the property "dead" is set on the object no corpse will be
 * created, or if the second_life() function called on the object
 * returns a non-zero value the corpse will not be created.
 * <p>
 * This method calls the second_life() function on the current object,
 * if this returns 1 it must handle all the the death code itself.
 * This is used in the player code to override the death code.
 * @param thing the thing which killed us
 * @param weapon the weapon (sword,claw,etc) object that attacked and killed us
 * @param attack the actual attack that killed us
 * @return the corpse, or 0 if no more action is to be taken
 * @index second_life
 * @see death_helper()
 * @see make_corpse()
 * @see alter_corpse()
 */
varargs object do_death(object thing, object weapon, string attack) {
  object corpse, ob;
  mixed xp_leftover, sec_life;

#if DEBUG == 2
  combat_debug("Do death called.");
#endif

  /* Now we're in do_death, so reset the callout flag */
  this_object()->reset_callingdeath();

  if ( this_object()->query_property( "dead" ) ) {
    if ( !userp( this_object() ) ) {
      if(file_name(environment()) != "/room/rubbish") {
        this_object()->move("/room/rubbish");
      }
      return 0;
    }
    else
      return 0;
  }

  this_object()->end_combat();

  this_object()->add_property("xp before death", this_object()->query_xp());

  xp_leftover = death_helper(thing, 1);
  catch(sec_life = this_object()->second_life());
  if(sec_life) {
    // hand out leftover xp since we're not doing a corpse.
    if(xp_leftover && sizeof(xp_leftover) == 2 && xp_leftover[1] > 0) {
      foreach(ob in xp_leftover[0]) {
        ob->adjust_xp(xp_leftover[1], 1);
      }
    }
    this_object()->stop_all_fight();
    if( objectp( sec_life ) ) {
      return sec_life;
    } else {
      return 0;
    }
  }
  this_object()->add_property( "dead", time() );
  catch(DEATH->someone_died( this_object() ) );
  catch(this_object()->effects_thru_death() );
  this_object()->reset_protectors();
  if ( environment() ) {
    corpse = this_object()->make_corpse();
    if (corpse) {
      corpse->move(environment());
      corpse->add_property("XP", xp_leftover, 300);
      debug_printf("Command: %O", this_object()->query_last_action());
      if (!(alter_corpse( corpse, weapon, attack ))) {
//        log_file("alter_corpse", "%s: corpse: %O, weapon: %O, command: %s.\n",
//                 ctime(time()), corpse, weapon, _command_name);
      }
    }
  }
  this_object()->move( "/room/rubbish" );
  this_object()->stop_all_fight();
  return corpse;
}
