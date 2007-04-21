/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: c_compiler.c,v 1.1 1998/01/06 04:55:54 ceres Exp $
 * $Log: c_compiler.c,v $
 * Revision 1.1  1998/01/06 04:55:54  ceres
 * Initial revision
 * 
*/
#include "virtual.h"

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
    SERVER->add_method("c",  file_name(this_object()), "compile_c");
} /* initialize_methods() */

object compile_c(string name, int clone) {
    if (!clone) {
	name->go_ahead_make_my_womble();
	return efun::find_object(name);    /* If simul_efun done */
    } else {
	return efun::clone_object(name);   /*       -- || --     */
    }
} /* compile_c() */
