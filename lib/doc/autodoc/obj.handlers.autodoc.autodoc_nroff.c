.DT
autodoc_nroff.c
Disk World autodoc help
autodoc_nroff.c

.SH Description
.SP 5 5

This creates an nroff document from the generated automatic documentation.
It also writes out the single file descriptions for the methods with
documentation attached.

.EP
.SP 10 5


Written by Pinkfish

Started Tue Oct 28 11:39:47 EST 1997

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/autodoc.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^convert_html%^RESET%^
.EI
.SI 5
string convert_html(string text, int depth, int preformat, int edge_width)
.EI
.SP 7 5

Converts the html tags into nroff format...

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
text - the text to convert
.EP
.SP 9 5
depth - the current depth we are at
.EP
.SP 9 5
preformat - is this text preformated?
.EP
.SP 9 5
edge_width - the width to use as a buffer on the edge
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a nroff format file string

.EP

.SI 3
* %^BOLD%^create_nroff_file%^RESET%^
.EI
.SI 5
void create_nroff_file(object file, string output)
.EI
.SP 7 5

Writes the data from the autodoc file object out to a file in nroff
format.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
file - the autodoc file object
.EP
.SP 9 5
output - the output file to write to

.EP


