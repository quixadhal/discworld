/**
 * This is the system for case control.  It keeps tracks of all the current
 * open cases in the area and any modifications that are done to them.
 * @author Pinkfish
 * @started Tue Jul 11 18:09:01 PDT 2000
 */
#define NOMIC_SYSTEM_NO_CLASSES
#define NOMIC_SYSTEM_CASE_CLASSES
#include <nomic_system.h>

private mapping _open_cases;
private mapping _recently_closed_cases;
private mapping _closed_cases;
private int _next_case_id;
private int _next_note_id;

void save_me();
void send_council_inform(string area, int only_magistrates,
                         string mess);

void create() {
   _open_cases = ([ ]);
   _closed_cases = ([ ]);
   _recently_closed_cases = ([ ]);
   _next_case_id = 1;
   _next_note_id = 1;
} /* create() */

/**
 * This method creates an area.
 * @param area the name of the area to create
 */
void create_area(string area) {
   if (_open_cases[area]) {
      return ;
   }
   _open_cases[area] = ({ });
   _closed_cases[area] = ({ });
   _recently_closed_cases[area] = ({ });
} /* create_area() */

/**
 * This returns the cases which are currently open.
 * @param area the are to get the cases in
 * @return the list of opened cases
 */
int* query_open_cases(string area) {
   return _open_cases[area];
} /* query_open_cases() */

/**
 * This method returns the cases which are currently closed.
 * @param area the area to get the cases in
 * @return the list of closed cases
 */
int* query_closed_cases(string area) {
   return _closed_cases[area];
} /* query_closed_cases() */

/**
 * This method returns the set of recently closed cases.  These are
 * cases that can be appealed.
 * @param area the area to get the cases in
 * @return the list of recently closed cases
 */
int* query_recently_closed_cases(string area) {
   return _recently_closed_cases[area];
} /* query_recently_closed_cases() */

/**
 * This method returns the file name for the case given the id.
 * @param id the id to return
 * @return the file name
 */
string query_case_file_name(int id) {
   return NOMIC_CASE_SAVE + sprintf("cases/%02d/%d_case", id % 20, id);
} /* query_case_file_name() */

/**
 * This method finds the next case id.
 * @return the next case id
 */
int query_next_case_id() {
   int next;

   do {
      next = _next_case_id++;
   } while (sizeof(stat(query_case_file_name(next))));
   return next;
} /* query_next_case_id() */

private void save_case(class nomic_case stuff) {
   string fname;
   string tmp;

   fname = query_case_file_name(stuff->id);
   tmp = save_variable(stuff);
   unguarded((: write_file($(fname), $(tmp), 1) :));
} /* save_case() */

/**
 * This method returns the file name for the note given the id.
 * @param id the id to return
 * @return the file name
 */
string query_note_file_name(int id) {
   return NOMIC_CASE_SAVE + sprintf("notes/%02d/%d_note", id % 20, id);
} /* query_note_file_name() */

/**
 * This method finds the next note id.
 * @return the next note id
 */
int query_next_note_id() {
   int next;

   do {
      next = _next_note_id++;
   } while (sizeof(stat(query_note_file_name(next))));
   return next;
} /* query_next_note_id() */

private void save_note(class nomic_case_note stuff) {
   string fname;
   string tmp;

   fname = query_note_file_name(stuff->id);
   tmp = save_variable(stuff);
   unguarded((: write_file($(fname), $(tmp) ) :));
} /* save_note() */

private int create_case(string opener, string short, string* people) {
   class nomic_case new_case;

   new_case = new(class nomic_case);
   new_case->id = query_next_case_id();
   new_case->opener = opener;
   new_case->short = short;
   new_case->people = people;
   new_case->notes = ({ });
   new_case->events = ({ });
   new_case->time_opened = time();
   save_case(new_case);
   return new_case->id;
} /* create_case() */

private int create_note(string person, string text) {
   class nomic_case_note new_note;

   new_note = new(class nomic_case_note);
   new_note->id = query_next_note_id();
   new_note->person = person;
   new_note->text = text;
   new_note->time_created = time();
   save_note(new_note);
   return new_note->id;
} /* create_note() */

