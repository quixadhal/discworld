/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: armoury.c,v 1.34 2002/11/10 02:17:42 ceres Exp $
 */
/**
 * Keeps track of all the standard armours, weapons and clothing.
 *
 * @see /include/armoury.h
 * @index armour
 * @index weapon
 * @index scabard
 * @index clothing
 * @index clothes
 * @index jewellery
 * @index misc
 * @index plants
 * @author Furball
 * @changes Many many, by many people
 * @changes 4/1/97 Ceres
 * The discworld armoury was rewritten by Ceres on 4/1/97 to handle
 * subdirectories within the standard dirs.
 * @changes 5/2/2001 Pinkfish
 * Changed to allow the various domains to have their own armoury
 * subdirectories that can be found and specified with a flag to the
 * creation sequences.
 * @changes 23 Apr 2001 Ceres
 * Removed the recycling. It was a lot of overhead for little benefit.
 * @changes 24 Aug 2001 Siel
 * added /obj/misc/, /obj/plants/ and /obj/furnitures/ to the request list
 */
#include <move_failures.h>
#include <armoury.h>

/* two week timeout */
#define MAX_RECYCLE 6


inherit "/std/room/basic_room";

#define DEFAULT_RECYCLE_AREA "default"
#define SAVE "/save/armoury"
#define TYPES ({"armours", "clothes", "weapons", "plants", "foods", "scabbards", "jewelleries", "misc" })

private nosave int _total_requests;

private mapping _smalls;
private mapping _armour_list;
private mapping _weapon_list;
private mapping _clothing_list;
private mapping _jewellery_list;
private mapping _scabbard_list;
private mapping _food_list;
private mapping _misc_list;
private mapping _plant_list;
private mapping _areas;
private mapping forbidden;

private void rehash_all();
int rehash(string place);
string* walk_directory (string);

/** @ignore yes */
void save_me() {
   unguarded( (: save_object(SAVE, 3) :));
} /* save_me() */

/** @ignore yes */
void load_me() {
   unguarded( (: restore_object(SAVE, 1) :));
} /* load_me() */

void setup() {
   set_short( "" + mud_name() + " Armoury" );
   add_property( "determinate", "the " );
   set_light( 50 );
   set_long( "You are in the " + mud_name() + " Armoury.  From here you can list "
         "all of the \"weapons\", \"armour\", \"clothing\", \"jewellery\", "
         "\"food\", \"plants\", \"misc\" "
         "and \"scabbards\" that are available, and \"request\" one of "
         "them.  The armoury stores its list of items in mappings, which "
         "can be updated with \"rehash\".  The list of forbidden items "
         "can be listed out or added to with \"forbid\".\n" );
   _smalls = ([ ]);
   _areas = ([ ]);
   _armour_list = _weapon_list = _clothing_list = _plant_list = _misc_list =
      _jewellery_list = _scabbard_list = _food_list = ([ ]);
   // If it is in the boot sequence
   load_me();
   rehash_all();
} /* setup() */

/**
 * This returns the list of small objects available for the area.  These
 * are things that can be fgound in gutters and so on.
 * @param area the area of the recycling
 * @return the array of small objects
 */
object *query_smalls(string area) {
   if (!area) {
      area = DEFAULT_RECYCLE_AREA;
   }
   return _smalls[area];
}

/**
 * Returns the list of domain armoury items.
 * @param domain the domain/area to get the items from.
 * @return the area sub-mapping.
 */
mapping query_area(string domain) {
  if (_areas[domain]) {
      return _areas[domain];
  }

  return ([ ]);
} /* query_area() */

int request(string, int, string);
int forbid(string);

