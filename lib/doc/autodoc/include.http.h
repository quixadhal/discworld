.DT
http.h
Disk World autodoc help
http.h

.SH Description
.SP 5 5

The main header file for the web system.  This header files is based on
one which comes with apache web server software.

.EP
.SP 10 5

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/mime.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^http_body%^RESET%^
class http_body {
int state;
int remaining;
int read_chunked;
int read_length;
string buff;
mapping data;
}

.EI

.SI 3
* %^BOLD%^http_body_info%^RESET%^
class http_body_info {
string content_type;
int chunked;
int clength;
int byterange;
string boundary;
string range;
}

.EI

.SI 3
* %^BOLD%^http_request%^RESET%^
class http_request {
class http_session sess;
string hostname;
int time;
string request;
string status_line;
int status;
int proto_num;
string method;
string uri;
string args;
string filename;
string dir;
string user;
class http_request_header rh;
string location;
int id;
string the_request;
int state;
int persistent;
string compression;
int proxyreq;
int numredirect;
int cgi_request;
class http_body body;
class http_body_info bi;
class http_request prev;
int delayed;
}

.EI

.SI 3
* %^BOLD%^http_request_header%^RESET%^
class http_request_header {
int noheaders;
class mime_header headers_in;
class mime_header headers_out;
class mime_header err_headers_out;
int header_only;
}

.EI

.SI 3
* %^BOLD%^http_session%^RESET%^
class http_session {
int fd;
int time;
string address;
string name;
}

.EI


.SH Defines
.SI 3
* %^BOLD%^ACCESS_DENIED%^RESET%^
.EI
.SP 7 5
The response number for a acess denied message. 
.EP

.SI 3
* %^BOLD%^AUTH_REQUIRED%^RESET%^
.EI
.SP 7 5
The response number for a authorisation required message. 
.EP

.SI 3
* %^BOLD%^BAD_CMD%^RESET%^
.EI
.SP 7 5
The response number for a bad command message. 
.EP

.SI 3
* %^BOLD%^BODY_CHUNK_END%^RESET%^
.EI

.SI 3
* %^BOLD%^BODY_CHUNK_FOOTER%^RESET%^
.EI

.SI 3
* %^BOLD%^BODY_READ%^RESET%^
.EI

.SI 3
* %^BOLD%^BODY_START%^RESET%^
.EI

.SI 3
* %^BOLD%^CONTINUE%^RESET%^
.EI
.SP 7 5

The response number for a continue.

.EP

.SI 3
* %^BOLD%^DAYS%^RESET%^
.EI

.SI 3
* %^BOLD%^DIR_WWW%^RESET%^
.EI
.SP 7 5

The directory in which the web pages exist.

.EP

.SI 3
* %^BOLD%^HTTP_ACCEPT_ENCODING%^RESET%^
.EI
.SP 7 5

The header request type for compression.

.EP

.SI 3
* %^BOLD%^HTTP_CODING_TYPE%^RESET%^
.EI
.SP 7 5

To tell the system we are going to send the content compressed.

.EP

.SI 3
* %^BOLD%^HTTP_OK%^RESET%^
.EI
.SP 7 5

The response number for an ok.

.EP

.SI 3
* %^BOLD%^HTTP_PROTOCOL%^RESET%^
.EI
.SP 7 5

The version of the http protocol that is supported.

.EP

.SI 3
* %^BOLD%^HTTP_VERSION%^RESET%^
.EI
.SP 7 5

The version of the web server.

.EP

.SI 3
* %^BOLD%^HTTPD%^RESET%^
.EI
.SP 7 5

The path to the http daemon.

.EP

.SI 3
* %^BOLD%^INTERNAL_ERROR%^RESET%^
.EI
.SP 7 5
The response number for an internal error message. 
.EP

.SI 3
* %^BOLD%^LENGTH_REQUIRED%^RESET%^
.EI
.SP 7 5
The response number for a length required message. 
.EP

.SI 3
* %^BOLD%^MONTHS%^RESET%^
.EI

.SI 3
* %^BOLD%^MOVED_PERMANENTLY%^RESET%^
.EI
.SP 7 5
The response number for a moved permanently message. 
.EP

.SI 3
* %^BOLD%^MOVED_TEMP%^RESET%^
.EI
.SP 7 5
The response number for a moved temporarily message. 
.EP

.SI 3
* %^BOLD%^NO_CONTENT%^RESET%^
.EI
.SP 7 5
The response number for a no content message. 
.EP

.SI 3
* %^BOLD%^NOT_ALLOWED%^RESET%^
.EI
.SP 7 5
The response number for a not allowed message. 
.EP

.SI 3
* %^BOLD%^NOT_FOUND%^RESET%^
.EI
.SP 7 5
The response number for a not found message. 
.EP

.SI 3
* %^BOLD%^NOT_IMPLEMENTED%^RESET%^
.EI
.SP 7 5
The response number for a not implemented message. 
.EP

.SI 3
* %^BOLD%^NOT_MODIFIED%^RESET%^
.EI
.SP 7 5
The response number for a not modified message. 
.EP

.SI 3
* %^BOLD%^PARTIAL_CONTENT%^RESET%^
.EI
.SP 7 5
The response number for a partial content message. 
.EP

.SI 3
* %^BOLD%^PRECONDITION_FAILED%^RESET%^
.EI
.SP 7 5
The response number for a precondition failed message. 
.EP

.SI 3
* %^BOLD%^REQ_BODY%^RESET%^
.EI

.SI 3
* %^BOLD%^REQ_HEADERS%^RESET%^
.EI

.SI 3
* %^BOLD%^REQ_REQUEST%^RESET%^
.EI

.SI 3
* %^BOLD%^RES_DIR%^RESET%^
.EI

.SI 3
* %^BOLD%^RES_FILE%^RESET%^
.EI

.SI 3
* %^BOLD%^RES_NORMAL%^RESET%^
.EI

.SI 3
* %^BOLD%^SAVE_FILE%^RESET%^
.EI
.SP 7 5

Where to save the status information.

.EP

.SI 3
* %^BOLD%^status_drops_connection(x)%^RESET%^
.EI

.SI 3
* %^BOLD%^TIME_OUT%^RESET%^
.EI
.SP 7 5
The response number for a time out message. 
.EP

.SI 3
* %^BOLD%^TP(STR)%^RESET%^
.EI

