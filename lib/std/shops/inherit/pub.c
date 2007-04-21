/**
 * This is the inheritable bit of the pub.  The actual file you should
 * inherit is /std/shops/pub_shop.c
 * <p>
 * Based on the original "pub.c" code, this version allows you to buy an
 * actual drink (inna glass) instead of just forcefeeding you with whatever
 * you purchase.  You can also create food and serve it on plates, in boxes,
 * or whatever.
 * <p>
 * The items on sale are either cloned from the armoury, cloned from a file
 * or cloned in the "create_item()" code in the pub code itself.  Containers
 * for these items are done in the same way, using the "create_container()"
 * function.  This is the same as the "create_object()" system in
 * "item_shop.c".  You can also buy items for other people (or groups of
 * other people) in the pub.
 * <p>
 * There are several standard glasses and plates available for drinks and
 * food in the "pub_shop.h" header file, if you don't want to create the
 * glasses and plates yourself.  These plates use the "/obj/misc/plate.c"
 * inheritable file.
 * <p>
 * A "counter" will automatically be placed in the pub.  If any item cannot
 * be moved into the person who ordered it (i.e they are carrying too much )
 * it will be placed on the counter.  Empty glasses and so on will be
 * cleared from the counter after 3 seconds.  To ensure that this happens,
 * make sure that the container object you use has the property "pub item"
 * added to it.  Items will only be cleared if they are empty.
 * @author Lemming
 * @started 23/10/1999
 * @see /std/shops/pub_shop.c
 * @see /include/shops/pub_shop.h
 * @see /include/volumes.h
 */

#include <shops/pub_shop.h>
#include <armoury.h>
#include <money.h>
#include <move_failures.h>
#include <map.h>

inherit "/std/shops/inherit/open_shop";
inherit "/std/shops/inherit/shop_event";

//
// Predefinitions needed from room.c
//
void add_hidden_object(object ob);

private mapping _menu_items;
private mapping _menu_aliases;
private string _menu_header;
private string _language;
private int _display_header;
private string *_menu_subheadings;
private int _display_subheadings;
private int no_standard_alias;
private object _menu_object;
private object _counter;

void add_menu_alias( mixed alias, string actual );
string *calc_standard_aliases( string *aliases );
int do_buy( object *obs, string dir, string indir, mixed *args );
float query_discount( object ob );
object make_counter();

/** @ignore */
void create() {
   shop_event::create();
   _menu_items = ([ ]);
   _menu_aliases = ([ ]);

   _menu_header = "The menu reads:";
   _display_header = 1;

   _menu_subheadings = ({ "Appetisers", "Main Courses", "Desserts",
                         "Alcoholic Beverages", "Soft Drinks",
                         "Hot Drinks" });
   _display_subheadings = 1;

   if( base_name( this_object() ) + ".c" != __FILE__ ) {
      _menu_object = clone_object( PUB_MENU_FILE );
      _menu_object->set_pub( this_object() );
      add_hidden_object( _menu_object );

      _counter = make_counter();
   }
} /* create() */

/** @ignore */
void init() {
   this_player()->add_command( "buy", this_object(), "<string>" );
   this_player()->add_command( "buy", this_object(),
                     "<string> for <indirect:living:here>" );
   this_player()->add_command( "order", this_object(), "<string>",
                     (: do_buy( $1, $2, $3, $4 ) :) );
   this_player()->add_command( "order", this_object(),
                     "<string> for <indirect:living:here>",
                     (: do_buy( $1, $2, $3, $4 ) :) );
} /* init() */

