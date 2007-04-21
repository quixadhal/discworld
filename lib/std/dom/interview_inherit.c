/**
 *  Domain Interview Room
 *  @author Drakkos
 *  @started on 02/12/2000
 *  
 */
   
#include <player_handler.h>
#include <mail.h>
#define LORD capitalize (load_object ("/d/" + query_domain() + "/master")->query_lord())
#define SECURE "/d/" + query_domain() + "/secure/"
#define SAVE query_interview_dir() + "candidate_list"
#define LOG query_interview_dir() + "candidate_log.txt"
#define EVENTS "/global/events"

#define DEBUG 1

inherit "/std/room";

int add_creator (string);
int do_add (string);
int do_confirm (string);
int do_flag (string, string);
varargs int do_interview (object, string);
int do_query_text(string);
int do_list();
int do_read (string);
int flush();
int query_allowed_creators();
int query_interview();
int remove_candidate (string);
int remove_creator (string);
int remove_interviewed (string);
int set_interview_dir (string);
int set_part_two (string);
int query_part_two_required();

void set_acceptance(string);
void set_confirmation(string);
void set_rejection(string);
void set_interview (int);
void set_subject (string);
void set_interviewer(mixed);
void set_cc (string*);

string query_acceptance();
string query_applicants_text();
string query_cc();
string query_confirmation();
string query_domain();
string query_interview_dir();
string query_part_two();
string query_rejection();
string query_subject();

private nosave int _interview;
private nosave int _pt_two = 1;

private nosave string _acceptance_text;
private nosave string _confirmation_text;
private nosave string _domain;
private nosave string _filename;
private nosave string _interview_dir;
private nosave string _rejection_text;
private nosave string _subject;
private nosave string *_int;

private string *_allowed_creators = ({ });
private string *candidates = ({ });
private string *_cc = ({ });
private string *interviewed = ({ });
private string *submitted = ({ });
private string *accepted = ({ });
private string *rejected = ({ });

nosave string *movie = ({
  "First of all, I'd like to congratulate you for getting this far with the "
  "application process.  The second part of the application, in addition to "
  "testing descriptive and coding skills, is also designed to help weed out "
  "those who aren't really serious about becoming creators.  :-)",
  "I should make it clear at this point that getting this far doesn't mean "
  "that you're guaranteed a promotion.  We just want to get a fuller "
  "appreciation of applicants before we reach a decision on whether you are "
  "to be promoted or not.  :-)",
  "Being a creator, regardless of the rumours or jokes to the contrary, is "
  "not an easy option.  Unlike many MUDs, being a creator on " + mud_name() + " is "
  "not the same as early retirement.  You'll be expected to spend time "
  "coming to terms with the mudlib (the body of code that is used to "
  "create the MUD) and your domain (the areas you actually adventure "
  "in).  As a creator, you have considerable power over large segments of "
  "the MUD.  The important thing is learning how to use that power "
  "responsibly.", 
  "We have a number of strict rules on " + mud_name() + " .. rules you should be "
  "aware of before you make any decisions as to whether the role is for "
  "you.  Creators aren't allowed to affect the game in any measurable "
  "way.  Chatting is fine... killing NPCs at random or indiscriminately "
  "cloning cash for players is not.",
  "Being a creator is also not about policing the " + mud_name() + "   That falls "
  "under the jurisdiction of Admin and the Liaison domain.  As a coding "
  "creator your responsibilities lie in dealing with the code that "
  "makes up the MUD.  It is quite common that you will be approached by "
  "players with questions, particularly when they are no liaisons "
  "online.  In cases such as this, you are expected to refer them to "
  "a liaison or a Director.  If none are online, you are expected to ask "
  "them to wait until one is.",
  "In rare cases, it will be necessary for immediate action to be "
  "taken... in such cases, it may be acceptable for you to deal with a "
  "player problem yourself.  Always keep in mind however that you are "
  "here to code.  If it is liaising and player interaction that you think "
  "you would enjoy most, it would be best to consider an application "
  "instead to the Liaison domain.",
  "There is also a stern security policy on " + mud_name() + "   As a creator, you "
  "are expected not to violate this policy.  This means:  No passing "
  "creator information on to players, and no revealing what is posted on "
  "creator boards.   As a creator you have access to a lot of information "
  "players do not... this information should be treated as confidential "
  "unless you have Express Permission from your Leader that you may reveal "
  "it.  In short, we have to be able to trust you with sensitive "
  "information.",
  "The most important thing to remember is that you are expected to "
  "create.  If all you're really interested in is coding toys, then this "
  "role is not for you.  " + mud_name() + " is not strict in the sense of 'You have "
  "to code X rooms per day!', but you are expected to make a measurable "
  "and consistent contribution to your domain.  Your Leader and the "
  "deputy immediately supervising you will check up on this on a "
  "regular basis.",  
  "It may sound like an awful lot of work to be a creator... and it is.  "
  "But at the same time, it's an awful lot of fun, and provides you with "
  "the means of giving something back to a MUD that has hopefully provided "
  "you with many hours of entertainment.  :-)",
  "Now that the movie is over, are you still willing to continue your "
  "application to be a creator?  If not, then the exit 'drum' will "
  "return you to the " + mud_name() + "   If you are, hang around a while.  :-)"
  });

