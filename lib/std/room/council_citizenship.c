/**
 * This is the place you go to try and become a citizen of the council
 * area.
 * @author Pinkfish
 * @started Wed Sep 27 18:19:22 PDT 2000
 */

#define EXPRESSION_NO_CLASSES 1
#include <expressions.h>
inherit "/std/basic/expressions";
inherit "/std/room/basic_room";
inherit "/std/room/inherit/council_base";
inherit "/std/room/inherit/honours";

#include <nomic_system.h>
#include <player_handler.h>
#include <mail.h>

#define WEEK (7 * (24 * 60 * 60))

class approval_list {
   string* sponsors;
   mapping against;
   int time_added;
}

class approved_person {
   class approval_list info;
   int time_approved;
   string approver;
   int denied;
}

class blacklist {
   string blacklister;
   string reason;
}

class denied {
   string denier;
   string deny_reason;
   int time_denied;
}

private mapping _waiting_approval;
private mapping _blacklist;
private mapping _denied;
private class parse_node* _expression;
private class parse_node* _sponsor_expression;
private mapping _approved;
private int _last_posted;
private nosave string _save_file;

// variables
int variable_age_in_days(object player);
int variable_age_in_hours(object player);
int variable_level(object player);
int variable_denied_in_days(object player);
int function_citizen_of(string area, object player);
int function_magistrate_of(string area, object player);
// do_*
int do_apply();
int do_sponsor(string person);
int do_approve(string person);
int do_deny(string person, string reason);
int do_list();
int do_add_blacklist(string person, string reason);
int do_remove_blacklist(string person);
int do_list_blacklist(int verbose);
int do_list_expression();
int do_set_expression(string expr, int applicant);
int do_list_denied();
void load_me();

void create() {
   _waiting_approval = ([ ]);
   _blacklist = ([ ]);
   _denied = ([ ]);
   _approved = ([ ]);
   _last_posted = time();
   honours::create();
   expressions::create();
   //
   // Add all the functions in here.
   //
   add_allowed_variable("ageindays", EXPRESSION_TYPE_INTEGER,
                        (: variable_age_in_days :));
   add_allowed_variable("ageinhours", EXPRESSION_TYPE_INTEGER,
                        (: variable_age_in_hours :));
   add_allowed_variable("level", EXPRESSION_TYPE_INTEGER,
                        (: variable_level :));
   add_allowed_variable("timesincedeniedindays", EXPRESSION_TYPE_INTEGER,
                        (: variable_denied_in_days :));
   add_allowed_function("citizenof", EXPRESSION_TYPE_BOOLEAN,
                        ({ EXPRESSION_TYPE_STRING }),
                        (: function_citizen_of :));
   add_allowed_function("magistrateof", EXPRESSION_TYPE_BOOLEAN,
                        ({ EXPRESSION_TYPE_STRING }),
                        (: function_citizen_of :));

   add_help_file("citizenship");

   basic_room::create();
} /* create() */

void init() {
   add_command("apply", "[for] [citizenship]", (: do_apply() :));
   add_command("sponsor", "<string'person'> for citizenship",
               (: do_sponsor($4[0]) :));
   add_command("deny", "<string'person'> for citizenship because <string'reason'>",
               (: do_deny($4[0], $4[1]) :));
   add_command("list", "", (: do_list() :));
   add_command("expression", "list",
               (: do_list_expression() :));
   if (NOMIC_HANDLER->is_magistrate_of(query_council_area(),
                                      this_player()->query_name()) ||
       master()->query_lord(this_player()->query_name())) {
      add_command("blacklist", "add <string'person'> for <string'reason'>",
                  (: do_add_blacklist($4[0], $4[1]) :));
      add_command("blacklist", "remove <string'person'>",
                  (: do_remove_blacklist($4[0]) :));
      add_command("blacklist", "list {verbose|brief}",
                  (: do_list_blacklist($4[0] == "verbose") :));
      add_command("expression", "citizen <string'citizenship'>",
                   (: do_set_expression($4[0], 0) :));
      add_command("expression", "sponsor <string'citizenship'>",
                   (: do_set_expression($4[0], 1) :));
      add_command("list", "denied", (: do_list_denied :));
      add_command("approve", "<string'person'> for citizenship",
                  (: do_approve($4[0]) :));
   }
   honours::init();
} /* init() */

/**
 * This method sets the save file.
 * @param fname the name of the save file
 */
