/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: oob_server.c,v 1.2 2002/08/18 15:21:50 terano Exp $
 * $Log: oob_server.c,v $
 * Revision 1.2  2002/08/18 15:21:50  terano
 * Fixed a bug in i3.
 *
 * Revision 1.1  1998/01/06 05:19:18  ceres
 * Initial revision
 * 
*/
/* Server for OOB services of the Intermud 3 Protocol.
 * By Turrican@Discworld, 21-3-96. Based on code in the NM lib.
 */

#include <network.h>

inherit SERVER;

#include "oob_server.h"

private nosave int Listening = -1;
private nosave mapping Connections;

protected void create() {
  server::create();
  Connections = ([]);
  SetSocketType(MUD);
  SetDestructOnClose(1);
}

void Setup() {
  int x;

  if (file_name(previous_object()) != SERVICES_D) return;
  remove_call_out("eventTimeout");
  if (Listening != -1) {
    call_out("eventTimeout", TIMEOUT, Listening);
    return;
  }
  if ((x = eventCreateSocket(PORT_OOB)) < 0)
    if (this_object()) destruct(this_object());
  call_out("eventTimeout", TIMEOUT, x);
  Listening = x;
}

protected void eventTimeout(int fd) {
  eventServerAbortCallback(fd);
}

protected void eventSocketClosed(int fd) {
  if (Connections[fd])
    SERVICES_D->eventTimeout(((class connection)Connections[fd])->MudName);
  map_delete(Connections, fd);
}

protected void eventRead(int fd, mixed *packet) {
  if (!packet || sizeof(packet) < 2) return;
  remove_call_out("eventTimeout");
  call_out("eventTimeout", TIMEOUT, fd);
  switch(packet[0]) {
    int key;
  case "oob-begin":
    if (sizeof(packet) != 4) {
      eventTimeout(fd);
      return;
    }
    key = (int)SERVICES_D->GetKey(packet[1]);
    if (!key || (!packet[2] && (key != -1)) || (packet[2] &&
                                                key != packet[3])) {
      eventTimeout(fd);
      return;
    }
    if (!Connections[fd]) {
      class connection tmp;

      tmp = new(class connection);
      tmp->MudName = packet[1];
      Connections[fd] = tmp;
    }
    SERVICES_D->eventTimeout(packet[1]);
    eventWrite(fd, ({"oob-begin", mud_name(), 0, 0}));
    break;
  case "oob-end":
    if (!Connections[fd]) {
      eventTimeout(fd);
      return;
    }
    if (((class connection)Connections[fd])->Buffer)
      eventWrite(fd, ((class connection)Connections[fd])->Buffer);
    eventWrite(fd, ({"oob-end", mud_name()}));
    break;
  case "mail":
    if (!Connections[fd]) {
      eventTimeout(fd);
      return;
    }
    SERVICES_D->eventReceiveMail(packet);
    break;
  case "mail-ack":
    if (!Connections[fd]) {
      eventTimeout(fd);
      return;
    }
    SERVICES_D->eventReceiveMailAck(packet);
    break;
  default:
    break;
  }
}
