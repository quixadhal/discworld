inherit "/std/effects/basic";
#define EFFECT "/std/spells/wizard/failure/calm"
 
/* calming poison.  This is used for the ananconda's */
 
void create()
{
   set_boundaries( ({ -40, -20 }), ({ 40, 80 }) );
}

mixed *query_coordinate() { return ({ -30, 60 }); }
 
query_attrs(coord)
{
   int total, intens;
   
   total = distance_within_region(coord);
   intens = total * 3;
   if (intens > 100) intens = 100;
 
   return ({ 5, 95, ({ }), ({ ({ "bright blue", intens }) }),
             ({ }), 
             ({ ({ "tangy", intens }), ({ "slightly tangy", intens/2 }) })
      });
}
 
action_drunk(ob, coord, quantity)
{
   int total, condam;
   string obmess, roommess;
   
   total = distance_within_region(coord);
   
   condam = total * quantity;

   condam = condam/1000;

   obmess = "You feel really amazingly calm.\n";
   roommess = ob->query_cap_name() + " face looks dreamy.\n"+
              ob->query_cap_name() + " smiles dreamily.\n";
   tell_object(ob, obmess);
   tell_room(environment(ob), roommess, ob);
   this_player()->add_effect(EFFECT, condam);
   return;
}
 
action_taste(ob, coord, quantity)
{
   if (quantity >= 1) tell_object(ob, "You feel a sense of calmness sweep "+
                                      "over your for a second.\n");
}

int query_anaconda_calmer() { return 1; }
