/* the standard race... */
inherit "/std/object";

mixed *bits;
/* The amazing 'bits' array :
 * at the mo has 3 elements per bit:
 * name, alias, array
 * where the array has the form:
 * bit_come_from, weight, general_value, list_of_bits_on_this_bit
 * All incredibly gruesome hey?
 */

void add_bit(string name, string alias, mixed *varr)
{
   int i;

   i = member_array(name, bits);
   if (i >= 0) {
      if (pointerp(bits[i+1])) i--;
      bits[i] = name;
      bits[i+1] = alias;
      bits[i+2] = varr;
      return;
   }
   bits += ({ name, alias, varr });
   return;
}

void create() {
  ::create();
   bits = ({ });
  set_long("The standard boring race.  Get a new one soon, this one is "+
           "pretty sad.\n");
   set_name("strange");
  reset_get();
   add_bit("head", "", ({ 40, 0, 
      "left ear", "right ear", "left eye", "right eye", "nose", "scalp",
      "tongue", "teeth", "skull" }) );
   add_bit("left ear", "ear", ({ "head", 1, 0 }));
   add_bit("right ear", "ear", ({ "head", 1, 0 }));
   add_bit("left eye", "eye", ({ "head", 1, 0 }));
   add_bit("nose", "", ({ "head", 1, 0 }));
   add_bit("scalp", "", ({ "head", 3, 0 }));
   add_bit("tongue", "", ({ "head", 2, 0 }));
   add_bit("teeth", "", ({ "head", 2, 0 }));
   add_bit("skull", "cranium", ({ "head", 20, 0 }));
   add_bit("left arm", "arm", ({ 0, 80, 0, "left hand" }));
   add_bit("right arm", "arm", ({ 0, 80, 0, "right hand" }));
   add_bit("torso", "chest", ({ 0, 400, 0, "heart", "liver", "left kidney",
      "right kidney", "left lung", "right lung", "spleen" }));
   add_bit("left hand", "hand", ({ "left arm", 15, 0, "left pinky", 
      "left third finger", "left index finger", "left ring finger",
      "left thumb" }));
   add_bit("right hand", "hand", ({ "right arm", 15, 0, "right pinky",
      "right third finger", "right index finger", "right ring finger",
      "right thumb" }));
   add_bit("left pinky", "finger", ({ "left hand", 1, 0 }));
   add_bit("left third finger", "finger", ({ "left hand", 1, 0 }));
   add_bit("left index finger", "finger", ({ "left hand", 1, 0 }));
   add_bit("left ring finger", "finger", ({ "left hand", 1, 0 }));
   add_bit("left thumb", "finger", ({ "left hand", 1, 0 }));
   add_bit("right pinky", "finger", ({ "right hand", 1, 0 }));
   add_bit("right third finger", "finger", ({ "right hand", 1, 0 }));
   add_bit("right index finger", "finger", ({ "right hand", 1, 0 }));
   add_bit("right ring finger", "finger", ({ "right hand", 1, 0 }));
   add_bit("right thumb", "finger", ({ "right hand", 1, 0 }));
   add_bit("heart", "", ({ "torso", 10, 0 }));
   add_bit("liver", "", ({ "torso", 5, 0 }));
   add_bit("left kidney", "kidney", ({ "torso", 5, 0 }));
   add_bit("right kidney", "kidney", ({ "torso", 5, 0 }));
   add_bit("left lung", "lung", ({ "torso", 5, 0 }));
   add_bit("right lung", "lung", ({ "torso", 5, 0 }));
   add_bit("spleen", "", ({ "torso", 5, 0 }));
   add_bit("genitals", "", ({ 0, 5, 0 }));  /* sex ???? */
   add_bit("left leg", "leg", ({ 0, 300, 0, "left foot" }));
   add_bit("right leg", "leg", ({ 0, 300, 0, "right foot" }));
   add_bit("left foot", "foot", ({ "left leg", 50, 0 })); /* toes ... god */
   add_bit("right foot", "foot", ({ "right leg", 50, 0 }));
}

/* armour querying one... I forget what it is called */
int query_number_type(string type) {
  if (type == "ring")
    return 2;
  return 1;
}
 
/* number of weapons allowed to be wielded */
int query_number_wielded() {
  return 1;
}
 
/* race description used on the player object...
 * There will be a race varible on the player object which allows
 * the race to keep needed information on it
 */
string query_race_description() { return "A small non descript person."; }
 
/* bonuses and minuses on given skills.
 */
int query_skill_bonus(int lvl, string skill) {
  return 0; /* the standard race has no modifications */
}
 
/* when the player logs on this function is called.
 * it allows you to start up the player with whatever he needs.
 */
int player_start(object player) {
   return 1;
}
 
/* and guess what this does?
 * Beware... If some is straight dested this will not be called,
 * So remember that you still must check your array for null members.
 */
int player_quit(object player) {
  return 1;
}

int query_dark(int light) {
  if (light < 20)
    return 1;
}

/*
 * is used by monsters...
 * You can trpa this and set up anything that a monster might get at 
 * certain level in this race
 */
void set_level(int lvl) {
}

mixed *query_bits()
{
   return bits;
}

mixed *query_bit(string s)
{
   int i;

   i = member_array(s, bits);
   if (i < 0) return ({ });
   if (pointerp(bits[i+1])) i--;
   return ({ bits[i], bits[i+1], bits[i+2] });
}
