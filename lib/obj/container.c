#include "move_failures.h"
#include "parse_command.h"
#include "potion_attrs.h"

inherit "/std/container";
inherit "/std/basic/close_lock";

#define MAX_INVENT 40
#define C_CLOSED 1
#define C_TRANS 2
#define C_OPAQUE 1

static int full_weight, leak_rate, hb_count;

/* liquid/potion stuff */
mixed *misc_attrs;   /* purely physical attributes ... all the 'inactive' parts */
static mixed *all_attrs;    /* misc_attrs merged with active_attrs */
static mixed *active_attrs; /* this is the attrs due to the position in the potion space */

int volume, max_volume;  /* total volume, and max volume for this container */
int water;    /* the 'water' part of the volume ... the volume that is used in */
              /* potion space activities.  The volume that is not 'water' is */
              /* considered 'inactive' in any potion reactions */

int *ps_coord; /* coordinate in the potion_space */
int ps_quantity; /* magic number describing quantity of active junk in this potion */

static string *potion_id, *potion_adjective;
static int volume_to_womble;	/* kludge for the fraction stuff */

void set_volume_to_womble(int n) { volume_to_womble = n; }
int query_volume_to_womble() { return volume_to_womble; }

void set_full_weight(int i) { full_weight = i; }
void set_leak_rate(int i) { leak_rate = i; }
int query_full_weight() { return full_weight; }
int query_leak_rate() { return leak_rate; }

int query_volume() { return volume; }
void set_volume(int i) { volume = i; if (leak_rate) set_heart_beat(1); }

int query_max_volume() { return max_volume; }
/*
void set_max_volume(int i) { max_volume = i; }
 */

int *query_ps_coord() { return ps_coord; }
int query_ps_quantity() { return ps_quantity; }

void set_ps_quantity(int n) { ps_quantity = n; }

int *query_all_attrs() { return all_attrs; }
int *query_misc_attrs() { return misc_attrs; }
int *query_active_attrs() { return active_attrs; }

void set_water_volume(int n) { water = n; if (leak_rate) set_heart_beat(1); }
int query_water_volume() { return water; }

void set_max_weight(int i) {
  if (!max_volume && i)
    max_volume = i*10;
  max_weight = i;
} /* set_max_weight() */

void set_max_volume(int i) {
  if (!max_weight && i)
    max_weight = i/10;
  max_volume = i;
} /* set_max_volume() */

int empty_formula()  
/* formula to work out how close to empty someone can get casually */
{
   int vol_lost;

   vol_lost = 2 + volume * 95 / 100;
   if (vol_lost > volume) vol_lost = volume;
   return vol_lost;
} /* empty_formula() */

cull_neutral_obs(ob)
{
   if (ob->query_vect()) return 1;
   return 0;
}
 
contents_vect()
{
   object *inv;
   int *v;
   int ang, str, tot, i;
 
   inv = all_inventory(this_object());
   inv = filter_array(inv, "cull_neutral_obs", this_object());
   
   if (sizeof(inv) == 0) return ({ 0, 0, 0 });
 
   tot = ang = str = 0;
 
   for (i = 0; i < sizeof(inv); i++) {
      v = inv[i]->query_vect(); 
      tot += v[0];
      ang += v[1] * v[0];
      str += v[2] * v[0];
   }
   
   return ({ ang / tot, str / tot, tot / sizeof(inv) });
}
 
dest_active_contents()
{
   object *inv;
   int i;
 
   inv = all_inventory(this_object());
   inv = filter_array(inv, "cull_neutral_obs", this_object());
 
   for (i = 0; i < sizeof(inv); i++) inv[i]->dest_me();
   return sizeof(inv);
}
 
squidge_vector(v, meth_ang, meth_mult)
{
   int ca, cb;
   int da, cx, cy;
   object trig;
   
   trig = find_object("/obj/handlers/trig");
   
   da = meth_ang - v[0];
   ca = v[1] * trig->cos(da);
   cb = v[1] * trig->sin(da);
   ca = ca * meth_mult;
   cb /= meth_mult;
   ca /= 1000000;
   cb /= 1000000;
   cx = ca * trig->cos(meth_ang) + cb * trig->sin(meth_ang);
   cy = ca * trig->sin(meth_ang) - cb * trig->cos(meth_ang);
   return ({ cx / 1000000, cy / 1000000 });
}

string transparency_string(int trans)
/* return string describing transparency 'trans' */
{
   switch (trans) {
   case -10..9: return "opaque";
   case 10..24: return "milky";
   case 25..39: return "murky";
   case 40..59: return "cloudy";
   case 60..74: return "misty";
   case 75..89: return "slightly misty";
   case 90..110: return "clear";
/* if they've got the numbers stuffed up, they should _know_ about it :) */
   default: return "paisley";
   }
} /* transparency_string() */

string consistency_string(int cons)
/* return string describing consistency 'cons' */
{
   switch (cons) {
   case -10..9: return "watery liquid";
   case 10..24: return "slightly viscous watery liquid";
   case 25..39: return "runny syrup";
   case 40..59: return "syrup";
   case 60..74: return "jelly";  /* viscous liquid? */
   case 75..89: return "paste";
   case 90..110: return "solid";
/* see above comment in transparency */
   default: return "non-Newtonian fluid";
   }
} /* consistency_string() */

void update_potion_parse()
{
   int i;
   string *exploded;

   potion_adjective = adjectives +
                  ({ transparency_string(all_attrs[POTION_TRANSPARENCY]) });
   potion_id = ({ name }) + alias + 
                  ({ consistency_string(all_attrs[POTION_CONSISTENCY]) });
   for (i = 0; i < sizeof(all_attrs[POTION_NAMES]); i++) {
      exploded = explode(all_attrs[POTION_NAMES][i][0], " ");
      potion_id += ({ exploded[sizeof(exploded) - 1] });
      if (sizeof(exploded) > 1) {
         potion_adjective += exploded[0..sizeof(exploded) - 2];
      }
   }
   
   for (i = 0; i < sizeof(all_attrs[POTION_COLOURS]); i++) 
      potion_adjective += explode(all_attrs[POTION_COLOURS][i][0], " ");
   /* plurals?  Lets forget them for now */
}

