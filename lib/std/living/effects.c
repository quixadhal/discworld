/* to go in player object somewhere */

#include "effect.h"

static int next_id, heart_beat_number;

#define EFF_OB_NAME 0
#define EFF_HBNUM 1
#define EFF_ARG 2

#define EFF_SHADOW 0
#define EFF_IDNUM 1
#define EFF_HBFREQ 2

#define EFF_NSIZE 3
#define EFF_SIZE 3

#define BASE_RATE 60	/* a basic effect heart beat of 1 minute */

mixed *eff_save;
/* ({ effect_ob_name, hb#, arg }) */
static mixed *eff_nsave;
/* ({ shadow_ob, id#, hbf }) */

create()
{
   eff_save = ({ });
   eff_nsave = ({ });
   next_id = 0;
   heart_beat_number = 0;
   call_out("effect_heart_beat", BASE_RATE);
}

void add_effect(string eff, mixed arg)
{
   string shad;
   object ob;
   int hbf, i;
   
   hbf = (int)eff->query_heart_beat_frequency();
   if (hbf < 1) throw("bad heart beat frequency");
  
   for (i=0;i<sizeof(eff_save);i+=EFF_SIZE)
      if (eff_save[i+EFF_OB_NAME] == eff) {
/*
 * Hmm.  Well. if they have a function on them we join up, otherwise we
 * add it again.
 */
         ob = find_object(eff);
         if (function_exists("merge_effect", ob)) {
            eff_save[i+EFF_ARG] = eff->merge_effect(this_object(),
                  eff_save[i+EFF_ARG], arg);
            return ;
         }
      }
   if (shad = (string)eff->query_shadow_ob()) {
      ob = clone_object(shad);
      eff_nsave += ({ ob, next_id, hbf });
      ob->attach_to_player(this_object(), next_id);
   } else
      eff_nsave += ({ 0, next_id, hbf });
  
   ob = find_object(eff);
   if (function_exists("beginning", ob))
      eff_save += ({ eff, 0, (mixed)eff->beginning(this_object(), arg, next_id++) });
   else {
      eff_save += ({ eff, 0, arg });
      next_id++;
   }
}

void init_after_save()
/* called after the restore_object is done to init shadows etc */
{
   int i, neffs, hbf;
   string shad, effn;
   object ob;

   neffs = sizeof(eff_save) / EFF_SIZE;

   eff_nsave = allocate(neffs * EFF_NSIZE);
   
   for (i = 0; i < neffs; i++) {
      effn = eff_save[i * EFF_SIZE + EFF_OB_NAME];
      if (shad = (string) effn->query_shadow_ob()) {
         ob = clone_object(shad);
         eff_nsave[i * EFF_NSIZE + EFF_SHADOW] = ob;
         ob->attach_to_player(this_object(), next_id);
      }
      
      hbf = (int) effn->query_heart_beat_frequency();
      
      eff_nsave[i * EFF_NSIZE + EFF_IDNUM] = next_id;
      eff_nsave[i * EFF_NSIZE + EFF_HBFREQ] = hbf;
      (void)effn->restart(this_object(), eff_save[i * EFF_SIZE + EFF_ARG], next_id++);
   }
}

int *effects_matching(string eff)
/* return an array of all effect indexes matching the partial string 'eff' */
{
   int i, neffs;
   int *match;
   
   match = ({ });
   
   neffs = sizeof(eff_nsave) / EFF_NSIZE;
   for (i = 0; i < neffs; i++) {
#define X eff_save[i * EFF_SIZE + EFF_OB_NAME]
      if (extract(X->query_classification(), 0, strlen(eff)-1) == eff) match += ({ i });
#undef X
   }
   return match;
}

void delete_effect(int i)
/* delete shadow and remove effect from eff_ arrays */
{
   mixed arg;
   int id;

   id = eff_nsave[i * EFF_NSIZE + EFF_IDNUM];
   arg = eff_save[i * EFF_SIZE + EFF_ARG];

   /* kill the shadow if it exists */
   if (eff_nsave[i * EFF_NSIZE + EFF_SHADOW])
      (void)this_object()->remove_effect_shadow(id);
      /* possibly here we should make a check to see if the shadow */
      /* object pointer == null ... */

   eff_save[i * EFF_SIZE + EFF_OB_NAME]->end(this_object(), arg, id);
   eff_save = delete(eff_save, i * EFF_SIZE, EFF_SIZE);
   eff_nsave = delete(eff_nsave, i * EFF_NSIZE, EFF_NSIZE);
}

void effects_thru_death()
/* called when player dies to see which effects to jetison due
   to a new body ... should be basically all of them, but who 
   knows, someone may want to write one that persists */
{
   int i;

   for (i = sizeof(eff_nsave) / EFF_NSIZE - 1; i >= 0; i--) {
      if (eff_save[i * EFF_SIZE + EFF_OB_NAME]->survive_death() == 0) 
         delete_effect(i);
   }
}

void remove_effects(string eff)
/* removes all effects matching 'eff' guaranteed */
{
   int *effs, i, effno;
   
   effs = effects_matching(eff);
   
   for (i = sizeof(effs) - 1; i >= 0; i--)
      delete_effect(effs[i]);
}

void remove_effect_with_id(int id)
/* removes effect matching id guaranteed */
{
   int i;
   
   for (i = 0; i < sizeof(eff_nsave); i += EFF_NSIZE) {
      if (id == eff_nsave[i + EFF_IDNUM]) { 
         delete_effect(i / EFF_NSIZE);
         return;
      }
   }
}

mixed *query_effect(int id) {
  int i;

  for (i=0;i<sizeof(eff_save);i+=EFF_NSIZE) {
    if (id == eff_nsave[i+EFF_IDNUM])
      return eff_save[i..(i+EFF_SIZE-1)];
  }
} /* query_effect() */

void remove_effect(string eff)
/* removes first effect matching 'eff' guaranteed */
{
   int *effs, i, effno;
   
   effs = effects_matching(eff);
   
   if (!sizeof(effs)) return;
   
   delete_effect(effs[0]);
}

mixed *query_effect_save() {  return eff_save; }
mixed *query_effect_nsave() { return eff_nsave; }

void effect_heart_beat()
{
   int i, neffs, id;
   mixed res, arg;
   
   call_out("effect_heart_beat", BASE_RATE);

/* now, this could be called every 30 normal heart beats ... but
   is an extra call_out less efficient than doing 30 extra compares
   ??? */

   heart_beat_number++;
   neffs = sizeof(eff_save) / EFF_SIZE;
   
   for (i = neffs - 1; i >= 0; i--) {
      arg = eff_save[i * EFF_SIZE + EFF_ARG];
      id = eff_nsave[i * EFF_NSIZE + EFF_IDNUM];
      if (arg == REMOVE_THIS_EFFECT_NEXT_HB) { 
         remove_effect_with_id(id); 
         continue; 
      }
      if ((heart_beat_number % eff_nsave[i * EFF_NSIZE + EFF_HBFREQ]) 
           == 0) {
         res = (mixed)eff_save[i * EFF_SIZE + EFF_OB_NAME]->
            effect_heart_beat(this_object(),
               ++eff_save[i * EFF_SIZE + EFF_HBNUM], arg, id);
         if (res == REMOVE_THIS_EFFECT) remove_effect_with_id(id);
         else eff_save[i * EFF_SIZE + EFF_ARG] = res;
      }
   }
}
