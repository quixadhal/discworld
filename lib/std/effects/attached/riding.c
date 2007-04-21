/**
 * This effect is for the nice people who ride a horse.
 * @author Pinkfish
 * @started Fri Sep 24 1:00:00 1999
 * @classification mudlib.riding
 */

class riding {
   object transport;
   mixed saved_info;
}

/** @ignore yes */
string extra_look(object player) {
   return player->query_pronoun() + " is riding on " +
          environment(player)->the_short();
} /* extra_look() */

/** @ignore yes */
string query_classification() {
   return "mudlib.riding.transport";
} /* query_classification() */

/** @ignore yes */
int query_indefinate() {
   return 1;
} /* query_indefinate() */

/** @ignore yes */
class riding beginning(object player, mixed arg, int id) {
   class riding bing;

   bing = new(class riding);
   return bing;
} /* begining() */

/** @ignore yes */
class riding end(object player, class riding arg) {
} /* end() */

/** @ignore yes */
void restart(object player, class riding bing) {
   //
   // In here we should figure out if the horse has shown up and
   // then ride them...
   //
} /* restart() */
