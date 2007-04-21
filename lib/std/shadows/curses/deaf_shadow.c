/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: deaf_shadow.c,v 1.1 1998/01/06 04:35:35 ceres Exp $
 * $Log: deaf_shadow.c,v $
 * Revision 1.1  1998/01/06 04:35:35  ceres
 * Initial revision
 * 
*/
inherit "/std/effect_shadow";

#include "effect.h"

void event_whisper(object ob, string start, string mess, object *d, string lang) {
   int i;

   i = (int)arg() / 2;
   if (random(100) < i) {
      player->event_whisper(ob, start, mess, d, lang);
   } else {
      player->add_message(ob, "$N %hiss% something at you, but you just "+
"can't make it out.\n", 1);
   }
}

void event_person_say(object ob, string start, string mess, string lang) {
   string dum1, dum2;
   if (sscanf(start, "%sloud%s", dum1, dum2)) {
      if (random(100) > (100 - (int)arg()) / 2) {
         player->event_person_say(ob, start, mess, lang);
      } else {
         player->add_message(ob, "$N say$s something loudly, but "+
"you fail to hear it properly.\n", 1);
      }
   } else {
      if (random(100) < arg()) {
         player->event_person_say(ob, start, mess, lang);
      } else {
         player->add_message(ob, "$N say$s something, but you just can't "+
   "hear it properly.\n", 1);
      }
   }
}

void event_person_say_new(object ob, string start, string mess, string lang, 
  mixed avoid) {
   tell_object(player, sprintf("New Say: %O %O %O %O %O\n", ob, start, mess, lang, avoid));
}
