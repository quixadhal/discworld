/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: loader.c,v 1.1 1998/01/06 04:03:45 ceres Exp $
 * $Log: loader.c,v $
 * Revision 1.1  1998/01/06 04:03:45  ceres
 * Initial revision
 * 
*/
/* this is the file you use to preload things...
 * It will be added to the preload list on the maste object automaticly
 */

string *pre_load;

void create() {
  int i;
  string domain;
  
  foreach(domain in "/secure/master"->query_domains()) {
    //seteuid((string)"/secure/master"->creator_file("/d/"+domain+"/loader"));
    unguarded((: restore_object, "/d/"+ domain + "/loader" :));
    if (!pre_load)
      pre_load = ({ });
    for (i=0;i<sizeof(pre_load);i++) {
      if(pre_load && explode(pre_load[i], "/")[1] != domain)
        printf("invalid " + domain + " preload: " + pre_load[i] +
               "\n");
      else {
        printf(domain +" pre_loading "+pre_load[i]+".\n");
        if (catch(call_other(pre_load[i], "??")))
          call_out("do_load", 0, pre_load[i]);
      }
    }
  }
}

/* this is so the errors get done... ;) */
void do_load(string str) {
  call_other(str, "??");
}

int add_pre_load(string domain, string str) {
  if(member_array(domain, "/secure/master"->query_domains()) == -1)
    return 0;
  
  unguarded((: restore_object, "/d/"+ domain + "/loader" :));
  if (!pre_load)
    pre_load = ({ });
  
  if (member_array(str, pre_load) == -1)
    pre_load += ({ str });
  
  //seteuid((string)"/secure/master"->creator_file("/d/"+domain+"/loader"));
  save_object("/d/" + domain + "/loader");
  return 1;
}

int remove_pre_load(string domain, string str) {
  int i;

  if(member_array(domain, "/secure/master"->query_domains()) == -1)
    return 0;
  
  unguarded((: restore_object, "/d/"+ domain + "/loader" :));
  if (!pre_load)
    pre_load = ({ });

  if ((i=member_array(str, pre_load)) == -1)
    return 0;
  pre_load = delete(pre_load, i, 1);
  //seteuid((string)"/secure/master"->creator_file("/d/"+domain+"/loader"));
  save_object("/d/" + domain + "/loader");
}

string *query_pre_load(string domain) {
  if(member_array(domain, "/secure/master"->query_domains()) == -1)
    return ({ });
  
  unguarded((: restore_object, "/d/"+ domain + "/loader" :));
  if (!pre_load)
    pre_load = ({ });
  return pre_load;
}
