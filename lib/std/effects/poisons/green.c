inherit "/std/effects/basic";

/* this is the low level background poison */

void create()
{
   set_boundaries( ({ -50000, -50000 }), ({ 50000, 50000 }) );
}

int backdrop() { return 1; }

query_attrs(coord)
{
   int total;
   
   total = (coord[0] < 0) ? -coord[0] : coord[0];
   total += (coord[1] < 0) ? -coord[1] : coord[1];
   return ({ 5, 95, ({ }), ({ ({ "icky green", (total < 100) ? total : 100 }) }),
             ({ ({ "bile", (total < 100) ? total : 100 }) }), 
             ({ ({ "bile", (total < 100) ? total : 100 }) }) });
}

query_coordinate()
{
   return ({ -20, -20 });  /* hopefully some decent value.  If it gets masked, then we'll have to move it won't we? */
}

action_drunk(ob, coord, quantity)
{
   int total;

   if (quantity < 5) { /* taste amount */
      tell_object(ob, "You feel quite ill briefly.\n");
      tell_room(environment(ob), 
         ob->query_cap_name() + " looks green for a moment.\n", ob);
      return;
   }
   /* I want to add a long-term effect on to the player ... */
   tell_object(ob, "You feel really really ill.\n");
   tell_object(ob, "You are racked with spasms of vomiting.\n");
   tell_room(environment(ob),
      ob->query_cap_name() + " looks incredibly ill, and vomits violently.\n", ob);
   total = (coord[0] < 0) ? -coord[0] : coord[0];
   total += (coord[1] < 0) ? -coord[1] : coord[1];
   ob->adjust_hp(-random(total * quantity / 100));
   return;
}
