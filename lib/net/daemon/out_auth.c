/*  -*- LPC -*-  */
/*
 * $Id: out_auth.c,v 1.3 1999/11/23 12:58:56 turrican Exp $
 */
/*
 * This will connect up to an auth demon and get it to return our
 * login name.
 * Written by Pinkfish.
 * Rewritten by Turrican on 23-4-96.
 */
#include <network.h>

inherit CLIENT;

class auth {
  object Object;
  string Function;
}

private nosave mapping found;

#define AUTH_PORT 113

protected void create() {
  client::create();
  SetSocketType(STREAM);
  SetDestructOnClose(1);
  found = ([]);
} /* create() */

void query_auth(string func) {
  string addr;
  int port, fd;
  class auth auth;

  /* Disable for now, until we find a way to stop denial-of-service
     attacks */
  call_other(previous_object(), func, 0);
  return;
  addr = socket_address(previous_object());
  if (!addr) {
    call_other(previous_object(), func, 0);
    return;
  }
  sscanf(addr, "%s %d", addr, port);
  if ((fd = eventCreateSocket(addr, AUTH_PORT)) < 0) {
    call_other(previous_object(), func, 0);
    return;
  }
  auth = new(class auth);
  auth->Object = previous_object();
  auth->Function = func;
  found[fd] = auth;
  eventWrite(fd, sprintf("%d , %d\r\n", port,
                         query_ip_port(previous_object())));
} /* query_auth() */

protected void eventRead(int fd, string mess) {
  string name;
  class auth auth;

  mess = replace_string(mess, "\r\n", "\n");
  sscanf(mess, "%*d , %*d : %*s : %*s : %s\n", name);
  if (!(auth = (class auth)found[fd])) {
    eventAbortCallback(fd);
    return;
  }
  if (objectp(auth->Object)) {
    call_other(auth->Object, auth->Function, name);
    auth->Object = 0;
  }
  eventAbortCallback(fd);
}

protected void eventSocketClose(int fd) {
  class auth auth = (class auth)found[fd];

  if (auth) {
    if (objectp(auth->Object))
      call_other(auth->Object, auth->Function, 0);
    map_delete(found, fd);
  }
} /* close_callback() */
