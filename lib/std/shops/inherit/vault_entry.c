/*
 * $Locker:  $
 * $Id: vault_entry.c,v 1.2 2002/11/07 16:19:42 carmine Exp $
 */
/**
 * Standard vault inheritable object. There are two inheritables (this one and
 * /std/vault.c) which are provided to make it easy to setup your own
 * vaults.
 * This object should be inherited by your vault entrance area. It is used to
 * define most of the customisation for your vault.
 *
 * You must use set_vault_name and set_vault_entrance. You must also define
 * a vault proprietor NPC and use set_proprietor() to let the system know
 * which NPC it is. You should also check for this proprietors location and
 * existance every reset. You may also want to provide some protection to
 * prevent the NPC being killed too frequently.
 *
 * @author Ceres
 * @see /std/vault_entrance.c
 */
#include <money.h>
#include <vault.h>

#define PTS proprietor->the_short()
#define TIME_LIMIT 500

#define SPAM_GUARD (60 * 3)


void add_exit(string name, string dest, string type);
void modify_exit(string name, mixed* stuff);
mixed query_property(string name);
void add_property(string name, mixed value);
void remove_property(string name);

string location, vault_name, vault_room;
int visit_cost, open_cost;
object proprietor;

/* Setup functions which should be called by the child of this object to
   set the properties of a specific vault */

/**
 * This function is used to set the location used for calculation of which
 * currency the vault proprietor will accept.
 * @param location The name of the location.
 */
void set_location(string str) { location = str; }

/**
 * This is used to set the cost to open an account (in monetary units)
 * @param cost The cost of a new account
 */
void set_open_cost(int i) { open_cost = i; }

/**
 * This is used to set the cost of a visit to the vault (in monetary units)
 * @param cost The cost of a vist
 */
void set_visit_cost(int i) { visit_cost = i; }

/**
 * This is the object that is the proprietor of the vault. This should be an
 * NPC.
 * @param proprietor The proprietor NPC object.
 */
void set_proprietor(object ob) { proprietor = ob; }

/**
 * Used to set the name of the vault, as used for the save file. This should
 * be a unique name for this vault.
 * @param name The name of the vault.
 */
void set_vault_name(string str) { vault_name = str; }

/**
 * Used to set the exit from this vault entrance room into the vault itself.
 * The parameters are just like those of add_exit.
 * @param direction The direction of the exit.
 * @param dest The destination object. Usually the filename of the destination.
 * @param type An exit type as in add_exit.
 * @return void
 * @see add_exit
 */
void set_vault_exit(string direc, mixed dest, string type) {
  vault_room = dest;
  add_exit(direc, dest, type);
  modify_exit( direc, ({ "function", "check_customer" }) );
  modify_exit( direc, ({ "closed", 1}));
}

/**
 * @ignore yes
 *
 * Internal function used to setup a new account.
 */
int apply_for_account() {
  string owner;
  object container;

  if(!proprietor || environment(proprietor) != this_object())
    return notify_fail("The vault is closed since the proprietor is "
                       "missing.\n");
  
  /* We don't want one... */
  this_player()->add_succeeded_mess( this_object(), "", ({ }) );

  write("You ask for an account.\n");
  say((string)this_player()->one_short() +" asks for an account.\n");

  owner = (string)this_player()->query_name();

  // I have no idea why this spam guard stuff is in here, but I've left it
  // just in case :)
  if ( master()->file_exists( VAULT_SAVE +vault_name+ "/"+ owner +".o" ) ||
       query_property( owner ) ) 
  {
    if ( this_player()->query_property( "vault warned" ) ) {
      proprietor->do_command( "glare "+ this_player()->query_name() );
      proprietor->do_command( "say I just said, you already have an "
         "account." );
      return notify_fail("");
    }
   
    proprietor->do_command( "say You already have an account!" );
    this_player()->add_property( "vault warned", 1, SPAM_GUARD );
    return 1;
  }

  if ( this_player()->query_value_in( location) < open_cost ) {
    if ( this_player()->query_property( "vault warned" ) ) {
      proprietor->do_command( "glare "+ this_player()->query_name() );
      proprietor->do_command( "say I just said, you don't have enough money!" );
      return notify_fail("");
    }
   
    proprietor->do_command("say You do not have the "+
                           (string)MONEY_HAND->money_value_string(open_cost,
                                                                  location) +
                           " that it costs to open an account." );
    this_player()->add_property( "vault warned", 1, SPAM_GUARD );
    return 1;
  }

  proprietor->do_command("say Very well, that'll be "+
                         (string)MONEY_HAND->money_value_string( open_cost,
                                                                 location) +
                         ", please." );
  this_player()->pay_money( (mixed *)MONEY_HAND->create_money_array( open_cost,
                                                                     location),
                            location);
  write( "You pay "+PTS+".\n" );
  say( (string)this_player()->the_short() +" pays "+proprietor->the_short()+
       ".\n" );

  proprietor->do_command("say As a new account holder, "
                         "your first use of the facility will be free.  "
                         "Thereafter, it will cost "+
                         (string)MONEY_HAND->money_value_string( visit_cost,
                                                                 location) +
                         " each time.");
  add_property( owner, 1 );

  // Force it to make a save file
  container = vault_room->init_container(this_player());
  container->dest_me();
  
  return 1;
} /* apply_for_account() */

