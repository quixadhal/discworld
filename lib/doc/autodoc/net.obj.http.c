.DT
http.c
Disk World autodoc help
http.c

.SH Description
.SP 5 5

A simple HTTP/1.0 complaint browser for retrieving web pages.
.EP
.SP 10 5


Written by Taffyd

Started 13/11/2001

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/object and /net/inherit/client.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/network.h and /include/socket_errors.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^http_connection%^RESET%^
class http_connection {
object owner;
string address;
string ip_address;
string path;
int port;
string data;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^close_socket%^RESET%^
.EI
.SI 5
void close_socket(int fd)
.EI

.SI 3
* %^BOLD%^do_http_get%^RESET%^
.EI
.SI 5
int do_http_get(string url)
.EI

.SI 3
* %^BOLD%^eventRead%^RESET%^
.EI
.SI 5
void eventRead(int fd, string message)
.EI

.SI 3
* %^BOLD%^got_ip_address%^RESET%^
.EI
.SI 5
void got_ip_address(string address, string ip_address, int key)
.EI

.SI 3
* %^BOLD%^query_prevent_shadow%^RESET%^
.EI
.SI 5
int query_prevent_shadow()
.EI


