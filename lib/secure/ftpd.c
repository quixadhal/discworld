/*  -*- LPC -*-  */
/*
 * $Locker: ceres $
 * $Id: ftpd.c,v 1.12 2003/03/21 02:28:03 ceres Exp ceres $
 * 
*/
/* 
 * FTP daemon, complete rewrite using classes.
 * By Turrican@Discworld, 21-4-96.
 */

#include <network.h>

inherit SERVER;

#include <ftp.h>
#include <localtime.h>
#include <player_handler.h>

#define DELAY_LOG_FLUSH 5

private nosave mapping socket_info, data_sockets;
private nosave mapping _log_file_info = ([ ]);
private nosave int _log_file_flush_id;

protected void create() {
  server::create();
  seteuid("Root");
  socket_info = ([]);
  data_sockets = ([]);
  SetSocketType(STREAM);
  write(mud_name() + "\n");
  if(mud_name() != "Discworld")
    call_out("setup_ftp", 2);
  else
    destruct(this_object());
} /* create() */

/**
 * This method flushes out all the buffered stuff for the log files.
 */
private void flush_log_files() {
  string fname;
  string data;
  
  _log_file_flush_id = 0;
  foreach (fname, data in _log_file_info) {
      map_delete(_log_file_info, fname);
      unguarded((: write_file, fname, data :));
   }
   _log_file_info = ([ ]);
} /* flush_log_files() */

void log_write(string name, string fmt, mixed *args ...) {
    if (!_log_file_flush_id) {
     _log_file_flush_id = call_out((: flush_log_files :), DELAY_LOG_FLUSH);
  }
  if (!_log_file_info[name]) {
     _log_file_info[name] = "";
  }
  if (sizeof(args)) {
    _log_file_info[name] += sprintf(fmt, args ...);
  } else {
    _log_file_info[name] += fmt;
  }
}

protected void setup_ftp() {
  int x;

  if ((x = eventCreateSocket(FTP_PORT)) < 0) {
    if (this_object()) destruct(this_object());
    return;
  }
  call_out("check_connections", 5*60);
} /* setup_ftp() */

/* returns an array of users connected to ftpd */
string *query_connections() {
  class session *vals, val;
  string *list;

  list = ({ });
  vals = (class session *)values(socket_info);
  foreach (val in vals) {
    if (val->user_name) {
      if (val->logged_in) {
        list += ({ capitalize(val->user_name) });
      } else {
        list += ({ "login" });
      }
    }
  }
  return list;
} /* query_connections() */

protected string ls(string path, int mask) {
   string *files, tmp, tmp2, creator, domain;
   int i, j, s, current_time;
   mixed *xfiles, *stats;
   
#ifdef DEBUG
    TP(sprintf("ls(%s,%d)\n",path,mask));
#endif
   /* If the path is a directory, get contents uynless MASK_D is set */
   if (!(mask & MASK_D) && file_size(path) == -2) {
      if (path[ <1 ] == '/')
         path += "*";
      else
         path += "/*";
   }
   
   /* begin narrow columnar "nlst" */
   if (!(MASK_L & mask)) {
      files = get_dir(path);
      /* can only happen if permissions are messed up at account level */
      if (!files)
         return "";
      if (!(MASK_A & mask))
         files -= ({ ".", ".." });
      if (!(i = sizeof( files )))
         return "";
      /* no wild cards...must have been the exact pathname to a file */
      if (strsrch(path, '*') == -1 && strsrch(path, '?') == -1) {
         return files[0] + "\n";
      }
      /* remove globber at end of path, leave a trailing slash */
      j = strsrch(path, '/', -1);
      path = path[0..j];
      while (i--) {
         /* scan next level down for files */
         tmp = sprintf("%s%s/", path, files[i]);
         if (MASK_F & mask) {
            if (strsrch(tmp, "/./") != -1 || strsrch(tmp, "/../") != -1) {
               files[i] += "/";
               continue;
            }
            if (file_size(tmp) == -2)
               files[i] += "/";
            else if (stat(tmp[0..<2])[2])
               files[i] += "*";
         }
      }
      if (MASK_C & mask)
         return sprintf("%-#70s\n", implode(files, "\n"));
      else
         return implode(files, "\n") + "\n";
   }
   
   /* don't recurse */
   if (!(MASK_R & mask)) {
      /* begin long "list" */
      xfiles = get_dir(path, -1);
      if (!(mask & MASK_A))
         xfiles = filter_array(xfiles, "check_dots", this_object());
      if (!xfiles || !(s = sizeof( xfiles )))
         return "total 0\n";
      files = allocate(s);
      
      /* the Unix-like file permissions are mainly for effect...hopefully it
       * isn't too much, since anything more would likely be too cpu intensive
       * and cause it to max eval... */
      creator = (string)master()->author_file(path);
      if (!creator) {
         creator = "Root";
      }
      domain = (string)master()->domain_file(path);
      if (!domain) {
         domain = "Root";
      }
      i = strsrch(path, '/', -1);
      if (i >= 0)
         path = path[0..i];
      current_time = time();
      for (i = 0; i < s; i++) {
         /* process timestamp */
         tmp2 = ctime((xfiles[i])[2]); /* get last modified timestamp */
         if ((xfiles[i])[2] + (6 * 30 * 24 * 60 * 60) < current_time ||
             (xfiles[i])[2] - (60 * 60) > current_time ) {
            /* MMM DD  YYYY */
            tmp = sprintf("%s  %s", tmp2[4..9], tmp2[20..23]);
         } else {
            /* MMM DD hh:mm */
            tmp = tmp2[4..15];
         }
         j = (xfiles[i])[1];   /* get filesize */
         if (j == -2) {
            /* directory */
            files[i] = sprintf("drwxrwxr-x   0 %-8s %-8s        0 %12s %s",
                               creator, domain, tmp,
                               (xfiles[i])[0]+((MASK_F & mask)?"/":""));
         } else {
            /* file */
            stats = stat(path + (xfiles[i])[0]);
            files[i] = sprintf("-rw%crw-r--   1 %-8s %-8s %8d %12s %s",
                               sizeof(stats) > 1 && stats[2] ? 'x' : '-',
                               creator, domain, j, tmp,
                               (xfiles[i])[0] + (sizeof(stats) > 1 && stats[2] &&
                                                 (MASK_F & mask)?"*":""));
         }
      }
      return sprintf("total %i\n",s)+implode(files, "\n")+"\n";
   }
   /* begin long recursive "list" WARNING! still experimental */
   if( path[<1 .. <1] != "*" )
      return ls( path, (mask & ~MASK_R) );
   path = path[ 0 .. <3 ];
   files = ({ "" });
   tmp = "";
   while( sizeof( files ) ) {
      reset_eval_cost();
      if( files[ 0 ] == "" )
         tmp += ls( path + "/*", (mask & ~MASK_R) );
      else
         tmp += "\n" +files[ 0 ][ 1 .. <1 ] + ":\n" +
                ls( path + files[ 0 ] + "/*", (mask & ~MASK_R) );
      xfiles = get_dir( path + files[ 0 ] + "/*", -1 );
      if( xfiles && ( s = sizeof( xfiles ) ) ) {
         for (i = 0; i < s; i++) {
            j = (xfiles[i])[1];   /* get filesize */
            if ((j == -2) && (xfiles[i][0] != ".") && (xfiles[i][0] != "..")) {
               files += ({ files[ 0 ] +"/"+ xfiles[i][0] });
            }
         }
      }
      files = files[ 1 .. <1 ];
   }
   return tmp;
} /* ls() */

