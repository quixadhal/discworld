.DT
mime.c
Disk World autodoc help
mime.c

.SH Description
.SP 5 5

A parser for RFC 822 compliant messages, with emphasis on
the headers in them, also known as MIME headers.


RFC 822 describes the format of all Internet messages. This includes things like email, usenet news and HTTP protocol messages.
.EP
.SP 10 5


Written by Turrican

Started 22 May 1998

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/mime.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^get_email_addrs%^RESET%^
.EI
.SI 5
mixed * get_email_addrs(string arg)
.EI
.SP 7 5

This method extracts valid email adresses from the given
header field or string.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
arg - the string to parse
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array consisting of an array of machine usable email adresses
(no whitespace and comments) and an array of the full addresses
.EP

.SI 3
* %^BOLD%^parse_headers%^RESET%^
.EI
.SI 5
class mime_header parse_headers(string message)
.EI
.SP 7 5

This method parses an RFC 822 compliant message and extracts all
the headers into a class mime_header. This class contains a mapping
with the header field names as keys, so you can easily select
the headers you need.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
message - the message to be parsed
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a class mime_header with the headers from the message

.EP

.SI 3
* %^BOLD%^rewrite_field%^RESET%^
.EI
.SI 5
string rewrite_field(string header_field)
.EI
.SP 7 5

This method rewrites local email addresses (as found in mudmail)
to be usable outside Discworld, for instance from an email client.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
header_field - the string to rewrite
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the new string
.EP

.SI 3
* %^BOLD%^rewrite_header%^RESET%^
.EI
.SI 5
string rewrite_header(string message)
.EI
.SP 7 5

This method rewrites all the fields from an RFC 822 compliant message
to make the message suited for Internet transport. It uses the
rewrite_field() method to achieve this. The affected header fields
are:

.EP
.SO 8 2 -12

	*	To: 
	*	From: 
	*	Cc: 
	*	Bcc: 
	*	Reply-To: 
.EO
.SP 7 5

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
message - the message to rewrite
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the possibly modified message, suited for Internet transport
.EP