/**
 * This is the method used to add items to the menu.  Only the first four
 * parameters are required, the rest are optional.
 * <p>
 * The "type" parameter is used to determine which section of the menu the
 * item should reside in, and should be selected from those in the
 * "pub_shop.h" header file.
 * <p>
 * The "item" parameter is used to generate the actual product on sale.  This
 * value can be:
 * <ul>
 * <li> a name, which is passed to the "create_item()" function in the room code
 * <li> a filename, which is cloned, or
 * <li> an armoury identifier, which is passed to the armoury handler.
 * </ul>
 * <p>
 * The "container" parameter is the same as the "item" parameter, except that
 * it refers to the container the item comes in and is passed to
 * "create_container()" instead of "create_item()".  You can also use the
 * standard containers listed in the "pub_shop.h" header file.  It is
 * optional, and setting it to 0 will cause it to be ignored.
 * <p>
 * The "volume" parameter is optional and is passed directly to
 * "set_amount()" on the object cloned from "item".  It can be used to alter
 * the volume of a liquid cloned from a file, so the file itself does not
 * have to be changed.  It is optional, and setting it to 0 will cause it to
 * be ignored.  If this is set to 0 then the item will fill up the
 * container.  Standard volume definitions can be found in the "volumes.h"
 * header file.
 * <p>
 * The last parameter, "intox", is optional and is used only by NPCs to
 * determine how alcoholic an item is.  This should be between 0 and 10,
 * with 0 being non-alcoholic and 10 being something like Suicider.  It 
 * defaults to 0.  Note:  This has no effect on the actual alcohol content
 * of the item.
 * <p>
 * The different volumes of standard containers can be taken from
 * /include/volumes.h which has defines for all the standard volumes.
 * @param name the description to be displayed on the menu
 * @param type the type of item
 * @param cost the cost of the item
 * @param item the name, filename or armoury identifier for the item itself
 * @param container the name, filename or armoury identifier for the
 * container (optional)
 * @param volume the volume that the item should be set to (optional)
 * @param intox the intoxification value, on a scale of 0 to 10 (optional)
 * @example
 * // Add a main course called "Big meat pie", cloned from the file
 * // "/obj/food/meatpie.food"
 * add_menu_item( "Big meat pie", PUB_MAINCOURSE, 1000,
 *    "/obj/food/meatpie.food" );
 * @example
 * // Same as above, but let's put the pie on a plate
 * add_menu_item( "Big meat pie", PUB_MAINCOURSE, 1000,
 *    "/obj/food/meatpie.food", PUB_STD_PLATE );
 * @example
 * // Add a glass of ale, with the ale cloned from a file and the glass
 * // cloned in the "create_container()" function in the room code
 * add_menu_item( "Pint of ale", PUB_ALCOHOL, 500, "/obj/food/ale.food",
 *    "new_pint_glass" );
 * @example
 * // The same as above, but we only want half a pint of ale in the glass, we
 * // want to use the standard glasses in the header file and we want to set
 * // the intoxification value of the ale to 5
 * add_menu_item( "Half-pint of ale in a pint glass", PUB_ALCOHOL, 300,
 *    "/obj/food/ale.food", PUB_STD_PINT, VOLUME_HALFPINT, 5 );
 * @example
 * // Create a beefburger with added vodka in the "create_item()" function in
 * // the room code and put it in a small satchel from the armoury
 * add_menu_item( "Beefburger with special sauce", PUB_MAINCOURSE, 800,
 *    "vodka_burger", "small satchel", 0, 7 );
 * @see query_menu_items()
 * @see remove_menu_item()
 * @see /include/volumes.h
 */
varargs void add_menu_item(
   string name,
   int type,
   int cost,
   string item,
   string container,
   int volume,
   int intox
) {
   class menu_item new_item;
   string noun, alias;
   string *adjectives, *aliases;

   if( intox < 0 )
      intox = 0;
   if( intox > 10 )
      intox = 10;

   new_item = new( class menu_item );
   new_item->type = type;
   new_item->cost = cost;
   new_item->item = item;
   new_item->container = container;
   new_item->volume = volume;
   new_item->intox = intox;
   _menu_items[name] = new_item;

   if( no_standard_alias ) {
      if( lower_case( name ) != name )
         add_menu_alias( lower_case( name ), name );
      return;
   }

   adjectives = explode( lower_case( name ), " " );
   noun = adjectives[sizeof(adjectives) - 1];
   adjectives = adjectives[0..sizeof(adjectives) - 2];

   aliases = calc_standard_aliases( adjectives );

   foreach( alias in aliases ) {
      add_menu_alias( implode( ({ alias, noun }), " " ), name );
   }
} /* add_menu_item() */

/** @ignore */
string *calc_standard_aliases( string *array ) {
   int i, num_aliases;
   string *new_alias, *aliases;

   if( !sizeof( array ) )
      return ({ 0 });

   if( sizeof( array ) == 1 )
      return ({ array[0], 0 });

   aliases = calc_standard_aliases( array[0..sizeof( array ) - 2] );

   num_aliases = sizeof( aliases );

   for( i = 0; i < num_aliases; i++ ) {
      new_alias = ({ aliases[i], array[ sizeof( array ) - 1] });
      aliases += ({ implode( new_alias, " " ) });
   }

   return aliases;
} /* calc_standard_aliases() */

