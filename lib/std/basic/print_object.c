/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: print_object.c,v 1.1 1998/01/06 03:59:10 ceres Exp $
 * $Log: print_object.c,v $
 * Revision 1.1  1998/01/06 03:59:10  ceres
 * Initial revision
 * 
*/
/**
 * This class prints out an object in a useful format.
 * Mostly used for debugging.  The resuls of this can mostly be
 * acheived using <code>sprintf("%O", ob)</code>.
 */

/**
 * This method returns the value of the variable in 
 * useful format.  This can mostly be emulated with the
 * use of <code>sprintf("%O", ob)</code>.
 * @param ob the variable to print out info for
 * @param off the current depth offset
 * @return a nice string
 */
varargs string print_object(mixed ob, int off) {
  int i;

  write("                   "[0..off]);
  if (intp(ob)) {
    write(ob+"\n");
    return "int";
  }
  if (stringp(ob)) {
    write("\""+ob+"\"\n");
    return "string";
  }
  if (objectp(ob)) {
    write(file_name(ob)+"  ");
    if (ob->short())
      write("(short) "+ob->short()+"\n");
    else if (ob->query_name())
      write("(name)  "+ob->query_name()+"\n");
    else
      write("(Argghhhh)\n");
    return "object";
  }
  if (pointerp(ob)) {
    if (!sizeof(ob))
      write("({ })\n");
    else {
      write("({  -- size "+sizeof(ob)+"\n");
      for (i=0;i<sizeof(ob);i++)
        print_object(ob[i], off+3);
      write("                       "[0..off]+"})\n");
    }
    return "pointer";
  }
  return "oh dear";
} /* print_object() */
