/**
 * This is to handle the mxp control elements of the mud.
 * @author Pinkfish
 * @started Fri Oct 24 13:12:03 PDT 2003
 */

/**
 * This puts in the mxp tags if the player has mxp turned on.
 * @param tag the tag to wrap the text in
 * @param output the original string
 * @param player the player to verify the mxp status on
 */
varargs string mxp_tag(string tag, string output, mixed player) {
#if efun_defined(has_mxp)
   if (!player) {
      player = this_player();
   }
   if (player == 1 || player->is_mxp_enabled()) {
   //if (player == 1 || (has_mxp(player) && player->query_creator())) {
      return "MXP<" + tag + "MXP>" + output + "MXP</" + tag + "MXP>";
   }
#endif
   return output;
}

/**
 * This puts in the mxp tags if the player has mxp turned on, it also
 * inserts arguments as needed.
 * @param tag the tag to wrap the text in
 * @param args the args to add
 * @param output the original string
 * @param player the player to verify the mxp status on
 */
varargs string mxp_tag_args(string tag, string args, string output, mixed player) {
#if efun_defined(has_mxp)
   if (!player) {
      player = this_player();
   }
   if (player == 1 || player->is_mxp_enabled()) {
   //if (player == 1 || (has_mxp(player) && player->query_creator())) {
      return "MXP<" + tag + " " + args + "MXP>" + output + "MXP</" + tag + "MXP>";
   }
#endif
   return output;
}

/**
 * THis makes a choice between the mxp string and the non-mxp
 * string.
 */
varargs string mxp_choice(string non_mxp, string mxp, mixed player) {
#if efun_defined(has_mxp)
   if (!player) {
      player = this_player();
   }
   if (player == 1 || player->is_mxp_enabled()) {
   //if (player == 1 || player->query_name() == "miranaka") {
      return mxp;
   }
#endif
   return non_mxp;
}

/**
 * This turns on the secure mxp mode.
 */
varargs string mxp_secure(object player) {
#if efun_defined(has_mxp)
   if (!player) {
      player = this_player();
   }
   if (player->is_mxp_enabled()) {
      return sprintf("%c[6z", 27);
   }
#endif
   return "";
}

/**
 * This turns on the open mxp mode.
 */
varargs string mxp_open(object player) {
#if efun_defined(has_mxp)
   if (!player) {
      player = this_player();
   }
   if (player->is_mxp_enabled()) {
      return sprintf("%c[5z", 27);
   }
#endif
   return "";
}

/**
 * This makes the next thingy sent to be secure.
 * @param player the playet to check for mxp status
 */
varargs string mxp_next_secure(object player) {
#if efun_defined(has_mxp)
   if (!player) {
      player = this_player();
   }
   if (player->is_mxp_enabled()) {
      return sprintf("%c[4z", 27);
   }
#endif
   return "";
}

/**
 * This makes the next thingy sent to be secure.
 */
varargs string mxp_expire(string category, object player) {
#if efun_defined(has_mxp)
   if (!player) {
      player = this_player();
   }
   if (player->is_mxp_enabled()) {
      return "MXP<EXPIRE " + category + "MXP>";
   }
#endif
   return "";
}
