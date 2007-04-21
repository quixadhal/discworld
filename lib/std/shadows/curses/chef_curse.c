/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: chef_curse.c,v 1.1 1998/01/06 04:35:35 ceres Exp $
 * $Log: chef_curse.c,v $
 * Revision 1.1  1998/01/06 04:35:35  ceres
 * Initial revision
 * 
*/
/* chef.x - convert English on stdin to Mock Swedish on stdout
 *
 * HISTORY
 *
 * Apr 15, 1992; John Hagerman: Created.
 */
/*
 * Taken from the above and converted into lpc by Pinkfish.
 * Converted to effects sytem by Jeremy, 5-Jun-96
 */

inherit "/std/effect_shadow";

string *replace_matrix =  ({
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

string mangle_speech( string s ) {
    string s1;

    s1 = player->mangle_speech( s );
    return replace( " " + (stringp(s1)?s1:s), replace_matrix)[1..10000];
} /* mangle_speech() */
