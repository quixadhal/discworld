/**
 * This is the handler to help control the world map.  It co-ordinates
 * all the terrains connecting themselves together.  All terrains are
 * rectangular.
 * @author Pinkfish
 * @started Fri Mar 22 16:58:35 PST 2002
 */
#include <terrain_map.h>
class region {
   mixed* terrains;
   string* features;
   int last_touched;
}

#define SAVE_DIR "/save/world_map/"

#define BOUNDARY TERRAIN_MAP_WORLD_BOUNDARY 
#define REGION_SIZE TERRAIN_MAP_WORLD_REGION_SIZE
#define REGION_BOUNDARY TERRAIN_MAP_WORLD_REGION_BOUNDARY
#define OFFSET TERRAIN_MAP_WORLD_OFFSET

#define REGION_WIDTH TERRAIN_MAP_WORLD_REGION_WIDTH

void load_me();
private void generate_main_templates();
private void generate_blocking_templates();

// Array of array of regions
private nosave mixed* _regions;
//private string* _features;
private nosave mixed* _templates;
private nosave mixed* _blocking_templates;

void create() {
   // Make a 10 x 10 grid.
   _regions = allocate(REGION_WIDTH, (: allocate(REGION_WIDTH) :));
   //_features = ({ });
   seteuid(getuid());
   generate_main_templates();
   generate_blocking_templates();
   //load_me();
}

private void generate_main_templates() {
   int size;
   mixed* last;
   int x;
   int y;
   int u;
   int v;
   int e;
   int i;

   _templates = allocate(12);

   last = ({ ({ 1 }) });
   for (size = 0; size < sizeof(_templates); size++) {
      for (i = 0; i < sizeof(last); i++) {
         last[i] = ({ 0 }) + last[i] + ({ 0 });
      }
      _templates[size] = ({ allocate((size + 1) * 2 + 1) }) + last +
                         ({ allocate((size + 1) * 2 + 1) });
      // Draw a circle of the right size.
      x = 0;
      y = size + 1;
      u = 1;
      v = 2 * (size + 1) - 1;
      e = 0;

      while (x <= y) {
         _templates[size][size + 1 + x][size + 1 + y] = 1;
         _templates[size][size + 1 + x][size + 1 - y] = 1;
         _templates[size][size + 1 - x][size + 1 + y] = 1;
         _templates[size][size + 1 - x][size + 1 - y] = 1;
         _templates[size][size + 1 + y][size + 1 + x] = 1;
         _templates[size][size + 1 + y][size + 1 - x] = 1;
         _templates[size][size + 1 - y][size + 1 + x] = 1;
         _templates[size][size + 1 - y][size + 1 - x] = 1;

         if (size > 0) {
            _templates[size][size + 1 + x - 1][size + 1 + y] = 1;
            _templates[size][size + 1 + x - 1][size + 1 - y] = 1;
            _templates[size][size + 1 - x + 1][size + 1 + y] = 1;
            _templates[size][size + 1 - x + 1][size + 1 - y] = 1;
            _templates[size][size + 1 + y - 1][size + 1 + x] = 1;
            _templates[size][size + 1 + y - 1][size + 1 - x] = 1;
            _templates[size][size + 1 - y + 1][size + 1 + x] = 1;
            _templates[size][size + 1 - y + 1][size + 1 - x] = 1;
         }
         x++;
         e += u;
         u += 2;
         if (v < 2 * e) {
            y--;
            e -= v;
            v -= 2;
         }
         if (x > y) {
            break;
         }
      }
      last = copy(_templates[size]);
   }
}

