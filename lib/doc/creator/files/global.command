.DT
/global/command
Discworld creator help
/global/command

.SH Name
.SI 5
/global/command.c - daemon for commands
.EI

.SH Description
.SP 5 5
To write a command that uses the commands daemon, you can just put it
in a directory in /cmds.  General player commands should go in /cmds/player,
creator commands in /cmds/creator, etc.  If you add a new directory, be
sure to add the new directory to the player's searchpath.
E.g. when you make a directory /cmds/guild, you would add this line somewhere
in the player object:

AddSearchPath( ({ "/cmds/guild" }) );

You might consider adding a #define in the include file for the commands 
daemon, /include/command.h.

The directory /cmds/guild-race is special.  This replaces the old guild/race
commands system which used to live in /std/commands.  Commands that
are placed in this directory, or subdirectories thereof, are only allowed
to be used by livings if they have been taught to them, i.e. if they are
in a livings query_known_commands() array.  If you want your command
to be teachable, you need to define a function with the following prototype:

int teach( object ob );

Where `ob' is the object to be taught and this_player() is the teacher.
A return value of 0 means failure, and 1 means success.  You should
probably do some checks on the skills of both teacher and learner.
A command can be given to a living with the add_known_command() lfun, which
is defined in /global/guild-race.c, which both players and NPCs inherit.

The name of the command as it has to be typed by the player should be the
filename of the command.  To use abbreviation, like the '*' in add_action(),
use a '_' in the filename where you would normally have placed the '*'.
E.g. the look command is named l_ook.c.

The main function of the command, which gets called by the commands daemon,
has the following prototype:

mixed cmd( string args, string verb );

So if your command is called kill, it will get called like this:

Player types "kill Turrican",
The cmd function in the kill command gets called like this:

  kill->cmd( "Turrican", "kill" );

The function cmd() is just like any other add_action'ed function, return 0
for a failure, return 1 for success.

There is also the possibility to use Discworlds great parser, and have
add_command() like commands.  To do this, you need to define the following
function:

mixed *query_patterns();

This function should return an array of the following form:
.EP

.SI 5
({ <pattern1>, <function_pointer1>, <pattern2>, <function_pointer2>, ...
   <patternn>, <function_pointern> })
.EI

.SP 5 5
Where `patternn' is the add_command() style pattern, and
`function_pointern' is the function to be evaluated when what the player
typed matches `patternn'.  The standard prototype for your cmd() function
changes when you use this way of parsing.  It depends on how you
defined your function pointer.  You have the full set of add_command()
arguments at your disposal, so your maximum prototype looks like this:
.EP

.SI 5
mixed cmd( object *indirect_obs, string dir_match, string indir_match,
           mixed *args, string pattern );
mixed cmd( object **indirect_obs, string *dir_match, string indir_match,
           mixed *args, string pattern );
.EI

.SP 5 5
For more help on this, please see the add_command() manual page.
Look at the file /cmds/living/put.c for a nice example.

Help for your commands can be written as nroff-like documentation and put
in the apropriate directory in /doc, or you can define a help() function
in your command, with the following prototype:

string help();

This should return a helpfull text about your command.

A template is available in /cmds/template.c.
.EP

.SH Example
.SI 5
See /cmds/template.c, or a file in one of the dirs in /cmds.
.EI

.SH See also
.SI 5
add_action(), add_command()
.EI
