/* the standard race... */
inherit "/std/object";

void create() {
  ::create();
  set_long("The standard boring race.  Get a new one soon, this one is "+
           "pretty sad.\n");
  reset_get();
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
void set_level(int lvl) {
}