void save() {
  unguarded ((: save_object (SAVE) :));
}

void load() {
  unguarded ((: restore_object (SAVE) :));
}

void create() {

  do_setup++;
  ::create();
  do_setup--;

  add_property ("no scry", 1);
  add_property ("no teleport", 1);
  add_help_file ("/doc/creator/room/interview_help");
         
  if ( !do_setup ) {
    this_object()->setup();
    this_object()->reset();
  }
}


void init() {
  ::init();
  this_player()->add_command ("show", this_object(), "movie");
  this_player()->add_command ("stop", this_object(), "movie");
  this_player()->add_command ("interview", this_object(), 
      "<indirect:living>", (: do_interview ($1[0]) :));
  this_player()->add_command ("interview", this_object(), 
      "<indirect:living> with <string>", (: do_interview ($1[0], $4[1]) :));
      
  this_player()->add_command ("end", this_object(), "interview");
  this_player()->add_command ("add", this_object(), "<string> [as "
      "candidate]", (: do_add ($4[0]) :));
  this_player()->add_command ("query", this_object(), "applicants");
  this_player()->add_command ("query", this_object(), "permitted "
    "[creators]", (: query_allowed_creators () :));
  this_player()->add_command ("query", this_object(), 
    "{confirmation|acceptance|rejection} [text]",
    (: do_query_text ($4[0]) :));
  this_player()->add_command ("delete", this_object(), "<string> [from "
      "candidates]", (: remove_candidate ($4[0]) :));        
  this_player()->add_command ("clear", this_object(), "[all] candidates",
      (: flush() :));
  this_player()->add_command ("confirm", this_object(), "<string>",
      (: do_confirm ($4[0]) :));
  this_player()->add_command ("grant", this_object(), "[access to] "
    "<string>", (: add_creator ($4[0]) :));
  this_player()->add_command ("remove", this_object(), "<string> "
    "[as allowed]", (: remove_creator ($4[0]) :));
  this_player()->add_command ("flag", this_object(), "<string> as "
    "{accepted|rejected}", (: do_flag ($4[0], $4[1]) :));
  this_player()->add_command ("read", this_object(), 
    "interview for <string>",
    (: do_read ($4[0]) :));   
  this_player()->add_command ("list", this_object(), "interviews");     

}

/** 
 * Tests to see if a particular creator is permitted to make use of the 
 * interview room.  Domain lords and deputies are always permitted.
 * @param name the name of the object attempting to use the room.
 * @return 1 if the creator is allowed, 0 if they are not.
 */

void set_movie (string *m) {
  movie = m;
}

int query_permitted(string name) {
  object ob = load_object("/d/" + query_domain() + "/master");
  
  
  if (!ob) {
    printf ("Erk, this room is horribly borken!!  Please set the domain "
      "correctly.\n");
    return 0;
  }
  if (member_array (ob, previous_object (-1)) != -1) {
    return 2;
  }
    
  if (master()->high_programmer (name)) {
    return 1;
  }
  
  
#ifdef DEBUG
  if (name == "drakkos") {
    return 1;
  }  
#endif  
  
  if ((ob->query_lord() != name) &&  
    !(ob->query_deputy (name)) && (member_array (name, _allowed_creators) 
    == -1)) {
        return 0;
    }    
    
  return 1;
}

/** 
 * Logs text to the interview text file, which will be in the domain's 
 * secure directory, in a sub-directory called 'interviews'.  Interviews are
 * stored with the name of the subject, plus a .txt extension:  drakkos.txt, 
 * for example.
 * @param text the text to be written to the file.
 */

private void write_to_file (string text) {
  unguarded ((: write_file (query_interview_dir()
        +query_subject() +".txt", 
        sprintf ("%-=*s", 79, $(text) + "\n")) :));
}
      
/** 
 * If there is currently a 'subject' of the room... in other words, an 
 * interview is underway, all says in the room get logged to the interview
 * file.
 * @ignore yes
 */
  
void event_person_say(object ob, string start, string mess, string lang) {
  if (query_subject()) {
      write_to_file (sprintf ("%s says: %s\n", ob->short(),
          mess));
  }        
}

void event_emote(object ob, string mess) {
  if (query_subject()) {
      write_to_file (replace (EVENTS->convert_message (mess), ({"$C$", ""})));
  }        
}

/** 
 * This function will show the embedded 'movie' text at regular intervals.
 * @return returns 0 on failure, 1 on success.
 */


