/**
 * This is a stand alone feature handler for dealing with features that
 * span over multiple terrains.
 * @author Pinkfish
 * @started Mon Apr  1 12:15:24 PST 2002
 */

inherit "/std/room/inherit/terrain_map/distance_str";

private int _max_range;
private int _max_day_range;
private int _max_night_range;
private int _min_range = -1;
private int _range_offset;
private int _blocking;
private object _region;

class feature_pos {
   int xstart;
   int ystart;
   int width;
   int height;
}

/**
 * This method sets the region to use for the system.  If the region is
 * set as a string, then that region is cloned, if it is set as and
 * object, it is just used.
 * @param region the region to use
 */
void set_region(mixed region) {
   if (_region) {
      _region->dest_me();
   }

   if (stringp(region)) {
      _region = clone_object(region);
   } else if (objectp(region)) {
      _region = region;
   }
}

/**
 * This method returns the region object.  This should be used
 * for setup calls.
 * @return the region object
 */
object query_region_ob() {
   return _region;
}

/**
 * This method sets the range offset.  The offset is added to the
 * distance of the feature for calculating the distance.  If the
 * point is inside the offset the distance is ignored and the
 * distance it then calculated from the edge of the offset.  This
 * can be used to generate circular features.
 * @param offset the offset to use
 */
void set_range_offset(int offset) {
   _range_offset = offset;
}

/**
 * This method returns the range offset.  The offset is added to the
 * distance of the feature for calculating the distance.  If the
 * point is inside the offset the distance is ignored and the
 * distance it then calculated from the edge of the offset.  This
 * can be used to generate circular features.
 * @return the current range offset
 */
int query_range_offset() {
   return _range_offset;
}

/**
 * This method sets the maximum range for the feature to be seen at.
 * @param range the maximum range to be seen at
 */
void set_max_day_range(int range) {
  _max_day_range = range;
   if (_max_day_range > _max_range) {
      _max_range = _max_day_range;
   }
}

/**
 * This method returns the maximum range the feature can be seen at.
 * @return the maximum range of the feature
 */
int query_max_day_range() {
   return _max_day_range;
}

/**
 * This method sets the maximum range for the feature to be seen at.
 * @param range the maximum range to be seen at
 */
void set_max_night_range(int range) {
   _max_night_range = range;
   if (_max_night_range > _max_range) {
      _max_range = _max_night_range;
   }
}

/**
 * This method returns the maximum range the feature can be seen at.
 * @return the maximum range of the feature
 */
int query_max_night_range() {
   return _max_night_range;
}

/**
 * This method sets the minimum range for the feature to be seen at.
 * @param range the minimum range to be seen at
 */
void set_min_range(int range) {
  _min_range = range;
}

/**
 * This method returns the minimum range the  feature can be seen at.
 * @return the minimum range of the feature
 */
int query_min_range() {
   return _min_range;
}

/**
 * This method returns the maximum possible range the item can be seen
 * at.
 * @return the maximum possible range
 */
int query_max_range() {
   return _max_range;
}

/**
 * This method sets the blocking status of the feature.  This will only work
 * with local features and not distant features.
 * @param blocking the blocking flag
 */
int set_blocking(int blocking) {
   _blocking = blocking;
}

/**
 * This method returns the blocking status of the feature.  This will only work
 * with local features and not distant features.
 * @return the blocking status of the features
 */
int query_blocking() {
   return _blocking;
}

/**
 * This function needs to be overridden to display the actual
 * message to the feature.  The direc argument is a mapping of the
 * form, key = direction, value = distance.
 * @param direc the directions and distances
 * @param night the night feature
 * @return the feature descriptions
 */
string calc_feature_desc(mapping direc, int night, int visibility) {
   return "This needs to be set.";
}

/**
 * This method filters the returned descs to make sure they are all
 * within the needed range.
 * @param direcs the mappings of directions and distance
 */
mapping filter_distances(mapping direcs) {
   return filter(direcs, (: $2 <= _max_range :));
}

/**
 * This is the function used by the system to find the message for this
 * feature.
 * @param x the x-coordinate
 * @param y the y-coordinate
 * @param z the z-coordinate
 * @param no_filter whether or not to filter by distance
 */
mapping query_feature_desc_from(int x, int y, int z, int no_filter) {
   mapping bits;

   if (_region) {
      bits = _region->query_feature_desc_from(x, y, z);
      if (bits) {
        if(no_filter)
          return bits;
        else
          return filter_distances(bits);
      }
   }
   return ([ ]);
}

/**
 * This method determines if the feature is inside the specified region
 * or not.
 * @param x1 the top x
 * @param y1 the top y
 * @param x2 the bottom x
 * @param y2 the bottom y
 */
int is_inside_region(int x1, int y1, int x2, int y2) {
   return _region->is_inside_region(x1, y1, x2, y2, query_max_range());
}

/** @ignore yes*/
void dest_me() {
   if (_region) {
      _region->dest_me();
   }
   destruct(this_object());
}
