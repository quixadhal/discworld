
/*
 * This is a pawned item receipt.
 * It can be turned back into its item if reclaimed at a pawn shop.
 * Based heavily on /obj/misc/al_receipt.c
 *
 * Vashti 22nd September 2002
 */

#include <am_time.h>
#include <armoury.h>
#include <money.h>
#include <move_failures.h>
#include <player.h>
#include <virtual.h>

#define TO this_object()
#define TP this_player()

inherit "/obj/misc/paper" ;

private int    _expired ;
private int    _expired_sold ;
private int    _expiry_time ;
private int    _item_value, _obplural ;
private int    _timestamp;
private mixed  _static_save;
private string _obname, _obpath, _obshort ;
private string _shop_path, _virt_name ;

object do_kiss();


/** @ignore yes */
void setup() {
    set_name( "receipt" );
    set_short( "pawned item receipt" ) ;
    add_adjective( ({ "pawned", "item" }) );
    set_long( "This is a small slip of paper, with a black border.\n" );
    set_main_plural( "pawned item receipts" ) ;
    add_plural( "receipts" );
    add_adjective( "pawned" ) ;
    set_value( 0 ) ;
    set_weight( 1 );
  
    _obname = "unknown object";
    _obpath = "***";
    _timestamp = time();
} /* setup */


/** @ignore yes */
void init() {
    add_command( "reclaim", "<direct:object:me'receipt'>" ) ;
} /* init */


/** @ignore yes */
void reset() {
    object item ;
    
    if ( ( ! _expired ) && time() > _expiry_time ) {
        _expired = 1 ;
        set_short( "expired pawned item receipt" ) ;
        add_adjective( "expired" ) ;
        
        if ( ! _expired_sold ) {
            _expired_sold = 1 ;
            item = do_kiss() ;
            if ( item ) {
                item->move( _shop_path->query_store_room() ) ;
            }
        }
    }
} /* reset */


/**
 * This sets the short of the pawned object.
 */
void set_obshort( string s ) {
  _obshort = s;
} /* set_obshort */


/**
 * This returns the short of the pawned object.
 * @return [string] The short of the pawned object.
 */
string query_obshort() {
  return _obshort;
} /* query_obshort */


/**
 * This sets the time at which this receipt will die.
 * @param time The time in seconds when this receipt will be killed.
 */
void set_expiry_time( int time ) {
    _expiry_time = time ;
} /* set_expiry_time */


/**
 * This returns the Unix time in seconds at which this receipt will expire.
 * @return [int] The time of death in seconds.
 */
int query_expiry_time() {
    return _expiry_time ;
} /* query_expiry_time */


/**
 * This sets the value of the item this receipt is based on.
 * @param value The value of the original item.
 */
void set_item_value( int value ) {
    _item_value = value ;
} /* set_item_value */


/**
 * This returns the value of the item this receipt was based on.
 * @return [int] the value of the original item.
 */
int query_item_value() {
    return _item_value ;
} /* query_item_value */


/**
 * This allows us to set the path of the shop that spawned us, so that this
 * receipt can only be redeemed at that shop.
 * @param shop - either a shop object or a string path.
 */
void set_shop_path( mixed shop ) {
    if ( objectp( shop ) ) {
        _shop_path = base_name( shop ) ;
        return ;
    }
    
    _shop_path = shop ;
    return ;
} /* set_shop_path */


/**
 * This returns the path to the shop that this receipt was spawned by.
 * @return [string] the path to the cloning shop.
 */
string query_shop_path() {
    return _shop_path ;
} /* query_shop_path */


/**
 * Sets the name of the object.  This will be used so the item recipt
 * can be identified.
 * @param name the object name
 */
void set_obname( string name ) {
    _obname = name;
} /* set_obname */


/**
 * Sets the path to the object.  The path of the object for the recipt to
 * work with.
 * @param path the object path
 */
void set_object( string path ) {
    _obpath = path;
} /* set_object */


/**
 * This method sets the virtual object name of the item.
 * @param name the virtual object name
 */
void set_virtobname( string name ) {
    _virt_name = name;
} /* set_virtobname */


/**
 * This method returns the name of the object.
 * @return the name of the object
 */
string query_obname() {
    return _obname;
} /* query_obname */


/**
 * This method returns the path of the object.
 * @return the path of the object
 */
string query_obpath() {
    return _obpath;
} /* query_obpath */


/**
 * This method returns the timestamp of the reciept.  This is the time the
 * receipt was created.
 * @return the timestamp
 */
int query_timestamp() {
    return _timestamp;
} /* query_timestamp */


/**
 * This method returns the virtual object path name of the item.
 * @return the virtual object name
 */
string query_virt_obname() {
    return _virt_name;
} /* query_virt_obname */


/**
 * This method returns the save information for the object.  THis is not
 * just the static save information, it is an array of two elements.  The
 * first is the static save information and the second is the dynamic
 * save information.
 * @return the save information
 */
mixed query_static_save() {
  return _static_save;
} /* query_static_save */


/**
 * This allows us to check if the object is a pawned item receipt.
 * @return 1 if the object is a pawned item receipt.
 */
