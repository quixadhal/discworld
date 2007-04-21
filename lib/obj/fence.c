/* A basic fence for thieves and others to sell stolen items to. */

#include <armoury.h>
#include <money.h>
#include <move_failures.h>
#include <thief.h>

inherit "/obj/monster";

#define MAX 500
#define SKILL "other.trading.valueing"
#define PAY_RATES ({ 200, 95, \
                     400, 90, \
                     800, 85, \
                     1600, 80, \
                     3200, 75, \
                     6400, 70, \
                     12800, 65, \
                     60 })

class hist_offer {
  object ob;
  int old_offer;
  int value;
}

class offer {
  object who;
  int amount;
  int offer_time;
  object *objects;
}

class offer this_offer;
object cont;
string *move_data = ({ });
string fence_type;
mapping old_offers = ([ ]);

void whisper( object, string );
void give_back();

object query_cont() {
  return cont;
}

void create() {
  do_setup++;
  ::create();
  do_setup--;
  if( !do_setup )
    this_object()->setup();

  // Give him his bag.
  cont = clone_object( "/std/container" );
  cont->move(this_object());
  cont->reset_drop();
  cont->reset_get();
  cont->set_name( "sack" );
  cont->set_short( "sack" );
  cont->set_long( "This is a large hessian sack.\n" );
  cont->add_property( "nosteal", 1 );

  set_wimpy( 20 );
  basic_setup( "human", "thief", 500 );

  add_respond_to_with( ({ "@say", ({ "yes", "Yes", "ok", "okay" }) }),
    "#do_yes" );
  add_respond_to_with( ({ "@nod", ({ "you" }) }), "#do_yes" );
  add_respond_to_with( ({ "@whisper", ({ "yes", "Yes", "ok", "okay" }) }),
    "#do_yes" );
  add_respond_to_with( ({ "@say", ({ "no", "No" }) }), "#do_no" );
  add_respond_to_with( ({ "@whisper", ({ "no", "No" }) }), "#do_no" );
  add_respond_to_with( ({ "@shake", ({ "you" }) }), "#do_no" );

}

void init() {
  ::init();
  this_player()->add_command( "fence", this_object(),
    "<indirect:object:me> to <direct:object>" );
}

void dest_me() {
  if( cont )
    cont->dest_me();
  ::dest_me();
}

void reset() {
  object ob;

  // Don't dest anything if we're in the middle of dealing with someone.
  if( this_offer )
    return;
  
  foreach( ob in all_inventory( cont ) )
    ob->dest_me();
}

void check_cont() {
  if ( !cont )
    cont = clone_object( "/std/container" );
}

void set_fence_type( string str ) {
  fence_type = str;
}

int query_cost( object thing, object buyer ) {
   return (int)thing->query_value_at( this_object() );
}

string cost_string( object thing, string place, object buyer ) {
  return (string)MONEY_HAND->money_value_string( query_cost( thing, buyer ),
                                                 place );
}

// Determine item type
string query_item_type( object ob ) {
  if( member_array( "weapons", ob->query_plurals()) != -1 )
    return "weapons";
  if( member_array( "jewellery", ob->query_plurals()) != -1 ||
      ob->query_property( "shop type" ) == "jewellers" )
    return "jewellery";
  if( member_array( "armours", ob->query_plurals()) != -1 )
    return "armour";
  if( member_array( "clothes", ob->query_plurals()) != -1 )
    return "armour";
  if( member_array( "gems", ob->query_plurals()) != -1 )
    return "gems";

  return "";
}

// Judge the value of an item
int judge_value( object ob, string type ) {
  int value, variance, skill;
  value = query_cost( ob, this_object() );
  if( type != "" )
    skill = query_skill_bonus( SKILL + "." + type );
  else
    skill = query_skill_bonus( SKILL );

  if( !skill )
    skill = 1;
  
  variance = value / sqrt( skill );

  value += random( variance ) - random( variance );
  
  return value;
}

// Work out where we are (for money strings)  Make the default AM.
string calc_place() {
  string place;
  
  place = environment()->query_property( "place" );
  if( !place || ( place == "" ) )
    place = "Ankh-Morpork";
  return place;
}

