inherit "/std/object";

void setup() {
  set_name("mailer");
  set_short("Mailer object");
  set_long(
"It is a small black box with pictures of small red demons painted on "+
"the front.  Though, some of them look suspicously like frogs.\n");
} /* setup() */

void init() {
  add_action("do_mail", "mail");
} /* init() */

int do_mail(string str) {
  return (int)"/obj/handlers/mail_track"->mail(str);
} /* do_mail() */