protected void data_conn(int fd, string mess, string name, int type) {
   int new_fd, ret, data_mode;
   string data_mode_name, addr;
   class session sess = (class session)socket_info[fd];
   class dataconn t;
   
  if (type == STRING || (type == FILE && sess->type == STRING)) {
    data_mode_name = "ASCII";
    data_mode = STREAM;
  } else {
    data_mode_name = "BINARY";
    data_mode = STREAM_BINARY;
  }

  t = new(class dataconn);
  t->path = name;
  t->data = (type == STRING ? replace_string(mess, "\n", "\r\n") : mess);
  t->pos = sess->offset;
  t->parent_fd = fd;
  t->type = type;
  t->len = (type == STRING ? strlen(mess) : file_size(mess));

  if (sess->pasv_fd != -1) {
    if (sess->pasv_cb) {
      TP("Accepting after delay...\n");
      new_fd = socket_accept(sess->pasv_fd, "data_read_callback",
                             "data_write_callback");
      if (new_fd < 0) {
        eventWrite(fd, "425 Can't open data connection.\r\n");
        socket_close(sess->pasv_fd);
        sess->pasv_fd = -1;
        return;
      }
      socket_close(sess->pasv_fd);
      sess->pasv_fd = new_fd;
    } else {
      TP("No connection yet...\n");
      data_sockets[sess->pasv_fd] = t;
      return;
    }
  } else {
    sess->use_default = 1;
  
    if (sess->data_fd != -1) {
      eventWrite(fd, "425 Can't open data connection.\r\n");
      return;
    }
    new_fd = socket_create(data_mode, "data_read_callback",
                           "data_close_callback");

    if (new_fd < 0) {
      eventWrite(fd, "425 Can't create data socket.\r\n");
      return;
    }

    if (!sess->data_addr) {
      eventWrite(fd, "425 Can't open data connection.\r\n");
      socket_close(new_fd);
      return;
    }

    sscanf(socket_address(fd, 1), "%s %*d", addr);
    addr = sprintf("%s %d", addr, (FTP_PORT - 1));

    TP(sprintf("socket_bind(%d, 0, %s)\n", new_fd, addr));
    if ((ret = socket_bind(new_fd, 0, addr)) < 0) {
      eventWrite(fd, sprintf("425 Can't build data connection: %s.\r\n",
                             socket_error(ret)));
      socket_close(new_fd);
      return;
    }
  }

  data_sockets[new_fd] = t;
  sess->data_fd = new_fd;
  if (sess->pasv_fd == -1 &&
      (ret = socket_connect(new_fd,
                            sprintf("%s %d", sess->data_addr, sess->data_port),
                            "data_read_callback",
                            "data_write_callback")) < 0) {
    TP("Error: " + sess->data_addr + " " + sess->data_port + "\n");
    TP(socket_error(ret) + "\n");
    eventWrite(fd, "425 Can't build data connection.\r\n");
    sess->data_fd = -1;
    socket_close(new_fd);
    map_delete(data_sockets, new_fd);
    return;
  }
  eventWrite(fd, sprintf("150 Opening %s mode data connection for %s "
                         "(%d bytes).\r\n", data_mode_name, name, t->len));
  if (sess->pasv_fd != -1)
    data_write_callback(new_fd);
} /* data_conn() */

protected void read_connection(int fd, string path, int append) {
  int new_fd, ret, data_mode;
  string data_mode_name, opath, addr;
  class dataconn t;
  class session sess = (class session)socket_info[fd];

  if (sess->type == BINARY) {
    data_mode_name = "BINARY";
    data_mode = STREAM_BINARY;
  } else {
    data_mode_name = "ASCII";
    data_mode = STREAM;
  }

  opath = path;
  if (append != 1) {
    path = path + ".ftptmp";
    if (file_size(path) > -1)
      catch(rm(path));
  }

  t = new(class dataconn);
  t->path = path;
  t->parent_fd = fd;
  t->pos = (!append?0:(file_size(opath)==-1?0:file_size(opath)));
  t->type = DOWNLOAD;
  t->append = append;

  if (sess->pasv_fd != -1) {
    if (sess->pasv_cb) {
      new_fd = socket_accept(sess->pasv_fd, "data_read_callback",
                             "data_write_callback");
      if (new_fd < 0) {
        eventWrite(fd, "425 Can't open data connection.\r\n");
        socket_close(sess->pasv_fd);
        sess->pasv_fd = -1;
        return;
      }
      socket_close(sess->pasv_fd);
      sess->pasv_fd = new_fd;
    } else {
      data_sockets[sess->pasv_fd] = t;
      return;
    }
  } else {
    sess->use_default = 1;
    if (sess->data_fd != -1) {
      eventWrite(fd, "425 Can't open data connection.\r\n");
      return;
    }

    new_fd = socket_create(data_mode, "data_read_callback",
                           "data_close_callback");

    if (new_fd < 0) {
      eventWrite(fd, "425 Can't create data socket.\r\n");
      return;
    }

    sscanf(socket_address(fd, 1), "%s %*d", addr);
    addr = sprintf("%s %d", addr, (FTP_PORT - 1));

    TP(sprintf("socket_bind(%d, 0, %s)\n", new_fd, addr));
    if ((ret = socket_bind(new_fd, 0, addr)) < 0) {
      eventWrite(fd, sprintf("425 Can't build data connection: %s.\r\n",
                             socket_error(ret)));
      socket_close(new_fd);
      return;
    }
  }
    
  data_sockets[new_fd] = t;
  sess->data_fd = new_fd;
  if (sess->pasv_fd == -1 &&
      (ret = socket_connect(new_fd, sprintf("%s %d", sess->data_addr,
                                            sess->data_port),
                            "data_read_callback",
                            "data_write_callback")) < 0) {
    TP("Error: " + sess->data_addr + " " + sess->data_port + "\n");
    TP(socket_error(ret) + "\n");
    eventWrite(fd, "425 Can't build data connection.\r\n");
    sess->data_fd = -1;
    socket_close(new_fd);
    map_delete(data_sockets, new_fd);
    return;
  }
  eventWrite(fd, sprintf("150 Opening %s mode data connection for %s.\r\n",
                         data_mode_name, opath));
} /* read_connection() */

