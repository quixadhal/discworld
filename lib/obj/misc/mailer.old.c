inherit "std/object";

#include "mail.h"

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
}

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
}

void status_line() {
  if (just_mail)
    return ;
  if (!sizeof(mail))
    write("[None] ");
  else
    write("[1-"+sizeof(mail)+"]("+(cur+1)+") ");
  write("Mail (+ - R r m q x d h f s ?) : ");
}

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
}

int load_me(string str) {
  int i;
  if (sscanf(file_name(previous_object()),MAILER+"#%d",i)!=1) {
    write("Bad boy\n");
    return 0;
  }
  seteuid("mailer");
  restore_object("/save/post/"+str);
  if (!mail)
    mail = ({ });
  from = str;
  return 1;
}

void save_me() {
  if (from)
    save_object("/save/post/"+from);
}

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
}

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
}

void get_message(string from, string to, string subject, string cc, string body) {
  mail += ({ ({ time(), from, to, subject, cc, body }) });
  if (new == -1)
    new = sizeof(mail)-1;
  if (deleted)
    deleted += ({ 0 });
  save_me();
}

void send_message() {
  int i;
  object new_mailer, bing, ob;
  string *goto;

  goto = explode(send_to, " ");
  send_to = (string)this_player()->expand_nickname(lower_case(implode(goto, "")));
  if (MAIL_TRACK->query_list(send_to))
    goto = (mixed)MAIL_TRACK->query_numbers(send_to);
  else
    goto = ({ send_to });
  if (cc)
    goto += explode(implode(explode(lower_case(cc), " "), ""), ",");
  for (i=0;i<sizeof(goto);i++) {
    goto[i] = (string)this_player()->expand_nickname(goto[i]);
/* ok expand mailing lists... */
    if (MAIL_TRACK->query_list(goto[i])) {
      goto += (mixed)MAIL_TRACK->query_numbers(goto[i]);
      goto = delete(goto,i,1);
    }
  }
  new_mailer = clone_object(MAILER);
  for (i=0;i<sizeof(goto);i++) {
    if(goto[i])
      write("Sending to : "+goto[i]+"\n");
    if ((bing=(mixed)MAIL_TRACK->find_mailer(goto[i]))) {
      bing->get_message(from, send_to, subject, cc, body);
      if (environment(this_player()))
        if (!environment(this_player())->send_message(goto[i])) {
          write("A small frog with a post office hat wanders up and takes "+
                "your mail.\n");
          say("A small frog with a post office hat wanders up and takes "+
              this_player()->query_name()+"'s mail.\n");
          if ((ob = find_player(goto[i]))) {
            tell_object(ob, "A small frog wanders up and tells you, "+
                        "you have some mail from "+this_player()->query_name()+
                        "! (bing)\n");
            tell_room(environment(ob), "A small frog wanders up to "+goto[i]+
                      " and whispers something to him.\n", ({ ob }) );
          }
        }
    } else
      if ("secure/login"->test_user(goto[i])) {
        new_mailer->load_me(goto[i]);
        new_mailer->get_message(from, send_to, subject, cc, body);
        if (environment(this_player()))
          environment(this_player())->send_message(goto[i]);
      }
  }
  new_mailer->dest_me();
}

string message_string(int i) {
  return "Sent at "+ctime(mail[i-1][M_TIME])+
              "\nMessage #"+i+(deleted[i-1] ? ("  (deleted)\n") : "\n")+
              "From : "+mail[i-1][M_FROM]+"\nTo : "+mail[i-1][M_TO]+
              "\nSubject : "+mail[i-1][M_SUB]+"\nCc : "+
              mail[i-1][M_SUB]+"\n"+mail[i-1][M_BODY];
}

void read_message(int i) {
  write("\n\n");
  this_player()->set_finish_func("finish_message");
  this_player()->more_string(message_string(i));
}

void finish_message() {
  status_line();
  input_to("read_loop");
}

void delete_things() {
  int i;
  while (i<sizeof(mail)) {
    if (deleted[i]) {
      mail = delete(mail,i,1);
      deleted = delete(deleted,i,1);
    } else
      i++;
  }
}

int mail_message(string to, string bo) {
  string *bing;

  bing = explode(to, " ");
  send_to = implode(bing, "");
  send_to = (string)this_player()->expand_nickname(send_to);
  if (!"secure/login"->test_user(send_to) &&
      !MAIL_TRACK->query_list(send_to)) {
    write(send_to+" does not exist, try try again\n");
    status_line();
    input_to("read_loop");
    return 1;
  }
  write("Subject : ");
  input_to("get_subject");
  subject = 0;
  send_to = to;
  cc = "";
  if (!bo)
    body = "";
  else
    body = bo;
}

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
}

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
}

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
}

int do_forward(string to, string sub, string bo) {
  send_to = to;
  subject = sub;
  body = bo;
  cc = 0;
  send_message();
  status_line();
  input_to("read_loop");
  return 1;
}

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
    if (!"/secure/login"->test_user(bits[i])) {
      write("Sorry "+bits[i]+" does not exist.\n");
      bits = delete(bits, i--, 1);
    }
  cc += ","+implode(bits,",");
  write("Cc: ");
  input_to("get_cc");
  return 1;
}

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
               if (do_this_last)
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
                                 mail[num-1][M_CC]+(mail[num-1][M_TO]!=from ?
                                  ","+mail[num-1][M_TO]: ""));
                   return 0;
               } else {
/* ok so handle the fact that we may be a CC and send to the person who
 * it was addressed to as well. */
                 reply_message(mail[cur-1][M_FROM], mail[cur-1][M_SUB],
                                 mail[cur-1][M_CC]+(mail[cur-1][M_TO]!=from ?
                                  ","+mail[cur-1][M_TO]: ""));
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
}

void dest_me() {
  MAIL_TRACK->delete_mailer(this_object());
  ::dest_me();
}

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
}
