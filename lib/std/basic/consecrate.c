/**
 * Inheritable to handle basic consecration things.
 * We can consecrate any item without using shadows or properties.
 * Hooray!
 * @author Terano
 * @started 6/6/2001
 */

private string _god;

/**
 * @ignore
 */
void set_god( string deity ) {
  if ( stringp( deity ) ) 
    deity = lower_case( deity );
  _god =  deity;
  return;
}

/**
 * Consecrates an item
 */
void set_deity( string deity ) {
  if ( stringp( deity ) ) 
    deity = lower_case( deity );
  _god = deity;
  return;
}

/**
 * @ignore
 */
string query_god() { return _god; }

/**
 * @ignore
 */
string query_deity() { return _god; }

/**
 * Returns 1 if an item is queried to a particular god.
 * @arg God Name of the God to check.
 */
int query_consecrated( string god ) { 
  if ( !stringp( god ) ) 
    return 0;

  god = lower_case( god );

  if ( _god )
    return god == _god; 

  return this_object()->query_property( god );
}

/**
 * @ignore
 */
mixed *stats() {
   return 
      ({
         ({ "consecrated", _god }),
      });
} 

/**
 * @ignore
 */
mapping query_static_auto_load() {
   return ([
      "god" : _god      
   ]);
}

/**
 * @ignore
 */
void init_static_arg( mapping map ) {
   if ( !undefinedp( map[ "god" ] ) )
      _god = map[ "god" ];
}

