/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: query_ident.c,v 1.2 1998/03/24 07:28:59 pinkfish Exp $
 * $Log: query_ident.c,v $
 * Revision 1.2  1998/03/24 07:28:59  pinkfish
 * Add some docs to make the simulefuns more usefully lookupable.
 *
 * Revision 1.1  1998/01/06 05:13:15  ceres
 * Initial revision
 * 
*/
/**
 * Cotains some simul_efuns.
 * @author Pinkfish
 */
/** @ignore yes */
string query_ident(object ob) {
  switch (explode(file_name(previous_object()), "/")[0]) {
    case "secure" :
    case "obj" :
      return (string)ob->query_my_ident();
  }
  return 0;
} /* query_ident() */
