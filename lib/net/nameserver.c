/*
 * Reads in the nameserver config file and gets all of the
 * ip's from it.
 */
#include "socket.h"
#include "inet.h"
#define SERVER_PORT 7685
#define TIMEOUT (5*60)
#define CONFIG "/net/config/"
#define ME machines[mud_name()]
#define JUST_ADDR(BING) sscanf(BING, "%s %d", BING, womble)

#define NAME 0
#define REVERSE 1
#define LAST_REQ 2
#define SERVICE 3

mapping machines,
        reverse,
        services,
        failed,
        requests,
        service_req,
        sent,
        my_write,
        services;
string *nameservers,
       *pending;
int my_port, womble;
int my_socket, open_server, current_ns;

void load_mappings(string str, string gf);
void setup_nameserver(int port);
void query_nameserver();
void close_callback(int fd);
void write_callback();
void write_service(int fd);

void create() {
  string bing;

  nameservers = ({ });
  machines = ([ ]);
  services = ([ "default" : ([ ]) ]);
  failed = ([ ]);
  reverse = ([ ]);
  pending = ({ });
  sent = ([ ]);
  requests = ([ ]);
  my_write = ([ ]);
  load_mappings(CONFIG+"nameserver", CONFIG+"services");
/* The default... */
  setup_nameserver(my_port);
} /* create() */

void load_mappings(string filen, string ser) {
  string file, *bits;
  string name, address, flags, tmp;
  int i, port, de_port;

  file = read_file(filen);
  if (file) {
/* Need to do it twice me thinks.  Other wise odd sized bits would loose. */
    file = implode(explode(file, " ") - ({ "" }), " ");
    bits = explode(file, "\n");
    for (i=0;i<sizeof(bits);i++) {
/* Do we have a comment? */
      if (bits[i][0] == '#' || !strlen(bits[i]))
        continue;
/* Ok, addresses are of the form, mudname address flags */
/* flags are NS for nameserver.  (All that is named at the moment) */
      flags = "";
      if (sscanf(bits[i], "%s %s", name, address) == 2) {
        port = SERVER_PORT;
        if (sscanf(address, "%s:%d%s", address, port, flags) != 3)
          sscanf(address, "%s %s", address, flags);
        else
          sscanf(flags, " %s", flags);
        switch (lower_case(flags)) {
          case "ns" :
            nameservers += ({ address+" "+port });
            break;
        }
        machines[name] = address+" "+port;
/* Case for multiple muds? frog is lets sleeze it... */
        reverse[address] = name;
        reverse[address+" "+port] = name;
      }
    }
  }
  my_port = SERVER_PORT;
  sscanf(ME, "%s %d", file, my_port);
  file = read_file(ser);
  if (file) {
    file = implode(explode(file, " ") - ({ "" }), " ");
    file = lower_case(file);
    bits = explode(file, "\n");
    for (i=0;i<sizeof(bits);i++) {
      if (bits[i][0] == '#' || !strlen(bits[i]))
        continue;
/* The pattern is, name, port */
      if (sscanf(bits[i], "%s %s %s", name, address, tmp)) {
        if (name != "default") {
          name = machines[name];
          de_port = SERVER_PORT;
          sscanf(name, "%s %d", file, de_port);
        } else
          de_port = 0;
/* Can only set up things about known muds... */
        if (!name) continue;
        if (!services[name])
          services[name] = ([ ]);
        if (tmp[0] == '-') {
          sscanf(tmp[1..100], "%d", port);
          services[name][address] = de_port+port;
        } else if (tmp[0] == '+') {
          sscanf(tmp[1..100], "%d", port);
          services[name][address] = de_port+port;
        } else if (sscanf(tmp, "%d", port) == 1)
          if (name == "default")
            services[name][address] = ({ port });
          else
            services[name][address] = port;
      }
    }
  }
} /* load_mappings() */

/*
 * Ok, we lookup in our nice array, if we get a hit we returns that name
 * if not, we try the first name server.  Bit of a problem here though.
 *
 * Means the return time is not deterministic.
 * Oh well.  They will have to live with it.
 * Keep a failed array so we don't try the same name several million times.
 */
void do_request(int type, string name, string func, mixed args, string *ser) {
  if (!ser) ser = ({ });
  if (!requests[type])
    requests[type] = ([ ]);
  if (!requests[type][name]) {
    pending += ({ type, name });
    if (current_ns == 0)
      requests[type][name] = ({ sizeof(nameservers)-1, ser });
    else
      requests[type][name] = ({ current_ns - 1, ser });
  } else
    requests[type][name][1] += ser;
  requests[type][name] += ({ ({ previous_object(), func, args }) });
  query_nameserver();
} /* do_request() */

