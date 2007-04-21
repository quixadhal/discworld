/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: loader.c,v 1.1 1998/01/06 04:03:45 ceres Exp $
 * $Log: loader.c,v $
 * Revision 1.1  1998/01/06 04:03:45  ceres
 * Initial revision
 * 
*/
/**
 * This is the domain preloader that handles preloading important files for
 * the domains.
 *
 * When the mud boots this object loads each item in each domains preload
 * list. It will only load files which are within the domain that is being
 * preloaded. In other words you cannot have the AM loader load
 * /guilds/Ankh-Morpork/myfile this must be done in the guilds pre-loader.
 *
 * Functions are provided to enable domain leaders and deputies to add and
 * remove files from the domains preload list.
 *
 * @see add_pre_load, remove_pre_load, query_pre_load
 */

string *pre_load;

/** @ignore yes */
void create() {
  int i;
  string domain;
  
  foreach(domain in "/secure/master"->query_domains()) {
    unguarded((: restore_object, "/d/"+ domain + "/loader" :));
    if (!pre_load)
      pre_load = ({ });
    for (i=0;i<sizeof(pre_load);i++) {
      if(pre_load[i] && explode(pre_load[i], "/")[1] != domain)
        printf("invalid " + domain + " preload: " + pre_load[i] +
               "\n");
      else {
        printf(domain +" pre_loading "+pre_load[i]+".\n");
        if(catch(call_other(pre_load[i], "??")))
          call_out("do_load", 0, pre_load[i]);
      }
    }
  }
}

/** @ignore yes */
void do_load(string str) {
  call_other(str, "??");
}

/** @ignore yes */
private int validate(string domain, object who) {
  string name;
  
  if(member_array(domain, "/secure/master"->query_domains()) == -1)
    return 0;

  if(!userp(who) || !interactive(who))
    return 0;
  
  name = who->query_name();
  
  if(!!"/secure/master"->query_trustee(name) &&
     !"/secure/master"->is_leader_of(name, domain) &&
     !"/secure/master"->is_deputy_of(name, domain))
    return 0;

  return 1;
}

/**
 * This method adds a file to a domains preload list.
 *
 * @param domain The domain
 * @param str The basename of the file to be added.
 * @return 1 for success, 0 for failure.
 */
int add_pre_load(string domain, string str) {
  if(!validate(domain, previous_object(1)))
    return 0;

  if(explode(str, "/")[1] != domain)
    return 0;
  
  unguarded((: restore_object, "/d/"+ domain + "/loader" :));
  if (!pre_load)
    pre_load = ({ });
  
  if (member_array(str, pre_load) == -1)
    pre_load += ({ str });
  
  save_object("/d/" + domain + "/loader");
  return 1;
}

/**
 * This method removes a file from a domains preload list.
 *
 * @param domain The domain
 * @param str The basename of the file to be removed.
 * @return 1 for success, 0 for failure.
 */
int remove_pre_load(string domain, string str) {
  int i;

  if(!validate(domain, previous_object(1)))
    return 0;
    
  unguarded((: restore_object, "/d/"+ domain + "/loader" :));
  if (!pre_load)
    pre_load = ({ });

  if ((i=member_array(str, pre_load)) == -1)
    return 0;
  pre_load = delete(pre_load, i, 1);
  save_object("/d/" + domain + "/loader");
}

/**
 * This method returns the domains preload list.
 *
 * @param domain The domain
 * @return A list of files to be preloaded
 */
string *query_pre_load(string domain) {
  if(member_array(domain, "/secure/master"->query_domains()) == -1)
    return ({ });
  
  unguarded((: restore_object, "/d/"+ domain + "/loader" :));
  if (!pre_load)
    pre_load = ({ });
  return pre_load;
}
