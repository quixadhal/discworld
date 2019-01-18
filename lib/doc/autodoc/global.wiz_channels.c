.DT
wiz_channels.c
Disk World autodoc help
wiz_channels.c

.SH Description
.SP 5 5

This method handles the command line and returns the type of command that
is represented by the command line.
.EP
.SP 10 5

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/newudp.h, /include/network.h, /include/broadcaster.h, /include/socket_errors.h and /include/creator.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^channel_list%^RESET%^
.EI
.SI 5
nomask string * channel_list()
.EI

.SI 3
* %^BOLD%^wiz_commands%^RESET%^
.EI
.SI 5
void wiz_commands()
.EI


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^handle_command_line%^RESET%^
.EI
.SI 5
int handle_command_line(string channel, string mess)
.EI


