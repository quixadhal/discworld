/*
 * House creation object
 */
inherit "/std/object";

#include <housing.h>

#define ORIENTATION ({ "north", "northeast", "east", "southeast", "south", "southwest", "west", "northwest", "up" })
#define OPP_ORIENTATION ({ "south", "southwest", "west", "northwest", "north", "northeast", "east", "southeast", "down" })

#define PATH "/std/houses/"

int do_create(string type, string orientation, string location, string out,
              string region, string address, int overwrite,
              int test);
string list_plans();

void setup() {
  set_name("ball");
  set_short("small jade ball");
  add_adjective(({"small", "jade"}));
  set_long("This non-descript jade ball can be used to make player houses.\n");
  set_read_mess((: list_plans() :));
  set_weight(1);
  set_value(0);
  seteuid("Room");
}

void init() {
  this_player()->add_command("make", this_object(),
           "<word'floor plan'> <word'orientation'> "
           "<word'location'> <word'outside'> <word'region'> <string:quoted'address'>",
           (: do_create($4[0], $4[1], $4[2], $4[3], $4[4], $4[5], 0, 1) :) );
  this_player()->add_command("makereal", this_object(),
           "<word'floor plan'> <word'orientation'> "
           "<word'location'> <word'outside'> <word'region'> <string:quoted'address'>",
           (: do_create($4[0], $4[1], $4[2], $4[3], $4[4], $4[5], 0, 0) :) );
  this_player()->add_command("maketest", this_object(),
           "<word'floor plan'> <word'orientation'> "
           "<word'location'> <word'outside'> <word'region'> <string:quoted'address'>",
           (: do_create($4[0], $4[1], $4[2], $4[3], $4[4], $4[5], 1, 1) :) );
}

string list_plans() {
  string ret, str;
  string plan;
  
  ret = "To create a house use:\n"
    "  'make <floor plan> <orientation> <location> <outside> <region>\n"
    "        <address>' where:\n"
    "  'makereal <floor plan> <orientation> <location> <outside> <region>\n"
    "        <address>' where:\n"
    "  'makeforce <floor plan> <orientation> <location> <outside> <region>\n"
    "        <address>' where:\n"
    "  floor plan is one of the standard floor plans\n"
    "  orientation is its orientation (ie. the direction you enter it to)\n"
    "  location is the directory to write the houses files into\n"
    "  address is a the unique address for the house\n"
    "  region is the region of the house\n"
    "The 'makereal' command will add the house into the housing handler, "
    "the 'make' command will do a normal test make and the 'makeforce' "
    "will overwrite any existing housing files.\n\n"
    "Example:\n"
    "$I$5=   makereal twobedhouse south /d/am/fluff/housing OUT+room1 am \"12 Fluff Street\"\n\n$I$0="
    "The following floorplans are available:\n";

  str = "";
  foreach(plan in get_dir(PATH + "*")) {
    if(plan != "." && plan != ".." && file_size(PATH + plan) == -2 )
      str += plan + "\n";
  }
  
  ret += sprintf("%-#*s", this_player()->query_cols(), str);
  return ret;
}

int do_create(string type, string orientation, string location, string out,
              string region, string address, int overwrite,
              int test) {
  string room;
  string tmp;
  string fname;
  mapping files;
  mapping main_file;
  string number;
  int orient;
  int i;
  
  if(file_size(PATH+type) != -2) {
    return notify_fail("No such floor plan\n");
  }

  orient = member_array(orientation, ORIENTATION);
  if(orient == -1) {
    return notify_fail("No such orientation.\n");
  }

  if(file_size(location) != -2) {
    return notify_fail("No such location.\n");
  }
  
  // Get the number from the address.
  if (address[0] < '0' || address[0] > '9') {
     add_failed_mess("Address must start with a number.\n");
     return 0;
  }

  i = 0;
  do {
     i++;
  } while (address[i] != ' ');
  number = address[0..i-1];
  address = address[i..];

  // Try and figure out the real 'out' exit.
  if (out[0] == '/') {
     if (out[<2..] == ".c") {
        out = out[0..<3];
     }
     out = "\"" + out + "\"";
  } else {
     // If there is a '+' in here assume we have a define.
     // Otherwise put a PATH + in front.
     if (member_array('+', out) == -1) {
        out = "PATH+\"" + out + "\"";
     }
  }

  files = ([ ]);
  main_file = ([ ]);
  if (type[<1] != 'M') {
     main_file[""] = 0;
  }

  foreach(room in get_dir(PATH+type+"/*.c")) {
    fname = location + "/" + replace_string(room, "N", number);
    if(file_size(fname) != -1 && !overwrite) {
      write("Error: file already exists: " + fname + ".\n");
      continue;
    }
    
    tmp = read_file(PATH+type+"/"+room);
    if(!tmp || tmp == 0) {
      return notify_fail("Error reading " + PATH+type+"/"+room + ".\n");
    }

    // exit and wall stuff, figuring out correct orientation.
    for(i=0; i<sizeof(ORIENTATION); i++) {
      tmp = replace_string(tmp, "%"+i+"%", ORIENTATION[(i + orient) % 8]);
    }

    // House number
    tmp = replace_string(tmp, "%N%", number);
    // Save directory
    tmp = replace_string(tmp, "%savedir%", "SAVE_DIR");
    if (strsrch(tmp, "MAIN ROOM") != -1) {
       if (type[<1] == 'M') {
          main_file[room[1..1]] = fname;
       } else {
          main_file[""] = fname;
       }
    } else {
       if (type[<1] == 'M') {
          if (!files[room[1..1]]) {
             files[room[1..1]] = ({ });
          }
          files[room[1..1]] += ({ fname });
       } else {
          if (!files[""]) {
             files[""] = ({ });
          }
          files[""] += ({ fname });
       }
    }
    tmp = replace_string(tmp, "%out%", out);
    tmp = replace_string(tmp, "%orientation%", "" + (orient % 2));
    write("Writing " + fname + ".\n");
    write_file(fname, tmp, 1);
  }

  // Add the house into the housing handler.
  if (!test) {
     foreach (tmp, room in main_file) {
        if (tmp == "-") {
           continue;
        }
        if (!room) {
           room = files[tmp][0];
           files[tmp] = files[tmp][1..];
        }
        if (room[<2..] == ".c") {
           room = room[0..<3];
        }
        if (files[tmp]) {
           files[tmp] = map(files[tmp], (: ($1[<2..] == ".c"?$1[0..<3]:$1) :));         } else {
           files[tmp] = ({ });
        }
        HOUSING->add_house(room, files[tmp], number + tmp + address, region, 1);
        HOUSING->set_for_sale(room);
     }
  }


  if (!main_file["-"]) {
     tmp = main_file[""];
  } else {
     tmp = main_file["-"];
  }

  write("You need to put the following code into " + out + " if the "
        "house entrance is a door (NB "
        "if you are doing this as an up/down exit then change the "
        "direction set below):\n");
  write("   add_exit(\"" + orientation + "\", \"" + tmp + "\", \"door\");\n");
  write("   \"" + tmp + "\"->setup_doors(); // this may not be needed check\n\n");
  write("You need to put the following code into " + out + " if the "
        "house entrance is not a door:\n");
  write("   add_exit(\"" + orientation + "\", \"" + tmp + "\", \"path\");\n");
  return 1;
}

void init_dynamic_arg(mapping map) {
   ::init_dynamic_arg(map);
   set_read_mess( (: list_plans() :) );
} /* init_dynamic_arg() */
