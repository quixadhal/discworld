/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: strip_colours.c,v 1.4 1999/07/11 10:07:35 pinkfish Exp $
 * $Log: strip_colours.c,v $
 * Revision 1.4  1999/07/11 10:07:35  pinkfish
 * A work around for a driver bug.
 *
 * Revision 1.3  1999/05/19 04:14:57  pinkfish
 * Fixed.
 *
 * Revision 1.2  1998/03/24 07:28:59  pinkfish
 * Add some docs to make the simulefuns more usefully lookupable.
 *
 * Revision 1.1  1998/01/06 05:13:15  ceres
 * Initial revision
 * 
*/
inherit "/secure/simul_efun/mxp";
nosave mapping term_cache;

#define TERM_HANDLER "/obj/handlers/term"

/**
 * Cotains some simul_efuns.
 * @author Pinkfish
 */
/**
 * This method strips the colour codes out of the resultant string.
 * This is used to send strings to systems outside the mud (like
 * the web and stuff.
 * @param str the string to strip
 * @return the stripped string
 */
string strip_colours(string str) {
  if (!mapp(term_cache)) {
  //if (1) {
    if (catch(term_cache = TERM_HANDLER->set_term_type("dumb"))) {
      term_cache = ([ ]);
    }
  }
  if (str != "%^") {
     return terminal_colour(str, term_cache);
  }
  return "";
} /* strip_colours() */
