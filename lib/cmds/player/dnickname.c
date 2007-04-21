/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: dnickname.c,v 1.2 2002/02/23 02:15:12 presto Exp $
 * $Log: dnickname.c,v $
 * Revision 1.2  2002/02/23 02:15:12  presto
 * Added query_patterns()
 *
 * Revision 1.1  1998/01/06 05:29:43  ceres
 * Initial revision
 * 
*/
/* command, trial out by Turrican for a commands daemon. */

inherit "cmds/base";

mixed cmd(string str) {
  mapping map_nicknames;

  map_nicknames = this_player()->query_nicknames();
  if (!map_nicknames)
    map_nicknames = ([ ]);
   
  if (!map_nicknames[str]) {
    notify_fail("That nickname does not exist.\n");
    return 0;
  }
  map_nicknames = m_delete(map_nicknames, str);
  this_player()->set_nicknames(map_nicknames);
  write("Deleted nickname \"" + str + "\".\n");
  return 1;
}


mixed *query_patterns()  {
  return ({ "<string'nickname'>", (: cmd($4[0]) :) });
}
