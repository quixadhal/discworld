/**
 *  Applications handler
 *  @author Drakkos
 *  @started 30/01/2003
 *
 */
#include <board.h>
#include <mail.h>
#include <player.h>
#include <playerinfo.h>
#include <player_handler.h>
#include <applications.h>
#include <library.h> 
 
int find_application (string, string);
int find_type (string);
void mail_message(int, int);
string post_application (int);
int complete_application (int); 
  
nosave class application_type *types = ({ });
class application *applications = ({ });
mapping domain_lockout = ([
     "liaison"      : 0,
     "am"           : 0,
     "ram"          : 0,
     "sur"          : 0,
     "klatch"       : 0,
     "forn"         : 0,
     "cwc"          : 0,
     "playtesters"  : 0
   ]);
   

void load_me() {
  unguarded ((: restore_object (APPLY_SAVE_FILE) :));
}

void save_me() {
  unguarded ((: save_object (APPLY_SAVE_FILE) :));
}
   
mapping query_vouches_for_all (string name) {
  mapping tmp = ([ ]);
  
  for (int i = 0; i < sizeof (applications); i++) {
    if (!undefinedp(applications[i]->vouches[name])) {
      tmp[applications[i]->name + " (" + applications[i]->type + ")"] 
        = applications[i]->vouches[name];
    } 
  }
  return tmp;
}
   
varargs void add_application_type (string t, int v, string b, int g, int q, int a, 
  int ti, string *qu, string intro, string responsibility) {
  class application_type tmp;
  
  tmp = new (class application_type, type: t, vouches_required: v, board: b,
    gl_requirement: g, qp_requirement: q, age_requirement: a, questions: qu, 
    intro_text: intro, timeout: ti, responsibility: responsibility);
    
  types += ({ tmp });
  

}

int toggle_domain_lock (string dom) {
  object ob = load_object ("/d/" + dom + "/master");
  
  if (ob->query_lord() != this_player()->query_name()) {
    return -1;
  }
  
  if (domain_lockout[dom]) {
    domain_lockout[dom] = 0;
  }
  else {
    domain_lockout[dom] = 1;
  }
  save_me();
}

int query_applications_accepted(string dom) {
  return (undefinedp (domain_lockout[dom]) ? -1 : domain_lockout[dom]);
}  

string query_responsibility (string type) {
  int i;
  object ob;
  string dom;
   
  i = find_type (type);
  
  if (i == -1) {
    dom = type;
    type = "creator";
    i = find_type (type);
  }
  else {
    dom = type;
  }
 
  
   
  if (undefinedp (types[i]->responsibility)) {
    ob = load_object ("/d/" + dom + "/master");
    
    if (!ob) {
      tell_creator ("drakkos", "%s.\n", dom, type);
      return 0;
    }
    return ob->query_lord();
  }
  else {
    ob = load_object ("/d/" + types[i]->responsibility+ "/master");
    
    if (!ob) {
      return "No-one";
    }
    return ob->query_lord();
  }    
}


mapping query_domain_status() {
  return copy (domain_lockout);
}
int add_application (string t, string n, string te, string dom) {
  class application tmp;
  int i;
  
  i = find_application (n, t);
  
  if (i != -1) {
    return 0;
  }
  
  tmp = new (class application, type: t, name: n, text: te, time: time(),
    domain: dom, vouches: ([ ]));
 
  applications += ({ tmp });
  i = find_application (n, t);
  
  complete_application (i);

  save_me();

  return 1;
    
}

