/*
 * The wholely non useful inetd written by pinkfish@discworld.
 * Blue.
 */
#include "socket.h"

/*
 * inetd type means it all runs through ident's bound socket.
 * It does all sort of useful and hairy things this way.
 */
#define VALID_TYPE ({ "mud", "tcp", "udp", "inetd" })
#define INETD_TYPE 3
#define CONFIG "/net/config/"
#define NAMESERVER "/net/nameserver"

/* This is indexed by file descriptor */
mapping services,
        to_go,
        my_valid,
        close_it;
int my_socket;

void load_config(string str);
void close_callback(int fd);

void create() {
  seteuid(getuid());
  services = ([ ]);
  to_go = ([ ]);
  my_valid = ([ ]);
  close_it = ([ ]);
  load_config(CONFIG+"inetd");
} /* create() */

/* Make sure our inetd server is running */
void connect_inetd() {
  if (my_socket) /* Already up */
    return ;
  my_socket = socket_create(STREAM, "inetd_read", "close_callback");
  if (my_socket < 0) {
    write_file("/log/INETD", "Failed to find service create socket "+
               socket_error(my_socket)+".\n");
    return ;
  }
  NAMESERVER->lookup_service("inetd", mud_name(), "finish_lookup");
} /* connect_inetd() */

void finish_lookup(string name, string host, int port) {
  int ret;

  if (!port)
    return ;
  ret = socket_bind(my_socket, port);
  if (ret < 0) {
    socket_close(my_socket);
    my_socket = 0;
    write_file("/log/INETD", "Failed to bind socket for "+name+
               "-"+socket_error(my_socket)+".\n");
    return ;
  }
  ret = socket_listen(my_socket, "inetd_connect");
  if (ret < 0) {
    socket_close(my_socket);
    my_socket = 0;
    write_file("/log/INETD", "Failed to listen socket for "+name+
               "-"+socket_error(my_socket)+".\n");
    return ;
  }
} /* finish_lookup() */

void load_config(string name) {
  string data,
         str, file,
         *lines;
  int i, type;

  data = read_file(name);
  if (!data) return ;
  data = replace(data, "\t", " ");
  data = replace(data, "  ", " ");
  data = replace(data, "  ", " ");
  lines = explode(data, "\n");
  for (i=0;i<sizeof(lines);i++) {
    while (strlen(lines[i]) && lines[i][0] == ' ')
      lines[i] = lines[i][1..1000];
    if (!strlen(lines[i]) || lines[i][0] == '#') continue;
    if (sscanf(lines[i], "%s %s %s", name, str, file) == 3) {
      if ((type = member_array(str, VALID_TYPE)) == -1)
        continue;
      if (type == INETD_TYPE) {
        my_valid[name] = file;
        connect_inetd();
      } else
        NAMESERVER->lookup_service(name, mud_name(), "found_service",
                                 ({ type, file }));
    }
  }
} /* load_config() */

void found_service(string name, string host, int port, mixed args) {
  int fd, ret;

/* Failed :( */
  if (!port) {
    write_file("/log/INETD", "Failed to find service "+name+".\n");
    return ;
  }
/* Ok...  we have a service...  so set ourselves up a look thingy */
  fd = socket_create(args[0], "read_callback", "close_callback");
  if (fd < 0) {
    write_file("/log/INETD", "Failed to create socket for "+name+
               "-"+socket_error(fd)+".\n");
    return ;
  }
  ret = socket_bind(fd, port);
  if (ret < 0) {
    socket_close(fd);
    write_file("/log/INETD", "Failed to bind socket for "+name+
               "-"+socket_error(fd)+".\n");
    return ;
  }
  if (args[0] != DATAGRAM) {
    ret = socket_listen(fd, "listen_callback");
    if (ret < 0) {
      socket_close(fd);
      write_file("/log/INETD", "Failed to listen socket for "+name+
               "-"+socket_error(fd)+".\n");
      return ;
    }
  }
/* Ok...  tis setup.  Bing! */
/* Do we need to remember what port we are on?  Nahhh... */
  services[fd] = args[1];
} /* found_service() */

void read_callback(int fd, string str) {
  string file;

  file = services[fd];
  if (!file) {
    socket_close(fd);
    return ;
  }
  file->read_callback(fd, str);
} /* read_callback() */