// Return an amount of money as a nice string
string cost_str( int amt ) {
  return MONEY_HAND->money_string( MONEY_HAND->create_money_array( amt,
                                   calc_place() ) );
}

// From /std/shop.
int scaled_value( int n ) {
  int *fish, i, tot;

  fish = PAY_RATES;
  if( n < fish[0] ) {
    return n;
  }
  else {
    tot = fish[0];
    n -= fish[0];
  }

  i = 0;
  while( n && ( ( i + 1 ) < sizeof( fish ) ) ) {
    if( n >= fish[i]) {
      tot += fish[i+1] * fish[i] / 100;
      n -= fish[i];
    } else {
      tot += fish[i+1] * n / 100;
      n = 0;
    }
    i += 2;
  }

  if( n ) {
    tot += ( n * fish[sizeof( fish ) - 1] ) / 100;
  }

  return tot;
} /* scaled_value() */

int do_fence( object *in_dir, string direct, string indirect, mixed args,
  string format ) {
  int offer, their_skill, returned, burdened, light;
  string type, skill, zone;
  object ob, *offered, *not_speciality, customer, thing;

  if( !this_player()->query_creator() &&
      !this_player()->query_visible( this_object() ) ) {
    this_object()->do_command( "puzzle" );
    this_object()->do_command( "wonder if someone is trying to be funny" );
    return 0;
  }
  light = environment( this_object() )->query_light();

  if ( ( light < 20 ) || ( light > 100 ) ) {
    if ( light > 100 ) {
      this_object()->do_command( "say I can't see anything in the glare!" );
    } else {
      this_object()->do_command( "say I can't see anything in the gloom!" );
    }
    return 0;
  }

  if( this_offer &&
      this_offer->who &&
      environment( this_offer->who ) == environment( this_object() ) ) {
    if( this_offer->offer_time > time() - 30 ) {
      if( this_offer->who != this_player() )
        whisper( this_player(), "Sorry, I'm already helping " +
          ( this_offer->who )->the_short() + "." );
      else
        whisper( this_player(), "I'm still waiting for a response from you!" );
      this_player()->add_succeeded_mess( this_object(), "", offered );
      return 1;
    } else {
      // Lets return the items coz they didn't answer in time.
      whisper( this_offer->who, "I guess you don't want my offer then." );
      customer = this_offer->who;
      burdened = 0;
      returned = 0;
    
      foreach( thing in this_offer->objects ) {
        // Sometimes the items turn into 0s.
        if( !thing ) {
          whisper( customer, "Seems there is a hole in my sack!" );
          continue;
        }
        if( thing->move( customer ) != MOVE_OK ) {
          burdened = 1;
          thing->move( environment( this_object() ) );
          whisper( customer, "I'll just put the " + thing->short() +
            " down here since you are too burdened to accept it." );
        } else {
          returned = 1;
        }
      }
    
      if( burdened && returned ) {
      tell_object( customer, this_object()->a_short() + " returns the other "
        "items to you.\n" );
      } else {
        if( returned )
          tell_object( customer, this_object()->a_short() + " returns the "
            "items to you.\n" );
      }
      tell_room( environment(), this_object()->a_short() + " returns the "
        "items to " +  customer->a_short() + ".\n", ({ customer }) );
      this_offer = 0;
    }
  }

  not_speciality = ({ });

  if( sizeof( this_object()->query_move_zones() ) ) {
    foreach( zone in this_object()->query_move_zones() ) {
      move_data += ({zone});
      this_object()->remove_move_zone(zone);
    }
    this_object()->add_move_zone( 0 );
  }
  
  this_offer = new( class offer,
                    objects    : ({ }),
                    who        : this_player(),
                    offer_time : time()
                  );
  
  in_dir = filter_array( in_dir, (: !$1->query_keep() :) );

  if( sizeof( in_dir ) < 1 ) {
    whisper( this_player(), "You have nothing to fence that you're not overly "
      "fond of, it seems." );
    this_player()->add_succeeded_mess( this_object(), "", offered );
    this_offer = 0;
    return 1;
  }

  if( sizeof( in_dir ) > 10 ) {
    whisper( this_player(), "Sorry, I can't handle that many items at "
      "once." );
    this_player()->add_succeeded_mess( this_object(), "", offered );
    this_offer = 0;
    return 1;
  }    

  tell_room( environment( this_object() ), this_object()->query_short() +
            " studies " + query_multiple_short( in_dir ) + ".\n" );
  
  foreach( ob in in_dir ) {
    // Player doesn't have item.
    if( environment( ob ) != this_offer->who ) {
      whisper( this_player(), "Do you think I'm blind?  I can see that you "
        "don't have " + strip_colours( ob->the_short()) + "." );
      continue;
    }
    
    if( base_name(ob) == "/obj/money" ) {
        whisper( this_player(), "I don't accept coins, I'm afraid. The market "
          "fluctuates far too much for it to be worthwhile.");
        continue;
    }

    // Can't move to sack.
    if( ob->move( cont ) != MOVE_OK ) {
      whisper( this_player(), "If I can't take an item you can't fence it "
        "so you can't fence " + strip_colours( ob->the_short()) + "." );
      continue;
    }
    
    // Item have do_not_sell on it.
    if( ob->do_not_sell() ) {
      whisper( this_player(), "You can't sell the " +
        strip_colours( ob->the_short() ) + "." );
      if( ob->move( this_player() ) != MOVE_OK ) {
        ob->move( environment( this_object() ) );
        whisper( this_player(), "So I'll just put it down here." );
      }
      else {
        whisper( this_player(), "So you can have it back." );
      }
      continue;
    }
    
    if( ob->query_property( "fenced" ) ) {
      do_command( "' Hang on a sec... this looks awful familiar." );
      do_command( "glare " + this_offer->who->query_name() );
      do_command( "' Just what are you trying to pull here?!" );
      do_command( "' I think I'll hang onto these little trinkets "
                  "until we can establish their correct ownership..." );
      // Annulling the offer effectively confiscates their crap.
      this_offer = 0;
      this_player()->add_failed_mess( this_object(), this_object()->
        the_short() + " confiscates $I.\n", in_dir );
      return 0;
    }

    type = query_item_type( ob );
    offer = judge_value( ob, type );
    
    // Item not worth anything or smaller than a pence.
    if( !offer ||
        offer < 4 ) {
      whisper( this_player(), ob->the_short() + " isn't worth anything." );
      if( ob->move( this_player() ) != MOVE_OK ) {
        ob->move( environment( this_object() ) );
        whisper( this_player(), "So I'll just put it down here." );
      }
      else {
        whisper( this_player(), "So you can have it back." );
      }
      continue;
    }
    
    // Are we a fence for this type of object?
    if( type != fence_type ) {
      offer -= ( offer / 10 );
      not_speciality += ({ ob });
    }
    
    // Now do the profit (just like a shop)
    offer = (int)scaled_value( offer );
    // Remember our previous offers.
    if( !undefinedp( old_offers[ob] ) ) {
      if( !undefinedp( old_offers[ob][ob->query_value()] ) ) {
        // Found it
        offer = old_offers[ob][ob->query_value()];
      } else {
        old_offers[ob][ob->query_value()] = offer;
      }
    } else {
      old_offers[ob] = ([ ob->query_value() : offer ]);
    }
    
    // Now see if we can fleece them
    if( type != "" )
      skill = SKILL + "." + type;
    else
      skill = SKILL;

    their_skill = this_player()->query_skill_bonus( skill );
    if( !their_skill)
      their_skill = 2;

    // Reduce the offer dependant on their valueing skill
    if( sqrt( their_skill ) < 3 )
      offer -= offer / 3;
    else {
      offer -= ( offer ) / sqrt( their_skill );
    }
    
    // Item not worth anything after all the adjustments.
    if( !offer ||
        offer < 4 ) {
      whisper( this_player(), ob->the_short() + " isn't worth anything." );
      if( ob->move( this_player() ) != MOVE_OK ) {
        ob->move( environment( this_object() ) );
        whisper( this_player(), "So I'll just put it down here." );
      }
      else {
        whisper( this_player(), "So you can have it back." );
      }
      continue;
    }

    this_offer->amount += offer;
    this_offer->objects += ({ ob });
  }

  if( sizeof( not_speciality ) == 1 )
    whisper( this_player(), "Well, " +
      strip_colours( query_multiple_short( not_speciality ) ) + " isn't really "
      "my speciality." );
  else
    if( sizeof( not_speciality ) > 1 )
      whisper( this_player(), "Well, " +
        strip_colours( query_multiple_short( not_speciality ) ) + " aren't "
        "really my speciality." );
  
  this_player()->add_succeeded_mess( this_object(), "", offered );

  if( !sizeof( this_offer->objects ) ) {
    whisper( this_player(), "Well, that was a waste of time." );
    this_offer = 0;
    return 1;
  }

  // Tell em how much and wait for their response
  whisper( this_player(), "I'll give you " + cost_str( this_offer->amount ) +
          " for " + strip_colours( query_multiple_short( this_offer->
          objects ) ) + ", what do you think?" );
  return 1;
}

