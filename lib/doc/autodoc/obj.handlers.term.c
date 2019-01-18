.DT
term.c
Disk World autodoc help
term.c

.SH Description
.SP 5 5

This object handles all the terminal related stuff.  Figuring out to 
display each colour and verifying that the colours are correct.
The terminal handler.  This has all sorts of utterly useless junk on it
Share and enjoy.
Thrown up by Pinkfish.

   Modified by Godot to add the xterm terminal. (10/16/92)
   Added vt100 Chrisy 15 may 93

   Definitions:

       <colour> foreground colour B_<colour> background colour BOLD bold or bright letters ENDTERM unknown FLASH flashing or blinking letters INITTERM unknown RESET return to the normal font -- not a terminal reset. STATUS unknown WINDOW unknown REVERSE reverse video mode UNDERLINE underline mode 
.EP
.SP 10 5

.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^query_colour_codes%^RESET%^
.EI
.SI 5
string * query_colour_codes()
.EI
.SP 7 5

This method returns the array of usable colours.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of usable colours

.EP

.SI 3
* %^BOLD%^query_term_types%^RESET%^
.EI
.SI 5
string * query_term_types()
.EI
.SP 7 5

This method returns all the currently available types of terminals.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of terminal types

.EP

.SI 3
* %^BOLD%^set_network_term_type%^RESET%^
.EI
.SI 5
mixed set_network_term_type(string str)
.EI
.SP 7 5

This returns the mapping of colours based on the terminal type
entered in here.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the terminal type
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
mapping of colours to escape codes

.EP

.SI 3
* %^BOLD%^set_term_type%^RESET%^
.EI
.SI 5
mapping set_term_type(string str)
.EI
.SP 7 5

This returns the mapping of colours based on the terminal type
entered in here.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the terminal type
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
mapping of colours to escape codes

.EP


