/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: gossip.c,v 1.4 2002/08/03 15:52:37 danbala Exp $
 * $Log: gossip.c,v $
 * Revision 1.4  2002/08/03 15:52:37  danbala
 * typo fix
 *
 * Revision 1.3  2000/07/20 18:54:10  gruper
 * Removed duplicate call to event_person_say().
 *
 * Revision 1.2  2000/06/23 02:37:18  pinkfish
 * Allow it to work with non-existent gossip streams.
 *
 * Revision 1.1  1998/01/06 04:39:52  ceres
 * Initial revision
 * 
*/
/*
 * The gossip shadow.  Keeps track of everything that people say.  Then
 * repeats it back in a nice happy go frog format.
 */
inherit "/std/effect_shadow";

#include <gossip.h>

mixed *_says;

private void fixup_says() {
   int i;
   int num;

   if (!_says) {

      _says = ({
    ({ "Pinkfish", "Chickens look good under ultra violet light.", }),
    ({ "Deutha", "In the woods there is a singing lemon.  It has the "
                 "answers to all the quests." }),
    ({ "Turrican", "I am not a number, I am a free womble." }),
    ({ "Ceres", "I want to be a cabbage, I want to see the sea, I want "
                "to live in rabbit land, I want to be with thee.", }),
      });

      /* We only want a random number of these to start with */
      num = random(sizeof(_says));
      while (num > 0) {
         i = random(sizeof(_says));
         _says = _says[0..i-1] + _says[i+1..];
         num--;
      }
   }
} /* fixup_says() */

void event_person_say(object ob, string start, string mess, string lang) {
   int i;
   string *bits;
   string *filter;

   player->event_person_say(ob, start, mess, lang);

   /* Only want players gossip... */
   if (!userp(ob)) {
      return ;
   }

   fixup_says();

   filter = GOSSIP_HANDLER->query_filter();
   bits = explode(mess, " ");
   for (i = 0; i < sizeof(bits); i++) {
      if (member_array(bits[i], filter) != -1) {
         player->event_person_say(ob, start, mess, lang);
         return ;
      }
   }

   if (_says[<1][0] != ob->query_cap_name() ||
       _says[<1][1] != mess) {
      _says += ({ ({ ob->query_cap_name(), mess }) });
   }

   if (!random(50)) {
      GOSSIP_HANDLER->add_gossip(ob->query_cap_name(), mess);
   }

} /* event_person_say() */

void do_gossip_say(string str) {
   int i;
   string *stuff;
   int chance;

   if (!str) {
      str = "$name$ said: $mess$";
   }

   chance = arg();

   if (!intp(chance)) {
      chance = 5;
   }

   fixup_says();

   /* Will handle random gossip off the gossip pool */
   if (sizeof(_says) > 0 && random(chance)) {
      i = random(sizeof(_says));
      stuff = _says[i];
   } else {
      stuff = GOSSIP_HANDLER->query_random_gossip();
   }
   if (!stuff) {
      i = random(sizeof(_says));
      stuff = _says[i];
   }
   if (!stuff) {
      str = replace(str, ({ "$name$", "fluff",
                            "$mess$", "big fluff" }));
   } else {
      str = replace(str, ({ "$name$", stuff[0],
                            "$mess$", stuff[1] }));
   }
   player->do_command("say "+str);
} /* do_gossip_say() */

mixed *query_gossip_says() {
   return _says;
} /* query_gossip_says() */
