/**
 * This is specifically for creating rooms in the world map system, where
 * it just makes and passes out the room details.  So it does not need to
 * include all the room crap.
 * @author Pinkfish
 * @started Fri Jun 28 16:14:19 PDT 2002
 */
#include <terrain_map.h>

inherit TERRAIN_MAP_FACTORY_BASE;

void create() {
   ::create();
   this_object()->setup();
} /* create() */
