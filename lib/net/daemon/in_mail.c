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
#define SAVE "/net/save/"
 
#define log(x) log_file("MS", x)
#define MS_SAVE "mail-queue"
#define FLUSH_TIME 86400
#define AGE_TIME 604800
#define EOF "%EOF%"
#define EOT "%EOT%"
 
static string upd;
static mapping new_mail_queue, sockets;
static string receiver, from, to, subject, message,cc;
static int date;
static string mud;
static mixed mqi;
static mixed outgoing;
 
mixed mail_queue;
int date_last_flushed;
 
void process_message(int fd);
void flush_mail_queue();
void age_queue();
void bad_port(string mud, string from, string message);
 
void reset() {
  if ((time() - date_last_flushed) > FLUSH_TIME)
  {
    mqi = keys(mail_queue);
    flush_mail_queue();
    date_last_flushed = time();
  }
  age_queue();
} /* reset() */
 
void create() {
  seteuid((string)"/secure/master"->creator_file(this_object()));
  mail_queue = ([ ]);
  sockets = ([ ]);
  mqi = ({ });
  restore_object(SAVE + MS_SAVE);
  mqi = keys(mail_queue);
  flush_mail_queue();
} /* create() */

/*
 * This adds on the lmud and lname bit if it is not in name@address
 * format.
 */
string convert_name(string lname, string lmud) {
  string tmp, tmpaddr;
 
  if (sscanf(lname, "%s@%s", tmp, tmpaddr) != 2) {
    tmp = lname;
    tmpaddr = lmud;
  }

/*
 * this doe not handle aliases for the same mud at the moment.  I
 * will add in some new code here real soon now.  Maybe...
 */ 
  if (tmpaddr != mud_name()) return tmp + "@" + tmpaddr;
  return tmp;
} /* convert_name() */

/*
 * Ahhh, so this send the intermud mail...
 */
void remote_mail(string own, string mud, mixed lto, mixed lcc, 
                 string lfrom, string lsubject, int ldate, string lmessage) {
  int i;
  string *tmp;
 
  if (file_name(previous_object()) != MAILER) return;
 
  if (!lmessage) lmessage = "\n";
 
  tmp = explode(lmessage, "\n");
  for (i = 0; i < sizeof(tmp); i++) {
    if (tmp[i] == EOF) 
      tmp[i] = EOF + ".";
    else if (tmp[i] == EOT)
      tmp[i] == EOT + ".";
  }
 
  if (!tmp) tmp = ({ });
  lmessage = implode(tmp, "\n");
  if (!mail_queue[mud]) mail_queue[mud] = ({ });
 
  mail_queue[mud] += ({
      ([ "recipient" : own, "to" : lto, "cc" : lcc,
         "from" : lfrom, "subject" : lsubject, "date" : ldate, 
         "message" : lmessage ]) 
                     });
  save_object(SAVE + MS_SAVE);
  mqi += ({ mud });
} /* remote_mail() */
 
void bad_port(string lmud, string lfrom, string msg) {
  object ob;
 
  ob = find_player(lfrom);
  if (!ob) return;
 
  tell_object(ob,
"The mud " + lmud + " doesn't exist or has a bad port address.\n" +
"If the mud should exist, notify your local admins.\n");
/* Cough, cough...
  tell_object(ob,
"Saving letter in: " + TMP_DIR + "/" + lfrom + ".dead.letter\n");
  write_file(TMP_DIR + "/" + lfrom + ".dead.letter", msg);
 */
  mail_queue[lmud] = 0;
} /* bad_port() */
 
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
 
void close_callback(int id) {
  map_delete(sockets, id);
  return;
} /* close_callback() */
 
void connected(int id) {
  sockets[id] = ([ "msg" : "" ]);
} /* connected() */
 
void read_callback(int id, string data) {
  if (data == (EOT + "\n")) {
    sockets[id]["msg"] += data;
    process_message(id);
    return;
  }
  sockets[id]["msg"] += data;
} /* read_callback() */
 