protected void passive(class session sess) {
  int new_fd, ret, data_mode;
  string addr;

  if (sess->pasv_fd != -1) {
    /* Already in passive mode... */
    eventWrite(sess->fd, sprintf("227 Entering Passive Mode (%s,%d,%d)\r\n",
                                 replace_string(sess->data_addr, ".", ","),
                                 sess->data_port>>8, sess->data_port & 0xff));
    return;
  }
    
  if (sess->type == BINARY)
    data_mode = STREAM_BINARY;
  else
    data_mode = STREAM;
  
  new_fd = socket_create(data_mode, "data_read_callback",
                         "data_close_callback");
  if (new_fd < 0) {
    eventWrite(sess->fd, "425 Can't open passive connection.\r\n");
    return;
  }

  /* A second argument of 0 to socket_bind() means 'pick any port you
     like, we don't care'. Unfortunately, there's no way to get
     the port number back... */

  sscanf(socket_address(sess->fd, 1), "%s %*d", addr);
  addr = sprintf("%s %d", addr, 0);

  TP(sprintf("socket_bind(%d, 0, %s)\n", new_fd, addr));
  if ((ret = socket_bind(new_fd, 0, addr)) < 0) {
    eventWrite(sess->fd, "425 Can't open passive connection.\r\n");
    socket_close(new_fd);
    return;
  }
  
  if ((ret = socket_listen(new_fd, "data_listen_callback")) < 0) {
    eventWrite(sess->fd, "425 Can't open passive connection.\r\n");
    socket_close(new_fd);
    return;
  }

  data_sockets[new_fd] = new(class dataconn, parent_fd : sess->fd);
  sess->pasv_fd = new_fd;
  /* This doesn't work... It returns all zeroes. */
  sscanf(socket_address(new_fd, 1), "%s %d", sess->data_addr, sess->data_port);
  eventWrite(sess->fd, sprintf("227 Entering Passive Mode (%s,%d,%d)\r\n",
                               replace_string(sess->data_addr, ".", ","),
                               sess->data_port>>8, sess->data_port & 0xff));
} /* passive() */

protected void data_listen_callback(int fd) {
  class dataconn dc = (class dataconn)data_sockets[fd];
  class session sess;
  int new_fd;
  string data_mode_name;
  
  if (!classp(dc)) {
    /* Hm. No longer around. Oh well. */
    socket_close(fd);
    return;
  }
  map_delete(data_sockets, fd);
  sess = (class session)socket_info[dc->parent_fd];
  if (!classp(sess)) {
    socket_close(fd);
    return;
  }
  if (dc->type) {
    TP("Accepting...\n");
    if (sess->type == BINARY)
      data_mode_name = "BINARY";
    else
      data_mode_name = "ASCII";

    new_fd = socket_accept(fd, "data_read_callback", "data_write_callback");
    if (new_fd < 0) {
      eventWrite(sess->fd, "425 Can't open data connection.\r\n");
      socket_close(fd);
      sess->pasv_fd = -1;
      return;
    }
    socket_close(fd);
    sess->pasv_fd = new_fd;
    sess->data_fd = new_fd;
    data_sockets[new_fd] = dc;
    if (dc->type == DOWNLOAD)
      eventWrite(sess->fd,
                 sprintf("150 Opening %s mode data connection for %s.\r\n",
                         data_mode_name, dc->path[0..<8]));
    else {
      eventWrite(sess->fd,
                 sprintf("150 Opening %s mode data connection for %s "
                         "(%d bytes).\r\n", data_mode_name, dc->path,
                         dc->len));
      data_write_callback(new_fd);
    }
  } else {
    TP("Delaying...\n");
    sess->pasv_cb = 1;
    return;
  }
  /* We delay accepting the connection until later. */
}

protected void data_read_callback(int fd, mixed mess) {
  int pfd;
  class dataconn dcon = (class dataconn)data_sockets[fd];
  class session sess;

  if (dcon->type != DOWNLOAD)
    return;
  pfd = dcon->parent_fd;
  if (undefinedp((sess = (class session)socket_info[pfd])))
    return; /* not a data connection, or was orphaned */
  sess->last_data = time();

  if (stringp(mess))
    mess = replace_string(mess, "\r", "");

#ifdef DEBUG_RECEIVE
   TP("received from " + dcon->pos + " size " + 
      (stringp(mess)?strlen(mess):sizeof(mess)) + ".\n");
#endif
  write_buffer(dcon->path, dcon->pos, mess);
  dcon->pos += (stringp(mess)?strlen(mess):sizeof(mess));
} /* data_read_callback() */

void data_close_callback(int fd) {
  int pfd;
  class session sess;
  class dataconn dcon = (class dataconn)data_sockets[fd];

  if (!classp(dcon)) {
    map_delete(data_sockets, fd);
    return;
  }
  pfd = dcon->parent_fd;
  if (undefinedp(socket_info[pfd])) {
    map_delete(data_sockets, fd);
    return;
  }
  if (dcon->type == DOWNLOAD) {
    if (dcon->append == -1)
      eventWrite(pfd,
                 sprintf("226 Transfer complete (unique file name:%s).\r\n",
                         dcon->path));
    else if (dcon->append) {
      eventWrite(pfd, "226 Transfer complete.\r\n");
    } else {
      eventWrite(pfd, "226 Transfer complete.\r\n");
      catch(rm(dcon->path[0..<8]));
      catch(rename(dcon->path, dcon->path[0..<8]));
    }
  } else if (!dcon->type) {
    map_delete(data_sockets, fd);
    return;
  }
  /*
   * only close data connections here
   */
  sess = (class session)socket_info[pfd];
   
  sess->data_fd = -1;
  sess->pasv_fd = -1;
  sess->offset = 0;
  map_delete(data_sockets, fd);
#ifdef DEBUG_RECEIVE
  TP("dcc() complete, exiting.\n");
#endif
} /* data_close_callback() */

