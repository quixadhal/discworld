.DT
group_handler.c
Disk World autodoc help
group_handler.c

.SH Description
.SP 5 5

This is the group handler.  It does all the handling of
player-run adventuring groups, but not alone.  It's
accompanied by the group commands, effect and shadow.
For paths to these files, consult /include/group_handler.h

This system owes its design first and foremost to the irreplaceable
Ceres who wrote the original system.  Next come all the valuable
Creators who shared their knowledge of LPC.  Last but not least
are the countless playtesters who dedicated their time and worked
out all (or most of) the kinks and provided feedback and suggestions.

So give 'em all a big hug.

.EP
.SP 10 5


Written by Tape

Started February 99

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/command.h, /include/player_handler.h, /include/login_handler.h, /include/broadcaster.h and /include/group_handler.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^group%^RESET%^
class group {
int start_time;
string short;
string leader_name;
object leader;
object * members;
object * invited;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_invite%^RESET%^
.EI
.SI 5
int add_invite(string name, object person, int flag)
.EI
.SP 7 5

This function allows you to add a person to the invite array
of a group.  Only invited people are allowed to join a group.
If the call succeeds, an internal call_out is started which
runs out after INVITE_TIMEOUT seconds and removes the person
from the array.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the group
.EP
.SP 9 5
person - the person we're inviting
.EP
.SP 9 5
flag - set to 1 if you don't want the auto-removal of the invite

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 if the group doesn't exist or the person is
   already invited

.EP

.SI 3
* %^BOLD%^add_member%^RESET%^
.EI
.SI 5
int add_member(string name, object person)
.EI
.SP 7 5

This function allows you to add a member to a group.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the group
.EP
.SP 9 5
person - the person you're adding
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 if the group doesn't exist or person is
   already a member

.EP

.SI 3
* %^BOLD%^broadcast_to_groups%^RESET%^
.EI
.SI 5
void broadcast_to_groups(string * name, string message)
.EI
.SP 7 5

With this function you can broadcaster a message to all or
some of the groups currently active.  It will not include
your name, so make sure you identify yourself if necessary.

If "name" is empty or 0, the message will be broadcasted
to all groups.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - a string array with names of groups
.EP

.SI 3
* %^BOLD%^create_group%^RESET%^
.EI
.SI 5
int create_group(string name)
.EI
.SP 7 5

This function allows you to create a new group.  Note that the
name of the group shouldn't be just any name.  It should first
be filtered through short_to_name().  The short, set with
set_group_short() can be set to anything.  The "name" of a
group is mostly used internally to query and set stuff in
the handler.  The short is visible to players.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the new group
.EP
.SP 9 5
founder - points to the thing creating the group

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 if the group already exists

.EP

.SI 3
* %^BOLD%^disband_group%^RESET%^
.EI
.SI 5
varargs void disband_group(string name, mixed message)
.EI
.SP 7 5

This function does a clean removal of a group from the handler.
If a message is specified, it is broadcasted to the group
before all members are removed.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the group to be disbanded
.EP
.SP 9 5
message - message to be broadcasted, if any
.EP

.SI 3
* %^BOLD%^init_dynamic_arg%^RESET%^
.EI
.SI 5
void init_dynamic_arg(mapping map)
.EI

.SI 3
* %^BOLD%^invitations_to%^RESET%^
.EI
.SI 5
object * invitations_to(string name)
.EI
.SP 7 5

This function lists all invitations to a particular group.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the group
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
objects who have been invited to the group

.EP

.SI 3
* %^BOLD%^is_group%^RESET%^
.EI
.SI 5
int is_group(string group)
.EI
.SP 7 5

This function checks if such a group exists at the moment.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
group - the name of the group
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the group exists, 0 otherwise

.EP

.SI 3
* %^BOLD%^is_invited%^RESET%^
.EI
.SI 5
int is_invited(string name, object person)
.EI
.SP 7 5

This function allows you to check whether an invitation for a
person is pending in a specific group.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the group
.EP
.SP 9 5
person - the person to be checked

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if such an invitation exists, 0 if not

.EP

.SI 3
* %^BOLD%^is_member%^RESET%^
.EI
.SI 5
int is_member(string name, object person)
.EI
.SP 7 5

This function allows you to check whether a person is a member
of a group.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the group
.EP
.SP 9 5
person - the person you want to check for membership

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if he is a member, 0 if he isn't (or the group doesn't exist)


.EP

.SI 3
* %^BOLD%^leader_of%^RESET%^
.EI
.SI 5
object leader_of(string name)
.EI
.SP 7 5

This function returns the person who is currently
leading the group, if any.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the group
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the object pointing to the leader, 0 if none

