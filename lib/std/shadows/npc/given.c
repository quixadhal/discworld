/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: given.c,v 1.5 2002/04/18 20:42:00 wodan Exp $
 * $Log: given.c,v $
 * Revision 1.5  2002/04/18 20:42:00  wodan
 * fixed the functionpointer/call_other check, i think, probably (just removed two lines)
 *
 * Revision 1.4  2002/04/15 01:26:18  wobin
 * Fixed a runtime due to driver changes.Sun Apr 14 14:22:50 2002:  *Trying to put function in array Object: /std/shadows/npc/given#105785 at line 21
 *
 * Revision 1.3  1998/10/09 10:16:16  terano
 * Changes for ranma
 *
 * Revision 1.1  1998/01/06 04:39:52  ceres
 * Initial revision
 * 
*/
inherit "/std/effect_shadow";

void event_enter(object ob, string mess, object from) {
   mixed *bit = ({ });
   int ret;

   /* Check to see if it is us the object is entering... */
   if (environment(ob) == player) {
      if(sizeof(arg()) > 1)
        bit = arg();
          else
        bit = ({ arg() });
//        if (functionp(bit[0])) {
           if (sizeof(bit) < 2) 
              ret = evaluate(bit[0], player, ob, mess, from);
           else 
              ret = call_other(bit[0], bit[1], player, ob, mess, from);	 
//      }
   }
   player->event_enter(ob, mess, from);
} /* event_enter() */
