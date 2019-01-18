.DT
artifact_handler.c
Disk World autodoc help
artifact_handler.c

.SH Description
.SP 5 5

Keeps track of all the artifacts and the players known artifacts
arrays.

.EP
.SP 10 5

.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_artifact%^RESET%^
.EI
.SI 5
void add_artifact(string name, string file, int max, int min, int bonus, int value)
.EI
.SP 7 5

Add a new artifact into the artificat mapping. 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the artifact
.EP
.SP 9 5
file - the file name of the artifact
.EP
.SP 9 5
max - the maximum level
.EP
.SP 9 5
min - the minimum level
.EP
.SP 9 5
bonus - the bonus required to make
.EP
.SP 9 5
value - the value of the artifact

.EP

.SI 3
* %^BOLD%^add_known%^RESET%^
.EI
.SI 5
void add_known(string person, string artifact)
.EI
.SP 7 5

Place an artifact into a players known artifact list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the name of the person knowing the artifact
.EP
.SP 9 5
artifact - the name of the artifact being know

.EP

.SI 3
* %^BOLD%^diagnose_artifact%^RESET%^
.EI
.SI 5
void diagnose_artifact(object thing, object person,  object)
.EI
.SP 7 5

Let a player attempt to find out what type of artifact the
thing is.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the item to diagnose
.EP
.SP 9 5
person - the person doing the diagnosis

.EP

.SI 3
* %^BOLD%^query_artifact%^RESET%^
.EI
.SI 5
int query_artifact(string name)
.EI
.SP 7 5

Returns the information about the named artificats.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the artificant name to get information on
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the informatioj on the artificant, 0 if the artifact does not exist

.EP

.SI 3
* %^BOLD%^query_artifact_bonus%^RESET%^
.EI
.SI 5
int query_artifact_bonus(string name)
.EI
.SP 7 5

Find the bonus to make associated with the artifact.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the artifact
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the bonus to make associated with the artifact

.EP

.SI 3
* %^BOLD%^query_artifact_file%^RESET%^
.EI
.SI 5
string query_artifact_file(string name)
.EI
.SP 7 5

Find the file associated with the artifact.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the artifact
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the file associated with the artifact

.EP

.SI 3
* %^BOLD%^query_artifact_level%^RESET%^
.EI
.SI 5
int query_artifact_level(string name, int bonus)
.EI
.SP 7 5

Find the level of the artifact.  The level is determined as the
difference between the minimum and maximum artifact level squared
divided by the bonus passed in minus the bonus to create  plus the difference
between the maximum and minimum levels plus the maximum level.
.EP
.SP 7 5
ie:
.EP
.SP 7 5
level = (((max-min) * (max-min)) / (bonus - make_bonus + max - min)) + min
.EP
.SP 7 5

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the artifact
.EP
.SP 9 5
bonus - the bonus modifier
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the level of the artifact

.EP

.SI 3
* %^BOLD%^query_artifact_max%^RESET%^
.EI
.SI 5
int query_artifact_max(string name)
.EI
.SP 7 5

Find the maximum level associated with the artifact.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the artifact
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the maximum level associated with the artifact

.EP

.SI 3
* %^BOLD%^query_artifact_min%^RESET%^
.EI
.SI 5
int query_artifact_min(string name)
.EI
.SP 7 5

Find the minimum level associated with the artifact.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the artifact
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the minimum level associated with the artifact

.EP

.SI 3
* %^BOLD%^query_artifact_value%^RESET%^
.EI
.SI 5
int query_artifact_value(string name, int bonus)
.EI
.SP 7 5

Determine the value of the artificant.  The value is determined by
the following equation:
.EP
.SP 7 5

.EP
.SP 7 5
value = (value * max) / query_artifiact_level(name, bonus)
.EP
.SP 7 5

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the artifact
.EP
.SP 9 5
bonus - the bonus modifier
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the value of the artifact

.EP

.SI 3
* %^BOLD%^query_artifact_value_info%^RESET%^
.EI
.SI 5
int query_artifact_value_info(string name)
.EI
.SP 7 5

Find the value associated with the artifact.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the artifact
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the value associated with the artifact

.EP

.SI 3
* %^BOLD%^query_artifacts%^RESET%^
.EI
.SI 5
mapping query_artifacts()
.EI
.SP 7 5

This method returns the artifacts mapping.
The artifacts mapping has elements of the form:
.EP
.SP 7 5

.EP
.SP 7 5
   name : ({ file, max level, min level, bonus to make, value info })
.EP
.SP 7 5
e.g. "wand of eternal light" :
.EP
.SP 7 5
       ({ "/obj/wands/eternal_light", 85, 65, 250, 10000 })
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the artificats mapping

.EP

.SI 3
* %^BOLD%^query_known%^RESET%^
.EI
.SI 5
int query_known(string person, string artifact)
.EI
.SP 7 5

A person will not forget how to recognise an artifact (unless they
    refresh, in which case all of them are forgotten, i.e. the file
    is wiped) so to let a person learn to recognise an artifact, add
    a new line to their file consisting of its name and to see if
    they know an artifact, check in their file for its name. This
    method doesn't require a library handler.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person check to see if they know the artifact
.EP
.SP 9 5
artifact - the artifact being checked
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they know it, 0 if they do not

.EP

.SI 3
* %^BOLD%^query_wand_name%^RESET%^
.EI
.SI 5
string query_wand_name()
.EI
.SP 7 5

The name of the wand.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the wand name

.EP

.SI 3
* %^BOLD%^remove_artifact%^RESET%^
.EI
.SI 5
void remove_artifact(string name)
.EI
.SP 7 5

Remove the artifact of the given name.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the artifiact to remove

.EP

.SI 3
* %^BOLD%^save_me%^RESET%^
.EI
.SI 5
void save_me()
.EI
.SP 7 5
Saves the current state to disk. 
.EP

.SI 3
* %^BOLD%^zap%^RESET%^
.EI
.SI 5
void zap(object thing, object person, object wand)
.EI
.SP 7 5

Zap the wand!
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the thing to zap with the wand
.EP
.SP 9 5
person - the person doing the zapping
.EP
.SP 9 5
wand - the wand being zapped

.EP


