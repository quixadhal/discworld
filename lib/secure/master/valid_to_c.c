/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: valid_to_c.c,v 1.1 1998/01/06 05:12:03 ceres Exp $
 * $Log: valid_to_c.c,v $
 * Revision 1.1  1998/01/06 05:12:03  ceres
 * Initial revision
 * 
*/
int valid_compile_to_c() {
  if (previous_object() == this_object())
    return 1;
  if (previous_object() != find_object("/secure/cmds/creator/compile"))
    return 0;
  if (!interactive(previous_object(1)))
    return 0;
  if (!this_object()->query_lord(previous_object(-1)))
    return 0;
  return 1;
} /* valid_compile_to_c() */