private class nomic_case_event create_event(int type, string person,
                                            string mess, mixed data) {
   class nomic_case_event event;

   event = new(class nomic_case_event);
   event->type = type;
   event->mess = mess;
   event->person = person;
   event->event_time = time();
   event->data = data;
   return event;
} /* create_event() */

/**
 * This method returns the short name of the event type.
 * @param type the type to find the name of
 * @return the name of the type
 */
string query_event_name(int type) {
   switch (type) {
   case NOMIC_CASE_EVENT_CLOSE :
      return "closed case";
   case NOMIC_CASE_EVENT_APPEAL :
      return "appealed case";
   case NOMIC_CASE_EVENT_ADD_PERSON :
      return "add person";
   case NOMIC_CASE_EVENT_REMOVE_PERSON :
      return "remove person";
   default :
      return "unknown";
   }
} /* query_event_name() */

/**
 * This method returns the case associated with the specified open
 * case.
 * @param area the area to look for the case in
 * @param id the id of the case
 * @return the case class
 */
class nomic_case query_open_case(string area, int id) {
   string fname;
   string tmp;

   if (!_open_cases[area]) {
      return 0;
   }

   if (member_array(id, _open_cases[area]) == -1) {
      return 0;
   }

   fname = query_case_file_name(id);
   tmp = unguarded((: read_file($(fname)) :));
   if (tmp) {
      return restore_variable(tmp);
   }
   return 0;
} /* query_open_case() */

/**
 * This method returns the case associated with the specified recently
 * closed case.
 * @param area the area to look for the case in
 * @param id the id of the case
 * @return the case class
 */
class nomic_case query_recently_closed_case(string area, int id) {
   string fname;
   string tmp;

   if (!_recently_closed_cases[area]) {
      return 0;
   }

   if (member_array(id, _recently_closed_cases[area]) == -1) {
      return 0;
   }

   fname = query_case_file_name(id);
   tmp = unguarded((: read_file($(fname)) :));
   if (tmp) {
      return restore_variable(tmp);
   }
   return 0;
} /* query_closed_case() */

/**
 * This method returns the case associated with the specified closed
 * case.
 * @param area the area to look for the case in
 * @param id the id of the case
 * @return the case class
 */
class nomic_case query_closed_case(string area, int id) {
   string fname;
   string tmp;

   if (!_closed_cases[area]) {
      return 0;
   }

   if (member_array(id, _closed_cases[area]) == -1) {
      return 0;
   }

   fname = query_case_file_name(id);
   tmp = unguarded((: read_file($(fname)) :));
   if (tmp) {
      return restore_variable(tmp);
   }
   return 0;
} /* query_closed_case() */

/**
 * This method return the exciting note associated with the case.
 * @param note_num the number of the note
 * @return the note class, or 0 on failure
 */
class nomic_case_note query_note(int note_num) {
   string fname;
   string tmp;

   fname = query_note_file_name(note_num);
   tmp = unguarded((: read_file($(fname)) :));
   if (tmp) {
      return restore_variable(tmp);
   }
   return 0;
} /* query_note() */

/**
 * This method opens a new case.
 * @param area the area the case is in
 * @param opener the person who opened it
 * @param short the short description of the case
 * @param people the people involved
 * @return 0 on failure and teh case id on success
 */
int add_new_case(string area, string opener, string short, string* people) {
   int case_id;

   if (!_open_cases[area]) {
      return 0;
   }
   case_id = create_case(opener, short, people);
   _open_cases[area] += ({ case_id });

   save_me();

   send_council_inform(area, 1, opener + " opened a case with " +
                      query_multiple_short(people));
   return case_id;
} /* add_new_case() */

/**
 * This method closes the case.
 * @param area the area the case is in
 * @param id the id of the case to close
 * @param closer the person that closes the case
 * @param result the string saying what the result of the case is
 * @param desc the description of the close reason
 * @param data special internal data used specifically for appeals
 * @return 1 on success, 0 on failure
 */
int close_case(string area, int id, string closer, string result,
               mixed data) {
   class nomic_case the_case;
   class nomic_case_event event;

   the_case = query_open_case(area, id);
   if (!the_case || !stringp(result)) {
      return 0;
   }

   event = create_event(NOMIC_CASE_EVENT_CLOSE, closer, result, data);
   the_case->events += ({ event });
   save_case(the_case);
   _open_cases[area] -= ({ the_case->id });
   _recently_closed_cases[area] += ({ the_case->id });
   save_me();
   send_council_inform(area, 1, closer + " closed a case with " +
                      query_multiple_short(the_case->people));
   return 1;
} /* close_case() */

