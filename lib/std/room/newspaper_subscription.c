/*
 *
 *$Id*
 *
 */

/**
 * Newspaper subscription room
 * This room handles subscriptions for one specific newspaper.
 * @see /obj/handlers/newspaper
 * @see help:subscription_room
 * @author Wirble
 * @started August 2001
 */

#define USE_LOGIN 1

/** The newspaper-include. */
#include <room/newspaper.h>

/** The include file for moving. */
#include <move_failures.h>

/** The mailer include-file. */
#include <mail.h>

/** The money include-file. */
#include <money.h>

/** The player-handler-include. */
#include <player_handler.h>

/** The number of issues newspaper boxes are purchased with. */
#define ISSUES_PER_BOX 12

inherit "/std/room/basic_room";

/* variables */
private mapping _subscribers;
private string _region;
private string _paper;
private string _parcel;
private nosave string _savefile;


/** @ignore */
void create() {
   ::create();

   add_help_file( "newspaper_subscription" );
   if( !_subscribers ) {
      _subscribers = ([ ]);
   }
} /* void create() */

/**
 * This queries the prices for subscriptions and boxes.
 * @return price per issue to parcel subscribers
 */
int query_price_per_issue_subscriber() {
   int deposit_cost = 400;
   return deposit_cost + ( NEWSPAPER_HANDLER->query_paper_cost( _paper ) * 2 );
} /* query_price_per_issue_subscriber() */

/**
 * This method queries the cost per issue for boxes.
 * @return cost per issue for a box
 */
int query_price_per_issue_box() {
   return NEWSPAPER_HANDLER->query_paper_cost( _paper );
} /* query_price_per_issue_box() */

/**
 * This returns the base cost of the newspaper box.
 * @return the base cost of the newspaper box.
 */
int query_price_newspaper_box() {
   return load_object( NEWSPAPER_SUBSCRIPTION_OB )->query_value() +
           ISSUES_PER_BOX * NEWSPAPER_HANDLER->query_paper_cost( _paper );
} /* query_price_newspaper_box() */

/**
 * This returns the base cost of the international newspaper box.
 * @return the base cost of the international newspaper box.
 */
int query_price_international_newspaper_box() {
   return (load_object( NEWSPAPER_SUBSCRIPTION_OB )->query_value() +
           ISSUES_PER_BOX * NEWSPAPER_HANDLER->query_paper_cost( _paper )) * 2;
} /* query_price_international_newspaper_box() */




/** @ignore */
private void load_me() {
   if( !_savefile ||
       ( file_size( _savefile ) <= 0 && file_size( _savefile + ".o" ) <= 0 ) ) {
      return;
   }
   unguarded( (: restore_object( _savefile + ".o", 1 ) :) );
} /* private void load_me() */


/** @ignore */
private void save_me() {
   if( !_savefile ) {
      return;
   }
   unguarded( (: save_object( _savefile, 1 ) :) );
} /* private void save_me() */

/** @ignore */
void dest_me() {
   save_me();
   ::dest_me();
} /* void dest_me() */

/** @ignore */
private void send_termination_mail( string name, string mess ) {
   object player;

   if( ( player = find_living( name ) ) &&
       environment( player ) ) {
      tell_room( environment( player ),
                 "An imp zips past " + player->the_short() + " screaming \"You "
                 "have mail!\" in its high-pitched voice.\n" );
   }

   AUTO_MAILER->auto_mail( name,
                           _paper,
                           "Your subscription for " + _paper,
                           0,
                           "Greetings.\n"
                           "\n"
                           "Your subscription for " + _paper +
                           " has ended.\n"
                           "\n" +
                           mess + "\n" );
} /* private void send_termination_mail() */

/** @ignore */
private int withdraw_from_account( string name, int price ) {
   string bank;
   int amount = 0;
   int amount_in_account;

   foreach( bank in BANK_HANDLER->query_accounts( name ) ) {
      if( BANK_HANDLER->query_account( name, bank ) >= price ) {
         BANK_HANDLER->adjust_account( name, bank, -price );
         return 1;
      } else {
         amount += BANK_HANDLER->query_account( name, bank );
      }
   }
   if( amount >= price ) {
      foreach( bank in BANK_HANDLER->query_accounts( name ) ) {
         amount_in_account = BANK_HANDLER->query_account( name, bank );
         if( amount_in_account < amount ) {
            amount -= amount_in_account;
            BANK_HANDLER->adjust_account( name, bank, -amount_in_account );
         } else {
            BANK_HANDLER->adjust_account( name, bank, -amount );
            return 1;
         }
      }
   }
   send_termination_mail( name, "You failed to pay for an issue of " + _paper +
                                "." );
   return 0;
} /* private int withdraw_from_account() */


