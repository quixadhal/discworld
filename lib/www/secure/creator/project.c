/**
 *  Creator Project Management System Web Interface
 *  @author Drakkos (god forgive me)
 *  @started 26/08/2001
 *
 */
 
 
#include <http.h>
#include <player_handler.h>
#include <project_management.h>
#include <permissions.h>

string get_project_menu();
varargs string get_add_project(mapping);
string get_web_page (string, string);
string www_project_info (class project);
varargs string get_modify_project (string, string, string);
varargs string get_add_note (mapping, string);
string get_add_task (mapping, string);
string do_add_file (mapping, string); 
string get_modify_task (string, string);
string get_add_file (mapping, string);
string get_modify_note (string, string);
string get_modify_file (string, string, int);
string get_notes_list (string*);
string get_notes_summary (string);
string get_tasks_summary (string);
string get_view_project (string, int);
string get_view_page (string, string, string);
string get_view_note (string, string);
string get_files_summary (string);



string get_id(string str) {
  return implode (map (explode (str, " "), (: capitalize ($1) :)),
    "");   
}

varargs string *explode_to_array (string str, int cap) {
  string *arr;
  arr = explode (replace (str, ({" ", ""})), ",");

  if (!cap) {
    arr = map (arr, (: lower_case ($1) :));
  }
  
  return arr;
}  
  
string* normalise_guilds (string str) {
  string *valid_guilds = ({"wizards", "witches", "priests", "warriors",
    "thieves", "assassins"});  
  string *good = ({ });
  string *start;
  
  start = explode_to_array (str);
  
  foreach (string s in start) {
    if (member_array (s, valid_guilds) == -1) {
      s = s + "s";
    }
    if (member_array (s, valid_guilds) != -1 && 
        member_array (s, good) == -1) {
      good += ({ s });
    }
  }
      
  return good;
}


/**
 *  Take the type of timescale and the number, and multiply them together
 *  to give a time value estimate.
 *  @param number the number of units
 *  @param value the type of units
 *  @returns the number of seconds until the estimated completion date.
 */
 
int get_time_value (string number, string value) {
  int est, time;
  
  est = to_int (number);
  
  switch (value) {
    case "day":
      time = DAY;
    break;
    case "week":
      time = DAY * 7;
    break;
    case "month":
      time = DAY * 30;
    break;
    case "year":
      time = DAY * 365;
    break;
  }
  
  est *= time;

  return est;
}

string* get_twiki_links (class project p) {
  string *ret = ({ });
  string *details;
  string domain, page;
  if (!sizeof (p->twiki)) {
    return ret;
  }
  
  foreach (string t in p->twiki) {
    details = explode (t, ".");
  
    if (sizeof (details) == 2) {
      domain = details[0];
      page = details[1];
    }
    else {
      domain = p->domains[0];
      page = t;
    }

    ret += ({ "<a href=" + TWIKI_BASE + capitalize (domain) + "/" + page + ">"
     + page + "</a>"});
    
  }
  
  return ret;    
}

string* get_domain_links (class project p) {
  string *ret = ({ });
  if (!sizeof (p->domains)) {
    return ret;
  }
  
  foreach (string d in p->domains) {
    ret += ({ "<a href=project.c?misc=Domlist&domain=" + d + ">"
      + capitalize (d) + "</a>" });
  }
  
  if (!sizeof (ret)) {
    ret = ({"Error!"});
  }
  
  
  return ret;    
}

string *get_creator_links (mixed cre) {
  string *creators;
  string *ret = ({ });
  
  if (arrayp (cre)) {
    creators = cre;
  }
  else {
    creators = ({ cre });
  }
  
  creators = sort_array (creators, 1);
  
  foreach (string c in creators) {
    ret += ({ "<a href=project.c?misc=Crelist&creator=" + c + ">"
        + capitalize (c) + "</a>" });
  }
      
  return ret;
  
}


string parse_details (class project *tmp) {
  string ret = "";

  if (!sizeof (tmp)) {
    return "There are no projects in this category.<P>";
  }

  
  ret += "<ol>";
  
  foreach (class project t in tmp) {
    ret += "<li>";
    ret += "<TABLE width=100%>";
    ret += "<TD width=80%>";      
    ret += sprintf ("<B>%s:</b>  %s (%s).<br>%s<br>",
      query_multiple_short (map (t->domains, (: capitalize ($1) :))), t->id, 
      t->name, (sizeof (t->creators) ? 
      "Assigned creators: " + query_multiple_short (get_creator_links 
      (t->creators)) + "." : "<b>Unassigned</b>"));
    if (t->last_touched) {
      ret += "  Last touched: " + ctime (t->last_touched) + " by " + 
      (t->touched_by ? query_multiple_short (get_creator_links 
      (t->touched_by)) : "Nobody");
    }
    else {
      ret += "  This project has not been touched since creation.";
    }
    
    ret += "<TD>";      
               
    ret += "<b>[ </b><a href=project.c?projecttype=Details&pid=" 
      + t->id + ">Details</a> <b>|</b> "
      "<a href=project.c?pid=" + 
      t->id + "&projecttype=Edit>Edit</a> <b>]</b>" 
      "<TR>";
    ret += "</TABLE>";

  }
  
  ret += "</ol>";
    
  return ret;
}

string get_project_list() {
  class project *tmp = PROJECT_HANDLER->data_dump();
  
  return parse_details (tmp);;
  
}


string get_domain_project_list(string domain) {
  class project *tmp = PROJECT_HANDLER->filter_by_query((["domains" : 
    domain]));
  
  return parse_details (tmp);;
  
}

string get_creator_project_list(string creator) {
  class project *tmp = PROJECT_HANDLER->filter_by_query((["creators" : 
    creator]));
  
  return parse_details (tmp);;
  
}

/**
 *  Get the HTML code for the estimated completion data drop down menus.
 *  @returns the HTML code.
 */
 
varargs string get_estimate(int ignore) {
  string ret = "<SELECT NAME=\"number\">";
  
  for (int i = 0; i < 13; i++) {
    ret += "<OPTION VALUE + \"" + i + "\">" + i;
  }
  
  ret += "</SELECT>";    
  
  ret += "<SELECT NAME=\"time\">";
    
  foreach (string stat in ({"day", "week", "month", "year"}))  {   
    ret += "<OPTION value = \"" + stat+ "\">" + stat;
  }  
  
  ret += "</SELECT>";
  
  if (!ignore) {
    ret += "<INPUT type = \"checkbox\" name = \"newest\"> Update?";
  }
  
  return ret;
}

string get_tasks_summary (string id) {
  class task *tmp = PROJECT_HANDLER->project_task_data(id);
  string ret = "";
  

  ret += "<HR>"
    "<B>Tasks: </B>" 
    "<b>[</b> <a href=project.c?pid=" + id + "&tasktype=Add>"
    "Add New Task</a> <b>]</b><p>";
  
  if (sizeof (tmp)) {
    ret += "<ol>";
      
    foreach (class task t in tmp) {
      ret += "<li><TABLE width=100%>";
      ret += "<TD width=70%>";      
      ret += sprintf ("<b>%s (%s) [%d%%] </b><br>%s<br>Added by %s on %s",
        t->id, t->task_name, to_int (t->percent_complete), (sizeof (t->assigned_to) ? 
          "Assigned creators: " + query_multiple_short (
          get_creator_links (t->assigned_to)) :
          "<b>Unassigned</b>"), query_multiple_short (get_creator_links 
            (t->added_by)), 
          ctime (t->date_added));
      ret += "<TD>";
      ret += "<b>[</b> <a href=project.c?tid=" + t->id + 
        "&tasktype=Details&pid=" 
        + id + ">Details</a> | "
        "<a href=project.c?tid=" + t->id + "&tasktype=Edit&pid=" + id +">"
      "Edit</a> <b>]</b>";
      ret += "<TR>";
      ret += "</TABLE>";
  
    }
    
    ret += "</ol>";
  }
    
  return ret;
}  


varargs string get_view_task (string id, string task) {
  string ret = "";
  class task tmp;

  ret += get_view_project (id, 0);
  
  
  tmp  = PROJECT_HANDLER->get_task_file (id, task);
    
  if (!tmp) {
    return "";    
  }
  
  ret += "<HR>"
      "<B>Task " + task+ ":  </B><a href=project.c?tid=" 
      + tmp->id + "&tasktype=Edit&pid=" 
      + id + ">Edit Task</a><P> "
    "<form action=\"project.c\" METHOD=POST "
    "action=\"project.c?\">"
    "<TABLE width=100%>"
    "<TR>"
    "<TH width=30% align = \"left\"> Task ID: </TH>" 
    "<TD>" + tmp->id + "<TD>"  
    "<TR>"
    "<TH align = \"left\"> Task Name: </TH>" 
    "<TD>" + tmp->task_name + "<TD>"  
    "<TR>"
    "<TH align = \"left\"> Added by: </TH>" 
    "<TD>" + query_multiple_short (get_creator_links(tmp->added_by)) + " at " 
      + ctime (tmp->date_added) 
      + "<TD>"  
    "<TR>"
    "<TH align = \"left\"> Comments: </TH>" 
    "<TD>" + (tmp->comments? tmp->comments : "") + "<TD>"  
    "<TR>"
    "<TH align = \"left\"> Project creators (separated by comma): "
    "</TH>" 
    "<TD>";
    if (sizeof (tmp->assigned_to)) {      
      ret += query_multiple_short (get_creator_links 
        (tmp->assigned_to));  
    }
  ret += "<TD><TR>"
    "<TH align = \"left\"> Percent Complete: </TH>" 
    "<TD>" + tmp->percent_complete+ "<TD>"  
    "<TR>"
    "<TH align = \"left\"> Task Aims: </TH>" 
    "<TD>" + tmp->aims + "<TD>";
    if (tmp->completion_date) {
    ret += "<TR><TH align = \"left\"> Estimated completion: </TH>" 
    "<TD>" + ctime (tmp->completion_date) + "<TD>"  
    "<TR>";
  }    
    
  ret += "</TABLE>";

  return ret;
}

varargs string get_view_file (string id, string file, string task) {
  string ret = "";
  class file tmp;
  
  ret += get_files_summary (id);
  
  tmp  = PROJECT_HANDLER->get_file (id, file, task);
    
  if (!tmp) {
    return "";    
  }
  
  ret += "<HR>"
      "<B>Access for " + file + ":  </B><a href=project.c?filetype=Delete&pid=" 
      + id + "&fid="+file + "&task=" + task + ">Delete this File</a><P> "
    "<form action=\"project.c\" METHOD=POST "
    "action=\"project.c?\">"
    "<TABLE width=100%>"
    "<TR>"
    "<TH width=30% align = \"left\"> Filename: </TH>" 
    "<TD>" + tmp->file + "<TD>"  
    "<TR>"
    "<TH align = \"left\"> File Access: </TH>" 
    "</SELECT>"    
    "<SELECT NAME=\"access\">";
    
    foreach (string stat in ({"read", "read+write", "read+write+grant"}))  {   
      ret += "<OPTION value = \"" + stat+ "\">" + stat;
    }  
  
    ret += "</SELECT>"    
    "<TD>"  
    "<TR>"
    "<TH align = \"left\"> Leader Only: </TH>" 
    "<SELECT NAME=\"leader\">";
    
    foreach (string stat in ({"yes", "no"}))  {   
      ret += "<OPTION value = \"" + stat+ "\">" + stat;
    }  
    ret += "</SELECT>"  
    "</TABLE>";

  return ret;
}