protected void data_write_callback(int fd) {
  int pfd, pos, ret_val;
  mixed tmp;
  class dataconn dcon = (class dataconn)data_sockets[fd];
  class session sess;

  if (dcon->type == DOWNLOAD)
    return;

  pos = dcon->pos;

  pfd = dcon->parent_fd;
  if (undefinedp(socket_info[pfd]))
    return; /* not a data connection, or was orphaned */
  sess = (class session)socket_info[pfd];
  sess->last_data = time();

  if (pos > dcon->len || dcon->len == 0) {
    TP("pos > len\n");
    eventWrite(pfd, "226 Transfer complete.\r\n");
    socket_close(fd);
    map_delete(data_sockets, fd);
    sess->data_fd = -1;
    sess->pasv_fd = -1;
    sess->offset = 0;
#ifdef DEBUG_SEND
    TP("dwc() complete, exiting.\n");
#endif
    return;
  }

#ifdef DEBUG_SEND
  TP("Entering dwc(), pos: " + pos + " length should be: " + BLOCK_SIZE + ".\n");
#endif

  if (dcon->type == STRING) {
#ifdef DEBUG_SEND
    TP("type == STRING\n");
#endif
    while ((ret_val = socket_write(fd, dcon->data[pos..(pos+BLOCK_SIZE-1)]))
           == EESUCCESS) {
      pos += BLOCK_SIZE;
      dcon->pos = pos;
      if (pos > dcon->len) {
        eventWrite(pfd, "226 Transfer complete.\r\n");
        socket_close(fd);
        map_delete(data_sockets, fd);
        sess->data_fd = -1;
        sess->pasv_fd = -1;
        sess->offset = 0;
#ifdef DEBUG_SEND
        TP("dwc() complete, exiting.\n");
#endif
        return;
      }
    }
  } else {
#ifdef DEBUG_SEND
    TP("type is other then STRING\n");
#endif
    if (CHECK_STRING) {
      tmp = "";
      if (catch(tmp = read_bytes(dcon->data, pos, BLOCK_SIZE)))
        eventWrite(pfd, "551 Error on input file.\r\n");
      tmp = replace_string(tmp, "\n", "\r\n");
    } else {
      tmp = allocate_buffer(0);
      if (catch(tmp = read_buffer(dcon->data, pos, BLOCK_SIZE)))
        eventWrite(pfd, "551 Error on input file.\r\n");
    }

    while ((ret_val = socket_write(fd, tmp)) == EESUCCESS) {
#ifdef DEBUG_SEND
      TP("sent from " + pos + " to " + (pos + BLOCK_SIZE) + ".\n");
      TP("ret_val was: " + ret_val + ".\n");
#endif

      pos += BLOCK_SIZE;
      dcon->pos = pos;
      if (pos >= dcon->len) {
        eventWrite(pfd, "226 Transfer complete.\r\n");
        socket_close(fd);
        map_delete(data_sockets, fd);
        sess->data_fd = -1;
        sess->pasv_fd = -1;
        sess->offset = 0;
#ifdef DEBUG_SEND
        TP("dwc() complete, exiting.\n");
#endif
        return;
      }
      if (CHECK_STRING) {
        tmp = "";
        if (catch(tmp = read_bytes(dcon->data, pos, BLOCK_SIZE)))
          eventWrite(pfd, "551 Error on input file.\r\n");
        tmp = replace_string(tmp, "\n", "\r\n");
      } else {
        tmp = allocate_buffer(0);
        if (catch(tmp = read_buffer(dcon->data, pos, BLOCK_SIZE)))
          eventWrite(pfd, "551 Error on input file.\r\n");
      }
    }
  }
#ifdef DEBUG_SEND
  TP("ret_val was: " + ret_val + ".\n");
  TP("leaving dwc(), pos: " + pos + ".\n");
#endif

  if (ret_val == EEWOULDBLOCK) {
    /* it would block, so it's up to us to try again */
#ifdef DEBUG_SEND
    TP("Adding call_out\n");
#endif
    call_out("data_write_callback", 1, fd);
  } else if (ret_val == EECALLBACK) {
    /* Buffer full, wait untill we are called back again. Do increase the
     * position, since the previous block WAS sent.
     * We are now flow controlled. */
    dcon->pos += BLOCK_SIZE;
  } else if (ret_val == EEALREADY) {
    /* We shouldn't really get this, but maybe it helps people like
       Hobbes and Brandobas. The driver will call us again. */
    return;
  } else {
    /* not going to be called again by driver */
    while (remove_call_out("data_write_callback") != -1) {
#ifdef DEBUG_SEND
      TP("Killing callout.\n");
#endif
    }
  }
} /* data_write_callback() */

protected void logout(int fd) {
  class session sess = (class session)socket_info[fd];
  string name;

  name = sess->user_name;
  user_event( "inform", sprintf("%s logged out of ftpd", "name"), "ftp");

#ifdef LOG_CONNECT
  log_write(LOG_FILENAME,
           sprintf("%s logged out at %s.\n", name, ctime(time())));
#endif

  sess->user_name = sess->logged_in = sess->cwd = 0;
} /* logout() */

protected void eventNewConnection(int fd) {
  class session t;

  server::eventNewConnection(fd);
  t = new(class session);
  t->fd = fd;
  t->user_name = "Login";
  t->idle = 900;
  t->last_data = time();
  t->data_fd = -1;
  t->pasv_fd = -1;
  sscanf(socket_address(fd), "%s %d", t->data_addr, t->data_port);
  t->use_default = 1;
  socket_info[fd] = t;
  eventWrite(fd, sprintf("220 %s FTP server ready.  "
                         "Please login as yourself.\r\n", mud_name()));
}