/** @ignore yes */
void init() {
   string comm;
   ::init();
   add_command("rehash", "{foods|clothes|weapons|armours|misc|jewelleries|"
                         "scabbards|plants}",
                (:rehash($4[0]) :));
   add_command("rehash", "domain <string'domain'>",
                (:rehash($4[0]) :));
   add_command("request", "<string'item name'> <number'percent'>",
                (:request($4[0], $4[1], 0):));
   add_command("request", "<string'item name'> <number'percent'> in <string'area'>",
               (:request($4[0], $4[1], $4[2]):));
   add_command("forbid", "", (:forbid, "":));
   add_command("forbid", "<string'name'>", (:forbid($4[0]):));
   foreach(comm in ({"weapons", "armour", "clothing", "jewellery", "plant",
                     "misc", "scabbards", "food", })) {
      add_command(comm, "", (:call_other(this_object(), $(comm)):));
   }
} /* init() */

/* Internal Functions */

// create a list of armours/whatever.  base_dir is the dir to look in,
// extensions is the file extensions to look for.  The base_dir will be
// recursed to find all files in sub-dirs as well.
/** @ignore yes */
mapping make_list(string base_dir, string *extensions) {
  string *dirs, dir, extension, file_name;
//   mixed *tmp, *file;
  mapping list;

  dirs = walk_directory (base_dir);

  // make a mapping of names to filenames
  list = ([ ]);
  foreach(dir in dirs) {
    foreach(extension in extensions) {
      foreach(file_name in unguarded((: get_dir, dir+"*"+extension :))) {
        if (file_name[0] != '.') {
          list[ replace_string(explode(file_name, ".")[0], "_", " ") ] =
                    dir + "" + file_name;
        }
      }
    }
  }

  return list;
} /* make_list() */


/** @ignore yes */
int clean_up(int i) { return 0; }

// External functions

/**
 * This will choose a small item from the specific area.
 * @param area the are to check in
 * @return the small object to choose
 */
object choose_small_item(string area) {
  object ob;

  if(!area)
    area = DEFAULT_RECYCLE_AREA;

  if(!_smalls || !_smalls[area] || !arrayp(_smalls[area]) || !_smalls[area][0])
    return 0;

  ob = clone_object(_smalls[area][0]);
  _smalls[area] = _smalls[area][1..];
  return ob;
}

/**
 * General item retreival function.  It returns a new item
 * it handles any sort of item.  The percentage is the
 * percentage condition in which it is returned.
 * The "word" parameter is the name of the requested item, essentially
 * the filename without directory and with "_"'s changed to " "'s. The
 * "percent" parameter is used as measure of how good it
 * is compared to a fully functional one. So a 50% dagger is only 50% as
 * good as a 100% one.
 * <p>
 * The file armoury.h defines the ARMOURY variable.  You need to include
 * this into your file to use it.  You should include this file and
 * uyse ythis define rather than the full path to the armoury, as
 * otherwise your code could stuff up :)
 * @param word the name of the item to retreive
 * @param percent the percentage condition to be in
 * @param area the area to choose the item in
 * @see /include/armoury.h
 * @return the requested object, or 0 on failure
 * @example
 * // get a slightly worn long sword and give it to the npc.
 * ARMOURY->request_item("long sword", 85)->move(npc);
 * @example
 * // Get a random condition armour.
 * armour = ARMOURY->request_item("leather jacket", random(100));
 */
object request_item(string word, int percent, string area) {
  int add_area;
  object thing;
  string filename;

  if(!word) {
    return 0;
  }
  if (!area) {
    area = DEFAULT_RECYCLE_AREA;
  }

  if (_areas[area] &&
      _areas[area][word]) {
    filename = _areas[area][word];
    add_area = 1;
  } else if(_weapon_list[word]) {
    filename = _weapon_list[word];
  } else if(_armour_list[word]) {
    filename = _armour_list[word];
  } else if(_clothing_list[word]) {
    filename = _clothing_list[word];
  } else if(_jewellery_list[word]) {
    filename = _jewellery_list[word];
  } else if(_scabbard_list[word]) {
    filename = _scabbard_list[word];
  } else if(_misc_list[word]) {
    filename = _misc_list[word];
  } else if(_plant_list[word]) {
    filename = _plant_list[word];
  } else if(_food_list[word]) {
    filename = _food_list[word];
  } else {
    if(base_name(previous_object()) != "/cmds/creator/req_uest") {
      printf( "No file for item \"%s\".\n", word );
      log_file( "ARMOURY", "%s No file for %s (%s)\n", ctime(time())[4..9],
                word, file_name( previous_object() ) );
    }
    return 0;
  }