void write_callback(int fd) {
  int last, i;

  if (!to_go[fd])
    to_go[fd] = 1;
  if (!pointerp(to_go[fd])) {
    services[fd]->write_callback(fd);
    return ;
  }
  while (sizeof(to_go[fd]) && (last = socket_write(fd, to_go[fd][0])) >= 0)
    to_go[fd] = to_go[fd][1..100];
  if (!sizeof(to_go[fd])) {
    to_go[fd] = (last >= 0);
    if (to_go[fd]) {
      if (close_it[fd]) {
        close_callback(fd);
        map_delete(close_it, fd);
      } else
        services[fd]->write_callback(fd);
    }
  }
} /* write_callback() */

void write_fd(int fd, mixed mess) {
  int bing;

/* Security violation... */
  if (previous_object() != this_object() &&
      previous_object() != services[fd] &&
      file_name(previous_object()) != services[fd])
    return ;
  if (!pointerp(to_go[fd])) {
/*
 * If in here there is nothing in the queue already.  so we assume we can
 * send down the link.
 */
    to_go[fd] = ({ mess });
    bing = 1;
  } else
    to_go[fd] += ({ mess });
  if (bing)
    write_callback(fd);
} /* write_fd() */

/*
 * This sets the close_it flag or closes it immediately if there is
 * nothing to go and we have the correct flags.
 */
void close_fd(int fd) {
  int bing;

  if (previous_object() != this_object() &&
      previous_object() != services[fd] &&
      file_name(previous_object()) != services[fd])
    return ;
  bing = intp(to_go[fd]) && to_go[fd];
  if (bing) {
    close_callback(fd);
  } else
    close_it[fd] = 1;
} /* close_fd() */

void close_callback(int fd) {
  if (!services[fd])
    return ;
  services[fd]->close_callback(fd);
  map_delete(services, fd);
  map_delete(my_valid, fd);
/* Just to make sure... */
  socket_close(fd);
} /* close_callback() */

void listen_callback(int fd) {
  int new_fd;

  if ((new_fd = socket_accept(fd, "read_callback", "write_callback")) < 0) {
    write_file("/log/INETD", "Failed to accept a connection "+
               "-"+socket_error(new_fd)+".\n");
    return ;
  }
  services[new_fd] = services[fd];
/* whats all this then? */
  call_out("close_callback", 5*60*60, new_fd);
  to_go[new_fd] = 1;
} /* listen_callback() */

void inetd_connect(int fd) {
  int new_fd;

  if ((new_fd = socket_accept(fd, "inetd_read", "inetd_write")) < 0) {
    write_file("/log/INETD", "Failed to accept a connection "+
               "-"+socket_error(new_fd)+".\n");
    return ;
  }
  services[new_fd] = "request";
  socket_write(new_fd, "SERVICE?\n");
  to_go[new_fd] = 1;
  call_out("close_callback", 5*60*60, new_fd);
} /* inetd_connect() */

void inetd_read(int fd, string mess) {
  string *bits;

  if (!services[fd]) {
    socket_close(fd);
    return ;
  }
  switch (services[fd]) {
    case "waiting" :
/* Should be asking us for a service... */
      if (!my_valid[fd]) {
        socket_close(fd);
        return ;
      }
      if (mess == "SERVICE?\n") {
/* They are... */
/*
 * The old one doesnt use a response code....  so...  We won't
 * either for now.
        services[fd] = "response";
 */
        if (my_valid[fd][2])
          socket_write(fd, my_valid[fd][0]+" "+my_valid[fd][2]+"\n");
        else
          socket_write(fd, my_valid[fd][0]+"\n");
        services[fd] = my_valid[fd][1];
        map_delete(my_valid, fd);
        services[fd]->connected(fd);
      }
      return ;
    case "response" :
      if (mess == "YES!\n") {
/* Ok!  Bings are us */
        if (!my_valid[fd]) {
          socket_close(fd);
          return ;
        }
        services[fd] = my_valid[fd][1];
        map_delete(my_valid, fd);
        services[fd]->connected(fd);
      } else {
        socket_close(fd);
        if (my_valid[fd])
          my_valid[fd][1]->failed("unknow service");
        map_delete(my_valid, fd);
        return ;
      }
      return ;
    case "request" :
      bits = explode(mess, "\n");
      bits = explode(bits[0], " ")+bits[1..1000];
/*
      mess = mess[0..strlen(mess)-2];  /* Strip the \n */
      mess = bits[0];
      if (!my_valid[mess]) {
/* Invalid service... */
        socket_write(fd, "UNKNOWN SERVICE\n");
        socket_close(fd);
        return ;
      }
/* Don't need the yes string.
      socket_write(fd, "YES!\n");
 */
      services[fd] = my_valid[mess];
      services[fd]->connected(fd);
      if (sizeof(bits) > 1) {
        read_callback(fd, implode(bits[1..1000], "\n"));
      }
      return ;
    default :
      read_callback(fd, mess);
      return ;
  }
} /* inetd_read() */

