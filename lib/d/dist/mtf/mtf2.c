/* MTF v1.0 Rywfol 990910 */

#include "mtfincl.h";

mixed *map;
mapping key_data;

class map_class {
  string filename;
  int counter;
  string inheritname;
  string basename;
  string short;
  string long;
  mapping exits;
}

class exit {
  string dest;
  int ypos;
  int xpos;
}

/*
 * Find how the rooms interconnect.
 * We look in a square around the item for + & - chars.
 * If we find one we've found an exit.
 */
mapping get_exits(int ypos, int xpos) {
  int x, y;
  class map_class data;
  mapping exits = ([ ]);
  
  if(!classp(map[ypos][xpos]))
    return ([ ]);

  // check the rooms either side of this one.
  for(y = -1; y < 2; y++) {
    if((y*2)+ypos < 0)
      continue;
    if((y*2)+ypos >= sizeof(map))
      continue;
    
    for(x = -1; x < 2; x++) {
      if((x*2)+xpos < 0)
        continue;
      if((x*2)+xpos >= sizeof(map[(y*2)+ypos]))
        continue;
      
      if(!x && !y)
        continue;

      if(classp(map[(y*2)+ypos][(x*2)+xpos])) {
        data = map[(y*2)+ypos][(x*2)+xpos];

        if((map[y+ypos][x+xpos] == '*') ||
           (map[y+ypos][x+xpos] == '+' && (y == 0 || x == 0)) ||
           (map[y+ypos][x+xpos] == 'x' && (y != 0 && x != 0)) ||
           (map[y+ypos][x+xpos] == '|' && y != 0 && x == 0) ||
           (map[y+ypos][x+xpos] == '-' && y == 0 && x != 0) ||
           (map[y+ypos][x+xpos] == '\\' && y == -1 && x == -1) ||
           (map[y+ypos][x+xpos] == '\\' && y == 1 && x == 1) ||
           (map[y+ypos][x+xpos] == '/' && y == 1 && x == -1) ||
           (map[y+ypos][x+xpos] == '/' && y == -1 && x == 1)) {
          exits[DIRECTIONS[y+1][x+1]] = new(class exit,
                                            dest : upper_case(data->basename)+
                                            " + \"" + data->filename + "\"",
                                            ypos : y+ypos,
                                            xpos : x+xpos);
        } else if(map[y+ypos][x+xpos] != ' ') {
          debug_printf("Unknown direction char: %O (%c), %d - %d, %d - %d %O %s",
                       map[y+ypos][x+xpos], map[y+ypos][x+xpos], x, xpos, y, ypos,
                       map[ypos][xpos],
                       data->basename);
        }
      }
    }
  }

  //  debug_printf("%0", exits);

  return exits;
}

#ifdef 0
/*
 *
 */
