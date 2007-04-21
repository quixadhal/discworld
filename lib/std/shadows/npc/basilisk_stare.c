/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: basilisk_stare.c,v 1.2 2003/04/02 16:16:42 carmine Exp $
 * $Log: basilisk_stare.c,v $
 * Revision 1.2  2003/04/02 16:16:42  carmine
 * Fixed bug with "helpless" message
 *
 * Revision 1.1  2002/01/27 09:02:51  siel
 * Initial revision
 *
 * Revision 1.1  1998/01/06 04:37:11  ceres
 * Initial revision
 * 
*/
#include <move_failures.h>

inherit "/std/effect_shadow";

int query_basil_stare_shad(){ return 1; }

string query_passed_out_message()
{
  string *msgs = ({
    "The eyes!  They're burning!  Can't think!\n",
    "Your concentration is too fragmented.\n",
    "Why do that when you can just sit here and fall into those eyes?\n",
    "Arrrrgghhhh!  Too much pain!\n"
    }) ;
  return msgs[random(sizeof(msgs))];
}

int move_with_look(object dest, string messin, string messout)
{
   tell_object(player, "Your mind is not here to help you control "+
                       "your movement.\n");
   return MOVE_NO_DROP;
}


int attack_by(object obj)
{
   if( !obj || ( !obj->one_short() ) ) {
      tell_room( environment( player ), player->one_short() + " is totally "
         "helpless.\n", player );
   }
   else {
      tell_room(environment(player), player->one_short()+
        " is totally helpless against "+obj->one_short()+"'s attack.\n",
        player);
   }
   
   tell_object(player, "Oh dear! How do I defend myself? I forgot!\n") ;
   return 1;
}

/* This next function prevents people from dodging or parrying!
 * Ain't I a stinker? */
int query_special_manoeuvre() { return 0; }
