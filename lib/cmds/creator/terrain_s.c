inherit "/cmds/base";
#include <terrain_map.h>

/**
 * Useful command to find out information about the terrain.
 * Contains a bunch of small useful commands.
 */

/**
 * This one maps the area around the creator.
 */
int do_map() {
   string map;
   object hand;
   int* coords;
   int x;
   int y;

   hand = environment(this_player())->query_map_handler();
   if (!hand) {
      add_failed_mess("You must be in a terrain to use this command.\n");
      return 0;
   }

   coords = environment(this_player())->query_terrain_coords();
   x = coords[0];
   y = coords[1];
   map = hand->query_debug_map(x, y, 13, x, y);
   write(map);
   add_succeeded_mess("");
   return 1;
}

/**
 * This one maps the whole terrain.
 */
int do_map_terrain() {
   string map;
   object hand;
   string* map_data;
   int i;

   hand = environment(this_player())->query_map_handler();
   if (!hand) {
      add_failed_mess("You must be in a terrain to use this command.\n");
      return 0;
   }

   map_data = hand->query_area_map();
   map = "";
   for (i = sizeof(map_data) - 1; i >= 0; i--) {
      map += map_data[i] + "\n";
   }
   write("$P$Map$P$" + map);
   add_succeeded_mess("");
   return 1;
}

/**
 * This method shows the features associated with this terrain.
 */
int do_features_distant() {
   object hand;
   string title;
   mixed tmp;
   string fluff;
   string ret;
   int* coords;
   int x;
   int y;
   int z;
   string* features;

   hand = environment(this_player())->query_map_handler();
   if (!hand) {
      add_failed_mess("You must be in a terrain to use this command.\n");
      return 0;
   }

   coords = environment(this_player())->query_co_ord();
   x = coords[0];
   y = coords[1];
   z = coords[2];

   features = TERRAIN_MAP_WORLD_MAP->query_features_at(x, y);
   if (!sizeof(features)) {
      add_failed_mess("No distant features to list.\n");
      return 0;
   }

   // Find the distant features.
   ret = "List of distant features:\n";
   foreach (title in features) {
      tmp = title->query_feature_desc_from(x, y, z);
      if (tmp && tmp != "") {
         if (stringp(tmp)) {
            ret += "$I$3=%^BOLD%^" + title + "%^RESET%^: " + tmp + "\n";
         } else if (pointerp(tmp)) {
            foreach (fluff in tmp) {
               ret += "$I$3=%^BOLD%^" + title + "%^RESET%^: " + fluff + "\n";
            }
         }
      }
   }

   write("$P$Distant Features$P$" + ret);
   return 1;
}

/**
 * This method lists all the local features in this room.
 */
int do_features_local() {
   mapping features;
   string ret;
   object hand;
   mapping hand_features;
   string name;
   mixed stuff;

   hand = environment(this_player())->query_map_handler();
   if (!hand) {
      add_failed_mess("You must be in a terrain to use this command.\n");
      return 0;
   }

   hand_features = hand->query_features();
   features = environment(this_player())->query_features();
   if (!features) {
      add_failed_mess("You must be in a terrain to use this command.\n");
      return 0;
   }

   if (!sizeof(features)) {
      add_failed_mess("No local features.\n");
      return 0;
   }

   ret = "List of features:\n";
   foreach (name, stuff in hand_features) {
      ret += "$I$3=%^BOLD%^" + name + "%^RESET%^ (" + 
             file_name(hand->query_feature_region(name)) + ")";
      if (features[name]) {
         ret += " " + features[hand] + "\n";
      } else {
         ret += " Not visible here.\n";
      }
   }
   write("$P$Features$P$" + ret);
   return 1;
}

/**
 * This method maps a specific local feature showing the range that it covers
 * in the terrain.
 */
