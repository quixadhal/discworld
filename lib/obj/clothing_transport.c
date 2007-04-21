/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: clothing_transport.c,v 1.1 2000/07/14 22:08:51 pinkfish Exp $
 * $Log: clothing_transport.c,v $
 * Revision 1.1  2000/07/14 22:08:51  pinkfish
 * Initial revision
 *
 * Revision 1.8  1999/11/02 20:04:30  pinkfish
 * Fix up the formatting a little.
 *
 * Revision 1.7  1999/10/25 12:45:16  dragonkin
 * Can't remember...
 *
 * Revision 1.6  1999/05/07 01:44:43  dragonkin
 * Added cloner support
 *
 * Revision 1.5  1999/05/06 17:32:55  dragonkin
 * Added support for the virtuals handler
 *
 * Revision 1.4  1998/05/14 18:22:55  pinkfish
 * Fix up the static arg stuff.
 *
 * Revision 1.3  1998/03/12 08:06:57  pinkfish
 * Added documentation.
 *
 * Revision 1.2  1998/02/27 04:56:32  pinkfish
 * CHanged to stop people being able to pick up
 * pigns and stuff and put them into
 * containers.
 *
 * Revision 1.1  1998/01/06 04:58:17  ceres
 * Initial revision
 * 
*/
/**
 * This is the clothing file for transports.
 * @author Pinkfish
 * @started Wed Nov 17 00:39:37 PST 1999
 * @see /std/npc/types/transport.c
 */
inherit "/obj/clothing";

private int _mount_difficulty_modifier;
private int _direction_difficulty_modifier;
private string _race;

void create() {
   clothing::create();
} /* create() */

/**
 * This method sets the race the clothing is for.
 * @param race the race that the clothing is for
 * @return the race the clothing is for
 */
void set_worn_by_race(string race) {
   _race = race;
} /* set_worn_by_race() */

/**
 * This method queries the race the clothing is for.
 * @return the race the clothing is for
 */
string query_worn_by_race() {
   return _race;
} /* query_worn_by_race() */

/**
 * This method set the mount difficulty modifier.
 * @param diff the mount difficulty modifier
 */
void set_mount_difficulty_modifier(int diff) {
   _mount_difficulty_modifier = diff;
} /* set_moun_difficult_modifier() */

/**
 * This method queries the mount difficulty modifier.
 * @return the mount difficulty modifier
 */
int query_mount_difficulty_modifier() {
   return _mount_difficulty_modifier;
} /* query_mount_difficulty_modifier() */

/**
 * This method sets the direction difficulty modifier.
 * @param diff the direction difficulty modifier
 */
void set_direction_difficulty_modifier(int diff) {
   _direction_difficulty_modifier = diff;
} /* set_direction_difficulty_modifier() */

/**
 * This method queries the direction difficulty modifier.
 * @return the direction difficulty modifier
 */
int query_direction_difficulty_modifier() {
   return _direction_difficulty_modifier;
} /* query_direction_difficulty_modifier() */

/** @ignore yes */
mapping int_query_static_auto_load() {
   return ([
      "::" : clothing::int_query_static_auto_load(),
      "worn by race" : _race,
      "mount difficulty" : _mount_difficulty_modifier,
      "direction difficulty" : _direction_difficulty_modifier
   ]);
} /* query_static_auto_load() */

/** @ignore yes */
mapping query_static_auto_load() {
   if ( base_name(this_object()) != __FILE__[0..<3]) {
      return ([ ]);
   }
   return int_query_static_auto_load();
} /* query_static_auto_load() */

/** @ignore yes */
void init_static_arg(mapping map) {
   if (map["::"]) {
      clothing::init_static_arg(map["::"]);
   }
   _race = map["worn by race"];
   _mount_difficulty_modifier = map["mount difficulty"];
   _direction_difficulty_modifier = map["direction difficulty"];
} /* init_static_arg() */
