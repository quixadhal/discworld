.DT
dictionary.c
Disk World autodoc help
dictionary.c

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
This class inherits the following classes /net/inherit/client.
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
string message;
mixed definitions;
mixed * working;
string ip_address;
int status;
int count;
string search_mode;
class request current_request;
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

.SI 3
* %^BOLD%^request%^RESET%^
class request {
string word;
string search_mode;
int type;
int i;
function callback;
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
* %^BOLD%^add_request%^RESET%^
.EI
.SI 5
void add_request(class request bing)
.EI

.SI 3
* %^BOLD%^check_send%^RESET%^
.EI
.SI 5
void check_send()
.EI

.SI 3
* %^BOLD%^close_connection%^RESET%^
.EI
.SI 5
void close_connection()
.EI

.SI 3
* %^BOLD%^close_socket%^RESET%^
.EI
.SI 5
void close_socket(int fd, object who)
.EI

.SI 3
* %^BOLD%^define_word%^RESET%^
.EI
.SI 5
int define_word(string word, int all, function call_back)
.EI

.SI 3
* %^BOLD%^do_callback%^RESET%^
.EI
.SI 5
void do_callback(function callb, string word, mixed result, mixed * data)
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
* %^BOLD%^inform_of_problem%^RESET%^
.EI
.SI 5
void inform_of_problem(class request bing, string mess)
.EI

.SI 3
* %^BOLD%^make_connection%^RESET%^
.EI
.SI 5
void make_connection()
.EI

.SI 3
* %^BOLD%^remove_request%^RESET%^
.EI
.SI 5
void remove_request(class request bing)
.EI

.SI 3
* %^BOLD%^send_request%^RESET%^
.EI
.SI 5
void send_request(class request bing)
.EI

.SI 3
* %^BOLD%^spell_word%^RESET%^
.EI
.SI 5
int spell_word(string word, int all, function call_back)
.EI


