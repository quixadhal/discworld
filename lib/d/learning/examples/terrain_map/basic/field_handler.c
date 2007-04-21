#include <terrain_map.h>

inherit TERRAIN_MAP_HANDLER_BASE;

// For the purposes of this example, the map is called 'field.map',
// and is located in the same directory as this file.
string query_map_file() { return __DIR__ + "field"; }

void  setup_handler()

{
   // Setup our 'obstacle'. We put in the description to be seen in
   // adjacent locations, and an add_item to make it consistent.
   add_obstacle_type("*", "A tall fence blocks your way to the $D.",
                "fence", "A tall, unclimable wooden fence.");
   
   // Setup the standard field room - simply maps the '.' characters
   // onto the filename.
   add_room_type(".", __DIR__ + "field");
   
   // Setup the standard field road - maps the ',' characters
   // onto the filename. We also need the description to be seen in
   // adjacent locations, and details for an add_item entry in
   // said adjacent locations.
   add_road_type(",", __DIR__ + "path",
            "The path meanders $D.",
            "A small path meanders its way across the field to the $D.",
            "path",
            "The path has been trampled down by the hooves of time.");
   
   // To make this a little more interesting, let's add a feature.
   add_feature("bigtree", 4, 1, 1, 1, 4, 
                ({ "A giant tree towers over you to the $D.",
                   "A little way to the $D, you can see a huge tree.",
                   "You can just make out a large tree, a way off in an "
                   "$Derly direction." }), 
                   "tree", "A fairly large, and quite beautiful tree." );

   // And to make it even more interesting, let's give the tree some chats.
   set_feature_chats("bigtree", 
                     ({ "The branches on the tree to the $D "
                        "sway slightly in the breeze." }) );

   // Finally, we add our exit from the terrain map
   add_exit_at(2, 0, "south", __DIR__ + "entrance", "road");
}
