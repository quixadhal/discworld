/* /std/basic/property */
 
mapping map_prop;
 
void create() {
  map_prop = ([ ]);
}
 
int add_property(string var, mixed val) {
  int i;
 
  if (!stringp(var))
    return 0;
  if (!map_prop)
    map_prop = ([ ]);
  map_prop[var] = val;
  return 1;
}
 
int remove_property(string var) {
  int i;
 
  if (!map_prop)
    map_prop = ([ ]);
  map_prop = m_delete(map_prop, var);
  return 1;
}
 
int query_property(string str) {
  int i;
 
  if (!map_prop)
    map_prop = ([ ]);
  return map_prop[str];
}
 
int query_property_exists(string str) {
  int i;
  if (!map_prop)
    map_prop = ([ ]);
  return (member_array(str, m_indices(map_prop))) != -1;
}
 
mapping query_properties() { return map_prop ; }

int add_properties(mapping map) {
  map_prop += map;
}

/* std/basic/property: */
mixed *query_init_data() {
   return ({ "properties", map_prop, "add_properties/p/" });
} /* query_init_data() */