  if(file_size( filename ) > 0)
    thing = clone_object(filename);

  if(!thing) {
    printf( "Cannot find item \"%s\".\n", word );
    log_file( "ARMOURY", "Cannot find %s (%s)\n", word,
              file_name( previous_object() ) );
    return 0;
  }

  _total_requests++;

  if(!_smalls[area])
    _smalls[area] = ({ });

  // If there's less than 20 items in the smalls list and this item meets
  // all the criteria then add it.
  if(sizeof(_smalls[area]) < 20 &&
     thing->query_weight() < roll_MdN(5, 20) && // must be small
     thing->query_value() < random(4000) && // not too valuable
     !thing->query_liquid() && // must not be a liquid
     !sizeof((mapping)thing->query_value_info()) &&
     strsrch(filename, "_pt") == -1 && // not in pt
     strsrch(filename, "_dev") == -1 && // not in dev
     !thing->query_property("no recycling")) { // must be recyclable
    _smalls[area] += ({ filename });
  }

  if (add_area)
    thing->add_property(ARMOURY_RECYCLING_AREA_PROP, area);

  thing->set_percentage( percent );
  return thing;
} /* request_item() */

/**
 * This method tries to find a match for the object if it has moved between
 * directories.
 * @param path the old path
 * @return the new path
 */
string remap_file_path(string path) {
   string* bits;
   mapping list;
   string new_fname;

   bits = explode(path, "/");
   if (bits[0] != "obj") {
      return path;
   }
   switch (bits[1]) {
   case "armours" :
      list = _armour_list;
      break;
   case "weapons" :
      list = _weapon_list;
      break;
   case "clothes" :
      list = _clothing_list;
      break;
   case "jewellery" :
      list = _jewellery_list;
      break;
   case "scabbards" :
      list = _scabbard_list;
      break;
   case "food" :
      list = _food_list;
      break;
   case "misc" :
      list = _misc_list;
      break;
   case "plants" :
      list = _plant_list;
      break;
   }

   if (list) {
      new_fname = list[replace_string(explode(bits[<1], ".")[0], "_", " ")];
      if (new_fname) {
         return new_fname;
      }
   }

   return path;
} /* remap_file_path() */

// Action functions

// Update the list of known items
/** @ignore yes */
int rehash(string thing) {
  string special;
  int found;

  switch (thing) {
  case "armours" :
     _armour_list=make_list("/obj/armours/", ({".arm", ".c"}));
     break;
  case "clothes" :
     _clothing_list=make_list("/obj/clothes/", ({".clo", ".c"}));
     break;
  case "weapons" :
     _weapon_list = make_list("/obj/weapons/", ({".wep", ".c"}));
     break;
  case "scabbards" :
     _scabbard_list = make_list( "/obj/scabbards/", ({ ".sca", ".c" }));
     break;
  case "jewellery" :
  case "jewelleries" :
     _jewellery_list = make_list( "/obj/jewellery/", ({ ".arm", ".clo", ".c" }));
     break;
  case "food" :
  case "foods" :
     _food_list = make_list( "/obj/food/", ({ ".ob", ".food", ".c" }));
     break;
  case "misc" :
     _misc_list = make_list( "/obj/misc/", ({ ".ob", ".c" }));
     break;
  case "plants" :
     _plant_list = make_list( "/obj/plants/", ({ ".ob", ".food", ".c" }));
     break;
  default :
    // Rehash the domains...
    if (file_size("/d/" + thing + "/items") == -2) {
      _areas[thing] = make_list( "/d/" + thing + "/items/",
                                  ({ ".arm", ".c", ".clo", ".wep", ".sca",
                                     ".ob", ".food" }));
      found = 1;
    }
    if (file_size("/d/" + thing + "/armoury") == -2) {
      foreach (special in get_dir("/d/" + thing + "/armoury")) {
         if (file_size("/d/" + thing + "/armoury/" + special + "/") == -2) {
           _areas[special] = make_list( "/d/" + thing + "/armoury/" + special + "/",
                                  ({ ".arm", ".c", ".clo", ".wep", ".sca" }));
         }
      }
       return 0;
    }
  }

  save_me();

  write("Rehash of " + thing + " complete.\n");
  return 1;
}

