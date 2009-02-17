/**
 * The basic object.  This pulls together all the bits needed to create
 * a basic object.
 *
 * @author Pinkfish
 * @change many any many times
 * @see /std/room.c
 */
#include <cmr.h>

inherit "/std/basic/extra_look";
inherit "/std/basic/enchant";
inherit "/std/basic/id";
inherit "/std/basic/misc";
inherit "/std/basic/property";
inherit "/std/basic/read_desc";
inherit "/std/basic/auto_load";
inherit "/std/basic/help_files";
inherit "/std/basic/theft_callback";
inherit "/std/basic/effects";

#define AUTO_LOAD_TAG "basic.object";

nosave int do_setup;
nosave string create_me;
nosave string colour;
private string* _materials;
nosave object player;

void create() {
  _materials = ({ });
  extra_look::create();
  id::create();
  misc::create();
  property::create();
  read_desc::create();
  effects::create();

  seteuid( (string)"/secure/master"->
      creator_file( file_name( this_object() ) ) );
  if ( this_player() ) {
    create_me = (string)this_player()->query_name();
  } else {
    create_me = "who knows";
  }
  if ( !do_setup ) {
    this_object()->setup();
  }
} /* create() */

/**
 * Gives the object a name.  Sets the name to the word and makes it the
 * short description  and attempts to create the pural short description
 * from it.
 * @param name the name to use
 */
void set_name( string word ) {
  if ( query_name() && ( query_name() != "object" ) ) {
    ::set_name(word);
    return;
  }
  ::set_name(word);
  if ( !short_d ) {
    short_d = word;
  }
  add_plural( pluralize( word ) );
} /* set_name() */

/** @ignore yes */
string query_long_details(string arg, int dark, object looker) {
   string stuff;
 
   stuff = "";
   if ( this_player()->query_see_octarine() ) {
      stuff += enchant_string();
   }
   if ( sizeof( query_read_mess() ) ) {
      stuff += "It appears to have something written on it.\n";
   }
   return stuff;
} /* query_long_details() */

/**
 * This method replaces any colours of materials in the given string
 * with the appropriate ccolours or materials for this object.
 * @param player the player attempting to check the colour/material.
 * @param text the text to replace
 * @return a new string with the dollar identities expanded.
 */ 
string replace_long_dollars(object player, string text) { 
   if ( colour ) {
      text = replace( text, "$colour$",
          (string)CMR_HANDLER->identify_colour( colour, player ) );
   }
   if ( sizeof(_materials) ) {
      text = replace( text, "$material$",
          (string)CMR_HANDLER->identify_material( _materials[0], player, 1 ) );
   }

   return text;
} /* replace_long_dollars() */ 

/**
 * The description of an object when someone looks at it.  This handles
 * the extra code for messages, colour and material.
 * @param word the word it was matched on
 * @param dark is it dark from the player's perspecitive
 * @return the long description
 */
string long( string word, int dark ) {
   string stuff;

   stuff = ::long( word, dark );
   stuff += calc_extra_look();
   stuff += query_long_details(word, dark, this_player());
   stuff = replace_long_dollars( this_player(), stuff ); 

   return stuff;
} /* long() */

/**
 * The colour of the object.
 * @return the colour of the objecy
 */
string query_colour() { return colour; }
/**
 * Sets the colour of the object.
 * @param word the colour to make the object
 */
void set_colour( string word ) { colour = word; }

/**
 * The material of the object.
 * @return the name of the material
 */
string query_material() {
   if (sizeof(_materials)) {
      return _materials[0];
   }
   return 0;
}

/**
 * This returns all of the materials of the object.
 * @return all the materials
 */
string* query_materials() {
    string *new_materials;
    
    // We need this for the moment, because a silly mistake was made
    // which was storing lots of arrays in arrays instead of just
    // an array of strings. 

    new_materials = ({ });

     if(!_materials) _materials=({ });
    foreach( mixed bit in _materials ) { 
        if ( stringp( bit ) ) {
            new_materials += ({ bit });
        }
        else {
            if ( arrayp( bit ) ) {
                foreach( mixed inner_bit in bit ) {
                    if( stringp( inner_bit ) ) {
                        new_materials += ({ inner_bit });
                    }
                }
            }
        }
    }

   return new_materials;
} /* query_materials() */
/**
 * Sets the material of the object, this adds this material to the
 * front of the material list.
 * @param word the material to set it to
 */
void set_material( mixed word ) {
   if (!arrayp(word)) {
      word = ({ word });
   }     
   // NB: It is _already_ an array so we don't need to put it in
   // brackets!  This puts an array in an array ...eek.

   if (sizeof (word)) {
      _materials = word + _materials;
      add_adjective(word);
   }
}

/**
 * This method adds a material to the list of current materials.
 * @param material the material to add
 */
void add_material(mixed material) {
   if (pointerp(material)) {
      _materials |= material;
   } else if (stringp(material)) {
      _materials += ({ material });
   }
   add_adjective(material);
} /* add_material() */

