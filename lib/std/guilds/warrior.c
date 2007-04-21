#include <player_handler.h>
#include <top_ten_tables.h>
#include <playtesters.h>

#define SAVE_FILE ( "/save/guilds/warriors/warrior_ob" )

inherit "/std/guilds/standard";

void load_file();
void save_file();

void setup() {
  set_name("warriors");
  set_short("Warriors' Guild");
  set_long("The Warriors' Guild.\n\n"
           "The Warriors' Guild is for the brave and "
           "heroic.  As a member you can expect to "
           "recieve a sound background in beating, "
           "bashing, slicing, and dicing a variety of "
           "opponents up with a wide selection of "
           "weapons.  The evaluation of arms and armour "
           "will also be covered in detail.  Join this "
           "guild if you wish to journey to interesting "
           "places, meet interesting people, and give "
           "them a sound thrashing.\n");
  set_gp_skill("fighting.points");
  set_start_pos("/d/dist/pumpkin/squash/squash5");
  set_skills(({  "fighting.points",
                 "fighting.combat.melee.sharp",
                 "fighting.combat.melee.pierce",
                 "fighting.combat.melee.blunt",
                 "fighting.combat.melee.unarmed",
                 "fighting.combat.range.thrown",
                 "fighting.combat.range.fired",
                 "fighting.combat.parry.melee",
                 "fighting.combat.parry.range",
                 "fighting.combat.dodging.melee",
                 "fighting.combat.dodging.range",
                 "fighting.combat.special.weapon",
                 "fighting.combat.special.unarmed",
                 "other.evaluating.weapons",
                 "other.evaluating.armour"  }));
} /* setup() */


int query_skill_cost(string skill) {
  string *bits;
  if(member_array(skill, skills) != -1)
    return 3;
  bits = explode(skill, ".");
  switch(bits[0]) {
    case "faith" :
      return 15;
    case "fighting" :
      return 5;
    case "magic" :
      return 50;
    default :
      return 10;
  }
} /* query_skill_cost() */

int query_skill_max_level( string skill ) {
  string *bits;
  if(member_array(skill, skills) != -1)
    return 300;
  bits = explode(skill, ".");
  switch (bits[0]) {
    case "faith" :
      return 25;
    case "fighting" :
      return 50;
    case "magic" :
      return 5;
    case "other" :
      return 25;
    default :
      return 10;
  }
} /* query_skill_max_level() */

string query_title( object player ) {
  int lvl, female;
  female = (int)player->query_gender() == 2;
  lvl = query_level( player );
  switch( lvl ) {
      case 0..15: return "the Novice Fighter";
      case 16..30: return "the Apprentice Fighter";
      case 31..45: return "the Skillful Fighter";
      default: return "the Grave of All Hope";
    }
} /* query_title() */

/**
 *Method is used to see if a player is a warrior
 *@param player name of player
 */
int query_warrior( string player ) {
  if( PLAYER_HANDLER->test_guild( player ) == "/std/guilds/warrior" ) {
    return 1;
  }
  return 0;
}

/**
 *Method is used to see if a player is a warrior
 *@param player player object
 */
int query_warrior_ob( object player ) {
  if( player->query_guild_ob() == "/std/guilds/warrior" ) {
    return 1;
  }
  return 0;
}