mixed *find_nearby(int ypos, int xpos, int distance, int direc) {
  string *dirs;
  int ny, nx;
  class exit tmp;

  if(distance > 4)
    return 0;
  
  if(!classp(map[ypos][xpos]))
    return 0;

  dirs = keys(map[ypos][xpos]->exits);
  if(direc >= sizeof(dirs))
    return 0;

  tmp = map[ypos][xpos]->exits;
  ny = tmp[]->ypos;
  nx = tmp[]->xpos;
  if(nx == xpos && ny == ypos) {
    if(direc == sizeof(dirs) -1)
      return 0;
    else
      direc++;
  }

  if(!find_nearby(ny, nx, ++distance);
  
}
#endif
     
/*
 * This is a lot like get_exits except that it goes further along the road.
 */
string calc_long(int ypos, int xpos) {
  int x, y, sameroad;
  class map_class data;
  string *exits = ({ });
  string *junctions = ({ });
  string long_str;

  mixed *nearby;
  
  if(!classp(map[ypos][xpos]))
    return "";

  /*
   * First we'll see how many exits we have that lead to this road.
   * 0 == end of the road
   * 1 == nothing special
   * 2 == fork
   */
  sameroad = 0;
  for(y = -1; y < 2; y++) {
    if((y*2)+ypos < 0)
      continue;
    if((y*2)+ypos >= sizeof(map))
      continue;
    
    for(x = -1; x < 2; x++) {
      if((x*2)+xpos < 0)
        continue;
      if((x*2)+xpos >= sizeof(map[(y*2)+ypos]))
        continue;
      
      if(!x && !y)
        continue;

      if(classp(map[(y*2)+ypos][(x*2)+xpos])) {
        data = map[(y*2)+ypos][(x*2)+xpos];
        
        // Do we have another room in this road?
        if(map[y+ypos][x+xpos] == '+' &&
           map[ypos][xpos]->basename ==
           map[(y*2)+ypos][(x*2)+xpos]->basename) {
          sameroad++;
          exits += ({ DIRECTIONS[y+1][x+1] });
        }
      }
    }
  }

  // check the rooms either side of this one to see if this is a junction.
  for(y = -1; y < 2; y++) {
    if((y*2)+ypos < 0)
      continue;
    if((y*2)+ypos >= sizeof(map))
      continue;
    
    for(x = -1; x < 2; x++) {
      if((x*2)+xpos < 0)
        continue;
      if((x*2)+xpos >= sizeof(map[(y*2)+ypos]))
        continue;
      
      if(!x && !y)
        continue;

      if(classp(map[(y*2)+ypos][(x*2)+xpos])) {
        data = map[(y*2)+ypos][(x*2)+xpos];
        
        // Do we have a junction?
        if(((map[y+ypos][x+xpos] == '*') ||
            (map[y+ypos][x+xpos] == '+' && (y == 0 || x == 0)) ||
            (map[y+ypos][x+xpos] == 'x' && (y != 0 && x != 0)) ||
            (map[y+ypos][x+xpos] == '|' && y != 0 && x == 0) ||
            (map[y+ypos][x+xpos] == '-' && y == 0 && x != 0) ||
            (map[y+ypos][x+xpos] == '\\' && y == -1 && x == -1) ||
            (map[y+ypos][x+xpos] == '\\' && y == 1 && x == 1) ||
            (map[y+ypos][x+xpos] == '/' && y == 1 && x == -1) ||
            (map[y+ypos][x+xpos] == '/' && y == -1 && x == 1)) && 
           map[ypos][xpos]->basename !=
           map[(y*2)+ypos][(x*2)+xpos]->basename) {
          if(member_array(map[(y*2)+ypos][(x*2)+xpos]->short, junctions) == -1)
            junctions += ({ map[(y*2)+ypos][(x*2)+xpos]->short });
        }
      }
    }
  }

#ifdef 0
  // Not a junction so lets start looking for junctions nearby.
  if(!sizeof(junctions)) {
    nearby = find_nearby(ypos, xpos);
  }
#endif
  
  /*
   * Build the long desc using priorities of things.
   */
  long_str = "This is ";
  if(sizeof(junctions)) {
    long_str += map[ypos][xpos]->short + " at the junction with " +
      query_multiple_short(junctions) +
      ".\n";
  } else if(sameroad == 0) {
    long_str += "the end of " + map[ypos][xpos]->short;
  } else if(sameroad == 1) {
    long_str += "a fork in " + map[ypos][xpos]->short +
      " where it splits heading " +
      query_multiple_short(exits);
  } else {
    long_str += map[ypos][xpos]->short;
  }
  

  //  debug_printf("%O", exits);
  return long_str;
}

/*
 * Write the file out.
 */
int write_this_file(string fdir, int ypos, int xpos) {
  class map_class room;
  string str, tmp, exit;
  mapping exits;
  int i;

  room = map[ypos][xpos];
  
  str = "#include \"path.h\";\n\n";
  str += "inherit " + room->inheritname + ";\n";
  str += "\n";
  str += "void setup() {\n";
  str += "  set_short(\"" + room->short + "\");\n";
  str += "  set_long(\"" + room->long + "\\n\");\n";
  str += "  set_light( 80 );\n";

  // Build the exits into the string
  exits = room->exits;

  foreach(exit in keys(exits)) {
    /*
     * A little wizardry.
     * ne,se,sw,nw are all odd numbered in the array.
     * so, if we have an odd numbered exit and the exits on either side of it
     * also exist then make it secret. This stops courtyards getting
     * overcrowded with exits.
     */
    i = member_array(exit, DIRECTIONS);
    if(i % 2 == 1 &&
       exits[DIRECTIONS[i-1]] &&
       exits[DIRECTIONS[(i+1) % sizeof(DIRECTIONS)]])
      str += sprintf("  add_exit(\"%s\", %s, \"secret\");\n",
                     exit, exits[exit]->dest);
    else
      str += sprintf("  add_exit(\"%s\", %s, \"road\");\n",
                     exit, exits[exit]->dest);
  }
  
  str += "}\n";
  
  // Create the directory if necessary and a path.h to go in it.
  if(file_size(fdir + "/" + room->basename) != -2) {
    mkdir(fdir + "/" + room->basename);
    tmp = "#include \"../path.h\"\n";
    tmp += "#undef PATH\n";
    tmp += "#define PATH __DIR__\n";
    write_file(fdir + "/" + room->basename + "/path.h", tmp, 1);
  }

  //  debug_printf("Str: %s", str);
  return write_file( fdir + "/" + room->basename + "/" + room->filename + ".c",
                     str, 1 );
}

/**
 * Read the map into a mixed array (be better if it was a class.
 */
mixed *read_map(string mapfile) {
  int i, j;
  mixed *temp_map;
  string *lines;
  
  lines = explode(read_file(mapfile), "\n");
  temp_map = allocate(sizeof(lines));

  for(i=0; i<sizeof(lines); i++) {
    temp_map[i] = allocate(strlen(lines[i]));
    for(j=0; j<strlen(lines[i]); j++) {
      if(key_data[lines[i][j]]) {
        // inc the room count.
        //        debug_printf("%c:%s", lines[i][j], key_data[lines[i][j]][2]);
        key_data[lines[i][j]][0] = to_int(key_data[lines[i][j]][0]) + 1;

        if(sizeof(key_data[lines[i][j]]) < 4)
          debug_printf("Key data too small: %O", key_data[lines[i][j]]);
        temp_map[i][j] = new(class map_class,
                             filename : key_data[lines[i][j]][2] +
                             key_data[lines[i][j]][0],
                             counter : key_data[lines[i][j]][0],
                             inheritname : key_data[lines[i][j]][1],
                             basename : key_data[lines[i][j]][2],
                             short : key_data[lines[i][j]][3]);
      } else
        temp_map[i][j] = lines[i][j];
    }
  }

  return temp_map;
}

/* Read the key file into a mapping.
 * The mapping is indexed by key character and contains
 * ({ counter, inheritname, filename, name })
 * counter is the start value for the counter (so we can start a street at some
 * value if we've already got rooms for it from another map).
 * inheritname is the name of the file to be inherited
 * filename is the base filename for the room eg. short
 * name is the name of the street eg. Short Street.
 */
mapping read_keys(string keyfile) {
  mapping key_data;
  string line, *lines;
  
  key_data = ([ ]);
  lines = explode(read_file(keyfile), "\n");
  foreach(line in lines) {
    key_data[line[0]] = explode(line[1..], "\t");
    //debug_printf("%c: %s", line[0], key_data[line[0]][2]);
  }

  return key_data;
}

/* Do the actual mapping */
int map_to_files( string mapfile, string fdir, string keyfile ) {
  int tot;
  string *file_tmp;
  string pstr;
  int x, y;
  
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

  key_data = read_keys(keyfile);
  map = read_map(mapfile);

  //  debug_printf("Map read %O", map);
  // go through the map line by line.
  for(y = 0; y < sizeof(map); y++)
    for(x = 0; x < sizeof(map[ y ]); x++)
      if(classp(map[y][x])) {
        map[y][x]->exits = get_exits(y, x);
      }

  reset_eval_cost();
  for(y = 0; y < sizeof(map); y++)
    for(x = 0; x < sizeof(map[ y ]); x++)
      if(classp(map[y][x]))
        map[y][x]->long = calc_long(y, x);

  reset_eval_cost();
  for(y = 0; y < sizeof(map); y++)
    for(x = 0; x < sizeof(map[ y ]); x++)
      if(classp(map[y][x]))
        tot += write_this_file(fdir, y, x);
  
  pstr = "/* Created at " + ctime( time() ) + " */\n";
  pstr += "#define CITYROOM \"/d/am/cityroom\"\n";
  pstr += "#undef CITY\n";
  pstr += "#define CITY \"" + fdir + "/\"\n";

  foreach(x in keys(key_data))
    pstr += "#define " + upper_case(key_data[x][2]) + " CITY + \"" + key_data[x][2] +
      "/\"\n";
  tot += write_file( fdir+ "/path.h", pstr, 1 );
  return tot;
}