mixed* generate_blocking(int x, int y, string* start,
                          string* straight, string* diagonal) {
   int i;
   int dx;
   int ex;
   int dy;
   int balance;
   int sx;
   int sy;
   int blocked;
   int blocked_straight;
   int blocked_diagonal;
   int first_block;

   first_block = -1;
   for (i = 0; i <= sizeof(start); i++) {
      // Generate a line from 0,0 to 0,i
      sx = 0;
      sy = 0;
      ex = sizeof(start);
      dx = sizeof(start);
      dy = i;
      dy <<= 1;
      balance = dy - dx;
      dx <<= 1;
      blocked_straight = blocked_diagonal = blocked = 0;
      while (sx != ex) {
         if (blocked) {
            if (first_block == -1) {
               first_block = i;
            }
            start[sy] = start[sy][0..sx-1] + "*" + start[sy][sx+1..];
         } else {
            start[sy] = start[sy][0..sx-1] + " " + start[sy][sx+1..];
         }
         if (blocked_straight) {
            straight[sy] = straight[sy][0..sx-1] + "*" + straight[sy][sx+1..];
         } else {
            straight[sy] = straight[sy][0..sx-1] + " " + straight[sy][sx+1..];
         }
         if (blocked_diagonal) {
            diagonal[sy] = diagonal[sy][0..sx-1] + "*" + diagonal[sy][sx+1..];
         } else {
            diagonal[sy] = diagonal[sy][0..sx-1] + " " + diagonal[sy][sx+1..];
         }
         if (sx == x && sy == y) {
            blocked_diagonal = blocked_straight = blocked = 1;
            diagonal[sy] = diagonal[sy][0..sx-1] + "@" + diagonal[sy][sx+1..];
            start[sy] = start[sy][0..sx-1] + "@" + start[sy][sx+1..];
            straight[sy] = straight[sy][0..sx-1] + "@" + straight[sy][sx+1..];
         }
         if (sx == x && sy == y + 1) {
            blocked_straight = 1;
            straight[sy] = straight[sy][0..sx-1] + "|" + straight[sy][sx+1..];
         }
         if (sx == x + 1 && sy == y + 1) {
            blocked_diagonal = 1;
            diagonal[sy] = diagonal[sy][0..sx-1] + "\\" + diagonal[sy][sx+1..];
         }
         if (balance >= 0) {
            sy++;
            balance -= dx;
         }
         balance += dy;
         sx++;
      }
   }
   // Need to redo back in the other direction to remove extra unneeded blocking
   // bits.
   if (first_block >= (sizeof(start) / 2)) {
      for (i = sizeof(start); i >= 6; i--) {
         // Generate a line from 0,0 to 0,i
         sx = 0;
         sy = 0;
         ex = sizeof(start);
         dx = sizeof(start);
         dy = i;
         dy <<= 1;
         balance = dy - dx;
         dx <<= 1;
         blocked = 0;
         while (sx != ex) {
            start[sy] = start[sy][0..sx-1] + " " + start[sy][sx+1..];
            if (sx == x && sy == y) {
               break;
            }
            if (balance >= 0) {
               sy++;
               balance -= dx;
            }
            balance += dy;
            sx++;
         }
      }
   }
   return ({ start, straight, diagonal });
}

private void generate_blocking_templates() {
   int x;
   int y;
   int size;
   mixed* basic;
   string* start;
   string line;

   basic = _templates[<1];
   size = sizeof(_templates);
   // Chop it into just a quarter.
   line = "";
   for (x = 0; x < size + 1; x++) {
      line += " ";
   }
   start = allocate(size + 1, line);
   _blocking_templates = allocate(size + 1, ({ }));
   for (x = 0; x < size + 1; x++) {
      _blocking_templates[x] = allocate(size + 1);
   }
   // We only need to generate for one octant.
   // That is from the straight line to the x == y line.
   for (x = 0; x < size + 1; x++) {
      for (y = x; y < size + 1; y++) {
         // Only if it is in the circle.
         if (basic[x + size][y + size] && (x != 0 || y != 0)) {
            // Generate the template for here.
            _blocking_templates[x][y] = generate_blocking(y, x, copy(start), copy(start), copy(start));
         }
      }
   }
}

/**
 * Find the template to use for the area blocked by an obstacle.
 * This will do nifty template fitting stuff for obstacles.
 * @param x the x co-ordinate of the blockage
 * @param y the y co-ordinate of the blockage
 * @param type the type is 0 for normal, 1 for straight, 2 for diagonal
 */