/**
 * @ignore yes
 */
void init() {
  add_command( "apply", "for [an] account", (: apply_for_account :) );
} /* init() */

/**
 * @ignore yes
 *
 * Internal function to check if this player is a customer
 */
int check_customer( string verb, object thing, string special ) {
  string owner;
  object vroom;
  
  if(!proprietor || environment(proprietor) != this_object())
    return notify_fail("The vault is closed since the proprietor is "
                       "missing.\n");
  
  // Added by Oaf, 15 Oct 2000, to stop NPCs wandering into vaults.
  if( !interactive(thing) )
    return 0;

  owner = (string)thing->query_name();
  
  if ( !master()->file_exists( VAULT_SAVE +vault_name+ "/" + owner +".o" ) &&
       !query_property( owner ) ) 
  {
    tell_object( thing, PTS+
                 " intercepts you.\n" );
    tell_room( this_object(), PTS+" intercepts "+
               (string)thing->the_short() +".\n",
               thing );

    if ( thing->query_property( "vault warned" ) ) {
      proprietor->do_command( "glare "+ thing->query_name() );
      proprietor->do_command( "say I just said, you don't have an account "
         "so I won't let you in!" );
      return notify_fail("");
    }

    proprietor->do_command("say You do not have an account!");
    if( interactive(thing) )
      proprietor->do_command("say For "+
                             (string)MONEY_HAND->money_value_string(open_cost,
                                                                    location)+
                             ", you could apply for an account.\n" );
    thing->add_property( "vault warned", 1, SPAM_GUARD );
    return notify_fail( "" );
  }
  
  if ( !( vroom = find_object( vault_room ) ) ) {
    vault_room->load_me();
    vroom = find_object( vault_room );
  }

  if( sizeof( filter_array( all_inventory( vroom ), (: userp( $1 ) :) ) ) ) {
    tell_object( thing, PTS+" intercepts you.\n" );
    tell_room( this_object(), PTS+" intercepts "+ (string)thing->the_short() 
       +".\n", thing );

    if( thing->query_property( "vault warned" ) ) {
      proprietor->do_command( "glare "+ thing->query_name() );
      proprietor->do_command( "say I just told you, someone is using the "
         "facility at the moment!" );
      return notify_fail("");
    }

    proprietor->do_command("say Someone is using the facility at the moment!");
    thing->add_property( "vault warned", 1, SPAM_GUARD );
    return notify_fail( "" );
  }
    
  if( query_property( owner ) ) {
    remove_property(owner);
    tell_object(thing, PTS+" waves you through.\n" );
    tell_room( this_object(), PTS+" waves "+(string)thing->the_short() +
      " through.\n", thing );
    call_out( "check_idler", TIME_LIMIT, thing );
    return 1;
  }
  
  if ( (int)thing->query_value_in( location ) < visit_cost ) {
    tell_object( thing, PTS+" intercepts you.\n" );
    tell_room( this_object(), PTS+" intercepts "+ (string)thing->the_short() 
      +".\n", thing );
    
    if ( thing->query_property( "vault warned" ) ) {
      proprietor->do_command( "glare "+ thing->query_name() );
      proprietor->do_command( "say I just said, you don't have enough "
         "money to use the facility!" );
      return notify_fail("");
    }
   
    proprietor->do_command("say It costs "+
                           (string)MONEY_HAND->money_value_string( visit_cost,
                                                                   location ) +
                           " to use the facility!" );
    thing->add_property( "vault warned", 1, SPAM_GUARD );
    return notify_fail( "" );
  }
  
  thing->pay_money((mixed *)MONEY_HAND->create_money_array(visit_cost,
                                                           location),
                   location);
  tell_object( thing, "You pay "+PTS+" who waves you through.\n" );
  tell_room( this_object(), (string)thing->the_short() +
             " pays "+PTS+" who waves "+ (string)thing->query_objective() +
             " through.\n", thing );

  remove_call_out("check_idler");
  call_out("check_idler", TIME_LIMIT, thing);
  return 1;
} /* check_customer() */


void check_idler(object who) {
  if(who && base_name(environment(who)) == vault_room) { 
    tell_object(who, "You've been in here too long so " + PTS + " comes and "
              "drags you out.\n");
    who->move_with_look(this_object(),
                      "$N is dragged out of the vault by the hair.",
                      "$N is dragged out of the vault by the hair.");
  }
}

/**
 * @ignore yes
 */
void player_quitting(object who, object env) {
}