void void_liquid()
{
   volume = 0;
   water = 0;

   misc_attrs = allocate(POTION_ATTRS_SIZE);
   misc_attrs[POTION_CONSISTENCY] = 0;
   misc_attrs[POTION_TRANSPARENCY] = 100;
   misc_attrs[POTION_NAMES] = ({ });
   misc_attrs[POTION_COLOURS] = ({ });
   misc_attrs[POTION_SMELLS] = ({ });
   misc_attrs[POTION_FLAVOURS] = ({ });
   
   ps_coord = (int *)POTION_SPACE_HANDLER->neutral_coordinate();
   ps_quantity = 0;
   
/* Hmmm.  I wonder if having these all the same array will cause problems */
   active_attrs = all_attrs = misc_attrs;
   update_potion_parse();
}

void create() {
  container::create();
  close_lock::create();
  
  void_liquid();

  add_property("liquid", 1);
  add_property("watertight", 1);

  leak_rate = 1000;
}

void init() {
/*  liquid::init(); */
  (void)this_player()->add_command("drink", this_object());
  (void)this_player()->add_command("splash", this_object(), "%D %p %I");
  (void)this_player()->add_command("rub", this_object(), "%D %p %I");
  (void)this_player()->add_command("apply", this_object(), "%D %p %I");
  (void)this_player()->add_command("pour", this_object(), "%D %p %I");
  (void)this_player()->add_command("taste", this_object());
  (void)this_player()->add_command("smell", this_object());
/*
  add_action("fill", "fill");
 */
  this_player()->add_command("fill", this_object(), "%I %p %D");
  this_player()->add_command("fill", this_object(), "%I %d/%d up %p %D");
  this_player()->add_command("fill", this_object(), "%I %d/%d full %p %D");
  (void)this_player()->add_command("empty", this_object());

  container::init();  /* ? */
  close_lock::init();
}

int query_weight() {
  if (!full_weight)
    return ::query_weight();
  return ::query_weight() + ((loc_weight+(max_weight*volume)/max_volume)
                             *full_weight)/max_weight;
}

int query_weight_left() {
  if (!max_volume)
    return max_weight - loc_weight;
  return max_weight - loc_weight - (max_weight*volume)/max_volume;
}

int query_volume_left() {
  if (!max_weight)
    return max_volume - volume;
  return max_volume - volume - (max_volume*loc_weight)/max_weight;
}

int add_weight(int n) {
  int old, new, amt;

  if (query_weight_left() < n)
    return 0;
  if (!full_weight)
    return ::add_weight(n);
  old = query_weight();
  loc_weight += n;
  new = query_weight();
  if (new != old)
    if (environment() && !environment()->add_weight(new-old)) {
      loc_weight -= n;
      return 0;
    }
  return 1;
}

string pretty_short(int dark) {
  string s;
  if (trans == C_TRANS && closed != C_CLOSED) {
    s = query_multiple_short(all_inventory());
    return this_object()->short(dark)+short_status()+(s!=""?", it contains "+s:s);
  }
  return (string)this_object()->short(dark)+short_status();
}

string pretty_plural() {
  string s;
  if (trans != C_TRANS && closed != C_CLOSED)
    return ::pretty_plural();
  s = query_multiple_short(all_inventory());
  return ::pretty_plural()+short_status()+(s!=""?", they contain "+s:s);
}

int transfer_all_to(object dest) {
  object *ob;
  int i;
 
/* potion stuff?  Who knows */
  ob = all_inventory(this_object());
  for (i=0;i<sizeof(ob);i++)
    ob[i]->move(dest);
  if (first_inventory(this_object())) return 0;
  return 1;
}

string comma_list(string *lst)
/* I am sure this is done elsewhere, however I am not going to find it */
/* just now */
{
   int i;
   string result;

   if (sizeof(lst) < 1) return "";
   if (sizeof(lst) == 1) return lst[0];
   result = "";
   for (i = 0; i < sizeof(lst) - 2; i++) result += lst[i] + ", ";
   return result + lst[i] + " and " + lst[i+1];
}

