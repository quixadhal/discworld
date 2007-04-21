/**
 * This type of feature generates distance messages to the feature
 * based on how far away it is.  It uses a small basic string as the
 * main part of the description and adds in distance messages to show
 * how far away the item is.
 * @author Pinkfish
 * @started Mon Apr  1 12:50:02 PST 2002
 */
#include <terrain_map.h>

inherit TERRAIN_MAP_FEATURE_BASE;

private string _base_desc;
private int _very_close_distance;
private int _distance_multiplier = 1;

/**
 * This method sets the distance at which the description is set to
 * very close instead of a distance measurement.
 * @param close the distance to use
 */
void set_very_close_distance(int close) {
   _very_close_distance = close;
}

/**
 * This method returns the basic description for the feature.
 */
string query_base_description() {
   return _base_desc;
} 

/**
 * This method sets the basic description for the feature.
 * @param desc the basic description
 */
void set_base_description(string desc) {
   _base_desc = desc;
} 

/**
 * This method sets the distance multiplier for the feature.  This is amount
 * to multiply the range by to find the actual distance.
 */
void set_distance_multiplier(int mult) {
   _distance_multiplier = mult;
}

/** @ignore yes */
string calc_feature_desc(mapping direcs, int night, int visibility) {
   string direc;
   int dist;
   int new_dist;
   string* bits;

   bits = ({ });
   if(visibility < 1 && visibility != -1)
     visibility = 1;
   
   foreach (direc, dist in direcs) {
      dist += query_range_offset() ;
      new_dist = dist * 100 / visibility;
      if (visibility == -1 ||
          (new_dist < query_max_range() && dist > query_min_range())) {
         if (dist < _very_close_distance) {
            bits += ({ "very close to the " + direc });
         } else {
            bits += ({ query_distance_str(dist * _distance_multiplier) + " to the " + direc });
         }
      }
   }
   if (sizeof(bits) > 1) {
      return implode(bits[0..<2], ", ") + " and " + bits[<1] + " " +_base_desc;
   }
   if (sizeof(bits)) {
      return bits[0] + " " + _base_desc;
   }
   return 0;
}

/** @ignore yes */
string calc_map_feature_desc(mapping direcs, int fudge) {
   string direc;
   int dist;
   string* bits;

   bits = ({ });
   foreach (direc, dist in direcs) {
      dist += query_range_offset();
      if (dist < _very_close_distance * 5) {
        bits += ({ "very close to the " + direc });
      } else {
        bits += ({ ({"approximately ", "about ", "roughly "})[random(3)] +
                     query_distance_str(dist * _distance_multiplier) +
                     " to the " + direc });
      }
   }
   if (sizeof(bits) > 1) {
      return implode(bits[0..<2], ", ") + " and " + bits[<1] + " " +_base_desc;
   }
   if (sizeof(bits)) {
      return bits[0] + " " + _base_desc;
   }
   return 0;
}
