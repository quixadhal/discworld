/**
 *  Playtester interface to the project handler system
 *  @author Drakkos
 *  @started ah... I forget.
 *
 */
 
#include <project_management.h>
#include <playtesters.h>

inherit "/cmds/base";

string format_array (string*);
string get_output (class project*, int);

mapping _globals = ([ ]);
varargs int cmd(string bing, string bong, string proj) {
  class project temp, *tmp = ({ });
  mapping query = ([ ]);
  string *blue, *blah;
  string ret = "";  
  int verbose = 0, i;
  
  if (!PLAYTESTER_HAND->query_tester (this_player()->query_name())) {
    this_player()->add_failed_mess (this_object(), "Only playtesters may "
      "use this command.\n", ({ }));
    return 0;
  }
  
  if (bing == "list" && sizeof (bong)) {
    if (find_object ("/d/" + lower_case (bong) + "/master")) {
      bing = "domain";
    }
    else if (PLAYTESTER_HAND->query_tester (lower_case (bong))) {
      bing = "playtester";
    }
    else {
      bing = "nomatch";
    }
    
  }
         
               
  switch (bing) {
    case "assignments":      
      bong = lower_case (bong);
      blue = PROJECT_HANDLER->query_pt_projects (bong);
      ret = sprintf (capitalize (bong) + " $I$5=%s.\n", (sizeof (blue) ? "is "
      "assigned to the following projects: " + query_multiple_short (blue) :
      "is not currently assigned to any projects"));
    break;      
    case "allassign":
      blue = sort_array (PLAYTESTER_HAND->query_playtesters(), 1);
      foreach (string b in blue) {
        blah = PROJECT_HANDLER->query_pt_projects (b);
        ret += sprintf ("$I$5=%s is assigned to %s.\n",
          capitalize (b), 
          (sizeof (blah) ? query_multiple_short (blah) : "nothing"));
      }      
    case "list":
      query["status"] = "playtesting";            
      tmp = PROJECT_HANDLER->filter_by_query (query);
    break;
    case "domain":
      query["status"] = "playtesting";    
      query["domains"] = lower_case (bong);        
      tmp = PROJECT_HANDLER->filter_by_query (query);      
    break;
    case "playtester":
      query["status"] = "playtesting";    
      query["playtesters"] = lower_case (bong);        
      tmp = PROJECT_HANDLER->filter_by_query (query);
    break;
    case "details":
    case "report":
    case "assign":
    case "unassign":
      query["status"] = "playtesting";
      query["projid"] = bong;
      
      temp = PROJECT_HANDLER->get_project_file (bong);

      if (!temp) {
        this_player()->add_failed_mess (this_object(), "This project is "
          "not in the handler.  Remember case sensitivity.\n", ({ }));
        return 0;
      }


      if (temp->status != member_array ("playtesting", STATUS)) {
        this_player()->add_failed_mess (this_object(), "This project is "
          "not in playtesting.\n", ({ }));
        return 0;
      }
      
      if (temp) {
        tmp += ({ temp });
      }
      
      verbose = 1;
    break;
    case "passign":
    case "punassign":
       temp = PROJECT_HANDLER->get_project_file (proj);
       if (temp) {
        tmp += ({ temp });
      }
      
    break;     

  }

  
  if (!sizeof (tmp) && !sizeof (ret)) {
    ret += "There are no projects in playtesting for this category.\n";
  }
  else {    
    switch (bing) {
      case "passign":
        blue = PROJECT_HANDLER->add_playtester (proj, ({ bong }));
        
        if (!sizeof (blue)) {
          ret = "You cannot perform this operation.";
        }
        else {
          ret = "You assign " + capitalize (bong) + " to project " +
            proj + ".\n";
        }        
      break;      
      case "punassign":
        i = PROJECT_HANDLER->remove_playtester (proj, ({ bong }));
        
        if (!i) {
          ret = "This project does not exist.";
        }
        else {
          ret = "You remove " + capitalize (bong) + " from project " +
            proj + ".\n";
        }        
      break;          
      break;
      case "list":
      case "domain":
      case "playtester":
      case "details":
        ret = get_output (tmp, verbose);
      break;
      case "report":
      tell_object (this_player(), "Please enter your report for project " +
        bong + ".\n");
      this_player()->do_edit(0, "end_of_report");
      _globals [this_player()->query_name()]  = bong;
      break;  
      case "assign":
      if (!PLAYTESTER_HAND->query_exec_access 
        (this_player()->query_name())) {
        this_player()->add_failed_mess (this_object(), "Only PT "
          "execs can assign themselves to projects.\n", ({ }));
        return 0;
      }
      else {
        PROJECT_HANDLER->add_playtester (bong, 
          ({ this_player()->query_name() }), this_player()->query_name());    
        ret += "You assign yourself to the project " + bong;    
      }
      break;
      case "unassign":
      if (!PLAYTESTER_HAND->query_exec_access
        (this_player()->query_name())) {
        this_player()->add_failed_mess (this_object(), "Only PT "
          "Execs can unassign themselves from projects.\n", ({ }));
        return 0;
      }
      else {
        PROJECT_HANDLER->remove_playtester (bong, 
          ({ this_player()->query_name() }), this_player()->query_name());        
        ret += "You unassign yourself from the project " + bong;    
      }
      break;
        
    }
  }
  
  if (sizeof (ret)) {
    this_player()->more_string (ret);
  }
    
  return 1;
} /* cmd() */

