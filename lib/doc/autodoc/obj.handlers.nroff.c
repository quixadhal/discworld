.DT
nroff.c
Disk World autodoc help
nroff.c

.SH Description
.SP 5 5

An nroff like object for formating text files.


This creates a saveable file format thingy. 
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/nroff.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^cat_file%^RESET%^
.EI
.SI 5
string cat_file(string fname, int update)
.EI
.SP 7 5

Creates a string from a nroffed file.


If update is non zero it will look to see if the source file has been modified since the saved nroff file was created. If it has, this function returns 0 and the nroff file needs to be recreated. 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
fname - the file to process
.EP
.SP 9 5
update - force update if file is newer?
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the string representation of the nroff file

.EP

.SI 3
* %^BOLD%^create_nroff%^RESET%^
.EI
.SI 5
int create_nroff(string in_file, string out_file)
.EI
.SP 7 5

Makes the nroff saveable file.  Turns the input nroff file into a saved
output format.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
in_file - the file name been processed
.EP
.SP 9 5
out_file - the file to save it as
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the operation was successful

.EP

.SI 3
* %^BOLD%^html_file%^RESET%^
.EI
.SI 5
string html_file(string file, string title)
.EI
.SP 7 5

Turns the nroff file into a html output.  This works on nroff files
creating a html output from the mud source.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
file - the name to process
.EP
.SP 9 5
title - the title to give the document
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the html file

.EP

.SI 3
* %^BOLD%^query_file_name%^RESET%^
.EI
.SI 5
string query_file_name(string fname)
.EI
.SP 7 5

Will attempt to find the name of the nroffed file.  Will attempt to
find the name of the source file associated with the save file.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
fname - the name of the file to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the source file name or 0

.EP


