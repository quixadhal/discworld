.DT
learning.c
Disk World autodoc help
learning.c

.SH Description
.SP 5 5

Register a lesson with the learning system.

.EP
.SP 10 5

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/tasks.h, /include/shops/bank.h and /include/money.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^lesson%^RESET%^
class lesson {
mapping minimum_skills;
mapping maximum_skills;
string * materials;
int cost;
string currency;
string * lesson_ending;
object teacher;
class step * plan;
}

.EI

.SI 3
* %^BOLD%^step%^RESET%^
class step {
string * blurbs;
string skill;
int difficulty;
string * responses;
string * params;
string award_mess;
string succeed_mess;
string fail_mess;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_step%^RESET%^
.EI
.SI 5
int add_step(string lesson_name, string * blurbs, string skill, int diff, string * responses, string * params, string award_mess, string succeed_mess, string fail_mess)
.EI
.SP 7 5

Add a step to a lesson plan.  This is the interaction between student and 
teacher. One step should be defined for each thing you want the student
to learn.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
lesson_name - the name of the lesson this lesson plan is 
associated with. 
.EP
.SP 9 5
blurbs - An array of strings in order that they should be given 
during the lesson.  This is what the teacher says to the student. ', :, #, etc. 
can be used for say, emote and execute a command respectively.
.EP
.SP 9 5
skill - The skill being learned.
.EP
.SP 9 5
difficulty - The bonus level of the skill being learned.
.EP
.SP 9 5
responses - An array of strings.  These are the expected responses 
to the teacher's blurbs.
.EP
.SP 9 5
params - an array of strings given as arguments to the responses in 
order.
.EP
.SP 9 5
award_mess - The string a player sees if they get a TM award during this step. It
should not end in a newline \n.
.EP
.SP 9 5
succeed_mess - The string a player gets if they are successful in doing what the 
step asks.  It should not end in a newline \n.
.EP
.SP 9 5
fail_mess - A string telling the player that they've failed.  It should not 
end in a newline \n.

.EP

.SI 3
* %^BOLD%^has_required_cash%^RESET%^
.EI
.SI 5
int has_required_cash(string name, object player)
.EI
.SP 7 5
has_required_cash( string name, object player )
Give some, get some. Money makes a teacher more likely to teach you.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - name of the lesson they're trying to learn
.EP
.SP 9 5
player - player object of the player trying to learn

.EP

.SI 3
* %^BOLD%^has_required_materials%^RESET%^
.EI
.SI 5
int has_required_materials(string name, object player)
.EI
.SP 7 5
has_required_materials( string name, object player )
 Materials: Materials are objects which are necessary for a student to learn
something.  For instance, to learn to speak a language, you might need
a textbook. To learn to ride a horse, you might want to require a helmet.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - name of the lesson they're trying to learn
.EP
.SP 9 5
player - player object of the player trying to learn

.EP

.SI 3
* %^BOLD%^has_required_skills%^RESET%^
.EI
.SI 5
int has_required_skills(string name, object player)
.EI
.SP 7 5
has_required_skills( string name, object player )
Skills: Skills and skill levels which may be necessary to learn something
For instance, one should learn to walk before one learns to run.  If you
don't have sufficient skill levels in walking, you won't be able to learn
how to run.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - name of the lesson they're trying to learn
.EP
.SP 9 5
player - player object of the player trying to learn

.EP

.SI 3
* %^BOLD%^lesson_end%^RESET%^
.EI
.SI 5
int lesson_end(string name, object student)
.EI

.SI 3
* %^BOLD%^lesson_response%^RESET%^
.EI
.SI 5
int lesson_response(string name, object student, int lessonstep, string args)
.EI

.SI 3
* %^BOLD%^query_lessons%^RESET%^
.EI
.SI 5
string * query_lessons()
.EI
.SP 7 5

This method returns an array of the registered lessons.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a list of lesson names.

.EP

.SI 3
* %^BOLD%^register_lesson%^RESET%^
.EI
.SI 5
int register_lesson(string name, mapping minimum_skills, mapping maximum_skills, string * materials, int cost, string currency, string * lesson_ending, object teacher)
.EI

.SI 3
* %^BOLD%^start_lesson%^RESET%^
.EI
.SI 5
int start_lesson(string name, object player)
.EI
.SP 7 5

This method starts a lesson for a given player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
the - lesson name
.EP
.SP 9 5
the - player

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^lesson_step%^RESET%^
.EI
.SI 5
void lesson_step(string name, object student, int lessonstep)
.EI


