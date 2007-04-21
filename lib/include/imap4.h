#ifndef __IMAP4_H
#define __IMAP4_H

#define PORT_IMAP4 1143

#define IMAP4_VERSION "0.1"

#define IMAP_CONNECTED   0
#define IMAP_AUTHENTICATED   1
#define IMAP_SELECTED 2

#define LOGIN_OB "/secure/login"
#define FOLDER_H "/obj/handlers/folder_handler"

#define DEBUG
#ifdef DEBUG
#define TP(_x) tell_object(find_player("ceres"), (_x))
#else
#define TP(_x)
#endif

#define CHECK_STATE(_state) if ((_state) != sess->state) \
{ \
    eventWrite(fd, sprintf("* BAD Unknown command in %s state\r\n", \
                           imap_states[sess->state])); \
    break; \
} 

#define CHECK_CMD(_num, _str) if (sizeof(bits) < (_num)+2) \
                       { \
                         eventWrite(fd, _str); \
                         break; \
                       }

class imap_session {
  int fd;                      /* socket descriptor */
  int state;                   /* state of the connection */
  int time;                    /* time of last command */
  string user_name;            /* user name */
	string selected;             /* selected folder */
	string command;              /* last command */
  int *deleted;                /* list of deleted messages */
  int num_messages;            /* amount of messages in the folder */
  class mail_header *headers;  /* headers of messages in the folder */
  string *messages;            /* parsed messages */
  int *sizes;                  /* sizes of parsed messages */
  int last;                    /* number of last accessed message */
}

#endif /* __IMAP4_H */