int do_show() {
  
  if (!query_permitted (this_player()->query_name())) {
      this_player()->add_failed_mess (this_object(), "You cannot show a "
        "movie!\n", ({ }));
      return 0;
  }    
  
  else {            
    call_out ("play_movie", 5, 0);
  }
  
  this_player()->add_succeeded_mess (this_object(), "$N dim$s the light "
          "before pulling out a grubby looking movie projector and pointing "
          "it at an empty wall.  After flicking a switch, a thin ray of "
          "light shoots at the wall.\n", ({ }));
  return 1;
}

int do_stop() {
  
  int i;
  if (!query_permitted (this_player()->query_name())) {
      this_player()->add_failed_mess (this_object(), "You cannot stop a "
        "movie!\n", ({ }));
      return 0;
  }    
  
  do {
    i = remove_call_out ("play_movie");
  } while (i != -1);
  
  
  this_player()->add_succeeded_mess (this_object(), "$N stop$s the movie.\n", 
    ({ }));
  return 1;
}

/** 
 * This function does a person_say event on the environmen.  The 
 * content of the event is the relevant movie text to be said.
 * @param i the index of the movie to be be played.
 */

void play_movie (int i) {
  
  if (movie[i][0] == '#') {
    tell_room (this_object(), "The movie shows " + 
      movie[i][1..sizeof(movie[i])] + "\n");
  }
  else {
    event (this_object(), "person_say", "The voice from the movie says: ", 
      movie[i], "general");  
  }
  
  if (i < sizeof (movie)) {    
    call_out ("play_movie", 15, ++i);    
  }
  
}

/** 
 * This function is the main entry point for the interview code.  It 
 * will set the subject of the room, and write an entry in the interview
 * text containing who the interview is with, when it was started, and
 * who conducted the interview.
 * @param ob the object to interview
 * @return 0 on failure, 1 on success
 */


varargs int do_interview (object ob, string names) {
  string *inter = ({ });
  if (!query_permitted (this_player()->query_name())) {
      this_player()->add_failed_mess (this_object(), "You cannot start an "
        "interview!\n", ({ }));
      return 0;
  }    
  
  if (query_subject()) {
      this_player()->add_failed_mess (this_object(), "You'll have to end "
          "the interview with " + query_subject() + " first.\n", ({ }));
      return 0;
  }
  else {
      if (sizeof (names)) {
        names = replace (names, ({" ", ""}));
        inter = explode (names, ",");
        tell_creator ("drakkos", "%O.\n", inter);
        inter = filter (inter, (: find_player ($1) 
          && find_player ($1)->query_playtester() :));
        tell_creator ("drakkos", "%O.\n", inter);

      }
      inter += ({ this_player()->query_name() });
      set_subject (ob->query_name());   
      set_interviewer (inter);     
      write_to_file (sprintf ("Interview with %s, started %s.  Interview " 
        "conducted by %s.\n\n", ob->short(), ctime (time()), 
        query_multiple_short (inter)));
        
  }
  this_player()->add_succeeded_mess (this_object(), "$N press$es a button "
      "on $p tape recorder, indicating the interview is now being "
      "recorded.\n", ({ }));
  return 1;
}

/** 
 * The function to end the interview.  Will write an entry in the interview
 * text containing who the interview was with and when the interview was
 * ended.  It will then add the subject to the list of interviewed names and
 * save the object.  Finally, it will reset the subject of the room. 
 * @return 0 on failure, 1 on success.
 */

int do_end() {
  if (!query_permitted (this_player()->query_name())) {
      this_player()->add_failed_mess (this_object(), "You cannot end an "
        "interview!\n", ({ }));
      return 0;
  }    
  
  if (!query_subject()) {
      this_player()->add_failed_mess (this_object(), "You are not currently "
          "interviewing anyone!\n", ({ }));
      return 0;
  }
  else {
      if (member_array (query_subject(), candidates) != -1) {
          candidates -= ({query_subject()});
      }
      if (member_array (query_subject(), submitted) != -1) {
          submitted -= ({query_subject()});
      }
      
      interviewed += ({query_subject()});
      interviewed = uniq_array (interviewed);
      save();
      
      write_to_file (sprintf ("Interview with %s, ended %s.\n", query_subject(), 
        ctime (time())));
        
      set_subject (0);
      set_interviewer (0);
      this_player()->add_succeeded_mess (this_object(), "$N switch$es off "
          "$p tape recorder, indicating the interview is at an end.\n", ({ }));
      return 1;
  }
    
}

/** 
 * If there is currently a 'subject' of the room... in other words, an 
 * interview is underway, all souls in the room get logged to the intervie
 * file.
 * @ignore yes
 */
 
void event_soul(object ob, string str, mixed avoid) {
  if (environment (this_player()) != this_object()) {
    return;
  }
  if (query_subject()) {
    write_to_file (EVENTS->convert_message (str));
  }      
}

