/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: base_name.c,v 1.3 2001/08/06 23:32:57 ceres Exp $
 * $Log: base_name.c,v $
 * Revision 1.3  2001/08/06 23:32:57  ceres
 * Modified to failsafe.
 *
 * Revision 1.2  1998/03/24 07:28:59  pinkfish
 * Add some docs to make the simulefuns more usefully lookupable.
 *
 * Revision 1.1  1998/01/06 05:13:15  ceres
 * Initial revision
 * 
*/
/*
 *      /secure/simul_efun/base_name.c
 *      from the Nightmare mudlib
 *      returns the name of the file of an object without extensions
 *      based on the simul_efun originally by Truilkan@Basis
 *     created by Descartes of Borg 22 december 1992
 */
/**
 * Cotains some simul_efuns.
 * @author Pinkfish
 */
/**
 * This method returns the name of the file of an object without
 * an extensions.
 * @param val the ob or string name
 * @return the base name of the object
 */
string base_name(mixed val) {
  string name, base;

  if(!val)
    return "";
  
  if (stringp(val))
    name = val;
  else
    name = file_name(val);

  if (sscanf(name, "%s#%*d", base) == 2)
    return base;

  return name;
} 
