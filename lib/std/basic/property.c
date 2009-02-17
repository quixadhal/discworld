/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: property.c,v 1.11 2003/05/18 12:28:46 taffyd Exp $
 */
/* /std/basic/property */
/**
 * The property handling system.  This handles all the stuff to do with
 * properties.
 *
 * @author Pinkfish
 * @change
 * Change by Pinkfish Tue Jun  3 23:32:27 WST 1997
 * to make timed properties.
 * @change
 * Change by Terano Thurs 15 October, 1998
 * Added some statistics
 */
//#define TRACK_PROPERTY_STATS 1

mapping map_prop;
mapping timed_properties;
mapping property_stats;

void create() {
  /* We don't init the mapping any more to save memory */
  //  map_prop = ([ ]);
} /* create() */

/**
 * Adds a property onto the object.  Allows you to add a property
 * onto the current object.  You can also set a time for the property
 * to last.

 * @param var the name of the property
 * @param val the value to give the property
 * @param tim the time before the property should time out
 *
 * @return 1 if it succeeded, 0 if it failed.
 */
varargs int add_property(string var, mixed val, int tim) {
  
  if (!stringp(var))
    return 0;
  
  if (!map_prop)
    map_prop = ([ ]);

  map_prop[var] = val;
  
  if (tim > 0) {
    if (!timed_properties)
      timed_properties = ([ ]);
    timed_properties[var] = tim + time();
  }
#ifdef TRACK_PROPERTY_STATS
  if (!property_stats)
    property_stats = ([ ]);

  if (previous_object())
    property_stats[var] = ({ file_name( previous_object() ), time(), tim });
  else
    property_stats[var] = ({ "No previous object.", time(), tim });
#endif  
  return 1;
} 

/**
 * Removes a property off the object.  This removes the named property
 * from the object.
 *
 * @param var the name of the property
 * @return always returns 1
 */
int remove_property(string var) {
  if (map_prop) 
    map_delete(map_prop, var);
  if (timed_properties)
    map_delete(timed_properties, var);
#ifdef TRACK_PROPERTY_STATS
  if (property_stats)
    map_delete(property_stats, var);
#endif  
  return 1;
} /* remove_property() */

/**
 * Returns the value of the property.  This will return the current value
 * of the named property.
 * 
 * @param str the property to find
 * @return the value of the property
 * @see query_property_exists(), query_is_timed_property(), 
 * query_property_time_left()
 */
mixed query_property(string str) {
  if (!map_prop)
    return ([ ])["bing"];
  
  if (timed_properties && timed_properties[str] &&
      timed_properties[str] < time()) {
    map_delete(timed_properties, str);
    map_delete(map_prop, str);
#ifdef TRACK_PROPERTY_STATS
    if(property_stats)
      map_delete(property_stats, str);
#endif    
    return map_prop[str];
  }
  
  return map_prop[str];
} /* query_property() */

/**
 * Is this property value timed?
 *
 * @return 1 if it is, 0 if not
 * @param str the property to check
 */
int query_is_timed_property(string str) {
  if (timed_properties && timed_properties[str]) {
    if (timed_properties[str] < time()) {
      map_delete(timed_properties, str);
      map_delete(map_prop, str);
#ifdef TRACK_PROPERTY_STATS     
      map_delete(property_stats, str);
#endif      
      return 0;
    }
    return 1;
  }
  return 0;
} /* query_is_timed_property() */

/**
 * If this is a timed property it returns the amount of time left.
 *
 * @param str the property name
 * @return the amount ot time left
 */
int query_property_time_left(string str) {
  if (timed_properties &&
      timed_properties[str]) {
    if (timed_properties[str] < time()) {
      map_delete(timed_properties, str);
      map_delete(map_prop, str);
#ifdef TRACK_PROPERTY_STATS     
      map_delete(property_stats, str);
#endif      
      return 0;
    }
    return timed_properties[str] - time();
  }
  return 0;
} /* query_property_time_left() */

/**
 * Does this property exist?
 *
 * @param str the property to check
 * @return 1 if the property exists, 0 if it does not
 */
