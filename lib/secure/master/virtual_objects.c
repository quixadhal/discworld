/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: virtual_objects.c,v 1.3 2002/02/08 19:03:46 pinkfish Exp $
 * $Log: virtual_objects.c,v $
 * Revision 1.3  2002/02/08 19:03:46  pinkfish
 * Make it check for the existance of the file.
 *
 * Revision 1.2  2002/02/08 18:22:31  pinkfish
 * Add in code to allow easy setup of virtual terrains..
 * ./
 *
 * Revision 1.1  1998/01/06 05:12:03  ceres
 * Initial revision
 * 
*/
#include "/include/virtual.h"

object compile_object(string path) {
   string* bits;

   if (file_size(path) > 0) {
      return (object)SERVER->create_virtual_object(path);
   }

   // Check for a :
   bits = explode(path, ":");
   if (sizeof(bits) > 1) {
      if (file_size(bits[0]) > 0 ||
          file_size(bits[0] + ".c") > 0) {
         return bits[0]->create_virtual_object(bits[1..] ...);
      }
   }
} /* compile_object() */
