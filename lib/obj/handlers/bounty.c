/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: bounty.c,v 1.1 1998/01/06 05:03:33 ceres Exp $
 * $Log: bounty.c,v $
 * Revision 1.1  1998/01/06 05:03:33  ceres
 * Initial revision
 * 
*/
/*
 * Coded by:
 *     Bill, Turrican and Deutha.
*/

#include <money.h>

#define SAVE_FILE "/save/bounty"
#define EXPIRY 60 * 60 * 8 * 60
#define BASE 40000
#define TAXES 60
#define LOGIN "/secure/login"

class bounty {
   string short;
   string *victims;
   int value;
   int active;
   int time;
}

/*
 * These are existing bounties and bounties fulfilled but uncollected.
*/
mapping bounty, collect;

void create() {
   bounty  = ([ ]);
   collect = ([ ]);
   seteuid( "Room" );
   if ( file_size( SAVE_FILE +".o" ) > 0 )
      unguarded( (: restore_object, SAVE_FILE :) );
   call_out( "tidy_bounties", 120 );
} /* create() */

void save_file() { unguarded( (: save_object, SAVE_FILE :) ); }

void delete_bounty( string word ) {
   map_delete( bounty, word );
   save_file();
} /* delete_bounty() */

void tidy_bounties() {
   int now = time();
   string word;
   object thing;
   class bounty record;
/*
 * A bounty may only be fulfilled during a certain time before it expires.
*/
   foreach ( word, record in bounty ) {
      if ( LOGIN->test_creator( word ) ) {
         log_file( "BOUNTY", "%s: bounty removed from %s (creator)\n",
               ctime( now ), word );
         map_delete( bounty, word );
         continue;
      }
      if (record && record->time + EXPIRY < now ) {
         if (record->active )
            log_file( "BOUNTY", "%s: bounty on %s of %d expired\n",
                  ctime( now ), word, record->value );
         else
            log_file( "BOUNTY", "%s: inactive bounty on %s cancelled\n",
                  ctime( now ), word );
         thing = find_player( word );
         if ( objectp( thing ) )
            tell_object( thing, "You feel that there is no longer "
                  "a price on your head.\n" );
         map_delete( bounty, word );
         continue;
      }
   }
/*
 * A bounty may only be collected during a certain time before it expires.
*/
   foreach ( word, record in collect )
      if ( !LOGIN->test_user( word ) ) {
         map_delete( collect, word );
         continue;
      }
      if (record && record->time + EXPIRY < now ) {
         log_file( "BOUNTY", "%s: bounty of %d owed to %s expired\n",
               ctime( now ), record->value, word );
         thing = find_player( word );
         if ( objectp( thing ) )
            tell_object( thing, "You have the funniest feeling that "
                  "you just let a lot of money get away.\n" );
         map_delete( collect, word );
      }
   save_file();
} /* tidy_bounties() */

void clear_bounties() {
   if ( !master()->high_programmer( geteuid( this_player( 1 ) ) ) )
      return 0;
   bounty  = ([ ]);
   collect = ([ ]);
   save_file();
} /* clear_bounties() */

void dest_me() { destruct( this_object() ); }

