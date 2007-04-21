/**
 *  Apply command
 *  @author Drakkos
 *  @author Pinkfish
 *  @started 31/01/2003
 *
 */
#include <applications.h>
#include <board.h>
#include <playerinfo.h>
#include <mail.h>
#include <player.h>

inherit "/cmds/base";

varargs void begin_application (object, string, string);

class applying_player {
   int step;
   object player;
   string domain;
   string next_method;
   string type;
   string *questions;
   string *answers;
}

class vouch_info {
  string applicant;
  string voucher;
  string position;
}

void create() {
  seteuid (geteuid());
} 


int do_creator_apply(string domain, string type) {
  int temp;
  
  if (!domain) {
    domain = type;
  }
  
  if( this_player()->query_property( "guest" ) ) {
    write( "Only people with permanent characters are allowed to "
      "become Creators.\n" );
    return 1;
  }

  temp = APPLICATIONS_HANDLER->query_applications_accepted( domain );
  
  if( temp == -1 ) {
    this_player()->add_failed_mess( this_object(), "The domain " + 
       domain + " is not a valid domain.\n" );
    return 0;
  }
  
  if( temp == 0 ) {
    this_player()->add_failed_mess (this_object(), "The domain " +
    domain + " is not currently open for applications.\n");
    return 0;
  } 
  
  if( !APPLICATIONS_HANDLER->valid_application (this_player()->query_name(), 
    type ) && !this_player()->query_lord() ) {
      write( "Sorry, you have not fulfilled the requirements yet.\n" );
      return 1;
  }
    
  if (domain == "liaison") {
    type = "liaison";
  } 
  
  if (strsrch (domain, "playtester") != -1) {
    type = "playtester";
  } 


  begin_application (this_player(), type, domain);
  return 1;
} /* apply() */
 

void confirm (string message,  class applying_player tmp) {
  
  if (!message || !sizeof (message) || strsrch (message, "no") != -1 
    || strsrch (message, "n") != -1) {
    return;
  }
  else {
    call_other (this_object(), tmp->next_method, tmp->player, tmp);
  }
    
}

string query_application_info (string domain) {
  string ret;
  if( file_size( "/d/" + domain + "/application.txt" ) != -1 ) {
    ret = read_file( "/d/" + domain + "/application.txt" );
  }
  return ret;
}

varargs void begin_application (object player, string type, string domain) {
  class applying_player tmp;
  string ret = APPLICATIONS_HANDLER->query_intro_text (type);

  if( file_size( "/d/" + domain + "/application.txt" ) != -1 ) {
      ret += "\nPlease read the following information about applying to "
             "$C$" + domain + ": \n\n" ;
      ret += query_application_info (domain);
   }

  tell_object (this_player(), ret + "\n");
       
  tmp = new (class applying_player);
  tmp->type = type;
  tmp->domain = domain;
  tmp->answers = ({ });
  tmp->step = 1;
  tmp->next_method = "do_questions";
  tmp->player = player;
  
  tell_object (player, "Do you wish to continue your application?\n");
  this_player()->do_edit( "", "confirm", this_object(), 0, 
    tmp);         
}

varargs void do_questions (object player, class applying_player tmp) {
  if (tmp->step == 1) {
    tmp->questions = APPLICATIONS_HANDLER->query_questions (tmp->type);
  }
  
  tell_object (player, "%^BOLD%^" + tmp->questions[0] 
    + "%^RESET%^\n");

  this_player()->do_edit( "", "start_asking", this_object(), 0, 
    tmp); 
}

void start_asking(string message, class applying_player tmp) {
  object player = tmp->player;
  
  if (!message || sizeof (message) == 0) {
    tell_object (player, "Your application has been aborted.\n");
    return;
  }
  tmp->answers += ({ message });
  
  if (tmp->step == sizeof (tmp->questions)) {
    tell_object (player, "Do you wish to submit your application?\n");
    tmp->next_method = "submit_application";
    this_player()->do_edit( "", "confirm", this_object(), 0, 
      tmp);        
    return;
  }
  
  tell_object (player, "%^BOLD%^" + tmp->questions[tmp->step] 
    + "%^RESET%^\n");
  tmp->step += 1;
  
  this_player()->do_edit( "", "start_asking", this_object(), 0, 
    tmp); 
}