string get_files_summary (string id) {
  class file *tmp = PROJECT_HANDLER->query_files(id);
  string ret = "";
  string acc_str, cohort;
  
  ret += "<HR>"
    "<B>Files: </B>" 
    "<b>[</b><a href=project.c?pid=" + id + "&filetype=Add>"
    "Add New File Permission</a><b>]</b><P>";
             
  if (sizeof (tmp)) {             
    ret += "<ol>";        
     foreach (class file t in tmp) {
        switch (t->access) {
          case 4:
            acc_str = "read+write+grant";
          break;
          case 2:
            acc_str = "read+write";
          break;
          case 1:
            acc_str = "read";
          break;
        }
        
      if (t->task == "leader") {
        cohort = " the project leader";
      }
      else if (sizeof (t->task)) {
        cohort = " members assigned to task " + t->task;
      }
      else {
        cohort = " project members";
      }
      

      ret += "<li><TABLE width=100%>";
      ret += "<TD width=70%>";      
      ret += sprintf ("<b>%s</b>:  %s access for %s.",
        t->file, acc_str, cohort);
      ret += "<TD>";
      ret += "<b>[ </b>"
        "<a href=project.c?fid=" + t->file + "&filetype=Delete&pid=" + id 
        +"&task=" + t->task + ">"
      "Delete</a> <b>]</b>";
  
      ret += "<TR>";
      ret += "</TABLE>";
    }
    
    ret += "</ol>";

  }
  
  return ret;
}  

string get_notes_summary (string id) {
  class note *tmp = PROJECT_HANDLER->project_note_data(id);
  string ret = "";
  

  ret += "<HR>"
    "<B>Notes: </B>" 
    "<b>[</b><a href=project.c?pid=" + id + "&notetype=Add>"
    "Add New Note</a><b>]</b><P>";
             
  if (sizeof (tmp)) {             
    ret += "<ol>";
    
    foreach (class note t in tmp) {
      ret += "<li><TABLE width=100%>";
      ret += "<TD width=70%>";      
      ret += sprintf ("<b>%s</b><br>  Added by %s %s.",
        t->id, query_multiple_short (get_creator_links (t->added_by)), 
          ctime (t->date_added));
      ret += "<TD>";
      ret += "<b>[</b><a href=project.c?nid=" + t->id + 
        "&notetype=Details&pid=" 
        + id + ">Details</a> | "
        "<a href=project.c?nid=" + t->id + "&notetype=Edit&pid=" + id +">"
      "Edit</a> <b>]</b>";
  
      ret += "<TR>";
      ret += "</TABLE>";
    }
    
    ret += "</ol>";

  }
  
  return ret;
}  
/**
 *  Deal with updaing a project's task and returning the appropriate 
 *  web page.  It will check for consistency of data and the neccessary
 *  updating of the project management PROJECT_HANDLER.  It will only do this 
 *  if the user has valid access to perform the operation.
 *  @param data the task data
 *  @param name the name of the user.
 *  @returns the HTML code for the page
 */
  
protected string do_update_task(mapping data, string name) {
  string *required = ({"taskname"});
  string *creators;  
  class task tmp;
  int valid;
  
  foreach (string r in required) {
    if (undefinedp (data[r])) {
      return get_web_page (get_modify_task(data["projid"], 
      data["tid"]), "The " + r + " field is required.");
    }
  }
  
  data["projid"] = get_id (data["projid"]);
  data ["tid"] = get_id (data["tid"]);
  
  if (PROJECT_HANDLER->valid_operation (name, TASK, data["projid"])) {
      valid = 1;
  }
  
  if (!valid) {
    return get_web_page (get_modify_project(data["projid"]), 
        "You do not have access to perform this operation.");
  }    
   
  tmp = PROJECT_HANDLER->get_task_file (data["projid"], data["tid"]);   
  
  if (!tmp) {
      return get_web_page (get_modify_task(data["projid"], 
        data["tid"]), "Oh no, error!");
  }
  
  if (sizeof (data["assigned"])) {
    creators = filter (explode_to_array (data["assigned"]), 
      (: PLAYER_HANDLER->test_creator ($1) :));
  }
  if (tmp->task_name != data ["taskname"]) {
    tmp->task_name = data["taskname"];
  }
  if (tmp->comments != data ["comments"]) {
    tmp->comments = data["comments"];
  }

  if (tmp->percent_complete != data ["completed"]) {
    tmp->percent_complete = to_int (data["completed"]);
  }
    
  if (tmp->assigned_to != creators) {
    if (sizeof (creators)) {
      tmp->assigned_to = map (creators, (: lower_case ($1) :));
    }
    else {
      tmp->assigned_to = ({ });
    }
    
  }
  
  if (tmp->aims != data["aims"]) {
    tmp->aims = data ["aims"];
  }
  
  if (data ["newest"] == "on") {
    tmp->completion_date = time() + get_time_value (data["number"], 
      data["time"]);
  }
    
  PROJECT_HANDLER->set_task_file (data["projid"], data ["tid"], tmp, name);  
  
  return get_view_page (get_view_task (data["projid"], data["tid"]), 
    data["projid"], "Task modifed");      
}    

/**
 *  Deal with updating a project's note and returning the appropriate 
 *  web page.  It will check for consistency of data and the neccessary
 *  updating of the project management PROJECT_HANDLER.  It will only do this 
 *  if the user has valid access to perform the operation.
 *  @param data the note data
 *  @param name the name of the user.
 *  @returns the HTML code for the page
 */
 
protected string do_update_note(mapping data, string name) {
  string *required = ({"nid"}); 
  class note tmp;
  int valid;
  
  foreach (string r in required) {
    if (undefinedp (data[r])) {
      return get_web_page (get_modify_task(data["projid"], 
      data["nid"]), 
      "The " + r + " field is required.");
    }
  }

  data ["projid"] = get_id (data ["projid"]);
  data ["nid"] = get_id (data["nid"]);


  tmp = PROJECT_HANDLER->get_note_file (data["projid"], data["nid"]);   
  
  if (!tmp) {
      return get_web_page (get_modify_task(data["projid"], 
        data["nid"]), "Oh no, error!");
  }
  
  if (PROJECT_HANDLER->valid_operation (name, NOTE, data["projid"], 
      PROJECT_HANDLER->query_project_domains(data["projid"]))) {
      valid = 1;
  }
  
  if (!valid) {
    return get_web_page (get_modify_project(data["projid"]), 
        "You do not have access to perform this operation.");
  }    

  if (tmp->note != data ["note"]) {
    tmp->note = data["note"];
  }

    
  PROJECT_HANDLER->set_note_file (data["projid"], data ["nid"], tmp, name);    
      
  return get_view_page (get_view_note(data["projid"], data["nid"]), 
    data["projid"], "Note modifed");      
} 

/*
protected string do_update_file(mapping data, string name) {
  string *required = ({"file", "access", "lead"}); 
  class file tmp;
  int valid;
  
  foreach (string r in required) {
    if (undefinedp (data[r])) {
      return get_web_page (get_modify_file(data["projid"], 
      data["fid"], to_int (data["lead"])), 
      "The " + r + " field is required.");
    }
  }

  data ["projid"] = get_id (data ["projid"]);

  tmp = PROJECT_HANDLER->get_file (data["projid"], data["fid"], 
  to_int (data["lead"]));   
  
  if (!tmp) {
      return get_web_page (get_modify_file(data["projid"], 
        data["fid"], to_int (data["lead"])), "Oh no, error!");
  }
  
  if (PROJECT_HANDLER->valid_operation (name, FILE, data["projid"], 
      PROJECT_HANDLER->query_project_domains(data["projid"]))) {
      valid = 1;
  }
  
  if (!valid) {
    return get_web_page (get_modify_project(data["projid"]), 
        "You do not have access to perform this operation.");
  }    

  if (tmp->file != data ["file"]) {
    tmp->file = data["file"];
  }

  if (tmp->access != data ["access"]) {
    tmp->access = data["access"];
  }

  if (tmp->leader_only != data ["lead"]) {
    tmp->leader_only = data["lead"];
  }

    
  PROJECT_HANDLER->set_file (data["projid"], data ["fid"], data["access"],
  data ["leader"]);    
      
  return get_view_page (get_view_note(data["projid"], data["nid"]), 
    data["projid"], "Note modifed");      
} 
*/

/**
 *  Deal with updaing a project and returning the appropriate 
 *  web page.  It will check for consistency of data and the neccessary
 *  updating of the project management PROJECT_HANDLER.  It will only do this is
 *  the user has valid access to perform the operation.
 *  @param data the project data
 *  @param name the name of the user.
 *  @returns the HTML code for the page
 */
 
protected string do_update_project (mapping data, string name) {
  string *required = ({"projid", "name", "domains"});
  string *domains, *creators, *twiki, *sub, *pts;  
  class project tmp;
  int tmp_val, valid;
  
  foreach (string r in required) {
    if (undefinedp (data[r])) {
      return get_web_page (get_modify_project(data["projid"]), 
        "The " + r + " field is required.");
    }
  }

  data ["projid"] = get_id (data["projid"]);

  domains = filter (explode_to_array (data["domains"]), 
    (: find_object ("/d/" + $1 + "/master") :));
   
  if (PROJECT_HANDLER->valid_operation (name, PROJECT, data["projid"], 
      domains)) {
      valid = 1;
  }
  
  pts = explode_to_array (data["playtesters"]);

  tmp = PROJECT_HANDLER->get_project_file (data["projid"]);   

  if (sizeof (pts & tmp->playtesters) != sizeof (tmp->playtesters)) {
    if (PROJECT_HANDLER->valid_operation (name, PROJECT, data["projid"], 
      domains)) {
      valid = 1;
    }
  }
  
     
  if (!valid) {
    return get_web_page (get_modify_project(data["projid"]), 
        "You do not have access to perform this operation.");
  }  
  
  
  if (PROJECT_HANDLER->find_project (data["projid"]) == -1) {
      return get_web_page (get_modify_project(data["projid"]), 
        "The project " + data["projid"] + " does not exist.");
  }
   
  if (!sizeof (domains)) {
      return get_web_page (get_modify_project(data["projid"]), 
        "At least one valid domain required.");
  }

  creators = filter (explode_to_array (data["creators"]), 
    (: PLAYER_HANDLER->test_creator ($1) :));
     
  if (sizeof (data["twiki"])) {
    twiki = explode (replace (data["twiki"], ({" ", ""})), ",");
  }
  

  if (sizeof (data["sub"])) {
    sub = explode (replace (data["sub"], ({" ", ""})), ",");
  }      
  
  if (data["size"]) {
    tmp->size = member_array (data["size"], SIZE);    
  }
  if (data["complexity"]) {
    tmp->complexity = member_array (data["complexity"], COMPLEXITY);    
  }
  if (sizeof (data["guilds"])) {
    tmp->guild_specific = normalise_guilds (data["guilds"]);
      
  }
      
  if (tmp->name != data ["name"]) {
    tmp->name = data["name"];
  }
  if (tmp->pt_notes != data ["ptnotes"]) {
    tmp->pt_notes = data["ptnotes"];
  }
  if (tmp->leader != data ["leader"]) {
    tmp->leader = data["leader"];
  }
  
  if (tmp->description != data ["description"]) {
    tmp->description = data ["description"];
  }  

  if (tmp->percent_complete != data ["completed"]) {
    tmp->percent_complete = to_int (data ["completed"]);
  }  
  
  if (tmp->domains!= domains) {
    tmp->domains = domains;
  }
  
  if (tmp->creators != creators) {
    tmp->creators = map (creators, (: lower_case ($1) :));
  }

//  if (tmp->files != files) {
//    tmp->files = files;
//  }

  if (tmp->twiki != twiki) {
    tmp->twiki = twiki;
  }

  if (tmp->sub_projects != sub) {
    tmp->sub_projects = sub;
  }

  if (data["newest"] == "on") {
    tmp_val = get_time_value (data["number"], data["time"]);
    if (tmp_val > 0) {
      tmp->estimated_completion = (time() + tmp_val);
    }
    else {
      tmp->estimated_completion = 0;
    }    
  }

  if (STATUS[tmp->status] != data["status"] && 
    member_array (data["status"], STATUS) != -1 ) {
    tmp->status = member_array (data["status"], STATUS);
    PROJECT_HANDLER->catch_status_change (tmp->id, tmp->status);
  }

  PROJECT_HANDLER->assign_playtesters (data["projid"], pts, name);   
            
  PROJECT_HANDLER->set_project_file (data["projid"], tmp, name);    
      
  return get_view_page (get_view_project (data["projid"], 1), 
    data["projid"], "Project modifed");      
}  