void create() {
  seteuid (geteuid());
  load_me();
  add_application_type ("creator", 2, "applications", 150, 0,  5 * DAY, 14 * DAY, 
    ({
      "Why do you want to be a Creator on " + mud_name() + "?",
      "Do you have (or have you had) other characters on " + mud_name() + ", and if "
        "so what are their names?",
      "How many of the " + mud_name() + " books have you read?  How many do you "
        "own?",
      "Have you been into trouble with the Creators here on " + mud_name() + "?  "
        "Have you been suspended or banned from a MUD?  If so, why?",
      "Are you now, or have you been, a creator (or similar) on any other "
        "mud/mush/talker/etc.?",
      "How much time (in hours per week) do you think you can commit to "
        "" + mud_name() + "?",
      "What is your programming experience? Can you code in C or, more "
        "specifically, the variant of C known as LPC which the mud uses?",
      "What are you interested in creating on "
        "" + mud_name() + "?  Do you have any particular ideas or projects on "
        "which you would like to eventually work?",
      "Do you already know anyone who is a Creator here, who you "
        "have talked to about your application and has agreed to recommend "
        "you?  If so, who?",
      "Do you have anything else you'd like to tell us?",
        "To the best of your knowledge, is the information you have "
        "submitted in this application complete and correct?"
    }), 
    "You will be asked a number of questions.  There is no need "
      "to enter all your information during the first question, as you "
      "will be given plenty of opportunities to answer questions "
      "throughout your application.  If you would like to see what "
      "questions you will be asked, please read the list of questions.\n\n"
      "Please answer all the questions given, and you will get an "
      "opportunity at the end to let us know anything else about you "
      "that seems relevant.\n\n"
    "You will also be asked if the information you are submitting is "
      "true - lying on your creator application in an attempt to gain "
      "promotion under false pretenses is a serious offence... if you "
      "have withheld information that is pertinent to your application, "
      "or have willfully misrepresented the facts then you may find "
      "yourself subject to dismissal.  Please make an effort to be "
      "thorough and honest in your answers - in the long run, it will "
      "be much better for you than hiding anything you think may reflect "
      "badly on your application.\n");
  add_application_type ("liaison", 2, "applications", 150, 0,  5 * DAY, 14 * DAY,
   ({
      "Why do you want to be a Liaison on " + mud_name() + "?",
      "What experience do you already have from " + mud_name() + " which is relevant "
        "to this position?",
      "What experience do you have from the Roundworld (real life) which is "
        "relevant to this position?",
      "Do you have (or have you had) other characters on " + mud_name() + ", and if so "
        "what are their names?",
      "How many of the " + mud_name() + " books have you read?  How many do you "
        "own?",
      "Have you been into trouble with the Creators here on " + mud_name() + "?  "
        "Have you been suspended or banned from a MUD?  If so, why?",
      "Are you now, or have you been, a creator (or similar) on any other "
        "mud/mush/talker/etc.?",
      "How much time (in hours per week) do you think you can commit to "
        "" + mud_name() + "?  Also, please specify the approximate times that you "
        "usually log on and off (in Seattle time).",
      "Do you already know anyone who is a Creator here, who you "
        "have talked to about your application and has agreed to "
        "recommend you?  If so, who?",
      "Do you have anything else you'd like to tell us?",
      "To the best of your knowledge, is the information you have submitted "
        "in this application complete and correct?",
    }),
    "You will be asked a number of questions.  There is no need "
      "to enter all your information during the first question, as you "
      "will be given plenty of opportunities to answer questions "
      "throughout your application.  If you would like to see what "
      "questions you will be asked, please read the list of questions.\n\n"
      "Please answer all the questions given, and you will get an "
      "opportunity at the end to let us know anything else about you "
      "that seems relevant.\n\n"
    "You will also be asked if the information you are submitting is "
      "true - lying on your creator application in an attempt to gain "
      "promotion under false pretenses is a serious offence... if you "
      "have withheld information that is pertinent to your application, "
      "or have willfully misrepresented the facts then you may find "
      "yourself subject to dismissal.  Please make an effort to be "
      "thorough and honest in your answers - in the long run, it will "
      "be much better for you than hiding anything you think may reflect "
      "badly on your application.\n");
        
  add_application_type ("playtester", 3, "playtesters", 150, 0,  15 * DAY, 
    14 * DAY, 
    ({
      "Why do you want to be a playtester on " + mud_name() + "?",
      "Do you have (or have you had) other characters on " + mud_name() + ", and if so "
        "what are their names?",
      "What qualities do you feel you can bring to the role?",
      "Do you have any experience in a similar position in real life or "
        "on another MUD?",
      "Do you have anything else you'd like to tell us?",
      "To the best of your knowledge, is the information you have submitted "
        "in this application complete and correct?", 
    }), 
    "You will be asked a number of questions.  There is no need "
      "to enter all your information during the first question, as you "
      "will be given plenty of opportunities to answer questions "
      "throughout your application.  If you would like to see what "
      "questions you will be asked, please read the list of questions.\n\n"
      "Please answer all the questions given, and you will get an "
      "opportunity at the end to let us know anything else about you "
      "that seems relevant.\n\n"
    "You will also be asked if the information you are submitting is "
      "true - lying on your creator application in an attempt to gain "
      "promotion under false pretenses is a serious offence... if you "
      "have withheld information that is pertinent to your application, "
      "or have willfully misrepresented the facts then you may find "
      "yourself subject to dismissal.  Please make an effort to be "
      "thorough and honest in your answers - in the long run, it will "
      "be much better for you than hiding anything you think may reflect "
      "badly on your application.\n", 
      "playtesters");
   
} 