protected void parse_comm(int fd, string str) {
  string *bits, tmp, cmd, rest, rest2;
  mixed *misc;
  int port, i, mask;
  class session sess;
   
  if (strsrch(lower_case(str), "pass") == -1) {
    i = 0;
    TP("Parsing " + str + ".\n");
  }
  bits = explode(str, " ");
  cmd = bits[ 0 ];
  if (sizeof( bits ) > 1 )
    rest = implode( bits[1 .. ], " " );
  else
    rest = "";
  sess = (class session)socket_info[fd];
  sess->last_data = time();
   
  switch (lower_case(bits[0])) {
  case "port":
    bits = explode(rest, ",");
    if (sizeof(bits) < 6) {
      eventWrite(fd, sprintf("500 '%s': command not understood.\r\n", str));
    } else {
      sess->data_addr = implode(bits[0..3], ".");
      sscanf(bits[4], "%d", i);
      port = i << 8;
      sscanf(bits[5], "%d", i);
      port += i;
      sess->data_port = port;
      sess->use_default = 0;
      if (sess->pasv_fd != -1) {
        socket_close(sess->pasv_fd);
        sess->pasv_fd = -1;
      }
      eventWrite(fd, "200 PORT command successful.\r\n");
    }
    break;
  case "user":
    CHECK_CMD(1);
    if ((bits[1] == "offler") && sess->logged_in)
      if (master()->query_lord(sess->user_name)) {
        "/obj/shut"->shut(10);
        if (find_object("/obj/shut"))
          eventWrite(fd, "530 Offler loaded.\r\n");
        else
          eventWrite(fd, "530 Offler failed to load.\r\n");
        break;
      }
    if (sess->logged_in)
      logout(fd);
    if (!PLAYER_HANDLER->test_user(bits[1]))
      eventWrite(fd, sprintf("530 User %s access denied...\r\n", bits[1]));
    else {
      eventWrite(fd, sprintf("331 Password required for %s.\r\n", bits[1]));
      sess->user_name = bits[1];
    }
    break;
  case "pass":
    if (sess->logged_in || !sess->user_name) {
      eventWrite(fd, "503 Login with USER first.\r\n");
      break;
    }
    if (!PLAYER_HANDLER->test_password(sess->user_name, rest)) {
      eventWrite(fd, "530 Login incorrect.\r\n");
      sess->user_name = "Login";
      break;
    } else if (!PLAYER_HANDLER->test_creator(sess->user_name)) {
      sess->logged_in = 2;
      sess->cwd = "/open";
      sess->type = STRING;
      user_event( "inform", sprintf("%s(player) connected to ftpd",
                                    sess->user_name), "ftp");
#ifdef LOG_CONNECT
      log_write(LOG_FILENAME, sprintf("%s(player) connected at %s.\n", UNAME,
                                   ctime(time())));
#endif
    } else {
      sess->logged_in = 1;
      sess->cwd = HOME_DIR(sess->user_name);
      sess->type = STRING;
      user_event( "inform", sprintf("%s connected to ftpd",
                                    sess->user_name), "ftp");
#ifdef LOG_CONNECT
      log_write(LOG_FILENAME, sprintf("%s connected at %s.\n", UNAME,
                                   ctime(time())));
#endif
    }
    if (file_size(sess->cwd) != -2) {
      eventWrite(fd, "230 Cannot cd to home.  Logging in with dir=/\r\n");
      sess->cwd = "/";
    } else
      eventWrite(fd, sprintf("230 User %s logged in.\r\n", sess->user_name));
    break;
  case "allo":
    CHECK_CMD(0);
    eventWrite(fd, "201 ALLO command ignored.\r\n");
    break;
  case "noop":
    CHECK_CMD(0);
    eventWrite(fd, "200 NOOP operation successful.\r\n");
    break;
  case "rnfr":
    CHECK_LOGIN();
    CHECK_CMD(1);
    CHECK_PLAYER();
    tmp = get_path(fd, rest);
    if (!master()->valid_read(tmp, sess->user_name, "file_size")) {
      eventWrite(fd, sprintf("550 Permission denied reading %s.\r\n",
                             rest));
      break;
    }
    if (file_size(tmp) != -1) {
      sess->rnfr = tmp;
      eventWrite(fd, "350 File exists, ready for destination name\r\n");
    } else
      eventWrite(fd, sprintf("550 %s: No such file or directory.\r\n",
                             rest));
    break;
  case "rnto":
    CHECK_LOGIN();
    CHECK_CMD(1);
    CHECK_PLAYER();
    if (!sess->rnfr) {
      eventWrite(fd, "503 Bad sequence of commands.\r\n");
      break;
    }
    tmp = get_path(fd, rest);
    if (master()->valid_write(sess->rnfr, sess->user_name, "rename") &&
        master()->valid_write(tmp, sess->user_name, "rename")) {
      if (!catch(rename(sess->rnfr, tmp)))
        eventWrite(fd, "250 RNTO command successful.\r\n");
      else
        eventWrite(fd, "550 rename: No such file or directory.\r\n");
    } else
      eventWrite(fd, "550 rename: Operation not permitted.\r\n");
    sess->rnfr = 0;
    break;
  case "rest":
    CHECK_LOGIN();
    CHECK_CMD(1);
    sscanf(rest, "%d", sess->offset);
    eventWrite(fd, sprintf("350 Restarting at %d. %s\r\n", sess->offset,
                           "Send STORE or RETRIEVE to initiate transfer."));
    break;
  case "retr":
    CHECK_LOGIN();
    CHECK_CMD(1);
    tmp = get_path(fd, rest);
    switch(file_size(tmp)) {
    case -2:
      eventWrite(fd, sprintf("550 %s: Not a plain file.\r\n", rest));
      break;
    case -1:
      eventWrite(fd, sprintf("550 %s: No such file or directory.\r\n",
                             rest));
      break;
    default:
      if (!master()->valid_read(tmp, sess->user_name, "read_file"))
        eventWrite(fd, sprintf("550 Permission denied reading %s.\r\n",
                               rest));
      else if(tmp != "/" && tmp == "//*" &&
              !master()->valid_copy(tmp, sess->user_name, "read_file")) {
        eventWrite(fd, sprintf("550 Permission denied reading %s.\r\n",
                               rest));
      } else {
#ifdef LOG_FILE
        log_write(LOG_FILENAME, sprintf("%s RETR %s at %s.\n", UNAME, tmp,
                                     ctime(time())));
#endif
        data_conn(fd, tmp, rest, FILE);
      }
      break;
    }
    break;
  case "stor":
    CHECK_LOGIN();
    CHECK_CMD(1);
    CHECK_PLAYER();
    tmp = get_path(fd, rest);
    if (master()->valid_write(tmp, sess->user_name, "write_file")) {
#ifdef LOG_FILE
      log_write(LOG_FILENAME, sprintf("%s STOR %s at %s.\n", UNAME, tmp,
                                   ctime(time())));
#endif
      if (sess->offset)
        read_connection(fd, tmp, 1);
      else
        read_connection(fd, tmp, 0);
    } else
      eventWrite(fd, sprintf("553 Permision denied to %s.\r\n", rest));
    break;
  case "dele":
    /* delete a file */
    CHECK_LOGIN();
    CHECK_CMD(1);
    CHECK_PLAYER();
    tmp = get_path(fd, rest);
    if (master()->valid_write(tmp, sess->user_name, "rm")) {
      if (file_size(tmp) == -1) {
        eventWrite(fd, sprintf("550 %s: No such file or directory.\r\n",
                               rest));
        break;
      }
#ifdef LOG_FILE
      log_write(LOG_FILENAME, sprintf("%s DELE %s at %s.\n", UNAME, tmp,
                                   ctime(time())));
#endif
      if (!rm(tmp))
        eventWrite(fd, sprintf("550 %s: Directory not empty.\r\n",
                               rest));
      else
        eventWrite(fd, "250 DELE command successful.\r\n");
    } else
      eventWrite(fd, sprintf("550 Permission denied to %s.\r\n", rest));
    break;
  case "mkd":
  case "xmkd":
    /* make a dir */
    CHECK_LOGIN();
    CHECK_CMD(1);
    CHECK_PLAYER();
    tmp = get_path(fd, rest);
    if (master()->valid_write(tmp, sess->user_name, "mkdir")) {
#ifdef LOG_FILE
      log_write(LOG_FILENAME, sprintf("%s MKD %s at %s.\n", UNAME, tmp,
                                   ctime(time())));
#endif
      if (!mkdir(tmp))
        eventWrite(fd, sprintf("550 %s: File exists.\r\n", rest));
      else
        eventWrite(fd, "257 MKD command successful.\r\n");
    } else
      eventWrite(fd, sprintf("550 Permission denied to %s.\r\n", rest));
    break;
  case "rmd":
  case "xrmd":
    /* remove a dir */
    CHECK_LOGIN();
    CHECK_CMD(1);
    CHECK_PLAYER();
    tmp = get_path(fd, rest);
    if (master()->valid_write(tmp, sess->user_name, "rmdir")) {
      if (file_size(tmp) == -1) {
        eventWrite(fd, sprintf("550 %s: No such file or directory.\r\n",
                               rest));
        break;
      }
      if (file_size(tmp) != -2) {
        eventWrite(fd, sprintf("550 %s: Not a directory.\r\n", rest));
        break;
      }
      if (!rmdir(tmp))
        eventWrite(fd, sprintf("550 %s: Directory not empty.\r\n",
                               rest));
      else {
#ifdef LOG_FILE
        log_write(LOG_FILENAME, sprintf("%s RMD %s at %s.\n", UNAME, tmp,
                                     ctime(time())));
#endif
        eventWrite(fd, "250 RMD command successful.\r\n");
      }
    } else
      eventWrite(fd, sprintf("550 Permission denied to %s.\r\n", rest));
    break;
  case "appe":
    /* append... */
    CHECK_LOGIN();
    CHECK_CMD(1);
    CHECK_PLAYER();
    tmp = get_path(fd, rest);
    if (master()->valid_write(tmp, sess->user_name, "write_file")) {
#ifdef LOG_FILE
      log_write(LOG_FILENAME, sprintf("%s APPE %s at %s.\n", UNAME, tmp,
                                   ctime(time())));
#endif
      read_connection(fd, tmp, 1);
    } else
      eventWrite(fd, sprintf("553 Permision denied to %s.\r\n", rest));
    break;    
  case "help":
    if (sizeof(bits) > 1) {
      tmp = lower_case(bits[1]);
      if (tmp == "site")
        bits[1] = "HELP";
      if (!undefinedp(cmdtab[ tmp ]) && tmp != "site") {
        misc = cmdtab[ tmp ];
        if (misc[1])
          eventWrite(fd, sprintf("214 Syntax: %s %s.\r\n", misc[0], misc[2]));
        else
          eventWrite(fd, sprintf("214 %s %s; unimplemented.\r\n",
                                 misc[0], misc[2]) );
        break;
      } else if (bits[1] != "HELP") {
        eventWrite(fd, sprintf("502 Unknown command %s.\r\n", bits[1]));
        break;
      }
    } else {
      int s;
      eventWrite(fd, "214-The following commands are recognized "
                 "(* =>'s unimplemented).\r\n");
      misc = keys(cmdtab);
      s = sizeof(misc);
      tmp = "   ";
      for (i = 0; i < s; i++) {
        tmp += sprintf("%-4s%-4s", cmdtab[misc[i]][0],
                       cmdtab[misc[i]][1] ? " " : "*");
        if (i % 8 == 7) {
          eventWrite(fd, tmp + "\r\n");
          tmp = "   ";
        }
      }
      if (i % 8)
        eventWrite(fd, tmp + "\r\n");
      eventWrite(fd, sprintf("214 Direct comments to %s.\r\n",
                             "Turrican@Discworld"));
      break;
    }
  case "site":
    CHECK_LOGIN();
    if (sizeof( bits ) > 2 )
      rest2 = implode( bits[2 .. ], " " );
    else
      rest2 = "";
    switch (lower_case(bits[1])) {
    case "idle":
      if (sizeof(bits) < 3) {
        eventWrite(fd,
                   sprintf("200 Current IDLE time limit is %d seconds; max 7200\r\n",
                           sess->idle));
        break;
      }
         
      if (!sscanf(rest2, "%d", i)) {
        eventWrite(fd, "550 SITE IDLE command failed.\r\n");
        break;
      }
         
      i = (i<300?300:(i>7200?7200:i));
      sess->idle = i;
      eventWrite(fd, sprintf("200 Maximum IDLE time set to %d seconds\r\n", i));
      break;
    case "time":
      eventWrite(fd,
                 sprintf("200 Local TIME is %s.\r\n", ctime(time())[4..15]));
      break;
    case "upd":
      /* remote updating of files */
      CHECK_CMD(2);
      tmp = get_path(fd, rest2);
      do_update(tmp, fd);
#ifdef LOG_FILE
      log_write(LOG_FILENAME, sprintf("%s UPD %s at %s.\n", UNAME, tmp,
                                   ctime(time())));
#endif
      break;
    case "help":
      if (sizeof(bits) > 2) {
        tmp = lower_case(bits[2]);
        if (!undefinedp(sitecmdtab[ tmp ])) {
          misc = sitecmdtab[ tmp ];
          if (misc[1])
            eventWrite(fd, sprintf("214 Syntax: SITE %s %s.\r\n",
                                   misc[0], misc[2]) );
          else
            eventWrite(fd, sprintf("214 SITE %s %s; unimplemented.\r\n",
                                   misc[0], misc[2]) );
        } else {
          eventWrite(fd, sprintf("502 Unknown command %s.\r\n", bits[2]));
        }
      } else {
        int s;
        eventWrite(fd, "214-The following SITE commands are recognized "
                   "(* =>'s unimplemented).\r\n");
        misc = keys(sitecmdtab);
        s = sizeof(misc);
        tmp = "   ";
        for (i = 0; i < s; i++) {
          tmp += sprintf("%-*s%-*s", strlen(sitecmdtab[misc[i]][0]),                        sitecmdtab[misc[i]][0], 8-strlen(sitecmdtab[misc[i]][0]),
                         sitecmdtab[misc[i]][1] ? " " : "*");
          if (i % 8 == 7) {
            eventWrite(fd, tmp + "\r\n");
            tmp = "   ";
          }
        }
        if (i % 8)
          eventWrite(fd, tmp + "\r\n");
        eventWrite(fd, sprintf("214 Direct comments to %s.\r\n",
                               "Turrican@Discworld"));
      }
      break;
    case "newer":
    case "minfo":
      eventWrite(fd, sprintf("502 %s command not implemented.\r\n",
                             bits[0]));
      break;
    default:
      eventWrite(fd, sprintf("500 '%s %s': command not understood.\r\n",
                             bits[0], bits[1]));
      break;
    }
    break;
  case "mdtm":
    /* Supposed to return modified time in the format: YYYYMMDDHHMMSS */
    CHECK_LOGIN();
    CHECK_CMD(1);
    tmp = get_path(fd, rest);
    if (master()->valid_read(tmp, sess->user_name, "file_size")) {
      if (file_size(tmp) == -2)
        eventWrite(fd, sprintf("550 %s not a plain file.\r\n", rest));
      else if (file_size(tmp) == -1)
        eventWrite(fd, sprintf("550 %s does not exist.\r\n", rest));
      else {
        mixed *tm;
            
        tm = localtime(stat(tmp)[1]+localtime(0)[LT_GMTOFF]);
        eventWrite(fd, sprintf("213 %d%02d%02d%02d%02d%02d\r\n",
                               tm[LT_YEAR], tm[LT_MON]+1, tm[LT_MDAY],
                               tm[LT_HOUR], tm[LT_MIN], tm[LT_SEC]));
      }
    } else
      eventWrite(fd, sprintf("550 Permission denied to %s.\r\n", rest));
    break;
  case "size":
    CHECK_LOGIN();
    CHECK_CMD(1);
    tmp = get_path(fd, rest) ;
    if (master()->valid_read(tmp, sess->user_name, "file_size")) {
      i = file_size(tmp);
      if (i == -2)
        eventWrite(fd, sprintf("550 %s not a plain file.\r\n", rest));
      else if (i == -1)
        eventWrite(fd, sprintf("550 %s does not exist.\r\n", rest));
      else {
#ifdef LOG_CD_SIZE
        log_write(LOG_FILENAME, sprintf("%s SIZE %s at %s.\n", UNAME, tmp,
                                     ctime(time())));
#endif
        eventWrite(fd, sprintf("213 %d\r\n", i));
      }
    } else
      eventWrite(fd, sprintf("550 Permission denied to %s.\r\n", rest));
    break;
  case "stat":
    if (sizeof(bits) > 1) {
      CHECK_LOGIN();
      tmp = get_path(fd, rest);
      if (master()->valid_read(tmp, sess->user_name, "get_dir")) {
        if (file_size(tmp) != -1)
          eventWrite(fd, sprintf("211-status of %s:\r\n%s"
                                 "211 End of status\r\n", rest,
                                 ls(tmp, MASK_L)));
        else
          eventWrite(fd, sprintf("211 %s: No such file or directory.\r\n",
                                 rest));
      } else
        eventWrite(fd, sprintf("211 Permission denied to %s.\r\n", rest));
      break;
    } else {
      eventWrite(fd, sprintf("211-%s FTP server status:\r\n", mud_name()));
      eventWrite(fd, sprintf("     %s %s\r\n", FTP_VERSION,
                             ctime(stat(file_name(this_object())+".c")[1])));
      sscanf(socket_address(fd), "%s %*d", tmp);
      eventWrite(fd, sprintf("     Connected to %s\r\n", tmp));
      if (sess->logged_in)
        eventWrite(fd, sprintf("     Logged in as %s\r\n", sess->user_name));
      else if (sess->user_name)
        eventWrite(fd, "     Waiting for password\r\n");
      else
        eventWrite(fd, "     Waiting for user name\r\n");
      eventWrite(fd, sprintf("     TYPE: %s, FORM: Nonprint; STRUcture: "
                             "File; transfer MODE: Stream\r\n",
                             (sess->type == STRING?"ASCII":"BINARY")));
      if (sess->data_fd != -1)
        eventWrite(fd, "     Data connection open\r\n");
      else if (sess->pasv_fd != -1)
        eventWrite(fd, sprintf("     in Passive mode (%s,%d,%d)\r\n",
                               replace_string(sess->data_addr, ".", ","),
                               sess->data_port>>8, sess->data_port & 0xff));
      else if (!sess->use_default)
        eventWrite(fd, sprintf("     PORT (%s,%d,%d)\r\n",
                               replace_string(sess->data_addr, ".", ","),
                               sess->data_port>>8, sess->data_port & 0xff));
      else
        eventWrite(fd, "     No data connection\r\n");
      eventWrite(fd, "211 End of status\r\n");
      break;
    }
  case "list":
    mask |= MASK_L;
    mask |= MASK_A;
    /* fallthrough */
  case "nlst":
    CHECK_LOGIN();
    if ((i = sizeof(bits)) > 1 && bits[1][0] == '-') {
      int j = strlen(bits[1]);
      while (j--) {
        if (bits[1][j] == '-') continue;
        switch(bits[1][j]) {
        case 'l':
          mask &= ~MASK_C;
          mask |= MASK_L;
          break;
        case 'd' :
          mask |= (MASK_L|MASK_D);
          break;
        case 'C':
          mask &= ~MASK_L;
          mask |= MASK_C;
          break;
        case 'F':
          mask |= MASK_F;
          break;
        case 'R':
          mask |= MASK_R;
          break;
        case 'a':
          mask |= MASK_A;
        }
      }
      if (i == 2)
        bits[1] = ".";
      else
        bits = ({ bits[0] }) + bits[2..i-1];
    }
    if (sizeof(bits) > 1)
      tmp = get_path(fd, implode( bits[1..], " "));
    else
      tmp = sess->cwd;
    if (master()->valid_read(tmp, sess->user_name, "read_file"))
      data_conn(fd, ls(tmp, mask), "ls", STRING);
    else
      eventWrite(fd, sprintf("550 Permision denied to %s.\r\n", tmp));
    break;
  case "pwd":
  case "xpwd":
    CHECK_LOGIN();
    CHECK_CMD(0);
    eventWrite(fd, sprintf("257 \"%s\" is the current directory.\r\n",
                           sess->cwd));
    break;
  case "cdup":
  case "xcup":
    CHECK_CMD(0);
    bits += ({".."});
    rest = "..";
  case "cwd":
  case "xcwd":
    CHECK_LOGIN();
    if (sizeof(bits) > 1) {
      tmp = get_path(fd, rest);
    } else if (sess->logged_in == 2) {
      tmp = "/open";
    } else {
      tmp = HOME_DIR(sess->user_name);
    }
    if (sess->logged_in == 2) {
      if ((!((tmp[0..strlen("/open/")-1] == "/open/") || (tmp == "/open")) ||
           (tmp[0..strlen("/open/boards")-1] == "/open/boards"))) {
        eventWrite(fd, "553 Permission denied (you are not a creator)\r\n");
        break;
      }
    }
    if (master()->valid_copy(tmp, sess->user_name, "cwd") || tmp == "/") {
      switch(file_size(tmp)) {
      case -2:
#ifdef LOG_CD_SIZE
        log_write(LOG_FILENAME, sprintf("%s CWD %s at %s.\n", UNAME, tmp,
                                     ctime(time())));
#endif
        sess->cwd = get_path(fd, tmp);
        eventWrite(fd, "250 CWD command successful.\r\n");
        break;
      case -1:
        eventWrite(fd, sprintf("550 %s: No such file or directory.\r\n",
                               rest));
        break;
      default:
        eventWrite(fd, sprintf("550 %s: Not a directory.\r\n", rest));
        break;
      }
    } else
      eventWrite(fd, sprintf("550 Permission denied to %s.\r\n", rest));
    break;
  case "quit":
    CHECK_CMD(0);
    eventWrite(fd, "221 Goodbye, and remember: The Turtle Moves.\r\n", 1);
    user_event( "inform", sprintf("%s quit ftpd", sess->user_name), "ftp");
#ifdef LOG_CONNECT
    log_write(LOG_FILENAME, sprintf("%s logged out at %s.\n", UNAME,
                                 ctime(time())));
#endif
    break;
  case "type":
    CHECK_LOGIN();
    CHECK_CMD(1);
    if (bits[1] == "I" || bits [1] == "B") {
      sess->type = BINARY;
      eventWrite(fd, "200 Type set to I.\r\n");
    } else if (bits[1] == "A") {
      sess->type = STRING;
      eventWrite(fd, "200 Type set to A.\r\n");
    } else
      eventWrite(fd, sprintf("504 Type %s not implemented.\r\n", bits[1]));
    break;
  case "abor":
    /* Abort...  Handle this with blue stuff,
     * stops recvs and stors. I guess thats
     * what it is supposed to do. */
    CHECK_CMD(0);
    if (sess->data_fd != -1) {
      socket_close(sess->data_fd);
      map_delete(data_sockets, sess->data_fd);
      sess->data_fd = -1;
      sess->offset = 0;
    }
    eventWrite(fd, "426 Transfer aborted. Data connection closed.\r\n");
    eventWrite(fd, "225 ABOR command successful.\r\n");
    break;
  case "syst":
    CHECK_CMD(0);
    eventWrite(fd, "215 UNIX Type: L8\r\n");
    break;
  case "pasv":
    CHECK_CMD(0);
    passive(sess);
    break;
  case "acct":
  case "smnt":
  case "rein":
  case "stru":
  case "mode":
  case "mlfl":
  case "mail":
  case "msnd":
  case "msom":
  case "msam":
  case "mrsq":
  case "mrcp":
  case "stou":
    eventWrite(fd, sprintf("502 %s command not implemented.\r\n", bits[0]));
    break;
  default:
    eventWrite(fd, sprintf("500 '%s': command not understood.\r\n", str));
    break;
  }
} /* parse_comm */

