/**
 * This is region handler for a polygon region.
 * @author Pinkfish
 * @started Mon Apr  1 12:15:24 PST 2002
 */
inherit "/std/room/inherit/terrain_map/geometry";

class vertex {
   int x;
   int y;
}

private mixed    *_feature_direcs = ({ ({ "southwest", "south", "southeast" }),
                               ({ "west", "Oops", "east" }),
                               ({ "northwest", "north", "northeast" }) });

private class vertex* _vertexes = ({ });

/**
 * This method sets the position of the feature.
 * @param x the x coordinate of the feature
 * @param y the y coordinate of the feature
 * @param width the width of the feature
 * @param height the height of the feature
 */
void add_vertex(int x, int y) {
   class vertex vertex;

   vertex = new(class vertex, x : x, y : y);
   _vertexes += ({ vertex });
}

/**
 * This is the function that finds the distance and direction to the
 * current region.
 * @param x the x-coordinate
 * @param y the y-coordinate
 * @param z the z-coordinate
 * @return ({ direction, distance [, direc2, dist2, [...]] })
 */
mapping query_feature_desc_from(int x, int y, int z) {
   class vertex vert1;
   class vertex vert2;
   int i;
   float v1_x;
   float v1_y;
   float v2_x;
   float v2_y;
   float vm_x;
   float vm_y;
   float b;
   float dot1;
   float dot2;
   float min_distance;
   float distance;
   float xmod;
   float ymod;
   int xd;
   int yd;
   string direc;
   mapping dirs;
   class point p;

   dirs = ([ ]);
   // With a series of line segments we don't count the implied return segment
   for (i = 0; i < sizeof(_vertexes) - 1; i++) {
      vert1 = _vertexes[i];
      vert2 = _vertexes[(i + 1) % sizeof(_vertexes)];
      v1_x = to_float(vert1->x - vert2->x);
      v1_y = to_float(vert1->y - vert2->y);
      v2_x = to_float(x - vert2->x);
      v2_y = to_float(y - vert2->y);

      dot1 = v1_x * v2_x + v1_y * v2_y;
      if (dot1 <= 0.0) {
         // Distance to p1
         distance = sqrt(pow(vert2->x - x, 2) + pow(vert2->y - y, 2));
//printf("1) %O\n", distance);
         if ((!min_distance || distance < min_distance) && distance > 0.0001) {
            xmod = ((vert2->x - x) < 0) ? -0.5 : 0.5;
            ymod = ((vert2->y - y) < 0) ? -0.5 : 0.5;
            xd = to_int(xmod + to_float(vert2->x - x) / distance) + 1;
            yd = to_int(ymod + to_float(vert2->y - y) / distance) + 1;
            direc = _feature_direcs[yd][xd];
            min_distance = distance;
         }
      } else {
         dot2 = v1_x * v1_x + v1_y * v1_y;
         if (dot2 <= dot1) {
            // Distance to p2.
            distance = sqrt(pow(vert1->x - x, 2) + pow(vert1->y - y, 2));
            if ((!min_distance || distance < min_distance) && distance > 0.0001) {
//printf("2) %O\n", distance);
               xmod = ((vert1->x - x) < 0) ? -0.5 : 0.5;
               ymod = ((vert1->y - y) < 0) ? -0.5 : 0.5;
               xd = to_int(xmod + to_float(vert1->x - x) / distance) + 1;
               yd = to_int(ymod + to_float(vert1->y - y) / distance) + 1;
               direc = _feature_direcs[yd][xd];
               min_distance = distance;
            }
         } else {
            b = dot1 / dot2;
            vm_x = to_float(vert2->x) + b * v1_x;
            vm_y = to_float(vert2->y) + b * v1_y;
            // Distance to vm.
            distance = sqrt(pow(vm_x - x, 2) + pow(vm_y - y, 2));
//printf("3) (%O, %O) %O\n", vm_x, vm_y, distance);
            if ((!min_distance || distance < min_distance) && distance > 0.0001) {
               xmod = ((vm_x - x) < 0) ? -0.5 : 0.5;
               ymod = ((vm_y - y) < 0) ? -0.5 : 0.5;
               xd = to_int(xmod + to_float(vm_x - x) / distance) + 1;
               yd = to_int(ymod + to_float(vm_y - y) / distance) + 1;
               direc = _feature_direcs[yd][xd];
               min_distance = distance;
            }
         }
      }
      // Do specific direction checks.
      p = intersection_of_line_and_segment(0,0, 1,0, vert1->x, vert1->y,
                                           vert2->x, vert2->y);
      if (p) {
         if (p->x > x) {
            if (dirs["east"] && dirs["east"] > p->x) {
               dirs["east"] = p->x;
            }
         } else {
            if (dirs["west"] && dirs["west"] > p->x) {
               dirs["west"] = p->x;
            }
         }
      }
      p = intersection_of_line_and_segment(0,0, 0,1, vert1->x, vert1->y,
                                           vert2->x, vert2->y);
      if (p) {
         if (p->y > y) {
            if (dirs["north"] && dirs["north"] > p->y) {
               dirs["north"] = to_int(p->y);
            }
         } else {
            if (dirs["south"] && dirs["south"] > p->y) {
               dirs["south"] = to_int(p->y);
            }
         }
      }
      p = intersection_of_line_and_segment(0,0, 1,1, vert1->x, vert1->y,
                                           vert2->x, vert2->y);
      if (p) {
         if (p->x > x) {
            if (dirs["northeast"] && dirs["northeast"] > p->x) {
               dirs["northeast"] = to_int(p->x);
            }
         } else {
            if (dirs["southwest"] && dirs["southwest"] > p->x) {
               dirs["southwest"] = to_int(p->x);
            }
         }
      }
      p = intersection_of_line_and_segment(0,0, 1,-1, vert1->x, vert1->y,
                                           vert2->x, vert2->y);
      if (p) {
         if (p->y > y) {
            if (dirs["northwest"] && dirs["northwest"] > p->y) {
               dirs["northwest"] = p->y;
            }
         } else {
            if (dirs["southeast"] && dirs["southeast"] > p->y) {
               dirs["southeast"] = p->y;
            }
         }
      }
//printf("%O of %O: %O %O\n", i, sizeof(_vertexes), direc, min_distance);
   }

   dirs[direc] = to_int(min_distance);

/*
   ret = ({ direc, to_int(min_distance) });
   foreach (direc, i in dirs) {
       ret += ({ direc, i });
   }
 */
   return dirs;
}