/** 
 * Give the room some privacy.
 * @ignore yes
 */
int *query_co_ord() { 
  return 0; 
} 

/** 
 * Query the current state of applications.
 * @return 0 on failure, 1 on success
 */
int do_query() {
  if (!query_permitted (this_player()->query_name())) {
      this_player()->add_failed_mess (this_object(), "You cannot query "
        "creator candidates!\n",
          ({ }));
      return 0;
  }    
  
  if (!sizeof (query_applicants_text())) {
    this_player()->add_failed_mess (this_object(), "No information.\n");
  }
  else {
    tell_object (this_player(), this_object()->query_applicants_text());
  }  
  return 1;
}

/** 
 * Start off a creator application by sending them out the second part of
 * the application procedure.  
 * @param candidate the name to send the second part of the application to.
 * @return 0 on failure, 1 on success
 */
     
int do_add (string candidate) {
  int i = query_permitted (this_player()->query_name());
                   
  if (!i) {
      this_player()->add_failed_mess (this_object(), "You cannot add a "
        "candidate!\n", ({ }));
      return 0;
  }
  
  if (!PLAYER_HANDLER->test_user(candidate)) {
      this_player()->add_failed_mess (this_object(), "This player "
          "exists nowhere on " + mud_name() + " \n", ({ }));
      return 0;
      
  }
  
  if (query_part_two_required()) {
    if (member_array (candidate, candidates) == -1) {
        unguarded ((: write_file (LOG, sprintf ("%s added %s as a "
            "candidate on %s.\n", ($(i) == 2? "The " + query_domain() 
            + " master object" : this_player()->query_name()),
            $(candidate), ctime(time()))) :));
        candidates += ({candidate});
    }
    else {
      this_player()->add_failed_mess (this_object(), "This player has "
          "already been mailed about creatorship.\n", ({ }));
      return 0;
    }    
  }
  else if (member_array (candidate, submitted) == -1) {
        unguarded ((: write_file (LOG, sprintf ("%s added %s as a "
            "candidate on %s.\n", ($(i) == 2? "The " + query_domain() 
            + " master object" : this_player()->query_name()),
            $(candidate), ctime(time()))) :));
        submitted += ({candidate});
  } 
  else {
      this_player()->add_failed_mess (this_object(), "This player has "
          "already been mailed about creatorship.\n", ({ }));
      return 0;
  }     
    AUTO_MAILER->auto_mail( candidate, capitalize 
      (this_player()->query_name()), "Your application to " 
        + capitalize (query_domain()) + ".", query_cc(), 
        query_part_two(), 0, 0);
    save();
    this_player()->add_succeeded_mess (this_object(), "$N add$s a "
      "name to the candidate list.\n", ({ }));
    return 1;
}

string* query_submitted() {
  return submitted;
}


/** 
 * Return which candidates have been mailed, which have sent in their 
 * submission, and which are left to be interviewed or replied to.
 * @return the text to be displayed
 */
 
string query_applicants_text() {
  string ret = "", *temp = allocate (sizeof (submitted));
  int i;
  
  if (sizeof (submitted)) {
      for (i = 0; i < sizeof (submitted); i++) {
          if (find_player (submitted[i])) {
              temp [i] = "%^BOLD%^"+submitted[i] + "%^RESET%^";
          }
          else {
              temp [i] = submitted [i];
          }
      }
    }   
      
  if (query_part_two_required()) {
    if (sizeof (candidates)) {        
      ret += sprintf ("The candidates for %s still to submit answers to "
          "part two of the application process are: %s.\n", 
            capitalize (query_domain()), query_multiple_short (candidates));
    }    
  }
  else {              
    if (query_interview()) {
      if (sizeof (temp)) {
        ret += sprintf ("The following candidates for %s " + 
        (query_part_two_required()? "have submitted "
          "answers to part two of the test and " :  "") 
          + "are awaiting an interview.  "
          "Names in bold are currently online: %s.\n",
          capitalize (query_domain()), query_multiple_short (temp));
      }
    }
    else {
      ret += sprintf ("The following candidates for %s have submitted "
        "answers to part two of the test and are awaiting a response.  "
        "Names in bold are currently online: %s.\n",
        capitalize (query_domain()), query_multiple_short (temp));
    }        
  }
                              
  if (query_interview()) {
    if (sizeof (interviewed)) {        
        ret += sprintf ("The following candidates have been interviewed "
            "for %s, and are awaiting a response: %s.\n", 
            capitalize (query_domain()), query_multiple_short (interviewed));
    }
  }
  
  if (sizeof (accepted)) {
    ret += sprintf ("The following candidates have been accepted "
            "for %s: %s.\n", 
            capitalize (query_domain()), query_multiple_short (accepted));
  }

  if (sizeof (rejected)) {
    ret += sprintf ("The following candidates have been rejected from "
            "%s: %s.\n", 
            capitalize (query_domain()), query_multiple_short (rejected));
  }    
  
          
  if (!sizeof (ret)) {
      ret += "There are no outstanding candidates, and no-one has been "
          "interviewed recently.\n";
  }
  
  return ret;
}

