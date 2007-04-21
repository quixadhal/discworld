/**
 * This object handles all the different types of clothing, their
 * locations etc.
 * @author Pinkfish
 *
 * Fiddled!  Actually added the cloak and dresses areas and fixed here and
 * there.
 * @Siel, Summer 2001
 */

#include <clothing.h>

mapping _types;
mapping _equivilants;
mapping _zones;
string *_zone_level;

protected void add_clothing_type(string name,
                                 string *not_remove,
                                 string *hiding);
protected void add_clothing_zone(string zone, string *types);
private int fixup_types(string type, string *types);
protected void add_equivilant_type(string name, string equiv);
private void check_clothing_types();
string *query_clothing_zone(string zone);
string *query_clothing_cannot_remove(string type);
string can_wear_or_remove(object thing, object player);

void create() {
   _types = ([ ]);
   _equivilants = ([ ]);
   _zones = ([ ]);
   _zone_level = ({ });

   //
   // Special type for bandaids..
   //
   add_clothing_type("bandaid", ({ }), ({ }));

   /* Do not reorder this next section, is it in a nice order. */
   add_clothing_type("wig",
       ({ "hood", "helmet", "coif", "hat", "all covering" }),
       ({ "hood", "helmet", "coif", "hat", "all covering" }));
   add_clothing_type("hat",
       ({ "hood", "helmet", "all covering" }),
       ({ "hood", "helmet", "all covering" }));
   add_clothing_type("helmet",
       ({ "hood", "all covering" }),
       ({ "hood", "all covering" }));
   add_clothing_type("hood",
       ({ "all covering" }),
       ({ "all covering" }));
   add_clothing_type("earring",
       ({ "coif", "all covering" }),
       ({ "hood", "coif", "all covering" }));
   add_clothing_type("moustache",
       ({ "coif", "mask", "all covering" }),
       ({ "coif", "mask", "all covering" }));
   add_clothing_type("glasses",
       ({ "all covering" }),
       ({ "all covering" }));
   add_clothing_type("nosering",
       ({ "mask", "coif", "all covering" }),
       ({ "mask", "coif", "all covering" }));
   add_clothing_type("mask",
       ({ "all covering" }),
       ({ "all covering" }));
   add_clothing_type("headband",
       ({ "hat", "hood", "helmet", "coif", "all covering" }),
       ({ "hat", "hood", "helmet", "coif", "all covering" }));
   add_clothing_type("scarf",
       ({ "hood", "all covering" }),
       ({ "hood", "all covering"}));
   add_clothing_type("necklace",
       ({ "coif", "all covering" }),
       ({ "cloak", "coif", "all covering" }));
   add_clothing_type("collar",
       ({ "hood", "all covering" }),
       ({ "hood", "all covering"}));
   add_clothing_type( "shoulder",
       ({ "cloak", "cape", "sash", "robe",  "tabard", "breastplate",
          "hauberk", "haubergon", "byrnie", "jacket","jumper", "waistcoat",
          "shirt", "undershirt", "all covering" }),
       ({ "cloak", "robe", "tabard", "breastplate", "hauberk", "haubergon",
          "byrnie", "jacket", "jumper", "waistcoat", "shirt", "undershirt",
          "all covering" }));
   add_clothing_type( "bra",
       ({ "cloak", "cape", "sash", "robe",  "tabard", "breastplate",
          "hauberk", "haubergon", "byrnie", "jacket","jumper", "waistcoat",
          "shirt", "undershirt", "all covering", "long dress", "mini dress" }),
       ({ "cloak", "robe", "tabard", "breastplate", "hauberk", "haubergon",
           "byrnie", "jacket", "jumper", "waistcoat", "shirt", "undershirt",
           "all covering", "long dress", "mini dress" }));
   add_clothing_type("undershirt",
       ({ "cloak", "cape", "sash", "robe", "tabard", "breastplate",
          "haubergon", "hauberk", "byrnie", "jacket", "jumper", "waistcoat",
          "shirt", "harness", "all covering" }),
       ({ "cloak", "robe", "tabard", "breastplate", "haubergon", "hauberk",
          "byrnie", "jacket", "jumper", "shirt", "long dress", "mini dress",         
          "all covering" }));
   add_clothing_type("shirt",
       ({ "cloak", "cape", "sash", "robe", "tabard", "breastplate",
          "haubergon", "hauberk", "byrnie", "jacket",  "jumper", "waistcoat",
          "harness", "all covering" }),
       ({ "cloak", "robe", "haubergon", "hauberk", "byrnie", "jacket", 
          "jumper", "all covering" }));
   add_clothing_type("waistcoat",
       ({ "cloak", "cape", "sash", "robe", "tabard", "breastplate",
          "haubergon", "hauberk", "byrnie", "jacket", "jumper", "harness",
          "all covering" }),
       ({ "cloak", "robe", "haubergon", "hauberk", "byrnie", "jacket",
          "jumper", "all covering" }));
   add_clothing_type("harness",
       ({ "all covering" }),
       ({ "cloak", "all covering" }));
   add_clothing_type("jumper",
       ({ "cloak", "cape", "sash", "robe", "breastplate", "haubergon",
          "hauberk", "byrnie", "tabard", "jacket",  "all covering" }),
       ({ "cloak", "robe", "haubergon", "hauberk", "jacket", 
          "all covering" }));
   add_clothing_type("jacket",
       ({ "cloak", "cape", "sash", "robe", "tabard", "breastplate",
          "haubergon", "hauberk", "byrnie", "all covering" }),
       ({ "cloak", "robe", "hauberk", "all covering" }));


   add_clothing_type("tabard",
       ({ "cloak", "cape", "sash", "robe", "all covering" }),
       ({ "cloak", "robe", "all covering" }));
   add_clothing_type("robe",
       ({ "cloak", "cape", "sash", "all covering" }),
       ({ "cloak", "all covering" }));
   add_clothing_type("cloak",
       ({ "all covering" }),
       ({ "all covering" }));
   add_clothing_type("sash",
       ({ "cloak", "cape", "all covering" }),
       ({ "cloak", "all covering" }));
   add_clothing_type("cape",
       ({ "all covering" }),
       ({ "all covering" }));
   add_clothing_type("armband",
       ({ "all covering" }),
       ({ "cloak", "robe", "all covering" }));
   add_clothing_type("glove",
       ({ "all covering" }),
       ({ "all covering" }));
   add_clothing_type("bracelet",
       ({ "glove", "all covering" }),
       ({ "cloak", "glove", "all covering" }));
   add_clothing_type("ring",
       ({ "glove", "all covering" }),
       ({ "glove", "all covering" }));
   add_clothing_type("box",
       ({ "cloak", "robe", "all covering" }),
       ({ "cloak", "robe", "skirt", "all covering" }));
   add_clothing_type("apron",
       ({ "cloak", "all covering" }),
       ({ "cloak", "all covering" }));
   add_clothing_type("underwear",
       ({ "harness", "trousers", "greave", "hauberk", "haubergon",
          "byrnie", "belt", "trousers", "all covering" }),
       ({ "cloak", "robe", "skirt", "trousers", "all covering",
          "long dress", "mini dress" }));
   add_clothing_type("garter",
       ({ "greave", "hauberk", "trousers", "all covering"}),
       ({ "cloak", "robe", "greave", "hauberk", "trousers", "skirt",
          "all covering", "long dress", "mini dress"}));
   add_clothing_type("skirt",
       ({ "belt", "all covering" }),
       ({ "cloak", "robe", "all covering" }));
   add_clothing_type("trousers",
       ({ "greave", "hauberk", "belt" }),
       ({ "cloak", "hauberk", "robe", "all covering" }));
   add_clothing_type("belt",
       ({"hauberk", "haubergon", "byrnie", "all covering" }),
       ({ "cloak", "robe", "hauberk", "haubergon", "byrnie", "all covering" }));
   add_clothing_type("sock",
       ({ "overshoe", "boot", "chausse", "all covering" }),
       ({ "cloak", "overshoe", "boot", "chausse", "all covering",
          "long dress", "trousers" }));
   add_clothing_type("anklet",
       ({ }),
       ({ "cloak", "overshoe", "chausse", "all covering" }));
   add_clothing_type("boot",
       ({ "overshoe" }),
       ({ "overshoe", "all covering" }));
   add_clothing_type("overshoe",
       ({ }),
       ({ "all covering" }));
   add_clothing_type("long dress",
       ({ "cloak", "cape", "sash", "robe", "tabard", "breastplate",
          "haubergon", "hauberk", "byrnie", "jacket",  "jumper", "harness",
          "all covering","belt" }),
       ({ "cloak", "robe", "haubergon", "hauberk", "byrnie", "jacket",
          "jumper", "all covering" }));
   add_clothing_type("mini dress",
       ({ "cloak", "cape", "sash", "robe", "tabard", "breastplate",
          "haubergon", "hauberk", "byrnie", "jacket",  "jumper", "harness",
          "all covering","belt" }),
       ({ "cloak", "robe", "haubergon", "hauberk", "byrnie", "jacket",
          "jumper", "all covering" }));


   add_clothing_type("small shield",
       ({"all covering" }),
       ({ "all covering" }));
   add_clothing_type("shield",
       ({"all covering" }),
       ({ "all covering" }));
   add_clothing_type("large shield",
       ({"all covering" }),
       ({ "all covering" }));
   add_clothing_type("badge",
       ({ "all covering" }),
       ({ "cloak", "all covering" }));
   add_clothing_type("scabbard",
       ({ "all covering" }),
       ({ "cloak", "all covering" }));
   add_clothing_type("small scabbard",
       ({ "all covering" }),
       ({ "cloak", "robe", "all covering" }));
   add_clothing_type("belt scabbard",
       ({ "all covering" }),
       ({ "cloak", "robe", "all covering" }));

   // Maille armour

   add_clothing_type("coif",
       ({ "hood", "scarf", "mask", "helmet", "hat", "all covering"}),
       ({ "hood", "all covering"}));
   add_clothing_type("byrnie",
       ({ "cloak", "breastplate", "backplate", "tabard", "small scabbard",
          "belt scabbard", "robe", "cape", "sash", "all covering" }),
       ({ "cloak", "robe", "all covering" }));
   add_clothing_type("haubergon",
       ({ "cloak", "breastplate", "backplate", "tabard", "vambrace",
          "small scabbard", "belt scabbard", "robe", "cape", "sash",
          "all covering"}),
       ({ "cloak", "robe", "all covering"}));
   add_clothing_type("hauberk",
       ({ "cloak", "tabard", "vambrace", "greave", "breastplate", "backplate",
          "small scabbard", "belt scabbard", "robe", "cape", "sash",
          "all covering"}),
       ({ "cloak", "robe", "all covering" }));
   add_clothing_type("chausse",
       ({ "boot", "overshoe", "all covering"}),
       ({ "boot", "overshoe", "all covering"}));

   // Plate armour
   add_clothing_type("breastplate",
       ({ "cloak", "small scabbard", "belt scabbard", "robe", "tabard",
          "cape", "sash", "all covering" }),
       ({ "cloak", "robe", "all covering" }));
   add_clothing_type("backplate",
       ({ "cloak", "cape", "sash", "robe", "tabard", "all covering"}),
       ({ "cloak", "cape", "sash", "robe", "all covering"}));
   add_clothing_type("vambrace",
       ({ "glove", "all covering" }),
       ({ "cloak", "robe", "all covering" }));
   add_clothing_type("greave",
       ({ "all covering" }),
       ({ "cloak", "robe", "all covering" }));

   add_clothing_type("all covering", ({}), ({}));


   /* The clothing zones... */
   add_clothing_zone("head",
       ({ "hood", "helmet", "coif", "hat", "wig", "headband", "moustache",
          "glasses", "mask" }));
   add_clothing_zone("neck",
       ({ "cloak", "hood", "coif", "cape", "scarf", "collar" }));
   add_clothing_zone("chest",
       ({ "cloak", "sash", "robe", "breastplate", "haubergon", "hauberk",
          "byrnie", "tabard", "jacket","jumper", "shirt", "undershirt",
          "shoulder", "bra","shield", "large shield", "long dress",
          "mini dress", "apron", "waistcoat" }));
   add_clothing_zone("back",
       ({ "cloak", "cape", "sash", "robe", "hauberk", "haubergon", "byrnie",
          "jacket", "jumper", "shirt", "undershirt", "backplate",
          "long dress", "mini dress", "waistcoat" }));
   add_clothing_zone("abdomen",
       ({ "large shield", "robe", "breastplate", "byrnie", "hauberk",
          "skirt", "box", "trousers", "underwear", "long dress",
          "mini dress", "apron", "waistcoat" }));
   add_clothing_zone("arms",
       ({ "cloak", "small shield", "shield", "large shield", "robe",
          "vambrace", "hauberk", "haubergon", "jacket", "armband",
          "jumper", "shirt", "long dress" }));
   add_clothing_zone("hands",
       ({ "glove", "small shield", "shield", "large shield" }));
   add_clothing_zone("legs",
       ({ "cloak", "robe", "greave", "hauberk", "skirt", "trousers",
          "long dress" }));
   add_clothing_zone("feet",
       ({ "overshoe", "boot", "chausse", "sock", "anklet" }));

   /* The equivilant types. */
   add_equivilant_type("shoe", "boot");
   add_equivilant_type("sandal", "boot");
   add_equivilant_type("clog", "boot");
   add_equivilant_type("sabaton", "boot");
   add_equivilant_type("turban", "helmet");
   add_equivilant_type("gorget", "collar");
   add_equivilant_type("stocking", "sock");
   add_equivilant_type("coat", "cloak");
   add_equivilant_type("overall", "long dress");
   call_out((: check_clothing_types() :), 2);
} /* create() */

