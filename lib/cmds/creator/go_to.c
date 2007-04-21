/*  -*- LPC -*-  */
/*
 * $Locker: pinkfish $
 * $Id: go_to.c,v 1.4 2002/02/08 18:42:42 pinkfish Exp pinkfish $
 * $Log: go_to.c,v $
 * Revision 1.4  2002/02/08 18:42:42  pinkfish
 * Allow them to handle virtual room stuff more usefully.
 *
 * Revision 1.3  1999/07/03 19:56:52  sin
 * Added support for terrains for gob_ack.c
 *
 * Revision 1.2  1999/07/02 20:41:25  ceres
 * Small changes
 *
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
/* goto command, trial out by Turrican for a commands daemon. */

mixed cmd(string str) {
  object dest;
  string *names, nick;
  string tname;

  if (!str)
    return notify_fail("Teleport where?\n");

  nick = (string)this_player()->expand_nickname(str);
  dest = find_living(nick);
  if (!dest && objectp(this_player()->get_obvar(str))) {
    dest = this_player()->get_obvar(str);
    if (dest == environment(this_player()))
      return notify_fail("You look around and realise you are already there.\n");
    this_player()->set_last_location(base_name(environment(this_player())));
    this_player()->move_with_look( dest,
                                   (string)this_player()->query_mmsgin(),
                                   (string)this_player()->query_mmsgout() );
    return 1;
  }
  if (dest) {
    if(dest = environment(dest)) {
      if (dest == environment(this_player()))
        return notify_fail("You look around and realise you are already "
                           "there.\n");

      this_player()->set_last_location(base_name(environment(this_player())));
      this_player()->move_with_look( dest,
            (string)this_player()->query_mmsgin(),
            (string)this_player()->query_mmsgout() );
      return 1;
    } else
      return notify_fail(capitalize(nick) + " is not standing in a location.\n");
  } else {
    names = (string *)this_player()->get_cfiles(str);
    if(sizeof(names)) {
      str = names[0];
    } else {
      // If is had a : try anyway.
      if (strsrch(str, ":") == -1) {
         return notify_fail("No such room.\n");
      }
      if (str[0] != '/') {
         str = this_player()->query_current_path() + "/" + str;
      }
    }
    dest = load_object(str);
    if (!dest) {
      return notify_fail("Failed to load " + str + "\n");
    } else if(!dest->query_property("location")) {
      return notify_fail("Not a room: " + str + "\n");
    } else {
      if (dest == environment(this_player())) {
        notify_fail("You look around and realise you are already there.\n");
        return 0;
      }

      if (environment(this_player())) {
         tname = environment(this_player())->query_property("terrain name");
         if (tname) {
           this_player()->set_last_location(
             ({ tname, environment(this_player())->query_co_ord() })
             );
         } else {
           this_player()->set_last_location(
             base_name(environment(this_player()))
             );
         }
      }
      this_player()->move_with_look( dest,
                                     (string)this_player()->query_mmsgin(),
                                     (string)this_player()->query_mmsgout() );
      return 1;
    }
  }
}

void dest_me() {
  destruct(this_object());
}

void clean_up() {
  dest_me();
}

void reset() {
  dest_me();
}