/**
 *  Do a query on the data and return the results in a list. 
 *  @param query the query data
 *  @returns the HTML code for the page
 */
 
string web_query (mapping query) {
  class project *tmp;
  string ret = "";
  

  query ["touched"] = get_time_value (query["number"], query["time"]);
  
  tmp = PROJECT_HANDLER->filter_by_query (query);
  
  ret += "<B>" + capitalize (query_num (sizeof(tmp))) + 
  (sizeof (tmp) == 1 ? " project matches " : " projects match ") 
    + "this query: </B><P>";
  ret += parse_details (tmp);
  
  return ret;
}

protected string do_add_project (mapping data, string name) {
  string *required = ({"projid", "name", "domains"});
  string *domains, *creators, *twiki;  
  int valid = 0, size, complexity;

  foreach (string r in required) {
    if (undefinedp (data[r])) {
      return get_web_page (get_add_project(data), 
        "The " + r + " field is required.");
    }
  }
  
  data ["projid"] = get_id (data["projid"]);
  
  if (PROJECT_HANDLER->find_project (data["projid"]) != -1) {
      return get_web_page (get_add_project(data), 
        "The project " + data["projid"] + " already exists.");
  }
    
  domains = filter (explode_to_array (data["domains"]), 
    (: find_object ("/d/" + $1 + "/master") :));
   
  if (!sizeof (domains)) {
      return get_web_page (get_add_project(data), 
        "At least one valid domain required.");
  }
  
  if (PROJECT_HANDLER->valid_operation (name, PROJECT, data["projid"], 
      domains)) {
      valid = 1;
  }
  
  if (!valid) {
    return get_view_page (get_project_list (), 
      data["projid"], "You do not have access to perform this "
      "operation.");
  }

    
  creators = filter (explode_to_array (data["creators"]), 
    (: PLAYER_HANDLER->test_creator ($1) :));
     
  if (sizeof (data["twiki"])) {
    twiki = explode (replace (data["twiki"], ({" ", ""})), ",");
  }
  
  if (data["size"]) {
    size = member_array (data["size"], SIZE);
  }
  if (data["complexity"]) {
    complexity = member_array (data["complexity"], COMPLEXITY);
  }
  
  
  
//  if (sizeof (data["files"])) {
//    files = explode (replace (data["files"], ({" ", ""})), ",");
//  }
      
  
  PROJECT_HANDLER->add_project (data["projid"], data["name"], data["leader"],
    data["description"], domains, creators, name, size, complexity, 
    normalise_guilds (data["guilds"]));

  if (data["newest"] == "on") {
    PROJECT_HANDLER->set_completion_date(data["projid"], time() + 
      get_time_value (data["number"], data["time"]));
  }
    
          
  return get_view_page (get_view_project (data["projid"], 1), data["projid"], 
    "Project added.");      
}  

protected string do_add_note (mapping data, string name) {
  string *required = ({"nid", "note"});
  int valid;
  
  foreach (string r in required) {
    if (undefinedp (data[r])) {
      return get_web_page (get_add_note(data, data["projid"]), 
        "The " + r + " field is required.");
    }
  }
  
  data ["nid"] = get_id (data["nid"]);
  
  if (PROJECT_HANDLER->valid_operation (name, NOTE, data["projid"], 
      PROJECT_HANDLER->query_project_domains(data["projid"]))) {
      valid = 1;
  }
  
  if (!valid) {
    return get_web_page (get_modify_project (data["projid"]), 
        "You do not have access to perform this operation.");
  }

  if (PROJECT_HANDLER->find_project (data["projid"]) == -1) {
      return get_web_page (get_add_note(data, data["projid"]), 
        "The project " + data["projid"] + " does not exist.");
  }

  if (PROJECT_HANDLER->find_note (data["projid"], data["nid"]) != -1) {
      return get_web_page (get_add_note(data, data["projid"]), 
        "The task " + data["nid"] + " already exists.");
  }

  PROJECT_HANDLER->add_note_to_project (data["projid"], data["nid"], 
    data["note"], name);
                  
  return get_view_page (get_view_project (data["projid"], 1), data["projid"], 
    "Note Added.");      
}

protected string do_add_file (mapping data, string name) {
  string *required = ({"fid", "access", "task"});
  int valid, access, k;
  string task;
  object ob;
  
  foreach (string r in required) {
    if (undefinedp (data[r])) {
      return get_web_page (get_add_file(data, data["projid"]), 
        "The " + r + " field is required.");
    }
  }
  
  
  if (PROJECT_HANDLER->valid_operation (name, FILE, data["projid"], 
      PROJECT_HANDLER->query_project_domains(data["projid"]))) {
      valid = 1;
  }
  
  if (!valid) {
    return get_web_page (get_modify_project (data["projid"]), 
        "You do not have access to perform this operation.");
  }

  if (PROJECT_HANDLER->find_project (data["projid"]) == -1) {
      return get_web_page (get_add_note(data, data["projid"]), 
        "The project " + data["projid"] + " does not exist.");
  }
  
  if (data["task"] == "Leader only") {
    task = "leader";
  }
  else if (data["task"] == "All project members") {
    task = "";
  }
  else {
    task = data["task"];
  }
  
  if (sizeof (PROJECT_HANDLER->find_file (data["projid"], data["fid"], task))) {
      return get_web_page (get_add_file(data, data["projid"]), 
        "Permissions for this file and task  already exist.  Please "
        "remove if you wish to change the access rights.");
  }

  switch (data["access"]) {
    case "grant":
      access = GRANT_MASK;
    break;
    case "write":
      access = WRITE_MASK;
    break;
    default:
      access = READ_MASK;
    break;
  }

  ob = find_player (name);
    
  if (access == GRANT_MASK) {
  
    if (!ob) {
      return get_web_page (get_add_file(data, data["projid"]), 
        "You must be online to add grant access to a file.");
    }
  }
  
  k = master()->valid_grant(name, data["fid"], access);
  
  tell_creator ("drakkos", "%d.\n", k);
  if (!k) {
      return get_web_page (get_add_file(data, data["projid"]), 
        "You do not have valid access to add this file to the tracker.");
  }

  PROJECT_HANDLER->add_file (data["projid"], data["fid"], 
    access, name, task);
                  
  return get_view_page (get_view_project (data["projid"], 1), data["projid"], 
    "File Permission Added.");      
}

protected string do_add_task (mapping data, string name) {
  string *required = ({"tid", "taskname"});
  string *creators;
  int i, valid;
  
  foreach (string r in required) {
    if (undefinedp (data[r])) {
      return get_web_page (get_add_task(data, data["projid"]), 
        "The " + r + " field is required.");
    }
  }
  
  data ["tid"] = get_id (data["tid"]);

  if (PROJECT_HANDLER->find_project (data["projid"]) == -1) {
      return get_web_page (get_add_task(data, data["projid"]), 
        "The project " + data["projid"] + " does not exist.");
  }

  if (PROJECT_HANDLER->find_task (data["projid"], data["tid"]) != -1) {
      return get_web_page (get_add_task(data, data["projid"]), 
        "The task " + data["tid"] + " already exists.");
  }
  
  if (PROJECT_HANDLER->valid_operation (name, TASK, data["projid"])) {
      valid = 1;
  }
  
  if (!valid) {
    return get_view_page (get_view_task(data["projid"], 
      data ["tid"]), data["projid"], "You do not have access to "
      "perform this operation.");
  }  

  creators = filter (explode_to_array (data["creators"]), 
    (: PLAYER_HANDLER->test_creator ($1) :));
     
  PROJECT_HANDLER->add_task_to_project (data["projid"], data["tid"], name);
  
  if (sizeof (data["taskname"])) {
    PROJECT_HANDLER->set_task_name (data["projid"], data["tid"],
      data["taskname"]);
  }

  if (sizeof (data["aims"])) {
    PROJECT_HANDLER->set_task_aims (data["projid"], data["tid"],
      data["aims"]);
  }

  if (sizeof (data["comments"])) {
    PROJECT_HANDLER->set_task_comments (data["projid"], data["tid"],
      data["comments"]);
  }
   
  if (sizeof (creators)) {
    foreach (string c in creators) {
      PROJECT_HANDLER->add_task_creator (data["projid"], data["tid"],
        c);
    }      
  }

  i = to_int (data["completed"]);
  
  if (i > 0 && i <= 100) {
    PROJECT_HANDLER->set_task_completion (data["projid"], data["tid"],
      i);
  }
  
  if (sizeof(data["number"]) && data["newest"] == "on") {
   PROJECT_HANDLER->set_task_completion_date (data["projid"], data["tid"],
        time() + get_time_value (data["number"], data["time"]));
  }
  
                
    return get_view_page (get_view_task(data["projid"], 
      data ["tid"]), data["projid"], "Task Added");    
}

string get_view_project (string id, int verbose) {
  string ret = "";
  class project p;

//  verbose = 1;
    
  p = PROJECT_HANDLER->get_project_file (id);
    
  if (!p) {
    return "There is no project with that ID.\n";
  }  
  ret +="<B>Project " + id + ":  </B><b>[</b> <a href=project.c?pid=" + 
    p->id + "&projecttype=Edit>Edit Project</a> | " 
  "<a href=project.c?pid=" + p->id + "&projecttype=Details>"
    "Project Details</a><b> ]</b><P>" 
  "<B>" + PROJECT_HANDLER->query_touched (p->id) + "</B><P>";      

  ret += "<TABLE width=100%>"
  "<TR>"
  "<TH width=30% align = \"left\"> Project ID: </TH>"
  "<TD>" + p->id + "<TD>"
  "<TR>"
  "<TR>"    
  "<TH align = \"left\"> Project Name: </TH>" 
  "<TD>" + p->name + "<TD>"    
  "<TR>"    
  "<TH align = \"left\"> Project Domains: </TH>" 
  "<TD>" + query_multiple_short (get_domain_links(p)) 
    + "<TD>"   
  "<TR>"        
  "<TH align = \"left\"> Project Leader: </TH>" 
  "<TD>" + query_multiple_short (get_creator_links (p->leader)) + "<TD>";
  if (sizeof (p->creators)) {
    ret += "<TR>"    
    "<TH align = \"left\"> Project Creators: </TH>" 
    "<TD>" +  query_multiple_short (get_creator_links (p->creators)) 
      + "<TD>";
  }    
  
  if (verbose) {
    ret += "<TD>"        
    "<TR>"    
    "<TH align = \"left\"> Percent complete: </TH>" 
    "<TD>";  

    if (!p->percent_complete) {           
      ret += PROJECT_HANDLER->query_project_completeness (p->id);
    }
    else {
      ret += "" + p->percent_complete;
    }
    
    ret += "%";
       
    if (sizeof (p->playtesters)) {           
      ret += "<TD>"        
      "<TR>"    
      "<TH align = \"left\"> Assigned Playtesters: </TH>" 
      "<TD>";  
      ret += query_multiple_short (map (p->playtesters, 
        (: capitalize ($1) :)));   
    }   
    if (p->estimated_completion) {           
      ret += "<TD>"        
      "<TR>"    
      "<TH align = \"left\"> Estimated Completion Date: </TH>" 
      "<TD>";  
      ret += ctime (p->estimated_completion);
    }                
    if (p->description) {           
      ret += "<TD>"        
      "<TR>"    
      "<TH align = \"left\"> Description: </TH>" 
      "<TD>";  
      ret += p->description;
    }       

    if (p->size >= 0) {
      ret += "<TD>"        
      "<TR>"    
      "<TH align = \"left\"> Size: </TH>" 
      "<TD>";  
      ret += capitalize (SIZE[p->size]);
    }
    if (p->complexity >= 0) {
      ret += "<TD>"        
      "<TR>"    
      "<TH align = \"left\"> Complexity: </TH>" 
      "<TD>";  
      ret += capitalize (COMPLEXITY [p->complexity]);
    }
    if (sizeof (p->guild_specific)) {           
      ret += "<TD>"        
      "<TR>"    
      "<TH align = \"left\"> Guild Specific: </TH>" 
      "<TD>";  
      ret += query_multiple_short (map (p->guild_specific, 
        (: capitalize ($1) :)));   
    }       
    
    if (p->last_rotation) {
      ret += "<TD>"        
      "<TR>"    
      "<TH align = \"left\"> Last rotation: </TH>" 
      "<TD>";  
      ret += ctime(p->last_rotation);
    }      
      
    if (sizeof (p->pt_notes)) {           
      ret += "<TD>"        
      "<TR>"    
      "<TH align = \"left\"> Notes for Playtesters: </TH>" 
      "<TD>";  
      ret += p->pt_notes;
    }     
    ret += "<TD>"    
    "<TR>"    
    "<TH align = \"left\"> Currently in: </TH>" 
    "<TD>";  
    ret += capitalize (STATUS[p->status]);
    if (p->last_status_change) {           
      ret += "<TD>"        
      "<TR>"    
      "<TH align = \"left\"> Last Status Change: </TH>" 
      "<TD>";  
      ret += ctime (p->last_status_change);
    }    
    ret += "<TD>"
      "<TR>"               
      "<TH align = \"left\"> Added by: </TH>" 
      "<TD>";  
    ret += query_multiple_short (get_creator_links(p->added_by)) + " at " 
      + ctime (p->date_added);            
    if (sizeof (p->sub_projects)) {
      ret += "<TD>"
      "<TR>"    
      "<TH align = \"left\"> Subproject IDs: </TH>" 
      "<TD>";  
      ret += query_multiple_short (map (p->sub_projects, (: "<a href="
        "project.c?projecttype=Details&pid=" + $1 + ">" + $1 + "</a>":)));             
    }    
//    if (sizeof (p->files)) {
//      ret += "<TD>"
//      "<TR>"    
//      "<TH align = \"left\"> Project Files: </TH>" 
//      "<TD>";  
//      ret += query_multiple_short (p->files);        
//    }
    if (sizeof (p->twiki)) {
      ret += "<TD>"
      "<TR>"    
      "<TH align = \"left\"> Project Twiki: </TH>" 
      "<TD>";  
      ret += query_multiple_short (get_twiki_links (p));               
    }
  }

 ret += "</TABLE>";
      
  return ret;        
}

