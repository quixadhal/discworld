.DT
book.c
Disk World autodoc help
book.c

.SH Description
.SP 5 5

A nice book object.  A book will be a collection of pages.
You can tear pages out of a book, you can add pages (though adding
a page isnt overly useful, unless you bind it in of course).


Each page is an object. Pages that are torn out are handled as empty spots in the array. 


.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/object.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player.h, /include/move_failures.h and /include/playtesters.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_page_after%^RESET%^
.EI
.SI 5
int add_page_after(object page, int after)
.EI
.SP 7 5

This method adds a new page in after the selected page.  The pages
are numbered from 1, so adding a page after page 0 will place a
page on the start and after the last page onto the end.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
page - the page to add
.EP
.SP 9 5
after - the page to add it after
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^create_default_page_object%^RESET%^
.EI
.SI 5
object create_default_page_object()
.EI
.SP 7 5

This method creates a new default page object for this book.  THis
can be used for whatever nefarious purposes you want.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a new default page object

.EP

.SI 3
* %^BOLD%^is_current_page_torn_out%^RESET%^
.EI
.SI 5
int is_current_page_torn_out()
.EI
.SP 7 5

This method checks to see if the current page is torn out.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if torn out, 0 if not
.EP

.SI 3
* %^BOLD%^is_default_page%^RESET%^
.EI
.SI 5
int is_default_page(int num)
.EI
.SP 7 5

This method checks to see if the page is still the default page object
or if it is something else altogether.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is the default page object, 0 if not

.EP

.SI 3
* %^BOLD%^is_page_torn_out%^RESET%^
.EI
.SI 5
int is_page_torn_out(int page)
.EI
.SP 7 5

This method checks to see if the specified page is torn out.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
page - the page number to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is torn out, - if not
.EP

.SI 3
* %^BOLD%^make_page_blank%^RESET%^
.EI
.SI 5
int make_page_blank(int num)
.EI
.SP 7 5

This method makes the selected page blank.  It defaults it back to a
default page object as if the book had just been created.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
num - the page to make blank
.EP

.SI 3
* %^BOLD%^query_book%^RESET%^
.EI
.SI 5
int query_book()
.EI
.SP 7 5

Tells us if this is a book object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
always returns 1

.EP

.SI 3
* %^BOLD%^query_book_num%^RESET%^
.EI
.SI 5
int query_book_num()
.EI
.SP 7 5

This method returns the current number of the book.  This is assigned
and used in conjunction with the book handler.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current book number

.EP

.SI 3
* %^BOLD%^query_current_page%^RESET%^
.EI
.SI 5
object query_current_page()
.EI
.SP 7 5

Returns the object associated with the current open page.  If the
page does not actualy exist yet then the default base object
is returned.  This object should be handled carefully...  If the
page is torn out then the next readable page is returned or
0 is returned.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current page object
.EP

.SI 3
* %^BOLD%^query_current_page_clone%^RESET%^
.EI
.SI 5
object query_current_page_clone()
.EI
.SP 7 5

This function makes sure the page actually
exists.  If it does not exist, then it will clone one
up for us.  This one should be used as you will not end up with the
base object in this case.  If you are planning to modify the page,
then use this call.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the object of the current page

.EP

.SI 3
* %^BOLD%^query_default_page_object%^RESET%^
.EI
.SI 5
string query_default_page_object()
.EI
.SP 7 5

This method returns the default page object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the default page object (as a string)

.EP

.SI 3
* %^BOLD%^query_ignore_saved_pages%^RESET%^
.EI
.SI 5
int query_ignore_saved_pages()
.EI
.SP 7 5

This method returns the current setting of the ignore saved
pages variable.  If this is true then
the pages form the players auto_load stuff will be ignored totaly
and only the page creates in setup would be
used.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current ignore saved page attribute

.EP

.SI 3
* %^BOLD%^query_num_pages%^RESET%^
.EI
.SI 5
int query_num_pages()
.EI
.SP 7 5

Queries the number of pages in the book.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the number of pages in the book
.EP

