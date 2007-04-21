/**
 * Cotains some simul_efuns.
 * @author Pinkfish
 */
private string ob_short(object ob) {
   string ret;

   if (ob->query_clothing()) {
      ret = ob->query_pocket_mess();
   } else {
      ret = "$ob_short$";
   }
   return ret;
} /* ob_short() */

/**
 * This method produces the inside messages for the given object up
 * to the player object.  This one generates 'the_short' messages.
 * This should only be called with an object that is a container.
 * It will produce weird and incorrect messages otherwise.
 * @param ob the object to generate the short for
 * @param play the player to generate the short up to
 * @see inside_a_short()
 */
string inside_the_short(object ob, object play) {
   string str;

   if (!play) {
      play = this_player();
   }
   if (!ob) {
      ob = environment(play);
   }
   str = replace_string(ob_short(ob), "$ob_short$", ob->the_short());
   while (environment(ob) &&
          environment(ob) != play &&
          environment(ob) != environment(play)) {
      ob = environment(ob);
      str += " inside " + replace_string(ob_short(ob), "$ob_short$",
                                         ob->the_short());
   }
   return str;
} /* inside_the_short() */

/**
 * This method produces the inside messages for the given object up
 * to the player object.  This one generates 'a_short' messages.
 * This should only be called with an object that is a container.
 * It will produce weird and incorrect messages otherwise.
 * @param ob the object to generate the short for
 * @param play the player to generate the short up to
 * @see inside_the_short()
 */
string inside_a_short(object ob, object play) {
   string str;

   if (!play) {
      play = this_player();
   }
   if (!ob) {
      ob = environment(play);
   }
   str = replace_string(ob_short(ob), "$ob_short$", ob->a_short());
   while (environment(ob) &&
          environment(ob) != play &&
          environment(ob) != environment(play)) {
      ob = environment(ob);
      str += " inside " + replace_string(ob_short(ob), "$ob_short$",
                                         ob->the_short());
   }
   return str;
} /* inside_a_short() */

/**
 * This method produces the inside messages for the given object up
 * to the player object.  This one generates 'a_short' messages.
 * This should only be called with an object that is a container.
 * It will produce weird and incorrect messages otherwise.
 * @param ob the object to generate the short for
 * @param play the player to generate the short up to
 * @see inside_the_short()
 */
string inside_one_short(object ob, object play) {
   string str;

   if (!play) {
      play = this_player();
   }
   if (!ob) {
      ob = environment(play);
   }
   str = replace_string(ob_short(ob), "$ob_short$", ob->one_short());
   while (environment(ob) &&
          environment(ob) != play &&
          environment(ob) != environment(play)) {
      ob = environment(ob);
      str += " inside " + replace_string(ob_short(ob), "$ob_short$",
                                         ob->the_short());
   }
   return str;
} /* inside_one_short() */