string get_view_page (string page, string id, string title) {
  string ret = "";
  
  ret += page;

  ret += get_tasks_summary (id);
  ret += get_notes_summary (id);  
  ret += get_files_summary (id);  


  return get_web_page (ret, title);
}  
  
string www_task_info (class task p) {
  string ret = "";
  
  if (!p) {
    ret += "There is no task with that ID.\n";
  }
  
  else {
    ret += "<TABLE>"
    "<TR><TH align = \"left\"> Task ID: </TH>"
    "<TD>";
    ret += p->id;
    ret += "<TD>";
    ret += "<B>This task was added by: " + p->added_by + "</B>"
    "<TR>"    
    "<TH align = \"left\"> Task Name: </TH>" 
    "<TD>";  
    ret += p->task_name;
    ret += "<TD>"
    "<TR>";          
    ret += "<B>Added: " + ctime (p->date_added) + " </B>"
    "<TD>"    
    "<TR>"    
    "<TH align = \"left\"> Task creators: </TH>" 
    "<TD>";  
    ret += query_multiple_short (get_creator_links (p->assigned_to));
    ret += "<TD>"
    "<TR>";                   
    if (p->completion_date) {           
      ret += "<B> Estimated Completion Date: " + ctime (p->completion_date) 
        + "</B>"; 
    }                
    ret += "<TD>";        
    if (p->percent_complete) {           
      ret += "<TD>"        
      "<TR>"    
      "<TH align = \"left\"> Currently completed percentage: </TH>" 
      "<TD>";  
      ret += query_num (p->percent_complete);
      ret += "<TD>"
      "<TR>";   
    }   
    ret += "</TABLE>";
  }
  
  return ret;        
}


  
varargs string get_project_menu() {  
  string ret = "";
  string *no_project_list = ({"fluffy", "admin", "decrepit", "am_old"});
  string *domains = (master()->query_domains() - no_project_list);
  
  ret += "<form action=\"project.c\" METHOD=POST "
    "action=\"project.c?\">"
    "<TABLE>"
    "<TR><TH align = \"left\"> Projects: </TH>"
    "<TD>"  
    "<b>[</b> <a href=project.c?projecttype=Add>"
    "Add Project</a> | "    
    "<a href=project.c?projecttype=List>"
    "List Projects</a> | "
    "<a href=project.c?misc=Query> "
    "Query Projects </a> <b>]</b>"
    "<TD>"
    "<TR>"   
    "<TH align = \"left\"> Queries: </TH>"
    "<TD>"
    "<b>[</b> <a href=project.c?misc=Week> "
    "One Week Summary </a> | "
    "<a href=project.c?misc=Playtesting> "
    "Projects in Playtesting</a> <b>]</b>"
    "<TD>"
    "<TR>"
    "<TH align = \"left\"> Domains: </TH>" 
    "<TD> <b>[</b> ";
    
    for (int i = 0; i < sizeof (domains); i++) {      
      ret += "<a href=project.c?misc=Domlist&domain=" + domains[i] + ">"
        + capitalize (domains[i]) + "</a>";
      
      if (i != sizeof (domains) -1) {
        ret += " | ";
      }
      
    }
    
    ret += " <b>]</b></TABLE>"
    "<HR>";

  return ret;
  
}


varargs string get_add_project (mapping tmp) {
  string ret = "";
  
  if (!tmp) {
    tmp = ([ ]);
  }
  ret += "This is the add project page.  Enter the details of the project "
    "you wish to add, and then click 'add' to record them with the system.  "
    "Required fields are Project ID, Project Name and Project Description.  "
    "If you forget to complete any of these fields, you will be prompted "
    "again for details.<p><p>"
    "<hr><p>"
    "<TABLE>";
    
  ret += "<form action=\"project.c\" METHOD=POST "
    "action=\"project.c?\">";

  ret += "<TR>" 
    "<TH align = \"left\"> Project ID: </TH>"
    "<TD><INPUT NAME=\"projid\"";
  
  if (tmp["projid"]) {
    ret += "VALUE = \"" + tmp ["projid"] + "\"";
  }

  ret += "><TD>"  
    "<TR>"
    "<TH align = \"left\"> Project Name: </TH>" 
    "<TD><INPUT NAME=\"name\"";

  if (tmp["name"]) {
    ret += "VALUE = \"" + tmp ["name"] + "\"";
  }
  
  ret += "><TD>"  
    "<TR>"
    "<TH align = \"left\"> Project Leader: </TH>" 
    "<TD><INPUT NAME=\"leader\"";

  if (tmp["leader"]) {
    ret += "VALUE = \"" + tmp ["leader"] + "\"";
  }
  
  ret += "><TD>"  
    "<TR>"
    "<TH align = \"left\"> Project Domains (separated by comma): </TH>" 
    "<TD><INPUT NAME=\"domains\"";
  
  if (sizeof (arrayp (tmp ["domains"]))) {
    ret += "VALUE = \"" + tmp["domains"] + "\"";
  }
  ret += "><TD>"  
    "<TR>"
    "<TH align = \"left\"> Estimated Completion: </TH>" 
    "<TD>";
    ret += get_estimate();
    ret += "<TD>"  
    "<TR>"
    "<TH align = \"left\"> Project Creators (separated by comma): </TH>" 
    "<TD><INPUT NAME=\"creators\"";

  if (sizeof (tmp ["creators"])) {
    ret += "VALUE = \"" + tmp["creators"]  + "\"";
  }
  
  ret += "><TD>"  
    "<TR>"
    "<TH align = \"left\"> Project Size: </TH><TD>";
       
  ret += "<SELECT NAME=\"size\">";
  
  for (int i = 0; i < sizeof (SIZE); i++) {
    ret += "<OPTION VALUE + \"" + SIZE[i] ;
    if (tmp["size"] == SIZE[i]) {
      ret += "selected ";
    }
  
    ret += "\">" + SIZE[i];
  }
  
  ret += "</SELECT>";

  ret += "<TD>"  
    "<TR>"
    "<TH align = \"left\"> Project Complexity: </TH><TD>";
       
  ret += "<SELECT NAME=\"complexity\">";
  
  for (int i = 0; i < sizeof (COMPLEXITY); i++) {
    ret += "<OPTION VALUE + \"" + COMPLEXITY[i] ;
    if (tmp["complexity"] == COMPLEXITY[i]) {
      ret += "selected ";
    }
  
    ret += "\">" + COMPLEXITY[i];
  }
  
  ret += "</SELECT>";

    ret += "<TD>"  
    "<TR>"
    "<TH align = \"left\"> Guilds (Of the form 'wizards,thieves', separated by comma, or leave blank): </TH>" 
    "<TD><INPUT NAME=\"guilds\"";

  if (sizeof (tmp ["guilds"])) {
    ret += "VALUE = \"" + tmp["guilds"]  + "\"";
  }      
  
//  if (sizeof (tmp["twiki"])) {
//    foreach (string t in twiki) {
//      PROJECT_HANDLER->add_twiki (data["projid"], t);
//    }
//  }
  
  ret += "><TD>"  
    "<TR>"
    "<TH align = \"left\"> Subproject IDs (separated by comma): </TH>" 
    "<TD><INPUT NAME=\"sub\"";
//  ret += "><TD>"  
//    "<TR>"
//    "<TH align = \"left\"> Project Files (separated by comma): </TH>" 
//    "<TD><INPUT NAME=\"files\"";

//  if (sizeof (tmp ["files"])) {
//    ret += "VALUE = \"" + tmp["files"] + "\"";
//  }
  
  ret += "<TD>"  
    "<TR>"
    "<TH align = \"left\"> Project Twiki (separated by comma): </TH>" 
    "<TD><INPUT NAME=\"twiki\"";

  if (sizeof (tmp ["twiki"])) {
    ret += "VALUE = \"" + tmp["twiki"] + "\"";
  }
  
  ret += "><TD>"  
    "<TR>"
    "</TABLE>"
    "<B>Project Description:</B><P>" 
    "<TD><TEXTAREA cols=70 rows=6 wrap=\"virtual\""
      "NAME=\"description\">";
  if (tmp["description"]) {
    ret += tmp ["description"];
  }

  ret += "</TEXTAREA><P>";

  ret += "<INPUT TYPE=\"submit\" NAME = \"button\" VALUE=\"Add\">";
  ret += "<INPUT TYPE=\"reset\" VALUE=\"Reset\"><p>";
   
  return ret;
}

