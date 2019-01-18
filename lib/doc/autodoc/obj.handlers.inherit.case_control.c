.DT
case_control.c
Disk World autodoc help
case_control.c

.SH Description
.SP 5 5

This is the system for case control.  It keeps tracks of all the current
open cases in the area and any modifications that are done to them.
.EP
.SP 10 5


Written by Pinkfish

Started Tue Jul 11 18:09:01 PDT 2000

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/nomic_system.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_new_case%^RESET%^
.EI
.SI 5
int add_new_case(string area, string opener, string short, string * people)
.EI
.SP 7 5

This method opens a new case.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area the case is in
.EP
.SP 9 5
opener - the person who opened it
.EP
.SP 9 5
short - the short description of the case
.EP
.SP 9 5
people - the people involved
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure and teh case id on success

.EP

.SI 3
* %^BOLD%^add_note_to_case%^RESET%^
.EI
.SI 5
int add_note_to_case(string area, int id, string person, string text)
.EI
.SP 7 5

This method adds a note to the case.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to get the case in
.EP
.SP 9 5
id - the id of the case
.EP
.SP 9 5
person - the person adding the note
.EP
.SP 9 5
text - the actual note text
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^add_person_to_case%^RESET%^
.EI
.SI 5
int add_person_to_case(string area, int id, string adder, string person, string text)
.EI
.SP 7 5

This method adds a person to the case.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area the case is in
.EP
.SP 9 5
id - the id of the case
.EP
.SP 9 5
adder - the person that adds the person
.EP
.SP 9 5
person - the person to be added to the case
.EP
.SP 9 5
text - the reason for adding the person
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^appeal_case%^RESET%^
.EI
.SI 5
int appeal_case(string area, int id, string appealer, string reason)
.EI
.SP 7 5

This method sets the case as reopened for an appeal.  After an appeal
the case will be closed again.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area the case is in
.EP
.SP 9 5
id - the of the case to appeal
.EP
.SP 9 5
appealer - the person who does the appeal
.EP
.SP 9 5
reason - the reason for the appeal
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^close_case%^RESET%^
.EI
.SI 5
int close_case(string area, int id, string closer, string result, mixed data)
.EI
.SP 7 5

This method closes the case.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area the case is in
.EP
.SP 9 5
id - the id of the case to close
.EP
.SP 9 5
closer - the person that closes the case
.EP
.SP 9 5
result - the string saying what the result of the case is
.EP
.SP 9 5
desc - the description of the close reason
.EP
.SP 9 5
data - special internal data used specifically for appeals
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^create_area%^RESET%^
.EI
.SI 5
void create_area(string area)
.EI
.SP 7 5

This method creates an area.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the name of the area to create

.EP

.SI 3
* %^BOLD%^query_case_file_name%^RESET%^
.EI
.SI 5
string query_case_file_name(int id)
.EI
.SP 7 5

This method returns the file name for the case given the id.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
id - the id to return
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the file name

.EP

.SI 3
* %^BOLD%^query_closed_case%^RESET%^
.EI
.SI 5
class nomic_case query_closed_case(string area, int id)
.EI
.SP 7 5

This method returns the case associated with the specified closed
case.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to look for the case in
.EP
.SP 9 5
id - the id of the case
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the case class

.EP

.SI 3
* %^BOLD%^query_closed_cases%^RESET%^
.EI
.SI 5
int * query_closed_cases(string area)
.EI
.SP 7 5

This method returns the cases which are currently closed.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to get the cases in
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of closed cases

.EP

.SI 3
* %^BOLD%^query_event_name%^RESET%^
.EI
.SI 5
string query_event_name(int type)
.EI
.SP 7 5

This method returns the short name of the event type.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type to find the name of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the name of the type

.EP

.SI 3
* %^BOLD%^query_next_case_id%^RESET%^
.EI
.SI 5
int query_next_case_id()
.EI
.SP 7 5

This method finds the next case id.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the next case id

.EP

.SI 3
* %^BOLD%^query_next_note_id%^RESET%^
.EI
.SI 5
int query_next_note_id()
.EI
.SP 7 5

This method finds the next note id.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the next note id

.EP

.SI 3
* %^BOLD%^query_note%^RESET%^
.EI
.SI 5
class nomic_case_note query_note(int note_num)
.EI
.SP 7 5

This method return the exciting note associated with the case.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
note_num - the number of the note
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the note class, or 0 on failure

.EP

.SI 3
* %^BOLD%^query_note_file_name%^RESET%^
.EI
.SI 5
string query_note_file_name(int id)
.EI
.SP 7 5

This method returns the file name for the note given the id.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
id - the id to return
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the file name

.EP

.SI 3
* %^BOLD%^query_open_case%^RESET%^
.EI
.SI 5
class nomic_case query_open_case(string area, int id)
.EI
.SP 7 5

This method returns the case associated with the specified open
case.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to look for the case in
.EP
.SP 9 5
id - the id of the case
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the case class

.EP

.SI 3
* %^BOLD%^query_open_cases%^RESET%^
.EI
.SI 5
int * query_open_cases(string area)
.EI
.SP 7 5

This returns the cases which are currently open.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the are to get the cases in
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of opened cases

.EP

.SI 3
* %^BOLD%^query_recently_closed_case%^RESET%^
.EI
.SI 5
class nomic_case query_recently_closed_case(string area, int id)
.EI
.SP 7 5

This method returns the case associated with the specified recently
closed case.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to look for the case in
.EP
.SP 9 5
id - the id of the case
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the case class

.EP

.SI 3
* %^BOLD%^query_recently_closed_cases%^RESET%^
.EI
.SI 5
int * query_recently_closed_cases(string area)
.EI
.SP 7 5

This method returns the set of recently closed cases.  These are
cases that can be appealed.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to get the cases in
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of recently closed cases

.EP

.SI 3
* %^BOLD%^remove_person_from_case%^RESET%^
.EI
.SI 5
int remove_person_from_case(string area, int id, string remover, string person, string text)
.EI
.SP 7 5

This method removes a person from the case.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area the case is in
.EP
.SP 9 5
id - the id of the case
.EP
.SP 9 5
remover - the person that removes the person
.EP
.SP 9 5
person - the person to be removed to the case
.EP
.SP 9 5
text - the reason for removeing the person
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^update_recently_closed_cases%^RESET%^
.EI
.SI 5
void update_recently_closed_cases()
.EI
.SP 7 5

This method goes through and checks the recently closed cases to see
if any of them need to be moved to the closed set.

.EP


