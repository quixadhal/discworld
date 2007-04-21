/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: snoop_simul.c,v 1.2 1998/03/24 07:28:59 pinkfish Exp $
 * $Log: snoop_simul.c,v $
 * Revision 1.2  1998/03/24 07:28:59  pinkfish
 * Add some docs to make the simulefuns more usefully lookupable.
 *
 * Revision 1.1  1998/01/06 05:13:15  ceres
 * Initial revision
 * 
*/
/** @ignore yes */
int snoop(object sno, object snop) {
  if ("/secure/master"->valid_snoop(sno, snop, previous_object())) {
    if (!snop)
    efun::snoop(sno);
    else
      efun::snoop(sno, snop);
    return 1;
  }
  return 0;
} /* snoop() */