/** 
 * Clear the list of interviewed players.
 * @return 0 on failure, 1 on success
 */
 
int clear_interviewed() {

  if (!query_permitted (this_player()->query_name())) {
      this_player()->add_failed_mess (this_object(), "You cannot do this!\n",
          ({ }));
      return 0;
  }    
  interviewed = ({ });
  return 1;
}
/** 
 * Clear the list of player candidates.
 * @return 0 on failure, 1 on success
 */

int clear_candidates() {

  if (!query_permitted (this_player()->query_name())) {
      this_player()->add_failed_mess (this_object(), "You cannot do this!\n",
          ({ }));
      return 0;
  }    
  candidates = ({ });
  return 1;
}

/** 
 * Flush the candidate, interviewed and submitted lists.
 * @return 0 on failure, 1 on success
 */

int flush() {

  if (!query_permitted (this_player()->query_name())) {
      this_player()->add_failed_mess (this_object(), "You cannot do this!\n",
          ({ }));
      return 0;
  }    
  candidates = ({ });
  interviewed = ({ });
  submitted = ({ });
  accepted = ({ });
  rejected = ({ });
  return 1;
}

/** 
 * Delete a player from the list.  Will search through all three lists for
 * an occurance of the name before deleting it where found.
 * @param candidate the name of the player to remove.
 * @return 0 on failure, 1 on success
 */


int remove_candidate(string candidate) {

  int i;
  
  if (!query_permitted (this_player()->query_name())) {
      this_player()->add_failed_mess (this_object(), "You cannot do this!\n",
          ({ }));
      return 0;
  }    
  if (member_array (candidate, candidates) != -1) {
      candidates -= ({candidate});
      i = 1;
  }
  if (member_array (candidate, submitted) != -1) {
      submitted -= ({candidate});
      i = 1;
  }
  if (member_array (candidate, interviewed) != -1) {
      interviewed -= ({candidate});
      i = 1;
  }
  if (member_array (candidate, accepted) != -1) {
      accepted -= ({candidate});
      i = 1;
  }
  if (member_array (candidate, rejected) != -1) {
      rejected -= ({candidate});
      i = 1;
  }
        
  if (i == 1) {
    this_player()->add_succeeded_mess (this_object(), "$N delete$s a "
      "name from the candidate list.\n", ({ }));
    save();
    return 1;
  }
  
  this_player()->add_failed_mess (this_object(), "No candidate by that "
    "name is on the list.\n", ({ }));          
  return 0;
}

/** 
 * Add a candidate to the list.  Will not send part two of the application
 * to them.
 * @param name name of the player to add.
 * @return 0 on failure, 1 on success
 */


int add_candidate(string name) {

  if (!query_permitted (this_player()->query_name())) {
      this_player()->add_failed_mess (this_object(), "You cannot do this!\n",
          ({ }));
      return 0;
  }    
  
  if (!PLAYER_HANDLER->test_user(name)) {
      this_player()->add_failed_mess (this_object(), "This player "
          "exists nowhere on " + mud_name() + " \n", ({ }));
      return 0;
      
  }

  if (member_array (name, candidates) == -1) {
      candidates += ({name});
      return 1;
  }
  return 0;
}

int add_interviewed(string name) {

  if (!query_permitted (this_player()->query_name())) {
      this_player()->add_failed_mess (this_object(), "You cannot do this!\n",
          ({ }));
      return 0;
  }    
  
  if (!PLAYER_HANDLER->test_user(name)) {
      this_player()->add_failed_mess (this_object(), "This player "
          "exists nowhere on " + mud_name() + " \n", ({ }));
      return 0;
      
  }

  if (member_array (name, interviewed) == -1) {
      interviewed += ({name});
      return 1;
  }
  return 0;
}

int add_confirmed(string name) {

  if (!query_permitted (this_player()->query_name())) {
      this_player()->add_failed_mess (this_object(), "You cannot do this!\n",
          ({ }));
      return 0;
  }    
  
  if (!PLAYER_HANDLER->test_user(name)) {
      this_player()->add_failed_mess (this_object(), "This player "
          "exists nowhere on " + mud_name() + " \n", ({ }));
      return 0;
      
  }

  if (member_array (name, submitted) == -1) {
      submitted += ({name});
      return 1;
  }
  return 0;
}

/** 
 * Remove a player from the interviewed list.
 * @param candidate name of the player to add.
 * @return 0 on failure, 1 on success
 */


int remove_interviewed(string candidate) {

  if (!query_permitted (this_player()->query_name())) {
      this_player()->add_failed_mess (this_object(), "You cannot do this!\n",
          ({ }));
      return 0;
  }     
  if (member_array (candidate, interviewed) != -1) {
      interviewed -= ({candidate});
      return 1;
  }
  return 0;
}