/** @ignore */
private void deliver_issue( string name, int issue ) {
   int* editions;
   object player;
   object ob;

   editions = PLAYER_HANDLER->test_property( name, "Paper " + _paper );
   if( !editions ) {
      editions = ({ });
   }
   if( member_array( issue, editions ) == -1 ) {
      editions += ({ issue });
#ifdef USE_LOGIN
      "/secure/login"->special_add_property( name, "Paper " + _paper,
                                             editions );
#endif
#ifdef USE_PLAYER_HANDLER
      PLAYER_HANDLER->special_add_property( name, "Paper " + _paper,
                                             editions );
#endif 
      NEWSPAPER_HANDLER->add_edition_paper_sold( _paper, issue,
                         NEWSPAPER_HANDLER->query_paper_cost( _paper ) );
   } else {
      NEWSPAPER_HANDLER->add_edition_revenue( _paper, issue,
                         NEWSPAPER_HANDLER->query_paper_cost( _paper ) );
   }
   ob = clone_object( NEWSPAPER_OB );
   ob->set_paper( _paper );
   ob->set_edition( issue );
   if( _parcel ) {
      MAIL_PARCEL_HANDLER->deposit( name, _paper, _parcel, ({ ob }) );
   }
   if( ( player = find_living( name ) ) &&
       environment( player ) ) {
      tell_room( environment( player ),
                 "An imp rushes up to " + player->the_short() + " and squeaks "
                 "something that very much sounds like \"You have mail!\" "
                 "before zipping away again.\n" );
   }
   AUTO_MAILER->auto_mail( name,
                           _paper,
                           "A new issue of " + _paper,
                           0,
                           "Greetings.\n"
                           "\n"
                           "Issue " + issue + " of " + _paper + " has been "
                           "deposited for you at the " + _parcel +
                           " post-office.\n"
                           "\n"
                           "Enjoy.\n" );
} /* private void deliver_issue() */


/**
 * This function has to be called whenever a new issue of the paper is
 * released.
 */
void event_publish_paper(string paper) {
   string name;
   int issues;
   int price;
   int last_issue = NEWSPAPER_HANDLER->query_last_edition_num( _paper );

   if (paper != _paper) {
      return ;
   }

   price = query_price_per_issue_subscriber();

   if( _subscribers ) {
      foreach( name in keys( _subscribers ) ) {
         if( !PLAYER_HANDLER->test_user( name ) ) {
            map_delete( _subscribers, name );
         } else {
            issues = _subscribers[ name ];
            if( issues == -1 ) {
               if( withdraw_from_account( name, price ) ) {
                  deliver_issue( name, last_issue );
               } else {
                  map_delete( _subscribers, name );
               }
            } else if( issues >= 1 ) {
               deliver_issue( name, last_issue );
               _subscribers[ name ]--;
               if( _subscribers[ name ] == 0 ) {
                  map_delete( _subscribers, name );
                  send_termination_mail( name,
                                         "All issues of " + _paper + " that "
                                         "you have paid for have been "
                                         "delivered." );
               }
            }
         }
      }
   }
} /* void publish_issue() */


/**
 * This sets the region the newspaper is in. i.e. Ankh-Morpork or Klatch.  This
 * will be used for determining the correct currency.
 * @param new_region The region to use.
 */
void set_money_place( string new_region ) {
   _region = new_region;
} /* void set_money_place() */

/**
 * This method returns the money place for the newsper.
 * @return the money place for the newspaper
 */
string query_money_place() {
   return _region;
} /* query_money_place() */

/**
 * This sets the paper this particular room is for.
 * @param new_paper The paper's official name with correct capitalization.
 */
protected void set_paper( string new_paper ) {
   if (_paper) {
      NEWSPAPER_HANDLER->remove_newspaper_inform(_paper, file_name(this_object()));
   }
   _paper = new_paper;
   NEWSPAPER_HANDLER->add_newspaper_inform(_paper, file_name(this_object()));
} /* protected void set_paper() */