varargs string get_add_task (mapping tmp, string project) {
  string ret = "";
  
  if (!tmp) {
    tmp = ([ ]);
  }
  ret += "This is the add task page.  Enter the details of the task "
    "you wish to add, and then click 'add' to record them with the system.  "
    "Required fields are task ID, and task Name. you will be prompted "
    "again for details.<p><p>"
    "<hr><p>"
    "<TABLE>";

      
  ret += "<form action=\"project.c\" METHOD=POST "
    "action=\"project.c?\">";

 ret += "<TR>" 
    "<TH align = \"left\"> Project ID: </TH>"
    "<TD><INPUT NAME=\"projid\"";
  
  if (project) {
    ret += "VALUE = \"" + project + "\"";
  }
  ret += "<TD>"
    "<TR>" 
    "<TH align = \"left\"> Task ID: </TH>"
    "<TD><INPUT NAME=\"tid\"";
  
  if (tmp["tid"]) {
    ret += "VALUE = \"" + tmp ["tid"] + "\"";
  }

  ret += "><TD>"  
    "<TR>"
    "<TH align = \"left\"> Task Name: </TH>" 
    "<TD><INPUT NAME=\"taskname\"";

  if (tmp["taskname"]) {
    ret += "VALUE = \"" + tmp ["taskname"] + "\"";
  }
  
  ret += "><TD>"  
    "<TR>"
    "<TH align = \"left\"> Comments: </TH>" 
    "<TD><INPUT NAME=\"comments\"";

  if (tmp["comments"]) {
    ret += "VALUE = \"" + tmp ["comments"] + "\"";
  }
  
  ret += "><TD>"  
    "<TR>"
    "<TH align = \"left\"> Task Creators (separated by comma): </TH>" 
    "<TD><INPUT NAME=\"creators\"";

  if (sizeof (tmp ["creators"])) {
    ret += "VALUE = \"" + tmp["creators"]  + "\"";
  }
  
  ret += "><TD>"  
    "<TR>"
    "<TH align = \"left\"> Percent Completed: </TH>" 
    "<TD><INPUT NAME=\"completed\"";

  if (tmp["completed"]) {
    ret += "VALUE = \"" + tmp ["completed"] + "\"";
  }
  
  ret += "><TD>"  
    "<TR>"
    "<TH align = \"left\"> Estimated completion: </TH>" 
    "<TD>" + get_estimate();    
  ret += "<TD>"  
    "<TR>"
    "</TABLE>"
  "<B>Task aims:</B><P>" 
    "<TD><TEXTAREA rows=6 cols=70 wrap=\"virtual\""
      "NAME=\"aims\">";
  if (tmp["aims"]) {
    ret += tmp ["aims"];
  }
  ret += "</TEXTAREA><P>"  
    "<INPUT TYPE=\"submit\" NAME = \"button\" VALUE=\"Add Task\">"
    "<INPUT TYPE=\"reset\" VALUE=\"Reset\"><p>";
   
  return ret;
}


varargs string get_add_note (mapping tmp, string project) {
  string ret = "";
  
  if (!tmp) {
    tmp = ([ ]);
  }
  ret += "This is the add note page.  Enter the details of the note "
    "you wish to add, and then click 'add' to record them with the system.  "
    "Required fields are note ID, and the note itself. you will be prompted "
    "again for details.<p><p>"
    "<hr><p>"
    "<TABLE>";
    
  ret += "<form action=\"project.c\" METHOD=POST "
    "action=\"project.c?\">";

  ret += "<TR>" 
    "<TH align = \"left\"> Project ID: </TH>"
    "<TD><INPUT NAME=\"projid\"";
  
  if (project) {
    ret += "VALUE = \"" + project + "\"";
  }

  ret += "><TD>"  
    "<TR>" 
    "<TH align = \"left\"> Note ID: </TH>"
    "<TD><INPUT NAME=\"nid\"";
  
  if (tmp["nid"]) {
    ret += "VALUE = \"" + tmp ["nid"] + "\"";
  }

  ret += "><TD>"  
    "<TR>"
    "<TH align = \"left\"> Note: </TH>" 
    "<TD><TEXTAREA NAME=\"note\" rows=6 cols=70"
    " wrap=virtual>" + (tmp["note"] ? tmp ["note"] : 
    "") + "</TEXTAREA>"  
    "<TD>"
    "</TABLE>"
    "<INPUT TYPE=\"submit\" NAME = \"button\" VALUE=\"Add Note\">"
    "<INPUT TYPE=\"reset\" VALUE=\"Reset\"><p>";
   
  return ret;
}

varargs string get_add_file (mapping tmp, string project) {
  string ret = "";
  string *tasks = ({ });
  class project proj;
  
  proj = PROJECT_HANDLER->get_project_file (project);
  
  foreach (class task t in proj->tasks) {
    tasks += ({ t->id });    
  }
  
  
  if (!tmp) {
    tmp = ([ ]);
  }
   ret+= "<TABLE>"
    "<form action=\"project.c\" METHOD=POST "
    "action=\"project.c?\">"
    "<TR>" 
    "<TH align = \"left\"> Project ID: </TH>"
    "<TD><INPUT NAME=\"projid\"";
  
  if (project) {
    ret += "VALUE = \"" + project + "\"";
  }

  ret += "><TD>"  
    "<TR>" 
    "<TH align = \"left\"> Filename: </TH>"
    "<TD><INPUT NAME=\"fid\"";
  
  if (tmp["fid"]) {
    ret += "VALUE = \"" + tmp ["fid"] + "\"";
  }

  ret += "><TD>"  
    "<TR>"
    "<TH align = \"left\"> Access: </TH>" 
    "<TD>"
    "<SELECT NAME=\"access\">";
    
    foreach (string a in ({"read", "read+write", "read+write+grant"}))  {   
      ret += "<OPTION  \"" + a + "\">"+ a;
    }
    
  ret += "</SELECT>"   
    "<TD>"  
    "<TR>" 
    "<TH align = \"left\"> Access Rights: </TH>"
    "<TD>"
    "<SELECT NAME=\"task\">";
    
    foreach (string a in ({"All project members", "Leader only"}) +
      tasks)  {   
      ret += "<OPTION  \"" + a + "\"" + (tmp["task"] == a ? 
      "selected" : "") + ">"+ a;
    }
    
  ret += "</SELECT>"   
     "</TABLE>"
    "<INPUT TYPE=\"submit\" NAME = \"button\" VALUE=\"Add File\">"
    "<INPUT TYPE=\"reset\" VALUE=\"Reset\"><p>";

  return ret;
}

varargs string get_modify_project (string id, string swit, string id_two, 
  int lead) {
  string ret = ""; 
  
  class project tmp = PROJECT_HANDLER->get_project_file (id);
  
  
  if (!tmp) {
    return "";    
  }
  
  
  ret += "<form action=\"project.c\" METHOD=POST "
    "action=\"project.c?\">"
    "<B>Project " + id + ": </B><b>[</b><a href=project.c?pid=" 
    + id + "&projecttype=Delete>"
    "Delete This Project</a> | "
    "<a href=project.c?pid=" 
    + id + "&projecttype=Details>"
    "Project Details</a> <b>]</b>"
    "<P><B>" + PROJECT_HANDLER->query_touched (tmp->id) + "</B></P>"
    "<HR><TABLE><TR>" 
    "<TH align = \"left\"> Project ID: </TH>"
    "<TD><INPUT NAME=\"projid\""
    "VALUE = \"" + tmp->id + "\""  
    "><TD>"
    "<TR>"
    "<TH align = \"left\"> Project Name: </TH>" 
    "<TD><INPUT NAME=\"name\""
    "VALUE = \"" + tmp->name + "\""    
    "><TD>"  
    "<TR>"
    "<TH align = \"left\"> Project Leader: </TH>" 
    "<TD><INPUT NAME=\"leader\""
    "VALUE = \"" + tmp->leader + "\""  
    "><TD>"  
    "<B> This project was added by " + tmp->added_by +"</B>"     
    "<TR>"
    "<TH align = \"left\"> Project Domains (separated by comma): </TH>" 
    "<TD><INPUT NAME=\"domains\""
    "VALUE = \"" + implode (tmp->domains, ",") + "\""
    "><TD>"  
    "<B> Added: " + ctime (tmp->date_added) +"</B>"     
    "<TD>"
    "<TR>"
    "<TH align = \"left\"> Project Creators (separated by comma): </TH>" 
    "<TD><INPUT NAME=\"creators\""
    "VALUE = \"" + implode (tmp->creators, ",") 
    + "\"><TD>"
    "<TR>"
    "<TH align = \"left\"> Project Playtesters (separated by comma): </TH>" 
    "<TD><INPUT NAME=\"playtesters\"";
    if (tmp->playtesters) {
      ret += "VALUE = \"" + implode (tmp->playtesters, ",");
    }
    ret += "\">";

    ret += "<TD>"  
    "<TR>"
    "<TH align = \"left\"> Project Size: </TH><TD>";
       
    ret += "<SELECT NAME=\"size\">";
  
    for (int i = 0; i < sizeof (SIZE); i++) {
      ret += "<OPTION VALUE + \"" + SIZE[i] ;
      if (tmp->size  == i) {     
        ret += " selected ";
      }
  
      ret += "\">" + SIZE[i];
  }

    ret += "</SELECT>";     
    ret += "<TD>"  
    "<TR>"
    "<TH align = \"left\"> Project Complexity: </TH><TD>";
       
    ret += "<SELECT NAME=\"complexity\">";
  
    for (int i = 0; i < sizeof (COMPLEXITY); i++) {
      ret += "<OPTION VALUE + \"" + COMPLEXITY[i] ;
      if (tmp->complexity  == i) {
        ret += " selected ";
      }
  
      ret += "\">" + COMPLEXITY[i];
  }
  
  ret += "</SELECT>"    
    "<TD>"
    "<TR>"
    "<TH align = \"left\"> Guilds (Of the form 'wizards,thieves', separated by comma, "
      "or leave blank): </TH>" 
    "<TD><INPUT NAME=\"guilds\"";
    if (tmp->guild_specific) {
      ret += "VALUE = \"" + implode (tmp->guild_specific, ",");
    }
    ret += "\">";    
    ret += "<TD><TR>"
    "<TH align = \"left\"> Percent complete: </TH>" 
    "<TD><INPUT NAME=\"completed\"";
    if (tmp->percent_complete) {
      ret += "VALUE = \"" + tmp->percent_complete;
    }
    ret += "\"><TD><TR>";     
    if (tmp->estimated_completion) {
      ret += "<B> Estimated Completion: " + ctime 
        (tmp->estimated_completion) + "</B>";
    } 
   
    
    ret += "<TR>"
    "<TH align = \"left\"> Subproject IDs (separated by comma): </TH>" 
    "<TD><INPUT NAME=\"sub\"";    
    if (sizeof (tmp->sub_projects)) {
        ret += "VALUE = \"" + implode (tmp->sub_projects, ",") + "\"";
    }
  
    ret += "><TD>"   
    "<TR>"
    "<TH align = \"left\"> Project Twiki (separated by comma): </TH>" 
    "<TD><INPUT NAME=\"twiki\"";    
    if (sizeof (tmp->twiki)) {
        ret += "VALUE = \"" + implode (tmp->twiki, ",") + "\"";
    }
      
  ret += "><TD>"  
    "<TR>"
    "<TH align = \"left\"> Update Estimate: </TH>" 
    "<TD>";
    ret += get_estimate();
    ret += "<TD>"  
    "<TR>"
    "<TH align = \"left\"> Project Status: </TH>" 
    "<TD><SELECT NAME=\"status\">";
    
    foreach (string stat in STATUS)  {   
      ret += "<OPTION  \"" + stat+ "\"";
    
      if (stat == STATUS[tmp->status]) {
        ret += " selected "; 
      }
      
      ret += ">" + stat;
    }      
  ret += "</SELECT>"
    "</TABLE>"
    
    "<B>Project Description:</B><P>"
    "<TEXTAREA NAME=\"description\" rows=6 cols=70 "
    "wrap=\"virtual\">" + tmp->description+ "</TEXTAREA><P>"    
    "<B>Notes for Playtesters:</B><P>"
    "<TEXTAREA NAME=\"ptnotes\" rows=6 cols=70 "
    "wrap=\"virtual\">" + tmp->pt_notes + "</TEXTAREA><P>"
    "<INPUT TYPE=\"submit\" NAME = \"button\" VALUE=\"Update\">"
    "<INPUT TYPE=\"reset\" VALUE=\"Reset\">";    

    
  if (swit) {
    switch (swit) {
      case "task":
        ret += get_modify_task(id, id_two);
      break;
      case "note":
        ret += get_modify_note(id, id_two);
      break;
      case "file":
        ret += get_modify_file (id, id_two, lead);
    }
    
  }
    
  ret += get_tasks_summary (id);
  ret += get_notes_summary (id);
  ret += get_files_summary (id);

  
  return ret;
  
}