string *query_blocking_template(int x, int y, int type) {
   if (arrayp(_blocking_templates[x][y])) {
      return copy(_blocking_templates[x][y][type]);
   }
   return 0;
}

/**
 * This method returns the template for the specific size of the
 * map.  This is for visibility considerations.
 * @param size the size of the template
 * @return the template
 */
mixed* query_map_template(int size) {
   return copy(_templates[size - 1]);
}

/** @ignore yes */
void load_region(int x, int y) {
   class region region;
   string fname;

   if (_regions[x][y]) {
      _regions[x][y]->last_touched = time();
      return ;
   }
   fname = SAVE_DIR + "region_" + x + "_" + y;
   if (unguarded((: file_size($(fname)) :)) > 0) {
      region = unguarded( (: restore_variable(read_file($(fname), 1)) :));
   } else {
      region = new(class region,
                   terrains : allocate(REGION_SIZE, (: allocate(REGION_SIZE) :)),
                   features : ({ }));
   }
   _regions[x][y] = region;
   _regions[x][y]->last_touched = time();
}

/** @ignore yes */
void save_region(int x, int y) {
   string fname;
   class region region;

   if (_regions[x][y]) {
      region = _regions[x][y];
      fname = SAVE_DIR + "region_" + x + "_" + y;
      unguarded( (: write_file($(fname), save_variable($(region)), 1) :));
   }
}

/**
 * This method adds the specified terrain to the world map.
 */
int add_terrain(string path, int x1, int y1, int x2, int y2) {
   int tmp;
   int region_x;
   int region_y;

   if (x1 > x2) {
      tmp = x1;
      x1 = x2;
      x2 = tmp;
   }
   if (y1 > y2) {
      tmp = y1;
      y1 = y2;
      y2 = tmp;
   }

   if (x2 - x1 != BOUNDARY ||
       y2 - y1 != BOUNDARY) {
      // Not the correct size.
      debug_printf("Incorrect terrain size (%d x %d): %s\n",
                  x2 - x1, y2 - y1, path);
      return 0;
   }

   if (x1 % BOUNDARY ||
       y1 % BOUNDARY) {
      // Not  on a correct boundary.
      debug_printf("Incorrect boundary (%d - %d): %s\n",
                  x1 % BOUNDARY, y1 % BOUNDARY, path);
      return 0;
   }

   x1 = ((x1 + OFFSET) / BOUNDARY);
   y1 = ((y1 + OFFSET) / BOUNDARY);
   region_x = x1 / REGION_SIZE;
   region_y = y1 / REGION_SIZE;

   load_region(region_x, region_y);
   x2 = x1 % REGION_SIZE;
   y2 = y1 % REGION_SIZE;
   _regions[region_x][region_y]->terrains[x2][y2] = path;
   save_region(region_x, region_y);
   debug_printf("Added terrain (%d {%d}, %d {%d}) [%d,  %d]: %s\n",
                 x1, x2, y1, y2, region_x, region_y, path);
   return 1;
} /* add_terrain() */

/**
 * This method finds the terrain at the specific location.
 * @param x the x co-ordinate
 * @param y the y co-ordinate
 * @return the path to the terrain
 */
string find_terrain_at(int x, int y) {
   int region_x;
   int region_y;

   x = ((x + OFFSET) / BOUNDARY);
   y = ((y + OFFSET) / BOUNDARY);
   region_x = x / REGION_SIZE;
   region_y = y / REGION_SIZE;
   //debug_printf("Finding terrain (%d, %d) [%d,  %d]\n",
   //              x, y, region_x, region_y);
   load_region(region_x, region_y);
   return _regions[region_x][region_y]->terrains[x % REGION_SIZE][y % REGION_SIZE];
}

/**
 * This method finds the specific room at the specific location
 * @param x the x co-ordinate
 * @param y the y co-ordinate
 * @return the path to the room
 */
