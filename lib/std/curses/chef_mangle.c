/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: chef_mangle.c,v 1.1 1998/01/06 04:02:57 ceres Exp $
 * $Log: chef_mangle.c,v $
 * Revision 1.1  1998/01/06 04:02:57  ceres
 * Initial revision
 * 
*/
/* chef.x - convert English on stdin to Mock Swedish on stdout
 *
 * The WC definition matches any word character, and the NW definition matches
 * any non-word character.  Two start conditions are maintained: INW (in word)
 * and NIW (not in word).  The first rule passes TeX commands without change.
 *
 * HISTORY
 *
 * Apr 15, 1992; John Hagerman: Created.
 */
/*
 * Taken from the above and converted into lpc by Pinkfish.
 */

string *replace_matrix;

void create() {
  replace_matrix =  ({
"the", "zee",
"The", "Zee",
"ir", "ur",
"ow", "oo",
"th", "t",
"au", "oo",
"Au", "Oo",
"u", "oo",
"U", "Oo",
"tion", "shun",
"an", "un",
"An", "Un",
"en ", "ee ",
"ew", "oo",
"a", "e*",
"A", "E*",
"e* ", "e ",
"E* ", "E ",
"e*", "e",
"E*", "E",
"e ", "e-a ",
"f", "ff",
" o", " oo",
" O", " Oo",
"v", "f",
"V", "F",
"w", "v",
"W", "V", });
} /* create() */

string do_transmute(string str) {
  return replace(" "+str, replace_matrix)[1..10000];
} /* do_transmute() */
