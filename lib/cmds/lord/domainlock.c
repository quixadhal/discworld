/**
 * Domain lock command.
 * For those who are in control(tm).
 * <p>
 * @author Taffyd
 */

#include <applications.h>

inherit "/cmds/base";

int domain_check() {
  mapping map = APPLICATIONS_HANDLER->query_domain_status();
  string ret = "";
  
  foreach (string s, int o in map) {
    ret += capitalize (s) + " is " + (o ? "open" : "closed") + " to "
      "applications and is managed by " + 
      capitalize (APPLICATIONS_HANDLER->query_responsibility (s)) + ".\n"; 
  }
  
  this_player()->more_string (ret);
  return 1;
  
} /* domain_check() */

int cmd( string domain, int status ) { 
  int i = APPLICATIONS_HANDLER->query_domain_status()[domain];
  
  if (i == -1) {
    this_player()->add_failed_mess (this_object(), "You do not have "
      "permission to close the " + domain + " domain.\n");
    return 0;
  }
  
  if (status != i) {
    APPLICATIONS_HANDLER->toggle_domain_lock (domain);
    this_player()->add_succeeded_mess (this_object(), "$N furtively "
      "toggle$s something.\n", ({ }));
    return 1;
  }

  this_player()->add_failed_mess (this_object(), "Your domainlock is "
    "already set to that.\n", ({ }));  
  return 0;
} /* cmd() */

mixed *query_patterns() { 
    return ({ "<string'domain'> on", (: cmd( $4[0], 0 ):),
      "<string'domain'> off", (: cmd( $4[0], 1  ):),
      "list", (: domain_check() :) });
} /* query_patterns() */
