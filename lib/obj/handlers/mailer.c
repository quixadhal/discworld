inherit "std/object";
 
#include "mail.h"
#include "inet.h"
 
static int just_mail;
int new;
mixed mail;
static int *deleted, cur;
static string subject, cc, send_to, body, from;
static string do_this_last;
 
/*
 * functions should be called on this I suppose, although I suppose
 * every one should get their own one... hmmm.
 */
int *expand_range(string str);
int mail_message(string str, string bo);
int reply_message(string str, string sub, string cc);
void do_mail_message(string to, string from, string sub, string cc, string body,
                  int send_inter, string only_to);
 
set_do_this_last(bing) { do_this_last = bing; }
query_do_this_last() { return do_this_last; }
 
/* 
 * Ok format...
 * mail ({ ({ from, to, subject, ccers, body }) });
 */
 
void setup() {
  mail = ({ });
  set_short("Mailer object");
  set_long("The wonderous and zoopy mailer object. Beware of the chicken.\n");
/* should not be droped, should self destruct. */
  reset_drop();
  seteuid("mailer");
  new = -1;
} /* setup() */
 
void show_headers(string str) {
  int i, cols;
  string s;
 
  cols = (int)this_player()->query_cols();
  if (str) {
    restore_object("/save/post/"+str);
    from = str;
    if (new == -1)
      cur = 0;
    else
      cur = new;
  }
  if (!mail)
    mail = ({ });
  if (!sizeof(mail))
    write("No messages.\n");
  if (!deleted)
    deleted = allocate(sizeof(mail));
  for (i=0;i<sizeof(mail);i++) {
    s = "";
    if (i==cur)
      s += ">";
    else
      s += " ";
    if (deleted[i])
      s += "D ";
    else if (i>=new && new != -1)
      s += "N ";
    else
      s += "  ";
    s += (i+1)+": "+mail[i][M_FROM]+"  Sub: ";
    printf("%s%-=*s\n", s, cols-strlen(s),mail[i][M_SUB]);
  }
} /* show_headers() */
 
void status_line() {
  if (just_mail)
    return ;
  if (!sizeof(mail))
    write("[None] ");
  else
    write("[1-"+sizeof(mail)+"]("+(cur+1)+") ");
  write("Mail (+ - R r m q x d h f s ?) : ");
} /* status_line() */
 
/* this will be called when asked to read mail, what else? */
int read_mail(string str, string sub) {
  if (str) {
    restore_object("/save/post/"+this_player()->query_name());
    from = (string)this_player()->query_name();
    just_mail = 1;
    if (!sub)
      mail_message(str, 0);
    else
      reply_message(str, sub, "");
    return 1;
  }
/* ok this will also load the stuff... jolly good. */
  show_headers((string)this_player()->query_name());
  MAIL_TRACK->add_mailer(this_object(), from);
  status_line();
  input_to("read_loop");
  return 1;
} /* read_mail() */
 
int load_me(string str) {
  int i;
  if (file_name(previous_object())[0..strlen(MAILER)-1] != MAILER) {
    write("Bad boy\n");
    return 0;
  }
  seteuid("mailer");
  restore_object("/save/post/"+str);
  if (!mail)
    mail = ({ });
  from = str;
  return 1;
} /* load_me() */
 
void save_me() {
  if (from)
    save_object("/save/post/"+from);
} /* save_me() */
 
string new_mail(string str) {
  mixed s;
 
  new = -1;
  restore_object("/save/post/"+str);
  if (new != -1) {
/* Can I just say puke here please?  Thanks */
    if (!(s=(object)this_player()->query_guild_ob()) ||
        !(s=(object)s->query_start_pos()) ||
        !(s=(string)s->query_new_messages()))
       return "\nYou have NEW messages.\n"+
              "Go to the Post Office to read them.\n\n";
    return s;
  }
  return "";
} /* new_mail() */
 
string finger_mail(string str) {
  string ret;
 
  new = -1;
  mail = ({ });
  restore_object("/save/post/"+str);
  if (!mail || !sizeof(mail))
    return "No mail.\n";
  ret = ""+sizeof(mail)+" mail messages";
  if (new != -1) {
    ret += ", "+(sizeof(mail)-new)+" of them unread.\n";
  } else {
    ret += ".\n";
  }
  return ret;
} /* finger_mail() */
 
void get_message(string from, string to, string subject, string cc, string body) {
  mail += ({ ({ time(), from, to, subject, cc, body }) });
  if (new == -1)
    new = sizeof(mail)-1;
  if (deleted)
    deleted += ({ 0 });
  save_me();
} /* get_message() */

