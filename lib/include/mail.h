/**
 * This is the include file for the mailer.
 * @author Pinkfish
 * @started 1992 sometime
 */
#ifndef __SYS__MAIL
#define __SYS__MAIL

#ifndef __MAIL_H_NO_CLASSES
/**
 * The mail header class.
 * @member number the message number
 * @member status the current status of the message
 * @member subject the subject of the message
 * @member from who the message is from
 */
class mail_header {
  int number;
  string status;
  string subject;
  string from;
}

/**
 * The mail message class.
 * @member to the list of people the message is to
 * @member cc the list of people the message is cced to
 * @member body the actual message text
 * @member subject the subject of the message
 */
class mail_message {
  string *to;
  string *cc;
  string body;
  string from;
  string subject;
}
#endif

/**
 * A define of the days as strings, starting from Sun as 0 to Sat as 6.
 */
#define DAYS ({"Sun", "Mon", "Tue", "Wed", "Thu", \
               "Fri", "Sat"})

/**
 * A define of an array of the months as strings, starting from Jan as 0 and
 * Dec as 11.
 */
#define MONTHS ({"Jan", "Feb", "Mar", "Apr", "May", "Jun", \
     "Jul", "Aug", "Sep", "Oct", "Nov", \
     "Dec"})

/**
 * The path to the mail tracker handler.  Keeps track of who has which
 * mailer.
 */
#define MAIL_TRACK "/obj/handlers/mail_track"
/**
 * The path to the actual mailer object.
 */
#define MAILER "/obj/handlers/mailer"
/**
 * The path to the auto mailer object.
 */
#define AUTO_MAILER "/global/auto_mailer"
/**
 * The path to the smtp mail outgoing handler.
 */
#define OUT_MAIL "/net/daemon/out_mail"
/**
 * The path to the post office frog NPC.
 */
#define POST_FROG "/obj/monster/post_frog"
/**
 * The path to the place where the frogs try to go when they have
 * finished delivering their mail.
 */
#define FROG_HOME "/d/am/buildings/post/mailing"
/**
 * The path to the SMTP object.
 */
#define SMTP "/net/obj/smtp"
/**
 * The path to the location to look for external incoming mail.
 */
#define EXTERNAL_MAIL_PATH "/secure/mail/incoming/"
/**
 * This is the path to the parcel post handler.
 */
#define MAIL_PARCEL_HANDLER "/obj/handlers/parcel"

/**
 * This is the maximum number of messages storable in a single folder.
 */
#define MAX_FOLDER_SIZE 250
#define WARN_FOLDER_SIZE 100

#endif /* __SYS__MAIL */