/**
 * This sets the savefile for the room.
 * @param filename The full filename to use
 */
protected void set_savefile( string filename ) {
   _savefile = filename;
   if (_savefile) {
      load_me();
   }
} /* protected void set_savefile() */


/**
 * This sets the parcel region for the room and thereby determines to which post
 * office the newspapers will be delivered, i.e. Ankh-Morpork.
 * @param parcel_region The region to use.
 */
protected void set_parcel( string parcel_region ) {
   _parcel = parcel_region;
} /* protected void set_parcel() */

/** @ignore */
int do_subscribe( int numbers ) {
   string name;
   int price;
   string mess;

   if( !numbers || !this_player() || numbers < -1 ) {
      return 0;
   }
   name = this_player()->query_name();
   price = query_price_per_issue_subscriber();
   if( !_subscribers ) {
      _subscribers = ([ ]);
   }
   if( !undefinedp( _subscribers[ name ] ) && _subscribers[ name ] == -1 ) {
      add_failed_mess( "You already have a subscription for " + _paper +
                       ", so there is no need to re-subscribe.\n" );
      return 0;
   }
   if( numbers == -1 ) {
      _subscribers[ name ] = -1;
      add_succeeded_mess( ({"You subscribe to " + _paper + ".\n"
                            "Your bank-account(s) will be billed whenever a "
                            "new issue is released, until you unsubscribe.\n",
                            "$N fill$s out a subscription form.\n"
                          }) );
      save_me();
      return 1;
   }
   price *= numbers;
   if( this_player()->query_value_in( _region ) < price ) {
      add_failed_mess( "You cannot afford to pay for this many issues of " +
                       _paper + ".  It would cost you " +
                       MONEY_HAND->money_value_string( price, _region ) +
                       ".\n" );
      return 0;
   }
   if( undefinedp( _subscribers[ name ] ) ) {
      _subscribers[ name ] = numbers;
      if( numbers == 1 ) {
         mess = "one issue ";
      } else {
         mess = numbers + " issues ";
      }
   } else {
      _subscribers[ name ] += numbers;
      if( numbers == 1 ) {
         mess = "an additional issue ";
      } else {
         mess = "an additional " + numbers + " issues ";
      }
   }
   this_player()->pay_money( MONEY_HAND->create_money_array( price, _region ),
                            _region );

   add_succeeded_mess( ({"You subscribe to " + mess + "of " + _paper +
                         " and pay " + MONEY_HAND->money_value_string( price,
                         _region ) + ".\n",
                         "$N fill$s out a subscription form.\n"
                       }) );
   save_me();
   return 1;
} /* int do_subscribe() */


/** @ignore */
int do_unsubscribe() {
   if( undefinedp( _subscribers[ this_player()->query_name() ] ) ) {
      add_failed_mess( "You don't have a subscription for " + _paper +
                       ", so you cannot unsubscribe.\n" );
      return 0;
   }
   map_delete( _subscribers, this_player()->query_name() );
   add_succeeded_mess( ({"You successfully unscubscribe from " + _paper + ".\n",
                         "$N fill$s out a subscription form.\n"
                       }) );
   save_me();
   return 1;
} /* int do_unsubscribe() */


/** @ignore */
int do_buy( int international ) {
   object box;
   int price;


   if( international == 1 ) {
      price = query_price_international_newspaper_box();
   } else {
      price = query_price_newspaper_box();
   }
   if( !this_player() ) {
      return 0;
   }

   if( this_player()->query_value_in( _region ) < price ) {
      add_failed_mess( "You are too poor to afford this.\n" );
      return 0;
   } else {
      this_player()->pay_money( MONEY_HAND->create_money_array(
                                  price, _region ), _region );
      add_succeeded_mess( "$N purchase$s a newspaper box.\n" );
      box = clone_object( NEWSPAPER_SUBSCRIPTION_OB );
      box->set_paper( _paper );
      box->set_international( international );
      box->set_issues_left( ISSUES_PER_BOX );
      box->set_last_issue( NEWSPAPER_HANDLER->query_last_edition_num( _paper ));
      if( box->move( this_player() ) != MOVE_OK ) {
         box->move( environment( this_player() ) );
         write( "You cannot carry the box.  It has been put on the "
                      "floor.\n" );
      }
      return 1;
   }

} /* int do_buy() */


