/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: general.c,v 1.2 2002/07/30 20:58:30 pinkfish Exp $
 * $Log: general.c,v $
 * Revision 1.2  2002/07/30 20:58:30  pinkfish
 * Make the garbling work.
 *
 * Revision 1.1  2002/07/30 20:53:05  pinkfish
 * Initial revision
 *
 * Revision 1.1  1998/01/06 04:27:40  ceres
 * Initial revision
 * 
*/
/*
 * THis is a language to use where anyone can understand it.
 */

string long() {
  return 
"This is the normal language spoken by most creatures.\n";
}

mixed garble_tell(string start, string mess) {
  return ({ start, mess });
}

mixed garble_shout(string start, string mess) {
  return ({ start, mess });
}

mixed garble_say(string start, string mess) {
  return ({ start, mess });
}

string garble_text( mixed text, object thing, object player, string skill) {
   string bing;

   if (stringp(text)) {
      return text;
   }

   if ( pointerp( text ) ) {
      if ( ( bing = (string)text[ 0 ]->garble_text( text, thing, player ) ) ) {
         return bing;
      }
   }

   return "You cannot read the furry writing.\n";
}


