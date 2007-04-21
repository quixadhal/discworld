/**
 * An inherit to use in the terrain stuff to deal with distance messages
 * when they are needed.
 * @author Pinkfish
 * @started Mon Apr  1 12:40:00 PST 2002
 */
#include <terrain_map.h>

/**
 * This method returns a string displaying how long the specified
 * distance is.
 * @param dist the distance
 */
private string query_distance_str_int(int dist, int show_one) {
   if (dist < 3) {
      if (dist > 1) {
         return query_num(dist) + " feet";
      } else if (show_one) {
         return "one foot";
      } else {
         return "foot";
      }
   } else if (dist < TERRAIN_MAP_HALF_MILE) {
      dist /= 3;
      if (dist > 50) {
         dist = dist / 50 * 50;
      }
      if (dist > 1) {
         return query_num(dist) + " yards";
      }
      if (show_one) {
         return "one yard";
      } else {
         return "yard";
      }
   } else if (dist < TERRAIN_MAP_ONE_MILE) {
      return "half mile";
   } else {
      dist /= TERRAIN_MAP_ONE_MILE;
      if (dist > 10) {
         dist = dist / 10 * 10;
      }
      if (dist > 100) {
         dist = dist / 50 * 50;
      }
       if (dist > 1000) {
         dist = dist / 500 * 500;
      }
      if (dist > 1) {
         return query_num(dist) + " miles";
      } else {
         if (show_one) {
            return "one mile";
         } else {
            return "mile";
         }
      }
   }
}

/**
 * This method returns a string displaying how long the specified
 * distance is.
 * @param dist the distance
 */
string query_distance_str(int dist) {
   return query_distance_str_int(dist, 1);
}

/**
 * This method returns a string displaying how long the specified
 * distance is, without any ones.
 * @param dist the distance
 */
string query_distance_str_no_ones(int dist) {
   return query_distance_str_int(dist, 0);
}
