/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: valid_binary.c,v 1.5 2003/02/11 08:59:32 pinkfish Exp $
 * $Log: valid_binary.c,v $
 * Revision 1.5  2003/02/11 08:59:32  pinkfish
 * Add in the /www directory ot the binary list.
 *
 * Revision 1.4  2000/06/19 04:15:54  pinkfish
 * Allow binaries to be created for the domain directories too.
 *
 * Revision 1.3  1998/08/19 08:04:54  pinkfish
 * Add in the cmds dir to the list which binaries are saved for.,
 *
 * Revision 1.2  1998/08/17 13:06:04  pinkfish
 * Turn on binaries for he secure dir as well.
 *
 * Revision 1.1  1998/01/06 05:12:03  ceres
 * Initial revision
 * 
*/
int valid_save_binary(string fname) {
  string *bits;

  bits = explode(fname, "/");
  switch (bits[0]) {
/*
    case "secure" :
      if ( bits[ 1 ] == "simul_efun" )
        return 0;
*/
    case "global" :
    case "std" :
    case "secure" :
    case "cmds" :
    case "d" :
    case "www" :
      return 1;
    case "obj" :
      //if (sscanf(bits[sizeof(bits)-1], "%*s-%*d%*s") == 0)
        return 1;
  }
  return 0;
} /* valid_save_binary() */