// Now they've given a response either stop or take their goods
// and give them money.
void do_yes( object person ) {
  object money, *selling, ob;
  mixed *m_array;
  string zone;

  if( !this_offer || person != this_offer->who )
    return;

  whisper( person, "You've got a deal." );
  selling = ({ });
  foreach( ob in this_offer->objects ) {
    ob->add_property( "fenced", (string)person->query_name() );
    selling += ({ ob });
  }
  
  m_array = (mixed *)MONEY_HAND->create_money_array( this_offer->amount,
                                                     calc_place() );
  money = clone_object( MONEY_OBJECT );
  money->set_money_array( m_array );

  if( (int)money->move( person ) != MOVE_OK) {
    money->move( environment( this_object() ) );
    whisper( person, "You're too heavily burdened to accept all "
      "that money, so I'll just put it on the floor." );
  } else {
    tell_object( person, this_object()->a_short() + " slips you " +
      cost_str( this_offer->amount ) + ".\n" );
    tell_room( environment(), this_object()->a_short() + " slips some coins "
      "to " + person->a_short() + ".\n", ({person}) );
  }
  this_offer = 0;

  if( sizeof( move_data ) > 0 ) {
    foreach( zone in move_data ) {
      this_object()->add_move_zone( zone );
    }
    this_object()->remove_move_zone( 0 );
    move_data = ({ });
  }
}