/**
 * This method returns a list of all the items currently available in
 * the pub.
 * @return items available, listing type, price and intoxification value
 * @see add_menu_item()
 * @see remove_menu_item()
 */
mapping query_menu_items() {
   return _menu_items;
} /* query_menu_items() */

/**
 * This method checks to see if this is a pub.
 * @return always return 1
 */
int query_pub() {
   return 1;
} /* query_pub() */

/**
 * This method sets the language to use in the shop.
 * @param language the language to use
 */
void set_language(string language) {
   _language = language;
} /* set_language() */

/**
 * This method returns the language used in the shop.
 * @return the language used in the shop
 */
string query_language() {
   return _language;
} /* query_language() */

/**
 * This method allows you to remove an item from those currently available
 * in the pub.
 * @param name the name of the item to remove
 * @return 1 if successful, 0 if unsuccessful
 * @see add_menu_item()
 * @see query_menu_items()
 */
int remove_menu_item( string name ) {
   if( !_menu_items[name] ) {
      return 0;
   }

   map_delete( _menu_items, name );
   return 1;
} /* remove_menu_item() */

/** @ignore */
string string_menu( string *items ) {
   int loop;
   string str, place;

   str = ""; 
   place = this_object()->query_property( "place" );
   if( !place || ( place == "" ) ) {
      place = "default";
   }

   for( loop = 0; loop < sizeof(items); loop++ ) {
      str += sprintf( "    %-30s %s\n", items[loop],
             MONEY_HAND->money_value_string(
             _menu_items[items[loop]]->cost, place ) );
   }

   return str;
} /* string_menu() */

/** @ignore */
string *query_items_of_type( int type ) {
   int i;
   string *selected;
   string *items;

   selected = ({ });
   items = keys( _menu_items );

   for( i = 0; i < sizeof(items); i++ ) {
      if( _menu_items[items[i]]->type == type ) {
         selected += ({ items[i] });
      }
   }

   selected = sort_array( selected, (: _menu_items[$1]->cost -
                                       _menu_items[$2]->cost :) );

   return selected;
} /* query_items_of_type() */

/** @ignore */
string string_menu_of_type( int type ) {
   string str;
   string *items;

   items = query_items_of_type( type );
   if( !sizeof(items) ) {
      return "";
   }

   if( _display_subheadings ) {
      str = _menu_subheadings[ type ] + "\n";
   }

   str += string_menu( items );
   return sprintf( "%-=*s\n", (int)this_player()->query_cols(), str );
} /* string_menu_of_type() */

/**
 * This method produces the menu from the item information, with the menu
 * header at the top, all items available grouped by type and sorted by
 * cost.  If you don't want the menu printed this way then mask this
 * function and return your own.
 * @return the menu text
 * @see add_menu_item()
 * @see set_display_header()
 * @see set_menu_header()
 * @see set_display_subheadings()
 * @see set_menu_subheadings()
 */
string read() {
   string ret;

   ret = "\n";

   if( _display_header ) {
      ret += _menu_header + "\n";
   }

   ret += string_menu_of_type( PUB_APPETISER );
   ret += string_menu_of_type( PUB_MAINCOURSE );
   ret += string_menu_of_type( PUB_DESSERT );
   ret += string_menu_of_type( PUB_ALCOHOL );
   ret += string_menu_of_type( PUB_SOFTDRINK );
   ret += string_menu_of_type( PUB_HOTDRINK );

   return ret;
} /* read() */

/**
 * This method allows you to switch the menu header (defaults to "The menu
 * reads:") that appears at the top of the menu on and off.
 * @param value set to 1 to display header or 0 to remove them
 * @see query_display_header()
 * @see set_menu_header()
 * @see query_menu_header()
 */
void set_display_header( int value ) {
   _display_header = value;
} /* set_display_header() */

/**
 * This method returns a flag stating whether display of the menu header
 * is on or off.
 * @return 1 for header, 0 for no header
 * @see set_display_header()
 * @see set_menu_header()
 * @see query_menu_header()
 */
int query_display_header() {
   return _display_header;
} /* query_display_header() */

/**
 * This method sets the header that appears at the top of the menu.  By
 * default this is "The menu reads:".
 * @param header the new menu header
 * @see query_menu_header()
 * @see set_display_header()
 * @see query_display_header()
 */
void set_menu_header( string header ) {
   _menu_header = header;
} /* set_menu_header() */
 
/**
 * This method returns the current menu header text.
 * @return the menu header text
 * @see set_menu_header()
 * @see set_display_header()
 * @see query_display_header()
 */