/**
 * This method sets the case as reopened for an appeal.  After an appeal
 * the case will be closed again.
 * @param area the area the case is in
 * @param id the of the case to appeal
 * @param appealer the person who does the appeal
 * @param reason the reason for the appeal
 * @return 1 on success, 0 on failure
 */
int appeal_case(string area, int id, string appealer, string reason) {
   class nomic_case the_case;
   class nomic_case_event event;

   the_case = query_recently_closed_case(area, id);
   if (!the_case || !stringp(reason)) {
      return 0;
   }

   event = create_event(NOMIC_CASE_EVENT_APPEAL, appealer, reason, 0);
   the_case->events += ({ event });
   save_case(the_case);
   _recently_closed_cases[area] -= ({ the_case->id });
   _open_cases[area] += ({ the_case->id });
   save_me();
   send_council_inform(area, 1, appealer + " appeals a case with " +
                      query_multiple_short(the_case->people));
   return 1;
} /* appeal_case() */

/**
 * This method adds a note to the case.
 * @param area the area to get the case in
 * @param id the id of the case
 * @param person the person adding the note
 * @param text the actual note text
 * @return 1 on success, 0 on failure
 */
int add_note_to_case(string area, int id, string person, string text) {
   class nomic_case the_case;
   int note_id;

   the_case = query_open_case(area, id);
   if (!the_case) {
      return 0;
   }

   note_id = create_note(person, text);
   the_case->notes += ({ note_id });
   save_case(the_case);
   save_me();
   return 1;
} /* add_note_to_case() */

/**
 * This method adds a person to the case.
 * @param area the area the case is in
 * @param id the id of the case
 * @param adder the person that adds the person
 * @param person the person to be added to the case
 * @param text the reason for adding the person
 * @return 1 on success, 0 on failure
 */
int add_person_to_case(string area, int id, string adder, 
                       string person, string text) {
   class nomic_case the_case;
   class nomic_case_event event;

   the_case = query_open_case(area, id);
   if (!the_case || !stringp(text) ||
       member_array(person, the_case->people) != -1) {
      return 0;
   }

   event = create_event(NOMIC_CASE_EVENT_ADD_PERSON, adder, text, 0);
   the_case->events += ({ event });
   the_case->people += ({ person });
   save_case(the_case);
   return 1;
} /* add_person_to_case() */

/**
 * This method removes a person from the case.
 * @param area the area the case is in
 * @param id the id of the case
 * @param remover the person that removes the person
 * @param person the person to be removed to the case
 * @param text the reason for removeing the person
 * @return 1 on success, 0 on failure
 */
int remove_person_from_case(string area, int id, string remover, 
                       string person, string text) {
   class nomic_case the_case;
   class nomic_case_event event;

   the_case = query_open_case(area, id);
   if (!the_case || !stringp(text) ||
       member_array(person, the_case->people) == -1) {
      return 0;
   }

   event = create_event(NOMIC_CASE_EVENT_REMOVE_PERSON, remover, text, 0);
   the_case->events += ({ event });
   the_case->people -= ({ person });
   save_case(the_case);
   return 1;
} /* remove_person_to_case() */

/**
 * This method goes through and checks the recently closed cases to see
 * if any of them need to be moved to the closed set.
 */
void update_recently_closed_cases() {
   string area;
   int* stuff;
   int id;
   class nomic_case the_case;
   class nomic_case_event event;
   int last_close;

   foreach (area, stuff in _recently_closed_cases) {
      foreach (id in stuff) {
         the_case = query_recently_closed_case(area, id);
         last_close = 0;
         foreach (event in the_case->events) {
            if (event->type == NOMIC_CASE_EVENT_CLOSE) {
               last_close = event->event_time;
            }
         }
         if (last_close < time() - NOMIC_CASE_APPEAL_TIME) {
            //
            // Ok, move the case into the really closed bunch.
            //
            _closed_cases[area] += ({ id });
            _recently_closed_cases[area] -= ({ id });
         }
      }
   }
} /* update_recently_closed_cases() */