void process_message(int id) {
  mixed tmp, tmp2;
  string *totmp, *cctmp;
  int i, j, res;
 
  sscanf(sockets[id]["msg"], "%s\n%s", mud, tmp);
  sockets[id]["msg"] = tmp;
 
  tmp = explode(sockets[id]["msg"], EOF);
  tmp = tmp[0..sizeof(tmp)-2];
  tmp2 = allocate(sizeof(tmp));
 
  for (i = 0; i < sizeof(tmp); i++) {
    tmp2[i] = explode(tmp[i], "\n");
  }
  for (i = 0; i < sizeof(tmp2); i++) {
    receiver = convert_name(tmp2[i][0], mud_name());
    totmp = explode(tmp2[i][1], " ");
    for (j = 0; j < sizeof(totmp); j++)
      totmp[j] = convert_name(totmp[j], mud);
    cctmp = explode(tmp2[i][2], " ");
    if (cctmp[0])
      for (j = 0; j < sizeof(cctmp); j++)
        cctmp[j] = convert_name(cctmp[j], mud);
    else
      cctmp = ({ });
    from = convert_name(tmp2[i][3], mud);
    subject = tmp2[i][4];
    sscanf(tmp2[i][5], "%d", date);
    message = implode(tmp2[i][6..sizeof(tmp2[i]) - 1], "\n");
/*
 * Ok...  IN here we need to do a call out to check the validity
 * of the mud this is comming from.
 *
 */
    NAME_D->lookup_mud(mud, "got_address", ({ socket_address(id),
                                              receiver, totmp, cctmp,
                                              from, subject, date,
                                              message }));
  }
} /* process_message() */

void mail_postmaster(string from, string subject, mixed *args) {
  MAILER->do_mail_message("postmaster", from, subject,
                       0, ">From: "+args[4]+"\nTo: "+
                       implode(args[2], ", ")+"\nCc: "+
                       implode(args[3], ", ")+"\nSubject: "+
                       args[5]+"\nDate: "+ctime(args[6])+
                       "\n"+args[7], 1);
} /* mail_postmaster() */

void got_address(string host, string addr, mixed *args) {
  object ob;
  string *to, name, name2;
  int port;

  if (!addr) {
/* Unknown mud just tried to mail us... */
/*
 * Now, what exactly should I do about it?  Mail it to the postmaster?
 * Yeah, sounds good to me...
 */
    mail_postmaster(args[4], "From unknown mud", args);
/* Don't bother returning it to them. */
    return ;
  }
  sscanf(addr, "%s %d", name, port);
  sscanf(args[0], "%s %d", name2, port);
  if (!name || !name2) {
    mail_postmaster(args[4], "Strange error", args);
    return ;
  }
  if (name != name2) {
    mail_postmaster(args[4], "Forged mail", args);
    return ;
  }
/* Ok, its right.  Now deliver it... */
  MAILER->do_mail_message(implode(args[2], ","), args[4], args[5],
                       implode(args[3], ","), args[7], 0, args[1]);
} /* got_address() */
 
void flush_mail_queue() {
  string *muds, address, port;
  int id;
 
  if (!sizeof(mqi)) return;
 
  outgoing = mail_queue[mqi[0]];
  id = INET_D->open_service(mqi[0], "mail");
  if (id < 0) {
    log("flush_mail_queue: open_service: " + socket_error(id) + "\n");
    mqi -= ({ mqi[0] });
    flush_mail_queue();
  }
} /* flush_mail_queue() */

/* I wonder when this gets called?  By magic obviously. */
void service_callback(int id) {
  int i;
 
  previous_object()->write_socket(id, mud_name() + "\n");
  for (i = 0; i < sizeof(outgoing); i++) {
    previous_object()->write_socket(id, outgoing[i]["recipient"] + "\n");
    previous_object()->write_socket(id, implode(outgoing[i]["to"], " ") + "\n");
    previous_object()->write_socket(id, implode(outgoing[i]["cc"], " ") + "\n");
    previous_object()->write_socket(id, outgoing[i]["from"] + "\n");
    previous_object()->write_socket(id, outgoing[i]["subject"] + "\n");
    previous_object()->write_socket(id, outgoing[i]["date"] + "\n");
    previous_object()->write_socket(id, outgoing[i]["message"] + "\n");
    previous_object()->write_socket(id, EOF + "\n");
  }
  previous_object()->write_socket(id, EOT + "\n");
  previous_object()->close_socket(id);
  map_delete(mail_queue, mqi[0]);
  mqi -= ({ mqi[0] });
  save_object(SAVE + MS_SAVE);
  call_out("flush_mail_queue", 60);
} /* service_callback() */