int do_feature_local_map(string feature) {
   object hand;
   mapping hand_features;
   string ret;

   hand = environment(this_player())->query_map_handler();
   if (!hand) {
      add_failed_mess("You must be in a terrain to use this command.\n");
      return 0;
   }

   hand_features = hand->query_features();

   if (!hand_features[feature]) {
      add_failed_mess("There is no feature called " + feature + ".\n");
      return 0;
   }

   ret = hand->query_debug_map_feature(feature);

   write("$P$" + feature + "$P$" + ret);
   return 1;
}

/**
 * This method maps a specific local feature showing the range that it covers
 * in the terrain.
 */
int do_feature_distant_map(string feature) {
   object hand;
   string* dist_features;
   string ret;
   int* coords;
   int x;
   int y;
   int z;

   hand = environment(this_player())->query_map_handler();
   if (!hand) {
      add_failed_mess("You must be in a terrain to use this command.\n");
      return 0;
   }

   coords = environment(this_player())->query_co_ord();
   x = coords[0];
   y = coords[1];
   z = coords[2];
   dist_features = TERRAIN_MAP_WORLD_MAP->query_features_at(x, y);

   if (member_array(feature, dist_features) == -1) {
      add_failed_mess("There is no distant feature " + feature + ".\n");
      return 0;
   }

   ret = hand->query_debug_map_feature_distant(feature);

   write("$P$" + feature + "$P$" + ret);
   return 1;
}

/**
 * This prints the status of the terrain.  The x,y real co-ordinates, size,
 * features and distant features.
 */
int do_terrain_status() {
   string ret;
   object hand;
   mixed* coords;
   mapping features;
   string name;
   int x;
   int y;
   int z;
   string* dist_features;
   mapping terrs;

   hand = environment(this_player())->query_map_handler();
   if (!hand) {
      add_failed_mess("You must be in a terrain to use this command.\n");
      return 0;
   }

   ret = "%^BOLD%^" + file_name(hand) + ":%^RESET%^\n";
   ret += "Size: " + hand->query_dimensions()[0] + ", " + hand->query_dimensions()[1] + "\n";
   coords = hand->query_real_coords();
   if (coords) {
      ret += "Real: " + coords[0][0] + ", " + coords[0][1] + " -- " +
             coords[1][0] + ", " + coords[1][1] + "\n";
   }

   features = hand->query_features();
   foreach (name in keys(features)) {
      ret += name + ": " + file_name(hand->query_feature_ob(name)) + "\n";
   }

   coords = environment(this_player())->query_co_ord();
   x = coords[0];
   y = coords[1];
   z = coords[2];
   dist_features = TERRAIN_MAP_WORLD_MAP->query_features_at(x, y);
   foreach (name in dist_features) {
      ret += name + " (distant feature)\n";
   }


   hand->find_all_adjacent_terrains();
   terrs = hand->query_adjacent_terrains();
   if (terrs["-1:-1"]) {
      ret += "Southwest: " + terrs["-1:-1"] + "\n";
   }
   if (terrs["0:-1"]) {
      ret += "South: " + terrs["0:-1"] + "\n";
   }
   if (terrs["40:-1"]) {
      ret += "Southeast: " + terrs["40:-1"] + "\n";
   }
   if (terrs["40:0"]) {
      ret += "East: " + terrs["40:0"] + "\n";
   }
   if (terrs["40:40"]) {
      ret += "NorthEast: " + terrs["40:40"] + "\n";
   }
   if (terrs["0:40"]) {
      ret += "North: " + terrs["0:40"] + "\n";
   }
   if (terrs["-1:40"]) {
      ret += "Northwest: " + terrs["-1:40"] + "\n";
   }
   if (terrs["-1:0"]) {
      ret += "West: " + terrs["-1:0"] + "\n";
   }
   write("$P$Status$P$" + ret);
   return 1;
}

mixed* query_patterns() {
   return ({ "map", (: do_map() :),
             "map terrain", (: do_map_terrain :),
             "features local", (: do_features_local :),
             "features distant", (: do_features_distant :),
             "status", (: do_terrain_status :),
             "features local <string'feature to map'>",
                  (: do_feature_local_map($4[0]) :),
             "features distant <string'feature to map'>",
                  (: do_feature_distant_map($4[0]) :) });
}
