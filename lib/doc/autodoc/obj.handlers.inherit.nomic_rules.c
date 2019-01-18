.DT
nomic_rules.c
Disk World autodoc help
nomic_rules.c

.SH Description
.SP 5 5

This handler deals with all the nomic rules for councils.
.EP
.SP 10 5


Written by Pinkfish

Started Fri Jun 23 19:51:04 PDT 2000

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
* %^BOLD%^add_nomic_rule%^RESET%^
.EI
.SI 5
int add_nomic_rule(string area, int type, string creator, string text)
.EI
.SP 7 5

This method adds a nomic rule into the system.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area the rule is added in
.EP
.SP 9 5
type - the type of rule to add
.EP
.SP 9 5
creator - the person that created the rule
.EP
.SP 9 5
text - the text associated with the rule
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the new number of the rule, NOMIC_ERROR on fai;l

.EP

.SI 3
* %^BOLD%^change_nomic_rule%^RESET%^
.EI
.SI 5
int change_nomic_rule(string area, class nomic_rule rule)
.EI
.SP 7 5

This method moves a rule from one type to another.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area the rule is added in
.EP
.SP 9 5
rule - the rule to move
.EP
.SP 9 5
type - the new type of the rule
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the new number of the rule, NOMIC_ERROR on fai;l

.EP

.SI 3
* %^BOLD%^create_area%^RESET%^
.EI
.SI 5
int create_area(string area)
.EI
.SP 7 5

This method creates an new nomic area.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the new nomic area name
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^find_nomic_rule%^RESET%^
.EI
.SI 5
class nomic_rule find_nomic_rule(string area, string creator, string text)
.EI
.SP 7 5

This method searches through for the specified rule.  This stops the
same rule being added more than once by some error in the code.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area the rule is in
.EP
.SP 9 5
creator - the creator of the rule
.EP
.SP 9 5
text - the text of the rule
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the rule if it is found, 0 if not

.EP

.SI 3
* %^BOLD%^int_remove_nomic_rule%^RESET%^
.EI
.SI 5
int int_remove_nomic_rule(string area, int id)
.EI
.SP 7 5

This method by passes the standard nomic rules check, it can be used
to remove an immutable rule.

.EP

.SI 3
* %^BOLD%^query_all_nomic_rules%^RESET%^
.EI
.SI 5
class nomic_rule * query_all_nomic_rules(string area)
.EI
.SP 7 5

This method returns all the nomic rules for the area.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to get the rule sin
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
all the rules

.EP

.SI 3
* %^BOLD%^query_can_change_rule%^RESET%^
.EI
.SI 5
int query_can_change_rule(string area, class nomic_rule rule)
.EI
.SP 7 5

This method checks to see if the specified rule can be changed.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area in which the rule will be changed in
.EP
.SP 9 5
rule - the rule to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it can be changed, 0 if not

.EP

.SI 3
* %^BOLD%^query_nomic_rule%^RESET%^
.EI
.SI 5
class nomic_rule query_nomic_rule(string area, int id)
.EI
.SP 7 5

This method returns a nomic rule for the specified area.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to get the rule in
.EP
.SP 9 5
id - the id of the rule to match
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if failed, the class if succeeded

.EP

.SI 3
* %^BOLD%^query_type_name%^RESET%^
.EI
.SI 5
string query_type_name(int type_no)
.EI
.SP 7 5

This method returns the name of the corresponding type name.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type_no - the type number
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the type name string

.EP

.SI 3
* %^BOLD%^query_type_number%^RESET%^
.EI
.SI 5
int query_type_number(string name)
.EI
.SP 7 5

This method returns the number for the corresponding type name.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the area to get the number of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the type number

.EP

.SI 3
* %^BOLD%^remove_nomic_rule%^RESET%^
.EI
.SI 5
int remove_nomic_rule(string area, int id)
.EI
.SP 7 5

This method removes a nomic rule from the system.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area the rule is removed in
.EP
.SP 9 5
id - the id of the rule to remove
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the status of the removal, NOMIC_ERROR on fai;l

.EP

.SI 3
* %^BOLD%^rule_as_string%^RESET%^
.EI
.SI 5
string rule_as_string(int indent, class nomic_rule rule)
.EI
.SP 7 5

This method returns the rule as a string.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
rule - the rule to make a string
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the string version of us

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^add_nomic_rule_class%^RESET%^
.EI
.SI 5
int add_nomic_rule_class(string area, class nomic_rule new_rule, int type)
.EI
.SP 7 5

This method adds a nomic rule into the system.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area the rule is added in
.EP
.SP 9 5
new_rule - the rule that is being added
.EP
.SP 9 5
type - the type of the rule
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the new number of the rule, NOMIC_ERROR on fai;l

.EP

.SI 3
* %^BOLD%^move_nomic_rule%^RESET%^
.EI
.SI 5
int move_nomic_rule(string area, class nomic_rule rule, int type)
.EI
.SP 7 5

This method moves a rule from one type to another.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area the rule is added in
.EP
.SP 9 5
rule - the rule to move
.EP
.SP 9 5
type - the new type of the rule
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the new number of the rule, NOMIC_ERROR on fai;l

.EP