/**
 *  Find a particular application.
 *
 *  @param name The name of the candidate.
 *  @param type The type of application.
 *  @return the index of the candidate, or -1 if none found.
 *
 */
 
int find_application (string name, string type) {
  for (int i = 0; i < sizeof (applications); i++) {
    if (applications[i]->name == name && applications[i]->type == type) {
      return i;
    }
  }
  return -1;
}

/**
 *  Find a particular application type.
 *
 *  @param type The type of application.
 *  @return the index of the type, or -1 if none found.
 *
 */
 
 
int find_type(string type) {
  for (int i = 0; i < sizeof (types); i++) {
    if (types[i]->type == type) {
      return i;
    }
  }
  return -1;
}

void reset() {
  int j;
  class application *tmp = ({ });
  for (int i = 0; i < sizeof (applications); i++) {
    j = find_type (applications[i]->type);
    
    if (applications[i]->time + types[j]->timeout < time()) {
      tmp += ({ applications[i] });
      mail_message (0, i);
    }  
  }
  
  if (sizeof (tmp)) {
    applications -= tmp;
    save_me();
  }
  
}

int delete_vouch (string who, string name, string position) {
  int i = find_application (name, position);
  
  if (i == -1) {
    return 0;
  }
  if (undefinedp (applications[i]->vouches[who])) {
    return 0;
  }
  map_delete (applications[i]->vouches, who);
  save_me();
  return 1;
}
/**
 *  Deletes an application from the system.
 *
 *  @param name The name of the aplicant
 *  @param type The type of application.
 *  @return 1 for success, 0 for failure.
 *
 */
 
int delete_application (string t, string n) {
  int i = find_application (n, t);
  
  if (i == -1) {
    return 0;
  }
  
  applications -= ({ applications[i] });
  save_me();
  
  return 1;
}

/**
 *  Marks an application as completed... as in, all the vouches have been 
 *  received.
 *
 *  @param i The index of the application.
 *  @return 1 for success, 0 for failure.
 *
 */
 
private int complete_application (int i) {
  int vouches = sizeof (keys (applications[i]->vouches));
  int j;
  
  j = find_type (applications[i]->type);
  
  if (vouches >= types[j]->vouches_required) {
    post_application (i);
    delete_application (applications[i]->type, applications[i]->name);
    save_me();
    return 1;    
  }
  return 0;
}

/**
 *  Returns the number of vouches a player has received for an application.
 *
 *  @param i The index of the application.
 *  @return 1 for success, 0 for failure.
 *
 */
 
