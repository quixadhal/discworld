/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: simul_efun.c,v 1.1 1998/01/06 05:12:03 ceres Exp $
 * $Log: simul_efun.c,v $
 * Revision 1.1  1998/01/06 05:12:03  ceres
 * Initial revision
 * 
*/
/*
 * Give a path to a simul_efun file. Observe that it is a string returned,
 * not an object. But the object has to be loaded here. Return 0 if this
 * feature isn't wanted.
 */
string get_simul_efun() {
  string fname;
  fname = "/secure/simul_efun";
  if (catch(call_other(fname, "??"))) {
    write("Failed to load " + fname + "\n");
    shutdown();
    return 0;
  }
  return fname;
} /* get_simul_efun() */