string liquid_name()
{
   string liq_name;
   mixed *names, *colours;
   string *hi, *med, *lo;   /* groups of strings with hi/med/lo intensities */
   int no_names, no_colours, i;
   
   names = all_attrs[POTION_NAMES];
   colours = all_attrs[POTION_COLOURS];
   
   no_names = (sizeof(names) < 1 || names[0][1] < VERY_SMALL_AMOUNT);
   no_colours = (sizeof(colours) < 1 || colours[0][1] < VERY_SMALL_AMOUNT);
   
   if (no_names && no_colours) {
      liq_name = "a colourless " + 
         transparency_string(all_attrs[POTION_TRANSPARENCY]) + " " +
         consistency_string(all_attrs[POTION_CONSISTENCY]);
   } else if (no_names) {
      if (colours[0][1] < SMALL_AMOUNT) liq_name = "a faint " + colours[0][0];
      else liq_name = add_a(colours[0][0]);
      liq_name += " " + transparency_string(all_attrs[POTION_TRANSPARENCY]) +
                     " " + consistency_string(all_attrs[POTION_CONSISTENCY]);
      med = ({ });
      i = 1;
      while (i < sizeof(colours) && colours[i][1] >= SMALL_AMOUNT)
         med += ({ colours[i++][0] });
      
      lo = ({ });
      while (i < sizeof(colours) && colours[i][1] >= VERY_SMALL_AMOUNT)
         lo += ({ colours[i++][0] });
      
      if (sizeof(med)) {
         liq_name += " with swirls of " + comma_list(med);
      }
      
      if (sizeof(lo)) {
         if (sizeof(med)) liq_name += " and faint streaks of " + comma_list(lo);
         else liq_name += " with faint streaks of " + comma_list(lo);
      }
   } else if (no_colours) {
      i = 0;
      med = ({ });
      lo = ({ });
      while (i < sizeof(names) && names[i][1] >= SMALL_AMOUNT) 
         med += ({ names[i++][0] });
      while (i < sizeof(names) && names[i][1] >= VERY_SMALL_AMOUNT)
         lo += ({ names[i++][0] });
      
      if (sizeof(med) > 1) liq_name = "a mixture of " + comma_list(med);
      else if (sizeof(med) == 1) liq_name = med[0];
      
      if (!sizeof(med) && sizeof(lo) > 1) 
         liq_name = "a diluted mixture of " + comma_list(lo);
      else if (!sizeof(med) && sizeof(lo) == 1)
         liq_name = "diluted " + lo[0];
      else if (sizeof(med) && sizeof(lo))
         liq_name += "and small quantities of " + comma_list(lo);
   } else {
      if (names[0][1] > colours[0][1] / 2) 
      /* arbitrary relationship ... names are 'twice' as visible as colours */
      /* if the primary name is more visible than the primary colour, */
      /* then we use this scenario: */
      /*   "chunky soup (a yellow cloudy syrup with swirls of green, orange */
      /*    and brown)" */
      /* else we use the colour scenario: */
      /*   "a yellow cloudy syrup containing chunky soup with swirls of green, */
      /*    orange and brown" */
      {
         /* this little fragment copied directly from 'no_colours' above */
         i = 0;
         med = ({ });
         lo = ({ });
         while (i < sizeof(names) && names[i][1] >= SMALL_AMOUNT) 
            med += ({ names[i++][0] });
         while (i < sizeof(names) && names[i][1] >= VERY_SMALL_AMOUNT)
            lo += ({ names[i++][0] });
      
         if (sizeof(med) > 1) liq_name = "a mixture of " + comma_list(med);
         else if (sizeof(med) == 1) liq_name = med[0];
      
         if (!sizeof(med) && sizeof(lo) > 1) 
            liq_name = "a diluted mixture of " + comma_list(lo);
         else if (!sizeof(med) && sizeof(lo) == 1)
            liq_name = "diluted " + lo[0];
         else if (sizeof(med) && sizeof(lo))
            liq_name += "and small quantities of " + comma_list(lo);
         
         /* this little fragment copied directly from 'no_names' above */
         
         if (colours[0][1] < SMALL_AMOUNT) liq_name += " (a faint " + colours[0][0];
         else liq_name += " (" + add_a(colours[0][0]);
         liq_name += " " + transparency_string(all_attrs[POTION_TRANSPARENCY]) +
                        " " + consistency_string(all_attrs[POTION_CONSISTENCY]);
         med = ({ });
         i = 1;
         while (i < sizeof(colours) && colours[i][1] >= SMALL_AMOUNT)
            med += ({ colours[i++][0] });
         
         lo = ({ });
         while (i < sizeof(colours) && colours[i][1] >= VERY_SMALL_AMOUNT)
            lo += ({ colours[i++][0] });
         
         if (sizeof(med)) {
            liq_name += " with swirls of " + comma_list(med);
         }
         
         if (sizeof(lo)) {
            if (sizeof(med)) liq_name += " and faint streaks of " + comma_list(lo);
            else liq_name += " with faint streaks of " + comma_list(lo);
         }
         liq_name += ")";
      } else {   /* phew, coloured liquid containing names now */
         /* this little fragment _almost_ directly copied from no_names above */
         if (colours[0][1] < SMALL_AMOUNT) liq_name = "a faint " + colours[0][0];
         else liq_name = add_a(colours[0][0]);
         liq_name += " " + transparency_string(all_attrs[POTION_TRANSPARENCY]) +
                        " " + consistency_string(all_attrs[POTION_CONSISTENCY]);
         
         med = ({ });
         i = 0;
         while (i < sizeof(names) && names[i][1] >= VERY_SMALL_AMOUNT)
            med += ({ names[i++][0] });
         
         liq_name += " containing " + comma_list(med);
         
         med = ({ });
         i = 1;
         while (i < sizeof(colours) && colours[i][1] >= SMALL_AMOUNT)
            med += ({ colours[i++][0] });
         
         lo = ({ });
         while (i < sizeof(colours) && colours[i][1] >= VERY_SMALL_AMOUNT)
            lo += ({ colours[i++][0] });
         
         if (sizeof(med)) {
            liq_name += " with swirls of " + comma_list(med);
         }
         
         if (sizeof(lo)) {
            if (sizeof(med)) liq_name += " and faint streaks of " + comma_list(lo);
            else liq_name += " with faint streaks of " + comma_list(lo);
         }
      } /* yayayayaya.  done! */
   }
   return liq_name;
}

string query_liquid_short()
{
   if (sizeof(all_attrs[POTION_NAMES]) && sizeof(all_attrs[POTION_COLOURS])) {
      if (all_attrs[POTION_NAMES][0][1] > all_attrs[POTION_COLOURS][0][1] / 2) {
         return all_attrs[POTION_NAMES][0][0];
      }
   }
   
   return (sizeof(all_attrs[POTION_COLOURS]) ? 
           all_attrs[POTION_COLOURS][0][0] : "colourless") + " " +
             transparency_string(all_attrs[POTION_TRANSPARENCY]) + " " +             
             consistency_string(all_attrs[POTION_CONSISTENCY]);
}

/* ok modifyed back to using writes... */
/*        ^^^ lies!  all lies! */
/* Ok, Ok.  Sulk */
string long(string str, int dark) {
  string ret;
  int bing;

  ret = query_long();
  ret += calc_extra_look();
  if (trans != C_OPAQUE && closed != C_CLOSED)
    ret += query_contents(short(dark)+" contains:\n");
  /* what the hell is long_status, and why is it here? */
  ret += long_status();

/* the long for the water inside of it */

  if (trans != C_OPAQUE && closed != C_CLOSED && volume) {
    bing = (volume*8 + max_volume/16)/max_volume;
    if (bing <= 0)
      return ret + "It is slightly wet from "+liquid_name()+".\n";
    if (bing >= 8)
      return ret + "It is full to the brim with " + liquid_name()+".\n";
    return ret + "It is " + ({ "an eighth", "a quarter",
              "three eighths", "half", "five eighths", "three quarters",
              "seven eighths" })[bing-1]+" full of "+liquid_name()+".\n";
  }
  return ret;
}

