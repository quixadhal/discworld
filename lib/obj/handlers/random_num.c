/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: random_num.c,v 1.2 1999/02/20 05:47:17 jeremy Exp $
 * $Log: random_num.c,v $
 * Revision 1.2  1999/02/20 05:47:17  jeremy
 * Added an 'lseed' parameter to random() to allow passing in
 * a seed.
 *
 * Revision 1.1  1998/01/06 05:03:33  ceres
 * Initial revision
 * 
*/
/**
 * Generate reproducible sequences of random numbers.
 * This is designed to produce a random number from the same
 * seed.  This will make sequences of reproduceable random
 * numbers.
 *
 * Useful for things like garbling of text and stuff so the garble
 * always looks the same...
 */

int seed = 100;

varargs int random(int max, mixed lseed);

#define Q           51924
#define R           10855
#define MULT        41358
#define MOD   21474836647

#define MAX_VALUE (MOD-1)

/* Throw away this many random numbers on startup */
#define STARTUP_RANDS  16

/**
 * Set the seed for the generator.
 * @param new_seed the seed to use.
 */
void set_seed(int new_seed) {
  int i;

  if (seed <= 0)
    seed = efun::random(200);
  else
    seed = new_seed;

/* Throw away some initial values */
  for (i=0;i<STARTUP_RANDS;i++)
    random(200);
} /* set_seed() */

/**
 * Generate a random number.  If lseed is an int, it is used as the seed.
 * If lseed is a one-element array of ints, lseed[0] is used as the seed
 * and the new seed is passed back.  Otherwise, the previously set seed
 * is used.
 * @param max the maximum value for the number.
 * @param lseed the seed to use, either an int or a one-element array
 * of int.
 * @return a number from 0..max-1 (inclusive)
 */
varargs int random(int max, mixed lseed) {
  int k, residue, curseed, mode;

  if (undefinedp(lseed)) {
    curseed = seed;
  } else if (intp(lseed)) {
    curseed = lseed;
    mode = 1;
  } else if (arrayp(lseed) && (sizeof(lseed) == 1) && (intp(lseed[0]))) {
    curseed = lseed[0];
    mode = 2;
  } else {
    curseed = seed;
  }
  k = curseed / Q;
  residue = MULT * (curseed - Q*R) - R*k;
  if (residue < 0)
    residue += MOD;
  if (mode == 0) {
    seed = residue;
  } else if (mode == 2) {
    lseed[0] = residue;
  }
  return residue % max;
} /* random() */

/**
 * Returns the currently specified seed.
 * @return the current seed
 */
int query_seed() {
  return seed;
} /* query_seed() */