void submit_application (object player, class applying_player tmp) {
  int i;
  string *text = ({ });
    
  for (int j = 0; j < sizeof (tmp->questions); j++) {
    text += ({ "%^CYAN%^" +sprintf( "%2d) %-=*s", ( j + 1), 70, 
      tmp->questions[j] ) + "%^RESET%^\n" });
    text += ({ sprintf( "      %-=*s\n\n", 72, tmp->answers[j] )});
  }
  
  i = APPLICATIONS_HANDLER->add_application (tmp->type, player->query_name(), 
    implode (text, "\n"), tmp->domain);
      
  if (!i) {
    tell_object (player, "Something has gone wrong!\n");
    return;
  }
  else {
    tell_object (player, "Thank you for your application... it has been "
      "placed in the handler ready for people to vouch.\n");
  }
  

}

int do_list_types() {
  string ret = "";
  string *types;
  
  types = APPLICATIONS_HANDLER->query_types();
  
  ret += "You can apply for the following positions with this command:\n\n";
  
  foreach (string t in types) {
    ret += t + "\n";
  }
  
  this_player()->more_string (ret);
  return 1;
  
}

int do_list_requirements(string type) {
  string ret = APPLICATIONS_HANDLER->application_requirements (type);
    
  this_player()->more_string (ret);
  return 1;
  
}

int domain_status() {
  mapping map = APPLICATIONS_HANDLER->query_domain_status();
  string ret = "";
  
  foreach (string s, int o in map) {
    ret += (o ? "%^BOLD%^%^CYAN%^" : "") + capitalize (s) + " is " +
           (o ? "open" : "closed") + "%^RESET%^ to "
      "applications and is managed by " + 
      capitalize (APPLICATIONS_HANDLER->query_responsibility (s)) + ".\n"; 
  }
  
  //this_player()->more_string (ret);
  write("$P$Domain Status$P$" + ret);
  return 1;
  
}

int do_list_questions (string type) {
  string *quest = APPLICATIONS_HANDLER->query_questions (type);
  
  tell_object (this_player(), "\n\nPlease take time to write your application.  "
     "Preferably, prepare an answer to each of these questions off-line "
     "and copy your response into the editor on the MUD if possible.  "
     "You will be asked these questions one-by-one during the application "
     "process, so you should answer each question thoroughly.  Remember, "
     "this is a formal application to a position of responsibility, so "
     "treat each question seriously.\n\n");


  this_player()->more_string (implode (quest, "\n\n"));
  return 1;
}

int do_vouch (string name, string position) {
  int i = APPLICATIONS_HANDLER->find_application (name, position);
  class vouch_info tmp;
  
  tmp = new (class vouch_info);
  
  if (i == -1) {
    write (capitalize (name) + " has not applied for a " 
      + position + " position.\n");
    return 1;
  }
  
  tell_object (this_player(), "%^BOLD%^Why do you think " + capitalize (name) 
    + " would be a suitable " + position + "? %^RESET%^");
  
  tmp->applicant = name;
  tmp->voucher = this_player()->query_name();
  tmp->position = position;
  this_player()->do_edit( "", "vouch_edit", this_object(), 0, 
    tmp);
  return 1;    
}

void vouch_edit(string message, class vouch_info tmp) {
  object ob;
  int i;
  string name = tmp->applicant;
  string position = tmp->position;

  ob = find_player (tmp->voucher);

  if (!sizeof (message) || message == "") {
    if (ob) {
      tell_object (ob, "Your vouch has been aborted.\n");
    }
  }
  else {
    i = APPLICATIONS_HANDLER->make_vouch (position, name, tmp->voucher, message);
  
    switch (i) {
      case 2:
        tell_object (ob, "You submit your vouch for " + capitalize (name) 
          + " as a " + position + ".\n");
      break;
      case 1:
        tell_object (ob, "You resubmit your vouch for " + capitalize (name) 
          + " as a " + position + ".\n");
      break;
      case 0:
        tell_object (ob, "You cannot submit a vouch for yourself.\n");
      break;
      case -1:
        tell_object (ob, "There is no application for " + capitalize (name) + 
          " as a " + position + ".\n");
      break;      
    }
  }      
 

  

}

