.DT
pattern_compiler.c
Disk World autodoc help
pattern_compiler.c

.SH Description
.SP 5 5

Compiles up the patterns for use by the add_command system.
.EP
.SP 10 5


Written by Pinkfish
.EP



.SH See also
.SP 5 5
/global/new_parse->add_command()
.EP
.SH Includes
.SP 5 5
This class includes the following files /include/user_parser.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^clean_cache%^RESET%^
.EI
.SI 5
void clean_cache()
.EI

.SI 3
* %^BOLD%^compile_pattern%^RESET%^
.EI
.SI 5
nomask mixed * compile_pattern(string str)
.EI
.SP 7 5

Compiles the pattern.   Does no cache checking.  This returns
only the compiled pattern, it also creates the short pattern as a
side effect.  The short pattern can be queried by using
query_short_pattern()
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the pattern to compile
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the compiled pattern
.EP

.SI 3
* %^BOLD%^query_pattern%^RESET%^
.EI
.SI 5
nomask mixed * query_pattern(string pattern)
.EI
.SP 7 5

Returns the compiled pattern to the caller.   This checks to see if they
pattern is in its internal cache and if it is, it uses that.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pattern - the pattern to compiler
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the compiler pattern
.EP

.SI 3
* %^BOLD%^query_short_pattern%^RESET%^
.EI
.SI 5
string query_short_pattern(string str)
.EI
.SP 7 5

Returns the short pattern for the given pattern string.   The short pattern
is the message which is shown to the players.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the pattern to get the short for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the short pattern

.EP

.SI 3
* %^BOLD%^stats%^RESET%^
.EI
.SI 5
mixed * stats()
.EI