string query_menu_header() {
   return _menu_header;
} /* query_menu_header() */

/**
 * This method allows you to switch the subheadings ("Alcoholic Beverages",
 * "Meals", etc) that appear above different types of items on and off.
 * @param value set to 1 to display subheadings or 0 to remove them
 * @see query_display_subheadings()
 * @see set_menu_subheadings()
 * @see query_menu_subheadings()
 */
void set_display_subheadings( int value ) {
   _display_subheadings = value;
} /* set_display_subheadings() */

/**
 * This method returns a flag stating whether display of the menu subheadings
 * is on or off.
 * @return 1 for headings, 0 for no headings
 * @see set_display_subheadings()
 * @see set_menu_subheadings()
 * @see query_menu_subheadings()
 */
int query_display_subheadings() {
   return _display_subheadings;
} /* query_display_subheadings() */

/**
 * This method sets the subheadings that appear at the top of the menu.
 * @param subheading the subheading to change (use the #defines listed in
 * "pub_shop.h")
 * @param text the new menu subheading text
 * @see query_menu_subheadings()
 * @see set_display_subheadings()
 * @see query_display_subheadings()
 */
void set_menu_subheadings( int subheading, string text ) {
   _menu_subheadings[ subheading ] = text;
} /* set_menu_subheadings() */
 
/**
 * This method returns the current menu subheading text.
 * @return the menu subheadings
 * @see set_menu_subheadings()
 * @see set_display_subheadings()
 * @see query_display_subheadings()
 */
string *query_menu_subheadings() {
   return _menu_subheadings;
} /* query_menu_subheadings() */
 
/**
 * This method allows you to add an alias to an item sold in the pub.  Many
 * aliases are added by default so you shouldn't have to use this too
 * often.  See the help on "set_no_standard_alias()" to see what aliases are
 * added automatically.
 * @param alias the alias to add
 * @param alias the real item that the alias refers to
 * @example
 * // Allow "buy lancre ale" instead of "buy Ale from Lancre"
 * add_menu_alias( "lancre ale", "Ale from Lancre" );
 * @see add_menu_aliases()
 * @see query_menu_aliases()
 * @see remove_menu_alias()
 * @see set_no_standard_alias()
 */
void add_menu_alias( mixed alias, string actual ) {
   string bing;

   if (arrayp(alias)) {
      foreach (bing in alias) {
         add_menu_alias(bing, actual);
      }
   }
   _menu_aliases[alias] = actual;
} /* add_menu_alias() */

/**
 * This method allows you to add multiple aliases at once.  Many aliases are
 * added by default so you shouldn't have to use this too often.  See the 
 * help on "set_no_standard_alias()" to see what aliases are added 
 * automatically.
 * @param aliases an array of aliases to add
 * @param alias the real item that the aliases refer to
 * @example
 * // Add friendly aliases to "Beef burger and chips"
 * add_menu_aliases( ({ "beef burger",
 *                      "beefburger",
                        "burger" }), "Beef burger and chips" );
 * @see add_menu_alias()
 * @see query_menu_aliases()
 * @see remove_menu_alias()
 * @see set_no_standard_alias()
 */
void add_menu_aliases( string *aliases, string actual ) {
   string alias;

   foreach( alias in aliases ) {
      add_menu_alias( alias, actual );
   }
} /* add_menu_aliases() */

/**
 * This method returns a list of all the aliases currently available in
 * the pub.
 * @return alias : real name
 * @see add_menu_alias()
 * @see add_menu_aliases()
 * @see remove_menu_alias()
 * @see set_no_standard_alias()
 */
mapping query_menu_aliases() {
   return _menu_aliases;
} /* query_menu_aliases() */

/**
 * This method allows you to remove an alias from those currently available
 * in the pub.
 * @param alias the alias to remove from the list
 * @return 1 if successful, 0 if unsuccessful
 * @see add_menu_alias()
 * @see add_menu_aliases()
 * @see query_menu_aliases()
 * @see set_no_standard_alias()
 */
int remove_menu_alias( string alias ) {
   if( !_menu_aliases[alias] ) {
      return 0;
   }

   map_delete( _menu_aliases, alias );
   return 1;
} /* remove_menu_alias() */

