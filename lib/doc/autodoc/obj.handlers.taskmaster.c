.DT
taskmaster.c
Disk World autodoc help
taskmaster.c

.SH Description
.SP 5 5

This handler gives access to the functions needed for the taskmaster
system.

Rewritten by Shrike to include degrees of success.

.EP
.SP 10 5


Written by Deutha
.EP



.SH See also
.SP 5 5
help::tasks

.EP
.SH Includes
.SP 5 5
This class includes the following files /include/tasks.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^attempt_task%^RESET%^
.EI
.SI 5
varargs mixed attempt_task(int difficulty, int bonus, int upper, mixed extra, int degree)
.EI
.SP 7 5

Do not use this function. Use perform_task instead.
.EP

.SI 3
* %^BOLD%^attempt_task_e%^RESET%^
.EI
.SI 5
varargs mixed attempt_task_e(int difficulty, int bonus, int upper, int half, int degree)
.EI
.SP 7 5

Do not use this function. Use perform_task instead.
.EP

.SI 3
* %^BOLD%^compare_skills%^RESET%^
.EI
.SI 5
varargs mixed compare_skills(object offob, string offskill, object defob, string defskill, int modifier, int off_tm_type, int def_tm_type, int degree)
.EI
.SP 7 5

This function will compare the skills of two objects and return which one
won and if the winner got a TM.
With equal skills the chances are 50/50 as to who will win. As the balance
shifts so do the chances. Additionally a modifier can be applied to
account for conditions favouring one or the other. This should be
considered a percentage eg. -50 will add 50% to the defenders chances of
winning.
If

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
offob - The attacking object
.EP
.SP 9 5
offskill - The name of the skill the attacker is using
.EP
.SP 9 5
deffob - The defending object
.EP
.SP 9 5
deffskill - The name of the skill the defender is using
.EP
.SP 9 5
modifier - The percentage modifier
.EP
.SP 9 5
off_tm_type - This should be one of the standard definitions in
/include/tasks.h and is for the attacker
.EP
.SP 9 5
def_tm_type - This should be one of the standard definitions in
/include/tasks.h and is for the defender
.EP
.SP 9 5
degree - Enable the degree-of-success code.  If this is used, the
 return value becomes a class instead of an int.

.EP

.SI 3
* %^BOLD%^is_critical%^RESET%^
.EI
.SI 5
int is_critical(int margin)
.EI
.SP 7 5

Figure out whether a given margin of success/failure will result
in a critical or not.  Takes an integer between -100 and 100,
representing the margin by which a skillcheck succeeded, and does
some logarithmic-type equations to figure out whether it should
result in a critical or not.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
margin - the margin of success.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the result is a critical, 0 otherwise.

.EP

.SI 3
* %^BOLD%^perform_task%^RESET%^
.EI
.SI 5
varargs mixed perform_task(object person, string skill, int difficulty, int tm_type, int degree)
.EI
.SP 7 5

This function will perform a task.  It handles all the stuff about
looking up the skill, and giving the tm advance, but doesn't give
any messages to the player, you'll have to do that.
If you set the 'degree' parameter to 1, it will return a 3-element
class instead of the standard binary success/failure result.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the one attempting the task (could be any living thing)
.EP
.SP 9 5
skill - the skill tested against
.EP
.SP 9 5
difficulty - the lowest bonus where the attempt can succeed
.EP
.SP 9 5
tm_type - This should use one of the standard definitions in 
/include/tasks.h. They are:
       TM_FIXED - for use where the difficulty is a fixed value
       TM_FREE  - for use when the tm attempt doesn't cost anything.
       TM_CONTINUOUS - for use in continuous actions eg. combat or sneak
       TM_COMMAND - for use with guild commands
       TM_RITUAL - when the action is a ritual
       TM_SPELL - when the action is a spell
       TM_NONE - when you want there to be no chance of a tm
.EP
.SP 9 5
degree - Whether or not to use the degree of success code

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
BARF if something screwed up, AWARD if the task succeeded, and
should give an advance, SUCCEED if it succeeded, FAIL if it failed. If
the degree of success code is used, it will pass out a 3-element class.

.EP

.SI 3
* %^BOLD%^set_control%^RESET%^
.EI
.SI 5
void set_control(mixed * args)
.EI
.SP 7 5

This function should only be used in the very rare situations
where the last skill checked with query_skill_bonus() wasn't the
one relevant for the task attempt.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
args - an array consisting of ({ object player, string skill })

.EP