void set_save_file(string fname) {
   _save_file = fname;
   load_me();
} /* set_save_file() */

/**
 * This method returns the current save file name.
 * @return the current save file name
 */
string query_save_file() {
   return _save_file;
} /* query_save_file() */

/**
 * This loads all the exciting data for the citizenship room.
 */
void load_me() {
   if (!_save_file) {
      return ;
   }
   unguarded( (: restore_object(_save_file, 1) :));
   if (!_expression) {
      _expression = parse_boolean_string("ageindays >= 2 and timesincedeniedindays > 14");
   }
   if (!_sponsor_expression) {
      _sponsor_expression = parse_boolean_string("ageindays >= 10");
   }
   if (!_approved) {
      _approved = ([ ]);
   }

   remove_call_out("post_weekly_results");
   call_out("post_weekly_results", _last_posted + WEEK - time());
} /* load_me() */

/**
 * This method saves everything!
 */
void save_me() {
   if (!_save_file) {
      return ;
   }
   unguarded( (: save_object(_save_file, 1) :));
} /* save_me() */

/**
 * This method posts the weekly results about the citizenship approvals
 * to the council board.
 */
void post_weekly_results() {
   string post;
   string* cit;
   string person;
   string denied;

   cit = sort_array(keys(_approved), 1);
   if (sizeof(cit) > 0) {
      post = "Here are the weekly citizenship application results:\n\n";
      denied = "";
      foreach (person in cit) {
         if (!_approved[person]->denied) {
            if (sizeof(_approved[person]->info->sponsors)) {
               post += "  " + capitalize(person) + " " +
                      ctime(((class approval_list)_approved[person]->info)->time_added)[4..9] +
                      " sponsors " +
                      query_multiple_short(map(_approved[person]->info->sponsors,
                                               (: capitalize($1) :)));
            } else {
               post += "  " + person + " no sponsor";
            }
            post += " approved " +
                    ctime(_approved[person]->time_approved)[4..9] +
                    " by " + capitalize(_approved[person]->approver) + ".\n";
         } else {
            if (sizeof(_approved[person]->info->sponsors)) {
               denied += "  " + capitalize(person) + " " +
                      ctime(((class approval_list)_approved[person]->info)->time_added)[4..9] +
                      " sponsors " +
                      query_multiple_short(map(_approved[person]->info->sponsors,
                                               (: capitalize($1) :)));
            } else {
               denied += "  " + person + " no sponsor";
            }
            denied += " denied " +
                    ctime(_denied[person]->time_denied)[4..9] +
                    " by " + capitalize(_denied[person]->denier) + ".\n";
         }
      }

      if (strlen(denied) > 0) {
         post += "\n\nThese people have been denied citizenship:\n" +
                 denied;
      }

      NOMIC_HANDLER->post_citizen_message(query_council_area(),
                                         "Citizenship results.",
                                         post);
      _approved = ([ ]);
      save_me();
   }
   _last_posted = time();
   remove_call_out("post_weekly_results");
   call_out("post_weekly_results", _last_posted + WEEK - time());
} /* post_weekly_results() */

/**
 * This is the method used for someone to apply for citizenship.
 * @return 1 on success, 0 on failure
 */
int do_apply() {
   class approval_list womble;
   string str;

   //
   // First check everything that could prevent them from applying,
   // citizenship already, blacklisted...
   //
   if (NOMIC_HANDLER->is_citizen_of(query_council_area(),
                                           this_player()->query_name())) {
      add_failed_mess("You are already a citizen.\n");
      return 0;
   }

   if (_blacklist[this_player()->query_name()]) {
      add_failed_mess("You are not allowed to become a citizen here.\n");
      return 0;
   }

   if (_waiting_approval[this_player()->query_name()]) {
      add_failed_mess("You are already asking for approval.\n");
      return 0;
   }

   if (!evaluate_expression(_expression, this_player())->value) {
      str = query_expression_string(_expression, 0);
      add_failed_mess("You do not meet the requirements to be a citizen, the "
                      "requirements currently are:\n" + str + "\n");
      return 0;
   }

   if (this_player()->query_creator()) {
      add_failed_mess("Creators cannot be citizens.\n");
      return 0;
   }

   //
   // If in the grace period of the council then it is automatically accepted.
   //
   if (NOMIC_HANDLER->is_grace_phase(query_council_area())) {
      // Check and see if they are a citizen anywhere is.
      str = NOMIC_HANDLER->query_citizen(this_player()->query_name());
      if (str) {
         add_failed_mess("You are already a citizen of " + str +
                         " and cannot join here.\n");
         return 0;
      }
      NOMIC_HANDLER->add_citizen(query_council_area(), this_player()->query_name());
      add_succeeded_mess("$N become$s a citizen of " + query_council_area() +
                          ".\n");
      return 1;
   }

   //
   // Ok, put them into the approval list.
   //
   womble = new(class approval_list);
   womble->sponsors = ({ });
   womble->against = ([ ]);
   womble->time_added = time();
   _waiting_approval[this_player()->query_name()] = womble;
   save_me();

   //
   // Notify the magistrates of the event.
   //
   send_council_inform(1,
              this_player()->query_cap_name() + " applied for citizenship in " +
              query_council_area());

   add_succeeded_mess(({ "You apply for citizenship.\n",
                         "$N applies for citizenship.\n" }));
   return 1;
} /* do_apply() */