/** 
 * Confirm receipt of a candidate's submission.
 * @param candidate name of the player to add.
 * @return 0 on failure, 1 on success
 */

int do_confirm (string candidate) {
  
  if (!query_permitted (this_player()->query_name())) {
      this_player()->add_failed_mess (this_object(), "You cannot do this!\n",
          ({ }));
      return 0;
  }    

  if (!PLAYER_HANDLER->test_user(candidate)) {
      this_player()->add_failed_mess (this_object(), "This player "
          "exists nowhere on " + mud_name() + " \n", ({ }));
      return 0;
      
  }    
  
  if (member_array (candidate, submitted) != -1) {
    this_player()->add_failed_mess (this_object(), "You have already "
      "confirmed receipt of this candidate's submission.\n", ({ }));
    return 0;
  }
  if (member_array (candidate, interviewed) != -1) {
    this_player()->add_failed_mess (this_object(), "This candidate "
      "has already been interviewed.\n", ({ }));
    return 0;
  }
  
  if (member_array (candidate, candidates) != -1) {
    candidates -= ({candidate});
  }

    submitted += ({candidate});
             
    AUTO_MAILER->auto_mail( candidate, capitalize 
        (this_player()->query_name()), "Thank you for your submission.",  
        query_cc(), query_confirmation(), 0, 0);
    save();
  
  this_player()->add_succeeded_mess (this_object(), "$N confirm$s "
    "receipt of a candidate's application.\n", ({ }));
  return 1;
}

/** 
 * Set the domain the room exists in.
 * @param dom name of the domain.
 */
  
void set_domain(string dom) {
  _domain = dom;
}

/** 
 * Return the domain the room exists in.
 * @return The name of the domain, or 'unset' if none is set.
 */

string query_domain () {
  if (!_domain) {
    return "unset";
  }
  else {
    return _domain;
  }
  
}

/** 
 * Set the text of the confirmation mail.
 * @param dom name of the domain.
 */


void set_confirmation(string text) {  
  _confirmation_text = text;
}

/** 
 * Returns the confirmation mail text.
 * @return The confirmation text
 */

string query_confirmation() {
  
  if (!sizeof (_confirmation_text)) {
  
    if (query_interview()) {
      return sprintf ("Bing!\n\n"
        "Thank you for sending in the second part of the application.  "
        "The next part of the application process will be an interview "
        "to discuss your application to the %s domain.  "
        "This interview is likely to take around an hour of your time, "
        "so if you could mail me with a list of your "
        "common online times, hopefully we'll be able to make an "
        "appointment that is mutually convenient.\n\nRegards,\n%s.",
        query_domain(), LORD); 
    }
    else {
      return sprintf ("Bing!\n\n"
        "Thank you for sending in the second part of the application.  "
        "Your application to the %s domain is now being considered, and "
        "you should expect to hear back within a week or two with an "
        "answer.\n\nRegards,\n%s.",
        query_domain(), LORD);
    }
                
  }
  else {
    return _confirmation_text;
  }
}

/** 
 * Return the domain's lord.
 * @return the capitalised name of the lord.
 */

string query_lord() {
  return LORD;
}

void set_part_two_required (int i) {
  _pt_two = i;
}

int query_part_two_required () {
  return _pt_two;
}

/** 
 * Set whether an interview is required of creator candidates.
 * @param i the interview state.
 */

  
void set_interview(int i) {
  _interview = i;
}

/** 
 * Return whether creator candidates require an interview for this domain.
 * @return 0 if an interview is not required.  Anything else if it is.
 */

int query_interview() {
  return _interview;
}

/** 
 * Query the subject of an interview if one is ongoing.
 * @return 0 if there is no subject.  The name of the subject if there is.
 */

string query_subject(){
  if (_subject) {
    return _subject;
  }

  return 0;
}

/** 
 * Set the subject of an interview.
 * @pram name name of the subject.
 */

private void set_subject (string name) {
    _subject  = name;
}      

private void set_interviewer (mixed name) {
  if (arrayp (name)) {
    _int  = name;
  }
  else {
    _int = ({ name });
  }
}      

string *query_interviewer() {
  return _int;
}

int query_is_interviewer (string name) {
  if (!sizeof (_int)) {
    return 0;
  }
  
  if (member_array (name, _int) != -1) {
    return 1;
  }
  return 0;
}  

/** 
 * Query the creators permitted to use this room.
 * @return an empty array if no creators are permitted.  The array of creators 
 * otherwise.
 */

string* query_permitted_creators() {
  return copy (_allowed_creators);
}

/** 
 * Permit a creator to use this room..
 * @param name name of the creator to allow
 */

private void add_permitted_creator(string name) {
  if (member_array (name, _allowed_creators) == -1) {
    _allowed_creators += ({ name });
  }
  
}

