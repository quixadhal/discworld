inherit "/std/effects/basic";
 
/* background constitution poison */
 
void create()
{
   set_boundaries( ({ -20, 40 }), ({ 20, 80 }) );
}
 
int backdrop() { return 1; }
 
query_attrs(coord)
{
   int total, intens;
   
   total = distance_within_region(coord);
   intens = total * 3;
   if (intens > 100) intens = 100;
 
   return ({ 5, 95, ({ }), ({ ({ "burnt orange", intens }) }),
             ({ ({ "rubber", intens / 2 }) }), 
             ({ ({ "charcoal", intens }), ({ "rubber", intens/2 }) })
      });
}
 
action_drunk(ob, coord, quantity)
{
   int total, condam;
   string obmess, roommess;
   
   total = distance_within_region(coord);
   
   condam = total * quantity / 500;
   
   switch (condam) {
   case 0:
      obmess = "You briefly feel like you have been punched in the stomach.\n";
      roommess = ob->query_cap_name() + " looks slightly winded.\n";
      break;
   case 1:
   case 2:
      obmess = "You feel like someone has just punched you in the stomach.\n";
      roommess = ob->query_cap_name() + " doubles over looking out of breath.\n";
      break;
   case 3:   
      obmess = "You feel like someone has punched you viciously in the stomach.\n";
      roommess = ob->query_cap_name() + " nearly collapses.\n";
      break;
   case 4:
   case 5:
      obmess = "You collapse with a feeling of your life being drained.\n";
      roommess = ob->query_cap_name() + " collapses with a look of having the "+
         "stuffing knocked out of them.\n";
      break;
   default:
      obmess = "You reel and collapse with all your health fleeing your body.\n";
      roommess = ob->query_cap_name() + " collapses with a face palid as the "+
         "face death would have if death had a face.\n";
      break;
   }
 
   tell_object(ob, obmess);
   tell_room(environment(ob), roommess, ob);
   ob->adjust_tmp_con(-condam);
   ob->reset_all();
   return;
}
 
action_taste(ob, coord, quantity)
{
   if (quantity >= 1) tell_object(ob, "Your tongue feels numb.\n");
}
