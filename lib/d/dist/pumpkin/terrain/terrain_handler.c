#include <terrain_map.h>
#include "path.h"

inherit TERRAIN_MAP_HANDLER_BASE;

string query_map_file() {
    return __DIR__ + "terrain";
}

void setup_handler() {
    set_newline_mode(0);
   // The visible distance is the basic distance things should be visible
   // using the 'map' command.
   set_visible_distance(5);

   // This sets up an obstacle for all the 'H's in the map.  They will be a
   // hedge and the rooms beside will be able to look at the hedge, the last
   // two parameters is an item name and a description.
   // The second parameter is the description that will be used in the room
   // when beside the room.  If this description has a $D in it, it will be
   // expanded with a list of the directions the obstacle lies in.
   add_obstacle_type("H", "a lush, thick hedge surrounds the small garden",
     "hedge", "The hedge surrounds the garden, stretching thick and "
     "impenetrable towards the sky, protecting anyone in the garden from the "
     "gawking eyes of passers-by.");
   add_obstacle_type("*", "s big high lump that you cannot avoid looking at",
     "lump", "The lump is very lumpy.");

   // Add in a feature for the lump obstacle so people can see the lump
   // from a distance.
   add_feature("lump",
               16, 3, // bottom left of the bounding box.
               1, 1, // width, height of the bounding box
               15, // Distance at which the feature can be seen.
               ({ // Day descs.
                  "a lump towers above you to the $D",
                }),
               // Item you can look at
               "lump", "A lumpy thing that lumps around.",
               ({ // Night descs
                  "a lump at night to the $D",
                }),
               // Night item.
               "It looks just like a lump, even though it is dark.");

   // A road is an item that you can walk on.  It follows roughly the same
   // pattern as an obstacle, the second is a continuation direction, saying
   // where the path continues too, the third is the adjecent description,
   // which can be expanded with $D, the fourth and fifth are the items.
   add_road_type("=", __DIR__ "plainpath",
     "", "a nice sandy path leads through the plains.");
   add_road_type("+", __DIR__ "woodpath",
     "", "a nice sandy path leads through the woods.");

   // To give an example of a road that skips locations, here is a road that
   // allows you to zip along a few locations at a time.
   add_road_special_type("-", __DIR__ "desertpath",
     3, // Skip three with each movement
     1, // Road follwing turned on.
     0, // Base height of the room (0).
     "", "a nice sandy path leads through the woods.");

   add_room_type("L", __DIR__ "plains");
   add_room_type("W", __DIR__ "woods");
   add_room_type("d", __DIR__ "desert");

   // This is a fixed position exit in the terrain.  Gives the x,y co-ordinates
   // of the exit, where it leads do and the direction it takes.
   add_exit_at(0, 1, "west", GUMBOOT + "gumboot5", "road");
   add_exit_at(0, 18, "west", RABBIT + "rabbit4", "road");
}
