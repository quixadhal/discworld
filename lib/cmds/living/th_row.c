/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: th_row.c,v 1.12 2003/01/30 06:21:28 pinkfish Exp $
 * $Log: th_row.c,v $
 * Revision 1.12  2003/01/30 06:21:28  pinkfish
 * Make it cost gp.
 *
 * Revision 1.11  2000/09/30 17:26:57  taffyd
 * Added item to thrown_at().
 *
 * Revision 1.10  2000/07/02 10:15:41  taffyd
 * Added event notification when thrown successfully. :)
 *
 * Revision 1.9  1999/10/23 11:00:57  taffyd
 * Recode to fix bugs that seem to have seeped in.
 *
 * Revision 1.8  1999/07/24 12:13:26  terano
 * Added the ability to throw at inanimate objects!
 *
 * Revision 1.7  1999/04/05 23:01:34  ceres
 * Modified to use perform_task
 *
 * Revision 1.6  1998/09/15 00:32:34  pinkfish
 * Really fix up the pk_check().
 *
 * Revision 1.5  1998/08/25 10:52:13  pinkfish
 * Make the messages use the_shrot();
 *
 * Revision 1.4  1998/06/23 03:41:26  sin
 * Terano fixed the pk_check
 *
 * Revision 1.3  1998/03/28 06:28:47  ceres
 * Added a repeat check since some people have been abusing this command.
 *
 * Revision 1.2  1998/02/08 22:15:57  ceres
 * moved to new skill tree
 *
 * Revision 1.1  1998/01/06 05:28:43  ceres
 * Initial revision
 * 
*/

//You can now throw items at inanimate objects that have the
//targetable property, the bonus for that object is counted
//as the value that this property is set to!
//IE: add_property( "targetable", 300 );
//When a player attempts to throw something, it calls thrown_at( object player, 
//int switch ) on the target, with 1 being a hit, and 0 being a miss.

#define CREATOR "Ceres"

#include <player.h>
#include <move_failures.h>
#include <tasks.h>

#define ASKILL "fighting.combat.range.thrown"
#define DSKILL "fighting.combat.dodging.range"
#define PSKILL "fighting.combat.parry.range"

#define COMMAND_GP_COST 5

inherit "/cmds/base";

int valid_target( object target, object thrower, object item );

int cmd( mixed *indirect_obs ) {
    object target, item;
    int dchance, damage;

    if ( sizeof( indirect_obs[ 0 ] ) > 1 ) {
        add_failed_mess( "You can only throw one item at a time!\n" );
        return 0;
    }

    item = indirect_obs[ 0 ][ 0 ];

    if ( sizeof( indirect_obs[ 1 ] ) > 1 ) {
        add_failed_mess( "You can only throw $I at one target at a time!\n",
            ({ item }) );
        return 0;
    }

    target = indirect_obs[ 1 ][ 0 ];

    if ( item->query_wielded() != this_player() ) {
        add_failed_mess( "You must be holding $I to throw it.\n",
            ({ item }) );
        return 0;
    }

    if ( this_player() == target ) {
        add_failed_mess( "You cannot throw $I at yourself!\n", ({ item }) );
        return 0;
    }

    if ( !living( target ) && !target->query_property( "targetable" ) ) {
        add_failed_mess( "Why do you want to throw $I at " + 
            target->the_short() + "?\n", ({ item }) );
        return 0;
    }

    if (this_player()->query_gp() < COMMAND_GP_COST) {
        add_failed_mess( "You need " + COMMAND_GP_COST + " points to "
            "throw " + item->the_short() + " at " + target->the_short() +
            ".\n");
        return 0;
    }

    if ( !valid_target( target, this_player(), item ) ) {
        add_failed_mess( "You decide against throwing $I at " + 
            target->the_short() + ".\n", ({ item }) );
        return 0;
    }

    if ( living( target ) ) {
        switch ( (string)target->query_combat_response() ) {
            
            case "dodge" :
                dchance = target->query_skill_bonus( DSKILL );
            break;
            
            default:
                dchance = target->query_skill_bonus( PSKILL );
        }
    } 
    else {
        dchance = target->query_property( "targetable" );
    }

    
    dchance += item->query_weight() / ( this_player()->query_str() / 2 );
    
    if( !this_player()->query_visible( target ) ) {
        dchance /= 2;
    }

    this_player()->adjust_gp(-COMMAND_GP_COST);
    
  
    // lets see if they hit or not.
    
    switch( TASKER->perform_task(this_player(), ASKILL, dchance, TM_FREE) ) {
        
        case AWARD:
            tell_object(this_player(), "%^YELLOW%^You feel that your "
                "skill in throwing weapons has increased.%^RESET%^\n");
        
        case SUCCEED:
            
            tell_object( this_player(), "You throw " + item->the_short() +
                " at " + target->the_short() + " and hit!\n" );
            tell_room( environment( this_player() ),
                this_player()->the_short() + " throws " + item->a_short() + 
                    " at " + target->the_short() + " and hits!\n", 
                    this_player() );
                    
            if ( living( target ) ) {
                
                damage = ( item->query_weight() / 4 ) + 
                    ( this_player()->query_str() * 2) + 
                    ( this_player()->query_skill_bonus( ASKILL ) / 3 );
     
                damage -= target->query_ac( "blunt", damage );
                
                target->attack_by( this_player() );
                target->adjust_hp( -damage, this_player() );
                this_player()->attack_ob( target );
                
                event( item, "thrown", this_player(), target );
                    this_player()->adjust_time_left(-(ROUND_TIME*2));
            }                
            
            target->thrown_at( this_player(), 1, item );
      
            if ( item->move( target ) != MOVE_OK ) {
                item->move( environment( target ) );
            }
            
        break;
      
        default:
            tell_object( this_player(), "You throw " + item->the_short() + 
                " at " + target->the_short() + " but miss.\n" );
            
            tell_room( environment( this_player() ), 
                this_player()->the_short() + " throws " + item->a_short() +
                " at " + target->the_short() + " but misses.\n", this_player() );
                
            item->move( environment( target ) );
            
            if( !interactive( target ) && living( target ) ) {
                this_player()->attack_ob( target );
                target->attack_by( this_player() );
            }
            
            target->thrown_at( this_player(), 0 ); //They missed, but lets note that anyway            
        break;
    }   
    

    return 1;
} /* cmd() */


int valid_target( object target, object thrower, object item ) {
    mixed owner;
    
    if ( target->query_property( "dead" ) || target->query_auto_loading() ) {
        return 0;
    }
    
    if ( userp( target ) && !interactive( target ) ) {
        return 0;
    }
    
    if ( pk_check( thrower, target ) )
        return 0;
        
    if ( ( owner = target->query_owner() ) && pk_check( thrower, owner ) ) {
        return 0;
    }
    
    return 1;
} /* valid_target() */

mixed *query_patterns() {
    return ({ "<indirect:object:me'weapon'> at <indirect:object'target'>", 
        (: cmd($1) :) });
} /* query_patterns() */
