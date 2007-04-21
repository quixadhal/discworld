.DT
shovel.c
DW_wibble autodoc help
shovel.c

.SH Description
.SP 5 5

This is a clonable shovel object (a "shovel" in this case being anything
that can be used for digging, from a garden trowel to a steam shovel).
The "dig" command is defined with the following syntax:

.EP
.SO 6 2 -10

	*	"with <direct:object> in <string>" 
	*	"with <direct:object> in <indirect:object>" 
	*	"with <direct:object>" 
	*	"[in] <string> with <direct:object>" 
	*	"[in] <indirect:object> with <direct:object>" 
.EO
.SP 5 5

.EP
.SP 5 5
The do_dig() function tries to call a function dig() on this_player()'s environment, which should be defined as 

int dig( object indir, string indir_s ) 

where 
.EP
.SO 9 2 -13

	*	%^CYAN%^indir%^RESET%^is the <indirect:object> matched (if any) 
	*	%^CYAN%^indir_s%^RESET%^is the <string> matched, or the string that matched <indirect:object> 
.EO
.SP 5 5

.EP
.SP 5 5
Note that the command can match either actual objects or just strings; also note that, although it will match room items, the object passed won't really be of much use (which is why the string is passed also). 
.EP
.SP 10 5

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /obj/weapon.
.EP

