.DT
newspaper.c
Disk World autodoc help
newspaper.c

.SH Description
.SP 5 5

This is a general newspaper edition handler.  It will deal with keeping
track of all the data for all the newspapers on the mud.
.EP
.SP 10 5


Written by Pinkfish

Started Sat Apr 28 02:30:49 PDT 2001

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/autodoc.h, /include/nroff.h, /include/login_handler.h and /include/room/newspaper.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^edition%^RESET%^
class edition {
int date_published;
class article * articles;
class advert * adverts;
string headline;
int num_sold;
int num_web_hits;
int total_revenue;
}

.EI

.SI 3
* %^BOLD%^paper%^RESET%^
class paper {
class edition * published_editions;
string dir_name;
string long;
string description;
string office;
int paper_cost;
string language;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_edition_paper_sold%^RESET%^
.EI
.SI 5
void add_edition_paper_sold(string paper, int edition, int cost)
.EI
.SP 7 5

This method adds a web paper sold to the specified edition.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
paper - the paper which was sold
.EP
.SP 9 5
edition - the edition which was sold
.EP
.SP 9 5
cost - the amount it was sold for

.EP

.SI 3
* %^BOLD%^add_edition_revenue%^RESET%^
.EI
.SI 5
void add_edition_revenue(string paper, int edition, int revenue)
.EI
.SP 7 5

This method adds to the total revenue for this edition.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
paper - the paper to look up
.EP
.SP 9 5
edition - the edition to look up
.EP
.SP 9 5
revenue - the revenue to add

.EP

.SI 3
* %^BOLD%^add_edition_web_hit%^RESET%^
.EI
.SI 5
void add_edition_web_hit(string paper, int edition)
.EI
.SP 7 5

This method adds a web hit to the specified edition.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
paper - the paper to add a hit on
.EP
.SP 9 5
edition - the edition to add a hit on

.EP

.SI 3
* %^BOLD%^add_newspaper_inform%^RESET%^
.EI
.SI 5
void add_newspaper_inform(string paper, string ob)
.EI
.SP 7 5

This method adds an object to be informed of a paper being published.
When the paper is published, the function event_publish_paper,
is called on all the objects in this list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
paper - the paper to be informed about
.EP
.SP 9 5
ob - the object to be informed

.EP

.SI 3
* %^BOLD%^add_paper%^RESET%^
.EI
.SI 5
void add_paper(string name, string long, string desc, string office, string language)
.EI
.SP 7 5

This method adds a paper.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
paper - the name of the paper
.EP
.SP 9 5
long - the description on the newspapers themselves
.EP
.SP 9 5
desc - the description on the web page
.EP
.SP 9 5
office - the path to the office

.EP

.SI 3
* %^BOLD%^add_paper_area%^RESET%^
.EI
.SI 5
void add_paper_area(string area)
.EI
.SP 7 5

This method adds a paper area.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to add

.EP

.SI 3
* %^BOLD%^add_paper_to_area%^RESET%^
.EI
.SI 5
void add_paper_to_area(string paper, string area)
.EI
.SP 7 5

This method adds a paper to a paper area.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
paper - the paper to add to the area
.EP
.SP 9 5
area - the area to add the paper to

.EP

.SI 3
* %^BOLD%^convert_html%^RESET%^
.EI
.SI 5
string convert_html(string input)
.EI
.SP 7 5

This method changes an input string into a 'nroff' equivilant output
string.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the string to convert

.EP

.SI 3
* %^BOLD%^is_paper%^RESET%^
.EI
.SI 5
int is_paper(string paper)
.EI
.SP 7 5

This method checks to see if the paper exists.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
paper - the paper name to check for

.EP

.SI 3
* %^BOLD%^is_paper_area%^RESET%^
.EI
.SI 5
int is_paper_area(string area)
.EI
.SP 7 5

This method checks to see if the area exists or not.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to check for

.EP

.SI 3
* %^BOLD%^is_valid_edition%^RESET%^
.EI
.SI 5
int is_valid_edition(string paper, int edition)
.EI
.SP 7 5

This checks to see if the specified edition is valid or not.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
paper - the paper to check
.EP
.SP 9 5
edition - the edition to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it ok, 0 if not

.EP

.SI 3
* %^BOLD%^publish_paper%^RESET%^
.EI
.SI 5
int publish_paper(string paper, string headline, class article * articles, string * text, class advert * adds, string * editors)
.EI
.SP 7 5

This method publishes a new edition of the specified paper.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
paper - the paper to publish an edition of
.EP
.SP 9 5
articles - the articles in the paper
.EP
.SP 9 5
text - the corresponding text to the articles
.EP
.SP 9 5
adds - the adverts in the paper
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if successful, 0 if not

.EP

.SI 3
* %^BOLD%^query_all_papers%^RESET%^
.EI
.SI 5
string * query_all_papers()
.EI
.SP 7 5

This method returns the list of all the papers.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
all the papers

.EP

.SI 3
* %^BOLD%^query_article_text%^RESET%^
.EI
.SI 5
string query_article_text(class article art)
.EI
.SP 7 5

This method reads in the data associated with the specified article.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
article - the article to read the text for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the article text

.EP

.SI 3
* %^BOLD%^query_article_text_html%^RESET%^
.EI
.SI 5
string query_article_text_html(class article art)
.EI
.SP 7 5

This method reads in the data associated with the specified article
in html.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
article - the article to read the text for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the article text

.EP

.SI 3
* %^BOLD%^query_article_text_plain%^RESET%^
.EI
.SI 5
string query_article_text_plain(class article art)
.EI
.SP 7 5

This method reads in the data associated with the specified article
in plain output.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
article - the article to read the text for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the article text

.EP

.SI 3
* %^BOLD%^query_current_edition_num%^RESET%^
.EI
.SI 5
int query_current_edition_num(string paper)
.EI
.SP 7 5

This method returns the current edition of the paper.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
paper - the paper to get the edition of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current edition

.EP

.SI 3
* %^BOLD%^query_edition_adverts%^RESET%^
.EI
.SI 5
class advert * query_edition_adverts(string paper, int edition)
.EI
.SP 7 5

This method returns the adverts in the specified edition of the
paper.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
paper - the paper to look up
.EP
.SP 9 5
edition - the edition to look up
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the adverts in the edition

.EP

.SI 3
* %^BOLD%^query_edition_articles%^RESET%^
.EI
.SI 5
class article * query_edition_articles(string paper, int edition)
.EI
.SP 7 5

This method returns the articles in the specified edition of the
paper.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
paper - the paper to look up
.EP
.SP 9 5
edition - the edition to look up
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the articles in the edition

.EP

.SI 3
* %^BOLD%^query_edition_date%^RESET%^
.EI
.SI 5
int query_edition_date(string paper, int edition)
.EI
.SP 7 5

This method returns the date at which the specified issue was
published.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
paper - the paper to check
.EP
.SP 9 5
edition - the edition to look up
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the date at which the edition was published

.EP

.SI 3
* %^BOLD%^query_edition_num_sold%^RESET%^
.EI
.SI 5
int query_edition_num_sold(string paper, int edition)
.EI
.SP 7 5

This method returns the number of papers which have been sold.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
paper - the paper which was sold
.EP
.SP 9 5
edition - the edition to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the number of papers that have been sold

.EP

.SI 3
* %^BOLD%^query_edition_num_web_hits%^RESET%^
.EI
.SI 5
int query_edition_num_web_hits(string paper, int edition)
.EI
.SP 7 5

This method returns the number of web hits for the editon.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
paper - the paper which was hit
.EP
.SP 9 5
edition - the edition to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the number of web hits on the edition

.EP

.SI 3
* %^BOLD%^query_edition_revenue%^RESET%^
.EI
.SI 5
int query_edition_revenue(string paper, int edition)
.EI
.SP 7 5

This method returns the total revenue for this edition.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
paper - the paper to look up
.EP
.SP 9 5
edition - the edition to look up
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the headline of the edition

.EP

.SI 3
* %^BOLD%^query_language_for%^RESET%^
.EI
.SI 5
string query_language_for(string paper)
.EI
.SP 7 5

This function returns the language for the given newspaper.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
paper - The name of the paper.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
The language for the paper, "general" for none or 0 if for error

.EP

.SI 3
* %^BOLD%^query_last_edition_num%^RESET%^
.EI
.SI 5
int query_last_edition_num(string paper)
.EI
.SP 7 5

This method returns the last edition of the paper.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
paper - the paper to get the edition of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the last edition, 0 if there are no editions

.EP

.SI 3
* %^BOLD%^query_newspaper_informs%^RESET%^
.EI
.SI 5
string * query_newspaper_informs(string paper)
.EI
.SP 7 5

This method returns the list of objects that want to be informed about
this paper.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
paper - the paper to be informed about
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of objects to inform

.EP

.SI 3
* %^BOLD%^query_paper_cost%^RESET%^
.EI
.SI 5
int query_paper_cost(string paper)
.EI
.SP 7 5

This method returns the cost at which the paper will be sold.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
paper - the paper to set the cost of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the cost at which the paper will be sold

.EP

.SI 3
* %^BOLD%^query_paper_description%^RESET%^
.EI
.SI 5
string query_paper_description(string paper)
.EI
.SP 7 5

This method returns the description description of the paper.  The long
description is used in the newspapers around the place as it's
description.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
paper - the paper to get the long for

.EP

.SI 3
* %^BOLD%^query_paper_headline%^RESET%^
.EI
.SI 5
string query_paper_headline(string paper, int edition)
.EI
.SP 7 5

This method returns the headline for the selected edition of the paper.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
paper - the paper to look up
.EP
.SP 9 5
edition - the edition to look up
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the headline of the edition

.EP

.SI 3
* %^BOLD%^query_paper_long%^RESET%^
.EI
.SI 5
string query_paper_long(string paper)
.EI
.SP 7 5

This method returns the long description of the paper.  The long
description is used in the newspapers around the place as it's
description.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
paper - the paper to get the long for

.EP

.SI 3
* %^BOLD%^query_paper_office%^RESET%^
.EI
.SI 5
string query_paper_office(string paper)
.EI
.SP 7 5

This method returns the office of the paper.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
paper - the paper to look up
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the office of the paper

.EP

.SI 3
* %^BOLD%^query_papers_in_area%^RESET%^
.EI
.SI 5
string * query_papers_in_area(string area)
.EI
.SP 7 5

This method returns all the articles in the specified paper area.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to lookup
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the papers in the area, returns a null array when there are no papers

.EP

.SI 3
* %^BOLD%^remove_newspaper_inform%^RESET%^
.EI
.SI 5
void remove_newspaper_inform(string paper, string ob)
.EI
.SP 7 5

This method removes an object to be informed of a paper being published.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
paper - the paper to no longer be informed about
.EP
.SP 9 5
ob - the object to no longer be informed

.EP

.SI 3
* %^BOLD%^set_article_text%^RESET%^
.EI
.SI 5
void set_article_text(class article art, string text)
.EI
.SP 7 5

This moves the data for the article into the correct location.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
article - the article to set the text for
.EP
.SP 9 5
text - the text to set

.EP

.SI 3
* %^BOLD%^set_language_for%^RESET%^
.EI
.SI 5
void set_language_for(string paper, string language)
.EI
.SP 7 5

This function sets the language for the given paper.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
paper - The name of the paper.
.EP
.SP 9 5
language - The language to use.

.EP

.SI 3
* %^BOLD%^set_paper_cost%^RESET%^
.EI
.SI 5
void set_paper_cost(string paper, int cost)
.EI
.SP 7 5

This method sets the cost at which the paper will be sold.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
paper - the paper to set the cost of
.EP
.SP 9 5
cost - the new cost of the paper

.EP

.SI 3
* %^BOLD%^set_paper_description%^RESET%^
.EI
.SI 5
void set_paper_description(string paper, string description)
.EI
.SP 7 5

This method sets the description of the paper.  The description is
what is shown on the web page for the paper.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
paper - the paper to set the long for
.EP
.SP 9 5
long - the new long description of the paper

.EP

.SI 3
* %^BOLD%^set_paper_headline%^RESET%^
.EI
.SI 5
void set_paper_headline(string paper, int edition, string headline)
.EI
.SP 7 5

This method returns the headline for the selected edition of the paper.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
paper - the paper to look up
.EP
.SP 9 5
edition - the edition to look up
.EP
.SP 9 5
headline - the headline of the issue

.EP

.SI 3
* %^BOLD%^set_paper_long%^RESET%^
.EI
.SI 5
void set_paper_long(string paper, string long)
.EI
.SP 7 5

This method sets the long of the paper.  The long description is
used in the newspapers around the place.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
paper - the paper to set the long for
.EP
.SP 9 5
long - the new long description of the paper

.EP

.SI 3
* %^BOLD%^set_paper_office%^RESET%^
.EI
.SI 5
void set_paper_office(string paper, string office)
.EI
.SP 7 5

This method sets the office of the paper.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
paper - the paper to look up
.EP
.SP 9 5
office - the new office of the paper

.EP