/* ok parse command stuff */
string *parse_command_id_list() {
  return potion_id + ::parse_command_id_list();
} /* parse_command_id_list() */

/* string *parse_command_plural_id_list() { return plurals; } */
string *parse_command_adjectiv_id_list() {
  return potion_adjective + ::parse_command_adjectiv_id_list();
} /* parse_command_adjectiv_id_list() */

int sort_func(int *x, int *y)
{
   if (x[1] < y[1]) return 1;
   else return 0;
}

mixed *mix_liq(mixed *arr1, mixed *arr2, int vol1, int vol2, int tot_vol)
{
   int i, j;   /* general indexes into arrays */
   mixed *arr3;
   
   arr3 = allocate(sizeof(arr1));
 
   for (i = 0; i < sizeof(arr1); i++) {
      arr3[i] = ({ arr1[i][0], arr1[i][1] * vol1 / tot_vol });
      for (j = 0; j < sizeof(arr2); j++) {
         if (arr2[j][0] == arr1[i][0]) {
            arr3[i][1] += arr2[j][1] * vol2 / tot_vol;
            arr2[j][1] += 100000;         /* icky icky icky */
 /* have to be able to restore this because (due to the magic of
    arrays :( ) we are really working on the original */
            break;
         }
      }
   }
   
   for (i = 0; i < sizeof(arr2); i++) {
      if (arr2[i][1] >= 100000) {
         arr2[i][1] -= 100000;
      } else {
         arr3 += ({ ({ arr2[i][0], arr2[i][1] * vol2 / tot_vol }) });
      }
   }
   
   /* now lets sort it */
   arr3 = sort_array(arr3, "sort_func", this_object());
   /* now cull off 0's on the end */
   for (i = sizeof(arr3) - 1; i >= 0; i--) {
      if (arr3[i][1] > 0) break;
   }
   
   if (i < 0) arr3 = ({ });
   else arr3 = arr3[0..i];
   
   return arr3;
}
 
#ifdef BINGLEBONGLE
mixed *mix_liq(mixed *arr1, mixed *arr2, int vol1, int vol2, int tot_vol)
{
   int i, j;   /* general indexes into arrays */
   
   arr1 += ({ }); /* stop anything silly happening */
   arr2 += ({ });

   for (i = 0; i < sizeof(arr1); i++) {
      arr1[i][1] = arr1[i][1] * vol1 / tot_vol;
      for (j = 0; j < sizeof(arr2); j++) {
         if (arr2[j][0] == arr1[i][0]) {
            arr1[i][1] += arr2[j][1] * vol2 / tot_vol;
            arr2[j][0] = 0;
            break;
         }
      }
   }
   
   for (i = 0; i < sizeof(arr2); i++) {
      if (arr2[i][0]) {
         arr1 += ({ ({ arr2[i][0], arr2[i][1] * vol2 / tot_vol }) });
      }
   }
   
   /* now lets sort it */
   arr1 = sort_array(arr1, "sort_func", this_object());
   /* now cull off 0's on the end */
   for (i = sizeof(arr1) - 1; i >= 0; i--) {
      if (arr1[i][1] > 0) break;
   }
   
   if (i < 0) arr1 = ({ });
   else arr1 = arr1[0..i];
   
   return arr1;
}
#endif

mixed *merge_potions(mixed *potion1, mixed *potion2, int vol1, int vol2, int tot_vol)
{
   mixed *result;

   result = allocate(POTION_ATTRS_SIZE);
   result[POTION_CONSISTENCY] = 
      (potion1[POTION_CONSISTENCY] * vol1 +
       potion2[POTION_CONSISTENCY] * vol2) / tot_vol;
   result[POTION_TRANSPARENCY] = 
      (potion1[POTION_TRANSPARENCY] * vol1 +
       potion2[POTION_TRANSPARENCY] * vol2) / tot_vol;
   
   result[POTION_NAMES] = mix_liq(potion1[POTION_NAMES], potion2[POTION_NAMES],
                                  vol1, vol2, tot_vol);
   result[POTION_COLOURS] = mix_liq(potion1[POTION_COLOURS], potion2[POTION_COLOURS],
                                  vol1, vol2, tot_vol);
   result[POTION_FLAVOURS] = mix_liq(potion1[POTION_FLAVOURS], potion2[POTION_FLAVOURS],
                                  vol1, vol2, tot_vol);
   result[POTION_SMELLS] = mix_liq(potion1[POTION_SMELLS], potion2[POTION_SMELLS],
                                  vol1, vol2, tot_vol);
   return result;
}

void set_misc_attrs(mixed *new_misc_attrs, int misc_vol)
{
   misc_attrs = new_misc_attrs + ({ });
   volume = water + misc_vol;
   all_attrs = merge_potions(active_attrs, misc_attrs, water, volume - water, volume);
   update_potion_parse();
   if (leak_rate) set_heart_beat(1);
}