static void local_mail(string from, string real_to, string to, 
                       string subject, string cc,
                       string body, object new_mailer, int make_bounce) {
  object bing, ob;
  string name, mud;
  int irble;

  if ((bing=(mixed)MAIL_TRACK->find_mailer(real_to))) {
    bing->get_message(from, to, subject, cc, body);
    if (find_player(real_to) && sscanf(from, "%s@%s", name, mud) == 2) {
      ob = clone_object(POST_FROG);
      ob->move_player("X", COMM_ROOM);
      ob->send_mail(real_to, from);
    } else if (this_player() && environment(this_player()))
      if (!(irble = (int)environment(this_player())->send_message(real_to))) {
        ob = clone_object(POST_FROG);
        write("A small frog with a post office hat wanders up and takes "+
              "your mail.\n");
        say("A small frog with a post office hat wanders up and takes "+
            this_player()->query_name()+"'s mail.\n");
        ob->move(environment(this_player()));
        ob->send_mail(real_to, from);
        irble = 1;
/*
        if ((ob = find_player(to))) {
          tell_object(ob, "A small frog wanders up and tells you, "+
                      "you have some mail from "+this_player()->query_name()+
                      "! (bing)\n");
          tell_room(environment(ob), "A small frog wanders up to "+to+
                    " and whispers something to him.\n", ({ ob }) );
        }
 */
      }
  } else if ("secure/login"->test_user(real_to)) {
    new_mailer->load_me(real_to);
    new_mailer->get_message(from, to, subject, cc, body);
    if (!irble && find_player(real_to) && sscanf(from, "%s@%s", name, mud) == 2) {
      ob = clone_object(POST_FROG);
      ob->move_player("X", COMM_ROOM);
      ob->send_mail(real_to, from);
    } else if (this_player() && environment(this_player()))
      if (!(irble = (int)environment(this_player())->send_message(real_to))) {
        write("A small frog with a post office hat wanders up and takes "+
              "your mail.\n");
        say("A small frog with a post office hat wanders up and takes "+
            this_player()->query_name()+"'s mail.\n");
        ob = clone_object(POST_FROG);
        ob->move(environment(this_player()));
        ob->send_mail(real_to, from);
        irble = 1;
/*
        if ((ob = find_player(to))) {
          tell_object(ob, "A small frog wanders up and tells you, "+
                    "you have some mail from "+this_player()->query_name()+
                    "! (bing)\n");
          tell_room(environment(ob), "A small frog wanders up to "+to+
                    " and whispers something to him.\n", ({ ob }) );
        }
 */
      }
  } else {
    if (make_bounce) {
      do_mail_message(from, "postmaster", "Error!  User unknown", 0,
                   "Original message included:\n"+
                   ">From : "+from+"\nTo : "+to+
                   "\nSubject : "+subject+"\nCc : "+
                   cc+"\n"+body, 1, 0);
    }
  }
} /* local_mail() */

string check_local(string str) {
  string mud, name;

  if (sscanf(str, "%s@%s", name, mud) == 2) {
    if (mud == mud_name())
      return name;
    return 0;
  }
  return str;
} /* check_local() */

int check_address(string str) {
  string mud, name;

  if (!name)
    name = "";
  name = str;
  if (sscanf(str, "%s@%s", name, mud) != 2 || mud == mud_name()) {
/* local...  */
    return (int)"/secure/login"->test_user(name) ||
           (int)MAIL_TRACK->query_list(name);
  }
  return 1;
} /* check_address() */
 
