.DT
http.c
Disk World autodoc help
http.c

.SH Inherits
.SP 5 5
This class inherits the following classes /net/inherit/server.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/config.h, /include/player_handler.h, /include/localtime.h, /include/login_handler.h, /include/network.h, /include/login.h, /include/runtime_config.h, /include/socket_errors.h, /include/mime.h and /include/http.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^escape_html%^RESET%^
.EI
.SI 5
string escape_html(string s)
.EI

.SI 3
* %^BOLD%^find_last_token%^RESET%^
.EI
.SI 5
int find_last_token(string line, string token)
.EI

.SI 3
* %^BOLD%^find_token%^RESET%^
.EI
.SI 5
int find_token(string line, string token)
.EI

.SI 3
* %^BOLD%^format_date%^RESET%^
.EI
.SI 5
string format_date(int x)
.EI

.SI 3
* %^BOLD%^handle_body%^RESET%^
.EI
.SI 5
int handle_body(class http_request req)
.EI

.SI 3
* %^BOLD%^hex_to_decimal%^RESET%^
.EI
.SI 5
int hex_to_decimal(string str)
.EI

.SI 3
* %^BOLD%^ltime2sec%^RESET%^
.EI
.SI 5
int ltime2sec(mixed * ltime)
.EI

.SI 3
* %^BOLD%^mime%^RESET%^
.EI
.SI 5
string mime(string str)
.EI

.SI 3
* %^BOLD%^next_token%^RESET%^
.EI
.SI 5
string next_token(string ref line)
.EI

.SI 3
* %^BOLD%^parse_date%^RESET%^
.EI
.SI 5
int parse_date(string date)
.EI

.SI 3
* %^BOLD%^parse_uri%^RESET%^
.EI
.SI 5
void parse_uri(class http_request req, string uri)
.EI

.SI 3
* %^BOLD%^query_current_file%^RESET%^
.EI
.SI 5
string query_current_file()
.EI

.SI 3
* %^BOLD%^query_current_no_reads%^RESET%^
.EI
.SI 5
int query_current_no_reads(string file)
.EI

.SI 3
* %^BOLD%^query_files_read%^RESET%^
.EI
.SI 5
int query_files_read()
.EI

.SI 3
* %^BOLD%^tidy_files_read%^RESET%^
.EI
.SI 5
void tidy_files_read()
.EI

.SI 3
* %^BOLD%^unmime%^RESET%^
.EI
.SI 5
int unmime(string ref url, int is_path)
.EI

.SI 3
* %^BOLD%^uudecode%^RESET%^
.EI
.SI 5
string uudecode(string bufcoded)
.EI


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^auth_failure%^RESET%^
.EI
.SI 5
void auth_failure(class http_request req)
.EI

.SI 3
* %^BOLD%^auth_name%^RESET%^
.EI
.SI 5
string auth_name(class http_request req)
.EI

.SI 3
* %^BOLD%^basic_header%^RESET%^
.EI
.SI 5
void basic_header(class http_request req)
.EI

.SI 3
* %^BOLD%^check_fulluri%^RESET%^
.EI
.SI 5
string check_fulluri(class http_request req, string uri)
.EI

.SI 3
* %^BOLD%^clean_sockets%^RESET%^
.EI
.SI 5
void clean_sockets()
.EI

.SI 3
* %^BOLD%^close_connection%^RESET%^
.EI
.SI 5
varargs void close_connection(int fd, int persist)
.EI

.SI 3
* %^BOLD%^each_byterange%^RESET%^
.EI
.SI 5
int each_byterange(class http_request req, int ref offset, int ref length)
.EI

.SI 3
* %^BOLD%^eventNewConnection%^RESET%^
.EI
.SI 5
void eventNewConnection(int fd)
.EI

.SI 3
* %^BOLD%^eventRead%^RESET%^
.EI
.SI 5
varargs void eventRead(int fd, string str, mixed done)
.EI

.SI 3
* %^BOLD%^eventSocketClosed%^RESET%^
.EI
.SI 5
void eventSocketClosed(int fd)
.EI

.SI 3
* %^BOLD%^handle_request%^RESET%^
.EI
.SI 5
int handle_request(class http_request req, mixed done)
.EI

.SI 3
* %^BOLD%^http_header%^RESET%^
.EI
.SI 5
void http_header(class http_request req)
.EI

.SI 3
* %^BOLD%^internal_byterange%^RESET%^
.EI
.SI 5
int internal_byterange(int realreq, int ref tlength, class http_request req, string ref r_range, int ref offset, int ref length)
.EI

.SI 3
* %^BOLD%^internal_redirect%^RESET%^
.EI
.SI 5
void internal_redirect(class http_request req, string new_uri)
.EI

.SI 3
* %^BOLD%^parse_byterange%^RESET%^
.EI
.SI 5
int parse_byterange(string range, int clength, int ref start, int ref end)
.EI

.SI 3
* %^BOLD%^parse_headers%^RESET%^
.EI
.SI 5
void parse_headers(class http_request req, int i)
.EI

.SI 3
* %^BOLD%^resolve_callback%^RESET%^
.EI
.SI 5
void resolve_callback(string name, string addr, int key)
.EI

.SI 3
* %^BOLD%^send_file%^RESET%^
.EI
.SI 5
void send_file(int fd, string str)
.EI

.SI 3
* %^BOLD%^send_headers%^RESET%^
.EI
.SI 5
void send_headers(class http_request req, mapping headers, mixed * args ...)
.EI

.SI 3
* %^BOLD%^send_string%^RESET%^
.EI
.SI 5
void send_string(int fd, string str, int mod_date, string location, int no_length)
.EI

.SI 3
* %^BOLD%^send_string_array%^RESET%^
.EI
.SI 5
void send_string_array(int fd, string * str, int mod_date, string location, int no_length)
.EI

.SI 3
* %^BOLD%^set_byterange%^RESET%^
.EI
.SI 5
int set_byterange(class http_request req)
.EI

.SI 3
* %^BOLD%^set_last_modified%^RESET%^
.EI
.SI 5
int set_last_modified(class http_request req, int mtime)
.EI

.SI 3
* %^BOLD%^set_persist%^RESET%^
.EI
.SI 5
int set_persist(class http_request req)
.EI

.SI 3
* %^BOLD%^use_range_x%^RESET%^
.EI
.SI 5
int use_range_x(class http_request req)
.EI

.SI 3
* %^BOLD%^www_authenticate%^RESET%^
.EI
.SI 5
int www_authenticate(class http_request req)
.EI

.SI 3
* %^BOLD%^www_resolve%^RESET%^
.EI
.SI 5
int www_resolve(class http_request req)
.EI


