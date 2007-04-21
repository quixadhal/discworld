/**
 * This is a small inheritable to allow places to do engraving.  POut a
 * piece of code inthe init function which calls the engrave_init()
 * function.
 * @example
 * inherit "/std/room/basic_room";
 * inherit "/std/shops/engrave";
 *
 * void init() {
 *    basic_room::init();
 *    engrave_init();
 * } /\* init() *\/
 * @author Macchirton
 * @change 17th of May 1998 Pinkfish
 * Turned into an inheritable mbase and reworked to make work with add_command
 * correctly.
 *
 * Added set_engrave_language(string str) to solve the problem with 
 * "common" engravings  
 * - Danbala, Sept 2002
 */

#include <money.h>
#include <shops/engrave.h>

void set_engrave_language(string str);
protected int do_engrave( object *things, string message);
mixed query_property(string name);

private string _lingo = "general";

//private object shop_keeper;
//private int check_shop_keeper;

void create() {
   this_object()->add_help_file("engrave");
} /* create() */

/**
 * Sets the shop keeper for the engraving shop.  If the shop keeper is
 * not set then it will not be checked for.  If it is checked it will be
 * checked for the existance of.
 * @param ob the shop keeper to test for
 * @see query_engrave_shopkeeper()
 */
//void set_engrave_shopkeeper(object ob) {
//   check_shop_keeper = 1;
//   shop_keeper = ob;
//} /* set_engrave_shopkeeper() */

/**
 * This method returns the currently ste shop keeper for the engraving shop.
 * @return the shop keeper
 * @see set_engrave_shopkeeper()
 */
//object query_engrave_shopkeeper() {
//   return shop_keeper;
//} /*  query_engrave_shopkeeper() */

/**
 * This method should be called in the inheriting room's init function.  IUt
 * will setup the commands to allow the object to be engraved.
 * @see do_engrave()
 */
void engrave_init() {
   add_command( "engrave", "<string> on <indirect:object>",
                (: do_engrave($1, $4[0]) :));
} /* init() */

/**
 * This method is called when a living object is attempted to be engraved.
 * This should be overridden in the inheritable to print out the
 * message you wish to say.  If this function returns 1 then the
 * default failed message is not used.
 * @param liv the living objects
 * @return 0 use default fail mesasage, 1 override default fail message
 * @see do_engrave()
 */
protected int engrave_living(object *obs) {
   return 0;
} /* engrave_living() */

/**
 * This method is called if the objects in question are unable to be
 * engraved.
 * This should be overridden in the inheritable to print out the
 * message you wish to say.  If this function returns 1 then the
 * default failed message is not used.
 * @param obs the objects which could not be engraved.
 * @return 0 use default fail mesasage, 1 override default fail message
 * @see do_engrave()
 */
protected int engrave_wrong_items(object *obs) {
   return 0;
} /* engrave_wrong_items() */

/**
 * This method is called if the player does not have enough money to
 * complete the engraving.
 * This should be overridden in the inheritable to print out the
 * message you wish to say.  If this function returns 1 then the
 * default failed message is not used.
 * @param obs the objects which were unable to be engraved
 * @param cost the cost of the objects to be engraved
 * @return 0 use default fail mesasage, 1 override default fail message
 * @see do_engrave()
 */
protected int engrave_no_money(object *obs, int cost) {
   return 0;
} /* engrave_no_money() */

/**
 * This method is called when the engrave is successful.
 * This should be overridden in the inheritable to print out the
 * message you wish to say.  If this function returns 1 then the
 * default failed message is not used.
 * @see do_engrave()
 */
int engrave_objects(object *obs, int cost) {
   return 0;
} /* engrave_objects() */

/**
 * This method does the actual engraving.
 * @param things the things to get engraved.
 * @param message the message to engarve on the objects
 * @see engrave_init()
 */
protected int do_engrave( object *things, string message) {
   int cost;
   string coststring;
   string place;
   object *liv;
   object *engravable;

//   if ( check_shop_keeper && !shop_keeper){
//      add_failed_mess("There is noone here to engrave $I for you.\n", things);
//      return 0;
//   }

   liv = filter(things, (: living($1) :));
   if (sizeof(liv) && !engrave_living(liv)) {
      add_failed_mess("Cannot engrave messages on $I, they are living things.\n", liv);
   }
   things -= liv;
   if (!sizeof(things)) {
      return 0;
   }

   // Holy blade of soyin does weird things.
   engravable = filter(things,
                       (: ($1->query_property( "shop type" ) == "jewellers" ||
                         $1->query_property( ENGRAVE_PROP ) == 1 ||
                           $1->query_property( "shop type" ) == "armoury") &&
                          $1->query_short() != "holy blade of Soyin" :) );
   if (!sizeof(engravable)) {
      if (!engrave_wrong_items(things - engravable)) {
         add_failed_mess("Cannot engrave messages on $I.\n",
                         things - engravable);
      }
      return 0;
   }

   place = query_property("place");
   if (!place) {
      place = "default";
   }
   coststring = replace_string(message, " ", "");
   cost = (strlen(coststring) * 80) * sizeof(engravable);
   /*a dollar for 5 letters */
   if ( (int) this_player()->query_value_in( place ) < cost ) {
      if (!engrave_no_money(engravable, cost)) {
         add_failed_mess("Not enough money to engrave $I, you need " +
                         MONEY_HAND->money_value_string(cost, place) + ".\n",
                         engravable);
      }
      return 0;
   }
   if ( engrave_objects( engravable, cost ) ) {
      this_player()->pay_money( ( mixed *)MONEY_HAND->create_money_array( cost, 
                                place ), place );

      add_succeeded_mess( ({ "You have the message \"" + message + 
                            "\" engraved into $I for " +
                            MONEY_HAND->money_value_string(cost, place)
                            + ".\n", "$N gets $I engraved with a message.\n" }),
                            engravable);
      engravable->add_read_mess( message,
                 "neatly engraved letters", _lingo, 1);   
      return 1;
   }
   else return 0;

} /* do_engrave() */
/**
 * Init
 * Made compatable with the axiom Arghrgh is Arghrgh.
 * Also made OO complient.
 * Terano.
 */
void init() {
  engrave_init();
}

/**
 * This method sets which language (for example, "agatean", or
 * "morporkian") to be used when engraving.  It defaults to "general",
 * which is not to be recommended. 
 * @see do_engrave()
 */
void set_engrave_language(string lang) {
  _lingo = lang;
} /* set_language() */