/**
 * Find out if this polygon is inside the region, given the specified range
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
   class vertex vertex;
   class vertex vert1;
   class vertex vert2;
   int i;

   foreach (vertex in _vertexes) {
      if (vertex->x >= x1 && vertex->x <= x2 &&
          vertex->y >= y1 && vertex->y <= y2) {
         return 1;
      }
   }

   // Now check distances to the edges of the polygon.
   for (i = 0; i < sizeof(_vertexes); i++) {
      vert1 = _vertexes[i];
      vert2 = _vertexes[(i + 1) % sizeof(_vertexes)];
      if (distance_between_two_line_segments(x1, y1, x1, y2,
          vert1->x, vert1->y, vert2->x, vert2->y) < range) {
         return 1;
      }
      if (distance_between_two_line_segments(x1, y1, x2, y1,
          vert1->x, vert1->y, vert2->x, vert2->y) < range) {
         return 1;
      }
      if (distance_between_two_line_segments(x2, y1, x2, y2,
          vert1->x, vert1->y, vert2->x, vert2->y) < range) {
         return 1;
      }
      if (distance_between_two_line_segments(x1, y2, x2, y2,
          vert1->x, vert1->y, vert2->x, vert2->y) < range) {
         return 1;
      }
   }
   return 0;
}

/** @ignore yes */
void dest_me() {
   destruct(this_object());
}
