.DT
mime.h
Disk World autodoc help
mime.h

.SH Description
.SP 5 5

The header file for the MIME header parser.

.EP
.SP 10 5

.EP

.SH Classes

.SI 3
* %^BOLD%^mime_header%^RESET%^
class mime_header {
mapping header_m;
string * header_k;
}

.EI


.SH Defines
.SI 3
* %^BOLD%^isalnum(c)%^RESET%^
.EI
.SP 7 5

Checks if c is an alphanumeric character.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
c - the character to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if c is a letter or digit, 0 otherwise

.EP

.SI 3
* %^BOLD%^iscntrl(c)%^RESET%^
.EI
.SP 7 5

Checks if c is a control character.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
c - the character to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if c is a control character, 0 otherwise

.EP

.SI 3
* %^BOLD%^isdigit(c)%^RESET%^
.EI
.SP 7 5

Checks if c is a digit character.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
c - the character to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if c is a digit, 0 otherwise

.EP

.SI 3
* %^BOLD%^isspace(c)%^RESET%^
.EI
.SP 7 5

Checks if c is a white-space character.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
c - the character to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if c is a white-space character, 0 otherwise

.EP

.SI 3
* %^BOLD%^isxdigit(c)%^RESET%^
.EI
.SP 7 5

Checks if c is a hexadecimal digit.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
c - the character to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if c is a hexadecimal digit, 0 otherwise

.EP

.SI 3
* %^BOLD%^MIME%^RESET%^
.EI
.SP 7 5

The path to the MIME parser.

.EP

