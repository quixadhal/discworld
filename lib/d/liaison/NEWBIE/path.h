/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: path.h,v 1.2 2000/06/23 19:25:50 pinkfish Exp $
 *
 * $Log: path.h,v $
 * Revision 1.2  2000/06/23 19:25:50  pinkfish
 * Remove bits for the distribution lib.
 *
 * Revision 1.1  2000/03/01 06:06:59  ceres
 * Initial revision
 *
 * Revision 1.2  1998/09/09 12:35:27  aragorn
 * Changed Fighters to Warriors
 *
 * Revision 1.1  1998/01/16 02:19:20  arby
 * Initial revision
 *
*/
#define PATH "/d/liaison/NEWBIE/"
#ifdef __DISTRIBUTION_LIB__
#define GUILDS "None currently"
#else
#define GUILDS "Witches, Wizards, Warriors, Priests, Thieves, and Assassins"
#endif

#define SIGN_WIDTH 67

#define FOYER (PATH+"foyer")