int do_delete_vouch (string name, string position) {
  int i = APPLICATIONS_HANDLER->delete_vouch (this_player()->query_name(), 
    name, position);
    
  if (i) {
    tell_object (this_player(), "You delete your vouch for " +
      capitalize (name) + " as a " + position + ".\n");
  }
  else {
    tell_object (this_player(), "You have not vouched for " + 
      capitalize (name) + " as a " + position + ".\n");
  }
  
  return 1;
}

int query_my_vouch (string name, string position) {
  string v = APPLICATIONS_HANDLER->query_vouch (position, name, 
    this_player()->query_name());
    
  if (!v) {
    tell_object (this_player(), "There is no application for " 
      + capitalize (name) + " as a " + position + ".\n");
  }
  else {
    tell_object (this_player(), v + "\n");
  }
  return 1;
}

int delete_application (string pos) {
  int i = APPLICATIONS_HANDLER->delete_application (pos, 
    this_player()->query_name());
    
  if (!i) {
    tell_object (this_player(), "You have no pending applications for "
      "that position.\n");
  }
  else {
    tell_object (this_player(), "Your application has been deleted.\n");
  }       
  return 1;
}

int do_register() {

   if ( this_player()->query_property( "guest" )==1 ) {
      write ( "You are a guest. Registering as a player killer is not an "
        "option for you.\n");
      log_file( "CHEAT", ctime(time()) +": "+
        this_player()->query_short()+
        " (" +query_ip_number(this_player()) + ") (a guest) tried to "
        "register as a player killer.\n");
      return 1;
   }

   if (this_player()->query_player_killer()) {
      add_failed_mess( "You already are a player killer, silly!\n" );
      return 0;
   }

   if (-this_player()->query_time_on() < PKER_MINIMUM_AGE) {
      add_failed_mess( "You are not old enough to be a player killer.  You "
        "must be " + (PKER_MINIMUM_AGE / (60 * 60 * 24)) + " days old to "
        "register as a player killer.\n");
      return 0;
   }

   if (this_player()->query_property(PKER_NOT_ALLOWED)) {
      write( "Due to previous abuses of the playerkiller system, you are "
         "not allowed to become a playerkiller.\n");
      log_file( "CHEAT", ctime(time()) +": "+
        this_player()->query_short()+ " (" +query_ip_number(this_player()) + 
        ") (not allowed to be a pk) tried to register as a player "
        "killer.\n");
      return 1;
   }

   write( "%^BOLD%^%^YELLOW%^You are hereby registering yourself as "
          "a player killer.\n\n"
          "This means that you can attack and be attacked by other "
          "player killers.  Once you are a registered player killer, "
          "there is NO going back.  Please note, this is NOT a quest "
          "and you will NOT get any experience points from doing "
          "it.%^RESET%^\n");
   write( "Are you sure about this, (Y/N)?\n");
   input_to("register2");
   return 1;
} /* register() */

void register2(string str)
{
   str = lower_case(str);
   if (!strlen(str) ||
       (str[0] != 'y' && str[0] != 'n' && str[0] != 'q')) {
      write("You need to type 'y' or 'n'.\n");
      write("Are you sure about this, (Y/N)? ");
      input_to("register2");
      return ;
   }
   if (str[0] == 'n' ||
       str[0] == 'q') {
      write("You have chosen not to register as a playerkiller at this "
         "time.  You may choose to register later if you wish.\n");
   } else {
      write("%^BOLD%^%^YELLOW%^You are now a playerkiller, ready to kill and be killed.%^RESET%^\n");
      //this_player()->add_property(PKER,1);
      this_player()->set_player_killer(1);
      PLAYERINFO_HANDLER->add_entry( this_player(),
        this_player()->query_name(), "misc",
        "Registered as a Player Killer." );
   }
}