int number_of_vouches (string name, string type) {
  int i = find_application (name, type);
  
  if (i == -1) {
    return 0;
  }
  
  return sizeof (keys (applications[i]->vouches));
  
}

/**
 *  Query the vouch for a particular player in a particular category.
 *
 *  @param t The type of application.
 *  @param p The name of the player.
 *  @param n The name of the voucher.
 *  @return the text of the vouch or 0 if none exists.
 *
 */

string query_vouch (string t, string p, string n) {
  int i = find_application (p, t);
  
  if (i == -1) {
    return 0;
  }

  if (undefinedp (applications[i]->vouches[n])) {
    return "You have no vouch for this player in this position.\n";
  }
  return applications[i]->vouches[n];
}

int make_vouch (string t, string p, string n, string v) {
  int i = find_application (p, t);
  int newv = 1;
  
  if (i == -1) {
    return -1;
  }
  
  if (n == applications[i]->name) {
    return 0;
  }
  
  if (undefinedp (applications[i]->vouches[n])) {
    newv = 2;
  }
  applications[i]->vouches[n] = v;
  complete_application (i);
  save_me();
  return newv;
}

string* query_questions (string type) {
  int i = find_type (type);
  
  return types[i]->questions;
}

string query_intro_text (string type) {
  int i = find_type (type);
  
  return types[i]->intro_text;
}

int valid_application (string name, string type) {
  int i = find_type (type);
  object ob = find_player (name);

  if (ob->query_level() < types[i]->gl_requirement) {
    return 0;
  }
  if (-1 * ob->query_time_on() >= types[i]->age_requirement) {
    return 0;
  }
  
  if (LIBRARY->query_quest_points (name) < types[i]->qp_requirement) {
    return 0;
  }
  
  return 1;  
}

string application_requirements (string type) {
  int i = find_type (type);
  string ret = "";
  
  if (i == -1) {
    return "Applications of that category are not handled by this "
      "system.";
  }
  
  ret += "Applications for a " + types[i]->type + " position have the "
    "following requirements:\n\n";
  ret += "You need to be at least " + 
      query_time_string (types[i]->age_requirement) + " old.\n";
  ret += "You need to be at least guild level " + types[i]->gl_requirement + ".\n";
  ret += "You need to have at least " + types[i]->qp_requirement + " quest points.\n";
  ret += "You need to have " + types[i]->vouches_required + " vouches.\n";
  ret += "You have " + query_time_string (types[i]->timeout) + " to obtain your "
    "vouches.\n\n";
  ret += "Your application will not be posted for consideration until you have "
    "all your vouches completed.\n";
    

  return ret;  
}

mapping query_application_vouches (string type, string name) {
  int i = find_application (type, name);
  
  if (i == -1) {
    return 0;
  }
  else {
    return applications[i]->vouches;
  }
}


string format_application (int i) {
  string post;
  
  post = "%^BOLD%^Application:%^RESET%^\n"
         "=-=-=-=-=-=-\n\n" +  
         "Age: " + query_time_string(-1 * PLAYER_HANDLER->test_age 
         (applications[i]->name)) + "\n" + 
         "Application made: " + ctime (applications[i]->time) + "\n\n"
         + applications[i]->text + "\n\n";
         
  post += "\n\n%^BOLD%^Vouches:%^RESET%^\n"
          "=-=-=-=-\n";
          
  foreach (string v, string t in applications[i]->vouches) {
    post += "\n\n%^CYAN%^" + capitalize (v) + " (" + query_time_string (-1 * 
      PLAYER_HANDLER->test_age (v)) + "):%^RESET%^\n\n" + t;  
  }
  
  return post;
}

