/*  -*- LPC -*-  */
/*
 * $Id: fds.c,v 1.2 2000/02/18 13:36:24 turrican Exp $
 */

inherit "/cmds/base";

mixed cmd(string) {
  this_player()->more_string(dump_file_descriptors());
  return 1;
}
