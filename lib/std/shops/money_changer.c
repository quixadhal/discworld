/**
 * The money changer inherit.
 */
#include <money.h>
#include <move_failures.h>

inherit "/std/room/basic_room";

int change(string str1, string str2);

void create() {
   add_help_file("money_changer");
   ::create();
} /* create() */

void init() {
  ::init();
  add_command("change", "<string> {into|to} <string>",
                (: change($4[0], $4[2]) :) );
} /* init() */

/**
 * This method changes the money into a specific type of thing.
 * @param words the money to change
 * @param type what to change it into
 */
int change( string words, string type ) {
   int val_index;
   int value;
   int value_type;
   int value_unit;
   int value_change;
   int i;
   string place;
   mixed money;
   object changed;
   object *monies;
   mixed *change;
   mixed *values;

   place = query_property( "place" );
   if ( !place || ( place == "" ) ) {
      place = "default";
   }

   values = (mixed *)MONEY_HAND->query_values_in( place );
   MONEY_OBJECT->set_money_array( values );
   val_index = (int)MONEY_OBJECT->find_best_fit( type );
   if ( val_index == -1 ) {
      return notify_fail( "The coin type \""+ type +
            "\" is not legal tender here.\n" );
   }

   money = (mixed)MONEY_HAND->parse_money( words, this_player(), place );
   if ( intp( money ) ) {
      switch( money ) {
         case NO_MATCH :
            return notify_fail( "You do not have \""+ words +"\".\n" );
         case NO_MONEY :
            return notify_fail( "You can only change money here.\n" );
         default :
            return notify_fail( "You can only change legal tender.\n" );
      }
   }

   /* Pull out the provincial coins otherwise the player gets ripped off */
   if (place != "default")  {
      monies = MONEY_HAND->filter_legal_tender(money, "default");
      if (monies[0])  monies[0]->move(this_player());
      if (!monies[1])  {
         return notify_fail( "You can only change legal tender.\n" );
      }
      money = monies[1];
   }

   value = (int)money->query_value_in( place );
   //
   // Check and see if they have any of the type of money which we are trying
   // to change to in the money object.
   //
/*
   if ( place != "default" ) {
      value += (int)money->query_value_in( "default" );
   }
 */
   i = member_array(values[val_index], money->query_money_array());
   if (i != -1) {
      value_type = money->query_money_array()[i + 1] * values[val_index + 1];
   }
   value_unit = values[val_index + 1] * 100 / query_property("rate");
   //
   // Figure out the end value.
   //
   value_change = (value - value_type) % value_unit;
   value = ( (value - value_type) / value_unit ) * values[val_index + 1] +
           value_type;
   if ( value < values[ val_index + 1 ] ) {
      money->move( this_player() );
      return notify_fail(
        "That is not even worth one "+ values[ val_index ] +" after I "
        "subtract my commission.\n" );
   }

//printf("%O %O %O %O\n", value, value_unit, value_change, value_type);

   changed = clone_object( MONEY_OBJECT );
   changed->set_money_array( ({ values[ val_index ],
                                value / values[ val_index + 1 ] }));
   change = (mixed *)MONEY_HAND->create_money_array( value_change, place );
   this_player()->add_succeeded_mess(this_object(),
    ({ "You changed "+query_multiple_short( money->half_short( 1 ) ) +
         " into "+
         query_multiple_short( changed->half_short( 1 ) ) +".\n",
       "$N $V some money into "+type+".\n" }), ({ }) );
        
   //write( "You get "+ query_multiple_short( (string)money->half_short( 1 ) ) +
         //" changed into "+
         //query_multiple_short( (string)changed->half_short( 1 ) ) +".\n" );
   money->move( "/room/rubbish" );
   if ( sizeof( change ) ) {
      changed->adjust_money( change );
      write( "You were returned "+ (string)MONEY_HAND->money_string( change )
            +" in change.\n" );
   }
   if ( (int)changed->move( this_player() ) != MOVE_OK ) {
      write( "Unfortunately, you're too heavily burdened to accept all that "+
            "money, so it's put on the table for you.\n" );
      changed->move( this_object() );
   }
   return 1;
} /* change() */
