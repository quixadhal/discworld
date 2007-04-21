#include <terrain_map.h>

inherit "/cmds/base";

int cmd(int size) {
   mixed* data;
   int x;
   int y;
   string line;

   data = TERRAIN_MAP_WORLD_MAP->query_map_template(size);
   line = "";
   for (y = 0; y < sizeof(data); y++) {
      for (x = 0; x < sizeof(data[y]); x++) {
         if (data[x][y]) {
            line += "*";
         } else {
            line += " ";
         }
      }
      line += "\n";
   }
   write(line);
   return 1;
}

mixed* query_patterns() {
   return ({ "<number>", (: cmd($4[0]) :) });
}
