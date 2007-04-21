#include <network.h>
#include <mail.h>
#include <pop3.h>
#include <mime.h>
#include <player_handler.h>

inherit SERVER;

private void parse_comm(class pop_session sess, string str);

private string *pop_states = ({
  "AUTHORIZATION",
  "AUTHORIZATION",
  "TRANSACTION",
  "UPDATE"
});
private mapping Sockets;
private nosave mapping cache;
private nosave int reads, hits;
private mapping last;

protected void create() {
  server::create();
  SetSocketType(STREAM);
  SetDestructOnClose(1);
  Sockets = ([]);
  call_out("setup", 2);
  call_out("clean_sockets", 180);
  unguarded((: restore_object, "/net/save/pop3" :));
  if(!last)
    last = ([ ]);
} /* create() */

/** @ignore yes */
protected void setup() {
  if (eventCreateSocket(PORT_POP3) < 0) {
    if (this_object()) {
      destruct(this_object());
    }
  }
} /* setup() */

/** @ignore yes */
protected void eventNewConnection(int fd) {
  class pop_session sess;
  
  server::eventNewConnection(fd);
  sess = new(class pop_session, fd : fd, state : POP_AUTH_USER, command : "",
             deleted : ({}), messages : ({}));
  Sockets[fd] = sess;
  eventWrite(fd, sprintf("+OK POP3 %s Discworld v%s server ready\r\n",
                         query_host_name(), POP3_VERSION));
} /* eventNewConnection() */

/** @ignore yes */
protected void eventRead(int fd, string str) {
  string *bits, bit;
  class pop_session sess = Sockets[fd];

  if (!sess) {
    return;
  }
  if (str) {
    sess->command += str;
    if (strsrch(str, '\n') == -1) {
      return;
    }
  }
  sess->command = replace_string(sess->command, "\r", "");
  bits = explode(sess->command, "\n");
  if (!sizeof(bits)) {
    eventWrite(fd, "-ERR Null command\r\n");
  } else {
    foreach (bit in bits) {
      parse_comm(sess, bit);
    }
  }
  sess->command = "";
} /* eventRead() */

/** @ignore yes */
protected void eventSocketClosed(int fd) {
  class pop_session sess = Sockets[fd];

  if (!sess) {
    return;
  }
  map_delete(Sockets, fd);
} /* eventSocketClosed() */

private void sign_off(class pop_session sess) {
  int *deleted;
  
  if (sizeof(sess->deleted)) {
    deleted = map(sess->deleted, (: $2->headers[$1-1]->number :), sess);
    FOLDER_H->delete_it(sess->user_name, "inbox", deleted);
  }
  eventWrite(sess->fd, "+OK Sayonara\r\n", 1);
  unguarded((: save_object, "/net/save/pop3" :));
} /* sign_off() */

private void load_message(class pop_session sess, int message) {
  string mess;
  int which;

  reads++;

  which = sess->headers[message]->number;

  if(!cache)
    cache = ([ ]);
  if(cache[which]) {
    hits++;
    sess->messages[message] = cache[which];
    return;
  }
  
  mess = FOLDER_H->load_message(sess->user_name, "inbox",
                                sess->headers[message]->number);
  mess = MIME->rewrite_header(mess);
  mess = replace_string(mess, "\n", "\r\n");

  if (mess[<1] != '\n') {
    mess += "\r\n";
  }
  sess->messages[message] = mess;
  sess->sizes[message] = strlen(mess);
  
  cache[which] = copy(mess);
} /* load_message() */

