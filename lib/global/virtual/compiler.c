#include "virtual.h"
#include "compiler.h"

void initialize_methods();
string strip_string(string str);

mapping o_funcs,
        r_funcs,
        arm_funcs,
        wep_funcs,
        mon_funcs;
object return_ob;
int tmp_file_no;
 
void create() {
    initialize_methods();
    seteuid(getuid());
    o_funcs = O_FUN;
    r_funcs = O_FUN+R_FUN;
    arm_funcs = ARM_FUN+O_FUN;
    wep_funcs = WEP_FUN+O_FUN;
    mon_funcs = MON_FUN+O_FUN;
} /* create() */

object query_return_ob() { return return_ob; }
void set_return_ob(object ob) { return_ob = ob; }

void initialize_methods() {
    SERVER->add_method("r",  file_name(this_object()), "compile_r");
    SERVER->add_method("ob", file_name(this_object()), "compile_ob");
    SERVER->add_method("arm", file_name(this_object()), "compile_arm");
    SERVER->add_method("wep", file_name(this_object()), "compile_wep");
    SERVER->add_method("mon", file_name(this_object()), "compile_mon");
} /* initialize_methods() */

#define Error(s) write(s); log_file("VO_COMPILER", s); return 0;

mixed *extract_arguments(string str);

void do_clone(object dest, string str) {
  object ob;

  ob = clone_object(str);
  if (ob) ob->move(dest);
} /* do_clone() */

object prop_to_fun(string file, mapping funs, string class, int clone) {
    string  *segments, *ind, data, s1, dir, tmp_name;
    mixed   *val;
    int      i;

/*
 * Ok...  what we do is stick it in the same dir as the file we start
 * with, with the same name, but a nice number on ze end.
 */
    tmp_name = file+"-"+(tmp_file_no++)+".c";
    if (find_object(tmp_name))
      tmp_name->dest_me();
    ind = explode(file, "/");
    dir = implode(ind[0..sizeof(ind)-2], "/");
    rm(tmp_name);
    write_file(tmp_name,
               "#include \"virtual.h\"\n\n"+
               "void dest_me() { destruct(this_object()); }\n\n"+
               "void create() {\n"+
               "object clone;\n\n"+
               "  seteuid((string)\"/secure/master\"->creator_file(this_object()));\n");
    data = read_file(file);
    if (!data) {
        Error("prop_to_fun() : file " + file + " not found\n");
    }

    /*
     * Lines beinging with a # are a comment...
     */

    segments = explode("$\n"+data, "\n#");
    if (!segments) {
        Error("prop_to_fun() : Nothing but comments?\n");
    }
    segments[0] = segments[0][1..1000];
    for (i=1;i<sizeof(segments);i++)
      if (sscanf(segments[i], "%s\n%s", s1, segments[i]) != 2)
        segments[i] = "";
    data = implode(segments, "\n");

    /*
     * See example file for explanation of syntax.
     */

    segments = explode(strip_string(data), "::");
    ind = allocate(sizeof(segments) / 2);
    val = allocate(sizeof(segments) / 2);

    for (i = 0; i < sizeof(segments) / 2; i ++) {
        ind[i] = segments[i * 2];
        val[i] = replace(segments[i*2+1], "\n", " ");
	/*
	 * extract_arguments returns array of arguments-to-use in funcall
	 */
        if (ind[i] == "Class") {
            class = segments[i * 2 + 1];
        } else if (ind[i] == "include") {
            write_file(tmp_name, "#include <"+segments[i*2+1]+">\n");
        }
    }
    write_file(tmp_name, 
               "  clone = (object)SERVER->create_virtual_object(\""+class+
                                                               "\", 1);\n");
    write_file(tmp_name, "  clone->add_property(\"virtual name\", \""
                         +file+(clone?"#1":"")+"\");\n");

    for (i = 0; i < sizeof(ind); i ++) {
        ind[i] = lower_case(ind[i]);
        if (funs[ind[i]]) {
            if (!pointerp(funs[ind[i]])) {
                write_file(tmp_name, 
                           "  call_other(clone, \""+funs[ind[i]]+"\", "+val[i]
                                         +");\n");
/*
                           "val[i][1], 
                                  val[i][2], val[i][3], val[i][4], val[i][5]);
 */
            } else
                write_file(tmp_name,
                           "  call_other("+funs[ind[i]][1]+", "+
                           funs[ind[i]][0]+", "+val[i]+");\n");
/*
                call_other(funs[ind[i]][1], funs[ind[i]][0], clone,
                           val[i][0], val[i][1], val[i][2],
                           val[i][3], val[i][4], val[i][5]);
 */
        } else {
            write("Error: Unknown index "+ind[i]+"\n");
        }
    }
    write_file(tmp_name, "  \""+file_name(this_object())+"\"->"+
                            "set_return_ob(clone);\n} /* create() */\n");
    if (data = catch(tmp_name->bing())) {
      write("Error loading "+file+", "+data+"\n");
      rm(tmp_name);
    } else {
      tmp_name->dest_me();
      rm(tmp_name);
      return return_ob;
    }
} /* prop_to_fun() */

#define WHITESPACE(c) (c == 10 || c == 32 || c == '\n')

string strip_string(string str) {
    int i, j;

    j = strlen(str) - 1;
    for (; WHITESPACE(str[i]) && i < j; i ++);
    for (; WHITESPACE(str[j]) && j > i; j --);
    return str[i .. j];
} /* strip_space() */
      
mixed str_to_var(string str) {
    /*
     * We deal with a few different situations here.
     * If our first character is a linefeed, we have a chunk of text
     * that we will take "as is" but strip the linefeeds (for discworld)
     * ignoring the fact that spaces make exist between end-of-line and lf...
     * fuck it, that's a creator problem. :)
     *
     * After many attempts with explodes and implodes and shit, I am now
     * doing this in a LPC loop. too slow? I dunno. *shrug*
     */
    int i;

    str = strip_string(str);
    switch(str[0]) {
      case '{' : {
	  str = str[1 .. strlen(str) - 2];
	  return map_array(explode(str, ","), "str_to_var", this_object());
      }
      case '"' : {
	  str = str[1 .. strlen(str) - 2];
	  return replace(str, "\n", " ") + "\n";
      }
      case '0'..'9' :
      case '-' : {
        sscanf(str, "%d", i);
        return i;
      }
    }
    return str;
} /* str_to_var() */

mixed *extract_arguments(string str) {
    return map_array(explode(str, "//"), "str_to_var", this_object());
} /* extract_arguments() */

object compile_r(string name, int clone) {
    return prop_to_fun(name, r_funcs, "/std/room.c", clone);
} /* compile_r() */

object compile_ob(string name, int clone) {
    return prop_to_fun(name, o_funcs, "/std/object.c", clone);
} /* compile_ob() */

object compile_arm(string name, int clone) {
    return prop_to_fun(name, arm_funcs, "/obj/armour.c", clone);
} /* compile_ob() */

object compile_wep(string name, int clone) {
    return prop_to_fun(name, wep_funcs, "/obj/weapon.c", clone);
} /* compile_ob() */

object compile_mon(string name, int clone) {
    return prop_to_fun(name, mon_funcs, "/obj/monster.c", clone);
} /* compile_ob() */
