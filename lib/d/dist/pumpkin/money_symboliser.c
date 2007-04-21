/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: money.c,v 1.6 1998/12/02 07:27:55 pinkfish Exp $
 *
 * $Log: money.c,v $
 * Revision 1.6  1998/12/02 07:27:55  pinkfish
 * Make it handle -ve numbers.
 *
 * Revision 1.5  1998/09/10 19:46:51  pinkfish
 * Fix it up to handle the pence shortening as well.
 *
 * Revision 1.4  1998/08/18 23:12:06  pinkfish
 * Fix a spelling mistake.
 *
 * Revision 1.3  1998/08/18 20:08:16  pinkfish
 * Get it to handle a$ as well as just $
 *
 * Revision 1.2  1998/08/14 10:30:44  pinkfish
 * aAdd in code to turn a string back into a bvalue.
 *
 * Revision 1.1  1998/02/11 04:13:54  terano
 * Initial revision
 *
*/
/*
 * Symboliser for Pumpkin money.
 */

/**
 * This method turns a number into a string.  This is callec by the
 * money handler when it wants to create a nice string for the money
 * you have.
 * @param value the cost of the item to turn into a string
 * @return the value as a string
 * @see unsymbolise_string()
 * @see /obj/handlers/money_handler
 */
string symbolise_value( int value ) {
   int dollars;
   int pence;
   string s;

   if (value < 0) {
      s = "-";
      value = - value;
   } else {
      s = "";
   }
   dollars = value / 100;
   pence = ( value % 100 );
   if ( !pence ) {
      return "P$"+ s + dollars;
   }
   if ( !dollars ) {
      return s + pence +"p";
   }
   if ( pence < 10 ) {
      return "P$"+ s + dollars +".0"+ pence;
   }
   return "P$" + s + dollars + "." + pence;
} /* symbolise_value() */

/**
 * This method turns a string into a number.  This is called by the money
 * handler when it wishes to change a string into value.  This should
 * handle reversing the string, exactly as handled above.
 * @param str the string to turn into a value
 * @return the value, 0 if unable to decode
 * @see symbolise_value()
 * @see /obj/handlers/money_handler
 */
int unsymbolise_string( string str ) {
  int dollars;
  int pence;

  if (strlen(str) < 2) {
    return 0;
  }
  if (str[0] == '$' ||
      lower_case(str[0..1]) == "p$") {
     if (str[0] == 'p') {
       str = str[1..];
     }
     if (sscanf(str, "$%d.%d", dollars, pence) == 2) {
        return dollars * 100 + pence;
     } else if (sscanf(str, "$%d", dollars) == 1) {
        return dollars * 100;
     }
  } else if (lower_case(str[<1..<1]) == "p") {
     if (sscanf(str, "%dp", pence) == 1) {
       return pence;
     }
  }
  return 0;
} /* unsymbolise_string() */