/**
 * This method returns the list of things that are blocking the removal
 * of the specific object.
 * @param thing the thing to find what it is being blocked by
 * @param player the player to look it up on
 * @return the array of objects blocking it
 */
object* query_items_blocking(object thing, object player) {
   int i;
   int j;
   int k;
   string *stuff;
   object *blocking;
   mixed junk;
   mixed main_types;
   object *wearing;

   main_types = thing->query_type();
   if (stringp(main_types)) {
      main_types = ({ main_types });
   }
   wearing = player->query_wearing() - ({ thing });
   blocking = ({ });
   for (k=0;k<sizeof(main_types);k++) {
      stuff = query_clothing_cannot_remove(main_types[k]);
      if (!sizeof(wearing) || !stuff) {
         return ({ });
      }
      for (i = 0; i < sizeof(wearing); i++) {
         junk = wearing[i]->query_type();
         if (stringp(junk)) {
            junk = ({ junk });
         }
         for (j = 0; j < sizeof(junk); j++) {
            if (member_array(junk[j], stuff) != -1 &&
                 member_array(wearing[i], blocking) == -1) {
               blocking += ({ wearing[i] });
               break;
            }
         }
      }
   }
   return blocking;
} /* query_items_blocking() */

/**
 * This method returns whether or not a player can wear or remove a given item.
 * @param thing the item to be worn or removed.
 * @param player the person doing the wearing or removing.
 * @return 0 for success, a string for failure.
 */
