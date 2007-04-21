/**
 * This is region handler for a rectangular region.  This can also work
 * for circular regions by making a 0 width/height feature.
 * @author Pinkfish
 * @started Mon Apr  1 12:15:24 PST 2002
 */
inherit "/std/room/inherit/terrain_map/geometry";

class feature_pos {
   int xstart;
   int ystart;
   int width;
   int height;
}

private mixed    *_feature_direcs = ({ ({ "southwest", "south", "southeast" }),
                               ({ "west", "Oops", "east" }),
                               ({ "northwest", "north", "northeast" }) });

private class feature_pos* _positions = ({ });

/**
 * This method sets the position of the feature.
 * @param x the x coordinate of the feature
 * @param y the y coordinate of the feature
 * @param width the width of the feature
 * @param height the height of the feature
 */
void add_feature_position(int x, int y, int width, int height) {
   class feature_pos pos;

   pos = new(class feature_pos, xstart : x, ystart : y,
                                width : width, height : height);
   _positions += ({ pos });
}

/**
 * This is the function that finds the distance and direction to the
 * current region.
 * @param x the x-coordinate
 * @param y the y-coordinate
 * @param z the z-coordinate
 * @return ({ direction, distance })
 */
mapping query_feature_desc_from(int x, int y, int z) {
   int h;
   int v;
   int width;
   int height;
   float distance;
   float min_distance;
   string direc;
   class feature_pos pos;
   float xmod;
   float ymod;
   int xd;
   int yd;

   foreach (pos in _positions) {
      h = pos->xstart;
      v = pos->ystart;
      width = pos->width;
      height = pos->height;
      if (x >= (h + width)) {
         h = h + width - 1;
      } else if (x >= h) {
         h = x;
      }
      if (y >= (v + height)) {
         v = v + height - 1;
      } else if (y >= v) {
         v = y;
      }

      distance = sqrt(pow(x - h, 2) + pow(y - v, 2));
      if (distance > 0.0000001 && (distance < min_distance || !min_distance)) {
         xmod = ((h - x) < 0) ? -0.5 : 0.5;
         ymod = ((v - y) < 0) ? -0.5 : 0.5;
         xd = to_int(xmod + to_float(h - x) / distance) + 1;
         yd = to_int(ymod + to_float(v - y) / distance) + 1;
         direc = _feature_direcs[yd][xd];
         min_distance = distance;
      }
   }

   if (!direc) {
      return ([ ]);
   }

   return ([ direc : to_int(min_distance) ]);
}

/** @ignore yes */
void dest_me() {
   destruct(this_object());
}

/**
 * Find out if this rectangle is inside the region, given the specified range
 * at which it can be seen.  This will work by adding the ranges onto the
 * lines and checking for distance from the points of the polygon to the line
 * bordering the region and also checking for intersection with the bordering
 * line.  It also checks to see if all of the points are inside the region.
 * @param x1 the top x
 * @param y1 the top y
 * @param x2 the bottom x
 * @param y2 the bottom y
 */
int is_inside_region(int x1, int y1, int x2, int y2, int range) {
   class feature_pos position;
   int height;
   int width;

   foreach (position in _positions) {
      height = position->height;
      width = position->width;
      if (position->xstart >= x1 && position->xstart <= x2 &&
          position->ystart >= y1 && position->ystart <= y2) {
         return 1;
      }
      if (position->xstart >= x1 && position->xstart <= x2 &&
          position->ystart + height >= y1 && position->ystart + height <= y2) {
         return 1;
      }
      if (position->xstart +width >= x1 && position->xstart +width <= x2 &&
          position->ystart + height >= y1 && position->ystart + height <= y2) {
         return 1;
      }
      if (position->xstart +width >= x1 && position->xstart +width <= x2 &&
          position->ystart >= y1 && position->ystart <= y2) {
         return 1;
      }

/*
      if (distance_between_two_line_segments(x1, y1, x1, y2,
          position->xstart + width, position->ystart, position->xstart +width, position->ystart + height) < range) {
         return 1;
      }
      if (distance_between_two_line_segments(x1, y1, x2, y1,
          position->xstart, position->ystart + height, position->xstart +width, position->ystart + height) < range) {
         return 1;
      }
      if (distance_between_two_line_segments(x2, y1, x2, y2,
          position->xstart, position->ystart, position->xstart, position->ystart + height) < range) {
         return 1;
      }
      if (distance_between_two_line_segments(x1, y2, x2, y2, 
          position->xstart, position->ystart, position->xstart +width, position->ystart) < range) {
         return 1;
      }
 */
   }
   return 0;
}
