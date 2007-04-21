/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: server.c,v 1.4 2002/11/30 21:51:03 presto Exp $
 * $Log: server.c,v $
 * Revision 1.4  2002/11/30 21:51:03  presto
 * Check if ob was actually created in create_virtual_object before trying
 * to call functions on it
 *
 * Revision 1.3  2001/03/22 18:48:04  pinkfish
 * Make it use a property for the virtual name stuff.
 *
 * Revision 1.2  1999/06/22 02:19:11  ceres
 * Modified to save file reads
 *
 * Revision 1.1  1998/01/06 04:55:54  ceres
 * Initial revision
 * 
*/
/*
 * This is an experimental virtual-object server, written for DiscWorld
 * by Zellski. The frontend to the mudlib consists of the two functions
 * virtual_{clone, load}() which will return an object, with a 'real'
 * object as 'program' and 'variable space' but with state of properties
 * defined in some fashion by the contents of the file virtual-cloned/loaded.
 *
 * Drawbacks as yet:
 *  * The actual compilation methods are barely enough even for testing
 *    purposes but that is simple enough to expand.
 *  * The use of explode() for skipping whitespace is pathetically stupid. 
 *  * There is no support for comments in the virtual-files in prop-to-fun.
 *
 * A quick explanation of masters/clones.
 * There is no distinction as in LPC, the only one made is for finding
 * the object pointers from a virtual filename. For this there IS a 
 * virtual_clone and virtual_load that should be used. they produce much
 * the same objects, except they CAN choose to make a diff - the clone/load
 * flag is passed along to compilation. This is needed for .c files of course.
 * Remember though that even a 'master' BO, /foo/bar.x IS a clone in the end,
 * of some LPC-file. There is no support for using LPC masterobjects.
 * You can find a virtual 'master' but not a clone, you may only get a list
 * of clones made of a given file.
 */


#include "virtual.h"
 
void add_method(string suffix, mixed ob, string fun, int type);
mapping methods;

/*
 * Each entry in virtual_objects is of the form ({ master, clone1, ... }).
 * Each entry in methods on ({ object, function }).
 */

#define OB   0
#define FUN  1
#define TYPE 2

nosave mapping cache;

void create() {
  seteuid(getuid());
  methods = ([ ]);
  add_method("c", "/global/virtual/c_compiler.c", "compile_c", 
                        CLONE|LOAD);
  add_method("C", "/global/virtual/c_compiler.c", "compile_c", 
                        CLONE|LOAD);
  cache = ([ ]);
} /* create() */

object create_virtual_object(string name, int clone);

void add_method(string suffix, mixed ob, string fun, int type) {
  /*
   * teach us how to compile a new kind of file..
   */
  if (!type)
    type = CLONE;

  if (!methods[suffix]) {
    methods[suffix] = ({ ob, fun, type });
  }
} /* add_method() */

#define Error(s) write(s); log_file("VO_HANDLER", s); return 0;

string get_mod_time(string name) {
  if(cache[name])
    return cache[name];

  if(file_size(name) <= 0)
    cache[name] = "";
  else
    cache[name] = stat(name)[1];

  return cache[name];
}
object create_virtual_object(string name, int clone) {
  string *split;
  mixed  *method;
  object ob;

  split = explode(name, ".");
  method = methods[split[sizeof(split) - 1]];
  if (method) {
    if (clone ||
        (method[TYPE]&LOAD)) {
      ob =  (object) call_other(method[OB], method[FUN], name, clone);
      if (ob)  {
      ob->add_property(VIRTUAL_NAME_PROP, name);
        if (get_mod_time(name) != "")
          ob->add_property("virtual time", get_mod_time(name));
      }
      return ob;
    }
    return 0;
  }
  Error("create_virtual_object() : unknown suffix to " + name + "\n");
} /* create_virtual_object() */

void reset() {
  cache = ([ ]);
}
