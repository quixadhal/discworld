.DT
club_family.c
Disk World autodoc help
club_family.c

.SH Description
.SP 5 5

This is the handler for all things clubby, a club being a group of
players.  Each club must have a unique name.  It also handles elections
for various club positions.
.EP
.SP 10 5


Written by Pinkfish

Started Sun Sep 27 03:35:42 EDT 1998

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /obj/handlers/inherit/club_personal.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/am_time.h and /include/clubs.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^family_info%^RESET%^
class family_info {
mapping relationships;
string * never_join;
string tmp;
int tmp2;
}

.EI
.SP 7 5

The stuff releated to families.
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
relationships - the relations in this family
.EP
.SP 9 5
never_join - the list of people who can never join

.EP

.SI 3
* %^BOLD%^relationship_type_data%^RESET%^
class relationship_type_data {
string opposite;
int flags;
}

.EI
.SP 7 5

The information associated with relationship names.
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
opposite - the opposite relationship name
.EP
.SP 9 5
flags - flags associated with the relationship's

.EP


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_relationship%^RESET%^
.EI
.SI 5
int add_relationship(string name, string person, string dest_name, string dest, string relationship)
.EI
.SP 7 5

This method adds a relationship between two members of a family.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the family
.EP
.SP 9 5
person - the person the relationship is from
.EP
.SP 9 5
dest - the person the relationship is to
.EP
.SP 9 5
dest_name - the name of the family the other person is in
.EP
.SP 9 5
relationship - the relationship they have to you
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if succcessful, 0 if not

.EP

.SI 3
* %^BOLD%^add_valid_relationship%^RESET%^
.EI
.SI 5
void add_valid_relationship(string relation1, string relation2, int flags)
.EI
.SP 7 5

This method adds a valid relationship to the current list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
relation1 - the first relationship
.EP
.SP 9 5
relation2 - the second relationship
.EP

.SI 3
* %^BOLD%^is_relationship%^RESET%^
.EI
.SI 5
int is_relationship(string name, string person, string related, string relationship)
.EI
.SP 7 5

This method checks to see if the specified relationship exists.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the family the relationship is in
.EP
.SP 9 5
person - the person the relationship is from
.EP
.SP 9 5
related - to person they are related to
.EP
.SP 9 5
relationship - the type of relationship they have
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the relationship exists, 0 if not

.EP

.SI 3
* %^BOLD%^is_valid_interfamily_relationship%^RESET%^
.EI
.SI 5
int is_valid_interfamily_relationship(string str)
.EI
.SP 7 5

This method checks to see if the relation is a valid interfamily one.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the relationship to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the relation is a valid interfamily one
.EP

.SI 3
* %^BOLD%^is_valid_join_family_relationship%^RESET%^
.EI
.SI 5
int is_valid_join_family_relationship(string str)
.EI
.SP 7 5

This method checks to see if the relation is a valid join family one.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the relationship to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the relation is a valid join family one
.EP

.SI 3
* %^BOLD%^is_valid_relationship%^RESET%^
.EI
.SI 5
int is_valid_relationship(string str)
.EI
.SP 7 5

This method checks to make sure the specified relationship is valid.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
relation - the relationship to check for validity
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the relationship is valid
.EP

.SI 3
* %^BOLD%^move_family_member%^RESET%^
.EI
.SI 5
int move_family_member(string curr_family, string person, string dest_family)
.EI
.SP 7 5

This method moves a person from one family to another family.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
curr_family - the current family
.EP
.SP 9 5
person - the persons name
.EP
.SP 9 5
dest_family - the destination family
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if successful moved
.EP

.SI 3
* %^BOLD%^query_all_relationships%^RESET%^
.EI
.SI 5
string * query_all_relationships()
.EI
.SP 7 5

This method returns all the possible relationships, including the
gendered ones.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of possible relationships
.EP

.SI 3
* %^BOLD%^query_all_valid_relationships%^RESET%^
.EI
.SI 5
string * query_all_valid_relationships()
.EI
.SP 7 5

This method returns all the valid relationships.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
all the current valid relationship types
.EP

.SI 3
* %^BOLD%^query_club_cost_per_period%^RESET%^
.EI
.SI 5
int query_club_cost_per_period(string club_name)
.EI
.SP 7 5

This method determines how much the club will cost to run each
pay period.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club to get the fees for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the amount the club will cost in the next pay period
.EP

.SI 3
* %^BOLD%^query_opposite_relationship%^RESET%^
.EI
.SI 5
string query_opposite_relationship(string relation)
.EI
.SP 7 5

This method returns the opposite relationship to the specified one.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
relation - the relation to return the opposite of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the opposite of the relationship
.EP

.SI 3
* %^BOLD%^query_relationship_gender%^RESET%^
.EI
.SI 5
string query_relationship_gender(string relationship, int female, string region)
.EI
.SP 7 5

This method returns the genderised version of the relationship.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - relationship the relation to get the gender of
.EP
.SP 9 5
female - 0 for male, 1 for female
.EP
.SP 9 5
region - the region the relation is in
.EP

.SI 3
* %^BOLD%^query_relationships%^RESET%^
.EI
.SI 5
class relationship_data * query_relationships(string name, string person)
.EI
.SP 7 5

This method returns the relationships the selected person has to
the other people in the family.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the family
.EP
.SP 9 5
person - the person who to query the relationships for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of all the relationships the person has

.EP

.SI 3
* %^BOLD%^query_relationships_to%^RESET%^
.EI
.SI 5
string * query_relationships_to(string name, string person, string related)
.EI
.SP 7 5

This method returns how the first player is related to the second player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the family the relationship is in
.EP
.SP 9 5
person - the person the relationship is from
.EP
.SP 9 5
related - to person they are related to
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of relationships they have

.EP

.SI 3
* %^BOLD%^query_ungendered_relationship%^RESET%^
.EI
.SI 5
string query_ungendered_relationship(string relationship)
.EI
.SP 7 5

This returns the real relationship type based on the possible
genderised relationship.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
relationship - the relationship to get the real relationship for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the degendered relationship name
.EP

.SI 3
* %^BOLD%^remove_relationship%^RESET%^
.EI
.SI 5
int remove_relationship(string name, string person, string dest_name, string dest, string relationship)
.EI
.SP 7 5

This method removes a relationship from the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the family
.EP
.SP 9 5
person - the person the relationship is from
.EP
.SP 9 5
dest - the person the relationship is to
.EP
.SP 9 5
relationship - the relationship they have to you
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if succcessful, 0 if not

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^check_family_information%^RESET%^
.EI
.SI 5
void check_family_information(string name, string member, int startup)
.EI
.SP 7 5

This method checks the extra information for the club.  Please note
this does *not* save the information.  You need to do this yourself.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to check
.EP

.SI 3
* %^BOLD%^create_extra_data%^RESET%^
.EI
.SI 5
void create_extra_data(string name)
.EI