void inetd_write(int fd) {
  if (!services[fd]) {
    socket_close(fd);
    return ;
  }
  switch (services[fd]) {
    case "response" :
    case "waiting" :
    case "request" :
      to_go[fd] = 1;
      return ;
    default :
      write_callback(fd);
      return ;
  }
} /* inetd_write() */

/*
 * Sends a message off to the destination service, with closeing
 * and opening of sockets etc done all for you.
 * Maybe useful, dubious.
 */
void datagram_message(string name, string dest, string mess) {
  NAMESERVER->lookup_service(name, dest, "finish_datagram_open",
                             ({ previous_object(), name, mess }));
} /* datagram_message() */

/*
 * The last paramater bing is used for compatability with an older
 * version of inetd.  Please do not use this in any new daemons you
 * create.  I consider it somewhat of a hack.  This is also only
 * used in the INETD_TYPE case, not for streams or mud connections.
 */
void open_to_service(string name, int type, string dest, string bing) {
  if (type == DATAGRAM) {
/*
 * What we do here is resolv a named [port thingy and return that to
 * the calling object...
 */
    previous_object()->failed("unsupported type", name, name, dest);
    return ;
  }
  if (type == INETD_TYPE) {
    NAMESERVER->lookup_service("inetd", dest, "finish_inet_open", 
                                ({ previous_object(), name, bing }));
    return ;
  }
/* Else.... */
  NAMESERVER->lookup_service(name, dest, "finish_open",
                             ({ previous_object(), type }));
} /* open_to_service() */

void finish_open(string name, string host, int port, mixed *args) {
  int new_fd, ret;

  if (!args[0])
    return ;
  if (!port) {
    args[0]->failed("lookup", name, args[1], host);
    return ;
  }
  new_fd = socket_create(args[1], "read_callback", "close_callback");
  if (new_fd < 0) {
    args[0]->failed("socket_create", name, args[1], host, new_fd);
    return ;
  }
  ret = socket_connect(new_fd, host+" "+port, "read_callback",
                       "write_callback");
  if (ret < 0) {
    args[0]->failed("socket_connect", name, args[1], host, new_fd);
    return ;
  }
  call_out("close_callback", 5*60*60, new_fd);
  services[new_fd] = file_name(args[0]);
  args[0]->connected(new_fd);
} /* finish_open() */

void finish_inet_open(string name, string host, int port, mixed *args) {
  int new_fd, ret;

  if (!args[0])
    return ;
  if (!port) {
    args[0]->failed("lookup", name, args[1], host);
    return ;
  }
  new_fd = socket_create(STREAM, "inetd_read", "close_callback");
  if (new_fd < 0) {
    args[0]->failed("socket_create", name, args[1], host, new_fd);
    return ;
  }
  ret = socket_connect(new_fd, host+" "+port, "inetd_read",
                       "inetd_write");
  if (ret < 0) {
    args[0]->failed("socket_connect", name, args[1], host, new_fd);
    return ;
  }
  call_out("close_callback", 5*60*60, new_fd);
  my_valid[new_fd] = ({ args[1], file_name(args[0]), args[2] });
  services[new_fd] = "waiting";
} /* finish_inet_open() */

void finish_datagram_open(string name, string host, int port, mixed *args) {
  int s;

  if (!port) {
    if (args[0])
      args[0]->failed("lookup", name, args[1], host);
    return ;
  }
  s = socket_create(DATAGRAM, "blurble");
  if (s < 0) {
    if (args[0]) {
      args[0]->failed("socket_create", name, args[1], host);
    }
    return ;
  }
  if (socket_write(s, args[2], host+" "+port) < 0) {
/* Tarnation... */
    if (args[0])
      args[0]->failed("socket_write", name, args[1], host);
  }
  socket_close(s);
} /* finish_datagram_open() */
