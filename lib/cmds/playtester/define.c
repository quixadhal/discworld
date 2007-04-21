inherit "/cmds/base";
#include <network.h>

void finish_lookup(object player, string word, int result, mixed* results) {
   string ret;
   string* bing;

   if (result == NETWORK_SUCCESS) {
      if (!sizeof(results)) {
         tell_object (player, "No definitions for " + word + ".\n");
      } else {
         ret = "";
         foreach (bing in results) {
            ret += bing[1] + "\n" + bing[2] + "\n\n";
         }
         tell_object(player, "Definitions for " + word + ":\n" + ret);
      }
   } else {
      tell_object(player, "Error matching " + word + ": " + result);
   }
} /* finish_lookup() */

int cmd(string word) {
   NETWORK_DICTIONARY_HANDLER->define_word(word, 0,
                        (: finish_lookup($(this_player()), $1, $2, $3) :));
   write ("You look up a word.\n");
   return 1;
} /* cmd() */

mixed* query_patterns() {
   return ({ "<word>", (: cmd($4[0]) :) });
} /* query_patterns() */