.EP

.SI 3
* %^BOLD%^members_of%^RESET%^
.EI
.SI 5
object * members_of(string name)
.EI
.SP 7 5

This function returns all the members of a group.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the group
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an object * of the members

.EP

.SI 3
* %^BOLD%^notify_group%^RESET%^
.EI
.SI 5
void notify_group(string name, object broadcaster, mixed message)
.EI
.SP 7 5

This function broadcasts a message to the group's channel using
the broadcaster handler.  The first argument specifies the
group's name (not short), which also acts as the channel
name.  The second argument is the object doing the broadcasting.
The third argument varies.  It can either be a simple string,
in which case that string is printed as the message.  It can
also be a two-element string array.  The first element is
printed only to the object specified in the second argument.
The second element is printed to everyone else.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the group
.EP
.SP 9 5
object - person or object doing the broadcasting
.EP
.SP 9 5
message - the message to be broadcasted


.EP

.SI 3
* %^BOLD%^query_dynamic_auto_load%^RESET%^
.EI
.SI 5
mapping query_dynamic_auto_load()
.EI

.SI 3
* %^BOLD%^query_group_short%^RESET%^
.EI
.SI 5
string query_group_short(string name)
.EI
.SP 7 5

This returns the short description of a group.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the group
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the short of the group

.EP

.SI 3
* %^BOLD%^query_groups%^RESET%^
.EI
.SI 5
string * query_groups()
.EI
.SP 7 5

This returns the names of all groups currently in the database.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the names of all groups in database

.EP

.SI 3
* %^BOLD%^query_start_time%^RESET%^
.EI
.SI 5
int query_start_time(string name)
.EI
.SP 7 5

This function returns the time when the group was created.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the group
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the time when the group was created

.EP

.SI 3
* %^BOLD%^remove_group%^RESET%^
.EI
.SI 5
int remove_group(string name)
.EI
.SP 7 5

This function removes a group from the handler and is only used
internally.  No notification of this is given to players, and
no cleanup is done on their part.
If you want to force the deletion of a group, use disband_group()
instead.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - name of the group to be disbanded
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 if group doesn't exist
.EP

.SI 3
* %^BOLD%^remove_invite%^RESET%^
.EI
.SI 5
int remove_invite(string name, object person)
.EI
.SP 7 5

This function allows you to remove an invite of a person from
a group.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the group
.EP
.SP 9 5
person - the person you want to remove

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 if group doesn't exist or person hasn't been
   invited

.EP

.SI 3
* %^BOLD%^remove_member%^RESET%^
.EI
.SI 5
int remove_member(string name, object person)
.EI
.SP 7 5

This function allows you to remove a person from a group.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the group
.EP
.SP 9 5
person - the person you want removed

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 if group doesn't exist or person isn't a
   member

.EP

.SI 3
* %^BOLD%^rename_group%^RESET%^
.EI
.SI 5
int rename_group(string group, string new_group)
.EI
.SP 7 5

This method renames the group.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
group - the old group name
.EP
.SP 9 5
new_group - the new group name

.EP

.SI 3
* %^BOLD%^set_group_short%^RESET%^
.EI
.SI 5
int set_group_short(string name, string short_desc)
.EI
.SP 7 5

This sets the short description of a group.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the group
.EP
.SP 9 5
short_desc - the short of the group

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 if group doesn't exist

.EP

.SI 3
* %^BOLD%^set_leader%^RESET%^
.EI
.SI 5
varargs int set_leader(string name, object person, object appointer)
.EI
.SP 7 5

This function allows you to set a new leader for the group.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the group
.EP
.SP 9 5
person - the new leader
.EP
.SP 9 5
appointer - if this != 0, group is told he set the new leader

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 if the group doesn't exist

.EP

.SI 3
* %^BOLD%^short_to_name%^RESET%^
.EI
.SI 5
string short_to_name(string short)
.EI
.SP 7 5

This function concatenates the short of a group to a valid name
that can then be used with create_group().  The function will
also return invalid if a channel with such a name exists in the
broadcaster.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
short - the short of the group
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the name of the group, or "" if invalid

.EP

.SI 3
* %^BOLD%^shuffle_new_leader%^RESET%^
.EI
.SI 5
varargs object shuffle_new_leader(string group, int way, object * exclude)
.EI
.SP 7 5

This function allows you to choose a new leader for the group
in a variety of ways.  It only includes players who are
interactive().

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
group - the name of the group
.EP
.SP 9 5
way - 0 for random (no other ways present atm)
.EP
.SP 9 5
exclude - an object array of members to exclude from the start
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an object pointing to the new leader, or 0

.EP