mixed *change_concentration(mixed *potion, int new_conc)
{
   mixed *newpotion;
   int i;
   
   newpotion = potion + ({ });
   newpotion[POTION_CONSISTENCY] = newpotion[POTION_CONSISTENCY] * new_conc / 100;
   newpotion[POTION_TRANSPARENCY] = 
      (100 - (100 - newpotion[POTION_TRANSPARENCY]) * new_conc / 100);
   for (i = 0; i < sizeof(newpotion[POTION_NAMES]); i++) {
      newpotion[POTION_NAMES][i][1] = newpotion[POTION_NAMES][i][1] * new_conc / 100;
   }
   for (i = 0; i < sizeof(newpotion[POTION_COLOURS]); i++) {
      newpotion[POTION_COLOURS][i][1] = newpotion[POTION_COLOURS][i][1] * new_conc / 100;
   }
   for (i = 0; i < sizeof(newpotion[POTION_SMELLS]); i++) {
      newpotion[POTION_SMELLS][i][1] = newpotion[POTION_SMELLS][i][1] * new_conc / 100;
   }
   for (i = 0; i < sizeof(newpotion[POTION_FLAVOURS]); i++) {
      newpotion[POTION_FLAVOURS][i][1] = newpotion[POTION_FLAVOURS][i][1] * new_conc / 100;
   }
   return newpotion;
}

void set_ps_coord_quantity(int *new_coord, int new_quantity)
{
   mixed *ps_attrs;
   
   ps_attrs = (mixed *)POTION_SPACE_HANDLER->query_attrs_at(new_coord);
   active_attrs = change_concentration(ps_attrs, new_quantity * 100 / water);
   ps_quantity = new_quantity;
   ps_coord = new_coord;
   all_attrs = merge_potions(active_attrs, misc_attrs, water, volume - water, volume);
   update_potion_parse();
   (void)POTION_SPACE_HANDLER->potion_create(this_object(), new_coord, new_quantity);
}

mixed stats() {
  return container::stats() + close_lock::stats() + ({
    ({ "ps_quantity", query_ps_quantity() }),
   ({ "ps_coord", sprintf("%O", ps_coord) }),
    ({ "volume", query_volume() }),
    ({ "max volume", query_max_volume() }),
    ({ "water vol", query_water_volume() }),
    ({ "volume left", query_volume_left() }),
    ({ "weight left", query_weight_left() }),
    ({ "full_weight", query_full_weight(), }),
    ({ "leak_rate", query_leak_rate() }),
  });
}

object query_parse_id(mixed *arr) {
  volume_to_womble = 0;
  return ::query_parse_id(arr);
}

object query_frac_parse_id(mixed *arr) {
  int i;

  volume_to_womble = 0;
  if (arr[P_THING] < 0) {
    arr[P_THING]++;
    if (arr[P_THING] != 0)
      return 0;
    volume_to_womble = volume*arr[P_TOP]/arr[P_BOT];
    call_out("set_volume_to_womble", 0, 0);
    arr[P_THING] = -1784628;
    return this_object();
  }
  if (arr[P_THING] == 0)
    if ((i=(arr[P_MAX_NUM]*arr[P_TOP])/arr[P_BOT]) > arr[P_CUR_NUM]++)
      return this_object();
    else
      if (i+1 == arr[P_CUR_NUM]) {
        volume_to_womble = volume*arr[P_TOP]/arr[P_BOT];
        call_out("set_volume_to_womble", 0, 0);
        return this_object();
      }
  else
    return 0;

  if ((i=(arr[P_THING]*arr[P_TOP])/arr[P_BOT]) > arr[P_CUR_NUM]++)
    return this_object();
  else
    if (i+1 == arr[P_CUR_NUM]) {
      volume_to_womble = volume*arr[P_TOP]/arr[P_BOT];
      call_out("set_volume_to_womble", 0, 0);
      return this_object();
    }
  return 0;
}

int remove_volume(int vol_lost)
{
   int q_lost;
   
   if (!volume) {
      q_lost = ps_quantity;
      water = 0;
      ps_quantity = 0;
      return q_lost;
   }
   q_lost = ps_quantity * vol_lost / volume;
   water -= water * vol_lost / volume;
   volume -= vol_lost;
   ps_quantity -= q_lost;
   return q_lost;
}

void heart_beat() {
  int lost, off;

   if (leak_rate == 0 || volume <= 0) {
      set_heart_beat(0);
      return;
   }
  if (hb_count--) return ;
  hb_count = 10;
  lost = leak_rate;
  if (lost > volume)
    lost = volume;
  off = lost/100;
  if (off > 10)
    off = 10;
  tell_room(environment(),
            capitalize(query_liquid_short())+({ " drips slowly",
                               " drips",
                               " dribbles",
                               " trickles slowly",
                               " trickles",
                               " trickles rapidly",
                               " pours sluggishly",
                               " pours",
                               " streams",
                               " gushes",
                               " fountains", })[off]+" out of the "+
                                                          short(1)+".\n");
  (void)remove_volume(lost);
  if (!volume) {
    set_heart_beat(0);
/*    set_liquid_name(0); */
    void_liquid();
  }
}

void transfer_liquid_to(object dest, int vol_misc, mixed *misc, 
                                     int vol_water, int *coord, int quantity) 
{
   mixed *their_attrs;
   int their_vol, their_water, their_quantity, new_quantity, new_misc_vol;
   int *new_coord, *old_coord;

   their_attrs = (mixed *)dest->query_misc_attrs();
   their_vol = (int)dest->query_volume();
   their_water = (int)dest->query_water_volume();
   their_quantity = (int)dest->query_ps_quantity();
   new_quantity = quantity + their_quantity;

   if (!their_attrs)  /* can't be a potion ... give up */
      return ;
   
   new_misc_vol = vol_misc + their_vol - their_water;

   (void)dest->set_water_volume(their_water + vol_water);
   if (new_misc_vol) {
      their_attrs = merge_potions(misc_attrs, their_attrs, 
                               vol_misc, their_vol - their_water,
                               new_misc_vol);
      (void)dest->set_misc_attrs(their_attrs, new_misc_vol);
   }
   
   old_coord = (int *)dest->query_ps_coord();
   new_coord = allocate(2);
/* simple averaging ... */
   if (pointerp(ps_coord) && new_quantity) {
      new_coord[0] = (ps_coord[0] * quantity + old_coord[0] * their_quantity) / 
                     new_quantity;
      new_coord[1] = (ps_coord[1] * quantity + old_coord[1] * their_quantity) / 
                     new_quantity;
      dest->set_ps_coord_quantity(new_coord, new_quantity);
   }

}