/** 
 * Remove permission for a creator to use this room.
 * @param name name of the creator to remove
 */

private void remove_permitted_creator(string name) {
  if (member_array (name, _allowed_creators) != -1) {
    _allowed_creators -= ({ name });
  }
  
}

/** 
 * Entry point to the add creator command.
 * @param name name of the creator to allow
 * @return 0 on failure, 1 on success.
 */

int add_creator (string name) {
  object ob = find_object ("/d/" + query_domain() + "/master");
  
  if (this_player()->query_name() != ob->query_lord()) {
    this_player()->add_failed_mess (this_object(), "Only the lord "
      "may permit creators to use this room.\n", ({ }));
    return 0;
  }
  if (!PLAYER_HANDLER->test_creator (name)) {
      this_player()->add_failed_mess (this_object(), "Only creators "
        "may be permitted to use the interview room.\n", ({ }));
      return 0;
  }

  add_permitted_creator (name);
  save();
  
  this_player()->add_succeeded_mess (this_object(), "$N add$s a creator "
    "as allowed.\n", ({ }));
  return 1;
}

/** 
 * The entry point to the remove creator command.
 * @param name name of the creator to remove
 * @return 0 on failure, 1 on success.
 */

int remove_creator (string name) {

  object ob = find_object ("/d/" + query_domain() + "/master");
      
  if (this_player()->query_name() != ob->query_lord()) {
    this_player()->add_failed_mess (this_object(), "Only the lord "
      "may permit creators to use this room.\n", ({ }));
    return 0;
  }
  
  remove_permitted_creator (name);
  save();
  
  this_player()->add_succeeded_mess (this_object(), "$N remove$s a creator "
    "as allowed.\n", ({ }));
  return 1;
}  

/** 
 * Entry point to the query allowed creators command.
 * @return 0 on failure, 1 on success.
 */

int query_allowed_creators() {
  
  if (!sizeof (query_permitted_creators())) {
    this_player()->add_failed_mess (this_object(), "Only the domain lord and "
      "the domain deputies are currently permitted to use this room.\n", 
      ({ }));
    return 0;
  }
        
  tell_object (this_player(), sprintf ("In addition to the domain Lord and "
    "the domain deputies, the following creators are permitted "
    "to make use of this room: %s.\n", query_multiple_short 
    (query_permitted_creators())));
  return 1;
}

/** 
 * Return the text for the second part of the application.
 * @return application text.
 */

string query_part_two(){
  string temp;
  
  if (_filename) {
    temp = unguarded ((: read_file (_filename) :));
    
    if (sizeof (temp)) {
      return temp;
    }
  }
  
  return sprintf ("Bing!\n\nThank you for your application to the %s domain. "
    "\n\nYour application has been received and is awaiting processing.\n\n"
    "Regards,\n%s.", query_domain(), LORD);
}

/** 
 * Set the filename for the second part of the application.
 * @param file filename of the second part.
 * @return 0 on failure, 1 on success.
 */
 
int set_part_two(string file) {
  
  if (!sizeof (file)) {
    printf ("No file given for part two of application.  Aborting.\n");
    return 0;
  }
  
  if (file_size(file) == -1) {
    printf ("File given for set_part_two() does not exist.  No file set.\n");
    return 0;
  }
  
  _filename = file;
  return 1;
}

/** 
 * Return the directory where interviews are to be stored.
 * @return interview directory.
 */

string query_interview_dir(){
  if (sizeof (_interview_dir)) {
    return _interview_dir;
  }
  else {
    return SECURE;
  }
  
}

/** 
 * Set the directory where interviews are to be stored.
 * @param dir interview directory.
 * @return 0 on failure, 1 on success.
 */
 
int set_interview_dir(string dir) {
  
  if (!sizeof (dir)) {
    printf ("No directory given for interviews.  Aborting.\n");
    return 0;
  }
  
  if (file_size(dir) != -2) {
    printf ("Directory given for set_interview_dir does not exist.  "
      "Aborting.\n");
    return 0;
  }
  
  _interview_dir = dir;
  return 1;
}

/** 
 * Entry point for the query command.
 * @return 0 on failure, 1 on success.
 */
 
int do_query_text(string flag) {
  string temp;
  
  switch (flag) {
    case "confirmation":
      temp = query_confirmation();
    break;
    case "acceptance":
      temp = query_acceptance();
    break;
    case "rejection":
      temp = query_rejection();
    break;
    default:
      temp = "I'm sorry, there has been an error.\n";
    break;
  }
  
      
  tell_object (this_player(), temp);
  return 1;
}
  
/** 
 * Entry point for the flag command.  Sets an application as accepted or 
 * rejected, and sends out the appropriate form mail.
 * @param name name of candidate.
 * @param flag "accepted" or "rejected"
 * @return 0 on failure, 1 on success.
 */
