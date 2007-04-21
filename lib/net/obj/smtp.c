/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: smtp.c,v 1.4 2000/11/02 06:38:12 ceres Exp $
 * $Log: smtp.c,v $
 * Revision 1.4  2000/11/02 06:38:12  ceres
 * It needed a FQDN for our current sendmail config
 *
 * Revision 1.3  2000/02/18 17:21:39  turrican
 * Let Sendmail determine the FQDN
 *
 * Revision 1.2  1998/05/22 12:14:34  turrican
 * Added possibility of multiple recipients.
 *
 * Revision 1.1  1998/01/06 05:20:37  ceres
 * Initial revision
 * 
*/
/* 
 * SMTP client, written by Turrican@Discworld.
 * Created 4-2-96.
 */

#include <network.h>
#include <mail.h>
#include <mime.h>
#include "smtp.h"

inherit CLIENT;

private nosave mixed *data = ({});
private nosave string *messages = ({});
private nosave int fd = -1;

protected void create() {
  client::create();
  client::SetSocketType(STREAM);
  client::SetDestructOnClose(1);
}

void eventWrite(string mess) {
  return client::eventWrite(fd, mess);
}

void eventSendMail(mixed to, string from, string mess) {
  if (!to || !from || !mess) {
    return;
  }
  if (stringp(to)) {
    data += ({ ({ "MAIL FROM:<" + from + ".discworld@discworld.imaginary.com>\r\n",
                  "RCPT TO:<" + to + ">\r\n",
                  "DATA\r\n" }) });
  } else if (arrayp(to) && sizeof(to)) {
      string *tmpdata;
  
      tmpdata = ({ "MAIL FROM:<" + from + ".discworld@discworld.imaginary.com>\r\n" });
      tmpdata += map(to, (: "RCPT TO:<" + $1 + ">\r\n" :));
      tmpdata += ({ "DATA\r\n" });
      data += ({ tmpdata });
  } else {
    return;
  }
  mess = MIME->rewrite_header(mess);
  mess = replace(mess, ({"\n.", "\n..", "\n", "\r\n"}));
  messages += ({ mess });
  if (fd > 0) {
    return;
  }
  if ((fd = eventCreateSocket(SMTP_HOST, SMTP_PORT)) < 0) {
    data = data[0..<2];
    messages = messages[0..<2];
  }
}

protected void eventSuccess() {
  if (sizeof(data) && sizeof(data[0])) {
    TP(sprintf("SMTP: Writing %s\n", data[0][0]));
    eventWrite(data[0][0]);
    data[0] = data[0][1..];
  } else if (sizeof(data) && !sizeof(data[0])) {
    TP("SMTP: No more data, next message.\n");
    data = data[1..];
    if (!sizeof(data)) {
      TP(sprintf("SMTP: Writing QUIT\n"));
      eventWrite("QUIT\r\n");
    } else
      eventSuccess();
  }
}

protected void eventOKToSendMail() {
  if (sizeof(messages) && messages[0] && (strlen(messages[0]) > 0)) {
    TP("SMTP: Sending mail message.\n");
    eventWrite(messages[0] + "\n.\n");
    messages = messages[1..];
  } else {
    TP("SMTP: No message...\n");
    messages = messages[1..];
    eventWrite("RSET\r\n");
  }
}

protected void eventForward(string who) {
  if (who && who != "") {
    TP(sprintf("SMTP: Forwarding to %s.\n", who));
    eventWrite("RSET\r\n");
    eventWrite(sprintf("RCPT TO:<%s>\r\n", who));
  } else {
    TP("SMTP: Told to forward, but no forwardee...\n");
    eventWrite("QUIT\r\n");
  }
}

protected void eventRead(int fd, string mess) {
  string forward;

  if (!mess) return;
  mess = replace_string(mess, "\r", "");
  switch (mess[0..2]) {
    case "220":
      eventWrite(sprintf("HELO %s\r\n", MUD_IP_NAME));
      break;
    case "250":
    case "251":
      eventSuccess();
      break;
    case "354":
      eventOKToSendMail();
      break;
    case "421":
    case "450":
    case "451":
    case "452":
    case "500":
    case "501":
    case "502":
    case "503":
    case "504":
    case "550":
    case "552":
    case "553":
    case "554":
      TP(sprintf("<FATAL> SMTP: %s", mess));
      eventWrite("QUIT\r\n");
      break;
    case "551":
      if (sscanf(mess, "%*s<%s>%*s", forward) != 3) {
        TP(sprintf("SMTP: %s", mess));
        break;
      }
      eventForward(forward);
      break;
    default:
      break;
  }
}
