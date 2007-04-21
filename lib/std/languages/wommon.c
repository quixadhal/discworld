/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: wommon.c,v 1.1 1998/01/06 04:27:40 ceres Exp $
 * $Log: wommon.c,v $
 * Revision 1.1  1998/01/06 04:27:40  ceres
 * Initial revision
 * 
*/
/*
 * The standard language unit.
 */

string long() {
  return
"International womens day language.\n";
}

string filter_string(string str) {
  return replace(" "+str,
      ({ " he ", " they ",
         " He ", " They ",
         " HE ", " THEY ",
         " his ", " theirs ",
         " His ", " Theirs ",
         " HIs ", " THEIrs ",
         " HIS ", " THEIRS ",
         " him ", " them ",
         " Him ", " Them ",
         " HIm ", " THem ",
         " HIM ", " THEM ",
         "man", "person",
         "Man", "Person",
         "MAn", "PERson",
         "MAN", "PERSON", }))[1..10000];
} /* filter_string() */

mixed garble_tell(string start, string mess) {
  return ({ start, filter_string(mess) });
} /* garble_tell() */

mixed garble_shout(string start, string mess) {
  return ({ start, filter_string(mess) });
} /* garble_shout() */

mixed garble_say(string start, string mess) {
  return ({ start, filter_string(mess) });
} /* garble_say() */

mixed garble_text(string str, object ob) {
  return filter_string(str);
} /* garble_text() */