void send_message() {
  int i;
  object new_mailer, bing, ob;
  string *goto, *cc_e, *o_to;
  string local_name;
 
  o_to = goto = explode(send_to, ",");
  if (!goto) goto = ({ });
  if (!cc) cc = "";
  if (cc && cc != "" && cc != ",")
    goto += (cc_e = explode(replace(lower_case(cc), " ", ","), ",") - ({ "" }));
  else
    cc_e = ({ });
  for (i=0;i<sizeof(goto);i++) {
    goto[i] = (string)this_player()->expand_nickname(goto[i]);
/* ok expand mailing lists... */
    if (MAIL_TRACK->query_list(goto[i])) {
      if (member_array(goto[i], cc_e) == -1)
        cc_e += ({ goto[i] });
      goto += (mixed)MAIL_TRACK->query_members(goto[i]);
      goto = delete(goto,i,1);
      i--;
    } else if (member_array(goto[i], goto) != i) {
/* Make sure it only sends to any one person once... */
      goto = delete(goto, i, 1);
      i--;
    }
  }
  cc = implode(cc_e, ",");
  new_mailer = clone_object(MAILER);
  for (i=0;i<sizeof(goto);i++) {
    if (goto[i])
      write("Sending to : "+goto[i]+"\n");
    else {
      goto = delete(goto, i, 1);
      i--;
    }
    if (local_name = check_local(goto[i])) {
      local_mail(from, local_name, 
                 send_to,
                 subject, cc, body, new_mailer, 0);
      goto = delete(goto, i, 1);
      i--;
    }
  }
  new_mailer->dest_me();
/* send any or all intermud mail...  */
  if (sizeof(goto))
    OUT_MAIL->remote_mail(from, send_to, subject, cc, body, goto);
} /* send_message() */

void do_mail_message(string to, string from, string subject, string cc,
                 string body, int send_interfrog, string only_to) {
  string local_name;
  string *goto;
  int i;
  object new_mailer;

  if (!only_to) {
    to = replace(to, " ", ",");
    goto = explode(to, ",");
    if (cc) {
      cc = replace(cc, " ", ",");
      goto += explode(to, ",");
    }
  } else {
    only_to = replace(only_to, " ", ",");
    goto = explode(only_to, ",");
    if (!goto || !sizeof(goto))
      return ;
  }
  goto = goto - ({ "" });
/*
 * First of all, get rid of all duplicates... and expand mailing
 * lists...
 */
  for (i=0;i<sizeof(goto);i++) {
/* ok expand mailing lists... */
    if (MAIL_TRACK->query_list(goto[i])) {
      goto += (mixed)MAIL_TRACK->query_members(goto[i]);
      goto = delete(goto,i,1);
      i--;
    } else if (member_array(goto[i], goto) != i) {
/* Make sure it only sends to any one person once... */
      goto = delete(goto, i, 1);
      i--;
    }
  }
  new_mailer = clone_object(MAILER);
  for (i=0;i<sizeof(goto);i++) {
    if ((local_name = check_local(goto[i]))) {
/* Local mail... */
      local_mail(from, local_name, to, subject, cc, body, new_mailer, 1);
      goto = delete(goto, i, 1);
      i--;
    }
  }
  new_mailer->dest_me();
/* Ok, now the interfrog mail */
  if (send_interfrog && sizeof(goto))
    OUT_MAIL->remote_mail(from, to, subject, cc, body, goto);
} /* do_mail_message() */
 
string message_string(int i) {
  return "Sent at "+ctime(mail[i-1][M_TIME])+
              "\nMessage #"+i+(deleted[i-1] ? ("  (deleted)\n") : "\n")+
              "From : "+mail[i-1][M_FROM]+"\nTo : "+mail[i-1][M_TO]+
              "\nSubject : "+mail[i-1][M_SUB]+"\nCc : "+
              mail[i-1][M_CC]+"\n"+mail[i-1][M_BODY];
} /* message_string() */
 
void read_message(int i) {
  write("\n\n");
  this_player()->set_finish_func("finish_message");
  this_player()->more_string(message_string(i));
} /* read_message() */
 
void finish_message() {
  status_line();
  input_to("read_loop");
} /* finish_message() */
 
void delete_things() {
  int i;
  while (i<sizeof(mail)) {
    if (deleted[i]) {
      mail = delete(mail,i,1);
      deleted = delete(deleted,i,1);
    } else
      i++;
  }
} /* delete_things() */
 
int mail_message(string to, string bo) {
  int i;
  string *bing;
 
  send_to = (string)this_player()->expand_nickname(send_to);
  send_to = replace(to, " ", ",");
  bing = explode(send_to, ",") - ({ "" });
  for (i=0;i<sizeof(bing);i++) {
    if (!check_address(bing[i])) {
      write(bing[i]+" does not exist, try try again\n");
      bing = delete(bing, i, 1);
      i--;
    }
  }
  if (!sizeof(bing)) {
    write("No one to mail to.\n");
    status_line();
    input_to("read_loop");
    return 1;
  }
  send_to = implode(bing, ",");
  write("Subject : ");
  input_to("get_subject");
  subject = 0;
  send_to = to;
  cc = "";
  if (!bo)
    body = "";
  else
    body = bo;
} /* mail_message() */
 
