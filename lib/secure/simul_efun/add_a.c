/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: add_a.c,v 1.5 2001/06/01 21:16:33 ceres Exp $
 * $Log: add_a.c,v $
 * Revision 1.5  2001/06/01 21:16:33  ceres
 * bug?
 *
 * Revision 1.4  2000/10/04 21:34:07  pinkfish
 * Make it handle caps.
 *
 * Revision 1.3  1998/03/24 07:28:59  pinkfish
 * Add some docs to make the simulefuns more usefully lookupable.
 *
 * Revision 1.2  1998/03/06 11:26:38  pinkfish
 * Put the vowel stuff in here too.
 *
 * Revision 1.1  1998/01/06 05:13:15  ceres
 * Initial revision
 * 
*/
/**
 * Contains some simul_efuns.
 * @author Pinkfish
 */
/**
 * Adds an 'a' or 'an' to a string.  Determins if a 'a' or 'an' should be added
 * based on the ending of the word.  If you are doing this on an object you
 * should probably use a_short or the_short on the object.
 *
 * @param s the string to add an a to
 *
 * @see /std/basic/desc->a_short()
 * @see /std/basic/desc->the_short()
 * @see /std/basic/desc->one_short()
 */                
string add_a(string s) {
  int i;

  if (!stringp(s))
    return 0;
  i = 0;
  while (s[i] == ' ') i++;

  // Don't add an a or an if it already has one.
  if(s[i..i+1] == "a " || s[i..i+2] == "an ")
    return s;
  
  switch (s[i]) {
  case 'a':
  case 'e':
  case 'i':
  case 'o':
  case 'u':
  case 'A':
  case 'E':
  case 'I':
  case 'O':
  case 'U':
    return "an " + s[i..];
  default:
    return "a " + s[i..];
  }
} /* add_a() */


/*
 * What the hell would need this???
 */
/**
 * Determine if the input is a vowel.  Is the input integer a vowel?
 *
 * @param i the int to check for vowelness
 * @return 1 if the int is a vowel, 0 if not
 */
int vowel(int i) {
  return (i == 'a' || i == 'e' || i == 'i' || i == 'o' || i == 'u' ||
          i == 'A' || i == 'E' || i == 'I' || i == 'O' || i == 'U');
} /* vowel() */
