/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: data_compiler.c,v 1.1 1998/01/06 04:55:54 ceres Exp $
 * $Log: data_compiler.c,v $
 * Revision 1.1  1998/01/06 04:55:54  ceres
 * Initial revision
 * 
*/
#include "virtual.h"
#include <soul.h>
#include <random_names.h>

/*
 * This is the default .c extension compiler.  This is the default
 * so that basicly everything else manages to work correctly.
 * Could be bad if this method was lost...
 */
void initialize_methods();
 
void create() {
  initialize_methods();
  seteuid(getuid());
} /* create() */

void initialize_methods() {
  SERVER->add_method("s",  file_name(this_object()), "compile_soul", 1);
  SERVER->add_method("r_n",  file_name(this_object()), "compile_random_name", 1);
} /* initialize_methods() */

object compile_soul(string name, int) {
  object ob;

  if (name[0] != '/')
    name = "/"+name;
  if (name[0..strlen(SOUL_DIR)-1] != SOUL_DIR) {
    return 0;
  }
  name = name[strlen(SOUL_DIR)..];
  SOUL_COMPILER->compile_file(name);
  ob = clone_object("/std/object");
  ob->set_name("compile");
  ob->set_short("Compiling "+name+" now");
  call_out((: $(ob)->dest_me() :), 2);
  return ob;
} /* compile_soul() */

object compile_random_name(string name, int clone) {
  object ob;

  if (name[0] != '/')
    name = "/"+name;
  if (name[0..strlen(RANDOM_NAME_DATA_DIR)-1] != RANDOM_NAME_DATA_DIR) {
    return 0;
  }
  name = name[strlen(RANDOM_NAME_DATA_DIR)..];
  RANDOM_NAME_GENERATOR->open_file(name);
  ob = clone_object("/std/object");
  ob->set_name("compile");
  ob->set_short("Compiling "+name+" now");
  call_out((: $(ob)->dest_me() :), 2);
  return ob;
} /* compile_random_name() */
