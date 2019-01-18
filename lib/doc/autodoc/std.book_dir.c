.DT
book_dir.c
Disk World autodoc help
book_dir.c

.SH Description
.SP 5 5

Make a book with all the files in a dir.

.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/book.
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
* %^BOLD%^query_language%^RESET%^
.EI
.SI 5
string query_language()
.EI
.SP 7 5

Returns the language of the book

.EP

.SI 3
* %^BOLD%^set_book_dir%^RESET%^
.EI
.SI 5
void set_book_dir(string dir)
.EI
.SP 7 5

Sets the directory to look for the book pages in.  It scans every
file in this dir to get the pages for the book.
This sets the default paper to /obj/misc/nroff_paper.c
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory in which to search
.EP


