/* MTF v1.0 Rywfol 990910 */

#include "mtfincl.h";

mapping get_exits(int ypos, int xpos, mixed *map, mapping keys) {
  int x, y;
  mapping exits = ([ ]);
  string *keylist, room;

  keylist = keys(keys);

  room = map[ypos][xpos][0];
  // check the rooms either side of this one.
  for(y = -1; y < 2; y++) {
    if(y+ypos < 0)
      continue;
    if(y+ypos >= sizeof(map))
      continue;
    
    for(x = -1; x < 2; x++) {
      if(x+xpos < 0)
        continue;
      if(x+xpos >= sizeof(map[y+ypos]))
        continue;
      
      if(!x && !y)
        continue;

      if(map[y+ypos][x+xpos] != 0) {
        //debug_printf("Room %s has exit to %d,%d [%d,%d]", room, x+xpos,y+ypos, x, y);
        exits[DIRECTIONS[y+1][x+1]] = upper_case(map[y+ypos][x+xpos][3]) +
          " + \"" + map[y+ypos][x+xpos][0] + "\"";
      }
    }
  }
  //  debug_printf("%O", exits);
  return exits;
}

int write_this_file(string istring, string dir, string fname, string sname,
                    mapping exits) {
  string str, tmp, exit;
  int i;
  
  //  debug_printf("Creating %s", fname);
  
  str = "#include \"path.h\";\n\n";
  str += "inherit " + istring + ";\n";
  str += "\n";
  str += "void setup() {\n";
  str += "  set_short(\"" + sname + "\");\n";
  str += "  set_long(\"This is " + sname + " it hasn't had its long \"\n";
  str += "           \"description set yet.\\n\");\n";
  str += "  set_light( 80 );\n";

  // Remove extra exits.
  for(i=0; i<sizeof(LIST); i++) {
    if(exits[LIST[i]] &&
       exits[LIST[(i+1) % sizeof(LIST)]] &&
       exits[LIST[(i+2) % sizeof(LIST)]]) {
      /*      debug_printf("Removing %s[%d] and %s[%d], keeping %s[%d]",
                   LIST[(i) % sizeof(LIST)], i,
                   LIST[(i+2) % sizeof(LIST)], i+2,
                   LIST[(i+1) %sizeof(LIST)], i+1);
      */
      map_delete(exits, LIST[(i)]);
      map_delete(exits, LIST[(i+2) % sizeof(LIST)]);
    }
  }

  foreach(exit in keys(exits)) {
    str += sprintf("  add_exit(\"%s\", %s, \"road\");\n",
                   exit, exits[exit]);
  }
  
  str += "}\n";

  if(file_size(dir) != -2) {
    //    debug_printf("Creating directory %s", dir);
    mkdir(dir);
    tmp = "#include \"../path.h\"\n";
    tmp += "#undef PATH\n";
    tmp += "#define PATH __DIR__\n";
    write_file(dir + "/path.h", tmp, 1);
  }
    
  return write_file( dir + "/" + fname + ".c", str, 1 );
}

mixed *read_map(string mapfile, mapping keys) {
  int i, j;
  mixed *map;
  string *lines;
  
  lines = explode(read_file(mapfile), "\n");
  map = allocate(sizeof(lines));

  for(i=0; i<sizeof(lines); i++) {
    map[i] = allocate(strlen(lines[i]));
    for(j=0; j<strlen(lines[i]); j++) {
      if(keys[lines[i][j]]) {
        // inc the room count.
        keys[lines[i][j]][0] = to_int(keys[lines[i][j]][0]) + 1;
        // then give this room a filename.
        map[i][j] = ({ keys[lines[i][j]][2] + keys[lines[i][j]][0] }) + 
                         keys[lines[i][j]];
      } else {
        map[i][j] = 0;
      }
    }
  }

  log_file("/w/ceres/tmp.log", "%O", map);
  return map;
}

// Read the key file into a mapping.
mapping read_keys(string keyfile) {
  mapping keys;
  string line, *lines;
  
  keys = ([ ]);
  lines = explode(read_file(keyfile), "\n");
  foreach(line in lines)
    keys[line[0]] = explode(line[1..], "\t");

  return keys;
}

int map_to_files( string mapfile, string fdir, string keyfile ) {
  int tot;
  string *file_tmp;
  string pstr;
  int x, y;
  mapping keys;
  mixed *temp_map, temp_exits;
  
  if(!mapfile || mapfile == "")
    return NO_MAP_FILE;

  file_tmp = this_player()->get_files(mapfile);

  if(!file_tmp || sizeof(file_tmp) != 1 || file_size(file_tmp[0]) < 1)
    return NO_MAP_FILE;
  else
    mapfile = file_tmp[0];

  if(!fdir || fdir == "")
    file_tmp = ({ this_player()->query_path() });
  else
    file_tmp = this_player()->get_files(fdir);

  if(!file_tmp || sizeof(file_tmp) != 1 || file_size(file_tmp[0]) != -2)
    return NO_FILE_DIR;
  else
    fdir = file_tmp[0];
  
  if(!keyfile || keyfile == "" )
    keyfile = DEFAULT_KEYS;

  file_tmp = this_player()->get_files(keyfile);

  if(sizeof(file_tmp) != 1 || file_size(file_tmp[0]) < 1)
    return NO_KEY_FILE;
  else
    keyfile = file_tmp[0];

  keys = read_keys(keyfile);
  temp_map = read_map(mapfile, keys);

  //  debug_printf("Map read %O", temp_map);
  // go through the map line by line.
  for(y = 0; y < sizeof(temp_map); y++) {
    reset_eval_cost();
    for(x = 0; x < sizeof(temp_map[ y ]); x++) {
      if(temp_map[y][x] != 0) {
        temp_exits = get_exits(y, x, temp_map, keys);
        if(sizeof(temp_map[y][x]) < 5) {
          //          debug_printf("%O", temp_map[y][x]);
          return;
        }
        tot += write_this_file(temp_map[y][x][2],
                               fdir + "/" + temp_map[y][x][3],
                               temp_map[y][x][0], temp_map[y][x][4],
                               temp_exits);
      }
    }
  }

  //debug_printf("finished with files.");
  
  pstr = "/* Created at " + ctime( time() ) + " */\n";
  pstr += "#define CITY \"" + fdir + "/\"\n";

  foreach(x in keys(keys))
    pstr += "#define " + upper_case(keys[x][2]) + " CITY + \"" + keys[x][2] +
      "/\"\n";
  tot += write_file( fdir+ "/path.h", pstr, 1 );
  return tot;
}
