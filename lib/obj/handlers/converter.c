/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: converter.c,v 1.1 1998/01/06 05:03:33 ceres Exp $
 * $Log: converter.c,v $
 * Revision 1.1  1998/01/06 05:03:33  ceres
 * Initial revision
 * 
*/
/* 
 * Mail converter.
 * By Turrican@Discworld, May 1995
 */

mixed *mail;
nosave string *names;
nosave int no_more;

int query_busy(string name);

void create() {
  seteuid("mailer");
  names = ({ });
  no_more = 0;
}

int ok_to_shut() {
  no_more = 1;
  return (sizeof(names) == 0);
}

void convert_it(string pname) {
  int i, bing;

  if (no_more) {
    write("Can't start converting mail, try again after the reboot.\n");
    return;
  }
  if (query_busy(pname)) {
    write("Argh ! Already busy converting mail for "+pname+".\n");
    return;
  }
  mail = ({ });
  if (file_size("/save/post/"+pname+".o") == -1)
    return;
  unguarded((: restore_object, "/save/post/"+pname :));
  bing = sizeof(mail);
  if (!bing) {
    unguarded((: rm, "/save/post/"+pname+".o" :));
    return;
  }
  write("Converting mail for "+pname+".\n");
  names += ({ pname });
  for (i = 0; i < bing; i++) {
    reset_eval_cost();
    call_out("do_it", (i+1)*3, (mail[i] + ({ pname })));
  }
  call_out("reset_busy", (i+1)*3, pname);
  unguarded((: rm, "/save/post/"+pname+".o" :));
}

void do_it(mixed *arg) {
  object handler = clone_object("/obj/handlers/folder_handler");
  handler->add_it((["to" : ({ arg[6] }),
    "cc" : ({ }), "from" : arg[1], "subject" : arg[3], "body" :
    "From "+arg[1]+" "+ctime(arg[0])+
    "\nDate: "+ctime(arg[0])+
    "\nFrom: "+arg[1]+
    "\nTo: "+arg[2]+
    "\nSubject: "+arg[3]+
    "\nCc: "+(arg[4]?arg[4]:"")+"\n\n"+
    arg[5] ]), 1);
}

int query_busy(string name) {
  return (member_array(name, names) != -1);
}

void reset_busy(string name) {
  names -= ({ name });
  if (no_more && !sizeof(names)) {
    "/obj/shut"->shut(2);
  }
}
