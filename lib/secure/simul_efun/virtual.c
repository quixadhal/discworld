#include "/include/virtual.h"

object virtual_find_object(string name) {
  object ob;

  if (ob = (object)SERVER->find_virtual_object(replace(name, "//", "/")))
    return ob;
  return find_object(name);
} /* find_object() */

string virtual_file_name(object ob) {
  string name;
  int i;

  if (name = (string)ob->query_property("virtual name"))
    return name;
  name = file_name(ob);
  if (sscanf(name, "%s#%d", name, i) == 2)
    return name+".c#"+i;
  return name+".c";
} /* file_name() */

object virtual_clone(string str) {
  return (object)SERVER->virtual_clone(str);
} /* clone_object() */

object virtual_load(string str) {
  return (object)SERVER->virtual_load(str);
} /* virtual_load() */