int query_pawned_item_receipt() {
    return 1 ;
} /* query_pawned_item_receipt */


/**
 * This method sets the save information for the file.  It actually sets
 * both the static and dynamic information.  This assumes it is passed
 * an array of two elements, the first is the static save information and
 * the second is the dynamic save information.
 * @param args the data as defined above
 */
void set_static_save( mixed args ) {
  mixed p;

  _static_save = args;
  if (_obname != "unknown object") {
    return;
  }

  // Try to find the short in here somewhere
  p = args[0];
  while (mapp(p)) {
    if (!undefinedp(p["short"])) {
      _obname = p["short"];
      return;
    }
    p = p["::"];
  }
}


/**
 * This sets whether or not our object is actually a collective,
 * and so more than one object.
 */
void set_object_plural( object ob ) {
  if ( ob->query_collective() && ob->query_amount() > 1 ) {
    _obplural = 1;
  } else {
    _obplural = 0;
  }
} /* set_plural */


/**
 * This lets us check whether or not our object is a plural object.
 */
int query_object_plural() {
  return _obplural;
} /* query_plural */


/**
 * This returns a short which is accurate for both ordinary and
 * collective objects.
 */
string query_correct_short( object ob ) {
    if ( ! ob ) {
      return 0;
    }

    set_object_plural( ob );
    if ( _obplural ) {
      return pluralize( ob->query_short() );
    }
  
    return ob->query_short();
} /* query_correct_short */


/**
 * This method sets up the receipt for the specified object.
 * @param ob the object to setup the reciept for
 */
void setup_receipt( object ob ) {
   mixed* bits;

   bits = AUTO_LOAD_OB->fragile_auto_str_ob(ob);
   set_obname(ob->query_name());
   set_obshort( query_correct_short( ob ) );
   set_object(bits[1]);
   set_static_save(bits[2]);
} /* setup_receipt() */


/**
 * This takes a sentence and pluralises it appropriately.
 * @parma text The sentence to pluralise.
 * @param plural Whether or not the sentence should be in plural form.
 */
string pluralize_sentence( string text, int plural ) {
  if ( plural ) {
    return replace( text, ({
      "$o", "them",
      "$r", "they",
      "$ve", "ve"
    }) );
  }
  
  return replace( text, ({
    "$o", "it",
    "$r", "it",
    "$ve", "s"
  }) );
} /* pluralize_sentence */


/** @ignore yes */
string query_details() {
    if ( _expired ) {
        return pluralize_sentence(
          "This is a receipt for your pawned " + query_obshort() + ".\n"
          "Sadly, you have left it too long to collect $o and $r ha$ve "
          "expired, so the shop will put $o up for sale.  Better hurry to "
          + _shop_path->the_short() + " sharpish and try to buy $o back!",
          _obplural );
    }
    
    return pluralize_sentence(
      "This is a receipt for your pawned " + query_obshort() + ".\n"
      "You must reclaim $o from " + _shop_path->the_short() + " before "
      + AM_TIME_HANDLER->query_am_time( _expiry_time ) + " or $r will "
      "be sold.", _obplural );
}

/**
 * This sets up a message for us to get when we read the receipt.
 * Because we're using a function pointer, add_read_mess doesn't work.
 * @param lang The language to produce the message in.
 */
mixed *query_read_mess() {
  string lang;
  
  if ( _shop_path ) {
    lang = _shop_path->query_language();
  } else {
    lang = "general";
  }
  
  return ::query_read_mess()
    + ({ ({ (: query_details() :), "neat printing", lang, 0 }) });
} /* query_read_mess */


/** @ignore yes */
mapping query_dynamic_auto_load()
{
   return ([
     "::"         : ::query_dynamic_auto_load(),
     "obname"     : _obname,
     "obpath"     : _obpath,
     "obvalue"    : value,
     "virt"       : _virt_name,
     "timestamp"  : _timestamp,
     "static"     : _static_save,
     "been sold"  : _expired_sold,
     "expired"    : _expired,
     "expires on" : _expiry_time,
     "item value" : _item_value,
     "item short" : _obshort,
     "item plural": _obplural,
     "shop path"  : _shop_path
  ]);
} /* query_dynamic_auto_load() */


/** @ignore yes */
void init_dynamic_arg(mapping arg, object) {
   _obname = arg["obname"];
   if ( stringp( _obname ) ) {
       add_adjective( explode( lower_case( _obname ), " " ) );
   }
   _obpath = arg["obpath"];
   set_value(arg["obvalue"]);
   _virt_name = arg["virt"];
   _timestamp = arg["timestamp"];
   _static_save = arg["static"];
   _expired = arg["expired"] ;
   _expired_sold = arg["been sold"] ;
   _expiry_time = arg["expires on"] ;
   _item_value = arg["item value"] ;
   _obshort = arg["item short"];
   _obplural = arg["item plural"];
   _shop_path = arg["shop path"] ;
   ::init_dynamic_arg(arg["::"]);

   if ( _expired ) {
       set_short( "expired pawned item receipt" ) ;
       add_adjective( "expired" ) ;
       set_main_plural( "expired pawned item receipts" ) ;
       add_plural( "pawned item receipts" ) ;
   }
}


