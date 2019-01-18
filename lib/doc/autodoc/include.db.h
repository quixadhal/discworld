.DT
db.h
Disk World autodoc help
db.h

.SH Description
.SP 5 5

The header file for the database package.  Include this if your
object is going to make SQL queries to the database.

.EP
.SP 10 5

.EP

.SH Defines
.SI 3
* %^BOLD%^DB_ERROR_BAD_SOCKET%^RESET%^
.EI
.SP 7 5

THis is the error value if a socket was unable to be opened for
some reason.

.EP

.SI 3
* %^BOLD%^db_escape(query)%^RESET%^
.EI
.SP 7 5

Use this macro to properly escape text that you wish to use in an
SQL query.

.EP

.SI 3
* %^BOLD%^DB_HANDLER%^RESET%^
.EI
.SP 7 5

This is the handler to the database object.

.EP

.SI 3
* %^BOLD%^DB_SUCCESS%^RESET%^
.EI
.SP 7 5

This should be the value returned if we have success.

.EP

