inherit "/std/basic/print_object";
/* Ok armour...
 * This will be nasty...
 */
#include "weapon.h"
static mixed ac;
static mixed armour_types;

void create() {
  ac = ({ });
  armour_types = ({ });
} /* create() */
 
/* set the ac for the type directly.... */
int add_ac(string name, string type, mixed a_c) {
  int i;
 
  if (!stringp(type))
    return 0;
 
  if ((i=member_array(name,ac)) != -1)
    return 0;
  ac += ({ name, ({ a_c, type }) });
  if ((i=member_array(type, armour_types)) == -1)
    armour_types += ({ type, ({ sizeof(ac)-2 }) });
  else
    armour_types[i+1] += ({ sizeof(ac)-2 });
  return 1;
} /* add_ac() */

int remove_ac(string name) {
  int i, j, k;

  if ((i=member_array(name, ac)) == -1)
    return 0;
  j = member_array(ac[i+1][1], armour_types);
  k = member_array(i, armour_types[j+1]);
  armour_types[j+1] = delete(armour_types[j+1], k, 1);
  ac = delete(ac, i, 2);
  return 1;
} /* remove_ac() */

int calc_value(mixed arr) {
  int i, val;
 
  if (intp(arr))
    return random(arr);
  if (!pointerp(arr))
    return 0;
  if (sizeof(arr) == 1)
    return arr[F_FIXED];
  if (sizeof(arr) == 2)
    return arr[F_FIXED]+random(arr[F_RAND]);
  for (i=0;i<arr[F_NUM];i++)
    val += random(arr[F_DIE]);
  return val + arr[F_FIXED];
} /* calc_value() */
 
/* ok this gets our ac... ;) */
int query_ac(string type, int dam) {
  int val,
      i, j;
 
  if ((i=member_array(type, armour_types)) != -1)
    for (j=0;j<sizeof(armour_types[i+1]);j++)
      val += calc_value(ac[armour_types[i+1][j]+1][A_AC]);
  return val;
} /* query_ac() */

mixed *query_armour_class() { return ac; }

string calc_string(mixed b) {
  if (intp(b))
    return "rand("+b+")";
  if (!pointerp(b))
    return "Dead";
  switch (sizeof(b)) {
    case 1 :
      return ""+b[0];
    case 2 :
      return ""+b[0]+"+rand("+b[1]+")";
    case 3 :
      return ""+b[0]+"+"+b[1]+"d"+b[2];
    default :
      return "Oh hell";
  }
} /* calc_string() */

mixed *stats() {
  int i, j;
  mixed *ret;

  ret = ({ });
  for (i=0;i<sizeof(ac);i+=A_ARRAY_SIZE,j++)
    ret += ({ ({ "ARM"+j+" name", ac[i], }),
              ({ "     type", ac[i+1][A_TYPE], }),
              ({ "    class", calc_string(ac[i+1][A_AC]) }),
          });
  return ret;
} /* stats() */

void set_ac(mixed *bing) {
  int i;

  for (i=0;i<sizeof(ac);i+=A_ARRAY_SIZE)
    add_ac(ac[i], ac[i+1][A_TYPE], ac[i+1][A_AC]);
} /* set_ac() */

mixed *query_init_data() {
  return ({ "armour class", ac, "set_ac/p" });
} /* query_init_data() */