/**
 * This allows someone to sponsor someone for citizenship.
 */
int do_sponsor(string person) {
   string str;

   if (!PLAYER_HANDLER->test_user(person)) {
      add_failed_mess("Sorry, " + person + " is not a player.\n");
      return 0;
   }

   if (!NOMIC_HANDLER->is_citizen_of(query_council_area(),
                                           this_player()->query_name())) {
      add_failed_mess("Only citizens can sponsor someone.\n");
      return 0;
   }

   if (_blacklist[this_player()->query_name()]) {
      add_failed_mess("You are not allowed to sponsor citizens.\n");
      return 0;
   }

   if (!_waiting_approval[person]) {
      add_failed_mess("I am sorry, " + person + " is not currently "
                      "awaiting citizenship.\n");
      return 0;
   }

   if (!evaluate_expression(_sponsor_expression, this_player())->value) {
      str = query_expression_string(_sponsor_expression, 0);
      add_failed_mess("You do not meet the requirements to sponsor citizen, the "
                      "requirements currently are:\n" + str + "\n");
      return 0;
   }

   if (member_array(this_player()->query_name(),
                    _waiting_approval[person]->sponsors) != -1) {
      add_failed_mess("You are already sponsoring " + person + ".\n");
      return 0;
   }

   if (_waiting_approval[person]->against[this_player()->query_name()]) {
      add_failed_mess("You have already put in a reason for deny "
                      "citizenship to " + person + ".\n");
      return 0;
   }

   _waiting_approval[person]->sponsors += ({ this_player()->query_name() });
   save_me();
   add_succeeded_mess("$N $V someone for citizenship.\n");
   return 1;
} /* do_sponsor() */

/**
 * This method makes the citizen approved by a happy little magistrate.
 * @param person the person to approve
 */
int do_approve(string person) {
   person = lower_case( person );
   if (!PLAYER_HANDLER->test_user(person)) {
      add_failed_mess("Sorry, " + person + " is not a player.\n");
      return 0;
   }

   if (!NOMIC_HANDLER->is_citizen_of(query_council_area(),
                                           this_player()->query_name())) {
      add_failed_mess("Only citizens can sponsor someone.\n");
      return 0;
   }

   if (!_waiting_approval[person]) {
      add_failed_mess("I am sorry, " + person + " is not currently "
                      "awaiting citizenship.\n");
      return 0;
   }

   if (NOMIC_HANDLER->is_magistrate_of(query_council_area(),
                                       this_player()->query_name())) {
      //
      // This turns them into a citizen directly.
      //
      NOMIC_HANDLER->add_citizen(query_council_area(), person);
      _approved[person] = new(class approved_person);
      _approved[person]->info = _waiting_approval[person];
      _approved[person]->time_approved = time();
      _approved[person]->approver = this_player()->query_name();
      map_delete(_waiting_approval, person);
      save_me();
      send_council_inform(1, person + " granted citizenship by " +
              this_player()->query_cap_name() + " in " +
              query_council_area());
      add_succeeded_mess("$N add$s " + person + " as a citizen.\n");

      if (find_player(person)) {
         tell_object(find_player(person), "%^RED%^You have been granted "
                     "citizenship of " + query_council_area() +
                     "%^RESET%^.\n");
      } else {
         AUTO_MAILER->auto_mail(person, "Council of " + query_council_area(),
                                "Citizenship of " + query_council_area(),
                                "",
                                "Morning,\n\nYou have been granted "
                                "citizenship in " + query_council_area() +
                                ".\n\n"
                                "Good luck!\nCouncil Administrator.\n");
      }
      return 1;
   }

   add_failed_mess("You are not a magistrate of " + query_council_area() +
                   ".\n");
   return 0;
} /* do_approve() */