string can_wear_or_remove( object thing, object player ) {
   object *blocking;

   blocking = query_items_blocking(thing, player);
   if (sizeof(blocking)) {
     return "when you are wearing "+ query_multiple_short(blocking);
   }
   return 0;
} /* can_wear_or_remove() */

/**
 * This method adds a new clothing type into the existing clothing
 * type mapping.
 * @param name the name of the clothing type
 * @param not_remove the types which if worn you cannot remove this item
 * @param hiding the types which hides this clothing
 */
protected void add_clothing_type(string name,
                                 string *not_remove,
                                 string *hiding) {
   _types[name] = new (class clothing_type, _hiding : hiding,
                                           _not_remove : not_remove);
} /* add_clothing_type() */

/**
 * This method returns the information associated with the clothing
 * of a particular type.
 * <p>
 * This will returns a class containing the clothing type information
 * in it.
 * @param name the name of the type to lookup
 * @return the clothing type array, 0 if not found
 * @see query_clothing_cannot_remove()
 * @see query_clothing_cannot_remove()
 */
class clothing_type query_clothing_type(string name) {
   return _types[name];
} /* query_clothing_type() */

/**
 * This method returns the items you would be unable to remove the
 * type with.
 * @param type the type of clothing to check
 * @return array of types you would be unable to remove, 0 if invalid type
 * @see query_clothing_hiding()
 * @see query_clothing_depth()
 */
