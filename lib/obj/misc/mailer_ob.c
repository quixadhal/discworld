inherit "/std/object";

int do_mail(string str);

void setup() {
  set_name("mailer");
  set_short( "mailer object" );
  set_long("It is a small black box with pictures of small red demons "
           "painted on "+
           "the front.  Though, some of them look suspicously like frogs.\n");
  set_weight(1);
  reset_drop();
} /* setup() */

void init() {
  add_command("mail", "", (: do_mail(0) :) );
  add_command("mail", "<string>", (: do_mail($4[0]) :) );
} /* init() */

int do_mail(string str) {
  return (int)"/obj/handlers/mail_track"->mail(str);
} /* do_mail() */

