/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: multiple_short.c,v 1.14 2001/03/12 06:04:25 pinkfish Exp $
 * $Log: multiple_short.c,v $
 * Revision 1.14  2001/03/12 06:04:25  pinkfish
 * Fix up some problems in the code...
 *
 * Revision 1.13  2001/03/12 06:03:23  pinkfish
 * Fix up some issues with multiple short.
 *
 * Revision 1.12  2001/03/09 02:21:56  pinkfish
 * Add in some docs.
 *
 * Revision 1.11  2001/03/08 12:24:24  taffyd
 * Fixed up the fixes for the fixes for the fixes.
 *
 * Revision 1.10  2001/03/08 08:56:01  pinkfish
 * Fix up issues with multiple short.
 *
 * Revision 1.9  2001/03/08 01:08:31  pinkfish
 * Fix up some interesting things with query_multiple_short.
 *
 * Revision 1.8  2001/03/07 23:50:39  pinkfish
 * Make it handle dark as an extra paramebert.
 * Add ina dark check.
 *
 * Revision 1.7  2000/07/09 08:29:56  shrike
 * Made it aware of the quiet flags to the short functions.
 *
 * Revision 1.6  2000/04/25 22:15:30  pinkfish
 * Make the no_dollars arg work as it shold.
 *
 * Revision 1.5  2000/04/25 22:06:41  pinkfish
 * Make it really do no_dollars translations.
 *
 * Revision 1.4  1998/07/24 05:02:43  pinkfish
 * Some changes and some not so pleasant changes...
 *
 * Revision 1.3  1998/06/30 01:07:32  pinkfish
 * Change it so that the functions get calle don the object itselt to handle
 * things with variable short descriptions.
 *
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
/**
 * Returns a nice string which looks like "Frog, fish and chicken".  This
 * takes in an array of objects and or strings and returns a message
 * like that shown above.  It also does things like adding the number of
 * each type at the start of the function.
 * <p>
 * This function has two main uses: to produce the control codes for a
 * set of objects and to produce a nice list from a set of strings.
 * The "type" argument is optional and may be "a", "the", "one" or
 * "poss", defaulting to "a": it determines which of a_short,
 * the_short, one_short or poss_short will be used to make the control
 * code sequence for the objects in "args".  If all of the elements of
 * "args" are objects, it simply returns the block control code for
 * the relevant shorts of the objects.  Otherwise, it looks through
 * the elements one at a time, replacing objects by the result of
 * calling the relevant short function on them; finally it
 * concatenates the elements with commas between each pair, except
 * between the last pair which have " and " between them.  Since the
 * output will contain processing codes if there are any objects in
 * "args", it should be passed through convert_message before being
 * stored.
 * <p>
 * If you pass in a 1 for the 'quiet' flag, it will not take any of the
 * objects out of hiding.
 *
 * @param args the array to do the processing on
 * @param type the type of short to use "a", "the", "one", "poss"
 * @param no_dollars this forces the output to be a real string
 * @param quiet do not bring the objects out of hiding
 * @param dark how dark it is, used to hide object lists in the dark
 * @return the processed string
 */
varargs string query_multiple_short( mixed *args, string type,
                                     int no_dollars, int quiet,
                                     int dark ) {
   int i;
   //int found_object;
   string func;
   string ret;

   // If it is dark, we want it to not see the objects.
   if (no_dollars && sizeof(args) && objectp(args[0]) && undefinedp(dark) && 
       this_player() && environment(this_player())) {
      dark = this_player()->check_dark(environment(this_player())->query_light());
      if (dark) {
         return "some objects you cannot make out";
      }
   }


   args = copy(args);
   if ( !type ) {
      type = "a";
   }
   for ( i = 0; i < sizeof( args ); i++ ) {
      if ( !objectp( args[ i ] ) ) {
         break;
      }
   }
   /*
    * Changed to use a function instead of the fast direct method to handle
    * the problem where things with variable shorts are used in multiple
    * lists.
    */
   func = type + "_short";
   if ( i == sizeof( args ) && !no_dollars) {
      for ( i = 0; i < sizeof( args ); i++ ) {
         args[ i ] = call_other(args[ i ], func, quiet);
         //args[ i ] = "$"+ type +"_short:"+ file_name( args[ i ] ) +"$";
      }
      return "$M$"+ implode( args, "" ) +"$M$";
   }
   for ( i = 0; i < sizeof( args ); i++ ) {
      if ( objectp( args[ i ] ) ) {
         args[ i ] = call_other(args[ i ], func, quiet);
         //args[ i ] = "$"+ type +"_short:"+ file_name( args[ i ] ) +"$";
      } else if ( pointerp( args[ i ] ) ) {
         /* This may still be used somewhere. */
         args[ i ] = "$"+ type +"_short:"+ file_name( args[ i ][ 1 ] ) +"$";
      }/* else if (stringp(args[ i ]) && !no_dollars) {
         args[ i ] = "$mirror_short:" + args[i] + "$";
      } */
   }
   //if (!no_dollars) {
      //return "$M$"+ implode( args, "" ) +"$M$";
   //}
   i = sizeof( args );
   switch ( i ) {
      case 0 :
         return "";
      case 1 :
         ret = args[ 0 ];
         break;
      default :
         ret = implode( args[ 0 .. i - 2 ], ", " ) +" and "+ args[ i - 1 ];
         break;
   }
   if (no_dollars) {
      if (this_player()) {
         return this_player()->convert_message(ret);
      } else {
         return "/global/player"->convert_message(ret);
      }
   }
   return ret;
} /* query_multiple_short() */