int ensure_open()
{
   if (locked) {
      write("The " + short(1) + " is locked.\n");
      return 0;
   }
   if (closed) /* has to be closed */
      if (do_open()) {
         write("You open the "+short(1)+".\n");
         return 1;
      } else {
         write("You can't open the " + short(1) + ".\n");
         return 0;
      }

   return 1;
}

int do_pour(object *dest, string me, string him, string prep)
{
   int amount_poured, volume_transferred, old_water, old_volume;
   string liquid_desc;

   if (sizeof(dest) > 1) {
      write("Currently you can only pour into one object\n");
      return 0;
   }
   
   if (!ensure_open()) return 0;

   if (volume <= 0) {
      write("The " + short(0) + " you are trying to pour out of is empty.\n");
      return 0;
   }

   old_water = water;
   old_volume = volume;  /* so that when volume becomes 0 in this process */

   liquid_desc = query_liquid_short();

   if (volume_to_womble && volume_to_womble <= empty_formula()) 
      volume_transferred = volume_to_womble;
   else
      volume_transferred = empty_formula();

   amount_poured = remove_volume(volume_transferred);
   
   if (volume_transferred > dest[0]->query_volume_left()) {
      say(this_player()->short(0) + " attempts to pour from " + add_a(short(0))
         + " into " + add_a(dest[0]->short(0)) + " but ends up spilling "
         + liquid_desc + " all over the ground.\n");
      write(capitalize(liquid_desc) + " spills on the floor as you try to pour "
         + "from the " + short(0) + " into the " + dest[0]->short(0) + ".\n");

      amount_poured = (amount_poured * ((int)dest[0]->query_volume_left())) /
                         volume_transferred;
      volume_transferred = (int)dest[0]->query_volume_left();
   }
   
   transfer_liquid_to(dest[0], volume_transferred * (old_volume - old_water) / old_volume,
      misc_attrs, volume_transferred * old_water / old_volume, ps_coord, amount_poured);
   
   this_player()->add_succeeded(dest[0]);
   return 1;
}

int do_fill(object *to, mixed *args_b, mixed *args_a, mixed *args) {
   int m, n, i, run_out, volume_needed, their_volume, their_max,
       amount_poured, ok;

   if (intp(args[1]) && intp(args[2])) {
      m = args[0];
      n = args[1];
      if (m > n || m < 0 || n <= 0) {
         notify_fail("Interesting fraction you have there!\n");
         return 0;
      }
   } else {
      m = 1;
      n = 1;
   }

   if (locked) {
      notify_fail("The " + short(0) + " is locked!\n");
      return 0;
   }
   
   if (closed) {
      if (do_open()) {
         write("You open the " + short(0) + ".\n");
      } else {
         write("You cannot open the " + short(0) + ".\n");
         return 0;
      }
   }

   if (volume <= 0) {
      write("The " + short(0) + " is bone dry!\n");
      return 0;
   }
   
   run_out = 0;
   for (i = 0; i < sizeof(to) && !run_out; i++) {
      their_volume = (int)to[i]->query_volume();
      their_max = (int)to[i]->query_max_volume();

      if (their_max <= 0) {
         write("The " + to[i]->short(0) + 
               " doesn't look like it can be filled!\n");
         continue;
      }
   
      if (their_volume >= their_max) {
         write("The " + to[i]->short(0) + " is full to the brim already.\n");
         continue;
      }
   
      volume_needed = their_max * m / n;
      if (their_volume >= volume_needed) {
         write("The " + to[i]->short(0) + " is more than " + m + "/" + n +
           " full already.\n");
         continue;
      }
  
      ok++; 
      volume_needed -= their_volume;
   
      if (volume_needed > empty_formula()) {
         write("You drain the " + short(0) + " into the " + to[i]->short(0) + 
            " but it is not enough.\n");
         volume_needed = empty_formula();
         run_out = 1;
         this_player()->add_succeeded(to[i]);
      } else {
         this_player()->add_succeeded(to[i]);
/*
         write("You pour from the " + short(0) + " into the " + 
                to[i]->short(0) + ".\n");
 */
      }
  /* 
      say(this_player()->query_cap_name() + " pours " + query_liquid_short() + 
         " from the " + short(0) + " into the " + to[i]->short(0) + ".\n");
   */ 
      amount_poured = volume_needed * ps_quantity / volume;
      transfer_liquid_to(to[i], volume_needed * (volume - water) / volume,
         misc_attrs, volume_needed * water / volume, ps_coord, amount_poured);
      amount_poured = remove_volume(volume_needed);
   }
   return ok;
} /* do_fill() */

