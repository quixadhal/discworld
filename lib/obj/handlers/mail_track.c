inherit "std/object";
#include "mail.h"
#define SECURITY if (!"/secure/master"->high_programmer(geteuid(previous_object()))) return 0;

int add_member(string list, string mem);
void load_me();
void save_me();

/*
 * this object keeps track of who has a mailer at the moment and sends the
 * mail to there instead.
 * bounce.
 */

/*
 * Hmmm, well I think I will add mailing list support. Sounds cute huh?
 */

static object *our_mailers;
mapping mailing_lists;

void setup() {
  set_short("Mail tracker");
  set_long("Mail tracker object");
  reset_get();
  reset_drop();
  our_mailers = ({ });
  mailing_lists = ([ ]);
  load_me();
  move("/d/admin/room/mail_room");
  seteuid("Root");
} /* setup() */

int add_mailer(object ob, string str) {
  int i;
  if ((i=member_array(str, our_mailers)) == -1)
    our_mailers += ({ ob, str });
  else {
    write(i);
    if (our_mailers[i-1]) /* make sure it hasnt already died somehow */
      our_mailers[i-1]->dest_me(); /* we dont want two of them */
    our_mailers[i-1] = ob;
  }
  return 1;
} /* add_mailer() */

object find_mailer(string str) {
  int i;

  if ((i=member_array(str, our_mailers))==-1)
    return 0;
  return our_mailers[i-1];
} /* find_mailer() */

int delete_mailer(object ob) {
  int i;

  if ((i=member_array(ob, our_mailers))==-1)
    return 0;
  our_mailers = delete(our_mailers,i,2);
} /* delete_mailer() */

void we_are_done(object me) {
  if (me)
    me->dest_me();
} /* we_are_done() */

int mail(string str, string sub) {
  object mailer;

  mailer = clone_object(MAILER);
  mailer->set_do_this_last( ({ this_object(), "we_are_done", mailer }) );
  mailer->read_mail(str, sub);
  return 2;
} /* mail() */

void save_me() {
  save_object("/save/mail_track");
} /* save_me() */

void load_me() {
  restore_object("/save/mail_track");
} /* load_me() */

string *query_mailing_lists() {
  return keys(mailing_lists) + get_dir("/d/");
} /* query_mailing_lists() */

/* does this list exist??? */
int query_list(string list) {
  return !undefinedp(mailing_lists[list]) || (file_size("/d/"+list) == -2);
} /* query_list() */

string *query_members(string list) {
  if (mailing_lists[list])
    return mailing_lists[list][0] + ({ });
  if (file_size("/d/"+list) == -2)
    return (string *)("/d/"+list+"/master")->query_members() +
           ({ (string)("/d/"+list+"/master")->query_lord() });
  return ({ });
} /* query_members() */

int query_member(string list, string name) {
  if (!mailing_lists[list])
    if (file_size("/d/"+list) == -2)
      return (int)("/d/"+list+"/master")->query_member(name);
    else
      return 0;
  return member_array(name, mailing_lists[list][0]) != -1;
} /* query_member() */

int query_controller(string list, string name) {
  if (!mailing_lists[list])
    return 0;
  return member_array(name, mailing_lists[list][1]) != -1;
} /* query_controller() */

int create_list(string list, string creator) {
  int i;

  SECURITY
  if (mailing_lists[list])
    return 0;
  if (file_size("/d/"+list) == -2) {
    return 0;
  }
  if ("/secure/login"->test_user(list)) {
    return 0;
  }
  mailing_lists[list] = ({ ({ creator }), ({ creator }), creator });
  save_me();
  return 1;
} /* create_list() */

static int delete_list(string list) {
  if (!mailing_lists[list])
    return 0;
  map_delete(mailing_lists, list);
  save_me();
  return 1;
} /* delete_list() */

int add_controller(string list, string mem) {
  SECURITY
  if (!mailing_lists[list])
    return 0;
  if (member_array(mem, mailing_lists[list][1]) != -1)
    return 0; /* already a member... */
  mailing_lists[list][1] += ({ mem });
  add_member(list, mem);
  save_me();
} /* add_controller() */

int remove_controller(string list, string meme) {
  int i;

  SECURITY
  if (!mailing_lists[list])
    return 0;
  if ((i = member_array(name, mailing_lists[list][1])) == -1)
    return 0;
  mailing_lists[list][0] = delete(mailing_lists[list][1], i, 1);
  save_me();
} /* remove_controller() */

int add_member(string list, string mem) {
  SECURITY
  if (!mailing_lists[list])
    return 0;
  if (member_array(mem, mailing_lists[list][0]) != -1)
    return 0; /* already a member... */
  mailing_lists[list][0] += ({ mem });
  save_me();
  return 1;
} /* add_member() */

int delete_member(string list, string mem) {
  int i;

  SECURITY
  if (!mailing_lists[list])
    return 0;
  if ((i = member_array(mem, mailing_lists[list][0])) == -1)
    return 0;
  mailing_lists[list][0] = delete(mailing_lists[list][0], i, 1);
  remove_controller(list, mem);
  if (!sizeof(mailing_lists[list][0]))
    delete_list(list);
  save_me();
  return 1;
} /* delete_member() */