int do_query_all_vouches() {
  mapping tmp = APPLICATIONS_HANDLER->query_vouches_for_all 
    (this_player()->query_name());
  string *ret = ({ });
  string tmp2;

  if (!sizeof (tmp)) {
    this_player()->add_failed_mess (this_object(), "You have made no "
      "vouches for anyone.\n", ({ }));
    return 0;
  }
  
  foreach (string n, string v in tmp) {
    tmp2 = "$I$5=%^CYAN%^" + capitalize (n) + "%^RESET%^: " + v;
    ret += ({ tmp2 });      
  }
  this_player()->more_string (implode (ret, "\n\n"));
  return 1;
}

private int do_list_applicants() {
  string ret;
  
  if (!this_player()->query_lord()) {
    return 0;
  }
  
  ret = APPLICATIONS_HANDLER->list_applications();
  
  this_player()->more_string (ret);
  return 1;
}

private int do_browse_app (string type, string name) {
  int i = APPLICATIONS_HANDLER->find_application (name, type);
  string ret;
  
  if (i == -1) {
    this_player()->add_failed_mess (this_object(), "There is no "
      +type + " application for " + name + ".\n", ({ }));
    return 0;
  }
    
  ret = APPLICATIONS_HANDLER->format_application (i);

  this_player()->more_string (ret);
  return 1;
}

int query_my_vouches (string position) {
  mapping i = APPLICATIONS_HANDLER->query_application_vouches 
    (this_player()->query_name(), position);
  
  if (!i) {
    this_player()->add_failed_mess (this_object(), "You currently have no vouches "
      "for a " + position + " application.\n", ({ }));
    return 0;
  }
  else {
    tell_object (this_player(), "The following people have vouched for your " +
      position + " application:  " + query_multiple_short (map (keys (i), 
        (: capitalize ($1) :))) + ".\n");
    return 1;
  }       
}


mixed* query_patterns() {
   string t = "{" + implode (APPLICATIONS_HANDLER->query_types(), "|") 
    + "}";
   mixed *tmp; 
   tmp = ({ 
             "as creator in <word> domain", (: do_creator_apply($4[0], "creator") :),            
             "as player killer", (: do_register() :),
             "as playtester", (: do_creator_apply("playtesters", "playtester") :),            
             "as liaison", (: do_creator_apply(0, "liaison") :),            
             "delete my application as <word'position'>",
              (: delete_application ($4[0]) :),

             "list available positions", (: do_list_types() :),
             "list requirements for " + t + " applications", 
              (: do_list_requirements($4[0]) :),
             "list position status", (: domain_status() :),
             "list questions for " + t + " applications",
              (: do_list_questions ($4[0]) :),

             "vouch for <word'name'> as <word'position'>", 
              (: do_vouch ($4[0], $4[1]) :), 
             "delete vouch for <word'name'> as <word'position'>",
              (: do_delete_vouch ($4[0], $4[1]) :),
             "query vouch for <word'name'> as <word'position'>",
              (: query_my_vouch ($4[0], $4[1]) :),
             "query all my vouches", (: do_query_all_vouches() :),
             "query vouches for my <word> application", (: query_my_vouches($4[0]) :),

  });    
  
  if (this_player()->query_lord()) {
    tmp += ({"browse <word'type'> application for <word'name'>", (: do_browse_app($4[0], $4[1]) :) });    
    tmp += ({"list applicants", (: do_list_applicants() :) });
  }
  
  return tmp;          
} 

void clean_up() {
  return 0;
}

int application_board_access_check( int type, string board,
                                    object previous, string name ) {
   switch(type) {
      case B_ACCESS_READ :
         return 1;
      case B_ACCESS_WRITE :
         if( file_name(previous) == file_name( this_object() ) )
            return 1;
          if (base_name (previous) == APPLICATIONS_HANDLER) {
            return 1;
          }
         return master()->query_lord(name);
      case B_ACCESS_DELETE :
         // Players can always delete their own notes, do not need to code
         // for that.
         return 0;
   }
} /* application_board_access_check() */


void reset() {
}
