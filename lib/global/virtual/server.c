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
 
mapping virtual_objects;
mapping methods;

/*
 * Each entry in virtual_objects is of the form ({ master, clone1, ... }).
 * Each entry in methods on ({ object, function }).
 */

#define OB  0
#define FUN 1

void create() {
    seteuid(getuid());
    virtual_objects = ([ ]);
    methods = ([ "c" : ({ "/global/virtual/c_compiler.c", "compile_c" })  ]);
}

object create_virtual_object(string name, int clone);

void add_method(string suffix, object ob, string fun) {
    /*
     * teach us how to compile a new kind of file..
     */
    if (!methods[suffix]) {
	methods[suffix] = ({ ob, fun });
    }
}

void remove_method(string suffix) {
    methods = m_delete(methods, suffix);
}

object find_virtual_object(string name) {
    string str;
    int num;

    if (name[0] != '/') {
	name = "/" + name;
    }
    if (sscanf(name, "%s#%d", str, num) == 2) {
      if (sizeof(virtual_objects[name]) >= num || !num)
        return 0;
      return virtual_objects[name][num];
    }
    if (virtual_objects[name]) {
	return virtual_objects[name][0];
    }
    return 0;
}

object *query_clones(string name) {
    if (virtual_objects[name]) {
	return virtual_objects[name][1 .. 1000];
    }
    return ({ });
}

varargs object virtual_clone(string name) {
    object ob;
    int i;

    ob = create_virtual_object(name, 1);
/* Don't add it into the array if it failed! */
    if (!ob || ob->query_property("virtual name")) return ob;
    if (virtual_objects[name]) {
        virtual_objects[name] = ((object *)virtual_objects[name] - ({ 0 })) +
                                ({ ob });
    } else {
	virtual_objects[name] = ({ 0, ob });
    }
    return ob;
}    

varargs object virtual_load(string name, mapping handler) {
    object  ob;
    object *master_data;

    master_data = virtual_objects[name];

    if (master_data && master_data[0]) {
        /*
         * Have we compiled this VO before? Is the 'master' still around?
         */
        return master_data[0];
    }

    ob = create_virtual_object(name, 0);

    if (ob && ob->query_property("virtual name")) {
        if (master_data) {
            /*
             * Clean up the clone-array too, remove dested clones.
             */
            master_data = ({ ob }) + master_data[1 .. 1000];
        } else {
            master_data = ({ ob });
        }
        virtual_objects[name] = master_data;
    }
    return ob;
}

#define Error(s) write(s); log_file("VO_HANDLER", s); return 0;

object create_virtual_object(string name, int clone) {
    string *split;
    mixed  *method;

    split = explode(name, ".");
    method = methods[split[sizeof(split) - 1]];
    if (method) {
	return (object) call_other(method[OB], method[FUN], name, clone);
    }
    Error("create_virtual_object() : unknown suffix to " + name + "\n");
}
