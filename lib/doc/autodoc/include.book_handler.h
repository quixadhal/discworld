.DT
book_handler.h
Disk World autodoc help
book_handler.h

.SH Description
.SP 5 5

This file defines all the constants needed for the book handler and the
printing shop.
.EP
.SP 10 5


Written by David Bennett

Started Sun Feb 22 22:02:08 CST 1998
.EP



.SH See also
.SP 5 5
/obj/handlers/book_handler.c and /std/print_shop.c

.EP
.SH Defines
.SI 3
* %^BOLD%^BH_COPYRIGHT%^RESET%^
.EI
.SP 7 5

This is the property used to set the copyright status of the book.  The
value of the property should be the copyright holder of the book.
If you wish to setup a book which no one can copy then set this
variable to the name of the creator, or a random banished name.
.EP

.SI 3
* %^BOLD%^BH_IGNORE_PAGE%^RESET%^
.EI
.SP 7 5

This is the property to set to a non-zero value to make pages in books
be counted as ignored.  This should be done for things like fly leafs in
printed books and stuff like that.
.EP

.SI 3
* %^BOLD%^BOOK_HANDLER%^RESET%^
.EI
.SP 7 5

This is the path to the book handler object.
.EP

.SI 3
* %^BOLD%^COLLECT_ARR_SIZE%^RESET%^
.EI
.SP 7 5

This is the size of the array to allocate for the print shop information.
.EP

.SI 3
* %^BOLD%^COLLECT_AUTO_LOAD_NUM%^RESET%^
.EI
.SP 7 5

This is the index into the collection array for the number associated with
the book.  This the save file number, the data for the book is stored
in a different location to the book handler stuff itself to save
memory.
.EP

.SI 3
* %^BOLD%^COLLECT_LANGUAGE%^RESET%^
.EI
.SP 7 5

This is the index of the language which will be used in the printing of the
the book.
.EP

.SI 3
* %^BOLD%^COLLECT_NUM%^RESET%^
.EI
.SP 7 5

This is the index into the collection array for the number of copies
to print.
.EP

.SI 3
* %^BOLD%^COLLECT_OB_NAME%^RESET%^
.EI
.SP 7 5

This is the index of the string name of the object which was asked to
be printed.
.EP

.SI 3
* %^BOLD%^COLLECT_TIME%^RESET%^
.EI
.SP 7 5

This is the index into the collection array for the time at which it
ready to be picked up.
.EP

