#define SAVE_FILE "/save/potion_space"

/* ok guys, gals and geeks, this is the potion space handler

   Started by Ember the Ebullient on 30-June-1992

   There is one array.  It contains an object name, and limits on the
   volume it occupies in the potion space.  The potion space will
   initially start at 3 dimensions, and we see what happens from there.

   Ok, well, here goes ...
*/

mixed *effect_vols;

create()
{
   restore_object(SAVE_FILE);
}

add_effect(ob, x1, x2, y1, y2)
{
   if (!stringp(ob)) {
      tell_object(this_player(), "Effect object must be a string.\n");
      return 0;
   }
   effect_vols = ({ ob, x1, x2, y1, y2 }) + effect_vols;
/* backwards so we get 'masking' */
   save_object(SAVE_FILE);
   return 1;
}

remove_effect(ob)
{
   int i;

   for (i = 0; i < sizeof(effect_vols); i+=5)
      if (effect_vols[i] == ob) {
         effect_vols = effect_vols[0..i-1] + effect_vols[i+5..sizeof(effect_vols)];
         break;
      }

   save_object(SAVE_FILE);
   return 1;
}

query_effect_at(coord)
{
   int i;
   string *obs;

   obs = ({ });

   for (i = 0; i < sizeof(effect_vols); i+=5) {
      if (coord[0] >= effect_vols[i+1] && coord[0] <= effect_vols[i+2] &&
          coord[1] >= effect_vols[i+3] && coord[1] <= effect_vols[i+4]) 
         obs += ({ effect_vols[i] });
   }
   return obs;
}

query_attrs_at(coord)
{
   string *effects;
   int sum, i, pass_through;
   mixed *attrs, *temp_attrs;
   
   effects = query_effect_at(coord);
   if (!sizeof(effects)) return ({ 0, 100, ({ }), ({ }), ({ }), ({ }) });
   
   attrs = effects[0]->query_attrs(coord);
   sum = 100;
   pass_through = effects[0]->query_pass_through(coord);
   i = 1;
   while (pass_through && i < sizeof(effects)) {
   /* if 'backdrop' returns true, then it should only be efficacious if there */
   /* is no masking function ... regardless of pass through */
      if (effects[i]->backdrop()) break;
      temp_attrs = effects[i]->query_attrs(coord);
      attrs = "/obj/container"->merge_potions(attrs, temp_attrs, 
                                   sum, pass_through, sum + pass_through);
      sum += pass_through;
      pass_through = effects[i]->query_pass_through(coord);
      i++;
   }
   
   return attrs;
}

potion_drunk(ob, coord, quantity)
{
   string *effects;
   int i, pass_through;
   
   effects = query_effect_at(coord);
   pass_through = 100;
   for (i = 0; i < sizeof(effects) && pass_through; i++) {
      if (i > 0 && effects[i]->backdrop()) break;
      effects[i]->action_drunk(ob, coord, quantity * pass_through / 100);
      pass_through = pass_through * effects[i]->query_pass_through(coord) / 100;
   }
   return;
}

potion_touch(ob, coord, quantity)
{
   string *effects;
   int i, pass_through;
   
   effects = query_effect_at(coord);
   pass_through = 100;
   for (i = 0; i < sizeof(effects) && pass_through; i++) {
      if (i > 0 && effects[i]->backdrop()) break;
      effects[i]->action_touch(ob, coord, quantity * pass_through / 100);
      pass_through = pass_through * effects[i]->query_pass_through(coord) / 100;
   }
   return;
}

potion_smell(ob, coord, quantity)
{
   string *effects;
   int i, pass_through;
   
   effects = query_effect_at(coord);
   pass_through = 100;
   for (i = 0; i < sizeof(effects) && pass_through; i++) {
      if (i > 0 && effects[i]->backdrop()) break;
      effects[i]->action_smell(ob, coord, quantity * pass_through / 100);
      pass_through = pass_through * effects[i]->query_pass_through(coord) / 100;
   }
   return;
}

potion_create(ob, coord, quantity)
{
   string *effects;
   int i, pass_through;
   
   effects = query_effect_at(coord);
   pass_through = 100;
   for (i = 0; i < sizeof(effects) && pass_through; i++) {
      if (i > 0 && effects[i]->backdrop()) break;
      effects[i]->action_create(ob, coord, quantity * pass_through / 100);
      pass_through = pass_through * effects[i]->query_pass_through(coord) / 100;
   }
   return;
}

query_effect_vols()
{
   return effect_vols + ({ });
}

reset_effect_vols()
{
   effect_vols = ({ });
   save_object(SAVE_FILE);
}

neutral_coordinate()
{
   return ({ 0, 0 });
}