int fill(string s)
{
   string x, y, z;
   int m, n, i;
   object *from, *to;
   int their_volume, their_max, amount_poured, run_out, volume_needed;

   notify_fail("fill container [m/n up|full] from|with container\n");
   if (!s) return 0;
   
   if (sscanf(s, "%s from %s", x, y) != 2) 
      if (sscanf(s, "%s with %s", x, y) != 2) return 0;
   
   if (sscanf(x, "%s %d/%d up", z, m, n) != 3)
      if (sscanf(x, "%s %d/%d full", z, m, n) != 3) {
         z = x;
         m = n = 1;
      }

   if (m > n || m < 0 || n <= 0) {
      notify_fail("Interesting fraction you have there!\n");
      return 0;
   }
   
   from = find_match(y, ({ this_player(), environment(this_player()) }) );
   to = find_match(z, ({ this_player(), environment(this_player()) }) );

   if (member_array(this_object(), from) < 0) {
      notify_fail("Can't find " + y + ".\n");
      return 0;
   }
   
   if (sizeof(from) > 1) {
      notify_fail("Can only pour from one container at a time.\n");
      return 0;
   }
   
   if (locked) {
      notify_fail("The " + short(0) + " is locked!\n");
      return 0;
   }
   
   if (closed) {
      if (do_open()) {
         write("You open the " + short(0) + ".\n");
      } else {
         write("You cannot open the " + short(0) + ".\n");
         return 1;
      }
   }

   if (volume <= 0) {
      write("The " + short(0) + " is bone dry!\n");
      return 1;
   }
   
   if (sizeof(to) == 0) {
      write("Can't find " + z + ".\n");
      return 1;
   }
   
   run_out = 0;
   for (i = 0; i < sizeof(to) && !run_out; i++) {
      their_volume = (int)to[i]->query_volume();
      their_max = (int)to[i]->query_max_volume();

      if (their_max <= 0) {
         write("The " + to[i]->short(0) + " doesn't look like it can be filled!\n");
         continue;
      }
   
      if (their_volume >= their_max) {
         write("The " + to[i]->short(0) + " is full to the brim already.\n");
         continue;
      }
   
      volume_needed = their_max * m / n;
      if (their_volume >= volume_needed) {
         write("The " + to[i]->short(0) + " is more than " + m + "/" + n +
           " full already.\n");
         continue;
      }
   
      volume_needed -= their_volume;
   
      if (volume_needed > empty_formula()) {
         write("You drain the " + short(0) + " into the " + to[i]->short(0) + 
            " but it is not enough.\n");
         volume_needed = empty_formula();
         run_out = 1;
         if (i + 2 == sizeof(to)) 
            write("The " + to[i+1] + " remains unfilled.\n");
         else if (i + 2 < sizeof(to))
            write("The " + query_multiple_short(to[i+1..sizeof(to)-1]) + 
                  " remain unfilled.\n");
      } else {
         write("You pour from the " + short(0) + " into the " + to[i]->short(0) + ".\n");
      }
   
      say(this_player()->query_cap_name() + " pours " + query_liquid_short() + 
         " from the " + short(0) + " into the " + to[i]->short(0) + ".\n");
   
      amount_poured = volume_needed * ps_quantity / volume;
      transfer_liquid_to(to[i], volume_needed * (volume - water) / volume,
         misc_attrs, volume_needed * water / volume, ps_coord, amount_poured);
      amount_poured = remove_volume(volume_needed);
   }
   return 1;
}

int do_drink(object *dest, string me, string him, string prep)
{
   int amount_drunk;

   if (sizeof(dest)) {
      write("Drinking is a very simple operation - please don't complicate matters.\n");
      return 0;
   }
   
   if (!ensure_open()) return 0;

   if (volume <= 0) {
      write("The " + short(0) + " is bone dry!\n");
      return 0;
   }
   
  /* should do some fudging to add +/- 5 mls or something */
  /* possibly skill/stat dependant */
   
   if (volume_to_womble && volume_to_womble < empty_formula())
      amount_drunk = remove_volume(volume_to_womble);
   else
      amount_drunk = remove_volume(empty_formula());
  
   (void)POTION_SPACE_HANDLER->potion_drunk(this_player(), ps_coord, amount_drunk);
   return 1;
}

int do_empty(object *dest, string me, string him, string prep)
{
   if (sizeof(dest)) {
      write("Passing on to pour ... bad move.\n");
      return do_pour(dest, me, him, prep);
   }
/* this completely fails to work :( ^^^ */
   
   if (!ensure_open()) { write("It aint open!\n"); return 0; }

   if (volume == 0) {
      write("The " + short(0) + " is already empty.\n");
      return 0;
   }

   if (volume_to_womble && volume_to_womble <= empty_formula())
      (void)remove_volume(volume_to_womble);
   else
      (void)remove_volume(empty_formula());
   
/* should check spillage */
   return 1;
}

int do_splash(object *dest, string me, string him, string prep)
{
/* note that spashing needs to be changed to make the amount that 
   actually gets to the targe be related to some skill ... */
   int amount_splashed;

   if (sizeof(dest) > 1) {
      write("You can only splash one object at a time.\n");
      return 0;
   }

   if (!sizeof(dest))
     return 0;
   
   if (!ensure_open()) return 0;

   if (volume_to_womble && volume_to_womble <= empty_formula()) 
      amount_splashed = remove_volume(volume_to_womble);
   else
      amount_splashed = remove_volume(empty_formula());
   
   (void)POTION_SPACE_HANDLER->potion_touch(dest[0], ps_coord, amount_splashed);
   (void)this_player()->add_succeeded(dest[0]);
   return 1;
}

int do_rub(object *dest, string me, string him, string prep)
{
   int amount_rubbed;

   if (sizeof(dest) > 1) {
      write("You can only "+query_verb()+" on to one object at a time.\n");
      return 0;
   }

   if (!sizeof(dest))
     return 0;

   if (environment(dest[0]) != this_player()) {
      write("You can only "+query_verb()+" on to an object in your inventory.\n");
      return 0;
   }

   if (!ensure_open()) return 0;

   if (volume_to_womble && volume_to_womble <= empty_formula())
      amount_rubbed = remove_volume(volume_to_womble);
   else
      amount_rubbed = remove_volume(empty_formula());

   (void)POTION_SPACE_HANDLER->potion_touch(dest[0], ps_coord, amount_rubbed);
   (void)this_player()->add_succeeded(dest[0]);
   return 1;
}

do_apply(x, y, z, a, b, c)
{
   do_rub(x, y, z, a, b, c);
}

