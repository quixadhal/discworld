.DT
book_handler.c
Disk World autodoc help
book_handler.c

.SH Description
.SP 5 5

This handler object deals with all the books that have been published.
It keeps a save file of every book and a current book number.
It will allow for easy finding again of preprinted books and for the
easy removal of any books that happen to contain things like quest
solutions and stuff.

.EP
.SP 10 5


Written by Pinkfish

Started Sun Feb  1 20:24:33 CST 1998
.EP



.SH See also
.SP 5 5
/std/book.c and /std/print_shop.c

.EP
.SH Includes
.SP 5 5
This class includes the following files /include/language.h and /include/book_handler.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_banished_book%^RESET%^
.EI
.SI 5
int add_banished_book(int book_num, string reason)
.EI
.SP 7 5

This method banishes a book number for a certain reason.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
book_num - the book number to banish
.EP
.SP 9 5
reason - the reason it was banished
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success and 0 on failure.
.EP

.SI 3
* %^BOLD%^add_book%^RESET%^
.EI
.SI 5
int add_book(object book, int num, string pl_name)
.EI
.SP 7 5

Adds a newly minted book into our nice list of stuff.  This returns the
book number for this book.  You can use this to add a book if you already
know its book number, if it does not match to the saved book number then
it will return a new number.  If it does match it will return the
same number.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
book - the book to add
.EP
.SP 9 5
num - the possible book number it already has
.EP

.SI 3
* %^BOLD%^check_book_same%^RESET%^
.EI
.SI 5
int check_book_same(object book, int num)
.EI
.SP 7 5

Checks to see if the passed in book is the same as the current book
number that is set on it.  We only look at the text and ignore the
type and language with which it is written.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
book - the book to check
.EP
.SP 9 5
num - the book number to check against
.EP

.SI 3
* %^BOLD%^compare_page%^RESET%^
.EI
.SI 5
int compare_page(string page1, string page2)
.EI
.SP 7 5

Compares the open pages of two books to see if the text is the
same.  It ignores language and style of writing.  This would also
work on two pieces of paper.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
page1 - the first book
.EP
.SP 9 5
page2 - the second book
.EP

.SI 3
* %^BOLD%^compare_pages%^RESET%^
.EI
.SI 5
int compare_pages(string * pages1, string * pages2)
.EI
.SP 7 5

This method will try and figure out the percentage changed between the
two page arrays.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pages1 - the first page array
.EP
.SP 9 5
pages2 - the second page array
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the percentage the same

.EP

.SI 3
* %^BOLD%^load_it%^RESET%^
.EI
.SI 5
void load_it()
.EI
.SP 7 5

Loads the state information from the save file.
.EP

.SI 3
* %^BOLD%^query_all_banished_books%^RESET%^
.EI
.SI 5
mapping query_all_banished_books()
.EI
.SP 7 5

This method returns all the banished books.  The keys are the
numbers which are banished and the values are the reason for
the banishment.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the banished books mapping
.EP

.SI 3
* %^BOLD%^query_banished_book%^RESET%^
.EI
.SI 5
string query_banished_book(int book_num)
.EI
.SP 7 5

This method returns the banish string for the book.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
book_num - the book number to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the banish reason, or 0 if none
.EP

.SI 3
* %^BOLD%^query_book_exists%^RESET%^
.EI
.SI 5
int query_book_exists(int book_num)
.EI
.SP 7 5

This tests to see if the given book number exists.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
book_num - the number to test.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it exists, 0 if it does not.

.EP

.SI 3
* %^BOLD%^query_book_owner%^RESET%^
.EI
.SI 5
string query_book_owner(int book_num)
.EI
.SP 7 5

This method returns the owner associated with the book of a given
number.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
book_num - the book number to get the pages for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the owner of the book
.EP

.SI 3
* %^BOLD%^query_book_pages%^RESET%^
.EI
.SI 5
string * query_book_pages(int book_num)
.EI
.SP 7 5

This method returns the pages associated with the book of a given
number.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
book_num - the book number to get the pages for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the pages for the book
.EP

.SI 3
* %^BOLD%^query_pages%^RESET%^
.EI
.SI 5
string * query_pages(object book)
.EI
.SP 7 5

This method returns the array of useful pages in a book.  It removes
torn out pages and pages which are added by specific things in the
game, like the print shop adding a fly leaf.  It just returns the text
for the pages, since this is what we are interested in.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
book - the book to get the pages from
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of pages

.EP

.SI 3
* %^BOLD%^save_it%^RESET%^
.EI
.SI 5
void save_it()
.EI
.SP 7 5

Saves the current state information to the save file.
.EP


