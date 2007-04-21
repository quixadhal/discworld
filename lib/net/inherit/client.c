/*  -*- LPC -*-  */
/*
 * $Locker: pinkfish $
 * $Id: client.c,v 1.3 2002/06/25 18:23:13 ceres Exp pinkfish $
 * $Log: client.c,v $
 * Revision 1.3  2002/06/25 18:23:13  ceres
 * Fixed compiler warning
 *
 * Revision 1.2  2000/06/21 20:35:36  pinkfish
 * Fix up some bits.
 *
 * Revision 1.1  1998/01/06 05:19:01  ceres
 * Initial revision
 * 
*/
/*    /secure/lib/client.c
 *    from the Nightmare IVr1 Object Library
 *    a TCP client object
 *    created by Descartes of Borg 950428
 *    modified for the Discworld mudlib by Turrican 25-10-95
 */

#include <network.h>
#include "client.h"

private nosave int DestructOnClose, SocketType = -1;
private nosave string LogFile;
private nosave function Read;
private nosave mapping Sockets;

void create() {
  seteuid(getuid());
  Sockets = ([]);
}

int eventCreateSocket(string host, int port) {
  int x;
  class client Socket;

  Socket = new(class client);
  Socket->Blocking = 1;
  if( SocketType == -1 ) SocketType = STREAM;
  x = socket_create(SocketType, "eventReadCallback", "eventAbortCallback");
  if( x < 0 ) {
    eventSocketError("Error in socket_create().", x);
    return x;
  }
  Socket->Descriptor = x;
  x = socket_bind(Socket->Descriptor, 0);
  if( x != EESUCCESS ) {
    eventClose(Socket);
    eventSocketError("Error in socket_bind().", x);
    return x;
  }
  x = socket_connect(Socket->Descriptor, host + " " + port, 
                     "eventReadCallback", "eventWriteCallback");
  if( x != EESUCCESS ) {
    eventClose(Socket);
    eventSocketError("Error in socket_connect().", x);
    return x;
  }
  Sockets[Socket->Descriptor] = Socket;
  return Socket->Descriptor;
}

protected void eventAbortCallback(int fd) {
  if (!Sockets[fd]) return;
  ((class client)Sockets[fd])->Closed = 1;
  eventClose((class client)Sockets[fd]);
}

protected void eventReadCallback(int fd, mixed val) {
  if( functionp(Read) ) evaluate(Read, fd, val);
  else eventRead(fd, val);
}

protected void eventRead(int fd, mixed val) { }

protected void eventWriteCallback(int fd) {
  int x;
  class client Socket;
  int times;

  if( !Sockets[fd] ) return;
  Socket = (class client)Sockets[fd];
  Socket->Blocking = 0;
  x = EESUCCESS;
  times = 10;
  while( Socket->Buffer && x == EESUCCESS && times > 0) {
    times--;
    switch( x = socket_write(fd, Socket->Buffer[0]) ) {
    case EESUCCESS:
      break;
    case EECALLBACK:
      Socket->Blocking = 1;
      break;
    case EEWOULDBLOCK:
      call_out( (: eventWriteCallback($(fd)) :), 0);
      return;
    case EEALREADY:
      Socket->Blocking = 1;
      return;
    default:
      eventClose(Socket);
      eventSocketError("Error in socket_write().", x);
      return;
    }
    if( sizeof(Socket->Buffer) == 1 ) Socket->Buffer = 0;
    else Socket->Buffer = Socket->Buffer[1..];
  }
}

void eventWrite(int fd, mixed val) {
  class client Socket;

  if( !Sockets[fd] ) return;
  Socket = (class client)Sockets[fd];
  if( Socket->Buffer ) Socket->Buffer += ({ val });
  else Socket->Buffer = ({ val });
  if( Socket->Blocking ) return;
  else eventWriteCallback(fd);
}

protected void eventClose(class client sock) {
  if (!sock) return;
  if (!sock->Closed && (socket_close(sock->Descriptor) != EESUCCESS))
    return;
  if (!undefinedp(Sockets[sock->Descriptor]))
    map_delete(Sockets, sock->Descriptor);
  eventSocketClose(sock->Descriptor);
  sock = 0;
  if( DestructOnClose && !sizeof(keys(Sockets))) destruct(this_object());
}

protected void eventSocketClose(int fd) { }

int dest_me() {
  class client Socket;

  foreach (Socket in (class client *)values(Sockets))
    eventClose(Socket);
  if (this_object()) destruct(this_object());
  return 1;
}

protected void eventSocketError(string str, int x) { 
  if( LogFile ) 
    log_file(LogFile, ctime(time()) + "\n" + socket_error(x) + "\n");
}

function SetRead(function f) { return (Read = f); }

int SetSocketType(int type) { return (SocketType = type); }

int SetDestructOnClose(int x) { return (DestructOnClose = x); }
