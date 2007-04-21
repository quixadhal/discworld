/**
 * This effect is for the nice people who own a transport.
 * @author Pinkfish
 * @started Fri Sep 24 1:00:00 1999
 * @classification mudlib.riding.owned
 */
#include <player.h>
inherit "/std/basic/virtual_quit_control";

class riding {
   object* transport;
   string* environments;
   mixed save_stuff;
}

private nosave int _transport_id;

void create() {
   _transport_id = random(3000) + 1;
} /* create() */

/** @ignore yes */
string query_classification() {
   return "mudlib.riding.owned";
} /* query_classification() */

/** @ignore yes */
int query_indefinite() {
   return 1;
} /* query_indefinite() */

/** @ignore yes */
class riding beginning(object player, object transport, int id) {
   class riding bing;

   bing = new(class riding);
   bing->transport = ({ transport });
   bing->save_stuff = 0;
   transport->set_transport_id(_transport_id++);
   return bing;
} /* begining() */

/** @ignore yes */
class riding merge_effect(object player, class riding fluff, object transport) {
   if ( member_array(transport, fluff->transport) == -1) {
      if (!transport->query_transport_id()) {
         while(sizeof(filter(fluff->transport,
                             (: $1 && $1->query_transport_id() == $2 :),
                             _transport_id))) {
            _transport_id++;
         }
         transport->set_transport_id(_transport_id++);
      } else if (sizeof(filter(fluff->transport,
                        (: $1 && $1->query_transport_id() == $2 :),
                        transport->query_transport_id()))) {
         transport->dest_me();
         return fluff;
      }
      fluff->transport += ({ transport });
   }
   return fluff;
} /* merge_effect() */

/** @ignore yes */
void end(object player, class riding arg) {
   //tell_creator("pinkfish", "Ending owned effect...\n");
} /* end() */

/** @ignore yes */
void restart(object player, class riding bing) {
   int i;
   object* blue;
   object* green;

   //tell_creator("pinkfish", "Restart! %O\n", bing->transport);
   blue = filter(bing->transport, (: $1 :));
   if (!sizeof(blue) &&
       bing->save_stuff) {
      green = AUTO_LOAD_OB->load_auto_load_to_array(bing->save_stuff,
                                         player);
      //tell_creator("pinkfish", "Cuddles %O\n", green);
      for (i = 0; i < sizeof(bing->transport); i++) {
         if (green) {
            bing->transport[i] = green[i];
            green->move(bing->environments[i],
                        "$N appears with a puff of fluff.");
         }
      }
      //tell_creator("pinkfish", "Restart! %O\n", bing->transport);
   }
} /* restart() */

/** @ignore yes */
void clear_thingy(class riding bing) {
   bing->save_stuff = 0;
} /* clear_thingy() */

/** @ignore yes */
void saving(object player, class riding bing) {
   string *env;

   bing->transport -= ({ 0 });
   env = map(bing->transport,
             (: find_start_pos($1, environment($1)) :));
   bing->environments = map(bing->transport,
                            (: find_start_pos($1, environment($1)) :));
   bing->save_stuff = AUTO_LOAD_OB->create_auto_load( bing->transport );
   call_out( (: clear_thingy :), 2, bing);
   //tell_creator("pinkfish", "Saving %O %O\n", bing->transport, bing->environments);
} /* saving() */

/** @ignore yes */
void quiting(object player, class riding bing) {
   //tell_creator("pinkfish", "Quiting %O\n", bing->transport);
   call_out((: $1->move("/room/rubbish", 0,
                        "$N vanishes in a puff of fluff.") :),
            2, bing->transport);
} /* quiting() */

/**
 * This method removes the specified transport from the list of
 * current transports on the player.
 * @param player the player the transport is removed from
 * @param bing the class!
 * @param id the id of the effect
 * @param trans the transport to remove
 */
void remove_owner(object player, class riding bing, int id, object trans) {
//tell_creator("pinkfish", "Remove owner of %O (%O)\n", trans, player);
   bing->transport -= ({ trans });
   if (!sizeof(bing->transport)) {
      //tell_creator("pinkfish", "Deleting!\n");
      player->delete_effect(id);
   }
} /* remove_owner() */