/**
 * This method rehashes a specific directory of things.  At the moment we
 * fudge this a little.
 * @param dir the directory to rehash
 * @return 1 if it found something to rehash
 */
int rehash_specific_dir(string dir) {
   return rehash(explode(dir, "/")[1]);
}

/**
 * This will only be called in the bootup sequence -- yea right! I've made it
 * work a little more slowly.
 */
private void rehash_all() {
  string thing;
  int i;

  foreach(thing in TYPES + "/secure/master"->query_domains()) {
    i += 5;

    call_out("rehash", i, thing);
  }
} /* rehash_all() */

/** @ignore yes */
string *query_types() { return TYPES + keys(_areas); }

/** @ignore yes */
mapping query_items(string type, string filter) {
  mapping items;
  string item;

  switch(type) {
  case "armours":
    items = _armour_list;
    break;
  case "weapons":
    items = _weapon_list;
    break;
  case "clothes":
    items = _clothing_list;
    break;
  case "jewellery":
  case "jewelleries":
    items = _jewellery_list;
    break;
  case "scabbards":
    items = _scabbard_list;
    break;
  case "food":
  case "foods":
    items = _food_list;
    break;
  case "misc":
    items = _misc_list;
    break;
  case "plants":
    items = _plant_list;
    break;
  default:
    if(member_array(type, "/secure/master"->query_domains()) != -1 &&
       _areas[type])
      items = _areas[type];
    else
      return 0;
  }

  items = copy (items);
  if (sizeof (filter)) {
    foreach(item in keys(items))
      if(strsrch(item, filter) == -1)
        map_delete(items, item);
  }
  return items;
}

// Return an item for someone
/** @ignore yes */
int request( string word, int percentage, string area ) {
  object thing;

  thing = request_item( word, percentage, area );

  if ( !thing ) {
    add_failed_mess(word + " not found.\n");
    return 0;
  }
  if ( (int)thing->move( this_player() ) != MOVE_OK ) {
    write( (string)thing->a_short() +" has been placed in here.\n" );
    thing->move( this_object() );
  } else
    write( (string)thing->a_short() +
          " has been placed in your inventory.\n" );
  return 1;
} /* request() */

/** @ignore yes */
mixed stats() {
  string tmp;
  mixed *ret;

  ret = ({ });
  foreach(tmp in keys(_areas)) {
    ret += ({ tmp, sizeof(_areas[tmp]) });
  }
  
  return ::stats() + ({
    ({ "item requests", _total_requests, }),
      ({ "armours", sizeof(_armour_list) }),
      ({ "clothes", sizeof(_clothing_list) }),
      ({ "weapons", sizeof(_weapon_list) }),
      ({ "scabbards", sizeof(_scabbard_list) }),
      ({ "jewellery", sizeof(_jewellery_list) }),
      ({ "food", sizeof(_food_list) }),
      ({ "misc", sizeof(_misc_list) }),
      ({ "plants", sizeof(_plant_list) }),
      ret });
} /* stats() */

/** @ignore yes */
string* walk_directory (string dir) {
  string *tmp, *dirs = ({ }), *tmp2;
  mixed *file;

  tmp = get_dir(dir, -1);

  if (sizeof (tmp))
    dirs += ({dir});

  foreach(file in tmp) {
    if(file[1] == -2) {
      tmp2 = walk_directory (dir + file [0] + "/");
      if (sizeof (tmp2))
        dirs += tmp2;
    }
  }

  return dirs;
}