string find_room_at(int x, int y) {
   string path;

   path = find_terrain_at(x, y);
   if (path) {
      return path->find_room_at_real_coord(x, y);
   }
   return 0;
}

/**
 * This method adds a feature into the world map.
 * @param feature the feature to add
 */
void add_feature(string feature) {
   int x;
   int y;

   //_features += ({ feature });
   // Go through the regionsand figure out which ones this feature is in.
   for (x = 0; x < REGION_WIDTH; x++) {
      for (y = 0; y < REGION_WIDTH; y++) {
         if (feature->is_inside_region(
                                (x * REGION_WIDTH) * BOUNDARY - OFFSET,
                                (y * REGION_WIDTH) * BOUNDARY - OFFSET,
                                ((x + 1) * REGION_WIDTH) * BOUNDARY - OFFSET,
                                ((y + 1) * REGION_WIDTH) * BOUNDARY - OFFSET)) {
            load_region(x, y);
            _regions[x][y]->features |= ({ feature });
            save_region(x, y);
            debug_printf("Added feature to region %d, %d\n", x, y);
         }
      }
   }
}

/**
 * This method removes a feature from the world map.
 * @param feature the feature to remove
 */
void remove_feature(string feature) {
   int x;
   int y;

   for (x = 0; x < REGION_WIDTH; x++) {
      for (y = 0; y < REGION_WIDTH; y++) {
         load_region(x, y);
         if (member_array(feature, _regions[x][y]->features) != -1) {
            _regions[x][y]->features -= ({ feature });
            save_region(x, y);
         }
      }
   }
}

/**
 * This method returns the list of all the features available at the
 * specific location.
 * @param x the x position
 * @param y the y position
 * @return the features at the location
 */
string* query_features_at(int x, int y) {
   int region_x;
   int region_y;

   x = ((x + OFFSET) / BOUNDARY);
   y = ((y + OFFSET) / BOUNDARY);
   region_x = x / REGION_WIDTH;
   region_y = y / REGION_WIDTH;
   load_region(region_x, region_y);
   return _regions[region_x][region_y]->features;
}

/**
 * This method finds all the features inside the specified region.  This
 * is quite expensive in terms of evaluation cost, so do not try and call
 * this too much.  Cache the results if possible.
 * @param x1 the top x location
 * @param y1 the top y location
 * @param x2 the bottom x location
 * @param y2 the bottom y location
 * @return the features in the region
 */
string* query_features_in_region(int x1_orig, int y1_orig, int x2_orig, int y2_orig) {
   int region_x;
   int region_y;
   int x1;
   int y1;
   int x2;
   int y2;
   int tmp;
   string* features;
   string feat;

   x1 = ((x1_orig + OFFSET) / BOUNDARY) / REGION_WIDTH;
   y1 = ((y1_orig + OFFSET) / BOUNDARY) / REGION_WIDTH;
   x2 = ((x2_orig + OFFSET) / BOUNDARY) / REGION_WIDTH;
   y2 = ((y2_orig + OFFSET) / BOUNDARY) / REGION_WIDTH;

   if (x1 > x2) {
      tmp = x1;
      x1 = x2;
      x2 = tmp;
   }
   if (y1 > y2) {
      tmp = y1;
      y1 = y2;
      y2 = tmp;
   }
   features = ({ });

   if (x1 < 0) {
      x1 = 0;
   }
   if (y1 < 0) {
      y1 = 0;
   }
   if (x2 >= REGION_WIDTH) {
      x2 = REGION_WIDTH - 1;
   }
   if (y2 >= REGION_WIDTH) {
      y2 = REGION_WIDTH - 1;
   }

   // Now we loop through the possible regions.
   for (region_x = x1; region_x <= x2; region_x++) {
      for (region_y = y1; region_y <= y2; region_y++) {
         load_region(region_x, region_y);
         foreach (feat in _regions[region_x][region_y]->features) {
            if (feat->is_inside_region(x1_orig, y1_orig, x2_orig, y2_orig)) {
               features |= ({ feat });
            }
         }
      }
   }
   return features;
}
