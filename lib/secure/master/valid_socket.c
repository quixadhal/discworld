/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: valid_socket.c,v 1.1 1998/01/06 05:12:03 ceres Exp $
 * $Log: valid_socket.c,v $
 * Revision 1.1  1998/01/06 05:12:03  ceres
 * Initial revision
 * 
*/
int valid_socket(object ob, string func, mixed *info) {
  string fname;

  fname = file_name(ob);
  if (func != "external") {
    switch (explode(fname, "/")[0]) {
      case "net" :
      case "secure" :
        return 1;
    }
  } else {
    if (fname[0..11] == "/secure/cmds" ||
        fname[0..19] == "/secure/rcs_handler")
      return 1;
  }
  
  return 0;
} /* valid_socket() */