string *query_clothing_cannot_remove(string type) {
   class clothing_type bing;

   bing = _types[type];
   if (bing) {
      return bing->_not_remove;
   }
   return 0;
} /* query_clothing_not_remove() */

/**
 * This method returns the types which hide this type of thingy.
 * @param type the type of clothing to check
 * @return array of types which would hide this type, 0 if invalid type
 * @see query_clothing_cannot_remove()
 * @see query_clothing_depth()
 */
string *query_clothing_hiding(string type) {
   class clothing_type bing;

   bing = _types[type];
   if (bing) {
      return bing->_hiding;
   }
   return ({ });
} /* query_clothing_hiding() */

/**
 * This method returns the depth of the clothing type.  The parament
 * can also be an array, in which case it will return the minimum
 * non-zero depth.
 * @param type the type of clothing to check
 * @return the depth rating of the type
 * @see query_clothing_cannot_remove()
 * @see query_clothing_hiding()
 */
int query_clothing_depth(mixed type) {
   class clothing_type bing;
   int depth;
   string str;
   int min_depth;

   if (stringp(type)) {
      bing = _types[type];
      if (bing) {
         return bing->_depth;
      }
   } else if (pointerp(type)) {
      min_depth = 100000;
      foreach (str in type) {
         depth = query_clothing_depth(str);
         if (depth < min_depth && depth > 0) {
            min_depth = depth;
         }
      }
      return min_depth;
   }
   return 0;
} /* query_clothing_depth() */

