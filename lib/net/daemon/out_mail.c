/*
 * Intermud mail server, used by Huthar's mailer daemon
 * Original author: Huthar
 * Rewritten to conform to new socket efun specs,  Blackthorn (10/31/92)
 * Modified by Truilkan for use in Basis mudlib (11/01/92)
 * Modified by Pinkfish for use in the Discworld mudlib (5 jan 92)
 * (Stupid american date formats).
 */

#include "mail.h"
#include "nameserver.h"
#include "socket.h"
#include "inet.h"

#define SAVE "/net/save/"
 
#define log(x) log_file("MS", x)
#define MS_SAVE "mail-queue"
#define FLUSH_TIME 86400
#define AGE_TIME 604800
#define EOF "%EOF%"
#define EOT "%EOT%"
 
static mixed mqi;
static mixed outgoing;
 
mixed mail_queue;
int date_last_flushed;
 
void process_message(int fd);
void flush_mail_queue();
void age_queue();
 
void reset() {
  if ((time() - date_last_flushed) > FLUSH_TIME) {
    mqi = keys(mail_queue);
    flush_mail_queue();
    date_last_flushed = time();
  }
  age_queue();
} /* reset() */
 
void create() {
  seteuid((string)"/secure/master"->creator_file(file_name(this_object())));
  mail_queue = ([ ]);
  mqi = ({ });
  restore_object(SAVE + MS_SAVE);
  mqi = keys(mail_queue);
  flush_mail_queue();
} /* create() */

/*
 * Ahhh, so this send the intermud mail...
 */
void remote_mail(string from, string to, string subject, string cc,
                 string body, string *goto) {
  int i;
  string *tmp, name, mud;
 
  tmp = explode(body, "\n");
  while (tmp && (i = member_array(EOF, tmp)) != -1) {
    tmp[i] = EOF + ".";
  }
 
  if (!tmp) tmp = ({ });
  body = implode(tmp, "\n");

/* Ok, now make up a list of muds and who to send to at each one. */
  for (i=0;i<sizeof(goto);i++) {
    sscanf(goto[i], "%s@%s", name, mud);
    if (!mail_queue[mud]) {
      mail_queue[mud] = ({ });
    }
    mail_queue[mud] += ({
        ([ "recipient" : name, "to" : to, "cc" : cc,
         "from" : from, "subject" : subject, "date" : time(),
         "message" : body ]),
                     });
    mqi += ({ mud });
    if (member_array(mud, mqi) == -1)
      mqi += ({ mud });
  }
 
  save_object(SAVE + MS_SAVE);
} /* remote_mail() */
 
void dest_me() {
  destruct(this_object()); 
} /* dest_me() */
 
void set_mqi(mixed m) { mqi = m; }
string *query_mqi() { return mqi; }
 
mapping query_mail_queue() { return mail_queue; }
void set_mail_queue(mixed a) { mail_queue = a; }
 
void clear_mail_queue() {
  mail_queue = ([ ]);
  save_object(SAVE + "mail-queue");
} /* clear_mail_queue() */
 
void age_queue() {
  int i, j;
  string *key;
  mixed tmp;
 
  key = keys(mail_queue);
 
  for (i = 0; i < sizeof(key); i++) {
    tmp = mail_queue[key[i]];
    for (j = 0; j < sizeof(tmp); j++) {
      if (time() - tmp[j]["date"] > AGE_TIME) {
        log("Aging mail from: " +
            tmp[j]["from"] + ", dated: " + tmp[j]["date"] + "\n");
        tmp = delete(tmp, j, 1);
/* Thats probably what exclude_array does.
        exclude_array(tmp, j);
 */
      }
    }
  }
} /* age_queue() */
 
void flush_mail_queue() {
  string *muds, address, port;
  int id;
 
  if (!sizeof(mqi)) return;
 
  outgoing = mail_queue[mqi[0]];
/* ahhhh.  so this is what does the junk! */
  INET_D->open_to_service("mail", INETD_TYPE, mqi[0]);
} /* flush_mail_queue() */

/*
 * Send a bounce to all the messages in the outgoing queue...
 */
void bounce_message(string mess) {
  int i;

  for (i=0;i<sizeof(outgoing);i++) {
    MAILER->do_mail_message(outgoing[i]["from"], "Mail-Server",
                         "Delivery failed: "+mess, "",
                         "Message sent follows:\n>To: "+outgoing[i]["to"]+
                         "\n>Cc : "+ outgoing[i]["cc"]+"\n>Subject: "+
                         outgoing[i]["subject"]+"\n\n"+outgoing[i]["message"],
                         1);
  }
} /* bounce_message() */

void failed(string reason) {
  bounce_message(reason);
  map_delete(mail_queue, mqi[0]);
  mqi -= ({ mqi[0] });
  save_object(SAVE + MS_SAVE);
} /* failed() */

void connected(int id) {
  int i;
 
  previous_object()->write_fd(id, mud_name() + "\n");
  for (i = 0; i < sizeof(outgoing); i++) {
    previous_object()->write_fd(id, outgoing[i]["recipient"] + "\n");
    previous_object()->write_fd(id, replace(outgoing[i]["to"], ",", " ")+"\n");
    previous_object()->write_fd(id, replace(outgoing[i]["cc"], ",", " ")+"\n");
    previous_object()->write_fd(id, outgoing[i]["from"] + "\n");
    previous_object()->write_fd(id, outgoing[i]["subject"] + "\n");
    previous_object()->write_fd(id, outgoing[i]["date"] + "\n");
    previous_object()->write_fd(id, outgoing[i]["message"] + "\n");
    previous_object()->write_fd(id, EOF + "\n");
  }
  previous_object()->write_fd(id, EOT + "\n");
  previous_object()->close_fd(id);
  map_delete(mail_queue, mqi[0]);
  mqi -= ({ mqi[0] });
  save_object(SAVE + MS_SAVE);
  call_out("flush_mail_queue", 60);
} /* connected() */