void do_no( object player ) {
  string zone;
  object customer, thing;
  int returned, burdened;
    
  if( !this_offer || player != this_offer->who )
    return;

  whisper( player, "Ok, have it your own way then." );
  customer = this_offer->who;
  burdened = 0;
  returned = 0;

  foreach( thing in this_offer->objects ) {
    if( !thing ) {
      whisper( customer, "Seems thare is a hole in my sack!" );
      continue;
    }
    if( thing->move( customer ) != MOVE_OK ) {
      burdened = 1;
      thing->move( environment( this_object() ) );
      whisper( customer, "I'll just put the " + thing->short() +
        " down here since you are too burdened to accept it." );
    } else {
      returned = 1;
    }
  }

  if( burdened && returned ) {
  tell_object( customer, this_object()->a_short() + " returns the other items "
    "to you.\n" );
  } else {
    if( returned )
      tell_object( customer, this_object()->a_short() + " returns the items to "
        "you.\n" );
  }
  tell_room( environment(), this_object()->a_short() + " returns the "
    "items to " +  customer->a_short() + ".\n", ({ customer }) );
  this_offer = 0;

  if( sizeof( move_data ) > 0 ) {
    foreach( zone in move_data ) {
      this_object()->add_move_zone( zone );
    }
    this_object()->remove_move_zone( 0 );
    move_data = ({ });
  }
}

void whisper( object ob, string message ) {
  do_command( "whisper " + message + " to " + ob->query_name() );
}

int busy() {
  if( this_offer )
    return 1;
  return 0;
}