/**
 * This allows you to put in a reason for denying someone for citizenship.
 */
int do_deny(string person, string reason) {
   class denied fluff;

   if (!PLAYER_HANDLER->test_user(person)) {
      add_failed_mess("Sorry, " + person + " is not a player.\n");
      return 0;
   }

   if (!NOMIC_HANDLER->is_citizen_of(query_council_area(),
                                           this_player()->query_name())) {
      add_failed_mess("Only citizens can deny someone.\n");
      return 0;
   }

   if (!_waiting_approval[person]) {
      add_failed_mess("I am sorry, " + person + " is not currently "
                      "awaiting citizenship.\n");
      return 0;
   }

   if (NOMIC_HANDLER->is_magistrate_of(query_council_area(),
                                       this_player()->query_name())) {
      //
      // This denies them directly.
      //
      map_delete(_waiting_approval, person);
      fluff = new(class denied);
      fluff->deny_reason = reason;
      fluff->denier = this_player()->query_name();
      fluff->time_denied = time();
      _denied[person] = fluff;
      save_me();
      if (find_player(person)) {
         tell_object(find_player(person), "%^RED%^You have been denied "
                     "citizenship of " + query_council_area() +
                     "%^RESET%^.\n");
      } else {
         AUTO_MAILER->auto_mail(person, "Council of " + query_council_area(),
                                "Citizenship of " + query_council_area(),
                                "",
                                "Morning,\n\nYou have been denied as a "
                                "citizen of " + query_council_area() + ".\n\n"
                                "Bad luck!\nCouncil Administrator.\n");
      }

      add_succeeded_mess(({ "$N deny " + person + " as a citizen.\n",
                            "$N denies " + person + " as a citizen.\n"
                             }) );
      return 1;
   }

   if (member_array(this_player()->query_name(),
                    _waiting_approval[person]->sponsors) != -1) {
      add_failed_mess("You are already sponsoring " + person + ".\n");
      return 0;
   }

   if (_waiting_approval[person]->against[this_player()->query_name()]) {
      add_failed_mess("You have already put in a reason for deny "
                      "citizenship to " + person + ".\n");
      return 0;
   }

   _waiting_approval[person]->against[this_player()->query_name()] = reason;
   save_me();


   add_succeeded_mess("$N $V someone for citizenship.\n");
   return 1;
} /* do_deny() */

/**
 * This method lists all the current applicants.
 */
int do_list() {
   string ret;
   string ret_tmp;
   string ret_no_sponsor;
   int magistrate;
   string* people;
   string person;
   string denier;
   string reason;
   int index;
   int last;
   int no_sponsor;

   if (!sizeof(_waiting_approval)) {
      add_failed_mess("No one is waiting for citizenship approval.\n");
      return 0;
   }

   if (!NOMIC_HANDLER->is_citizen_of(query_council_area(),
                                           this_player()->query_name())) {
      add_failed_mess("Only citizens can see who is applying for "
                      "citizenship.\n");
      return 0;
   }

   magistrate = NOMIC_HANDLER->is_magistrate_of(query_council_area(),
                                                this_player()->query_name());

   people = keys(_waiting_approval);
   people = sort_array(people, (: ((class approval_list)_waiting_approval[$1])->time_added -
                                  ((class approval_list)_waiting_approval[$2])->time_added :));
   ret = ""; // "People currently waiting for sponsorship:\n";
   ret_no_sponsor = "";
   foreach (person in people) {
      if (!PLAYER_HANDLER->test_user(person)) {
         map_delete(_waiting_approval, person);
         continue;
      } 
      if (sizeof(_waiting_approval[person]->sponsors)) {
         ret += "$I$6=   " +
                ctime(((class approval_list)_waiting_approval[person])->time_added)[4..9] + ": " +
                capitalize(person) + " sponsored by " +
                query_multiple_short(map(_waiting_approval[person]->sponsors,
                                         (: capitalize($1) :)));
         no_sponsor = 0;
      } else {
         ret_no_sponsor += "$I$6=   " +
                ctime(((class approval_list)_waiting_approval[person])->time_added)[4..9] + ": " +
                capitalize(person) + " not sponsored by anyone";
         no_sponsor = 1;
      }

      ret_tmp = "";
      if (sizeof(_waiting_approval[person]->against)) {
         if (magistrate) {
             ret_tmp += ";\n";
             index = 1;
             last = sizeof(_waiting_approval[person]->against);
             foreach (denier, reason in _waiting_approval[person]->against) {
                ret_tmp += "- " + denier + " denied '" + reason + "'";
                /* Appends ";\n" for each denier but the last, only "\n" then */
                if( index == last ) {
                   ret_tmp += "\n";
                } else {
                   ret_tmp += ",\n";
                   index++;
                }
             }
         } else {
             ret_tmp += " denied by " +
                    query_num(sizeof(_waiting_approval[person]->against)) +
                    ".\n";
         }
      } else {
         ret_tmp += ".\n";
      }
      if (no_sponsor) {
         ret_no_sponsor += ret_tmp;
      } else {
         ret += ret_tmp;
      }
   }

   if (ret != "") {
      ret = "$I$0=Waiting for approval:\n" + ret;
   }

   if (ret_no_sponsor != "") {
      ret = "$I$0=Waiting for sponsorship:\n" + ret_no_sponsor + "\n" + ret;
   }

   this_player()->more_string(ret, "citizenship");
   return 1;
} /* do_list() */

