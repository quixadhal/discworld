/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: valid_hide.c,v 1.1 1998/01/06 05:12:03 ceres Exp $
 * $Log: valid_hide.c,v $
 * Revision 1.1  1998/01/06 05:12:03  ceres
 * Initial revision
 * 
*/
int valid_hide(object ob) {
  return query_lord(geteuid(ob));
} /* valid_hide() */