/**
 * This method allows you to turn on or off the addition of standard aliases
 * when new menu items are added.  By default it is turned on.  Standard
 * aliases are added as follows:
 * If you added an item called "Lancre vintage wine" the aliases added would
 * be:
 * <ul>
 * <li> vintage wine
 * <li> lancre wine
 * <li> wine
 * </ul>
 * An alias of the name in lowercase is always added regardless of whether
 * or not this flag is turned on or off.  You may wish to turn this off if
 * you are adding several items which could be mistaken for each other, for
 * instance "Lancre beer" and "Morporkian beer".
 * @param flag 0 if standard aliases should be added, 1 if not
 * @see add_menu_alias()
 * @see query_menu_aliases()
 * @see remove_alias()
 * @see query_no_standard_alias()
 */
void set_no_standard_alias( int flag ) {
   no_standard_alias = flag;
} /* set_no_standard_alias() */

/**
 * This method returns a flag stating whether standard aliases will be added
 * or not.
 * @return 0 if standard aliases will be added, 1 if not
 * @see set_no_standard_alias()
 */
int query_no_standard_alias() {
   return no_standard_alias;
} /* set_no_standard_alias() */

/** @ignore */
object create_real_object( string name ) {
   object item, container;

   if( _menu_items[name]->container ) {
      container = this_object()->create_container(
                                    _menu_items[name]->container );
      if( !container ) {
         container = clone_object( _menu_items[name]->container );
      }
      if( !container ) {
         container = ARMOURY->request_item(
                                 _menu_items[name]->container, 100 );
      }
   }

   if( _menu_items[name]->item ) {
      item = this_object()->create_item( _menu_items[name]->item );
      if( !item ) {
         item = clone_object( _menu_items[name]->item );
      }
      if( !item ) {
         item = ARMOURY->request_item( _menu_items[name]->item, 100 );
      }
   }

   if( item && _menu_items[name]->volume ) {
      item->set_amount( _menu_items[name]->volume );
   } else if ( item && !_menu_items[name]->volume &&
              ( _menu_items[name]->type == PUB_ALCOHOL ||
                _menu_items[name]->type == PUB_HOTDRINK ||
                _menu_items[name]->type == PUB_SOFTDRINK ) ) {
      item->set_amount( container->query_max_volume() -
                        container->query_volume() );
   }

   if( item && container ) {
      if( (int)item->move( container ) != MOVE_OK ) {
         write( "The " + container->short() + " is too small to hold " +
            item->the_short() + ".  Please file a bug report.\n" );
         item->move( "/room/rubbish" );
      }
   }

   if( container ) {
      return container;
   }

   if( item ) {
      return item;
   }

   return 0;
} /* create_real_object() */