void service_request(string name, string host, mixed ob, string func,
                     mixed args) {
  int fd, i;

  if (!service_req[host]) {
/* We need to open one... */
    fd = socket_create(STREAM, "got_service", "close_service");
    i = socket_connect(fd, host+" "+SERVER_PORT,
                       "got_service", "write_service");
    if (i < 0) {
      JUST_ADDR(host);
      call_other(ob, func, name, host, 0, args);
      return ;
    }
    service_req[fd] = host;
    service_req[host] = ({ fd, 2, ({ ob, func, name, args }) });
    return ;
  }
  service_req[host] += ({ ({ ob, func, name, args }) });
  write_service(service_req[host][0]);
} /* service_request() */

void got_service(int fd, string mess) {
  string name, host, str, host2;
  int port, i;
  mixed *bit;

  sscanf(mess, "%s^%d", name, port);
  host = service_req[fd];
  if (!host) return ;
  for (i=2;i<sizeof(service_req[host]);i++)
    if (service_req[host][i][2] == name) {
      bit = service_req[host][i];
      services[host][bit[2]] = port;
      host2 = host;
      JUST_ADDR(host2);
      call_other(bit[0], bit[1], bit[2], host2, port, bit[3]);
      service_req[host] = delete(service_req[host], i, 1);
      if (service_req[host][1] >= i)
        service_req[host][1]--;
      i--;
    }
  if (sizeof(service_req[host]) == 2) {
/* Just the control info... */
    map_delete(service_req, host);
    map_delete(service_req, fd);
    socket_close(fd);
  }
} /* got_service() */

void close_service(int fd) {
  int i;
  string host, host2;
  mixed *bit;

/* This means we fluffed it... */
  host2 = host = service_req[fd];
  if (!host) return ;
  JUST_ADDR(host2);
  for (i=2;i<sizeof(service_req[host]);i++) {
    bit = service_req[host][i];
    call_other(bit[0], bit[1], bit[2], host2, 0, bit[3]);
  }
  map_delete(service_req, host);
  map_delete(service_req, fd);
} /* close_service() */

void write_service(int fd) {
  string host;
  int pos;

  host = service_req[fd];
  if (!host) {
/* How did this get here? */
    socket_close(fd);
    return ;
  }
/* Anything left to write? */
  if ((pos = service_req[host][1]) > sizeof(service_req[host]))
    return ;
/* Yep! */
  if (socket_write(fd, SERVICE+"^"+service_req[host][pos][2]+"^0") < 0)
    return ;
  service_req[host][1]++;
} /* write_service() */

void lookup_mud(string name, string finitio, mixed args, string *ser) {
  string host;

  if (machines[name]) {
    host = machines[name];
/*
    JUST_ADDR(host);
 */
    call_other(previous_object(), finitio, name, host, args);
    return ;
  }
  if (failed[name] || !sizeof(nameservers)) {
    call_other(previous_object(), finitio, name, 0, args);
    return ;
  }
/* Ok, ask any name servers we know about */
  do_request(NAME, name, finitio, args, ser);
} /* lookup_mud() */

void reverse_lookup(string address, string func, mixed args, string *ser) {
  string *bits;
  int i;

  bits = keys(machines);
  if ((i=member_array(address, bits)) != -1) {
    bits = values(machines);
    call_other(previous_object(), func, address, bits[i], args);
    return ;
  }
  if (failed[address] || !sizeof(nameservers)) {
    call_other(previous_object(), func, address, 0, args);
    return ;
  }
/* Ok, now we be a nice little frog and setup a nameserver lookup thingy. */
  do_request(REVERSE, address, func, args, ser);
} /* reverse_lookup() */

void lookup_service(string name, string host, string func, string args) {
  string blue;

  if (host[0] < '0' || host[0] > '9') {
/* Need to lookup the host first... */
    this_object()->lookup_mud(host, "finish_hostlookup",
                           ({ previous_object(), func, name, args }), ({ }));
    return ;
  }
  if (services["default"][name]) {
    string hname;
    int port;

    port = SERVER_PORT;
    sscanf(host, "%s %d", hname, port);
    if (pointerp(services["default"][name]))
      call_other(previous_object(), func, name, hname,
                                          services["default"][name][0], args);
    else
      call_other(previous_object(), func, name, hname,
                                          port+services["default"][name], args);
    return ;
  }
  if (!host || host == ME) {
    host = ME;
    JUST_ADDR(host);
    if (!services[ME] || !services[ME][name])
      call_other(previous_object(), func, name, host, 0, args);
    else
      call_other(previous_object(), func, name, host, services[ME][name], args);
    return ;
  }
/* Irk!  Now we need to look the damn thing up :( */
  if (services[host][name]) {
    blue = host;
    JUST_ADDR(blue);
    call_other(previous_object(), func, name, blue, services[host][name], args);
    return ;
  }
  service_request(name, host, previous_object(), func, args);
} /* lookup_service() */