private string post_application (int i) {
  string post = "";
  int j = find_type (applications[i]->type);
  class reply_type rep;
  object ob, player;
  string response;

  post = format_application (i);

  rep= new(class reply_type);
  rep->type = B_REPLY_NAMED;
  rep->data = query_responsibility((applications[i]->domain ? applications[i]->domain : 
    applications[i]->type));
       
  ob = load_object ("/d/" + (applications[i]->domain ? 
    applications[i]->domain : types[j]->responsibility) + "/master");
  
  BOARD_HAND->add_message( types[j]->board, capitalize( applications[i]->name),
    capitalize( applications[i]->name ) + "'s " + (applications[i]->type == "creator" ? 
       applications[i]->domain + " domain" : capitalize (applications[i]->type)) + 
       " application", post, 0, rep );

  AUTO_MAILER->auto_mail( query_responsibility((applications[i]->domain ? 
    applications[i]->domain : applications[i]->type)), 
    applications[i]->name,
    "Application to " + capitalize( applications[i]->domain),
    "", post, 0, 0 );


  player = find_player (applications[i]->name);
  
  if (ob) {
    if (function_exists ("creator_application_callback", ob)) {
        response = call_other (ob, "creator_application_callback", 
        applications[i]->name, post);              
     }
   
    if (sizeof (response)) {
      tell_object (player, response);
    }
  }   

  if (applications[i]->type == "creator" ||  applications[i]->type == 
    "liaison") {
    PLAYERINFO_HANDLER->add_entry( this_object(), 
      applications[i]->name, "misc", "Applied to " 
        + applications[i]->domain + " domain." );  
  }
  else {
    PLAYERINFO_HANDLER->add_entry( player, 
      player->query_name(), "misc", "Applied as a " 
        + applications[i]->type+ "." );  
  }  
  
  mail_message (1, i);
  save_me();
  
  return post;
  
}

private void mail_message(int i, int j) {
  string ret = "";
  string mail;
  
  mail = "Greetings... this is an automated message from the "
    "" + mud_name() + " applications handler.\n\n$message$\n\n";
    
  switch (i) {
    case 0:
      ret = "You have not received the required number of vouches for "
        "your application for a " + applications[j]->type + " position "
        "within the specified time-frame.  Your application has been "
        "expired and if you wish to pursue further employment on the MUD "
        "you will need to resubmit an application.\n";
    break;
    case 1:
      ret = "You have received the required number of vouches for your "
        "application for a " + applications[j]->type + " position on the "
        "MUD.  Your application has been posted for consideration.  Thank "
        "you for your interest in pursuing employment within the MUD.\n";
    break;
  }            
  
  mail = replace (mail, ({"$message$", ret}));

  AUTO_MAILER->auto_mail( applications[j]->name, "Applications Procedure", 
    "Your " + applications[j]->type + " application", "",
    mail, 0, 0, 1 );
}

string list_applications() {
  string *ret = ({ });
  ret += ({"\n"});
  ret += ({"\n\n/---------------------------------------------------------------------------\\"});
  ret += ({sprintf ("| %|12s | %|12s | %|24s | %|2s  | %|10s |", 
  "Type", "Name", "Made At", "V", "Domain") });
  ret += ({"|---------------------------------------------------------------------------|"});

  for (int i = 0; i < sizeof (applications); i++) {
    ret += ({sprintf ("| %|12s | %|12s | %|s | %|2d  | %|10s |", 
    capitalize (applications[i]->type), 
      capitalize (applications[i]->name), ctime (applications[i]->time), 
        sizeof (applications[i]->vouches), 
        capitalize ((applications[i]->type == "creator" ? applications[i]->domain : 
        ""))) });
  }
  ret += ({"\\---------------------------------------------------------------------------/"});
  
  return implode (ret, "\n");
}

string *query_types() {
  return map (types, (: $1->type :));
}

int fix_name (string n, string n2) {
  int i;
  int c = 0;
  for (int j =0; j < sizeof (types); j++) {
    i = find_application (n, types[j]->type);
  
    if (i != -1) {
      applications[i]->name = n2;
      c++;
    }
  } 
  
  return c; 
}

string query_name() {
  return "Applications Handler";
}
