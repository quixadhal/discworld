/**
 * Handler to handle the gathering of implicitly existing
 * objects that require some skill to find, such as herbs.
 * Recognition (ie, knowing what it is once you've found it) may be
 * added later.<p>
 * The data used for gathering comes from two places: the handler's
 * database of gatherable items, and the room's local information.<p>
 * The handler's database contains a class entry for each gatherable:<br>
 * <dl>
 * <dt> skill (string)
 * <dd> The skill needed to find the item.
 * <dt> difficulty (int)
 * <dd> The taskmaster difficulty.
 * <dt> upper (int)
 * <dd> The taskmaster "upper" parameter.
 * <dt> extra (mixed)
 * <dd> The taskmaster "extra" parameter.
 * <dt> season (string *)
 * <dd> The seasons during which the gatherable is available (eg, plants).
 * If this is unset, the default is all seasons.
 * Note that, currently, it is the same season everywhere on the Disc.
 * <dt> quant (int or function pointer returning an int)
 * <dd> The quantity that will be gathered.  The function pointer is
 * passed three parameters: the room, the player, and the name of the
 * gathered item.  This allows, for example, a function to be called
 * on the room to calculate the quantity, such as:
 * <pre><br>
 *     <code>(: $1->room_func($2, $3) :)</code>
 * </pre>
 * <dt> ob (string)
 * <dd> The pathname of the object to be created.  If the object is
 * continuous, the <i>amount</i> of ob is set to <i>quant</i>; otherwise,
 * <i>quant</i> copies of the object are cloned.
 * </dl><p>
 * The room's local information is set when the gatherable is added
 * via add_item(), and consists of:<p>
 * <dl>
 * <dt> item name (string or function pointer returning a string)
 * <dd> The name that will be used to look up the gatherable in the
 * handler's database.  The function pointer is passed two parameters
 * when evaluated: the room, and the player.
 * <dt> scarcity (int or function pointer returning an int)
 * <dd> The percent chance of finding any quantity of the gatherable.
 * This is an additional
 * constraint on finding the gatherable, in addition to any skill
 * requirements and quantity calculations.  A scarcity of 100 (the default)
 * indicates no scarcity constraint.  A scarcity of 0 means that the
 * gatherable will never be found.
 * </dl>
 * @author Jeremy
 */

#include <weather.h>
#include <tasks.h>

#define INIT_FILE "/obj/gatherables/handler.dat"

class item_data {
    string skill;          // skill needed to find this item
    int    difficulty;     // taskmaster difficulty
    string *season;        // available season(s)
    mixed  quant;          // quantity (int or (int)(:<code>:))
    string ob;             // filename of object to be created
}

mapping items = ([ ]);

void create() {
    seteuid("/secure/master"->creator_file(file_name(this_object())));
    items = "/obj/handlers/data"->compile_data(({ INIT_FILE }));
} /* create() */

/**
 * This method will return an array of the information associated with
 * the item.  The array consists of:
 * <pre>
 *   ({
 *      skill,       // skill needed to find this item   (string)
 *      difficulty,  // taskmaster difficulty            (int)
 *      season,      // available season(s)              (string *)
 *      quant,       // quantity (int or (int)(:\<code\>:)) (mixed)
 *      ob           // filename of object to be created (string)
 *    })
 * </pre>
 * @return the item array as detailed above.
 * @param name the name of the item to query
 * @see add_item()
 */
mixed query_item( string name ) {
    class item_data h;

    if (!(items[name])) {
        return 0;
    }
    h = (class item_data)items[name];
    return ({ h->skill, h->difficulty, h->season, h->quant, h->ob });
} /* query_item() */

/**
 * This method adds an item into the current list of gatherable items.
 * @param name the name of the item to add
 * @param skill skill needed to find this item
 * @param diff taskmaster difficulty
 * @param season available season(s)
 * @param quant quantity (int or function pointer returning an int)
 * @param ob filename of object to be created
 * @see query_item()
 * @see gather_item()
 */
void add_item( string name, string skill, int diff,
               string *season, mixed quant, string ob ) {
    class item_data h;

    h = new( class item_data );
    h->skill = skill;
    h->difficulty = diff;
    h->season = season;
    h->quant = quant;
    h->ob = ob;
    items[name] = h;
} /* add_item() */

