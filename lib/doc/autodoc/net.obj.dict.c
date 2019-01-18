.DT
dict.c
Disk World autodoc help
dict.c

.SH Description
.SP 5 5

MUD based dictionary, conforms to the DICT new protocol found in RFC 2229. 
See ftp://ftp.isi.edu/in-notes/rfc2229.txt for more information.
.EP
.SP 10 5


Written by Taffyd@Discworld

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
* %^BOLD%^connection_data%^RESET%^
class connection_data {
int fd;
int dns_key;
string current_word;
string message;
mixed definitions;
mixed * working;
int status;
int action;
int count;
string search_mode;
}

.EI
.SP 7 5

This class handles connection details for each person using
the dictionary. 
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
fd - the file descriptor of the socket currently being
used by the dictionary.
member dns_key the resolve() key used when doing a nslookup.

.EP


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^calculate_short%^RESET%^
.EI
.SI 5
string calculate_short()
.EI

.SI 3
* %^BOLD%^close_socket%^RESET%^
.EI
.SI 5
void close_socket(int fd, object who)
.EI

.SI 3
* %^BOLD%^do_close%^RESET%^
.EI
.SI 5
int do_close()
.EI

.SI 3
* %^BOLD%^do_define%^RESET%^
.EI
.SI 5
int do_define(mixed * args)
.EI

.SI 3
* %^BOLD%^do_open%^RESET%^
.EI
.SI 5
int do_open()
.EI

.SI 3
* %^BOLD%^do_spell%^RESET%^
.EI
.SI 5
int do_spell(mixed * args)
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


