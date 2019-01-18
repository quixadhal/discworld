.DT
autodoc_html.c
Disk World autodoc help
autodoc_html.c

.SH Description
.SP 5 5

This creates an html document from the generated automatic documentation.

.EP
.SP 10 5


Written by Pinkfish

Started Tue Oct 28 11:39:47 EST 1997

.EP



.SH See also
.SP 5 5
/obj/handlers/autodoc/autodoc_handler
.EP
.SH Includes
.SP 5 5
This class includes the following files /include/config.h and /include/autodoc.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^create_html_file%^RESET%^
.EI
.SI 5
void create_html_file(object file, string output)
.EI
.SP 7 5

Writes the data from the autodoc file object out to a file in html
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

.SI 3
* %^BOLD%^create_html_index_file%^RESET%^
.EI
.SI 5
void create_html_index_file(string * index, string letter, string output)
.EI
.SP 7 5

Creates a html index file from the input index information.
This will contain all the functions starting with a certain letter
and the corresponding files they are mentioned in.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
index - the values in the index
.EP
.SP 9 5
letter - the letter this is an index of
.EP
.SP 9 5
output - the file to write the output to

.EP

.SI 3
* %^BOLD%^create_main_index%^RESET%^
.EI
.SI 5
void create_main_index(mapping chars, string output_dir)
.EI
.SP 7 5

Creates the basic index file which points to all the other index
files...  The input letters array is a paired array, the first element
is the letter and the second is the file name.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
letters - the names of all the other index files
.EP
.SP 9 5
output_dir - the directory to write stuff to

.EP


