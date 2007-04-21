/**
 *  PT Applications command
 *  @author Drakkos
 *  @started 27/08/2002
 *
 */


inherit "/cmds/base";
#include <playtesters.h>
#include <project_management.h>

string query_applicant_text();

varargs int cmd(string check, string name, string reason) {

  int i;
  string *names, *projects;
  string ret = "";
  class project *tmp, *tmp2;
  
  switch (check) {
    case "ptinfo":
      ret = PLAYTESTER_HAND->query_show_list_colour();
      tell_object (this_player(), ret);
      return 1;
    break;
    case "assign":
      i = PROJECT_HANDLER->find_project (name);
      
      if (i == -1) {
        this_player()->add_failed_mess (this_object(), "There is no project "
          "of that ID.\n", ({ }));
        return 0;
      }        
      else {
        names = PROJECT_HANDLER->assign_pts_to_project (i);
        
        if (!sizeof (names)) { 
          this_player()->add_failed_mess (this_object(), "Something went " 
            "wrong!\n", ({ })); 
            return 0; 
        }        
        else { 
          tell_object (this_player(), sprintf ("$I$5=You assign %s to project %s.\n", 
          query_multiple_short (map (names, (: capitalize ($1) :))), name)); 
        return 1; 
      } 
    }        
    break; 
    case "allassign": 
      tmp = PROJECT_HANDLER->get_playtesting_projects();
      
      tmp2 = filter (tmp, (: sizeof ($1->guild_specific) :));
      
      tmp -= tmp2;
      
      if (!sizeof (tmp) && !sizeof (tmp2)) {
          this_player()->add_failed_mess (this_object(), "There are no "
            "projects currently in playtesting.\n", ({ }));
          return 0;
      }
      else {
        
        for (int c = 0; c < 2; c++) {
          if (c == 0) {
            projects = map (tmp2, (: $1->id :));
          }
          else {
            projects = map (tmp, (: $1->id :));
          }
                    
          foreach (string p in projects) {
            names = PROJECT_HANDLER->assign_pts_to_project (p);
            if (!sizeof (names)) {
              continue;
            }
            ret += sprintf ("$I$5=You assign %s to project %s.\n", 
              query_multiple_short (map (names, (: capitalize ($1) :))), p);
          }
        }
                
        tell_object (this_player(), ret);
        return 1;          
    }
    break;          
    case "add":
      i = PLAYTESTER_HAND->add_playtester (name);
      
      if (i) {
        this_player()->add_succeeded_mess (this_object(), "$N add$s someone "
          "as a playtester.\n", ({ }));
        return 1;
      }
      else {
        this_player()->add_failed_mess (this_object(), "You cannot add this "
          "player as a playtester.\n", ({ }));
        return 0;
      }
    break;         
    case "remove":
      i = PLAYTESTER_HAND->remove_playtester (name, reason);
      
      if (i) {
        this_player()->add_succeeded_mess (this_object(), "$N remove$s someone "
          "as a playtester.\n", ({ }));
        return 1;
      }
      else {
        this_player()->add_failed_mess (this_object(), "You cannot remove this "
          "player as a playtester.\n", ({ }));
        return 0;
      }
    break;     
    case "shortlist":
            
      PT_APPLICATION_HANDLER->set_shortlist (name);
        this_player()->add_succeeded_mess (this_object(), "$N toggle$s "
          "something.\n", ({ }));
      return 1;
    break;          
    case "investigate":
        PT_APPLICATION_HANDLER->set_considering (name);
        this_player()->add_succeeded_mess (this_object(), "$N toggle$s "
          "something.\n", ({ }));
        return 1;
    break; 
    case "delete":
      i =  PT_APPLICATION_HANDLER->delete_applicant (name);
            
      if (i) {
        this_player()->add_succeeded_mess (this_object(), "$N delete$s someone "
          "as from the application process.\n", ({ }));
        return 1;
      }
      else {
        this_player()->add_failed_mess (this_object(), "You cannot remove this "
          "player from the application process.\n", ({ }));
        return 0;
      }
    break;
    case "holiday":
      i =  PLAYTESTER_HAND->query_leave(name);
            
      if (i) {
        PLAYTESTER_HAND->reset_leave(name);
        this_player()->add_succeeded_mess (this_object(), "$N reset$s someones "
          "playtester leave.\n", ({ }));
        return 1;
      }
      else {
        PLAYTESTER_HAND->set_leave(name, reason);
        this_player()->add_succeeded_mess (this_object(), "$N set$s someone "
          "as on leave.\n", ({ }));
        return 1;
      }
    break;               
  }   
  
}
       
  

mixed *query_patterns() {
   mixed *patterns = 
    ({ 
      "add <word>", (: cmd ("add", $4[0]) :),
      "remove <word'name'> <string'reason'>", (: cmd ("remove", $4[0], $4[1]) :),
      "holiday <word> <string>", (: cmd ("holiday", $4[0], $4[1]) :),
      "shortlist <word>", (: cmd ("shortlist", $4[0]) :),
      "investigate <word>", (: cmd ("investigate", $4[0]) :), 
      "delete <word>", (: cmd ("delete", $4[0]) :),
      "assign all", (: cmd ("allassign", "all") :),
      "assign <word>", (: cmd ("assign", $4[0]) :),         
      "info", (: cmd ("ptinfo") :) 
      });
    
  return patterns;     
} /* query_patterns() */       