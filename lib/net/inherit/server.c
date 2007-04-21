/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: server.c,v 1.1 1998/01/06 05:19:01 ceres Exp $
 * $Log: server.c,v $
 * Revision 1.1  1998/01/06 05:19:01  ceres
 * Initial revision
 * 
*/
/*    /lib/server.c
 *    from the Nightmare IV LPC Library
 *    a TCP server object
 *    created by Descartes of Borg 950429
 *    Hacked for Discworld by Turrican, 21-3-96.
 *    Modified by Turrican to allow files to be sent over, 23-4-96.
 *    Modified by Turrican to allow files to be partly sent over, 2-7-97.
 */


#include <network.h>
#include <server.h>

private nosave int DestructOnClose, SocketType = -1;
private nosave function Read;
private nosave class server Listen;
private nosave mapping Sockets;

protected void create() {
  seteuid(getuid());
  Sockets = ([]);
} /* create() */

int eventCreateSocket(int port) {
  int x;

  if (SocketType == -1)
    SocketType = STREAM;
  x = socket_create(SocketType, "eventServerReadCallback", 
                    "eventServerAbortCallback");
  if( x < 0 ) {
    eventSocketError("Error in socket_create().", x);
    return x;
  }
  Listen = new(class server);
  Listen->Blocking = 0; /* servers are not blocking to start */
  Listen->Descriptor = x;
  x = socket_bind(Listen->Descriptor, port);
  if (x != EESUCCESS) {
    eventClose(Listen);
    eventSocketError("Error in socket_bind().", x);
    return x;
  }
  x = socket_listen(Listen->Descriptor, "eventServerListenCallback");
  if (x != EESUCCESS) {
    eventClose(Listen);
    eventSocketError("Error in socket_listen().", x);
    return x;
  }
  return x;
} /* eventCreateSocket */

protected void eventServerListenCallback(int fd) {
  int x;

  x = socket_accept(fd, "eventServerReadCallback", 
                    "eventServerWriteCallback");
  if (x < 0) {
    eventSocketError("Error in socket_accept().", x);
    return;
  }
  eventNewConnection(x);
} /* eventServerListenCallback() */

void eventServerAbortCallback(int fd) {
  if (Listen && Listen->Descriptor == fd)
    eventClose(Listen);
  else if (Sockets[fd])
    eventClose(Sockets[fd]);
} /* eventServerAbortCallback() */

protected void eventServerReadCallback(int fd, mixed val) {
  if (functionp(Read))
    evaluate(Read, fd, val);
  else
    eventRead(fd, val);
} /* eventServerReadCallback() */

protected void eventRead(int fd, mixed val) {
} /* eventRead() */

protected void eventServerWriteCallback(int fd) {
  class server sock;
  class buff t;
  int x;

  if (Listen && Listen->Descriptor == fd)
    sock = Listen;
  else if (Sockets[fd])
    sock = (class server)Sockets[fd];
  else
    return;
  sock->Blocking = 0;
  if (!sock->Buffer && sock->Closing) {
    eventClose(sock);
    return;
  }
  x = EESUCCESS;
  while (sock->Buffer && x == EESUCCESS) {
    t = sock->Buffer[0];
    if (t->Type == FILE) {
      if (t->Size < (t->Pos+BLOCK_SIZE))
        t->Value = read_buffer(t->Filename, t->Pos, t->Size - t->Pos);
      else
        t->Value = read_buffer(t->Filename, t->Pos, BLOCK_SIZE);
      t->Pos += BLOCK_SIZE;
    }
    switch (x = socket_write(sock->Descriptor, t->Value)) {
    case EESUCCESS:
      break;
    case EECALLBACK:
      sock->Blocking = 1;
      break;
    case EEWOULDBLOCK:
      if (t->Pos)
        t->Pos -= BLOCK_SIZE;
      call_out( (: eventServerWriteCallback :), 1, fd);
      return;
    case EEALREADY:
      sock->Blocking = 1;
      return;
    default:
      eventClose(sock);
      eventSocketError("Error in socket_write().", x);
      return;
    }
    if (t->Type == STRING || t->Pos > t->Size) {
      if (sizeof(sock->Buffer) == 1) {
        sock->Buffer = 0;
        if (sock->Closing && !sock->Blocking)
          eventClose(sock);
      } else
        sock->Buffer = sock->Buffer[1..];
    }
  }
} /* eventServerWriteCallback() */

varargs void eventWrite(int fd, mixed val, int close, int type, int pos,
                        int size) {
  class server sock;
  class buff t;

  if (Listen && Listen->Descriptor == fd)
    sock = Listen;
  else if (Sockets[fd])
    sock = (class server)Sockets[fd];
  else
    return;
  if (val) {
    t = new(class buff);
    t->Type = type;
    if (type == FILE) {
      t->Filename = val;
      if (size)
        t->Size = size;
      else
        t->Size = file_size(val);
      t->Pos = pos;
    } else
      t->Value = val;
    if (sock->Buffer)
      sock->Buffer += ({ t });
    else
      sock->Buffer = ({ t });
  }
  sock->Closing = close;
  if (sock->Blocking)
    return;
  else
    eventServerWriteCallback(sock->Descriptor);
} /* eventWrite() */

protected void eventClose(class server sock) {
  if (!sock)
    return;
  if (Sockets[sock->Descriptor])
    map_delete(Sockets, sock->Descriptor);
  socket_close(sock->Descriptor);
  eventSocketClosed(sock->Descriptor);
  sock = 0;
  if (DestructOnClose && sock == Listen)
    destruct(this_object());
} /* eventClose() */

protected void eventSocketClosed(int fd) {
} /* eventSocketClosed() */

int dest_me() {
  eventClose(Listen);
  if (this_object())
    destruct(this_object());
  return 1;
} /* dest_me() */

protected void eventNewConnection(int fd) {
  Sockets[fd] = new(class server, Descriptor : fd);
} /* eventNewConnection() */

protected void eventSocketError(string str, int x) {
} /* eventSocketError() */

function SetRead(function f) {
  return (Read = f);
} /* SetRead() */

int SetSocketType(int type) {
  return (SocketType = type);
} /* SetSocketType() */

int SetDestructOnClose(int x) {
  return (DestructOnClose = x);
} /* SetDestructOnClose() */