varargs string get_modify_task (string id, string task) {
  string ret = "";  
  class task tmp;

  tmp  = PROJECT_HANDLER->get_task_file (id, task);
  
  ret += get_view_project (id, 0);
  
  ret += "<HR><TABLE>";  
  
  if (!tmp) {
    return "";    
  }
  
  ret += 
    "<form action=\"project.c\" METHOD=POST "
    "action=\"project.c?\">"
    "<B>Task " + id + ": </B><b>[</b><a href=project.c?pid=" + id 
    + "&tasktype=Delete&tid=" + task + ">"
    "Delete This Task</a> <b>]</b> <p>"    
    "<TABLE>"
    "<TH align = \"left\"> Project ID: </TH>" 
    "<TD><INPUT NAME=\"projid\""
    "VALUE = \"" + id + "\""    
    "><TD>"  
    "<TR>"
    "<TH align = \"left\"> Task ID: </TH>" 
    "<TD><INPUT NAME=\"tid\""
    "VALUE = \"" + tmp->id + "\""    
    "><TD>"  
    "<B>This task was added by " + tmp->added_by + "</B>"
    "<TR>"
    "<TH align = \"left\"> Task Name: </TH>" 
    "<TD><INPUT NAME=\"taskname\""
    "VALUE = \"" + tmp->task_name + "\""    
    "><TD>"  
    "<B>Added: " + ctime (tmp->date_added) + " </B>"
    "<TR>"
    "<TH align = \"left\"> Comments: </TH>" 
    "<TD><INPUT NAME=\"comments\""
    "VALUE = \"" + tmp->comments+ "\""  
    "><TD>";  
    if (tmp->completion_date) {
    ret += "<B>Estimated Completion: " + ctime (tmp->completion_date) 
      + "</B>";
    }
    ret += "<TR><TH align = \"left\"> Project creators (separated by "
    "comma): "
    "</TH>" 
    "<TD>"
    "<INPUT NAME=\"assigned\"";
    if (sizeof (tmp->assigned_to)) {      
      ret += "VALUE = \"" + implode (tmp->assigned_to, ",") + "\"";  
    }
    ret += "><TD><TR>"
      "<TH align = \"left\"> Percent Complete: </TH>" 
      "<TD><INPUT NAME=\"completed\""
      "VALUE = \"" + tmp->percent_complete+ "\""  
      "><TD>"  
      "<TR>"
      "<TH align = \"left\"> Estimated Completion date: </TH>"
      "<TD>";
    ret += get_estimate();
    ret += "<TD>"  
      "<TR>"
      "</TABLE>"
      "<B>Task Aims:</B><P>"
      "<TEXTAREA NAME=\"aims\" rows=6 cols=70 "
      "wrap=\"virtual\">" + tmp->aims+ "</TEXTAREA><P>"  
      "<INPUT TYPE=\"submit\" NAME = \"button\" VALUE=\"Update Task\">"
      "<INPUT TYPE=\"reset\" VALUE=\"Reset\"><p>";  
  return ret;
}


varargs string get_modify_file (string id, string file, string task) {
  string ret = "";  
  class file tmp;

  
  tmp  = PROJECT_HANDLER->get_file (id, file, task);
  
  ret += get_view_project (id, 0);
  
  ret += "<HR><TABLE>";  
  
  if (!tmp) {
    return "";    
  }
  ret += 
    "<form action=\"project.c\" METHOD=POST "
    "action=\"project.c?\">"
    "<B>File " + file + ": </B><b>[</b> <a href=project.c?pid=" + id 
    + "&filetype=Delete&fid=" + file+ "&task=" + task+ ">"
    "Delete This Permission</a> <b>]</b><p>"    
    "<TABLE width=100%>"
    "<TR>"
    "<TH align = \"left\"> Project ID: </TH>" 
    "<TD width=50%><INPUT NAME=\"projid\""
    "VALUE = \"" + id + "\""    
    "><TD>"  
    "<TR>"
    "<TH align = \"left\"> Filename: </TH>" 
    "<TD width=50%><INPUT NAME=\"file\""
    "VALUE = \"" + file + "\""    
    "><TD>"  
    "<TR>"
    "<TH align = \"left\"> Access: </TH>" 
    "<TD>"
    "<SELECT NAME=\"access\">";
    
    foreach (string a in ({"read", "write", "grant"}))  {   
      ret += "<OPTION  \"" + a + "\">"+ a;
    }
    
    ret += "</SELECT>"   
    "<TD>"  
    "<TR>"
    "<TH align = \"left\"> Leader Only: </TH>" 
    "<TD>"
    "<SELECT NAME=\"lead\">";
    
    foreach (string a in ({"yes", "no"}))  {   
      ret += "<OPTION  \"" + a + "\">" + a;
    }
    ret += "</SELECT>"
      "</TABLE>"
    "<INPUT TYPE=\"submit\" NAME = \"button\" VALUE=\"Update File\">"
    "<INPUT TYPE=\"reset\" VALUE=\"Reset\"><p>";

  return ret;
}

varargs string get_modify_note (string id, string note) {
  string ret = "";  
  class note tmp;

  
  tmp  = PROJECT_HANDLER->get_note_file (id, note);
  
  ret += get_view_project (id, 0);
  
  ret += "<HR><TABLE>";  
  
  if (!tmp) {
    return "";    
  }
  ret += 
    "<form action=\"project.c\" METHOD=POST "
    "action=\"project.c?\">"
    "<B>Note " + note + ": </B><b>[</b> <a href=project.c?pid=" + id 
    + "&notetype=Delete&nid=" + note + ">"
    "Delete This Note</a> <b>]</b><p>"    
    "<TABLE width=100%>"
    "<TR>"
    "<TH align = \"left\"> Project ID: </TH>" 
    "<TD width=50%><INPUT NAME=\"projid\""
    "VALUE = \"" + id + "\""    
    "><TD>"
    "<B>This note was added by " + tmp->added_by + "</B><BR>"
    "<TR>"    
    "<TH align = \"left\"> Note ID: </TH>" 
    "<TD><INPUT NAME=\"nid\""
    "VALUE = \"" + tmp->id + "\""    
    "><TD>"
    "<B>Added: " + ctime (tmp->date_added) + "</B>"      
    "</TABLE>"
    "<TEXTAREA NAME=\"note\" ROWS=6 COLS=70 "
    " wrap=virtual>" + tmp->note + "</TEXTAREA><P>"    
    "<INPUT TYPE=\"submit\" NAME = \"button\" VALUE=\"Update Note\">"
    "<INPUT TYPE=\"reset\" VALUE=\"Reset\"><p>";
  
  return ret;
}

varargs string get_view_note (string id, string note) {
  string ret = "";  
  class note tmp;

  ret += get_view_project (id, 0);  
  
  tmp  = PROJECT_HANDLER->get_note_file (id, note);
  
  
  if (!tmp) {
    return "";    
  }
    
  ret += "<HR>"
      "<B>Note " + note + ": </b><a href=project.c?nid=" + tmp->id + 
      "&notetype=Edit&pid=" 
      + id + ">Edit Note</a><P> "
    "<TABLE width=100%>"
    "<TR>"
    "<TH width=30% align = \"left\"> Note ID: </TH>" 
    "<TD>" + tmp->id + "<TD>"  
    "<TR>"
    "<TH align = \"left\"> Added by: </TH>" 
    "<TD>" + capitalize (tmp->added_by) + " at " + ctime (tmp->date_added) 
      + "<TD>"  
    "<TR>"
    "<TH valign = \"top\" align = \"left\"> Note: </TH>" 
    "<TD>" + replace (tmp->note, ({"\n\n", "<p>", "\n", "<br>"})) + "<TD>"  
    "<TR>"


    "</TABLE>";

  return ret;
}

varargs string get_query_project (mapping tmp) {
  string ret = "<TABLE>";
  
  if (!tmp) {
    tmp = ([ ]);
  }
  
  ret += "<form action=\"project.c\" METHOD=POST "
    "action=\"project.c?\">";

  ret += "<TR>" 
    "<TH align = \"left\"> Project Domains (separated by comma): </TH>"
    "<TD><INPUT NAME=\"domains\"";
  
  if (tmp["domains"]) {
    ret += "VALUE = \"" + tmp ["creators"] + "\"";
  }

  ret += "><TD>"  
    "<TR>"
    "<TH align = \"left\"> Project Creators (separated by comma): </TH>" 
    "<TD><INPUT NAME=\"creators\"";

  if (tmp["creators"]) {
    ret += "VALUE = \"" + tmp ["creators"] + "\"";
  }
  ret += "><TD>"  
    "<TR>"
    "<TH align = \"left\"> Project Playtesters (separated by comma): </TH>" 
    "<TD><INPUT NAME=\"playtesters\"";

  if (tmp["playtesters"]) {
    ret += "VALUE = \"" + tmp ["playtesters"] + "\"";
  }  

  ret += "><TD>"  
    "<TR>"
    "<TH align = \"left\"> Minimum completeness: </TH>" 
    "<TD><INPUT NAME=\"completeness\"";

  if (tmp["completeness"]) {
    ret += "VALUE = \"" + tmp ["completeness"] + "\"";
  }
    
  ret += "><TD>"  
    "<TR>"
    "<TH align = \"left\"> Project Leader: </TH>" 
    "<TD><INPUT NAME=\"leader\"";

  if (tmp["leader"]) {
    ret += "VALUE = \"" + tmp ["leader"] + "\"";
  }
  
  ret += "><TD>"  
    "<TR>"
    "<TH align = \"left\"> Project Status: </TH>" 
    "<TD><SELECT NAME=\"status\">";
    
    foreach (string stat in STATUS)  {   
      ret += "<OPTION  \"" + stat+ "\">" + stat;
    }

  ret += "<OPTION  \"all\" selected> all"  
    "</SELECT>"
    "<TR><TH align = \"left\"> Last touched: </TH>" 
    "<TD>" + get_estimate(1) + "<TD><TR>";

  if (tmp["leader"]) {
    ret += "VALUE = \"" + tmp ["leader"] + "\"";
  }

  ret +="</TABLE>"
    "<INPUT TYPE=\"submit\" NAME = \"button\" VALUE=\"Query\">"
    "<INPUT TYPE=\"reset\" VALUE=\"Reset\"><p>";

  return ret;
}

varargs string get_query_task (mapping tmp) {
  string ret = "<TABLE>";
  
  if (!tmp) {
    tmp = ([ ]);
  }
  
  ret += "<form action=\"project.c\" METHOD=POST "
    "action=\"project.c?\">";

  ret += "<TR>" 
    "<TH align = \"left\"> Task Creators (separated by comma): </TH>"
    "<TD><INPUT NAME=\"creators\"";
  
  if (tmp["creators"]) {
    ret += "VALUE = \"" + tmp ["creators"] + "\"";
  }

  ret += "><TD>"  
    "<TR>"
    "<TH align = \"left\"> Percent Completed: </TH>" 
    "<TD><INPUT NAME=\"completed\"";

  if (tmp["completed"]) {
    ret += "VALUE = \"" + tmp ["completed"] + "\"";
  }
  
  ret += "><TD>"  
    "<TR>"
    "</TABLE>"
    "<INPUT TYPE=\"submit\" NAME = \"button\" VALUE=\"Query\">"
    "<INPUT TYPE=\"reset\" VALUE=\"Reset\"><p>";

  return ret;
}



void create() {
  seteuid (geteuid());
}

  
string get_header(string title) {
  string ret = "";
  
  ret += "<html><head><title> Project Management 0.7a</title>";
  ret += "<meta http-equiv=\"Content-Type\" content=\"text/html; "
    "charset=iso-8859-1\"><meta http-equiv=\"Cache-Control\" "
     "content=\"expires=-1\"/></head><body bgcolor=\"#ffffff\" "
    "text=\"#000030\" link=\"#4a529c\" vlink=\"#b57339\">"
    "<table width=\"75%\" border=\"0\"><tr VALIGN=TOP>";
  ret += "<td><font face=\"arial,helvetica\"><img align=left "
    "src = \"http://discworld.imaginary.com/external//pics/dw4.gif\">"
    "</font></td>"
    "<td><font face=\"arial,helvetica\">" 
    "<h2>Project Management 0.7a</h2>";
  ret += "</font>"
    "<p><font face=\"arial,helvetica\"><h2>" + title + "</h2>"
    "</p>"
    "<a href=project.c?misc=instructions>Instructions</a>"
    "<br>"
    "<a href=../../secure>Back to the secure pages</a>"
    "</td></tr></table>";
  ret += get_project_menu();
  
  return ret;
}

