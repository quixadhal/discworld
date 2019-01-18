.DT
command.h
Disk World autodoc help
command.h

.SH Classes

.SI 3
* %^BOLD%^command%^RESET%^
class command {
string verb;
string file;
string args;
string filepart;
}

.EI

.SI 3
* %^BOLD%^command_class%^RESET%^
class command_class {
mapping patterns;
string * order;
}

.EI

.SI 3
* %^BOLD%^command_data%^RESET%^
class command_data {
int weight;
mixed * calls;
}

.EI


.SH Defines
.SI 3
* %^BOLD%^CMD_D%^RESET%^
.EI

.SI 3
* %^BOLD%^DIR_CREATOR_CMDS%^RESET%^
.EI

.SI 3
* %^BOLD%^DIR_EXEC_PLAYTESTER_CMDS%^RESET%^
.EI

.SI 3
* %^BOLD%^DIR_GUILD_CMDS%^RESET%^
.EI

.SI 3
* %^BOLD%^DIR_LIVING_CMDS%^RESET%^
.EI

.SI 3
* %^BOLD%^DIR_LORD_CMDS%^RESET%^
.EI

.SI 3
* %^BOLD%^DIR_PLAYER_CMDS%^RESET%^
.EI

.SI 3
* %^BOLD%^DIR_PLAYTESTER_CMDS%^RESET%^
.EI

.SI 3
* %^BOLD%^DIR_SECURE_CREATOR_CMDS%^RESET%^
.EI

.SI 3
* %^BOLD%^DIR_SECURE_LORD_CMDS%^RESET%^
.EI

.SI 3
* %^BOLD%^DIR_SECURE_PLAYER_CMDS%^RESET%^
.EI

.SI 3
* %^BOLD%^DIR_SENIOR_PLAYTESTER_CMDS%^RESET%^
.EI

