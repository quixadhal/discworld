.DT
mysql.c
Disk World autodoc help
mysql.c

.SH Description
.SP 5 5

This object handles all the mysql requests for the system.
It connects to an external python script todo the actual mysql processing,
it is done this way to make the sql requests non-blocking so they do not
slow down the mud.
.EP
.SP 10 5


Written by Pinkfish

Started Tue Jun 20 14:29:01 PDT 2000

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /net/inherit/client.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/db.h, /include/network.h and /include/socket_errors.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^make_sql_request%^RESET%^
.EI
.SI 5
void make_sql_request(string db, string user, string pass, string request, function finish)
.EI
.SP 7 5

This is the method you call to make an sql request.  You pass in the
database, user and password you wish to use to connect to the
sql sever.  You also pass in the request you wish to make and the call
back function to call.


The call back function will be passed two arguements, they are a 'type' and a 'data' arguement. The type will always be an integer and it registers the success or failure of the database query. The data will either be a string (in the case of an error) or it will be teh returned data (in the case of request). The returned data will be an array of mappings, the mappings contain keys of the field type and the data value being the returned data. 

The format of the function call is:
.EP
.SP 7 5
void return_function(int status, mapping data)
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
db - the database to connect to
.EP
.SP 9 5
user - the user to use
.EP
.SP 9 5
pass - the password to use
.EP
.SP 9 5
request - the request to make
.EP
.SP 9 5
finish - the call back function

.EP


