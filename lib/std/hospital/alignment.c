/**
 * This code is used to determine the alignment of NPCs in the hospital.
 */ 

/*
 * These defines set the shape of the distribution of alignments handed out
 * by pick_al.  Using the standard normal distribution, split into ten steps
 * on each side of the average, we can safely disregard the probability of
 * values outside of the range of -3.0 to 3.0 (99.7% of values will fall
 * within this range of three * standard deviation from the average).
 *        Range           Prob.
 *  P( 0.0 < z < 0.3 ) = 0.1179
 *  P( 0.3 < z < 0.6 ) = 0.1078
 *  P( 0.6 < z < 0.9 ) = 0.0902
 *  P( 0.9 < z < 1.2 ) = 0.0690
 *  P( 1.2 < z < 1.5 ) = 0.0483
 *  P( 1.5 < z < 1.8 ) = 0.0309
 *  P( 1.8 < z < 2.1 ) = 0.0180
 *  P( 2.1 < z < 2.4 ) = 0.0097
 *  P( 2.4 < z < 2.7 ) = 0.0047
 *  P( 2.7 < z ) =       0.0035
 *
 *  These probabilites are then converted into cumulative limits out of
 *  a total chance, as integers:
*/
#define TOTAL_CHANCE 150
#define CENTRE 0
#define LIMIT_ONE     35
#define LIMIT_TWO     68
#define LIMIT_THREE   95
#define LIMIT_FOUR   115
#define LIMIT_FIVE   130
#define LIMIT_SIX    139
#define LIMIT_SEVEN  145
#define LIMIT_EIGHT  147
#define LIMIT_NINE   149

/**
 * This function produces a random alignment for an npc, based on a
 * normal distribution around a supplied average, with a maximum and
 * minimum limit provided by a requested range.
 * <p>
 * The alignment produced will be within the range[ average - range,
 * average + range ]
 * @param average The average alignment for this npc
 * @param range The range request for alignment - the difference between
 * average and maximum possible alignment
 * <p>
 */
int pick_al( int average, int range ) {
   int choice, section, section_width, result;

   // Must be a positive range.
   if( range < 0 ) {
      range = -range;
   }

   // Must have a range of 10 at least.
   if( range < 10 ) {
      range = 10;
   }

   section_width = floor( range / 10 );

   choice = random( TOTAL_CHANCE );

   switch( choice ) {
      case CENTRE..LIMIT_ONE:
         section = 0;
         break;
      case ( LIMIT_ONE + 1 )..LIMIT_TWO:
         section = 1;
         break;
      case ( LIMIT_TWO + 1 )..LIMIT_THREE:
         section = 2;
         break;
      case ( LIMIT_THREE + 1 )..LIMIT_FOUR:
         section = 3;
         break;
      case ( LIMIT_FOUR + 1 )..LIMIT_FIVE:
         section = 4;
         break;
      case ( LIMIT_FIVE + 1 )..LIMIT_SIX:
         section = 5;
         break;
      case ( LIMIT_SIX + 1 )..LIMIT_SEVEN:
         section = 6;
         break;
      case ( LIMIT_SEVEN + 1 )..LIMIT_EIGHT:
         section = 7;
         break;
      case ( LIMIT_EIGHT + 1 )..LIMIT_NINE:
         section = 8;
         break;
      default:
         section = 9;
         break;
   }

   result = section * section_width;
   result = result + random( section_width );

   switch( random( 2 ) ) {
      case( 0 ):
         result = average + result;
         break;
      default:
         result = average - result;
         break;
   }

   result = result + average;
   return result;
} /* pick_al */