/**
 * This method returns the depth of the clothing type.  The parament
 * can also be an array, in which case it will return the maximum
 * non-zero depth.
 * @param type the type of clothing to check
 * @return the depth rating of the type
 * @see query_clothing_cannot_remove()
 * @see query_clothing_hiding()
 */
int query_clothing_maximum_depth(mixed type) {
   class clothing_type bing;
   int depth;
   string str;
   int max_depth;

   if (stringp(type)) {
      bing = _types[type];
      if (bing) {
         return bing->_depth;
      }
   } else if (pointerp(type)) {
      max_depth = 0;
      foreach (str in type) {
         depth = query_clothing_depth(str);
         if (depth > max_depth) {
            max_depth = depth;
         }
      }
      return max_depth;
   }
   return 0;
} /* query_clothing_depth() */

/**
 * This method returns a mapping of the current clothing types.
 * <p>
 * The make up of this array is:<br>
 * <dl>
 * <dh>first element
 * <dd> is the items which if you're wearing this
 * one cannot be removed.
 * <dh>second element
 * <dd>is the items which hide the given item.
 * </dl>
 * i.e. The wig type cannot be removed if types hood, helmet
 * or hat are also worn, also the wig type is not visible
 * if the types hood, helmet or hat are worn.
 * @return the mapping of clothing types
 * @see add_clothing_type()
 */
mapping query_all_clothing_types() {
   return _types;
} /* query_clothing_types() */

/**
 * This method will go through all the current types and make sure the
 * hidden and the cannot remove types are valid.  It will also figure
 * out depth numbers for all the types.
 */
private void check_clothing_types() {
   string ty;
   class clothing_type bits;
   string *stuff;
   string bing;
   string type;
   int rank;

tell_creator("presto", "called check_clothing_types\n");
   foreach (ty, bits in _types) {
      foreach (bing in bits->_not_remove) {
         if (!query_clothing_type(bing)) {
            write("Unknown clothing type " + bing + " in the not remove "
                  " for " + ty + ".\n");
         }
      }
      foreach (bing in bits->_hiding) {
         if (!query_clothing_type(bing)) {
            write("Unknown clothing type " + bing + " in the hiding "
                  " for " + ty + ".\n");
         }
      }
   }
   /*
    * Go through each zone and assign levels to them...
    */
   rank = 1;
   foreach (bing in _zone_level) {
      stuff = query_clothing_zone(bing);
      /* Set the top levels. */
      foreach (type in stuff) {
         bits = _types[type];
         if (!bits->_depth &&
             (sizeof(bits->_not_remove) == 0  ||
             bits->_not_remove[0] == "all covering")) {
            bits->_depth = rank;
            rank = fixup_types(type, stuff);
         }
      }
      foreach (type in stuff) {
         bits = _types[type];
         if (!bits->_depth) {
tell_creator("presto", "setting depth of %O to %d\n", type, rank + 1);
            bits->_depth = rank++;
         }
      }
   }
} /* check_clothing_types() */