void register_kill( object victim, object *attackers ) {
   int now, reward;
   string killed, killer, *killers;
   object thing;
   class bounty record;
/*
 * Don't let someone get others into trouble or collect
 * their own bounties by committing suicide.
*/
   if ( member_array( victim, attackers ) != -1 )
      return;
   if ( victim->query_creator() || victim->query_property( "no bounties" ) )
      return;
/*
 * Ignore it all if it happens in a pub.
*/
   if ( !environment( victim ) )
      return;
   if ( environment( victim )->query_property( "bar brawling" ) )
      return;
   now = time();
/*
 * This will be okay so long as modifications
 * are via short() or pretty_short().
*/
   killed = (string)victim->query_short();
/*
 * If there is no bounty on the victim, set up bounties on the killers.
*/
   if ( undefinedp( bounty[ lower_case( killed ) ] ) ) {
/*
 * Temporarily, since I'm off on holiday.
 *    Deutha.
*/
      return;
      foreach ( thing in attackers ) {
         if ( !objectp( thing ) )
            continue;
         if ( thing->query_creator() )
            continue;
         if ( !userp( thing ) && !thing->query_property( "unique" ) )
            continue;
/*
 * Inhumations shouldn't lead to bounties.
*/
         if ( ( (object)thing->query_property( "inhumer" ) == victim ) &&
               ( (object)victim->query_property( "inhumee" ) == thing ) )
            continue;
         killer = (string)thing->query_short();
         if ( undefinedp( bounty[ lower_case( killer ) ] ) ) {
            record = new( class bounty );
            bounty[ lower_case( killer ) ] = record;
            record->short = killer;
            record->victims = ({ killed });
         } else {
            record = (class bounty)bounty[ lower_case( killer ) ];
            if ( strsrch( lower_case( killed ),
                  lower_case( implode( record->victims, ", " ) ) ) == -1 )
               record->victims += ({ killed });
         }
         record->time = now;
         if ( record->active ) {
            record->value = sizeof( record->victims ) * BASE;
            if ( sizeof( record->victims ) > 1 )
               call_out( "inform_bounty", 30, thing );
         }
      }
      save_file();
      return;
   }
/*
 * There is a bounty.  Is it active?
*/
   if ( !( ( (class bounty)bounty[ lower_case( killed ) ] )->active ) )
      return;
/*
 * Work out who the legitimate killers are.
*/
   killers = ({ });
   foreach ( thing in attackers ) {
      if ( !objectp( thing ) )
         continue;
      if ( thing->query_creator() )
         continue;
      if ( !userp( thing ) && !thing->query_property( "unique" ) &&
            !thing->query_property( "bounty hunter" ) )
         continue;
      killer = (string)thing->query_short();
      killers += ({ killer });
      call_out( "well_done", 30, thing, (string)victim->query_gender() );
   }
/*
 * If there are valid killers, distribute the bounties appropriately
 * and update the records.
*/
   if ( sizeof( killers ) ) {
      reward = ( (class bounty)bounty[ lower_case( killed ) ] )->value /
            sizeof( killers );
      foreach ( killer in killers ) {
/*
 * Killers with bounties themselves don't get paid.  This should also
 * limit some forms of possible abuse.
*/
         if ( !undefinedp( bounty[ lower_case( killer ) ] ) )
            continue;
         if ( undefinedp( collect[ lower_case( killer ) ] ) ) {
            record = new( class bounty );
            collect[ lower_case( killer ) ] = record;
            record->short = killer;
            record->victims = ({ killed });
         } else {
            record = (class bounty)collect[ lower_case( killer ) ];
            record->victims += ({ killed });
         }
         record->time = now;
         record->value += reward;
      }
      log_file( "BOUNTY", "%s: bounty on %s fulfilled by %s\n",
            ctime( now ), lower_case( killed ),
            query_multiple_short( map_array( killers,
            (: lower_case( $1 ) :) ) ) );
      map_delete( bounty, lower_case( killed ) );
   }
   save_file();
} /* register_kill() */

void inform_bounty( object thing ) {
   if ( !thing )
      return;
   if ( !userp( thing ) )
      return;
   tell_object( thing, "You feel the price on your head rise.\n" );
} /* inform_bounty() */

void well_done( object thing, int gender ) {
   if ( !thing )
      return;
   if ( !userp( thing ) )
      return;
   tell_object( thing, "You feel good about killing a wanted "+
         ({ "creature", "man", "woman" })[gender] +".\n" );
} /* well_done() */

/*
 * This is interfaced by a bounty office to allow people to report murders.
*/
string report_murder( string killer ) {
   string place;
   class bounty record;
   if ( undefinedp( bounty[ lower_case( killer ) ] ) )
      return "According to the records, \""+ killer +
            "\" has never killed anyone.";
   killer = lower_case( killer );
   record = (class bounty)bounty[ killer ];
   if ( record->active ) {
      place = (string)environment( this_player() )->query_property( "place" );
      if ( !place || ( place == "" ) )
         place = "default";
      return "There is already a bounty of "+
            (string)MONEY_HAND->money_value_string( record->value, place ) +
            " offered for the death of "+ record->short +".";
   }
   record->active = 1;
   record->value = sizeof( record->victims ) * BASE;
   call_out( "issue_warrant", 30, killer );
   save_file();
   if ( strsrch( lower_case( (string)this_player()->query_short() ),
         lower_case( implode( record->victims, ", " ) ) ) == -1 )
      return "Ah, a concerned citizen reporting a murder.  I'll issue "
            "a bounty notice for "+ record->short +" right away.";
   return "I'm sorry to hear that "+ record->short +" caused you "
         "harm.  I'll issue a bounty notice right away.";
} /* report_murder() */

