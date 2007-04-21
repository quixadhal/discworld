/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: new_c_compiler.c,v 1.1 1998/01/06 04:55:54 ceres Exp $
 * $Log: new_c_compiler.c,v $
 * Revision 1.1  1998/01/06 04:55:54  ceres
 * Initial revision
 * 
*/
/*
 * Virtual compiler
 */

object clone_virtual_object(string name) {
  return efun::clone_object(name);
}