/**
 * This method lists the current set of denied people.
 */
int do_list_denied() {
   string* people;
   string person;
   string ret;

   if (!sizeof(_denied)) {
      add_failed_mess("No one has been denied yet.\n");
      return 0;
   }


   if (!NOMIC_HANDLER->is_magistrate_of(query_council_area(),
                                           this_player()->query_name())) {
      add_failed_mess("Only magistrates can see the denied list.\n");
      return 0;
   }

   people = keys(_denied);
   people = sort_array(people, 1);
   ret = "";
   foreach (person in people) {
      ret += "$I$5=" + person + " at " + ctime(_denied[person]->time_denied) +
             " by " + _denied[person]->denier +
             " for " + _denied[person]->deny_reason + ".\n";
   }

   this_player()->more_string(ret, "denied");
   return 1;
} /* do_list_denied() */

/**
 * This method lists the expression used for showing if someone is
 * allowed to be a citizen or not.
 */
int do_list_expression() {
   string str;

   str = query_expression_string(_expression, 0);
   write("The expression to check to make sure someone is allowed to "
         "apply for citizenship is:\n" + str + "\n");
   str = query_expression_string(_sponsor_expression, 0);
   write("The expression to check to make sure someone is allowed to "
         "sponsor an application for citizenship is:\n" + str + "\n");
   return 1;
} /* do_list_expression() */

/**
 * This method sets up the expression to use to check to make sure people
 * are allowed to apply for citizenship.
 * @param expression the expression to set the check to
 * @param sponsor_expr sets the sponsor or the citizen expression
 */
int do_set_expression(string expression, int sponsor_expr) {
   class parse_node* expr;

   if (!NOMIC_HANDLER->is_magistrate_of(query_council_area(),
                                        this_player()->query_name())) {
      add_failed_mess("Only magistrates can set an expression string.\n");
      return 0;
   }

   expr = parse_boolean_string(expression);
   if (!sizeof(expr)) {
      add_failed_mess(query_last_expression_error() + ".\n");
      return 0;
   }

   if (!sponsor_expr) {
      _expression = expr;
   } else {
      _sponsor_expression = expr;
   }
   save_me();

   add_succeeded_mess("$N set$s the citizenship requirements.\n");
   return 1;
} /* do_set_expression() */

/**
 * This method adds a person to the blacklist.
 * @param person the person to add
 * @param reason the reason for them to be blacklisted
 */
int do_add_blacklist(string person, string reason) {
   class blacklist bing;

   if (!NOMIC_HANDLER->is_magistrate_of(query_council_area(),
                                        this_player()->query_name())) {
      add_failed_mess("Only magistrates can manipulate the blacklist.\n");
      return 0;
   }

/*
   if (NOMIC_HANDLER->is_citizen_of(query_council_area(), person)) {
      add_failed_mess("You cannot blacklist someone who is already a "
                      "citizen.\n");
      return 0;
   }
 */

   if (!PLAYER_HANDLER->test_user(person)) {
      add_failed_mess("You cannot blacklist someone that does not exist.\n");
      return 0;
   }

   bing = new(class blacklist);
   bing->blacklister = this_player()->query_name();
   bing->reason = reason;
   _blacklist[person] = bing;
   save_me();

   add_succeeded_mess("$N add$s someone to the blacklist.\n");
   return 1;
} /* do_add_blacklist() */