void event_exit( object ob, string message, object to ) {
  object customer, thing;
  int returned, burdened;

  if( this_offer &&
      ob == this_offer->who ) {
    whisper( ob, "Since you're leaving, I'll give your stuff back." );
    customer = this_offer->who;
    burdened = 0;
    returned = 0;
  
    foreach( thing in this_offer->objects ) {
      if( !thing ) {
        whisper( customer, "Seems thare is a hole in my sack!" );
        continue;
      }
      if( thing->move( customer ) != MOVE_OK ) {
        burdened = 1;
        thing->move( environment( this_object() ) );
        whisper( customer, "I'll just put the " + thing->short() +
          " down here since you are too burdened to accept it." );
      } else {
        returned = 1;
      }
    }
  
    if( burdened && returned ) {
    tell_object( customer, this_object()->a_short() + " returns the other "
      "items to you.\n" );
    } else {
      if( returned )
        tell_object( customer, this_object()->a_short() + " returns the items "
          "to you.\n" );
    }
    tell_room( environment(), this_object()->a_short() + " returns the "
      "items to " +  customer->a_short() + ".\n", ({ customer }) );
    this_offer = 0;
  }
}

int attack_by( object ob ) {
  int burdened, returned;
  object customer, thing;
  
  if( this_offer &&
      this_offer->who &&
      environment( this_offer->who ) == environment( this_object() ) &&
      this_offer->offer_time > time() - 60 ) {
    do_command( "say Hey! I'm trying to do business here!" );
    whisper( this_offer->who, "Hey, you'd better take these." );
    customer = this_offer->who;
    burdened = 0;
    returned = 0;
  
    foreach( thing in this_offer->objects ) {
      if( !thing ) {
        whisper( customer, "Seems thare is a hole in my sack!" );
        continue;
      }
      if( thing->move( customer ) != MOVE_OK ) {
        burdened = 1;
        thing->move( environment( this_object() ) );
        whisper( customer, "I'll just put the " + thing->short() +
          " down here since you are too burdened to accept it." );
      } else {
        returned = 1;
      }
    }
  
    if( burdened && returned ) {
    tell_object( customer, this_object()->a_short() + " returns the other "
      "items to you.\n" );
    } else {
      if( returned )
        tell_object( customer, this_object()->a_short() + " returns the items "
                     "to you.\n" );
    }
    tell_room( environment(), this_object()->a_short() + " returns the "
               "items to " +  customer->a_short() + ".\n", ({ customer }) );
    this_offer = 0;
  }
  return ::attack_by(ob);
}

/*void event_person_say( object thing, string start, string mess, string lang) {
  if ( !( thing->query_current_language() == "morporkian" ) ) {
    lang = "morporkian";
  }
  ::event_person_say( thing, start, mess, lang );
}*/

// Called when time's up, being attacked, said no or is leaving.
void give_back() {
  int burdened, returned;
  object customer, thing;

  customer = this_offer->who;
  burdened = 0;
  returned = 0;

  foreach( thing in this_offer->objects ) {
    if( !thing ) {
      whisper( customer, "Seems thare is a hole in my sack!" );
      continue;
    }
    if( thing->move( customer ) != MOVE_OK ) {
      burdened = 1;
      thing->move( environment( this_object() ) );
      whisper( customer, "I'll just put the " + thing->short() + " down here "
        "since you are too burdened to accept it." );
    } else {
      returned = 1;
    }
  }

  if( burdened && returned ) {
  tell_object( customer, this_object()->a_short() + " returns the other items "
    "to you.\n" );
  } else {
    if( returned )
      tell_object( customer, this_object()->a_short() + " returns the items to "
        "you.\n" );
  }
  tell_room( environment(), this_object()->a_short() + " returns the "
    "items to " +  customer->a_short() + ".\n", ({ customer }) );
  this_offer = 0;
}

string query_current_offer() {
  string str;
  
  str = "\nCustomer: " + ( this_offer->who )->short()  + "\n";
  str += "Amount: " + this_offer->amount + "\n";
  str += "Offer time: " + this_offer->offer_time + "\n";
  str += "Objects: " + query_multiple_short( this_offer->objects );
  return str;
}