/** @ignore */
int do_check() {
   int i;

   if( !this_player() ) {
      return 0;
   }
   if( undefinedp( _subscribers[ this_player()->query_name() ] ) ) {
      add_succeeded_mess( ({"You have no subscription for " + _paper + ".\n",
                            "$N check$s something on a list.\n"
                          }) );
      return 1;
   }
   i = _subscribers[ this_player()->query_name() ];
   if( i == -1 ) {
      add_succeeded_mess( ({"Your subscription for " + _paper +
                            " will continue until you unsubscribe again.\n",
                            "$N check$s something on a list.\n"
                          }) );
      return 1;
   } else if( i == 1 ) {
      add_succeeded_mess( ({"Your subscription for " + _paper +
                            " will expire after the next issue.\n",
                            "$N check$s something on a list.\n"
                          }) );
      return 1;
   } else {
     add_succeeded_mess( ({ "Your subscription for " + _paper + " covers " + i +
                            " more issues.\n",
                            "$N check$s something on a list.\n"
                         }) );
      return 1;
   }
} /* int do_check() */


/** @ignore */
int do_list( string subscription_type ) {
   string* subscriber_list;
   string name;

   subscriber_list = ({ });
   if( !this_player() || !subscription_type ) {
      return 0;
   }
   if( subscription_type == "all" ) {
      foreach( name in keys( _subscribers ) ) {
         subscriber_list += ({ name });
      }
   } else if( subscription_type == "pre-paid" ) {
      foreach( name in keys( _subscribers ) ) {
         if( _subscribers[ name ] > 0 ) {
            subscriber_list += ({ name });
         }
      }
   } else if( subscription_type == "unlimited" ) {
      foreach( name in keys( _subscribers ) ) {
         if( _subscribers[ name ] == -1 ) {
            subscriber_list += ({ name });
         }
      }
   }
   if( sizeof( subscriber_list ) == 0 ) {
      subscriber_list += ({ "none" });
   }
   add_succeeded_mess( ({"The following people have a this kind of "
                         "subscription for " + _paper + ":\n" +
                         query_multiple_short( subscriber_list ) +
                         "\n",
                         "$N check$s some lists.\n"
                       }) );
   return 1;
} /* int do_list() */

/** @ignore */
void init() {
   string _office;
   object office = 0;

   add_command( "subscribe",
                "[to] <number> [issue|issues] of " + _paper,
                (: do_subscribe( $4[0] ) :) );
   add_command( "subscribe",
                "[to] " + _paper,
                (: do_subscribe( -1 ) :) );
   add_command( "unsubscribe",
                "[from] " + _paper,
                (: do_unsubscribe() :) );
   add_command( "buy",
                "[a] newspaper box",
                (: do_buy( 0 ) :) );
   add_command( "buy",
                "[an] international newspaper box",
                (: do_buy( 1 ) :) );
   add_command( "check",
                "subscription",
                (: do_check() :) );
   /* This if for the listing of the subscriptions */
   if( _office = NEWSPAPER_HANDLER->query_paper_office( _paper ) ) {
      office = load_object( _office );
   }
   if( this_player()->query_creator() ||
       ( office && office->is_editor(this_player()->query_name()) ) ) {
      add_command( "list",
                   "{all|unlimited|pre-paid} subscriptions",
                   (: do_list( $4[0] ) :) );
   }
   /* This is for error-handling */
   if( !this_player()->query_creator() ) {
      return;
   }
   if( !_parcel ) {
      write("%^WHITE%^%^BOLD%^Warning: This room's _parcel - variable "
                   "isn't set.%^RESET%^\n" );
   }
   if( !_region ) {
      write("%^WHITE%^%^BOLD%^Warning: This room's _region - variable "
                   "isn't set.%^RESET%^\n" );
   }
   if( !_paper ) {
      write("%^WHITE%^%^BOLD%^Warning: This room's _paper - variable "
                   "isn't set.%^RESET%^\n" );
   }
      if( !_savefile ) {
      tell_object( this_player(),
                   "%^WHITE%^%^BOLD%^Warning: This room's _savefile - variable "
                   "isn't set.%^RESET%^\n" );
   }


   ::init();
} /* void init() */