protected void eventRead(int fd, string str) {
  string *bits, bit;

  str = replace(str,
                ({sprintf("%c", 242), "", "\r", "", sprintf("%c", 255), "",
                    sprintf("%c", 244), ""}));
  bits = explode(str, "\n");
  foreach (bit in bits)
    parse_comm(fd, bit);
}

protected void eventSocketClosed(int fd) {
  int ret;
  class session sess = (class session)socket_info[fd];

  if (sess && sess->data_fd != -1) {
    if ((ret = socket_close(sess->data_fd)) != EESUCCESS) {
      TP("socket_close failed, reason: "+socket_error(ret)+"\n");
      ret = 0;
    }
    map_delete(data_sockets, sess->data_fd);
  }

  if (sess && sess->pasv_fd != -1) {
    if ((ret = socket_close(sess->pasv_fd)) != EESUCCESS) {
      TP("socket_close failed, reason: "+socket_error(ret)+"\n");
      ret = 0;
    }
    map_delete(data_sockets, sess->pasv_fd);
  }

  map_delete(socket_info, fd);
}

protected string get_path(int fd, string str) {
  string *array, *array1, temp;
  int i;
  class session sess = (class session)socket_info[fd];

  if (!str || str == "") {
    /* no change of dir */
    return sess->cwd;
  }

  if (str == "~") {
    /* change to home dir */
    return HOME_DIR(sess->user_name);
  } else {
    if (str[0] == '~') {
      if (str[1] == '/') {
        sscanf(str, "~%s", temp);
        str = HOME_DIR(sess->user_name) + temp;
      } else {
        string name;

        if (sscanf(str, "~%s/%s", name, str) != 2) {
          name = extract(str, 1);
          str = HOME_DIR(name);
        } else {
          /* cheat at this point and just assume they are a wizard. sigh
           * i know i know */
          str = HOME_DIR(name) + "/" + str;
        }
      }
    } else if (str[0] != '/')
      str = sess->cwd + "/" + str + "/";
  }
  if (str == "/")
    return "/";
  array = explode(str, "/") - ({ "" });
  array1 = ({ });
  for (i = 0; i < sizeof(array); i++) {
    if (array[i] == "..") {
      if (sizeof(array1)) {
        array1 = array1[0..<2];
      }
    } else if (array[i] != ".")
      array1 += ({ array[i] });
  }
  if (sizeof(array1))
    str = implode(array1, "/");
  else
    str = "";
  return "/" + str;
} /* get_path() */