void issue_warrant( string killer ) {
   object thing;
   thing = find_player( killer );
   if ( !thing )
      return;
   if ( !userp( thing ) )
      return;
   thing->remove_hide_invis( "hide" );
   tell_room( environment( thing ), "There is a small unspectacular flash "
         "of light and a small orange imp appears.  It promptly informs "+
         (string)thing->the_short() +" that a bounty has been placed on "+
         (string)thing->query_possessive() +" head.  The imp bows deeply, "
         " smiles evilly, and vanishes in an equally unspectacular flash "
         "of light.\n", thing );
   tell_object( thing, "There is a small unspectacular flash of light and "
         "a small orange imp appears.  It promptly informs you that a bounty "
         "has been placed on your head.  The imp bows deeply, smiles evilly, "
         "and vanishes in an equally unspectacular flash of light.\n" );
} /* issue_warrant() */

/*
 * This is interfaced by a bounty office to allow people to collect bounties.
*/
string collect_bounty() {
   int money;
   string collector, place;
   class bounty record;
   collector = lower_case( (string)this_player()->query_short() );
   if ( undefinedp( collect[ collector ] ) )
      return "According to the records, you've done nothing "
            "to deserve a bounty.";
   record = (class bounty)collect[ collector ];
   money = ( record->value * ( 100 - TAXES ) ) / 100;
   place = (string)environment( this_player() )->query_property( "place" );
   if ( !place || ( place == "" ) )
      place = "default";
   this_player()->adjust_money( (mixed *)MONEY_HAND->create_money_array( money,
         place ) );
   this_player()->set_title( "bounty", "Bounty Hunter" );
   map_delete( collect, collector );
   save_file();
   return "Ah yes, let's see.  Monies owed to you total "+
         (string)MONEY_HAND->money_value_string( record->value, place ) +
         ", minus death tax, inheritance tax, income tax, pension fund "
         "contribution, union dues, VAT, Save the Womble donations and "
         "administrative charges, which leaves you with the grand sum of "+
         (string)MONEY_HAND->money_value_string( money, place ) +
         ".  Nice doing business with you.";
} /* collect_bounty() */

int query_bounty( string word ) { return !undefinedp( bounty[ word ] ); }

int query_bounty_value( string word ) {
   if ( undefinedp( bounty[ word ] ) )
      return 0;
   return ( (class bounty)bounty[ word ] )->value;
} /* query_bounty_value() */

/*
 * Generate a list of bounties; creators see times as well.
*/
string query_all_bounties() {
   int cre;
   string list, word, place;
   class bounty record;
   cre = (int)this_player( 1 )->query_creator();
   if ( !sizeof( bounty ) )
      return "There are no bounties currently offered.\n";
   place = (string)environment( this_player() )->query_property( "place" );
   if ( !place || ( place == "" ) )
      place = "default";
   list = "";
   foreach ( word, record in bounty )
      if ( record->active ) {
         list += sprintf( " -   %-15s for %s\n", record->short,
               (string)MONEY_HAND->money_value_string( record->value,
               place ) );
         if ( cre )
            list += sprintf( "               %-#60s\n               %s\n",
                  implode( record->victims, "\n" ),
                  ctime( record->time + EXPIRY ) );
      } else
         if ( cre )
            list += sprintf( " -   %-15s (inactive)\n"
                  "               %-#60s\n               %s\n",
                  record->short, implode( record->victims, "\n" ),
                  ctime( record->time + EXPIRY ) );
   if ( list == "" )
      return "There are no bounties currently offered.\n";
   return "The following bounters are currently offered:\n"+ list;
} /* query_all_bounties() */

/*
 * Only creators should get this function called for them.
*/
string query_all_collectors() {
   string list, word, place;
   class bounty record;
   if ( !sizeof( collect ) )
      return "There are no bounties awaiting collection.\n";
   place = (string)environment( this_player() )->query_property( "place" );
   if ( !place || ( place == "" ) )
      place = "default";
   list = "";
   foreach ( word, record in collect )
         list += sprintf( " -   %-15s owed %s\n               %s\n",
               record->short,
               (string)MONEY_HAND->money_value_string( record->value, place ),
               ctime( record->time + EXPIRY ) );
   return "The following bounties are awaiting collection:\n"+ list;
} /* query_all_collectors() */

string query_bounty_info( string word ) {
   string place;
   class bounty record;
   place = (string)environment( this_player() )->query_property( "place" );
   if ( !place || ( place == "" ) )
      place = "default";
   if ( undefinedp( bounty[ word ] ) )
      return 0;
   record = (class bounty)bounty[ word ];
   if ( !record->active )
      return 0;
   return "A bounty of "+
         (string)MONEY_HAND->money_value_string( record->value, place ) +
         " is offered for the death of "+ record->short +" for the murder of "+
         query_multiple_short( record->victims ) +".\n";
} /* query_bounty_info() */
