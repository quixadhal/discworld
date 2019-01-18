.DT
newspaper_office.c
Disk World autodoc help
newspaper_office.c

.SH Description
.SP 5 5

An office from which a player run newspaper can be run.
.EP
.SP 10 5


Written by Obilix
.EP

.SH Change history
.SP 5 5
Pinkfish Fri Apr 27 12:08:29 PDT 2001
Turned into an inherit and made to use a newspaper handler for
distributing the actual text.

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/board.h, /include/nroff.h, /include/mail.h, /include/player_handler.h, /include/housing.h, /include/move_failures.h, /include/shops/bank.h, /include/room/newspaper.h and /include/money.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^category_data%^RESET%^
class category_data {
int cost_per_add;
int cost_per_ten_chars;
int open;
}

.EI

.SI 3
* %^BOLD%^our_article%^RESET%^
class our_article {
class article art;
int flags;
int length;
int suggested_payment;
}

.EI

.SI 3
* %^BOLD%^payee_data%^RESET%^
class payee_data {
int paid;
string message;
}

.EI

.SI 3
* %^BOLD%^reporter%^RESET%^
class reporter {
int date_added;
int num_articles;
int total_payed;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_editor%^RESET%^
.EI
.SI 5
void add_editor(string word)
.EI
.SP 7 5

This method adds a new editor to the paper.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the new editor to add

.EP

.SI 3
* %^BOLD%^add_reporter%^RESET%^
.EI
.SI 5
void add_reporter(string word)
.EI
.SP 7 5

This method adds a new reporter to the paper.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the new reporter to add

.EP

.SI 3
* %^BOLD%^add_setter%^RESET%^
.EI
.SI 5
void add_setter(string word)
.EI
.SP 7 5

This method adds a new setter to the paper.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the new setter to add

.EP

.SI 3
* %^BOLD%^add_sponsor%^RESET%^
.EI
.SI 5
void add_sponsor(string word)
.EI
.SP 7 5

This method adds a new sponsor to the paper.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the new sponsor to add

.EP

.SI 3
* %^BOLD%^board_access_check%^RESET%^
.EI
.SI 5
int board_access_check(int type, string board, object previous, string name)
.EI
.SP 7 5

This method is here to control access to a board if one is requested.
It allows only reporters to be able to see or write to it and it
allows editors to be able to eat messages from it.

.EP

.SI 3
* %^BOLD%^delete_article%^RESET%^
.EI
.SI 5
void delete_article(class our_article article)
.EI
.SP 7 5

This method deletes any bitsof the article off the disk.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
article - the article to delete

.EP

.SI 3
* %^BOLD%^do_advert_list%^RESET%^
.EI
.SI 5
int do_advert_list()
.EI

.SI 3
* %^BOLD%^do_category_add%^RESET%^
.EI
.SI 5
int do_category_add(string category)
.EI

.SI 3
* %^BOLD%^do_category_cost%^RESET%^
.EI
.SI 5
int do_category_cost(string category, string cost)
.EI

.SI 3
* %^BOLD%^do_category_cost_per_ten_chars%^RESET%^
.EI
.SI 5
int do_category_cost_per_ten_chars(string category, string cost)
.EI

.SI 3
* %^BOLD%^do_category_list%^RESET%^
.EI
.SI 5
int do_category_list()
.EI

.SI 3
* %^BOLD%^do_category_open%^RESET%^
.EI
.SI 5
int do_category_open(string category, string open)
.EI

.SI 3
* %^BOLD%^do_category_remove%^RESET%^
.EI
.SI 5
int do_category_remove(string category)
.EI

.SI 3
* %^BOLD%^do_everyone_status%^RESET%^
.EI
.SI 5
int do_everyone_status()
.EI
.SP 7 5

This is the status that everyone can see, not just the employees.

.EP

.SI 3
* %^BOLD%^do_lock%^RESET%^
.EI
.SI 5
int do_lock(int locked)
.EI

.SI 3
* %^BOLD%^do_submit_add%^RESET%^
.EI
.SI 5
int do_submit_add(string category)
.EI

.SI 3
* %^BOLD%^is_editor%^RESET%^
.EI
.SI 5
int is_editor(string word)
.EI
.SP 7 5

This method checks to see if the specified person is an editor for
the paper or not.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the person to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they are a editor

.EP

.SI 3
* %^BOLD%^is_locked%^RESET%^
.EI
.SI 5
int is_locked(class article art)
.EI
.SP 7 5

This method checks to see if the specified article is locked
or not.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
art - the article to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is locked, 0 if not

.EP

.SI 3
* %^BOLD%^is_owner%^RESET%^
.EI
.SI 5
int is_owner(string word)
.EI
.SP 7 5

This method checks to see if the specified person is an owner of
the paper or not.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the person to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they are a owner

.EP

.SI 3
* %^BOLD%^is_postponed%^RESET%^
.EI
.SI 5
int is_postponed(class article art)
.EI
.SP 7 5

This method checks to see if the specified article is postponed
or not.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
art - the article to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is postponed, 0 if not

.EP

.SI 3
* %^BOLD%^is_reporter%^RESET%^
.EI
.SI 5
int is_reporter(string word)
.EI
.SP 7 5

This method checks to see if the specified person is an reporter for
the paper or not.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the person to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they are a reporter

.EP

.SI 3
* %^BOLD%^is_setter%^RESET%^
.EI
.SI 5
int is_setter(string word)
.EI
.SP 7 5

This method checks to see if the specified person is an setter for
the paper or not.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the person to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they are a setter

.EP

.SI 3
* %^BOLD%^is_sponsor%^RESET%^
.EI
.SI 5
int is_sponsor(string word)
.EI
.SP 7 5

This method checks to see if the specified person is a sponsor for
the paper or not.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the person to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they are a sponsor

.EP

.SI 3
* %^BOLD%^load_article_text%^RESET%^
.EI
.SI 5
string load_article_text(class our_article art)
.EI
.SP 7 5

This method loads the text of an article.

.EP

.SI 3
* %^BOLD%^load_article_text_by_file%^RESET%^
.EI
.SI 5
string load_article_text_by_file(string fname)
.EI
.SP 7 5

This method is used by the web, it loads the article by the file name.

.EP

.SI 3
* %^BOLD%^move_article%^RESET%^
.EI
.SI 5
int move_article(int index, string up_down)
.EI
.SP 7 5

This method moves an article up or down in the current article list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
index - the index of the article to move
.EP
.SP 9 5
up_down - move it up or down

.EP

.SI 3
* %^BOLD%^ownership_change%^RESET%^
.EI
.SI 5
void ownership_change(string old_owner, string new_owner)
.EI
.SP 7 5

This method is called by the housing system when the ownership changes.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
old_owner - the old owner
.EP
.SP 9 5
new_owner - the newowner

.EP

.SI 3
* %^BOLD%^query_ads%^RESET%^
.EI
.SI 5
class advert * query_ads()
.EI

.SI 3
* %^BOLD%^query_all_articles%^RESET%^
.EI
.SI 5
class article * query_all_articles()
.EI
.SP 7 5

This method returns the 'article' class bits of the articles.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
all the articles.\

.EP

.SI 3
* %^BOLD%^query_all_editors%^RESET%^
.EI
.SI 5
string * query_all_editors()
.EI
.SP 7 5

This method returns all the current editors of the magazine.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
all the current editors

.EP

.SI 3
* %^BOLD%^query_all_payees%^RESET%^
.EI
.SI 5
mapping query_all_payees()
.EI
.SP 7 5

This method returns all the current payees of the magazine.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
who is to be payed and how much

.EP

.SI 3
* %^BOLD%^query_all_reporters%^RESET%^
.EI
.SI 5
string * query_all_reporters()
.EI
.SP 7 5

This method returns all the current reporters of the magazine.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
all the current reporters

.EP

.SI 3
* %^BOLD%^query_all_setters%^RESET%^
.EI
.SI 5
string * query_all_setters()
.EI
.SP 7 5

This method returns all the current setters of the magazine.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
all the current setters

.EP

.SI 3
* %^BOLD%^query_all_sponsors%^RESET%^
.EI
.SI 5
string * query_all_sponsors()
.EI
.SP 7 5

This method returns all the current sponsors of the magazine.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
all the current sponsors

.EP

.SI 3
* %^BOLD%^query_article%^RESET%^
.EI
.SI 5
class our_article query_article(string match)
.EI
.SP 7 5

This method finds an article based on a string.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the article

.EP

.SI 3
* %^BOLD%^query_article_index%^RESET%^
.EI
.SI 5
int query_article_index(string match)
.EI
.SP 7 5

This method returns the index of the article in the article array.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the article index

.EP

.SI 3
* %^BOLD%^query_article_log_file_name%^RESET%^
.EI
.SI 5
string query_article_log_file_name()
.EI
.SP 7 5

The file name of the log file for article events.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the article log file name

.EP

.SI 3
* %^BOLD%^query_articles%^RESET%^
.EI
.SI 5
class our_article * query_articles()
.EI

.SI 3
* %^BOLD%^query_bank_branch%^RESET%^
.EI
.SI 5
string query_bank_branch()
.EI
.SP 7 5

This method returns the bank branch to use for depositing money for
editions.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the bank branch to use

.EP

.SI 3
* %^BOLD%^query_category%^RESET%^
.EI
.SI 5
string query_category(string category)
.EI
.SP 7 5

This method tries to find a category based on some fuzzy matching.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
category - the category to lookup
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the fixed up category name

.EP

.SI 3
* %^BOLD%^query_current_edition%^RESET%^
.EI
.SI 5
int query_current_edition()
.EI
.SP 7 5

This method returns the current edition of the newspaper./
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current edition of the newspaper

.EP

.SI 3
* %^BOLD%^query_log_file_name%^RESET%^
.EI
.SI 5
string query_log_file_name()
.EI
.SP 7 5

The file name of the log file for events.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the log file name

.EP

.SI 3
* %^BOLD%^query_next_article_file_name%^RESET%^
.EI
.SI 5
string query_next_article_file_name()
.EI
.SP 7 5

This returns the next number to use for an article.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the next article number

.EP

.SI 3
* %^BOLD%^query_owner%^RESET%^
.EI
.SI 5
string query_owner()
.EI
.SP 7 5

This method returns the current owner of the paper.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the owner of the paper

.EP

.SI 3
* %^BOLD%^query_paper_name%^RESET%^
.EI
.SI 5
string query_paper_name()
.EI
.SP 7 5

This method returns the name of the paper.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the name of the paper

.EP

.SI 3
* %^BOLD%^query_proxy%^RESET%^
.EI
.SI 5
string query_proxy()
.EI
.SP 7 5

This method returns the proxy for this room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the proxy for the room

.EP

.SI 3
* %^BOLD%^query_the_paper_name%^RESET%^
.EI
.SI 5
string query_the_paper_name()
.EI
.SP 7 5

This method returns the name of the paper with a 'the' in front.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the name of the paper

.EP

.SI 3
* %^BOLD%^query_total_profit%^RESET%^
.EI
.SI 5
int query_total_profit(int num_articles)
.EI
.SP 7 5

This method figures out the total profit from the paper.

.EP

.SI 3
* %^BOLD%^remove_editor%^RESET%^
.EI
.SI 5
void remove_editor(string word)
.EI
.SP 7 5

This method removes a editor from the paper.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the editor to remove

.EP

.SI 3
* %^BOLD%^remove_reporter%^RESET%^
.EI
.SI 5
void remove_reporter(string word)
.EI
.SP 7 5

This method removes a reporter from the paper.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the reporter to remove

.EP

.SI 3
* %^BOLD%^remove_setter%^RESET%^
.EI
.SI 5
void remove_setter(string word)
.EI
.SP 7 5

This method removes a setter from the paper.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the setter to remove

.EP

.SI 3
* %^BOLD%^remove_sponsor%^RESET%^
.EI
.SI 5
void remove_sponsor(string word)
.EI

.SI 3
* %^BOLD%^save_article_text%^RESET%^
.EI
.SI 5
void save_article_text(class our_article art, string text)
.EI
.SP 7 5

This method saves an article.

.EP

.SI 3
* %^BOLD%^save_article_text_by_file%^RESET%^
.EI
.SI 5
void save_article_text_by_file(string fname, string text)
.EI
.SP 7 5

This method is used by the web, it loads the article by the file name.

.EP

.SI 3
* %^BOLD%^set_article_title_by_file%^RESET%^
.EI
.SI 5
void set_article_title_by_file(string fname, string title)
.EI
.SP 7 5

This method is used by the web, it loads the article by the file name.

.EP

.SI 3
* %^BOLD%^set_bank_branch%^RESET%^
.EI
.SI 5
void set_bank_branch(string bank)
.EI
.SP 7 5

This method sets the bank branch to use for depositing money when
editions are made.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
bank - the bank branch to use

.EP

.SI 3
* %^BOLD%^set_owner%^RESET%^
.EI
.SI 5
void set_owner(string person)
.EI
.SP 7 5

This method sets the current owner of the paper.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the new owner

.EP

.SI 3
* %^BOLD%^set_paper_name%^RESET%^
.EI
.SI 5
string set_paper_name(string name)
.EI
.SP 7 5

This method sets the name of the paper.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the new name of the paper

.EP

.SI 3
* %^BOLD%^set_proxy%^RESET%^
.EI
.SI 5
void set_proxy(string proxy)
.EI
.SP 7 5

This method sets up a proxy for the room.  A proxy means all the commands
are mirrored to the real location, so save files do not get messed up.
You set this if you have any extra rooms you want to be able to control
the paper.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
proxy - the proxy to set

.EP

.SI 3
* %^BOLD%^set_save_directory%^RESET%^
.EI
.SI 5
void set_save_directory(string dir)
.EI
.SP 7 5

This method sets the directory we are to use for all the
temporary files and data.  The actual pubished data will be stored
by the newspaper handler.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to put stuff in

.EP

.SI 3
* %^BOLD%^update_commands%^RESET%^
.EI
.SI 5
void update_commands(string word)
.EI
.SP 7 5

This method moves the player in and out of the room to fix up their
commands.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the player to move

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^load_me%^RESET%^
.EI
.SI 5
void load_me()
.EI
.SP 7 5

Loads the data in the file.

.EP

.SI 3
* %^BOLD%^save_me%^RESET%^
.EI
.SI 5
void save_me()
.EI
.SP 7 5

Saves the data in the file.

.EP