.SI 3
* %^BOLD%^query_open_page%^RESET%^
.EI
.SI 5
int query_open_page()
.EI
.SP 7 5

What is the current open page.  Returns 0 if the book is not
open.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current open page
.EP

.SI 3
* %^BOLD%^query_pages%^RESET%^
.EI
.SI 5
mixed * query_pages()
.EI
.SP 7 5

Returns the pages array.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array containing the pages information

.EP

.SI 3
* %^BOLD%^query_player%^RESET%^
.EI
.SI 5
object query_player()
.EI
.SP 7 5

This method will return  the player associated with the auto loading
process.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the player in use by the autoloading
.EP

.SI 3
* %^BOLD%^query_selected_page%^RESET%^
.EI
.SI 5
object query_selected_page(int num)
.EI
.SP 7 5

This method returns the contents of the selected page.  The number
must be greator than 0.  This will return the exact value of the
page, it will not search for non-torn out pages.  Becare with
the return value of this as the default page object might be
returned if the page does not actually exist.  If you need a
real object remember to use the clone version.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
num - the page to return
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure or if the page is torn out, the object on success

.EP

.SI 3
* %^BOLD%^query_selected_page_clone%^RESET%^
.EI
.SI 5
object query_selected_page_clone(int num)
.EI
.SP 7 5

This method returns the contents of the selected page, if a default
object is returned a real clone for the page is created instead.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
num - the page to return
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure or if the page is torn out, the object on success

.EP

.SI 3
* %^BOLD%^replace_page_with%^RESET%^
.EI
.SI 5
int replace_page_with(object page, int num)
.EI
.SP 7 5

This method replaces the selected page with a new page.  The old page
is dested if it can be and replaced with a nice shiny new page object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
page - the page to replace with
.EP
.SP 9 5
num - the page number to replace
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^set_book_num%^RESET%^
.EI
.SI 5
void set_book_num(int num)
.EI
.SP 7 5

This method sets the current number of the book.  This is assigned
and used in conjunction with the book handler.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
num - the new book number

.EP

.SI 3
* %^BOLD%^set_default_page_object%^RESET%^
.EI
.SI 5
void set_default_page_object(string obj)
.EI
.SP 7 5

Sets the object to use as the default page.  If this is not set the
the object /obj/misc/paper.c will be used.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object to use as the default page

.EP

.SI 3
* %^BOLD%^set_no_pages%^RESET%^
.EI
.SI 5
void set_no_pages(int no)
.EI
.SP 7 5

Set the number of pages in the book.  If there are too many pages
in the book, then pages are removed from the top to create the
correct size and if there are too few pages then pages are
added onto the end of the pages array.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
no - the number of pages
.EP

.SI 3
* %^BOLD%^set_open_page%^RESET%^
.EI
.SI 5
void set_open_page(int i)
.EI
.SP 7 5

Sets the currently open page.  This does all the magic needed to make
the book appear as if it is the currently opened page.  If the open
page is 0 then the book is closed.  If it is out of the upper
bound then the book is opened to the last page.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the page to open to
.EP

.SI 3
* %^BOLD%^set_player%^RESET%^
.EI
.SI 5
void set_player(object player)
.EI
.SP 7 5

This method is called by the auto loading process to set the current
player on this object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the player to set
.EP

.SI 3
* %^BOLD%^tear_current_page_out%^RESET%^
.EI
.SI 5
object tear_current_page_out(object dest)
.EI
.SP 7 5

This method tears the current page out of the book and returns it
to us.  This object will be moved into the destination so that it is
no longer inside us.  If it cannot be moved of the page has already
been remove then the function will return 0.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dest - the destination to move the page to
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the torn out page, or 0 on failure

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^set_ignore_saved_pages%^RESET%^
.EI
.SI 5
void set_ignore_saved_pages(int saved)
.EI
.SP 7 5

This method sets the current setting of the ignore saved
pages variable.  If this is true then
the pages form the players auto_load stuff will be ignored totaly
and only the page creates in setup would be
used.  This can only be called from an object
which inherits this one (hence the protected method).
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
saved - the new value of the saved attribute
.EP