/**
 * The pronoun of the object.
 * @return always returns "it"
 */
string query_pronoun() { return "it"; }
/**
 * The possessive of the object.
 * @return always returns "its"
 */
string query_possessive() { return "its"; }
/**
 * The objective of the object.
 * @return always returns "it"
 */
string query_objective() { return "it"; }

/**
 * Who cloned the object.  Used for security reasoin to trace who have
 * been cloning up objects for players.
 * @return the person who cloned the object
 */
string query_cloned_by() { return create_me; }

/**
 * This method sets the quality of the object.  All objects are by
 * default 'normal' quality.
 * @param quality the quality of the object
 */
void set_quality(int quality) {
   add_property("quality", quality);
} /* set_qualist() */

/**
 * This method returns the quality of the object.  The quality is used
 * in the crafts system, amongst other things.
 */
int query_quality() {
   return query_property("quality");
} /* query_quality() */

/**
 * @ignore yes
 */
mixed *stats() {
  if(!_materials) _materials=({ });
  return ({
    ({ "name" , query_name(), }),
    ({ "short", short( 0 ), }),
    ({ "plural", query_plural(0), }),
    ({ "weight", (int)this_object()->query_weight(), }),
    ({ "enchantment", query_enchant(), }),
    ({ "colour", colour, }),
    ({ "material", query_multiple_short(_materials), }),
    ({ "cloned by", create_me, }),
    ({ "length", (int)this_object()->query_length(), }),
    ({ "width", (int)this_object()->query_width(), }),
   }) + property::stats() + misc::stats() + effects::stats();
} /* stats() */

/** 
 * @ignore yes
 */
mapping int_query_static_auto_load() {
  return ([
    "name" : query_name(),
    "short" : short_d,
    "main plural" : plural_d,
    "long" : long_d,
    "alias" : query_alias(1),   // not faux ones
    "adjective" : query_adjectives(1),   // not faux ones
    "plural adjective" : query_plural_adjectives(),
    "plural" : query_plurals(),
    "value" : value,
    "value info" : value_info + ([ ]),
    "weight" : weight,
    "colour" : colour,
    "length" : length,
    "width" : width,
  ]);
} /* int_query_static_auto_load() */

/** 
 * @ignore yes
 */
mapping query_static_auto_load() {
  if ( !query_name() || ( query_name() == "object" ) ) {
    return 0;
  }
  if ( explode( file_name( this_object() ), "#" )[ 0 ] != "/std/object" ) {
    return ([ ]);
  }
  return int_query_static_auto_load();
} /* query_static_auto_load() */

/** 
 * @ignore yes
 */
mapping query_dynamic_auto_load() {
   mapping map;

   if ( !query_name() || ( query_name() == "object" ) ) {
      return 0;
   }

   if ( query_enchant() > this_object()->query_max_enchant() ) {
      set_enchant(this_object()->query_max_enchant());
   }

   map = ([
      "read mess" : ::query_read_mess(),
      "degrade enchantment" : query_degrade_enchant(),
      "enchantment" : query_real_enchant(),
      "enchantment time" : query_enchant_set_time(),
      "light" : query_my_light(),
      "materials" : query_materials(),
      "cloned by" : create_me,
   ]);

   if ( map_prop ) {
      map[ "properties" ] = copy( map_prop );
   }

   if ( timed_properties ) {
      save_player_properties();
      map[ "timed properties" ] = copy(timed_properties);
   }

   if ( sizeof(query_effs() ) ) {
     effect_freeze();
     effects_saving();
     map += ([
              "effects" : ({
                (mixed *)query_effs(),
                  (int *)query_eeq()
                  })
              ]);
     effect_unfreeze();
   }
   
   if ( this_object()->query_special() ) {
      map[ "special" ] =  (mapping)this_object()->query_special_info();
   }

   if ( query_deity() ) {
     map[ "consecrated" ] = query_deity();
   }

   return map;
} /* query_dynamic_auto_load() */

/** 
 * @ignore yes
 */
void init_static_arg( mapping map ) {

   if ( !mapp( map ) ) {
      return;
   }
   if ( !undefinedp( map[ "name" ] ) ) {
      set_name(map[ "name" ]);
   }
   if ( !undefinedp( map[ "short" ] ) ) {
      set_short( map[ "short" ] );
   }
   if ( !undefinedp( map[ "main plural" ] ) ) {
      set_main_plural( map[ "main plural" ] );
   }
   if ( !undefinedp( map[ "long" ] ) ) {
      set_long( map[ "long" ] );
   }
   if ( map[ "alias" ] ) {
      set_aliases( map[ "alias" ] );
   }
   if ( map[ "adjective" ] ) {
      set_adjectives( map[ "adjective" ] );
   }
   if ( map[ "plural adjective" ] ) {
      set_plural_adjectives( map[ "plural adjective" ] );
   }
   if ( map[ "plural" ] ) {
      set_plurals( map[ "plural" ] );
   }
   if ( !undefinedp( map[ "value" ] ) ) {
      set_value( map[ "value" ] );
   }
   if ( !undefinedp( map[ "value info" ] ) ) {
      value_info = map[ "value info" ];
   }
   if ( !undefinedp( map[ "weight" ] ) ) {
      set_weight( map[ "weight" ] );
   }
   if ( !undefinedp( map[ "colour" ] ) ) {
      set_colour( map[ "colour" ] );
   }
   if ( !undefinedp( map[ "material" ] ) ) {
      set_material( map[ "material" ] );
   }                             
   if ( !undefinedp( map[ "length" ] ) ) {
      set_length( map[ "length" ] );
   }                             
   if ( !undefinedp( map[ "width" ] ) ) {
      set_width( map[ "width" ] );
   }                             
   
} /* init_static_arg() */

