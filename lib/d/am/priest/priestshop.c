#include "path.h"
inherit "std/item-shop";

object ben;

create()
{
   ::create();
   set_light(60);
   set_short("Temple Infirmary");
   set_long("A small room off of the main worship area.  This is where "+
"Ben the herbalist sometimes dispenses healing drafts.\n");
   add_object("heal_mild", 10 + random(7));
   add_object("heal_medium", 6 + random(5));
   add_object("heal_strong", 3 + random(3));
   add_object("cure_drunk", 5 + random(3));
   add_exit("south", ROOM+"templeofsg", "standard");
}

reset()
{
   ::reset();
   if (!ben) {
      ben = clone_object("obj/monster");
      ben->set_name("ben");
      ben->set_long("Ben is sprightly middle-aged monk with a stomach "+
"indicative of heavy ale sampling.  He seems a jolly fellow, and is good "+
"in the making of potions to ease the injured and befuddled.\n");
      ben->set_class("priest");
      ben->set_race("human");
      ben->set_level(50);
      ben->give_money(30,30,"brass");
      ben->give_money(10,40,"copper");
      ben->give_money(0,3,"silver");
      ben->move(this_object());
   }
}

do_buy(s)
{
   if (!ben) {
      notify_fail("Ben is not here to serve you!\n");
      return 0;
   }
   return ::do_buy(s);
}


do_browse(s)
{
   if (!ben) {
      notify_fail("Ben is not here to serve you!\n");
      return 0;
   }
   return ::do_browse(s);
}

do_list(s)
{
   if (!ben) {
      notify_fail("Ben is not here to serve you!\n");
      return 0;
   }
   return ::do_list(s);
}

create_object(s) {
   object ob, label;
   int strength;
   string colour;
   string vial_long;

   switch (s) {
   case "heal_mild":
      vial_long = "A small label has 'mild healing' written on it.\n";
      colour = "yellow";
      strength = 100;
      break;
   case "heal_medium":
      vial_long = "A small label has 'moderate healing' written on it.\n";
      colour = "green";
      strength = 500;
      break;
   case "heal_strong":
      vial_long = "A small label has 'strong healing' written on it.\n";
      colour = "turquoise";
      strength = 2000;
      break;
   case "cure_drunk":
   vial_long = "A small label has 'easing of hangovers' written on it.\n";
      colour = "purple";
      strength = 101;
      break;
   }
   ob = clone_object("obj/container");
   ob->set_long("A small "+colour+" glass vial.\n"+vial_long);
   ob->set_leak_rate(0);
   ob->set_max_volume(125);
   ob->set_short("small "+colour+" vial");
   ob->set_name("vial");
   ob->add_adjective("small");
   ob->set_main_plural(colour + " vials");
   ob->add_adjective(colour);
   ob->set_volume(100);
   ob->set_value(30);   /* note that this doesn't change :( */
   ob->set_water_volume(100);
   if (s=="cure_drunk")
      ob->set_ps_coord_quantity( "/std/effects/healing/drunk"->query_coordinate(), strength);
   else
      ob->set_ps_coord_quantity( ({ 15, 15 }), strength);
   return ob;
}

query_cost(ob)
{
   switch(ob->query_ps_quantity()) {
   case 100:
      return 60;
   case 101:	/* drunk heal */
      return 500;
   case 500:
      return 350;
   case 2000:
      return 1400;
   }
   return ob->query_value();
}
dest_me()
{
   if (ben) ben->dest_me();
   ::dest_me();
}