mixed *query_patterns() {
    mixed *patterns = ({ "list", (: cmd("list") :),
    "list <word>", (: cmd ( "list", $4[0]) :),
    "details <word>", (: cmd ( "details", $4[0]) :),
    "report <word>", (: cmd ( "report", $4[0]) :), 
    "assign <word>", (: cmd ("assign", $4[0]) :),
    "unassign <word>", (: cmd ("unassign", $4[0]) :),
    "assignments [for] <word>", (: cmd ("assignments", $4[0]) :),
    "assignments", (: cmd ("allassign") :),
    });

    if (PLAYTESTER_HAND->query_exec_access (this_player()->query_name())) {
      patterns += ({ 
        "unassign <word> from <word>", (: cmd ("punassign", $4[0], $4[1]) :),
        "assign <word> to <word>", (: cmd ("passign", $4[0], $4[1]) :),
      });
    }
    
    return patterns;
  
} /* query_patterns() */

string format_array (string *tmp) {
  
  if (!sizeof (tmp)) {
    return "None";
  }
  
  return query_multiple_short (map (tmp, (: capitalize ($1) :)));
}

string get_output (class project *tmp, int verbose) {
  int i;
  string ret = "";
  if (sizeof (tmp)) {
    if (!verbose) {
      ret += "The following projects match this query:\n";
      ret += sprintf ("%-20s %-40s %s\n\n", "Project ID", 
          "Name", "Domains");    
      foreach (class project p in tmp ) {
        ret += sprintf ("%-20s %-40s %s\n", p->id, 
          p->name, format_array (p->domains)); 
      }
    }
    else {
      ret += "The details for this project are:\n\n";
      foreach (class project p in tmp ) {

        i = PROJECT_HANDLER->find_project (p->id);
        
        ret += sprintf ("$I$5=Project ID: %s\n$I$5=Project Name: %s\n$I$5="
        "Project Leader: %s\n$I$5=Description: %s\n$I$5=Domains: %s\n$I$5="
        "Creators: %s\n$I$5=Assigned Playtesters: "
        "%s.\n$I$5=Size: %s.\n$I$5=Complexity: %s.\n$I$5=Next Rotation: %s\n"
        "$I$5=Rotation: %d of %d.\n"
        "$I$5=Guilds: %s.\n"
        "$I$5=Playtester notes: "
        "%s\n$I$5=Flagged for playtesting: %s", 
        p->id, p->name, capitalize (p->leader), 
        replace (p->description, ({"\n\n", "\n"})), 
        format_array (p->domains), format_array (p->creators), 
        format_array (p->playtesters), (p->size != -1 ? SIZE[p->size] : 
        "Unknown"), 
        (p->complexity != -1 ? COMPLEXITY[p->complexity] : "Unknown"), 
        (PROJECT_HANDLER->query_number_rotations() > p->current_rotation ? 
        ctime (PROJECT_HANDLER->next_rotation_time (i)) : "On last rotation"),
        PROJECT_HANDLER->query_rotation (i), 
        PROJECT_HANDLER->query_number_rotations (i),
        (sizeof (p->guild_specific) ? query_multiple_short (p->guild_specific) :
        "All guilds"), 
        (sizeof (p->pt_notes) ? p->pt_notes : 
        "None"), ctime(p->last_status_change));        
      }
    }
  }
  
  return ret;    
    
}

void end_of_report (string body) {
  int i = 0, j;
  
  if (!sizeof (body)) {
    tell_object (this_player(), "No report given.  Aborting.\n");
    return;
  }    
  
  do { 
    i++;
    j = PROJECT_HANDLER->find_note (_globals[this_player()->query_name()],
      capitalize (this_player()->query_name()) + "Report" + i);
  } while (j != -1);
  
  PROJECT_HANDLER->add_note_to_project (_globals[this_player()->query_name()], 
    capitalize (this_player()->query_name()) + "Report" + i, body, 
    this_player()->query_name());
  PLAYTESTER_HAND->adjust_quota( this_player()->query_name(), 1 ); 
  map_delete (_globals, this_player()->query_name());   
  tell_object (this_player(), "Thank you for your report!\n");   
}
