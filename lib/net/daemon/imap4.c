/**
 * An attempt at an IMAP4 server. I don't know if I'll actually get it working
 * but I thought it might be fun to try.
 *
 * @author Ceres
 * @date 26 August 2000
 */
#include <network.h>
#include <mail.h>
#include <imap4.h>
#include <mime.h>

inherit SERVER;

private void parse_comm(class imap_session sess, string str);

private string *imap_states = ({
  "CONNECTED",
  "AUTHENTICATED",
  "SELECTED"
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
  unguarded((: restore_object, "/net/save/imap4" :));
  if(!last)
    last = ([ ]);
} /* create() */

/** @ignore yes */
protected void setup() {
  if (eventCreateSocket(PORT_IMAP4) < 0) {
    if (this_object()) {
      destruct(this_object());
    }
  }
} /* setup() */

/** @ignore yes */
protected void eventNewConnection(int fd) {
  class imap_session sess;
  
  server::eventNewConnection(fd);
  sess = new(class imap_session, fd : fd, state : IMAP_CONNECTED, command : "",
             deleted : ({}), messages : ({}));
  Sockets[fd] = sess;
  eventWrite(fd, sprintf("* OK IMAP4 %s Discworld v%s server ready\r\n",
                         query_host_name(), IMAP4_VERSION));
} /* eventNewConnection() */

/** @ignore yes */
protected void eventRead(int fd, string str) {
  string *bits, bit;
  class imap_session sess = Sockets[fd];

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
}

/** @ignore yes */
protected void eventSocketClosed(int fd) {
  class imap_session sess = Sockets[fd];

  if (!sess) {
    return;
  }
  map_delete(Sockets, fd);
} /* eventSocketClosed() */

private void sign_off(class imap_session sess) {
  int *deleted;
  
  if (sizeof(sess->deleted)) {
    deleted = map(sess->deleted, (: $2->headers[$1-1]->number :), sess);
    FOLDER_H->delete_it(sess->user_name, "inbox", deleted);
  }
  unguarded((: save_object, "/net/save/imap4" :));
} /* sign_off() */

private void load_message(class imap_session sess, int message) {
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

private void load_folder(class imap_session sess) {
  int *tmp, max, i;
  
  sess->headers = FOLDER_H->get_messages(sess->user_name, sess->selected);
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

private void parse_comm(class imap_session sess, string str) {
  string *bits, rest, cmd, tmp;
  int fd = sess->fd, i, j, k;
  string id;
	
	TP("Parsing " + str + ".\n");

  bits = explode(str, " ");
	id = bits[0];
  cmd = bits[1];

  sess->time = time();
  switch(lower_case(cmd)) {
	case "capability":
		eventWrite(fd, "* CAPABILITY IMAP4 IMAP4rev1 NAMESPACE AUTH=LOGIN\r\n");
		eventWrite(fd, sprintf("%s OK CAPABILITY completed.\r\n",
													 id, sess->num_messages));
		TP("Sent capability\n");
		break;
	case "noop":
		eventWrite(fd, id + " OK NOOP completed\r\n");
		break;
		
  case "logout":
		sign_off(sess);
		eventWrite(fd, id + " OK LOGOUT completed\r\n", 1);
    break;


	case "authenticate":
    CHECK_STATE(IMAP_CONNECTED);
		eventWrite(fd, sprintf("%s NO - unsupported authentication mechanism.\r\n",
													 id, sess->num_messages));
		break;
  case "login":
    CHECK_STATE(IMAP_CONNECTED);

		if (sizeof(bits) > 4 ) {
			rest = implode(bits[3..], " ");
		} else if(sizeof(bits) > 3) {
			rest = bits[3];
		}
    CHECK_CMD(2, id + " BAD - missing arguments\r\n");
		bits[2] = replace(bits[2], "\"", "");
		rest = replace(rest, "\"", "");
    if (!LOGIN_OB->test_password(bits[2], rest)) {
      sess->state = IMAP_CONNECTED;
      eventWrite(fd, id + " NO - login failure: user name or password rejected\r\n");
    } else {
      sess->state = IMAP_AUTHENTICATED;
			sess->user_name = bits[2];
			TP(bits[2] + " logged in.\n");
      eventWrite(fd, sprintf("%s OK LOGIN completed\r\n",
                             id, sess->num_messages));
    }
		break;

	case "select":
		CHECK_STATE(IMAP_AUTHENTICATED);

		CHECK_CMD(1, id + " BAD - missing argument.\r\n");
		sess->selected = replace(bits[2], ({"\"", "", "/", ""}));
		sess->state = IMAP_SELECTED;
		
		eventWrite(fd, id + " OK - SELECT completed\r\n");
		break;
		
	case "examine":
	case "create":
	case "delete":
	case "rename":
	case "subscribe":
	case "unsubscribe":
	case "list":
		CHECK_STATE(IMAP_AUTHENTICATED);
		foreach(tmp in MAILER->query_folders(sess->user_name)) {
			eventWrite(fd, "* LIST (\\Noselect) \"/\" /" + tmp + "\r\n");
		}
		eventWrite(fd, id + " OK LIST completed\r\n");
		break;
	case "lsub":
		CHECK_STATE(IMAP_AUTHENTICATED);
		foreach(tmp in MAILER->query_folders(sess->user_name)) {
			eventWrite(fd, "* LSUB () \"/\" /" + tmp + "\r\n");
		}
		eventWrite(fd, id + " OK LSUB completed\r\n");
		break;
	case "status":
	case "append":

	case "uid":
		CHECK_STATE(IMAP_SELECTED);

		CHECK_CMD(1, id + " BAD - missing argument.\r\n");
		switch(lower_case(bits[2])) {
		case "fetch":
			TP("Doing fetch\n");
			load_folder(sess);
			if(sscanf(bits[3], "%d:%d", j, k) != 2) {
				sscanf(bits[3], "%d:*", j);
				k = sess->num_messages;
			}

			for(i=j; i<k; i++) {
				eventWrite(fd, " * " + i + " FETCH (FLAGS (\\Seen) UID " +
									 sess->headers[i]->number + "\r\n");
			}
			eventWrite(fd, id + " UID FETCH completed\r\n");
			TP("Ok, done\n");
			break;
		case "copy":
		case "store":
		}
		break;
		
  default:
    eventWrite(fd, sprintf("-ERR Unknown command in %s state\r\n",
                           imap_states[sess->state]));
    break;
  }
	TP("done parsing\n");
} /* parse_comm() */

/** @ignore yes */
protected void close_connection(class imap_session sess) {
  if (!sess) {
    return;
  }
  eventWrite(sess->fd, "-ERR Autologout; idle for too long\r\n", 1);
} /* close_connection() */

/** @ignore yes */
protected void clean_sockets() {
  class imap_session sess;
       
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
