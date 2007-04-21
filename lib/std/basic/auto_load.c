/**
 * This file has generic methods of accessing auto load information.  This
 * will save memory and generaly be more useful than the previous version.
 * It shoudl also solve a lot of the container in container problems,
 * @author Pinkfish
 * @started Wed Feb 17 13:46:35 PST 1999
 */

/**
 * This method adds a value into the auto load mapping.
 * @param map the mapping to add to
 * @param file_name the file name tag of the object doing the adding
 * @param tag the tag name to be used
 * @param value the value to be stored with it.
 * @example
 * #define AUTO_LOAD_TAG "object"
 * 
 * mapping query_dynamic_auto_load() {
 *    mapping map;
 *
 *    map = ::query_dynamic_auto_load();
 *    map = add_auto_load_value(map, AUTO_LOAD_TAG, "name", name);
 * } /\* query_dynamic_auto_load() *\/
 */
mapping add_auto_load_value(mapping map,
                            string file_name,
                            string tag,
                            mixed value) {
   map[file_name + "  :  " + tag] = value;
   return map;
} /* add_auto_load_value() */

/**
 * This method will attempt to read the tag value for the given name out of
 * the auto load mapping.
 * <p>
 * This works backwards compatible with the old method of dealing with tags
 * and file names.
 * @param map the auto load mapping
 * @param file_name the file name tag to use
 * @param tag the tag to use in the file name
 * @example
 * #define AUTO_LOAD_TAG "object"
 * 
 * mapping init_dynamic_arg(mapping map) {
 *    mapping map;
 *
 *    map = ::query_dynamic_auto_load();
 *    map = add_auto_load_value(map, AUTO_LOAD_TAG, "name", name);
 * } /\* init_dynamic_arg() *\/
 */
mixed query_auto_load_value(mapping map,
                            string file_name,
                            string tag) {
   mixed tmp;

   if (tag == "::") {
      return map;
   }

   tmp = map[file_name + "  :  " + tag];
   if (tmp) {
      return tmp;
   }

   return map[tag];
} /* query_auto_load_value() */
