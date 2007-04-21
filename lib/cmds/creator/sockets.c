/*  -*- LPC -*-  */
/*
 * $Id: sockets.c,v 1.2 2000/02/18 14:25:09 turrican Exp $
 */

inherit "/cmds/base";

mixed cmd(string /* unused */) {
  this_player()->more_string(dump_socket_status());
  return 1;
}