int reply_message(string to, string sub, string rcc) {
  string s1;
 
  send_to = to;
  subject = sub;
  if (sscanf(sub,"Re: %s",s1)==1)
    subject = "Re(many): "+s1;
  else if (sscanf(sub,"Re(many): %s",s1)!=1)
    subject = "Re: "+sub;
  cc = rcc;
  body = "";
  write("Press return for a subject of \""+subject+"\"\nSubject : ");
  input_to("get_subject");
  return 1;
} /* reply_message() */
 
int get_subject(string str) {
  if (!str)
    str = "";
  if (str == "" && !subject) {
    write("Aborted.\n");
    status_line();
    input_to("read_loop");
    return 1;
  }
  if (str != "")
    subject = str;
  write("Enter your Cc's now.  Terminate with a blank line.\nCc: ");
  input_to("get_cc");
  return 1;
} /* get_subject() */
 
int finish_body(string str) {
  if (!str || str == "") {
    write("Aborted.\n");
    if (just_mail)
      return 1;
    status_line();
    input_to("read_loop");
    return 1;
  }
  body = str + (string)this_player()->append_signature();
  write("Ok.\n");
  send_message();
  write("Sent.\n");
  if (just_mail)
    return 1;
  status_line();
  input_to("read_loop");
  return 1;
} /* finish_body() */
 
int do_forward(string to, string sub, string bo) {
  send_to = to;
  subject = sub;
  body = bo;
  cc = 0;
  send_message();
  status_line();
  input_to("read_loop");
  return 1;
} /* do_forward() */
 
int get_cc(string str) {
  string *bits;
  int i;
 
  if (str == "" || !str || str == "**") {
    this_player()->do_edit(0, "finish_body");
    return 1;
  }
  str = replace(str, " ", ",");
/*
  str = implode(explode(str, " "), ",");
 */
  bits = explode(str, ",") - ({ "" });
  for (i=0;i<sizeof(bits);i++)
    if (!check_address(bits[i])) {
      write("Sorry "+bits[i]+" does not exist.\n");
      bits = delete(bits, i--, 1);
    }
  cc += ","+implode(bits,",");
  write("Cc: ");
  input_to("get_cc");
  return 1;
} /* get_cc() */
 
