#include "drinks.h"
/* ok handles hps, Spell/Power points and drinking/eating things...
 * Hmmmm, considering we dont want to have drinks/food I will not 
 * put that in at the moment.
 */
int max_hp,
    max_gp,
    total_xp,
    wimpy;
int hp,
    xp,
    gp;
private int *drink_info;
static mapping damage_done;

void create() {
  damage_done = ([ ]);
} /* create() */

int query_hp() { return hp; }
int query_max_hp() { return max_hp; }
int query_gp() { return gp; }
int query_max_gp() { return max_gp; }
int query_xp() { return xp; }
int query_total_xp() { return total_xp; }
mapping query_damage_done() { return damage_done; }
int set_wimpy(int i) {
  if (i <0 || i > 100)
    return -1;
  return (wimpy = i);
} /* set_wimpy() */

int query_wimpy() { return wimpy; }

int set_max_hp(int i) {
  if (max_hp)
    hp = hp*i/max_hp;
  else
    hp = max_hp;
  max_hp = i;
  if (hp>max_hp)
    hp = max_hp;
  return max_hp;
} /* set_max_hp() */

int set_hp(int i, object attacker) {
  if (i>max_hp)
    i = max_hp;
  hp = i;
  if (hp<0)
    call_out("do_death", 0, this_player());
  return hp;
} /* set_hp() */

int adjust_hp(int i, object attacker) {
  if (max_hp < 2) return hp;
  hp += i;
  damage_done[this_player()] += i;
  if (hp>max_hp)
    hp = max_hp;
  if (hp<0) /* eeek we died! */
    call_out("do_death", 0, this_player());
  return hp;
} /* adjust_hp() */

int set_max_gp(int i) {
  if (max_gp)
    gp = gp*i/max_gp;
  else
    gp = max_gp;
  max_gp = i;
  if (gp>max_gp)
    gp = max_gp;
  return max_gp;
} /* set_max_gp() */

int set_gp(int i) {
  gp = i;
  if (gp > max_gp)
    gp = max_gp;
  return gp;
} /* set_gp() */

int adjust_gp(int i) {
  if(gp + i < 0)
    return -1;
  gp += i;
  if (gp>max_gp)
    gp = max_gp;
  return gp;
} /* adjust_gp() */

int adjust_xp(int i) {
  if (i>0) {
    i = (100-wimpy)*i/100;
    total_xp += i;
  }
  xp += i;
/*
  if (interactive(this_object()))
    add_worth2(i,
          (string)"/secure/master"->creator_file(file_name(previous_object())));
*/
  return xp;
} /* adjust_xp() */

int adjust_volume(int type, int amt) {
  if (!pointerp(drink_info))
    drink_info = allocate(D_SIZEOF);
  if (type >= D_SIZEOF)
    return 0;
  return drink_info[type] += amt;
} /* adjust_volume() */

int query_volume(int type) {
  if (!pointerp(drink_info))
    drink_info = allocate(D_SIZEOF);
  if (type >= D_SIZEOF)
    return 0;
  return drink_info[type];
} /* query_volume() */

void update_volumes() {
  int i;

  if (!drink_info)
    return ;
  for (i=0;i<sizeof(drink_info);i++)
    if (drink_info[i] > 0)
      drink_info[i]--;
    else
      drink_info[i] = 0;
} /* update_volumes() */

string health_string() {
 if (hp < max_hp/10)
   return "is in very bad shape";
 if (hp < max_hp/5)
   return "is in bad shape";
 if (hp < max_hp/2)
   return "is not in a good shape";
 if (hp < max_hp - 200)
   return "is slightly hurt";
 return "is in good shape";
} /* health_string() */