void finish_hostlookup(string host, string found, mixed *args) {
  if (!found) {
    call_other(args[0], args[1], args[2], found, 0, args[3]);
    return ;
  }
  if (services["default"][args[2]]) {
    string hname;
    int port;

    port = SERVER_PORT;
    sscanf(found, "%s %d", hname, port);
    if (pointerp(services["default"][args[2]]))
      call_other(args[0], args[1], args[2], hname,
                             services["default"][args[2]][0], args[3]);
   else
      call_other(args[0], args[1], args[2], hname,
                             services["default"][args[2]]+port, args[3]);
    return ;
  }
  if (services[found][args[2]]) {
    host = found;
    JUST_ADDR(host);
    call_other(args[0], args[1], args[2], host, services[found][args[2]],
                                 args[3]);
    return ;
  }
  service_request(args[2], found, args[0], args[1], args[3]);
} /* finish_hostlookup() */

/*
 * This will open a connection to a nameserver and ask it, hey!  Do you
 * know about this name?
 *
 * This same port is used for service lookups.  So we need a nice flag
 * We also keep a list of places we have been.  To stop loops.
 */
void query_nameserver() {
  int i;
  if (open_server) {
    write_callback();
    return ;
  }
  open_server = socket_create(STREAM, "got_address", "close_callback");
  i = socket_connect(open_server, nameservers[current_ns],
                     "got_address", "write_callback");
  if (i < 0)
    close_callback(0);
/*
  else
    write_callback();
 */
} /* query_nameserver() */

void finish_request(int type, string first, mixed ret) {
  mixed *bing;
  int i;

  if (!requests[type])
    return ;
  if (!requests[type][first])
    return ;
  bing = requests[type][first];
  for (i=2;i<sizeof(bing);i++)
    catch(call_other(bing[i][0], bing[i][1], first, ret, bing[i][2]));
  map_delete(requests[type], first);
  if (!sizeof(requests[type]))
    map_delete(requests, type);
} /* finish_request() */

/*
 * This will only get called if our lookup failed.  See if we asked
 * the last name server, if so.  Sulk.
 */
int check_finish(int type, string first) {
  if (!requests[type] || !requests[type][first])
    return 1; /* Finished */
  if (requests[type][first][0] == current_ns) {
    finish_request(type, first, 0);
    return 1;
  }
} /* check_finish() */

void got_address(int fd, mixed message) {
  int i;
  int type;
  string name, res, tmp;

  sscanf(message, "%d^%s^%s", type, name, res);
  if (sent[type]) {
    sent[type] = (mixed *)sent[type] - ({ name });
    if (!sizeof(sent[type]))
      map_delete(sent, type);
  }
  switch (type) {
    case NAME :
      if (res== "0") {
/* Frog! We failed!  Sulk. */
/* Sigh....  We failed tell em about the bad news */
        if (check_finish(type, name))
          failed[type][name] = 1;
        break;
      }
/* We got an answer!  Yes! */
      tmp = res;
/*  We want more than just an address for this bit.  We just want an 
 * address with services...
      JUST_ADDR(tmp);
 */
      finish_request(type, name, tmp);
      machines[name] = res;
      break;
    case REVERSE :
      if (res == "0") {
        check_finish(type, name);
        break;
      }
      finish_request(type, name, res);
      machines[res] = name;
      break;
  }
/* Wait till we give up on this nameserver and move onto the next */
  if (!sizeof(sent) && !sizeof(pending)) {
/* Replyed to all of them */
    socket_close(open_server);
/* Does this call the close function? */
    close_callback(open_server);
  }
} /* got_address() */

void setup_nameserver(int port) {
  int i;

  my_socket = socket_create(STREAM, "read_callback", "interaction_close");
  socket_bind(my_socket, port);
  i = socket_listen(my_socket, "connect_callback");
} /* setup_nameserver() */

void write_callback() {
  string *path, s;
/*
 * Put all of the nameservers in the path as we will ask them all eventually.
 */
  if (!sizeof(pending))
    return ;
  if (requests[pending[0]])
    path = requests[pending[0]][1] + nameservers + ({ machines[mud_name()] });
  else
    path = nameservers + ({ machines[mud_name()] });
  if (socket_write(open_server, (s=(pending[0]+"^"+pending[1]+"^"
                                +implode(path, "$")))) >= 0) {
    if (sent[pending[0]])
      sent[pending[0]] += ({ pending[1] });
    else
      sent[pending[0]] = ({ pending[1] });
    pending = pending[2..1000];
  }
} /* write_callback() */

