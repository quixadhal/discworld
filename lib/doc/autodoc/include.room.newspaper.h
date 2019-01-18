.DT
newspaper.h
Disk World autodoc help
newspaper.h

.SH Description
.SP 5 5

The include file support stuff for newspaper offices and the 
newspaper controller.
.EP
.SP 10 5


Written by Pinkfish

Started Fri Apr 27 17:19:23 PDT 2001

.EP

.SH Classes

.SI 3
* %^BOLD%^advert%^RESET%^
class advert {
string text;
string author;
string category;
int date_written;
int value;
}

.EI
.SP 7 5

Information about an advert to be published in a newspaper.
.EP

.SI 3
* %^BOLD%^article%^RESET%^
class article {
string file_name;
string title;
string author;
int date_written;
int type;
string shown_name;
string colour;
}

.EI
.SP 7 5

Information about an article to be published in a newspaper.
.EP


.SH Defines
.SI 3
* %^BOLD%^NEWSPAPER_ARTICLE_ANONYMOUS_FLAG%^RESET%^
.EI
.SP 7 5

This means the article is anonymous.

.EP

.SI 3
* %^BOLD%^NEWSPAPER_ARTICLE_NO_XP_FLAG%^RESET%^
.EI
.SP 7 5

This means the article has no xp given for it.

.EP

.SI 3
* %^BOLD%^NEWSPAPER_ARTICLE_TYPE_ADVERTS%^RESET%^
.EI
.SP 7 5

This means the article is the advert insert point.

.EP

.SI 3
* %^BOLD%^NEWSPAPER_ARTICLE_TYPE_HTML%^RESET%^
.EI
.SP 7 5

This means the article is html.

.EP

.SI 3
* %^BOLD%^NEWSPAPER_ARTICLE_TYPE_MASK%^RESET%^
.EI
.SP 7 5

This is the mask to get the nroff type stuff.

.EP

.SI 3
* %^BOLD%^NEWSPAPER_ARTICLE_TYPE_NROFF%^RESET%^
.EI
.SP 7 5

This means the article is nroff.

.EP

.SI 3
* %^BOLD%^NEWSPAPER_ARTICLE_TYPE_PLAIN%^RESET%^
.EI
.SP 7 5

This means the article is plain text type.

.EP

.SI 3
* %^BOLD%^NEWSPAPER_ARTICLE_TYPE_SECTION%^RESET%^
.EI
.SP 7 5

This means the article is a section header.

.EP

.SI 3
* %^BOLD%^NEWSPAPER_HANDLER%^RESET%^
.EI
.SP 7 5

The location of the newspaper handler.

.EP

.SI 3
* %^BOLD%^NEWSPAPER_OB%^RESET%^
.EI
.SP 7 5

The location of the newspaper object itself.

.EP

.SI 3
* %^BOLD%^NEWSPAPER_PRESS_CREDITIALS%^RESET%^
.EI
.SP 7 5

The location of the press creditials file.

.EP

.SI 3
* %^BOLD%^NEWSPAPER_SUBSCRIPTION_OB%^RESET%^
.EI
.SP 7 5

The location of the housing subscription box.

.EP

.SI 3
* %^BOLD%^NEWSPAPER_WEB_HIT_REVENUE%^RESET%^
.EI
.SP 7 5

The revenue for each hit on the web page. (10 pence).

.EP

.SI 3
* %^BOLD%^NEWSPAPER_WWW_LOGO_BASE%^RESET%^
.EI
.SP 7 5

This is the start bit of the file name used for the file name of the
newspaper logo images.

.EP

.SI 3
* %^BOLD%^NEWSPAPER_XP_EDITOR%^RESET%^
.EI
.SP 7 5

This is the amount of xp an editor gets as a base for publishing an
issue.  They also get 10% of the total reporter xp.

.EP

.SI 3
* %^BOLD%^NEWSPAPER_XP_PER_ARTICLE%^RESET%^
.EI
.SP 7 5

This is the amount of xp a reporter gets for making an article.

.EP