int do_flag(string name, string flag) {
  
  if (member_array (name, candidates) != -1) {
        candidates -= ({name});
  }
  if (member_array (name, submitted) != -1) {
      submitted -= ({name});
  }
  if (member_array (name, interviewed) != -1) {
      interviewed -= ({name});
  }

  
  if (flag == "accepted") {
    AUTO_MAILER->auto_mail( name, capitalize 
      (this_player()->query_name()), "Congratulations!",  
          query_cc(), query_acceptance(), 0, 0);
    if (member_array (name, accepted) == -1) {
      accepted += ({name});
    }
  }
  else {
        
    AUTO_MAILER->auto_mail( name, capitalize 
      (this_player()->query_name()), sprintf ("Your application to the %s "
        "domain.", query_domain()), query_cc(), query_rejection(), 0, 0);    
    if (member_array (name, rejected) == -1) {
      rejected += ({name});
    }
    
  }
  
  this_player()->add_succeeded_mess (this_object(), sprintf ("$N flag$s a "
    "player as %s.\n", flag), ({ }));
  save();
  unguarded ((: write_file (LOG, sprintf ("%s flagged %s as %s "
          "on %s.\n", this_player()->query_name(),
          $(name), $(flag), ctime(time()))) :));
  return 1;
    
}

/** 
 * Set the rejection mail text.
 * @param text the rejection mail text
 */
 
void set_rejection(string text) {  
  _rejection_text = text;
}

/** 
 * Set the acceptance mail text.
 * @param text the acceptance mail text
 */
 
void set_acceptance(string text) {  
  _acceptance_text = text;
}

/** 
 * Query the rejection mail text.
 * @return the rejection text
 */

string query_rejection() {  
  if (!sizeof (_rejection_text)) {    
    return sprintf ("Hello.\n\n"
      "I'm sorry to say that your application to the %s domain has been "
      "unsuccessful.  My apologies that I cannot hire you at this time.  "
      "I will of course keep your details on file should you wish to "
      "apply again in the future.\n\nRegards,\n%s.",
          query_domain(), LORD);
  }
  else {
    return _rejection_text;
  }
}    

/** 
 * Query the acceptance mail text.
 * @return the rejection text
 */

string query_acceptance() {  
  if (!sizeof (_acceptance_text)) {    
    return sprintf ("Bing!\n\n"
      "It is my great pleasure to inform you that your application to the %s "
        "domain has been successful.  Congratulations!\n\nBefore being promoted, "
        "you will need to ensure you repay any debts you have.  If you wish "
        "to bequeath your inventory on a fellow player, this must also be "
        "done before you are employed - you won't get a chance once you're "
        "a creator.\n\nPlease send me a mail letting me know when you will "
        "be available for promotion and your orientation lecture.\n\n"
        "Congratulations,\n%s.",query_domain(), LORD);
  }
  else {
    return _acceptance_text;
  }
}    

/**
 * This function will take the name of a player and if the interview document
 * for that player exists, print it to the screen of this_player().
 * @param name the name of the player.
 * @return 0 on failure, 1 on success.
 */
 
int do_read (string name) {
  string path;
  
  if (!query_permitted (this_player()->query_name())) {
    this_player()->add_failed_mess (this_object(), "You cannot do this!\n",
        ({ }));
    return 0;
  } 
  
  path = query_interview_dir() + name + ".txt";
  
  if (!file_size (path)) {
    this_player()->add_failed_mess (this_object(), "This document does "
      "not exist.\n", ({ }));
    return 0;
  }
  
  this_player()->more_string (unguarded ((: read_file ($(path)) :)));
  return 1;    
}

/** 
 * Query the names cc'd by default.
 * @return an imploded string of cc'd names
 */
 
string query_cc() {
  if (sizeof (_cc)) {
    return implode (_cc, ",");
  }
  return 0;
}

/** 
 * Set the names cc'd by default.
 * @param i an array of cc'd names.
 */
void set_cc (string *i) {
  _cc = i;
}

int do_list() {
  string *list, text;
  
  if (!query_permitted (this_player()->query_name())) {
    this_player()->add_failed_mess (this_object(), "You are not permitted "
      "to do this.\n", ({ }));
    return 0;
  }  
  else {
    list = get_dir (query_interview_dir());
    list -= ({"candidate_list.o", "candidate_log.txt"});
  }
    
  list = map (list, (: capitalize ($1) :));
  text = replace (query_multiple_short (list), ({".txt", ""}));
  
  tell_object (this_player(), sprintf ("The following interviews have been "
    "conducted for the %s domain: %s.\n", query_domain(), 
      text));
  
  this_player()->add_succeeded_mess (this_object(), "$N list$s the completed "
    "interviews.\n", ({ }));
  return 1;
}

    
string* query_all_applicants() {
  return uniq_array (candidates + interviewed + submitted + accepted + rejected);
}

string* query_interviewed() {
  return interviewed;
}
