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
   set_long("The standard boring race.  Get a new one soon, this one is "+
            "pretty sad.\n");
   reset_get();
   bits = ({
      "head", "", ({ 0, 40, 0, 
         "left ear", "right ear", "left eye", "right eye", "nose", "scalp",
         "tongue", "teeth", "skull" }),
      "left ear", "ear", ({ "head", 1, 0 }),
      "right ear", "ear", ({ "head", 1, 0 }),
      "left eye", "eye", ({ "head", 1, 0 }),
      "right eye", "eye", ({ "head", 1, 0 }),
      "nose", "", ({ "head", 1, 0 }),
      "scalp", "", ({ "head", 3, 0 }),
      "tongue", "", ({ "head", 2, 0 }),
      "teeth", "", ({ "head", 2, 0 }),
      "skull", "cranium", ({ "head", 20, 0 }),
      "left arm", "arm", ({ 0, 80, 0, "left hand" }),
      "right arm", "arm", ({ 0, 80, 0, "right hand" }),
      "torso", "chest", ({ 0, 400, 0, "heart", "liver", "left kidney",
         "right kidney", "left lung", "right lung", "spleen" }),
      "left hand", "hand", ({ "left arm", 15, 0, "left pinky", 
         "left third finger", "left index finger", "left ring finger",
         "left thumb" }),
      "right hand", "hand", ({ "right arm", 15, 0, "right pinky",
         "right third finger", "right index finger", "right ring finger",
         "right thumb" }),
      "left pinky", "finger", ({ "left hand", 1, 0 }),
      "left third finger", "finger", ({ "left hand", 1, 0 }),
      "left index finger", "finger", ({ "left hand", 1, 0 }),
      "left ring finger", "finger", ({ "left hand", 1, 0 }),
      "left thumb", "finger", ({ "left hand", 1, 0 }),
      "right pinky", "finger", ({ "right hand", 1, 0 }),
      "right third finger", "finger", ({ "right hand", 1, 0 }),
      "right index finger", "finger", ({ "right hand", 1, 0 }),
      "right ring finger", "finger", ({ "right hand", 1, 0 }),
      "right thumb", "finger", ({ "right hand", 1, 0 }),
      "heart", "", ({ "torso", 10, 0 }),
      "liver", "", ({ "torso", 5, 0 }),
      "left kidney", "kidney", ({ "torso", 5, 0 }),
      "right kidney", "kidney", ({ "torso", 5, 0 }),
      "left lung", "lung", ({ "torso", 5, 0 }),
      "right lung", "lung", ({ "torso", 5, 0 }),
      "spleen", "", ({ "torso", 5, 0 }),
      "genitals", "", ({ 0, 5, 0 }),  /* sex ????  ohhh yes*/
      "left leg", "leg", ({ 0, 300, 0, "left foot" }),
      "right leg", "leg", ({ 0, 300, 0, "right foot" }),
      "left foot", "foot", ({ "left leg", 50,  "left big toe",
             "left second toe", "left third toe",
             "left fourth toe", "left little toe" }), /* toes ... god */
      "right foot", "foot", ({ "right leg", 50, "right big toe",
             "right second toe", "right third toe",
             "right fourth toe", "right little toe" }), /* toes ... god */
      "right little toe", "toe", ({ "right foot", 1, 0 }),
      "right second toe", "toe", ({ "right foot", 1, 0 }),
      "right third toe", "toe", ({ "right foot", 1, 0 }),
      "right fourth toe", "toe", ({ "right foot", 1, 0 }),
      "right big toe", "toe", ({ "right foot", 1, 0 }),
      "left little toe", "toe", ({ "left foot", 1, 0 }),
      "left second toe", "toe", ({ "left foot", 1, 0 }),
      "left third toe", "toe", ({ "left foot", 1, 0 }),
      "left fourth toe", "toe", ({ "left foot", 1, 0 }),
      "left big toe", "toe", ({ "left foot", 1, 0 }),
   });
  ::create();
  set_name("unknown creature");
}

/* armour querying one... I forget what it is called */
int query_number_type(string type) {
  if (type == "ring")
    return 2;
  if (type == "decorative")
    return 10;
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
string query_desc(object ob) { return "A small non descript person.\n"; }
 
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
void set_level(object ob, int lvl) {
  call_out("new_set_level",0, ({ lvl, ob }));
} /* set_level() */

void new_set_level(mixed *junk) {
  int lvl;

  lvl = junk[0];
  if (lvl >= 0) {
    junk[1]->add_attack("standard", 0, 100, 20+lvl, 0, 0, "blunt");
    junk[1]->add_ac("standard blunt", "blunt", 10+lvl/2);
    junk[1]->add_ac("standard sharp", "sharp", 10+lvl/2);
    junk[1]->add_ac("standard pierce", "piece", 10+lvl/2);
  } else {
    junk[1]->add_attack("standard", 0, 100, 20/(-lvl), 0, 0, "blunt");
    junk[1]->add_ac("standard blunt", "blunt", 10/-lvl);
    junk[1]->add_ac("standard sharp", "sharp", 10/-lvl);
    junk[1]->add_ac("standard piece", "pierce", 10/-lvl);
  }
} /* new_set_level() */

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

query_possible_bits(s)
{
   int i;
   string *poss;

   poss = ({ });
   for (i = 0; i < sizeof(bits); i+=3) {
      if (bits[i] == s || bits[i+1] == s) poss += ({ bits[i] });
   }
   return poss;
}