/**
 * Sets the player associated with the object.  This is used during the
 * autoload sequence.
 *
 * @param thing the player object we belong to
 * @see /global/auto_load.c
 */
void set_player( object thing ) { player = thing; }

/** 
 * @ignore yes
 */
void init_dynamic_arg( mapping map, object ) {
   int recycle;

   if ( !mapp( map ) )
      return;
   /*
    * Because recycling is needs to be preserved into new objects from
    * olds ones (generally speaking).
    */
   recycle = query_property("no recycling");
   if ( map[ "properties" ] )
      map_prop = map[ "properties" ];
   if (recycle)
      add_property("no recycling", recycle);
   if ( map[ "timed properties" ] ) {
      timed_properties = map[ "timed properties" ];
      reload_player_properties();
   }
   if ( sizeof( map[ "read mess" ] ) )
      set_read_mess( map[ "read mess" ] );
   if ( !undefinedp( map[ "materials" ] ) ) {
      _materials = map["materials"];
   }                             
   set_enchant(map["enchantment"]);
   set_degrade_enchant(map["degrade enchantment"]);
   if (map["enchantment time"]) {
      set_enchant_set_time(map["enchantment time"]);
   }
   set_light( map[ "light" ] );
   create_me = map[ "cloned by" ];
   if( map[ "effects" ] ) {
     set_effs( map[ "effects" ][ 0 ] );
     set_eeq( map[ "effects" ][ 1 ] );
     if ( environment() && shadow( this_object(), 0 ) )
       move_object( environment() );
   }
   init_after_save();

   if ( map[ "special" ] ) {
      clone_object( "/std/shadows/object/special_item" )->
            setup_shadow( this_object(), player );
      this_object()->set_special_info( map[ "special" ] );
   }
   if ( map[ "consecrated" ] ) {
     set_deity( map[ "consecrated" ] );
   }
} /* init_dynamic_arg() */

/** @ignore yes */
string query_readable_message(object player, int ignore_labels) {
   string ret;
   string bing;
   object *labels;
   object ob;
   int *enums;
   object store;

   ret = ::query_readable_message(player);
   if (!ignore_labels) {
      enums = (int *)this_object()->effects_matching( "object.label" );
      if ( !sizeof( enums ) ) {
         labels = ({ });
      } else {
         store = (object)this_object()->arg_of( enums[ 0 ] )[ 0 ];
         if ( !objectp( store ) ) {
            labels = ({ });
         } else {
            labels = (object *)store->find_inv_match( "" );
         }
      }
      if (sizeof(labels) && !ret) {
         ret = "";
      }
      foreach (ob in labels) {
         ret += "On " + ob->the_short() + " is written:\n";
         bing = ob->query_readable_message(player, 0);
         if (bing) {
            ret += bing;
         } else {
            ret += "Nothing.\n";
         }
      }
   }
   return ret;
} /* query_readable_message() */

/** @ignore yes */
string query_read_short(object player, int ignore_labels) {
   string ret;
   object *labels;
   int *enums;
   object store;

   ret = ::query_read_short(player);
   if (!ignore_labels) {
      enums = (int *)this_object()->effects_matching( "object.label" );
      if ( !sizeof( enums ) ) {
         labels = ({ });
      } else {
         store = (object)this_object()->arg_of( enums[ 0 ] );
         if ( !objectp( store ) ) {
            labels = ({ });
         } else {
            labels = (object *)store->find_inv_match( "" );
         }
      }
      if (sizeof(labels)) {
         if (ret) {
            ret += " and " + query_multiple_short(labels, "the") +
                   " stuck on $name$";
         } else {
            ret = query_multiple_short(labels, "the") +
                   " stuck on $name$";
         }
      }
   }
   return ret;
} /* query_read_short() */

/** @ignore yes */
string query_help_file_directory() {
   return "/doc/object/";
} /* query_help_file_directory() */

int clean_up(int inherited) {
  if(inherited) {
    log_file("CLEANUP", "%s %s cleaned up.\n", ctime(time()),
             file_name(this_object()));
    move("/room/rubbish");
  }
  return 1;
}

void dest_me() {
  effects_desting();
  ::dest_me();
}
