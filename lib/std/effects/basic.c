/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: basic.c,v 1.1 1998/01/06 04:06:18 ceres Exp $
 * $Log: basic.c,v $
 * Revision 1.1  1998/01/06 04:06:18  ceres
 * Initial revision
 * 
*/
#include "potion_attrs.h"

int x1, x2, y1, y2;

void set_boundaries(int *min, int *max)
{
   x1 = min[0];
   x2 = max[0];
   y1 = min[1];
   y2 = max[1];
}

int *query_coordinate()
/*
 * this is for setting up potions ... say a wizard wants to put a healing
 * potion on her monster, she would put the line
 * set_ps_coord_quantity("/std/effects/cure_wounds"->query_coordinate(), 1000)
 * on it ... this one is the dumb standard one which just averages them
 * to get the midpoint
 */
{
   return ({ (x1 + x2) / 2, (y1 + y2) / 2 });
}

#pragma no_warnings
mixed *query_attrs(int *coord)
/*
 * return attrs@coord.  attrs is an array with elements as defined in
 *  include/potion_attrs.h ... definitely to be masked
 */
{
   return ({ 0, 100, ({ }), ({ }), ({ }), ({ }) });
}

int query_pass_through(int *coord)
/*
 * how much of any 'masked' effects will be passed through?
 * returns percentage 
 */
{
   return 100;
}

/* 
backdrop(int *coord)
 * if this returns true if this effect is just a 'backdrop' 
 * if it _is_ then unless it is the first effect to be found, it 
 * will _not_ be called at all.  This is so that an area can have
 * a poison defined over it as a backdrop, then wherever useful potions
 * are placed on it the backdrop potion is not used, tho any given
 * healing/whatever potion will probably pass all or most through
 */

/*
void action_drunk(object ob, int *coord, int quantity)
 * this is called when a player drinks a potion @ relevant coordinate
 */

/*
void action_touch(object ob, int *coord, int quantity)
 * this is called when an object is splashed with a potion of this effect
 */

/*
void action_smell(object ob, int *coord, int quantity)
 * this is called when a player smells the potion.  The quantity is
 * a measure of the concentration of the potion ... basically
 * the potions ps_quantity * 100 / volume
 */

/*
void action_create(object ob, int *coord, int quantity)
 * this is called when a potion is assigned the coordinate appropriate.
 * This is intended for those effects that are spontaneous upon creation ...
 * eg.  Exploding spontaneously
 *      Turning into a sludge monster
 *      Such concentrated acid that it dissolves the container its in
 *      etc
 * 'ob' is the container with the liquid in it.
 */

void install_effect()
/*
 * install potion in potion space handler dbase
 */
{
   (void)POTION_SPACE_HANDLER->remove_effect(file_name(this_object()));
   (void)POTION_SPACE_HANDLER->add_effect(file_name(this_object()), x1, x2, y1, y2);
}

int sqrt(int p)
{
   int i;
   
   for (i = 1; i < 500; i++) if (i * i > p) return i-1;
}
 
int distance_within_region(int *coord)
{
   int dx, dy;
 
   if (x1 < 0) {
      if (x2 > 0)
         dx = (coord[0] < 0) ? -coord[0] : coord[0];
      else
         dx = (x2 - coord[0]);
   } else 
      dx = coord[0] - x1;
 
   if (y1 < 0) {
      if (y2 > 0)
         dy = (coord[1] < 0) ? -coord[1] : coord[1];
      else
         dy = (y2 - coord[1]);
   } else 
      dy = coord[1] - y1;
   
   return sqrt(dx * dx + dy * dy);
}
 
int distance_from_origin(int *coord)
{
   return sqrt(coord[0] * coord[0] + coord[1] * coord[1]);
}