/**
 *  This function requests the standard footer for the Discworld
 *  web-pages.
 *  @return string  The HTML code for the footer.
 *
 */
 
string get_footer() {
    return "/www/footer"->www_function("footer", ([ ])) +"</body></html>";
}

string get_web_page (string text, string title) {
  return get_header (title) + text + get_footer();
}


string get_instructions(string category) {
  string ret = "";
  

  ret += "<h2>Index Of Help</h2>"
    "<a href=project.c?misc=instructions&cat=access>Access "
        "Rights</a><br>" 
    "<a href=project.c?misc=instructions&cat=addnote>Adding a note</a><br>"        
    "<a href=project.c?misc=instructions&cat=addproject>Adding a project"
      "</a><br>"
    "<a href=project.c?misc=instructions&cat=addtask>Adding a task</a><br>"
    "<a href=project.c?misc=instructions&cat=domainstuff>Master object "
      "modifications</a><br>"
    "<a href=project.c?misc=instructions&cat=modnote>Modifying a note"
      "</a><br>"      
    "<a href=project.c?misc=instructions&cat=modproject>Modifying a "
      "project</a><br>"      
    "<a href=project.c?misc=instructions&cat=modtask>Modifying a "
      "task</a><br>"      
    "<a href=project.c?misc=instructions&cat=queryprojects>Querying the "
      "database</a><p>";
        
  switch (category){ 
    case "access":
      ret += "<h2>Access Rights</h2>"
      "The access rights for the system are as default:<p>"
      "<ol>"
      "<li>Highlords have full write to all data within the system."
      "<li>Everyone can read everything in the system."
      "<li>The lord and deputy of the playtesters domain can modify "
        "playtester assignments in any domain."
      "<li>Domain lords can assign playtesters to projects within their "
        "domains."
      "<li>Domain lords and deputies can add, delete and edit all projects, "
        "notes and tasks within their domain."       
      "<li>Project leaders can delete and modify the projects they are "
        "leading, and can add, delete and edit tasks and notes for the "
        "projects they are actively coding for."
      "<li>Project creators can add, delete and modify tasks and notes "
        "for the projects they are assigned to."
      "</ol>"
      "Domain lords can override 4, 5, 6 and 7 by modifying their master "
      "objects to over-ride the defaults.  See 'master object "
      "modifications' for more details.<p>";
    break;
    case "domainstuff":
      ret += "<h2>Domain stuff</h2>"
        "You can use the features of the project tracker within your "
        "own domain.  The simplest way to make use of it is to use "
        "the $project$ string in your creator's projects.  This will "
        "be expanded to their list of projects are returned from the "
        "tracker.<p>"
        "You can also over-ride the default functionality of the "
        "security restrictions by defining a function in your "
        "domain object called project_callback:<p>"
        "<pre>"
        "int project_callback (string name, string action, string project) {"
        "<br>  if (name == LORD) {"
        "<br>     return 1;"
        "<br>  }"
        "<br>  return 0;"
        "<br>}"
        "<br></pre>"
        "<p>This is an extreme example, of course, and restricts it so "
        "only the lord of the domain is allowed to modify any aspect "
        "of any project within the domain.<p>"
        "<b>Name</b> is the name of the creator making a change to the "
        "database.<br>"
        "<b>Action</b> is the action being performed, one of 'project', "
        "'task', 'note' or 'playtesting'<br>"
        "<b>Project</b> is the project being modified.<p>"
        "The function should return 1 if the action is allowed, 0 if it "
        "is not, and -1 if the defaults should be used.<p>";              
    break;
    case "addproject":
      ret += "<h2>Adding a Project</h2>"
      "First, click on 'add a project'.  This will bring up a form "
      "containing the information you need to enter into the system. "
      "Enter each of these pieces of information in turn as required.  "
      "You will need to enter at least a project ID, a project name, "
      "and at least one domain for the project.<p>"
      "<b>Project ID</b>:  This is the unique code for the project that "
      "identifies it in the database.  If you put mutliple words in "
      "this field, it will concatonate them into 'twiki notation'... "
      "so 'project tracker' becomes ProjectTracker.<p>"
      "<b>Project Name</b>: This is a descriptive name for the project.  "
      "This differs from the project ID in that the project ID is used "
      "internally within the tracker, whereas the name is used "
      "externally, for example in domain finger info and the list of "
      "projects in playtesting.<p>"
      "<b>Project Leader</b>: The leader for the project.  Project leaders "
      "have full access over the project in the tracker, and can update the "
      "project file, and add tasks and notes as required.  See " 
      "<a href=project.c?misc=instructions&cat=access>Access Rights</a> "
      "for more information on how access rights work.<p>"
      "<b>Project Domains</b>: The domains the project belongs to.  You "
      "must be a HL or the lord of at least one of the domains to add "
      "a project to the tracker.<p>"
      "<b>Estimated Completion</b>: The rough estimate of how long it "
      "will take to complete the project.  If you wish to have this "
      "estimate recorded with the system, check the 'update' checkbox.  "
      "If you do not, it will ignore any estimate you place into the "
      "system.<p>"
      "<b>Project Creators</b>: The creator assigned to the project.  "
      "These do not actually have to be members of the domain to "
      "be assigned, but if they are not they will not have their "
      "project displaying in their finger info.<p>"
      "<b>Subproject IDs</b>: The larger projects that form part of "
      "the main project.  While these stand as projects in themselves, "
      "their state of completion reflects on that of the main project.  "
      "The percentage of a project's completion will be modified by "
      "how complete any sub projects are.<p>"
      "<b>Project Files</b>: Any relevant files related to the project.<p>"
      "<b>Project Twiki</b>: Any relevant twiki pages for the project.  "
      "If you simply enter a twiki name (such as ProjectTracker) it will "
      "assume the twiki page may be found on the project's first domain.  "
      "So if you have a project belonging to Forn and Main, it will "
      "assume you mean Forn.ProjectTracker.  If you wish to direct the "
      "link to a particular twiki page, you will need to give the web, then "
      "the page... so Main.ProjectTracker.<p>"
      "<b>Description</b>: The description of what the project actually "
      "involves.<p>"
      "Once you've entered all the information you have, click 'add' to "
      "add the project to the database.  If you have left anything out "
      "or do not have authority to add projects, you will find yourself "
      "back at the add project page.<p>";
    break;
    case "addtask":
      ret += "<h2>Adding a Task</h2>"
      "First, click on the 'add new task' link in the project details.  "
      "page.  This will bring up a form "
      "containing the information you need to enter into the system. "
      "Enter each of these pieces of information in turn as required.  "
      "You will need to enter at least a project ID, a task ID, "
      "and a task ID.<p>"
      "<b>Project ID</b>:  This is the project that the task "
      "belongs to.<p>"      
      "<b>Task ID</b>:  This is the unique code for the task that "
      "identifies it in the database.  If you put mutliple words in "
      "this field, it will concatonate them into 'twiki notation'... "
      "so 'project tracker' becomes ProjectTracker.<p>"
      "<b>Task Name</b>: This is a descriptive name for the task."
      "<b>Comments</b>: Any points of note about this task, such as "
      "'everyone who ever worked on this project has died in a "
      "freak accident involving baby dolphins'.<p>"
      "<b>Task Creators</b>: The creators assigned to the task.  "
      "These do not actually have to be members of the project to "
      "be assigned.<p>"
      "<b>Percent complete</b>: How much of the task has been "
      "completed.<p>"
      "<b>Estimated completion</b>: The rough estimate of how long it "
      "will take to complete the project.  If you wish to have this "
      "estimate recorded with the system, check the 'update' checkbox.  "
      "If you do not, it will ignore any estimate you place into the "
      "system.<p>"
      "<b>Task aims</b>: What is the task concerned with.<p>"
      "Once you've entered all the information you have, click 'add' to "
      "add the task to the the project.  If you have left anything out "
      "or do not have authority to add tasks, you will find yourself "
      "back at the add tasks page.<p>";
    break;
    case "addnote":
      ret += "<h2>Adding a Note</h2>"
      "First, click on the 'add new note' link in the project details.  "
      "page.  This will bring up a form "
      "containing the information you need to enter into the system. "
      "Enter each of these pieces of information in turn as required.  "
      "You will need to enter at least a project ID, a note ID, "
      "and a note.<p>"
      "<b>Project ID</b>:  This is the project that the task "
      "belongs to.<p>"      
      "<b>Note ID</b>:  This is the unique code for the note that "
      "identifies it in the database.  If you put mutliple words in "
      "this field, it will concatonate them into 'twiki notation'... "
      "so 'project tracker' becomes ProjectTracker.<p>"
      "<b>Note</b>: The note itself.<p>";
    break;    
    case "modnote":
      ret += "<h2>Modifying a Note</h2>"
      "First, click on the 'edit note' for the note in the project "
      "details.  page.  This will bring up a form "
      "containing the information you need to enter into the system. "
      "Enter each of these pieces of information in turn as required.  "
      "You will need to enter at least a project ID, a note ID, "
      "and a note.<p>"
      "<b>Project ID</b>:  This is the project that the task "
      "belongs to.<p>"      
      "<b>Note ID</b>:  This is the unique code for the note that "
      "identifies it in the database.  If you put mutliple words in "
      "this field, it will concatonate them into 'twiki notation'... "
      "so 'project tracker' becomes ProjectTracker.<p>"
      "<b>Note</b>: The note itself.<p>";
    break;        
    case "modtask":
      ret += "<h2>Modifying a Task</h2>"
      "First, click on the 'edit' link for the task in the project "
      "details.  page.  This will bring up a form "
      "containing the information you need to enter into the system. "
      "Enter each of these pieces of information in turn as required.  "
      "You will need to enter at least a project ID, a task ID, "
      "and a task ID.<p>"
      "<b>Project ID</b>:  This is the project that the task "
      "belongs to.<p>"      
      "<b>Task ID</b>:  This is the unique code for the task that "
      "identifies it in the database.  If you put mutliple words in "
      "this field, it will concatonate them into 'twiki notation'... "
      "so 'project tracker' becomes ProjectTracker.<p>"
      "<b>Task Name</b>: This is a descriptive name for the task."
      "<b>Comments</b>: Any points of note about this task, such as "
      "'everyone who ever worked on this project has died in a "
      "freak accident involving baby dolphins'.<p>"
      "<b>Task Creators</b>: The creators assigned to the task.  "
      "These do not actually have to be members of the project to "
      "be assigned.<p>"
      "<b>Percent complete</b>: How much of the task has been "
      "completed.<p>"
      "<b>Estimated completion</b>: The rough estimate of how long it "
      "will take to complete the project.  If you wish to have this "
      "estimate recorded with the system, check the 'update' checkbox.  "
      "If you do not, it will ignore any estimate you place into the "
      "system.<p>"
      "<b>Task aims</b>: What is the task concerned with.<p>"
      "Once you've entered all the information you have, click 'add' to "
      "add the task to the the project.  If you have left anything out "
      "or do not have authority to add tasks, you will find yourself "
      "back at the add tasks page.<p>";
    break;    
    case "modproject":
      ret += "<h2>Modifying a Project</h2>"
      "First, click on the 'edit' link for the project.  This will bring up "
      "a form containing the information you need to enter into the system. "
      "Enter each of these pieces of information in turn as required.  "
      "You will need to enter at least a project ID, a task ID, "
      "and a task ID.<p>"
      "<b>Project ID</b>:  This is the unique code for the project that "
      "identifies it in the database.  If you put mutliple words in "
      "this field, it will concatonate them into 'twiki notation'... "
      "so 'project tracker' becomes ProjectTracker.<p>"
      "<b>Project Name</b>: This is a descriptive name for the project.  "
      "This differs from the project ID in that the project ID is used "
      "internally within the tracker, whereas the name is used "
      "externally, for example in domain finger info and the list of "
      "projects in playtesting.<p>"
      "<b>Project Leader</b>: The leader for the project.  Project leaders "
      "have full access over the project in the tracker, and can update the "
      "project file, and add tasks and notes as required.  See " 
      "<a href=project.c?misc=instructions&cat=access>Access Rights</a> "
      "for more information on how access rights work.<p>"
      "<b>Project Domains</b>: The domains the project belongs to.  You "
      "must be a HL or the lord of at least one of the domains to add "
      "a project to the tracker.<p>"
      "<b>Project Creators</b>: The creator assigned to the project.  "
      "These do not actually have to be members of the domain to "
      "be assigned, but if they are not they will not have their "
      "project displaying in their finger info.<p>"
      "<b>Project Playtesters</b>: The playtesters who are assigned "
      "to this project.  The lord of the domain and the lord or "
      "deputies of the playtesting domain can modify this field.<p>"
      "<b>Project Completeness</b>: How complete the project is.  If "
      "you set a value for this, it will over-ride the automatic "
      "calculation used.  If you leave this blank, the project "
      "completeness will be calculated by adding together the completess "
      "of all tasks and subprojects.  For projects that do not have either, "
      "the value will need to be updated manually.<p>"
      "<b>Subproject IDs</b>: The larger projects that form part of "
      "the main project.  While these stand as projects in themselves, "
      "their state of completion reflects on that of the main project.  "
      "The percentage of a project's completion will be modified by "
      "how complete any sub projects are.<p>"
      "<b>Project Files</b>: Any relevant files related to the project.<p>"
      "<b>Project Twiki</b>: Any relevant twiki pages for the project.  "
      "If you simply enter a twiki name (such as ProjectTracker) it will "
      "assume the twiki page may be found on the project's first domain.  "
      "So if you have a project belonging to Forn and Main, it will "
      "assume you mean Forn.ProjectTracker.  If you wish to direct the "
      "link to a particular twiki page, you will need to give the web, then "
      "the page... so Main.ProjectTracker.<p>"
      "<b>Update estimate</b>: The rough estimate of how long it "
      "will take to complete the project.  If you wish to have this "
      "estimate recorded with the system, check the 'update' checkbox.  "
      "If you do not, it will ignore any estimate you place into the "
      "system.<p>"
      "<b>Project status</b>: The status of the project.  Heap refers "
      "to a project that is still conceptual.  Development is a project "
      "that is actively being coded.  Playtesting is a project that has "
      "been approved for playtesting and is undergoing revisions.  "
      "Play is a project that has been put into the game.  Limbo is "
      "for projects that fall somewhere between these stages... mainly "
      "projects that are no longer conceptual, but are not under active "
      "development.<p>"
      "<b>Description</b>: The description of what the project actually "
      "involves.<p>"
      "Once you've entered all the information you have, click 'update' to "
      "modify the project in the database.  If you have left anything out "
      "or do not have authority to add projects, you will find yourself "
      "back at the modify project page.<p>";
    break;    
    case "queryprojects":
      ret += "<h2>Querying projects</h2>"
      "First, click on the 'query projects' link.  This will bring up "
      "a form containing the information you need to enter into the system. "
      "Enter each of these pieces of information in turn as required.  If "
      "you do not enter anything, you will simply get the list of all "
      "projects in the system (the same one you get by clicking "
      "'list projects').<p>"
      "<b>Project Domains</b>: The domains the project belongs to. This is "
      "an AND search... if you enter two domains here, it will look for "
      "projects that belong to domain A and domain B.<p>"
      "<b>Project creators</b>: The creators assigned to the project. "
      "This is an AND search... if you enter two creators here, it will "
      "look for projects that are assigned to creator A and creator B.<p>"
      "<b>Project playtesters</b>: The playtesters assigned to the project. "
      "This is an AND search... if you enter two PTs here, it will "
      "look for projects that are assigned to PT A and PT B.<p>"
      "<b>Minimum completeness</b>: Match projects only that have at "
      "least this level of completeness.  So if you want only projects "
      "that are at least half completed, you would search for 50.<p>"
      "<b>Project leader</b>: Search for the projects that this creator "
      "is a leader of.<p>"
      "<b>Project status</b>: Search for projects that have a specific "
      "status.  'all' will match all projects, regardless of status.<p>"
      "<b>Last touched</b>: Return only projects that have been touched "
      "in the specified amount of time.<p>"
      "These queries are cumulative, so if you enter 'drakkos' in the "
      "creator box, and 'forn' in the domain box, you will get all the "
      "projects in the forn domain that have drakkos as a creator, and "
      "so forth.<p>";   
    break;        
    default:
      ret += "This is the creator project management system for "
      "the Discworld MUD.<p>  "
      "The interface is hopefully fairly intuitive.  The top row of links "
      "are project options.  You can add a project if you have the authority "
      "to do so, list all the projects in the database, or list projects "
      "according to whatever information you are looking for.<p>"
      "The next row of links are the domains covered by the project tracker.  "
      "Click on a domain to get the project list for that domain only.<p>"
      "When you see a project you are interested in, click on 'details' for "
      "the project details and a list of any tasks/notes assosciated with "
      "that project.  Click on edit if you wish to alter the details for that "
      "project.  If you wish to delete a project, you must first click on "
      "edit and then the delete link from there.  That may seem awkwardly "
      "obscure, but it's intentional in that you have to follow a process to "
      "delete an entry from the database, rather than being able to do it "
      "with a misplaced click.<p>"  
      "<h2>Thx bye!</h2>"
      "The system as, at the moment, still undergoing adjustments and "
      "modifications.  If you have any feedback on the interface or requests "
      "for further functionality, mail Drakkos with your suggestions.<p>"
      "Thx bye, flyheads!<p>";      
  }
  
    
  return ret;
}