/**
 * This method removes a person from the blacklist.
 * @param person the person to remove
 */
int do_remove_blacklist(string person) {
   if (!NOMIC_HANDLER->is_magistrate_of(query_council_area(),
                                        this_player()->query_name())) {
      add_failed_mess("Only magistrates can manipulate the blacklist.\n");
      return 0;
   }

   if (!_blacklist[person]) {
      add_failed_mess("You cannot remove someone from a blacklist that is not "
                      "black listed already!\n");
      return 0;
   }

   map_delete(_blacklist, person);
   save_me();

   add_succeeded_mess("$N remove$s someone from the blacklist.\n");
   return 1;
} /* do_remove_blacklist() */

/**
 * This method lists the current blacklist.
 * @param verbose list in verbose mode or not
 */
int do_list_blacklist(int verbose) {
   string ret;
   string* people;
   string person;

   people = keys(_blacklist);
   people = sort_array(people, 1);
   if (!sizeof(people)) {
      add_failed_mess("There is no one in the blacklist currently.\n");
      return 0;
   }
   ret = "";
   if (!verbose) {
      ret += "$I$5=Blacklist is: " + query_multiple_short(people);
   } else {
      ret += "Blacklist:\n";
      foreach (person in people) {
         ret += "$I$5=" + person + " by " + _blacklist[person]->blacklister +
                " for " + _blacklist[person]->reason + ".\n";
      }
   }
   this_player()->more_string(ret, "blacklist");
   return 1;
} /* do_list_blacklist() */

/** @ignore yes */
int variable_age_in_days(object player) {
   int bing;

   bing = -player->query_time_on();
   return bing / (60 * 60 * 24);
} /* variable_age_in_days() */

/** @ignore yes */
int variable_age_in_hours(object player) {
   int bing;

   bing = -player->query_time_on();
   return bing / (60 * 60);
} /* variable_age_in_hours() */

/** @ignore yes */
int variable_level(object player) {
   return player->query_level();
} /* variable_age_in_days() */

/** @ignore yes */
int variable_denied_in_days(object player) {
   if (_denied[player->query_name()]) {
      return (time() - _denied[player->query_name()]->time_denied) / (60 * 60 * 24);
   }
   return 10000;
} /* variable_age_in_days() */

/** @ignore yes */
int function_citizen_of(string area, object player) {
   return NOMIC_HANDLER->is_citizen_of(area, player->query_name());
} /* function_citizen_of() */

/** @ignore yes */
int function_magistrate_of(string area, object player) {
   return NOMIC_HANDLER->is_magistrate_of(area, player->query_name());
} /* function_magistrate_of() */

/* Support stuff for the honours inherit. */
/** @ignore yes */
int is_allowed_to_change(string person) {
   return NOMIC_HANDLER->is_magistrate_of(query_council_area(), person);
} /* is_allowed_to_change() */

/** @ignore yes */
void inform_of_honour_change(string type, string person,
                             string reason, string changed_by) {
   string subject;
   string mess;

   if (type == "add") {
      subject = PLAYER_HANDLER->query_cap_name(person) +
                " added to the honour roll.";
      mess = PLAYER_HANDLER->query_cap_name(person) + " is added to the "
             "honour roll for this council.\n\nThis means they gain the "
             "honour of using the title(s) " +
             query_multiple_short(map(query_controlled_titles(),
                                      (: capitalize :))) +  ".\n\n" +
             "They were added by " +
             PLAYER_HANDLER->query_cap_name(changed_by) +
             " for:\n" + reason + "\n";
   } else {
      subject = PLAYER_HANDLER->query_cap_name(person) +
                " removed from the honour roll.";
      mess = PLAYER_HANDLER->query_cap_name(person) + " is removed from the "
             "honour roll for this council.\n\nThis means they lose the "
             "privilege of using the title " +
             query_multiple_short(map(query_controlled_titles(),
                                      (: capitalize :))) +  ".\n\n" +
             "They were removed by " +
             PLAYER_HANDLER->query_cap_name(changed_by) +
             " for:\n" + reason + "\n";
   }
   NOMIC_HANDLER->post_citizen_message(query_council_area(),
                                       subject,
                                       mess);
} /* inform_of-honour_change() */

