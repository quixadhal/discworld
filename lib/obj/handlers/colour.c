/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: colour.c,v 1.1 1998/01/06 05:03:33 ceres Exp $
 * $Log: colour.c,v $
 * Revision 1.1  1998/01/06 05:03:33  ceres
 * Initial revision
 * 
*/
#include <colour.h>

string calc_colour( int red, int green, int blue, int whitener ) {
   int main_hue;
   string result;

   result = "";
   if ( whitener > -1 ) {
      switch ( red + green + blue ) {
         case 0 .. 4 :
            return "colourless";
         case 5 .. 29 : 
            result += "faint ";
            break;
         case 30 .. 154 :
            break;
         default :
            result += "intense ";
      }
   }
   return result;
} /* calc_colour() */

/*

string add_colour( string word, int fine, int crude, int xp_thresh,
                int sp_thresh ) {
  int temp;
  temp = member_array( word, colour_names );
  if ( temp != -1 ) return "colour already exists";
  colour_names += ({ word });
  colour_details += ([ word : ({ fine, crude, xp_thresh, sp_thresh }) ]);
  save_cmr_handler();
  return "the colour "+ word +", a shade of "+ ( ( fine == crude )?
         "pure ":MODIFIERS[fine] ) + COLOURS[crude] +", with thresholds of "+
         xp_thresh +" xp and "+ sp_thresh +" sp";
} add_colour()

string identify_colour( string word, object player ) {
  int temp, *args;
  temp = member_array( word, colour_names );
  if ( temp == -1 ) return "unknown colour";
  args = allocate( 4 );
  args = colour_details[word];
   if ( !player )
      return word;
  switch ( ( ( player->query_total_xp() ) > args[2] ) +
           ( ( player->query_max_social_points() ) > args[3] ) ) {
    case 0: return COLOURS[args[1]];
    case 1: return ( ( ( args[0] == args[1] )?"pure ":MODIFIERS[args[0]] ) +
                     COLOURS[args[1]] );
    default: return word;
  }
} identify_colour()

*/
