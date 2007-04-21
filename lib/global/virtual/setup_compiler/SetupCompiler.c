/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: SetupCompiler.c,v 1.1 1998/01/06 04:56:31 ceres Exp $
 * $Log: SetupCompiler.c,v $
 * Revision 1.1  1998/01/06 04:56:31  ceres
 * Initial revision
 * 
*/
#include "SetupCompiler.h"

void initialize_methods();

object return_ob;
int tmp_file_no;

void create() {
  seteuid(getuid(this_object()));
  initialize_methods();
} /* create() */

object query_return_ob() { return return_ob; }
void set_return_ob(object ob) { return_ob = ob; }

void initialize_methods() {
    SERVER->add_method("vrm",  file_name(this_object()), "compile_vrm");
    SERVER->add_method("vro",  file_name(this_object()), "compile_vro");
    SERVER->add_method("vrw",  file_name(this_object()), "compile_vrw");
    SERVER->add_method("vra",  file_name(this_object()), "compile_vra");
    SERVER->add_method("vrc",  file_name(this_object()), "compile_vrc");
} /* initialize_methods() */

#define Error(s) write(s); log_file("VO_COMPILER", s); return 0;

object prop_to_fun(string file, int clone, string incfile ) {
    string  data, data2, s1, *ind, dir, tmp_name;
    int      i;
  
    tmp_name = file+"-"+(tmp_file_no++)+".c";
    if (find_object(tmp_name))
      tmp_name->dest_me();
    ind = explode(file, "/");
    dir = implode(ind[0..sizeof(ind)-2], "/");
    unguarded((: rm, tmp_name :));
    unguarded((: write_file, tmp_name,
               "object MyRoom;\n"
               "#include \""+incfile+"\"\n\n"
               "object this_room() { return MyRoom; }\n" :));
           

    data = read_file(file);
    if (!data) {
        Error("prop_to_fun() : file " + file + " not found\n");
    }

    unguarded((: write_file, tmp_name, data :));

    unguarded((: write_file, tmp_name, "\n\n"
               "void dest_me() { destruct(this_object()); }\n\n"
               "void setup();\n\n"
               "void create() {\n"
               "  seteuid((string)\"/secure/master\"->"
               "creator_file(this_object()));\n"
               "  MyRoom = (object)\""+SERVER+
               "\"->create_virtual_object(CLONEFILE, 1);\n"
               "  MyRoom->add_property(\"virtual name\", \""+
               file+(clone?"#1":"")+"\");\n"
               "\n  setup();\n"+
               "\n MyRoom->reset(); \n"
               "  \""+file_name(this_object())+"\"->"
               "set_return_ob(MyRoom);\n} /* create() */\n" :));


    if (data = catch(tmp_name->bing())) {
      write("Error loading "+file+", "+data+"\n");
      write("File could not be loaded.\n");

    } else {
      tmp_name->dest_me();
      unguarded((: rm, tmp_name :));
      return return_ob;
    }
} /* prop_to_fun() */

object compile_vrc(string name, int clone) {
    return prop_to_fun(name, clone, PATH+ROOMFUNCS );
} 

object compile_vmc(string name, int clone) {
    return prop_to_fun(name, clone, PATH+MON_FUNCS );
} 
     
object compile_vac(string name, int clone) {
    return prop_to_fun(name, clone, PATH+ARM_FUNCS );
} 

object compile_vwc(string name, int clone) {
    return prop_to_fun(name, clone, PATH+WEP_FUNCS );
} 

object compile_voc(string name, int clone) {
    return prop_to_fun(name, clone, PATH+OBJ_FUNCS );
} 