/** @ignore yes */
mixed *stats() {
  mixed *stuff;

  stuff = ::stats() + ({
      ({ "obname", _obname }),
      ({ "obpath", _obpath }),
      ({ "timestamp", _timestamp + " (" + ctime(_timestamp) + ")" }),
      ({ "been sold", _expired_sold }),
      ({ "expired", _expired }),
      ({ "expires on", _expiry_time }),
      ({ "item short", _obshort }),
      ({ "item value", _item_value }),
      ({ "item plural", _obplural }),
      ({ "shop path", _shop_path })
   });
  if (_virt_name)
    stuff += ({ ({ "virtname", _virt_name }) });
    
  return stuff;
} /* stats */


/** @ignore yes */
object do_kiss() {
    object thing;

    if (sizeof(_static_save) < 2) {
        if (this_player()->query_creator()) {
            write("This receipt has no auto-load information, possibly "
                  "because the object was broken when the player logged "
                  "out.  It will have to be replaced by hand.\n");
        } else {
            write("Sorry, but this object is too broken to repair "
                  "with a kiss.  You'll have to contact a creator.\n");
        }
        return 0;
    }

    // If we have virtual names, try and fiddle with this.
    if (_virt_name) {
       _virt_name = CLONER->other_file( _virt_name );
       if (file_size(_virt_name) <= 0) {
          // try and rewrite through the armoury
          _virt_name = ARMOURY->remap_file_path(_virt_name);
       }
       if (file_size(_virt_name) <= 0) {
           write("The receipt whispers: Sorry, I'm still broken.  "
                 "Try again some other time, or contact a creator.\n");
           return 0;
       }
    }

    if ( !catch( thing = (object)CLONER->clone( _obpath ) ) ) {
        if ( thing ) {
            thing->init_static_arg(_static_save[0]);
            thing->init_dynamic_arg(_static_save[1]);
            thing->add_property(VIRTUAL_NAME_PROP, _virt_name);
            move("/room/rubbish");
            return thing;
        } else {
            write("The receipt whispers: I'm going to need some "
                  "creator help.\n");
            thing->dest_me();
        }
    } else {
         write("The receipt whispers: Sorry, I'm still broken.  "
               "Try again some other time, or contact a creator.\n");
    }

    return 0;
} /* do_kiss() */


/**
 * This handles all the reclaiming of objects that have been pawned.
 */
int do_reclaim() {
    int cost, i ;
    object change, item ;
    string change_str, place ;
    
    /* Make sure we're a pawned item receipt */
    if ( ! query_pawned_item_receipt() ) {
        add_failed_mess( "$D is not a pawned item receipt.\n" ) ;
        return 0 ;
    }
    
    /* Make sure we're in the right place */
    if ( base_name( environment( TP ) ) != _shop_path ) {
        add_failed_mess(
            "This is not the right place for you to reclaim $D.  "
            "Try a pawn shop.\n" ) ;
        return 0 ;
    }

    /* Make sure that place is open for business */
    if ( ! environment( TP )->is_open( TP, 0 ) ) {
        return 0;
    }
    
    /* Make sure the receipt hasn't expired */
    if ( _expired ) {
        add_failed_mess( "Oh dear, your receipt has expired.  With luck, your "
            + query_obshort() + " will be available for sale in the shop.\n" ) ;
        return 0 ;
    }
    
    /* Do the money transfer thing */
    place = query_property( "place" ) ;
    if ( ! place || ( place == "" ) ) {
        place = "default" ;
    }
    
    cost = ( ( 100 + _shop_path->query_pawn_markup() ) * _item_value ) / 100 ;
    debug_printf( "cost = %d, _item_value = %d, markup = %d",
        cost, _item_value, _shop_path->query_pawn_markup() ) ;

    if ( TP->query_value_in( place ) < cost ) {
        add_failed_mess( "You don't have enough money to reclaim $D.  "
            "It would cost you "
            + MONEY_HAND->money_value_string( cost, place ) + "\n" ) ;
        return 0 ;
    }

    change = MONEY_HAND->pay_amount_from( cost, TP, place ) ;
    if ( change ) {
        change->move( TP ) ;
        change_str = change->short() ;
    }

    /* Make the receipt back into what it should be */
    item = do_kiss();
    if ( ! item ) {
        add_failed_mess(
          "Oh dear, your item doesn't seem to be here any more.  "
          "Please contact a liaison.\n" );
        return 0;
    }
    
    i = item->move( TP );
    if ( i != MOVE_OK ) {
        tell_object( TP, "You're too heavily burdened to accept "
          + item->the_short() + ", so it is placed on the floor for you.\n" );
        item->move( environment(TP) );
    }
    
    add_succeeded_mess( ({
        "$N $V " + item->the_short() + " for "
          + MONEY_HAND->money_value_string( cost, place )
          + ( change_str ? " and receive " + change_str + " in change" : "" )
          + ".\n",
        "$N $V $D and receives " + item->the_short() + " in exchange.\n"
    }) ) ;

    return 1 ;
} /* do_reclaim */
