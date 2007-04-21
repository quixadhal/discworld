/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: valid_override.c,v 1.2 2002/05/05 17:54:08 wodan Exp $
 * $Log: valid_override.c,v $
 * Revision 1.2  2002/05/05 17:54:08  wodan
 * Added the third argument to valid_override.
 *
 * Revision 1.1  1998/01/06 05:12:03  ceres
 * Initial revision
 * 
*/
int valid_override(string file, string func, string filename) {
  string *bing;

  bing = explode(file, "/") - ({ "", "." });
  switch (bing[0]) {
    case "secure" :
      return 1;
    case "std" :
    case "obj" :
    case "simul_efun" :
    case "global" :
    case "cmds" :
      return (func != "snoop");
    default :
      return 0;
  }
} /* valid_override() */
