/**
 * Player-run shop shopping bag object.
 * This object is given to customers of player-run shops.
 * This object should not be inherited by anything, but is created by the 
 * player_shop storeroom.
 * @author Ringo
 * @started 1st August 1999
 */
#include <move_failures.h>
#include <player_shop.h>

inherit "/obj/baggage";

void setup()
{
   set_name("bag");
   set_short("shopping bag");
   add_adjective("shopping");
   set_main_plural("shopping bags");
   set_long("This is a small paper shopping bag, designed to "
      "hold a number of small items.  It doesn't look very sturdy.\n");
   set_value(3);
   set_transparent();
   add_property("no recycling",1);

   /* Weight: 1 unit = 50g */
   set_weight(1);
   set_max_weight(MAX_BAG_WEIGHT);
}
/* setup() */

void change_condition()
{
   adjust_cond(-(10 + random(10)));
}

int test_add(object thing, int flag)
{
   if (sizeof(all_inventory()) >= MAX_BAG_ITEMS)
   {
      tell_object(this_player(), "The bag can only hold "+ 
         query_num(MAX_BAG_ITEMS)+ " items.\n");
      return 0;
   }
   if (!query_cond()) set_cond(20);
   call_out((: change_condition() :), 1);
   return ::test_add(thing,flag);
}
/* test_add() */

int test_remove(object thing, int flag, mixed dest)
{
   if (!query_cond()) set_cond(20);
   call_out((: change_condition() :), 1);
   return ::test_remove(thing,flag,dest);
}
/* test_remove() */