private void load_folder(class pop_session sess) {
  int *tmp, max, i;
  
  sess->headers = FOLDER_H->get_messages(sess->user_name, "inbox");
  tell_creator("ceres", "Headers: %d\n", sizeof(sess->headers));
  sess->messages = allocate(sizeof(sess->headers));
  sess->sizes = allocate(sizeof(sess->headers));
  sess->num_messages = sizeof(sess->headers);

  // the sizes will start out as an approximation. As we read actual messages
  // the real size will be put in place.

  tmp = sess->sizes;
  
  for(i=0; i<sizeof(sess->headers); i++)
    sess->sizes[i] = 1000;

  // If we'd already got some sizes then copy them into the new size array.
  if(tmp) {
    (sizeof(tmp) > sizeof(sess->headers)) ? max = sizeof(tmp) :
      max = sizeof(sess->headers);
    for(i=0; i<max; i++)
      sess->sizes[i] = tmp[i];
  }
} /* load_folder() */

private void parse_comm(class pop_session sess, string str) {
  string *bits, rest, cmd, message, header;
  int *sizes, lines, i, number, fd = sess->fd;
  
  if (strsrch(lower_case(str), "pass") == -1) {
    TP("Parsing " + str + ".\n");
  }
  bits = explode(str, " ");
  cmd = bits[0];
  if (sizeof(bits) > 1 ) {
    rest = implode(bits[1..], " ");
  } else {
    rest = "";
  }
  sess->time = time();
  switch(lower_case(cmd)) {
  case "dele":
    tell_creator("ceres", "Requesting deletion of %d\n", number);

    CHECK_STATE(POP_TRANSACTION);
    CHECK_CMD(1, "-ERR Missing message number argument\r\n");
    if ((sscanf(bits[1], "%d", number) != 1) || number < 1 ||
        number > sess->num_messages) {
      eventWrite(fd, "-ERR No such message\r\n");
    } else if (member_array(number, sess->deleted) != -1) {
      eventWrite(fd, sprintf("-ERR Message number %d already deleted\r\n",
                             number));
    } else {
      tell_creator("ceres", "Deleting %d\n", number);
      sess->deleted += ({ number });
      eventWrite(fd, "+OK Message deleted\r\n");
    }
    break;
  case "last":
    CHECK_STATE(POP_TRANSACTION);
    eventWrite(fd, sprintf("+OK %d\r\n", last[sess->user_name]));
    tell_creator("ceres", "Returning %d for last\n", last[sess->user_name]);
    break;
  case "list":
    CHECK_STATE(POP_TRANSACTION);
    if (sizeof(bits) > 1) {
      if ((sscanf(bits[1], "%d", number) != 1) || number < 1 ||
          number > sess->num_messages) {
        eventWrite(fd, "-ERR No such message\r\n");
      } else if (member_array(number, sess->deleted) != -1) {
        eventWrite(fd, sprintf("-ERR Message number %d already deleted\r\n",
                               number));
      } else {
        if (!sizeof(sess->headers)) {
          load_folder(sess);
        }
        eventWrite(fd, sprintf("+OK %d %d\r\n", number,
                               sess->sizes[number-1]));
      }
    } else {
      eventWrite(fd, "+OK Mailbox scan listing follows\r\n");
      if (!sizeof(sess->headers)) {
        load_folder(sess);
      }
      i = 0;
      if (!sizeof(sess->deleted)) {
        map(sess->sizes,
            function(int size, int ref idx, int fd) {
              reset_eval_cost();
              eventWrite(fd, sprintf("%d %d\r\n", ++idx, size));
            }, ref i, fd);
      } else {
        map(sess->sizes,
            function (int size, int ref idx, int fd, int *deleted) {
              reset_eval_cost();
              idx++;
              if (member_array(idx, deleted) == -1) {
                eventWrite(fd, sprintf("%d %d\r\n", idx, size));
              }
            }, ref i, fd, sess->deleted);
      }
      eventWrite(fd, ".\r\n");
    }
    break;
  case "noop":
    CHECK_STATE(POP_TRANSACTION);
    eventWrite(fd, "+OK No-op to you too!\r\n");
    break;
  case "pass":
    CHECK_STATE(POP_AUTH_PASS);
    CHECK_CMD(1, "-ERR Missing password argument\r\n");
    if (!PLAYER_HANDLER->test_password(sess->user_name, rest)) {
      sess->state = POP_AUTH_USER;
      eventWrite(fd, "-ERR Bad login\r\n");
    } else {
      sess->state = POP_TRANSACTION;
      load_folder(sess);
      //      sess->headers = FOLDER_H->get_messages(sess->user_name, "inbox");
      //      sess->num_messages = sizeof(sess->headers);
      eventWrite(fd, sprintf("+OK Mailbox open, %d messages\r\n",
                             sess->num_messages));
    }
    break;
  case "quit":
    switch (sess->state) {
    case POP_AUTH_USER:
    case POP_AUTH_PASS:
      eventWrite(fd, "+OK Sayonara\r\n", 1);
      break;
    case POP_TRANSACTION:
      sess->state = POP_UPDATE;
      sign_off(sess);
      break;
    }
    break;
  case "retr":
    CHECK_STATE(POP_TRANSACTION);
    CHECK_CMD(1, "-ERR Missing message number argument\r\n");
    if ((sscanf(bits[1], "%d", number) != 1) || number < 1 ||
        number > sess->num_messages) {
      eventWrite(fd, "-ERR No such message\r\n");
    } else if (member_array(number, sess->deleted) != -1) {
      eventWrite(fd, sprintf("-ERR Message number %d already deleted\r\n",
                             number));
    } else {
      if (!sizeof(sess->headers)) {
        load_folder(sess);
      }
      last[sess->user_name] = number;
      if(!sess->messages[number-1])
        load_message(sess, number-1);
      
      message = replace_string(sess->messages[number-1], "\n.", "\n..");
      eventWrite(fd, sprintf("+OK %d octets\r\n", sess->sizes[number-1]));
      eventWrite(fd, message + ".\r\n");
    }
    break;
  case "rset":
    CHECK_STATE(POP_TRANSACTION);
    sess->deleted = ({});
    last[sess->user_name] = 0;
    eventWrite(fd, "+OK Reset state\r\n");
    break;
  case "stat":
    CHECK_STATE(POP_TRANSACTION);
    if (!sizeof(sess->headers)) {
      load_folder(sess);
    }
    if (!sizeof(sess->deleted)) {
      tell_creator("ceres", "headers: %d, sizes %d\n", sizeof(sess->headers),
                   sizeof(sess->sizes));
      eventWrite(fd, sprintf("+OK %d %d\r\n", sess->num_messages,
                             implode(sess->sizes, (: $1 + $2 :))));
    } else {
      i = 0;
      sizes = map(sess->sizes, function(int size, int ref idx, int *deleted) {
                                 reset_eval_cost();
                                 idx++;
                                 if (member_array(idx, deleted) != -1) {
                                   return 0;
                                 } else {
                                   return size;
                                 }
                               }, ref i, sess->deleted);
      eventWrite(fd, sprintf("+OK %d %d\r\n",
                             (sess->num_messages - sizeof(sess->deleted)),
                             implode(sizes,
                                     (: reset_eval_cost(), $1 + $2 :))));
    }
    break;
  case "top":
    CHECK_STATE(POP_TRANSACTION);
    CHECK_CMD(1, "-ERR Missing message number argument\r\n");
    if ((sscanf(bits[1], "%d", number) != 1) || number < 1 ||
        number > sess->num_messages) {
      eventWrite(fd, "-ERR No such message\r\n");
    } else if (member_array(number, sess->deleted) != -1) {
      eventWrite(fd, sprintf("-ERR Message number %d already deleted\r\n",
                             number));
    } else {
      if (!sizeof(sess->headers)) {
        load_folder(sess);
      }

      last[sess->user_name] = number;
      if(!sess->messages[number-1])
        load_message(sess, number-1);

      if (sizeof(bits) > 2 && (sscanf(bits[2], "%d", lines) == 1) &&
          lines < sess->sizes[number-1]) {
        i = strsrch(sess->messages[number-1], "\r\n\r\n");
        header = sess->messages[number-1][0..i+3];
        message = replace_string(sess->messages[number-1][i+4..], "\n.",
                                 "\n..");
        eventWrite(fd, sprintf("+OK %d octets\r\n", sess->sizes[number-1]));
        eventWrite(fd, header);
        if (lines) {
          bits = explode(message, "\r\n");
          eventWrite(fd, implode(bits[0..lines-1], "\r\n") + "\r\n.\r\n");
        } else {
          eventWrite(fd, ".\r\n");
        }
      } else {
        message = replace_string(sess->messages[number-1], "\n.", "\n..");
        eventWrite(fd, sprintf("+OK %d octets\r\n", sess->sizes[number-1]));
        eventWrite(fd, message + ".\r\n");
      }
    }   
    break;
  case "uidl":
    CHECK_STATE(POP_TRANSACTION);
    if (sizeof(bits) > 1) {
      if ((sscanf(bits[1], "%d", number) != 1) || number < 1 ||
          number > sess->num_messages) {
        eventWrite(fd, "-ERR No such message\r\n");
      } else if (member_array(number, sess->deleted) != -1) {
        eventWrite(fd, sprintf("-ERR Message number %d already deleted\r\n",
                               number));
      } else {
        if (!sizeof(sess->headers)) {
          load_folder(sess);
        }
        eventWrite(fd, sprintf("+OK %d %d\r\n", number,
                               sess->headers[number-1]->number));
      }
    } else {
      eventWrite(fd, "+OK Unique-ID listing follows\r\n");
      if (!sizeof(sess->headers)) {
        load_folder(sess);
      }
      i = 0;
      if (!sizeof(sess->deleted)) {
        map(sess->headers,
            function(class mail_header hdr, int ref idx, int fd) {
              reset_eval_cost();
              eventWrite(fd, sprintf("%d %d\r\n", ++idx, hdr->number));
            }, ref i, fd);
      } else {
        map(sess->headers,
            function (class mail_header hdr, int ref idx, int fd, int *deleted) {
              reset_eval_cost();
              idx++;
              if (member_array(idx, deleted) == -1) {
                eventWrite(fd, sprintf("%d %d\r\n", idx, hdr->number));
              }
            }, ref i, fd, sess->deleted);
      }
      eventWrite(fd, ".\r\n");
    }
    break;
  case "user":
    CHECK_STATE(POP_AUTH_USER);
    CHECK_CMD(1, "-ERR Missing username argument\r\n");
    sess->user_name = bits[1];
    sess->state = POP_AUTH_PASS;
    eventWrite(fd, "+OK User name accepted, password please\r\n");
    break;
  case "":
    eventWrite(fd, "-ERR Null command\r\n");
    break;
  default:
    eventWrite(fd, sprintf("-ERR Unknown command in %s state\r\n",
                           pop_states[sess->state]));
    break;
  }
} /* parse_comm() */

/** @ignore yes */
protected void close_connection(class pop_session sess) {
  if (!sess) {
    return;
  }
  eventWrite(sess->fd, "-ERR Autologout; idle for too long\r\n", 1);
} /* close_connection() */

/** @ignore yes */
protected void clean_sockets() {
  class pop_session sess;
       
  foreach (sess in values(Sockets)) {
    if (!sess->time) {
      sess->time = time();
      continue;
    }
    if (time() - sess->time > 1800)
      close_connection(sess);
  }
  call_out("clean_sockets", 180);
} /* clean_sockets() */

mixed *stats() {
  if(!cache)
    cache = ([ ]);
  return  ({
    ({ "reads", reads, }),
      ({ "cache hit percent", reads != 0 ? (hits * 100) / reads : 0, }),
        ({ "messages in cache", sizeof(keys(cache)), }), 
          });
}
