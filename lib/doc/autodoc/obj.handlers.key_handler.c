.DT
key_handler.c
Disk World autodoc help
key_handler.c

.SH Description
.SP 5 5

This handler keeps track of the keys.  The keys are arranged in
groups and each key type in the group has a name and a set of
permissions associated with it.
.EP
.SP 10 5


Written by Deutha

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/object.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_key_permission%^RESET%^
.EI
.SI 5
int add_key_permission(string group, string key, string type)
.EI
.SP 7 5

This method adds in a specific permission type into the current
permission set for this key.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
group - the key group to add the permision to
.EP
.SP 9 5
key - the key to add the permission for
.EP
.SP 9 5
type - the permission type to add in
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if it is already added, 1 if not
.EP

.SI 3
* %^BOLD%^delete_key_name%^RESET%^
.EI
.SI 5
void delete_key_name(string group, string key)
.EI
.SP 7 5

This method removes a specified key name from the current key name
mapping.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
group - the group to remove the key from
.EP
.SP 9 5
key - the key type to remove the name of
.EP

.SI 3
* %^BOLD%^query_all_key_names%^RESET%^
.EI
.SI 5
mapping query_all_key_names(string group)
.EI
.SP 7 5

This method returns all the key names for the given key group.
The index of the mapping is the key type and the value is its name.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
group - the key group
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the mapping containing the key names
.EP

.SI 3
* %^BOLD%^query_all_key_permissions%^RESET%^
.EI
.SI 5
string * query_all_key_permissions(string group, string key)
.EI
.SP 7 5

This method returns all the permision types associated with the key.
The permission types are an array of string permision type names.  If
the key name is non-existant an empty array will be returned.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
group - the key group to lookup
.EP
.SP 9 5
key - the key name to lookup
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of permission types
.EP

.SI 3
* %^BOLD%^query_all_keys%^RESET%^
.EI
.SI 5
mapping query_all_keys(string group)
.EI
.SP 7 5

This method returns all the keys in the current group.  The mapping
consists has an index of the key name and a value of an array of
key permission types.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
group - the key group to get permissions for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a mapping containing all the permissions
.EP

.SI 3
* %^BOLD%^query_key_name%^RESET%^
.EI
.SI 5
string query_key_name(string group, string key)
.EI
.SP 7 5

This method returns the key name for a given key.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
group - the key group to lookup
.EP
.SP 9 5
key - the key type to look up
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the key's name
.EP

.SI 3
* %^BOLD%^query_key_permission%^RESET%^
.EI
.SI 5
int query_key_permission(string group, string key, string type)
.EI
.SP 7 5

This method will see if the given key perrmision exists for this
key in this particular group and key.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
group - the key group to look up in
.EP
.SP 9 5
key - the key to check
.EP
.SP 9 5
type - the permission type to see if it exists
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the permission exists, 0 if not
.EP

.SI 3
* %^BOLD%^remove_key_permission%^RESET%^
.EI
.SI 5
int remove_key_permission(string group, string key, string type)
.EI
.SP 7 5

This method removes a specific permission type from the given
key group and key.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
group - the key group to remove the key from
.EP
.SP 9 5
key - the key to remove the permission from
.EP
.SP 9 5
type - the type of permission to remove
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if the permission was not remove, 1 if it was
.EP

.SI 3
* %^BOLD%^set_key_name%^RESET%^
.EI
.SI 5
void set_key_name(string group, string key, string name)
.EI
.SP 7 5

This method sets the name of the specified key.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
group - the group to set the name of the key in
.EP
.SP 9 5
key - the type of key to set the name for
.EP
.SP 9 5
name - the name of the key to set
.EP

.SI 3
* %^BOLD%^set_key_permissions%^RESET%^
.EI
.SI 5
void set_key_permissions(string group, string key, string * types)
.EI
.SP 7 5

This method will set the key permissions for a certain sort of
key.  It will overwrite any current possible key permission sets.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
group - the key group to set the permissions for
.EP
.SP 9 5
key - the key to set the permission for
.EP
.SP 9 5
types - the new set of permission types
.EP


