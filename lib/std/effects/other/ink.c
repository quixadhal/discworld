inherit "/std/effects/basic";
 
/* ink.  ... bit of a dummy effect really */
 
create()
{
   set_boundaries(
      ({ -10, -10 }), ({ 10, -1 }) );
}
 
backdrop() { return 1; }	/* important due to the messages in drunk */
 
query_ink() { return 1; }
 
query_attrs(coord)
{
   return ({ 20, 0,
             ({ ({ "black ink", 100 }) }),
             ({ ({ "black", 100 }) }),
             ({ }), /* no smell */
             ({ ({ "ink", 100 }) })
      });
}
 
 
action_taste(ob, coord, quantity)
{
   tell_object(ob, "The ink tastes so bad you spit it out immediately.\n");
   tell_room(environment(ob), ob->query_cap_name() + 
      " spits black ink out with a look of distaste.\n");
}
 
action_drunk(ob, coord, quantity)
{
   int total;
 
   if (quantity < 5) { /* taste amount */
      action_taste(ob, coord, quantity);
      return;
   }
   tell_object(ob, "That ink was really awful.\n");
   tell_object(ob, "You vomit.\n");
   tell_room(environment(ob),
      ob->query_cap_name() + 
      " looks queasy and vomits black ink everywhere.\n", ob);
   ob->adjust_hp(-random(quantity));
   return;
}