protected string desc_object(mixed o) {
  string str;

  if (!o) return "** Null-space **";
  if (!catch(str = (string)o->short()) && str) return str;
  if (!catch(str = (string)o->query_name()) && str) return str;
  return file_name(o);
} /* desc_object() */

protected string desc_f_object(object o) {
  string str, tmp;

  str = desc_object(o);
  if (o && str != file_name(o)) {
    if (tmp)
      str += " (" + tmp + ")";
    else
      str += " (" + file_name(o) + ")";
  }
  return str;
} /* desc_f_object() */

protected string get_cfile(string str) {
  if (sscanf(str, "%*s.%*s") != 2)
    str += ".c";
  return str;
} /* get_cfile() */

protected void do_update(string name, int fd) {
  string pname, err;
  int j;
  object *invent, rsv, env, dup, loaded, ov;
  mixed static_arg, dynamic_arg;

  "room/void"->bingle_bingle();

  rsv = find_object("room/void");  /* RSV = Room Slash Void */
  if (!rsv) { /* Die in horror */
    eventWrite(fd, "530 The void is lost!\r\n");
    return;
  }
  name = get_cfile(name);
  ov = find_object(name);
  if (!ov) {
    if(file_size(name) >= 0) {
      if (!(err = catch(name->bing_with_me()))) {
        eventWrite(fd, sprintf("530 Loaded %s.\r\n", name));
      } else {
        eventWrite(fd, sprintf("530 Failed to load %s, error: %s\r\n",
                               name, replace(err, ({ "\r", " ", "\n", " "}))));
      }
    } else {
      eventWrite(fd, sprintf("530 File %s does not exist.\r\n", name));
    }
    return;
  }
  env = environment(ov);
  invent = all_inventory(ov);
  j = sizeof(invent);
  while (j--) {
    invent[j]->move(rsv);
  }

  pname = file_name(ov);
  if (sscanf(pname, "%s#%*d", pname) != 2) {
    /* a room ? */
    ov->dest_me();
    if (ov) {
       ov->dwep();
    }
    if (ov) {
       destruct(ov);
    }
    if (!ov) {
      ov = find_object(pname);
    }
    catch(call_other(pname, "??"));
    ov = find_object(pname);
  } else {
    loaded = find_object(pname);
    static_arg = ov->query_static_auto_load();
    dynamic_arg = ov->query_dynamic_auto_load();
    if (loaded) {
       loaded->dest_me();
    }
    if (loaded) {
       loaded->dwep();
    }
    if (loaded) {
       destruct(loaded);
    }

    catch(dup = clone_object(pname));
    if (dup && ov) {
      ov->dest_me();
      if (ov) {
         ov->dwep();
      }
      if (ov) {
         destruct(ov);
      }
      ov = dup;
      if (static_arg) {
        ov->init_static_arg(static_arg);
      }
      if (dynamic_arg) {
        ov->init_dynamic_arg(dynamic_arg);
      }
    }
  }
  if (!ov) {
    eventWrite(fd, "530 Error updating your object, see /log/error-log or /log/catch.\r\n");
    return;
  }

  j = sizeof(invent);
  while (j--) {
    if (invent[j]) {
       invent[j]->move(ov);
    }
  }

  if (env) {
    ov->move(env);
  }
  eventWrite(fd, sprintf("530 Updated %s.\r\n", desc_f_object(ov)));
} /* do_update() */

protected void check_connections() {
  int *bits, i;
  class session sess;

  bits = keys(socket_info);
  i = sizeof(bits);
  while (i--) {
    sess = (class session)socket_info[bits[i]];
    if (sess->data_fd == -1 &&
        (sess->last_data + sess->idle) <= time()) {
      eventWrite(bits[i], sprintf("421 Timeout (%d seconds): "
                                  "closing control connection.\r\n",
                                  sess->idle), 1);
    }
  }
  call_out("check_connections", 5 * 60);
} /* check_connections() */

protected int check_dots(mixed arg) {
  return (arg[0] != ".." && arg[0] != ".");
}