int read_loop(string str) {
  int num, i, *ms;
  string comm, s1, s2;
 
 
  if (just_mail)
    return 1;
  if (!cur)
    cur = 1;
/*
 * ok this should (theoreticaly) do some clever things and get the message
 * number being reffered to out of the junk.  But then again, maybe not.
 */
  if (str == "-")
    num = -1;
  else if (sscanf(str, "%d%s",num,str)!=2) {
     comm = "";
    if (sscanf(str, "%s %d %s", s1, num, s2) == 3)
      str = s1 + " " + s2;
  }
  if (sscanf(str, "%s %s", comm, str) != 2) {
    comm = str;
    str = "";
  }
 
  comm = extract(comm,0,0);
  switch (comm) {
    case "" : if (num > 0 && num <= sizeof(mail)) {
                read_message(num);
                cur = num;
                return 1;
              } else if (num==0) {
                if (++cur > sizeof(mail)) {
                  write("Run out of messages\n");
                  cur = sizeof(mail);
                } else {
                  read_message(cur);
                  return 1;
                }
              } else {
                write("Sorry message number must be in the range "+1+
                      " to "+sizeof(mail)+"\n");
              }
              break;
    case "+" : if (num == 0)
                 cur++;
               else
                 cur += num;
               if (cur > sizeof(mail) || cur < 1) {
                 write("Gone too far.\n");
                 if (num > 0)
                   cur = sizeof(mail);
                 else
                   cur = 1;
               } else
                 read_message(cur);
               break;
    case "-" : if (num == 0)
                 cur--;
               else
                 cur -= num;
               if (cur > sizeof(mail) || cur < 1) {
                 write("Gone too far.\n");
                 if (num>0)
                   cur = 1;
                 else
                   cur = sizeof(mail);
               }
               break;
    case "x" : write ("Dont let them massages pile up.\n");
               new = -1;
               save_me();
               if (do_this_last)
                 call_other(do_this_last[0], do_this_last[1], do_this_last[2]);
               else
                 dest_me();
               return 1;
    case "q" : delete_things();
               new = -1;
               save_me();
               write("Ok.\n");
               if (do_this_last && do_this_last[0] && stringp(do_this_last[1]))
                 call_other(do_this_last[0], do_this_last[1], do_this_last[2]);
               else
                 dest_me();
               return 1;
    case "d" : ms = expand_range(str);
               for (i=0;i<sizeof(ms);i++) {
                 deleted[ms[i]-1] = !deleted[ms[i]-1];
                 if (deleted[ms[i]-1])
                   write("Deleted message "+ms[i]+".\n");
                 else
                   write("Undeleted message "+ms[i]+".\n");
               }
               break;
    case "m" : mail_message(str,""); /* no cc at this point ;) */
               return 0;
	       break;
    case "r" : if (num)
                 if (num > sizeof(mail) || num < 1)
                   write("Number out of bounds (6!!!!)\n");
                 else {
                   reply_message(mail[num-1][M_FROM], mail[num-1][M_SUB],
                                 mail[num-1][M_CC]+","+
                                 implode(explode(mail[num-1][M_CC]+","+
                                                 mail[num-1][M_TO], ",") -
                                                 ({ from }), ","));
                   return 0;
               } else {
/* ok so handle the fact that we may be a CC and send to the person who
 * it was addressed to as well. */
                 reply_message(mail[cur-1][M_FROM], mail[cur-1][M_SUB],
                                 implode(explode(mail[cur-1][M_CC]+","+
                                                 mail[cur-1][M_TO], ",") -
                                                 ({ from }), ","));
                 return 0;
               }
               break;
    case "h" : show_headers(0);
               break;
    case "?" : cat("/doc/helpdir/mail");
               break;
    case "R" : if (num)
                 if (num > sizeof(mail) || num < 1)
                   write("Number out of bounds. Better hire a better chicken\n");
                 else /* dont remember CC's */ {
                   reply_message(mail[num-1][M_FROM], mail[num-1][M_SUB], "");
                   return 0;
               } else {
                 reply_message(mail[cur-1][M_FROM], mail[cur-1][M_SUB], "");
                 return 0;
               }
               break;
    case "s" : /* save to a file... only for the lonely, I mean wizs */
               if (!this_player()->query_creator()) {
                 write("Only creators can save files, sorry.\n");
                 break;
               }
               if (!num)
                 num = cur;
               if (num < 1 || num > sizeof(mail)) {
                 write("Message out of bounds.\n");
                 break;
               }
               str = (string)this_player()->get_path(str);
               if (!"/secure/master"->valid_write(str, geteuid(this_player()),
                        "write_file")) {
                 write("You cannot write to that file.\n");
                 break;
               }
               if ((i=file_size(str)) > 0)
                 write("Appending.\n");
               else if (i == -2) {
                 write("Directory\n");
                 break;
               }
               seteuid(geteuid(this_player()));
               if (!write_file(str, message_string(num)))
                 write("Write file failed.\n");
               else
                 write("Saved message "+num+" to "+str+".\n");
               seteuid("mailer");
               break;
    case "f" : /* forward a message... */
               if (!num)
                 num = cur;
               if (num < 1 || num > sizeof(mail)) {
                 write("Message out of bounds.\n");
                 break;
               }
               do_forward(str, "Fwd: "+mail[num-1][M_SUB],
                          ">"+replace(message_string(num), "\n", "\n>"));
/*
               mail_message(str, ">"+implode(explode(
                                 message_string(num), "\n"), "\n>"));
 */
               return 0;
  }
  status_line();
  input_to("read_loop");
} /* read_loop() */
 
void dest_me() {
  MAIL_TRACK->delete_mailer(this_object());
  ::dest_me();
} /* dest_me() */
 
int *expand_range(string str) {
  int *ms, i, num, start, end;
  string s1,s2;
 
  if (str == "" || !str)
    str = ""+cur;
 
  if (!sizeof(mail))
    return ({ });
 
  str = implode(explode(str, " "),"");
  ms = ({ });
  if (sscanf(str, "%sall%s", s1,s2)==2) {
    for (i=1;i<sizeof(mail)+1;i++)
      ms += ({ i });
    return ms;
  }
  while (sscanf(str, "%d%s",start,str) == 2) {
    if (start < 1)
      start = 1;
    if (start > sizeof(mail))
      start = sizeof(mail);
    if (str && str != "" && str[0] == '-') {
      sscanf(str, "-%d%s", end, str);
      if (end >= start) {
        if (end > sizeof(mail))
          end = sizeof(mail);
        for (i=start;i<=end;i++)
          if (member_array(i, ms) == -1)
            ms += ({ i });
      }
    } else
      if (member_array(start, ms) == -1)
        ms += ({ start });
    sscanf(str,",%s",str);
  }
  return ms;
} /* expand_range() */