/**
 * This method returns all of the current gatherable items.  It returns
 * this as a mapping of a class, so probably not overly useful except
 * for debugging.
 * @return mapping of a locally defined class
 * @see query_item()
 * @see add_item()
 */
mapping query_items() {
    return items;
} /* query_items() */

/**
 * This method attempts to gather some items in the environment of
 * the specified player.
 * @param word the item to try and gather
 * @param player the player doing the gathering
 * @return an array of gathered objects
 * @see query_item()
 * @see add_item()
 */
object *gather_item( string word, object player ) {
object *basket = ({ });              //The basket that our gathered items go into.
object *matched = ({ });             //Objects that match our word.
object item;                   //Object we are building.
mixed  *current_item;                //Data on current gatherable.
class item_data info;                //Info on the gatherable we're looking at.
string temp_name;                    //Name information.
int temp_scarcity;                   //Scarcity information.
int result;                    //Skill check for results.
int quantity;                  //Amount!

   if ( !objectp( player ) || !environment( player ) )
      return ({ });

   matched = match_objects_for_existence( word, ({ environment( player ) }) );

   if ( !sizeof( matched ) )
      return ({ });

   foreach( object temp in matched ) {
        //If it's in the room, just pick it up.
      if ( environment( temp ) == environment( player ) )
         basket += ({ temp });

       //If it's an add item, like most gatherables, we have to be more fancy.
      current_item = temp->query_gather();

        //I don't like doing it this way, I wish it was a class.
      temp_name = "";
      temp_scarcity = 0;

      //Whatever they matched isn't gatherable.
      if ( !sizeof( current_item ) )
         continue;

      if ( sizeof( current_item ) != 1 ) {
         error( "When Terano recoded this, he assumed that this array would "
            "only contain one array inside it. This turned out to be "
            "wrong.\n" );
      }

      for ( int x = 0; x < sizeof( current_item[0] ); x = x + 2 ) {
            //I don't know why this is an array, but it is.
         switch ( current_item[ 0 ][ x ] ) {
           case "item name":
           case "item_name":
                  temp_name = evaluate( current_item[ 0 ][ x+1 ],
               environment( player ), player );

              if ( !stringp( temp_name ) )
                    temp_name = 0;
                  break;
           case "scarcity":
                    temp_scarcity = evaluate(current_item[0][ x+1 ],
                  environment( player ), player, temp_name);
                    if ( !intp( temp_scarcity ) )
                        temp_scarcity = 0;
                    break;

         }
      }

      tell_creator( player, "Started to check for %s.\n", temp_name );

       if ( random( 100 ) >= temp_scarcity) {
         tell_creator( player, "Not found %s due to scarcity (%d).\n",
            temp_name, temp_scarcity );
         continue;
      }

      if (!items[temp_name]) {
         tell_creator( player, "Not found %s due to no info!\n",
            temp_name );
         continue;
        }

      info = items[ temp_name ];

      if ( sizeof( info->season ) ) {
          if (member_array( WEATHER->query_season(), info->season ) == -1) {
         tell_creator( player, "Not found %s: Out of season!\n",
            temp_name );
         continue;
        }
      }

      if ( stringp( info->skill ) && strlen( info->skill ) ) {
         result = TASKER->perform_task( player, info->skill ,info->difficulty, TM_FREE );

      switch( result ) {
         case FAIL:
         case BARF:
            tell_creator( player, "Not found %s: Skillcheck failed!\n",
               temp_name );
            continue;

         case AWARD:
            tell_object(player,
                  "%^YELLOW%^You have learned something new about gathering!%^RESET%^\n" );
         case SUCCEED:
            //We just use the result further down the line.
         }
      }

      quantity = evaluate( info->quant, environment( player ), player,
         temp_name );

      if ( !quantity || !intp( quantity ) )
         continue;

      item = clone_object( info->ob );

        if (item->query_continuous()) {
         item->set_amount( quantity );
            basket += ({ item });
        }
        else {
         item->dest_me();
         basket += allocate( quantity, (: clone_object( $(info->ob) ) :) );
      }
      continue; //Continue the foreach loop.
   }
   return basket;
} /* gather_item() */
