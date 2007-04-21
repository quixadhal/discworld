inherit "/cmds/base";

/**
 * This method allows a single alias to be removed.
 * Only doable by self or a lord
 * @param str the name of the alias to remove
 * @return 1 on success, 0 on failure
 */
varargs int unalias(string str, object who ) {
    if (!who) who = this_player();
   if (!who->is_alias(str)) {
      add_failed_mess("The alias '" + str + 
                      "' does not exist, cannot unalias.\n");
      return 0;
   }
   if (who->remove_player_alias(str)) {
      write("Successfully unaliased '" + str + "'.\n");
      return 1;
   } else {
      add_failed_mess("Unable to unalias '" + str + "'.\n");
      return 0;
   }
} /* unalias() */

/**
 * This method allows a single alias to be removed.
 * Only doable by self or a lord
 * @param str the name of the alias to remove
 * @return 1 on success, 0 on failure
 */
varargs int unalias_pattern(string filter) {
   mapping aliases;
   string* tmp;
   string* ok;
   string gfilter;
   string bing;

   aliases = this_player()->query_aliases();
   // DO the pattern thing.
   tmp = m_indices(aliases);
   if (regexp(filter,
        "[\\[\\]\\(\\)\\*\\?\\+][\\[\\]\\(\\)\\*\\?\\+]+") ) {
      add_failed_mess("Bad pattern to alias.\n");
      return 0;
   }
   if (filter[0] == '*' || filter[0] == '+') {
      add_failed_mess("Cannot start a regular expression with a '*' or '+', "                         "try: '.*' or '.+'.\n");
      return 0;
   }
   gfilter = "^" + filter;
   tmp = filter_array(tmp, (: $1 && regexp($1, $(gfilter)) :));

   ok = ({ });
   foreach (bing in tmp) {
      if (this_player()->remove_player_alias(bing)) {
         ok += ({ bing });
      }
   }

   if (sizeof(ok)) {
      add_succeeded_mess("Successfuly removed the aliases " +
            query_multiple_short(map(ok, (: "'" + $1 + "'" :))) + ".\n");
      return 1;
   }
   add_failed_mess("Unable to remove any of the aliases " +
            query_multiple_short(map(tmp, (: "'" + $1 + "'" :))) + ".\n");
   return 0;
} /* unalias() */

mixed *query_patterns() {
   return ({
      "pattern <word'alias'>", (: unalias_pattern($4[0]) :),
      "<word'alias'>", (: unalias($4[0]) :),
         });
} /* query_patterns() */