string www_request (string name, mapping args, class http_request request) {
  
  mapping data = request->body->data;
  class project *tmp;
  string ret = "";
  int i;

  if (request->user != "drakkos") {
    tell_creator ("drakkos", "%s: %d.\n", request->user, sizeof (data));
  }
    
  switch (args["tasktype"]) {
    case "Details":
      ret = get_view_page (get_view_task (args["pid"], args["tid"]),
        args["pid"], "Task Details");
    break;   
    case "Delete":
      if (PROJECT_HANDLER->valid_operation (request->user, TASK, 
        args["pid"])) {      
        PROJECT_HANDLER->delete_task(args["pid"], args["tid"], request->user);
        ret = get_view_page(get_view_project(args["pid"], 1), args["pid"], 
          "Task deleted.");
      }
      else {
        ret = get_web_page ("", "You do not have the authority to "
          "perform this operation.");
       }      
    break; 
    case "Add":
      ret = get_web_page (get_add_task(data, args["pid"]), "Adding a task");
    break;     
    case "Edit":
      ret = get_web_page (get_modify_task(args["pid"], args["tid"]), 
        "Editting a task");    
    break;          
  }    

  switch (args["notetype"]) {
    case "Details":
      ret = get_view_page (get_view_note (args["pid"], args["nid"]), 
        args["pid"], "Note Details");
    break;   
    case "Delete":
      if (PROJECT_HANDLER->valid_operation (request->user, NOTE, 
        args["pid"])) {      
        PROJECT_HANDLER->delete_note(args["pid"], args["nid"], request->user);
        ret = get_view_page(get_view_project(args["pid"], 1), args["pid"],
          "Note deleted.");
      }
      else {
        ret = get_web_page (get_modify_project (args["pid"]), "You do not "
          "have the authority to perform this operation.");
       }      
    break; 
    case "Add":
      ret = get_web_page (get_add_note(data, args["pid"]), "Adding a note");
    break;     
    case "Edit":
      ret = get_web_page (get_modify_note(args["pid"], args["nid"]), 
        "Editting a note");    
    break;           
  }  

  switch (args["filetype"]) {
    case "Add":
      ret = get_web_page (get_add_file(data, args["pid"]), "Adding File "
        "Permission");
    break;     
    case "Delete":
      if (PROJECT_HANDLER->valid_operation (request->user, FILE, 
        args["fid"])) {    
        tell_creator ("drakkos", "%O.\n", args);
        i = PROJECT_HANDLER->remove_file(args["pid"], args["fid"], 
          args["task"],
          request->user);
        tell_creator ("drakkos", "%d.\n", i);
        ret = get_view_page(get_view_project(args["pid"], 1), args["pid"], 
          "File Permissions deleted.");
      }
      else {
        ret = get_web_page ("", "You do not have the authority to "
          "perform this operation.");
       }        
    break;
  }  
    
  switch (args["projecttype"]) {
    case "Delete":
      if (PROJECT_HANDLER->valid_operation (request->user, PROJECT, 
        args["pid"])) {      
        PROJECT_HANDLER->delete_project (args["pid"], request->user);
        ret = get_web_page (get_project_list(), "Project deleted.");
      }
      else {
        ret = get_web_page (get_modify_project (args["pid"]), "You do not "
          "have the authority to perform this operation.");
       }       
    break;  
    case "Add":
      ret = get_web_page (get_add_project(), "Adding a project");
    break;    
    case "List":
      ret = get_web_page (get_project_list(), "Project List");
    break;        
    case "Details":
      ret = get_view_page (get_view_project(args["pid"], 1), args["pid"], 
        "Project Details");
    break;    
    case "Edit":
      ret = get_web_page (get_modify_project(args["pid"]), 
        "Editting a project");
    break;
    case "File":
      ret = get_web_page (get_files_summary (args["pid"]), 
        "File Permission Details");
    break;    
      
  }
  
  switch (args["misc"]) {    
    case "Query":
      ret = get_web_page (get_query_project(), "Query Projects");
    break;  
    case "Domlist":
      ret = get_web_page (get_domain_project_list(args["domain"]), "Projects "
        "in the " + args["domain"] + " domain");
    break;
    case "goto":
      ret = get_web_page (get_modify_project(data["Goto"]), "Project "
        "Details");
    break;
    case "Crelist":
      ret = get_web_page (get_creator_project_list (args["creator"]), 
          "Projects for " + capitalize (args["creator"]));
      break;
    case "instructions":
      ret = get_web_page (get_instructions (args["cat"]), 
          "Project Manager instructions");
    break;
    case "Week":
      tmp = PROJECT_HANDLER->filter_by_query((["touched" : 
      (7 * 24 * 60 * 60)]));
  
      if (!sizeof (tmp)) {
        ret = get_web_page ("There are no projects that have been touched "
          "within the past week.", "Projects Updated This Week");
      }
      else {
        ret = get_web_page (parse_details (tmp), "Projects Updated This Week");    
      }
    break;     
    case "Playtesting":
      tmp = PROJECT_HANDLER->filter_by_query((["status" : 
        "playtesting"]));
  
      if (!sizeof (tmp)) {
        ret = get_web_page ("There are no projects in playtesting.", 
          "Projects In Playtesting");
      }
      else {
        ret = get_web_page (parse_details (tmp), "Projects In Playtesting");    
      }
    break;       
  }
                
  switch (data["button"]) {
    case "Add":
      ret = do_add_project (data, request->user);
    break;
    case "Add Task":
      ret = do_add_task (data, request->user);
    break;
    case "Add Note":
      ret = do_add_note (data, request->user);
    break;
    case "Add File":
      ret = do_add_file (data, request->user);
    break;
    case "Query":
      ret = get_web_page (web_query (data), "Query Completed");
    break;
    case "Update":
      ret = do_update_project (data, request->user);
    break;
    case "Update Task":
      ret = do_update_task (data, request->user);
    break;
    case "Update Note":
      ret = do_update_note (data, request->user);
    break;            
  }    
  
  if (!sizeof (ret)) {
  tmp = PROJECT_HANDLER->filter_by_query((["touched" : 
   (7 * 24 * 60 * 60)]));
  
  ret = get_web_page (parse_details (tmp), "Projects Updated This Week");

  
  }    

//  ret = "<h1>Come back later, Drakkos is breaking stuff.</h1>";
   
  return ret;
} 





