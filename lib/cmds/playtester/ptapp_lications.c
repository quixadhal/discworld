/**
 *  PT Applications command
 *  @author Drakkos
 *  @started 27/08/2002
 *
 */


inherit "/cmds/base";
#include <playtesters.h>

string query_applicant_text();

varargs int cmd(string check, string name) {

  string ret = "";
  mixed args;
  
  switch (check) {
    case "comments":
      if (!sizeof (name)) {
        ret = PT_APPLICATION_HANDLER->get_comment_list();
      }
      else {
        ret = PT_APPLICATION_HANDLER->get_comments_on_applicant (name);
      }
    break;      
    break;    
    case "summary":
    case "list":
      ret = "";
      ret =   PT_APPLICATION_HANDLER->get_applicant_list();
    break;         
  }   
  
  tell_object (this_player(), ret);
  return 1;
}
       
  

mixed *query_patterns() {
   mixed *patterns = 
    ({ 
      "list", (: cmd("list") :),
    });
   
  if (this_player()->query_lord() || 
    load_object ("/d/playtesters/master")->query_deputy 
    (this_player()->query_name())) {      
    patterns += 
    ({ 
      "comments",  (: cmd ("comments") :),
      "comments for <string>", (: cmd ("comments", $4[0]) :),
    });
  }    
    return patterns;     
} /* query_patterns() */       