private int fixup_types(string type, string *types) {
   string bing;
   class clothing_type base;
   class clothing_type bits;
   int depth;

   base = _types[type];
   depth = base->_depth + 1;
   foreach (bing in types) {
      bits = _types[bing];
      /* Gravitate it upwards... */
      if (!bits->_depth &&
          member_array(type, bits->_not_remove) != -1) {
         bits->_depth = depth;
         depth = fixup_types(bing, types);
      }
   }
   return depth;
} /* fixup_types() */

/**
 * This method adds a clothing zone.
 * @param zone the zone name
 * @param types the types to add to the zone
 */
protected void add_clothing_zone(string zone, string *types) {
   string ty;

   /* Check to make sure each type exists. */
   foreach (ty in types) {
      if (!query_clothing_type(ty)) {
         write("Incorrect clothing type " + ty + " in zone " + zone + ".\n");
      }
   }
   _zones[zone] = types;
   _zone_level += ({ zone });
} /* add_clothing_zone() */

/**
 * This method sets the zones that a particular clothing type covers
 * @param type the clothing type
 * @zones the zones covered.
 */
protected void add_zone_coverage(string type, string *zones) {
  string zone;

  if(!query_clothing_type(type))
    write("Incorrect clothing type " + type + "\n");

  foreach(zone in zones)
    if(member_array(type, zones) != -1)
      _zones[zone] += ({ type });
}

/**
 * This method returns the clothing zones.  The mapping consists of
 * arrays of types which are in the particular zones.
 * @return the clothing zones
 */
mapping query_all_clothing_zones() {
   return _zones;
} /* query_clothing_zones() */

/**
 * This method returns the array of clothing types in the particular
 * zone.
 * @param zone the zone to get the types for
 * @return the clothing in the current zone
 */
string *query_clothing_zone(string zone) {
   return _zones[zone];
} /* query_clothing_zone() */

/**
 * This method finds the zone for the specific type.
 * @param type the type to find in the zones
 * @return the zone name, 0 if not found
 */
string query_zone_name(string type) {
   string zone;
   string *stuff;

   foreach (zone, stuff in _zones) {
      if (member_array(type, stuff) != -1) {
         return zone;
      }
   }
   return 0;
} /* query_zone_name() */

/**
 * This method finds all the zones for the specific type.
 * @param type the type to find in the zones
 * @return the zone names, 0 if not found
 */
string *query_zone_names(string type) {
   string zone;
   string *stuff;
   string *zones;

   if(_equivilants[type])
     type = _equivilants[type];

   zones = ({ });
   foreach (zone, stuff in _zones) {
     if (member_array(type, stuff) != -1 && zone) {
       zones += ({ zone });
     }
   }
   return zones;
} /* query_zone_name() */

/**
 * This method returns the level for the specified zone.
 * @param zone the zone to check
 * @return the level of the zone
 */
int query_zone_level(string zone) {
   int i;

   i = member_array(zone, _zone_level);
   return i * 100;
} /* query_zone_level() */

/**
 * This method adds an equivilant type.
 * @param name the name of the type
 * @param equiv what it is equivilant to
 */
protected void add_equivilant_type(string name, string equiv) {
   if (!query_clothing_type(equiv)) {
      write("Incorrect clothing type " + equiv + " in equivilant listing.\n");
   }
   _equivilants[name] = equiv;
} /*  add_equivilant_type() */

/**
 * This method returns all the equivilant types.
 * @return the equivilant types
 */
mapping query_all_equivilant_types() {
   return _equivilants;
} /* query_equivilant_types() */

/**
 * This method returns the equivilant type for the given type.
 * @param type the type to get the equivilant fo
 * @return the equivilant type
 */
string query_equivilant_type(string type) {
   return _equivilants[type];
} /* query_equivilant_type() */

/**
 * This method tells us if the type is valid.  It checks both normal
 * types and equivilant types.
 * @param type the type to check
 * @return 1 if it is valid, 0 if not
 * @query_equivilant_type()
 * @query_clothing_type()
 */
int query_valid_type(string type) {
   if (query_clothing_type(type) ||
       query_equivilant_type(type)) {
      return 1;
   }
   return 0;
} /* query_valid_type() */
