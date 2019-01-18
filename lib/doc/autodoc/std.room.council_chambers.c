.DT
council_chambers.c
Disk World autodoc help
council_chambers.c

.SH Description
.SP 5 5

This method lists the current set of rules from the specified area.
.EP
.SP 10 5

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/basic_room and /std/room/inherit/council_base.
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
* %^BOLD%^do_amend_current_rule%^RESET%^
.EI
.SI 5
int do_amend_current_rule(int num)
.EI
.SP 7 5

This method adds an amendment to a current rule.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
num - the number of the current rule
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_amend_motion%^RESET%^
.EI
.SI 5
int do_amend_motion(int num)
.EI
.SP 7 5

This method allows the person to amend a new rule.
Amendments are only allowed by magistrates and only during the discusion
phase.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
num - the id of the rule to comment on
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_comment_on%^RESET%^
.EI
.SI 5
int do_comment_on(int num)
.EI
.SP 7 5

This method allows the person to make a comment on a current rule.
Comments are only allowed by magistrates and only in the first two
phases.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
num - the id of the rule to comment on
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_create_motion%^RESET%^
.EI
.SI 5
int do_create_motion(string type)
.EI
.SP 7 5

This method creates a new rule.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type of the new rule
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_list_motions%^RESET%^
.EI
.SI 5
int do_list_motions(int brief)
.EI
.SP 7 5

This lists all the new rules up for discussion or whatever by the
magistrates.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_list_one_rule%^RESET%^
.EI
.SI 5
int do_list_one_rule(int num)
.EI
.SP 7 5

This method lists the current set of rules from the specified area.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_list_rules%^RESET%^
.EI
.SI 5
int do_list_rules()
.EI

.SI 3
* %^BOLD%^do_list_single_motion%^RESET%^
.EI
.SI 5
int do_list_single_motion(int num)
.EI
.SP 7 5

This lists all the new rules up for discussion or whatever by the
magistrates.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_remove_current_rule%^RESET%^
.EI
.SI 5
int do_remove_current_rule(int num)
.EI
.SP 7 5

This method removes a current rule.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
num - the number of the current rule
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_transmogrify_current_rule%^RESET%^
.EI
.SI 5
int do_transmogrify_current_rule(int num, string new_type)
.EI
.SP 7 5

This method transmogrifies a current rule.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
num - the number of the current rule
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_vote_on%^RESET%^
.EI
.SI 5
int do_vote_on(int num, string vote)
.EI
.SP 7 5

This method will handle voting on the specific new nomic rule.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
num - the rule number to vote on
.EP
.SP 9 5
vote - their vote
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP


