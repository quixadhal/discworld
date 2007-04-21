/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: snoop.c,v 1.4 1998/08/21 11:26:08 pinkfish Exp $
 * $Log: snoop.c,v $
 * Revision 1.4  1998/08/21 11:26:08  pinkfish
 * Erm, realy change the snoop log location :)
 *
 * Revision 1.3  1998/08/21 11:23:41  pinkfish
 * Move the snoop log into the admin dir.
 *
 * Revision 1.2  1998/05/05 14:09:15  pinkfish
 * Fix up qsnoop.
 *
 * Revision 1.1  1998/01/06 05:12:03  ceres
 * Initial revision
 * 
*/
int valid_snoop(object snooper, object snoopee, object pobj) {
  string verb;

  verb = this_player()->query_current_verb();
  if (snooper == snoopee) {
    tell_object(snooper, "You can't snoop yourself.\n");
    return 0;
  }
  if (snoopee && query_snoop(snoopee)) {
    tell_object(snooper,
                snoopee->query_cap_name()+" is already being snooped.\n");
    return 0;
  }
  if (snooper->query_snoopee()) {
    user_event( snooper->query_cap_name()+" stops " + verb + "ing "+
                snooper->query_snoopee()->query_name(), "snoop");
    if (!snooper->query_property("quiet snoop")) {
      tell_object((object)snooper->query_snoopee(),
                  snooper->query_cap_name()+" stops snooping you.\n");
    } else {
      snooper->remove_property("quiet snoop");
    }
    snooper->set_snoopee(0);
  }
  if (!snoopee) {
    return 1;
  }
  if (!snooper->query_creator())
    return 0;
  if (pobj == this_object()) {
    user_event( snooper->query_cap_name()+" starts qsnooping "+
                snoopee->query_name(), "snoop");
    return 1;
  }
  if (verb == "qsnoop" && query_lord(geteuid(snooper)) &&
      !query_lord(geteuid(snoopee))) {
    tell_object(snooper, "You are quiet snooping "+
                snoopee->query_cap_name()+"\n");
    snooper->add_property("quiet snoop", 1);
  } else {
    tell_object(snoopee, "You are being snooped by "+
                snooper->query_cap_name()+".\n");
  }
  unguarded( (: write_file("/d/admin/log/SNOOP", ctime(time()) + " " +
           (string)$(snooper)->query_cap_name() + " " +
           $(verb) + "s " + (string)$(snoopee)->query_cap_name() + ".\n") :) );
  snooper->set_snoopee(snoopee);
  if (verb == "qsnoop") {
    tell_object(snooper, "Please share with us the reason why you are " +
                "quiet snooping?\n: ");
    input_to("snoop_reason");
    snoop_list[snooper] = snoopee;
    return 0;
  }
  user_event( snooper->query_cap_name()+" starts " + verb + "ing "+
              snoopee->query_name(), "snoop");
  return 1;
} /* valid_snoop() */

void snoop_reason(string str) {
  object snooper;

  snooper = this_player();
  if (this_player(1) != this_player()) {
    write("Can't force people...\n");
    return ;
  }
  if (!high_programmer(geteuid(this_player()))) {
    write("Not a high programmer.\n");
    return ;
  }
  if (!snoop_list[snooper]) {
    write("The snoopee has just logged out.\n");
    return ;
  }
  if(!str) {
    write("Snoop canceled.\n");
    unguarded( (: write_file("/d/admin/log/SNOOP", "  Chickened out.\n") :) );
    return;
  }
  unguarded( (: write_file("/d/admin/log/SNOOP", "  Reason: " + $(str) +
                           "\n") :) );
  if (snoop(snooper, snoop_list[snooper]))
    write("Snoop suceeded,\n");
  else
    write("Snoop failed.\n");
} /* snoop_reason() */
