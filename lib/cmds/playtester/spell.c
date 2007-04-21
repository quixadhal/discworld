inherit "/cmds/base";
#include <network.h>

void finish_lookup(object player, string word, int result, mixed* results) {
   if (result == NETWORK_SUCCESS) {
      if (!sizeof(results)) {
         tell_object(player, "Matches for " + word + ": None.\n");
      } else {
         tell_object(player, "Matches for " + word + ": " +
                             query_multiple_short(map(results, (: $1[1] :))) +
                             "\n");
      }
   } else {
      tell_object(player, "Error matching " + word + ": " + result);
   }
} /* finish_lookup() */

int cmd(string word) {
   NETWORK_DICTIONARY_HANDLER->spell_word(word, 0,
                        (: finish_lookup($(this_player()), $1, $2, $3) :));
   write ("You look up a word.\n");
   return 1;
} /* cmd() */

mixed* query_patterns() {
   return ({ "<word>", (: cmd($4[0]) :) });
} /* query_patterns() */
