/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: potion_space.c,v 1.1 1998/01/06 05:03:33 ceres Exp $
 * $Log: potion_space.c,v $
 * Revision 1.1  1998/01/06 05:03:33  ceres
 * Initial revision
 * 
*/
#define SAVE_FILE "/save/potion_space"

/* ok guys, gals and geeks, this is the potion space handler

   Started by Ember the Ebullient on 30-June-1992

   There is one array.  It contains an object name, and limits on the
   volume it occupies in the potion space.  The potion space will
   initially start at 3 dimensions, and we see what happens from there.

   Ok, well, here goes ...
*/

mixed *effect_vols;

void create() {
  seteuid("Room");
  unguarded((: restore_object, SAVE_FILE :));
}

int add_effect(string ob, int x1, int x2, int y1, int y2) {
   if (!stringp(ob)) {
      tell_object(this_player(), "Effect object must be a string.\n");
      return 0;
   }
   effect_vols = ({ ob, x1, x2, y1, y2 }) + effect_vols;
/* backwards so we get 'masking' */
   unguarded((: save_object, SAVE_FILE :));
   return 1;
}

int remove_effect(object ob) {
   int i;

   for (i = 0; i < sizeof(effect_vols); i+=5)
      if (effect_vols[i] == ob) {
         effect_vols = effect_vols[0..i-1] + effect_vols[i+5..sizeof(effect_vols)];
         break;
      }

   unguarded((: save_object, SAVE_FILE :));
   return 1;
}

string *query_effect_at(int *coord) {
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

mixed *query_attrs_at(int *coord){
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

void potion_drunk(mixed ob, int *coord, int quantity) {
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

void potion_touch(mixed ob, int *coord, int quantity) {
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

void potion_smell(mixed ob, int *coord, int quantity) {
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

void potion_create(mixed ob, int *coord, int quantity) {
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

mixed *query_effect_vols() {
   return effect_vols + ({ });
}

/* Dangerous fucntion....
reset_effect_vols()
{
   effect_vols = ({ });
   unguarded((: save_object, SAVE_FILE :));
}
 */

int *neutral_coordinate() {
   return ({ 0, 0 });
}
