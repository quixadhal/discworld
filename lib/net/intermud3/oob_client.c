/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: oob_client.c,v 1.2 2002/08/18 15:21:50 terano Exp $
 * $Log: oob_client.c,v $
 * Revision 1.2  2002/08/18 15:21:50  terano
 * Fixed a bug in i3.
 *
 * Revision 1.1  1998/01/06 05:19:18  ceres
 * Initial revision
 * 
*/
/* client for OOB services of the Intermud 3 Protocol.
 * By Turrican@Discworld, 9-8-96. Based on code in the NM lib.
 */

#include <network.h>

inherit CLIENT;

private nosave mapping Connections = ([]);

protected void create() {
  client::create();
  SetSocketType(MUD);
  SetDestructOnClose(1);
}

int Connect(string mudname, int key, string func) {
  int fd, port;
  string ip, realname;
  mixed *info;

  if (previous_object() != SERVICES_D) return EESECURITY;
  realname = INTERMUD_D->GetMudName(mudname);
  if (!realname)
    return EEBADADDR;
  info = INTERMUD_D->GetMudList()[realname];
  ip = info[1];
  port = info[3];
  if (!port)
    return EETYPENOTSUPP;
  if (info[0] != -1)
    return EECONNREFUSED;
  if ((fd = eventCreateSocket(ip, port)) < 0)
    return EECONNECT;
  Connections[fd] = func;
  eventWrite(fd, ({ "oob-begin", mud_name(), 1, key }));
  return EESUCCESS;
}

protected void eventRead(int fd, mixed *packet) {
  if (!packet) return;
  switch (packet[0]) {
  case "oob-begin":
    if (undefinedp(Connections[fd])) return;
    call_other(SERVICES_D, Connections[fd], packet[1]);
    break;
  case "oob-end":
    dest_me();
    break;
  case "mail-ack":
    SERVICES_D->eventReceiveMailAck(packet);
    break;
  case "file-list-reply":
    SERVICES_D->eventReceiveFileList(packet);
    break;
  case "file-put-ack":
    SERVICES_D->eventReceivePutAck(packet);
    break;
  case "file-get-reply":
    SERVICES_D->eventReceiveFile(packet);
    break;
  default:
    break;
  }
}