void connect_callback(int fd) {
  int connected;

  if ((connected = socket_accept(fd, "read_callback",
                                     "my_write_callback")) < 0) {
    return ;
  }
  my_write[connected] = 1;
  call_out("time_out", TIMEOUT, connected);
} /* connect_callback() */

void time_out(int fd) {
  socket_close(fd);
  map_delete(my_write, fd);
} /* time_out() */

void my_write_callback(int fd) {
  if (!my_write[fd])
    my_write[fd] = 1;
  if (intp(my_write[fd]))
    return ;
  if (socket_write(fd, my_write[fd][0]) >= 0)
    my_write[fd] = my_write[fd][1..1000];
  if (!sizeof(my_write[fd]))
    map_delete(my_write, fd);
} /* my_write_callback() */

void interaction_close(int fd) {
  map_delete(my_write, fd);
} /* interaction_close() */

void send_back(int fd, string mess) {
  if (my_write[fd] && intp(my_write[fd])) {
/* Its ready to be sent to. */
    if (socket_write(fd, mess) < 0) {
/* Hmm.  bit of the old fail there... */
      my_write[fd] = ({ mess });
      return ;
    }
    map_delete(my_write, fd);
    return ;
  }
  my_write[fd] += ({ mess });
} /* send_back() */

void read_callback(int fd, mixed message, mixed blue) {
  string *bits;
  int i, type;
  string name, path;
  string *new_p;

  if (message[0] >= 'A' && message[0] <= 'Z') {
/* They are talking to our service.  Oblige them. */
    sscanf(message, "%s^%s", name, path);
    INETD->service_contact(fd, name, path);
    return ;
  }
  sscanf(message, "%d^%s^%s", type, name, path);
  new_p = explode(path, "$");
  switch (type) {
    case NAME :
/* Name lookup */
      if (machines[name]) {
        send_back(fd, type+"^"+name+"^"+machines[name]);
        return ;
      }
/*
 * Ok, we don't know about it.  Do we know about any other nameservers
 * not in the asked list?
 */
      if (sizeof(nameservers - new_p)) {
/* There are some! */
        lookup_mud(name, "tell_other_server", ({ fd, type }),
                   new_p);
      } else {
        send_back(fd, type+"^"+name+"^"+machines[name]);
      }
      return ;
    case REVERSE :
/* Reverse name lookup */
      bits = keys(machines);
      if ((i = member_array(name, bits)) != -1) {
        bits = values(machines);
        send_back(fd, type+"^"+name+"^"+bits[i]);
        return ;
      }
      if (sizeof(nameservers - new_p)) {
        reverse_lookup(name, "tell_other_server", ({ fd, type }),
                       new_p);
      } else {
        send_back(fd, type+"^"+name+"^0");
      }
      return ;
    case SERVICE :
/* Service lookup */
      if (!services[ME])
        send_back(fd, type+"^"+name+"^0");
      else
        send_back(fd, type+"^"+name+"^"+services[ME][name]);
      return ;
  } 
} /* read_callback() */

void tell_other_server(string name, string address, mixed *fd) {
/*
 * Don't bother checking if it is still around.  Just shove stuff down it.
 * it wont complain.
 */
  socket_write(fd[0], fd[1]+"^"+name+"^"+address);
} /* tell_other_server() */

void close_callback(int fd) {
  int i, j;
  mixed *tmp;

/*
 * This should be one of our outstanding connections to the rest of
 * of the world
 */
  current_ns++;
  if (current_ns >= sizeof(nameservers))
    current_ns = 0;
  pending = ({ });
  for (i=0;i<LAST_REQ;i++) {
    if (!requests[i]) continue;
    tmp = keys(requests[i]);
    for (j=0;j<sizeof(tmp);j++) {
/* Check for fail */
      if (!check_finish(i, tmp[j]) &&
          member_array(nameservers[current_ns], requests[i][tmp[j]][1]) == -1)
        pending += ({ i, tmp[j] });
    }
    if (!sizeof(requests[i]))
      map_delete(requests, i);
  }
  if (sizeof(pending)) {
    open_server = 0;
    query_nameserver();
  } else if (sizeof(requests))
/* We still have some unfinished bisness. */
    close_callback(open_server);
  else
    open_server = 0;
} /* close_callback() */

void interaction_close(int fd) {
  INETD->close_service(fd);
} /* interaction_close() */

mapping query_services() { return services + ([ ]); }