/** @ignore */
int do_buy( object *obs, string dir, string indir, mixed *args ) {
   int value, cost;
   string str, place;
   object person, thing;
   object *succeededpeople, *deadpeople, *failedpeople, *poorpeople;

   succeededpeople = ({ });
   deadpeople = ({ });
   failedpeople = ({ });
   poorpeople = ({ });

   str = args[0];

   if( this_player()->query_property( "dead" ) ) {
      add_failed_mess( "How can you expect to buy " + str + " when you're "
         "dead?\n" );
      return 0;
   }

   if( _menu_aliases[str] ) {
      str = _menu_aliases[str];
   }

   if( !_menu_items[str] ) {
      if (!broadcast_shop_event(PUB_EVENT_NOT_AVAILABLE, this_player(), str)) {
         add_failed_mess( "Sorry, " + str + " is not on the menu.\n" );
      }
      return 0;
   }

   // Check to make sure the pub is open.
   if ( !is_open( this_player(), _menu_items[str]->type ) ) {
      broadcast_shop_event(PUB_EVENT_NOT_OPEN, this_player());
      return 0;
   }

   if( !sizeof( obs ) ) {
      obs = ({ this_player() });
   }

   foreach( person in obs ) {
      if( person->query_property( "dead" ) ) {
         deadpeople += ({ person });
         continue;
      }

      if( !living( person ) ||
          !interactive( person ) && !person->query_property( "npc" ) ) {
         failedpeople += ({ person });
         continue;
      }

      cost = ( _menu_items[str]->cost ) * query_discount( this_player() );
      place = this_object()->query_property( "place" );
      if( !place || ( place == "" ) ) {
         place = "default";
      }
      value = (int)this_player()->query_value_in( place );
      if( place != "default" ) {
         value += (int)this_player()->query_value_in( "default" );
      }

      if( cost > value ) {
         poorpeople += ({ person });
         continue;
      }

      thing = create_real_object( str );
      if( !thing ) {
         add_failed_mess( "Something is buggered.  Please file a bug report.  "
            "Thank you.\n" );
         return 0;
      }

      this_player()->pay_money( (mixed *)MONEY_HAND->create_money_array(
                        cost, place ), place );

      succeededpeople += ({ person });

      if( (int)thing->move( person ) != MOVE_OK ) {
         if( (int)thing->move( _counter ) != MOVE_OK ) {
            thing->move( this_object() );
            write("You cannot pick " + thing->a_short() +
               " up.  It's left on the floor for you.\n" );
         } else {
           write("You cannot pick " + thing->a_short() +
              " up.  It's left on the counter for you.\n" );
         }
      }
   }

   /* This will be overidden if necessary */
   //add_succeeded_mess( this_object(), "", ({ }) );

   if( sizeof( succeededpeople ) ) {
      // Quite why compare_arrays() isn't an efun is beyond me...
      if (!broadcast_shop_event(PUB_EVENT_BOUGHT_STUFF, this_player(),
                           succeededpeople, str)) {
         if( !MAP_HANDLER->compare_arrays( succeededpeople,
                                           ({ this_player() }) ) ) {
            this_player()->add_succeeded_mess( this_object(),
               "$N $V $I.\n", ({ add_a(str) }) );
         } else {
            add_succeeded_mess("$N $V "
               + query_multiple_short( ({ thing }), "a" ) + " for $I.\n",
               succeededpeople);
         }
      }
   } else {
      if( sizeof( deadpeople ) ) {
         if (!broadcast_shop_event(PUB_EVENT_DEAD_PEOPLE, this_player(),
                              deadpeople, str)) {
            add_failed_mess("What use " +
               ( sizeof( deadpeople ) > 1?"have ":"has " ) +
               query_multiple_short( deadpeople, "one" ) + " got for " + str +
               "?\n" );
         }
      }
   
      if( sizeof( failedpeople ) ) {
         if (!broadcast_shop_event(PUB_EVENT_FAILED_PEOPLE, this_player(),
                              failedpeople, str)) {
            add_failed_mess("You can't buy anything for " +
               query_multiple_short( failedpeople, "one" ) + ".\n" );
         }
      }
   
      if( sizeof( poorpeople ) ) {
         if (!broadcast_shop_event(PUB_EVENT_POOR_PERSON, this_player(),
                              poorpeople, str)) {
            add_failed_mess("You cannot afford to order " + str +
               " for " + query_multiple_short( poorpeople, "one" ) + ".\n" );
         }
      }
      return 0;
   }
   
   return 1;
} /* do_buy() */

/** @ignore */
void dest_me() {
   if( _menu_object ) {
      _menu_object->dest_me();
   }
} /* dest_me() */

/** 
 * This function can be masked and used to determine a discount that is
 * applied to all items sold, so for instance you could check the guild of
 * 'ob' and give a discount to Witches, or something.  By default the
 * discount is zero, so query_discount returns 1.0
 * @param ob the object doing the buying
 * @return a float to multiply the price by
 * @example
 * // Give Priests a 10% discount
 * float query_discount( object ob ) {
 *    if( ob->query_guild_ob() == "/std/guilds/priest.c" )
 *       return 0.9;
 *    else
 *       return 1.0;
 * }
 */
float query_discount( object ob ) {
   return 1.0;
} /* query_discount() */

/**
 * This function creates the counter for the pub.  It defaults to cloning
 * PUB_COUNTER_FILE and making it a hidden object.
 * If you are creating your own counter object then making it hidden is a
 * good idea.  If it's not hidden, make sure it at least cannot be moved or
 * buried.
 * The counter should clear empty objects with the "pub item" property if
 * they are placed on it.  Making it clear non-empty objects is a bad idea,
 * since items will be placed here if the purchaser is unable to carry
 * them.
 * @return the new counter object
 * @see query_counter()
 * @see /include/shops/pub_shop.h
 */
object make_counter() {
   object ob;

   ob = clone_object( PUB_COUNTER_FILE );
   add_hidden_object( ob );

   return ob;
} /* make_counter() */

/**
 * This method returns the object currently being used as a counter in the
 * pub.
 * @return the file name of the counter
 * @see make_counter()
 */
object query_counter() {
   return _counter;
} /* query_counter() */


/**
 * This method returns the object currently being used as a menu in the
 * pub.
 * @return the file name of the menu
 */
object query_menu() {
   return _menu_object;
} /* query_menu() */