#define TASTE_AMOUNT 5
int do_taste()
{
   int amount_tasted;
   string desc;
   mixed *group;
   int i;
   /* be kind to tasters! */
 
   if (!ensure_open()) return 0;
 
   if (volume < TASTE_AMOUNT) {
      write("There is no liquid to taste.\n");
      return 0;
   }
   
   desc = "The " + query_liquid_short();
   if (!sizeof(all_attrs[POTION_FLAVOURS]) || 
     all_attrs[POTION_FLAVOURS][0][1] < VERY_SMALL_AMOUNT) 
      desc += " has no discernible flavour";
   else {
      group = ({ });
      for (i = 0; i < sizeof(all_attrs[POTION_FLAVOURS]) && 
           all_attrs[POTION_FLAVOURS][i][1] >= SMALL_AMOUNT; i++)
         group += all_attrs[POTION_FLAVOURS][i][0..0];
      
      if (sizeof(group)) {
         desc += " tastes of " + comma_list(group);
         if (i < sizeof(all_attrs[POTION_FLAVOURS]) && 
             all_attrs[POTION_FLAVOURS][i][1] >= VERY_SMALL_AMOUNT)
            desc += " with a faint hint of ";
      } else desc += " tastes faintly of ";
      
      group = ({ });
      for (; i < sizeof(all_attrs[POTION_FLAVOURS]) && 
           all_attrs[POTION_FLAVOURS][i][1] >= VERY_SMALL_AMOUNT; i++)
         group += all_attrs[POTION_FLAVOURS][i][0..0];
      
      if (sizeof(group)) desc += comma_list(group);
   }
   write(desc + ".\n");
   say(this_player()->query_cap_name() + " takes a small sip from the " +
       short(0) + ".\n");
   
   amount_tasted = remove_volume(TASTE_AMOUNT);
   (void)POTION_SPACE_HANDLER->potion_drunk(this_player(), ps_coord, TASTE_AMOUNT);
   return 1;
}
 
int do_smell()
{
   string desc;
   string *group;
   int i;
   /* be kind to smellers! */
   
   if (!ensure_open()) return 0;
 
   if (volume < 1) {
      write("There is no liquid to smell.\n");
      return 0;
   }
   
   desc = "The " + query_liquid_short();
   if (!sizeof(all_attrs[POTION_SMELLS]) || 
     all_attrs[POTION_SMELLS][0][1] < VERY_SMALL_AMOUNT) 
      desc += " has no distinct aroma";
   else {
      group = ({ });
      for (i = 0; i < sizeof(all_attrs[POTION_SMELLS]) && 
           all_attrs[POTION_SMELLS][i][1] >= SMALL_AMOUNT; i++)
         group += all_attrs[POTION_SMELLS][i][0..0];
      
      if (sizeof(group)) {
         desc += " smells of " + comma_list(group);
         if (i < sizeof(all_attrs[POTION_SMELLS]) && 
             all_attrs[POTION_SMELLS][i][1] >= VERY_SMALL_AMOUNT)
            desc += " with a faint hint of ";
      } else desc += " smells faintly of ";
      
      group = ({ });
      for (; i < sizeof(all_attrs[POTION_SMELLS]) && 
           all_attrs[POTION_SMELLS][i][1] >= VERY_SMALL_AMOUNT; i++)
         group += all_attrs[POTION_SMELLS][i][0..0];
      
      if (sizeof(group)) desc += comma_list(group);
   }
   write(desc + ".\n");
   say(this_player()->query_cap_name() + " takes a whiff of the " +
       short(0) + ".\n");
   
   (void)POTION_SPACE_HANDLER->potion_smell(this_player(), ps_coord, ps_quantity * 100 / volume);
   return 1;
}
 
mapping int_query_static_auto_load() {
  mapping tmp;

  tmp = ::int_query_static_auto_load();
  return ([ "::" : tmp, "leak rate" : leak_rate, "full weight" : full_weight,
            "trans" : trans, "difficulty" : difficulty,
            "key" : key, "trap open func" : trap_open_func, 
            "trap lock func" : trap_lock_func, "unlock skill" : unlock_skill,
            "trap open ob" : trap_open_ob, "trap lock ob" : trap_lock_ob, 
            "max volume" : max_volume,
          ]);
}

mapping query_dynamic_auto_load() {
  return ([
           "::" : ::query_dynamic_auto_load(),
           "locked" : locked,
           "stuck" : stuck,
           "closed" : closed,
           "volume" : volume,
           "water" : water,
           "misc attrs" : misc_attrs,
           "ps coord" : ps_coord,
           "ps quantity" : ps_quantity,
          ]);
} /* query_dynamic_auto_load() */

void init_dynamic_arg(mapping map) {
  if (map["::"])
    ::init_dynamic_arg(map["::"]);
  locked = map["locked"];
  stuck = map["stuck"];
  closed = map["closed"];
  volume = map["volume"];
  set_water_volume(map["water"]);
  if (volume == 0) {
    void_liquid();
    update_potion_parse();
    return;
  }
  if (map["misc attrs"])
    set_misc_attrs(map["misc attrs"], volume - water);
  if (map["ps coord"])
    set_ps_coord_quantity(map["ps coord"], map["ps quantity"]);
  update_potion_parse();
} /* init_dynamic_arg() */

void init_static_arg(mapping args) {
  if (args["::"])
    ::init_static_arg(args["::"]);
  if (!undefinedp(args["leak rate"]))
    leak_rate = args["leak rate"];
  if (!undefinedp(args["full weight"]))
    full_weight = args["full weight"];
  if (!undefinedp(args["trans"]))
    trans = args["trans"];
  if (!undefinedp(args["difficuly"]))
    difficulty = args["difficulty"];
  if (!undefinedp(args["key"]))
    key = args["key"];
  if (!undefinedp(args["trap open func"]))
    trap_open_func = args["trap open func"];
  if (!undefinedp(args["trap lock func"]))
    trap_lock_func = args["trap lock func"];
  if (!undefinedp(args["unlock skill"]))
    unlock_skill = args["unlock skill"];
  if (!undefinedp(args["trap open ob"]))
    trap_open_ob = args["trap open ob"];
  if (!undefinedp(args["trap lock ob"]))
    trap_lock_ob = args["trap lock ob"];
  if (!undefinedp(args["max volume"]))
    max_volume = args["max volume"];
} /* init_static_arg() */

/*
 * Do it all the time for containers...  They could have things in them.
 * too hard to andle rhe special cases you know...
 */
mixed query_static_auto_load() {
  if (file_name(this_object())[0..13] == "/obj/container")
    return int_query_static_auto_load();
  return ([ ]);
} /* query_static_auto_load() */