int query_property_exists(string str) {
  if (!map_prop)
    return 0;
  
  if (timed_properties &&
      timed_properties[str] &&
      timed_properties[str] < time()) {
    map_delete(timed_properties, str);
    map_delete(map_prop, str);
#ifdef TRACK_PROPERTY_STATS
    map_delete(property_stats, str);
#endif    
    return map_prop[str];
  }
  
  return !undefinedp(map_prop[str]);
} /* query_property_exists() */

/**
 * returns a mapping with all the properties in it.
 *
 * @return a mapping of all the properties
 */
mapping query_properties() { 
  if (map_prop)
    return map_prop ;
  return ([ ]);
} /* query_properties() */

/**
 * Returns a mapping of all the timed properties.
 * @return a mapping of all the timed properties
 */
mapping query_timed_properties() {
  if (timed_properties)
    return timed_properties;
  return ([ ]);
} /* query_timed_properties() */

/**
 * Adds a mapping of properties in, instead of just one.
 * @param map the mapping of properties
 */
void add_properties(mapping map) {
  if (!map_prop)
    map_prop = map;
  else
    map_prop += map;
}

/**
 * Resets all properties
 * @param map the mapping of properties
 */
void set_properties(mapping map) {
  map_prop = map;
}

/**
 * Updates the timeouts and stuff before saving the player object.
 */
protected void save_player_properties() {
  string prop;

  if(timed_properties)
    foreach(prop in keys(timed_properties))
      if(timed_properties[prop] < time())
        map_delete(timed_properties, prop);
      else
        timed_properties[prop] -= time();
}

/**
 * Fixes the times back up again after it has been loaded.
 */
protected void reload_player_properties() {
  string prop;
  
  if (timed_properties)
    foreach(prop in keys(timed_properties))
      if(!map_prop[prop]) // This just does a little integrity check.
        map_delete(timed_properties, prop);
      else
        timed_properties[prop] += time();
}

/**
 * @ignore yes
 */
mixed stats() {
  int i, bing;
  mixed *k, *v, *res;
  mapping props;
  
  props = (mapping)this_object()->query_properties();
  if ( !props )
    return ({ });
  k = keys( props );
  v = values( props );
  res = ({ });
  for ( i = sizeof( k ) - 1; i > -1; i-- ) {
    
    bing = 0;
    
    if (timed_properties &&
        timed_properties[ k[ i ] ] &&
        timed_properties[ k[ i ] ] < time()) {
      map_delete(timed_properties, k[ i ]);
      map_delete(map_prop, k[ i ]);
#ifdef TRACK_PROPERTY_STATS
      if(property_stats)
        map_delete(property_stats, k[i]);
#endif      
      continue;
    }
    
    if (timed_properties &&
        timed_properties[ k[ i ] ]) {
      bing = timed_properties[ k[ i ] ] - time();
    }
    
    if ( !stringp( k[ i ] ) || k[ i ][ 0 .. 3 ] == "news" ||
         k[ i ] [ 0 ] == '.' )
      continue;
    if ( pointerp( v[ i ] ) ) {
      res += ({ ({ "(" + k[ i ] + (bing ? ":"+bing : "") + ")",
                     array_to_string( v[ i ] ) }) });
      continue;
    }
    if ( mapp( v[ i ] ) ) {
      res += ({ ({ "(" + k[ i ] + (bing ? ":"+bing : "") + ")",
                     mapping_to_string( v[ i ] ) }) });
      continue;
    }
    res += ({ ({ "("+ k[ i ] + ( bing ? ":"+ bing : "" ) +")",
                   v[ i ] }) });
  }
  return res;
} /* stats() */

#ifdef TRACK_PROPERTY_STATS
/**
 * Returns some facts about the property, in the format
 * ({ Filename of object which added stat, time added, time expiring })
 *
 * @param The property statistics are wanted on
 * @return That stuff just mentioned.
 */

mixed query_property_stats( string var ) {
  if (!property_stats || undefinedp( property_stats[var] ) ) 
    return ({ });
  return property_stats[ var ];
}

/**
 * Return all property statistics
 *
 * @return A mapping full of stats
 * @see query_property_stats
 */
mapping query_stats() {
  mixed property1, property2;

  if (!property_stats)
    return ([ ]);

  foreach( property1, property2 in property_stats ) 
    if(undefinedp(map_prop[property1]) &&
       undefinedp(timed_properties[property1]))
      map_delete(property_stats, property1);
  return property_stats;
}
#endif
