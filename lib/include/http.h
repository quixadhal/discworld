/* ====================================================================
 * Copyright (c) 1995-1997 The Apache Group.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the Apache Group
 *    for use in the Apache HTTP server project (http://www.apache.org/)."
 *
 * 4. The names "Apache Server" and "Apache Group" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission.
 *
 * 5. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the Apache Group
 *    for use in the Apache HTTP server project (http://www.apache.org/)."
 *
 * THIS SOFTWARE IS PROVIDED BY THE APACHE GROUP ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE APACHE GROUP OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Group and was originally based
 * on public domain software written at the National Center for
 * Supercomputing Applications, University of Illinois, Urbana-Champaign.
 * For more information on the Apache Group and the Apache HTTP server
 * project, please see <http://www.apache.org/>.
 *
 */

/**
 * The main header file for the web system.  This header files is based on
 * one which comes with apache web server software.
 */
#ifndef __HTTP_H
#define __HTTP_H

#include <mime.h>

/**
 * The path to the http daemon.
 */
#define HTTPD "/net/daemon/http"

/**
 * The version of the http protocol that is supported.
 */
#define HTTP_PROTOCOL "HTTP/1.1"
/**
 * The version of the web server.
 */
#define HTTP_VERSION "Discworld/0.10.3"

/**
 * The header request type for compression.
 */
#define HTTP_ACCEPT_ENCODING "accept-encoding"
/**
 * To tell the system we are going to send the content compressed.
 */
#define HTTP_CODING_TYPE "Content-Encoding"

#define REQ_REQUEST 0
#define REQ_HEADERS 1
#define REQ_BODY 2

#define BODY_START 0
#define BODY_READ 1
#define BODY_CHUNK_FOOTER 2
#define BODY_CHUNK_END 3

/**
 * The directory in which the web pages exist.
 */
#define DIR_WWW "/www"

/**
 * The response number for a continue.
 */
#define CONTINUE 100
/**
 * The response number for an ok.
 */
#define HTTP_OK 200
/** The response number for a no content message. */
#define NO_CONTENT 204
/** The response number for a partial content message. */
#define PARTIAL_CONTENT 206
/** The response number for a moved permanently message. */
#define MOVED_PERMANENTLY 301
/** The response number for a moved temporarily message. */
#define MOVED_TEMP 302
/** The response number for a not modified message. */
#define NOT_MODIFIED 304
/** The response number for a bad command message. */
#define BAD_CMD 400
/** The response number for a authorisation required message. */
#define AUTH_REQUIRED 401
/** The response number for a acess denied message. */
#define ACCESS_DENIED 403
/** The response number for a not found message. */
#define NOT_FOUND 404
/** The response number for a not allowed message. */
#define NOT_ALLOWED 405
/** The response number for a time out message. */
#define TIME_OUT 408
/** The response number for a length required message. */
#define LENGTH_REQUIRED 411
/** The response number for a precondition failed message. */
#define PRECONDITION_FAILED 412
/** The response number for an internal error message. */
#define INTERNAL_ERROR 500
/** The response number for a not implemented message. */
#define NOT_IMPLEMENTED 501

nosave private mapping status_lines = ([
   100 : "100 Continue",
   101 : "101 Switching Protocols",
   200 : "200 OK",
   201 : "201 Created",
   202 : "202 Accepted",
   203 : "203 Non-Authoritative Information",
   204 : "204 No Content",
   205 : "205 Reset Content",
   206 : "206 Partial Content",
   300 : "300 Multiple Choices",
   301 : "301 Moved Permanently",
   302 : "302 Moved Temporarily",
   303 : "303 See Other",
   304 : "304 Not Modified",
   305 : "305 Use Proxy",
   400 : "400 Bad Request",
   401 : "401 Authorization Required",
   402 : "402 Payment Required",
   403 : "403 Forbidden",
   404 : "404 File Not Found",
   405 : "405 Method Not Allowed",
   406 : "406 Not Acceptable",
   407 : "407 Proxy Authentication Required",
   408 : "408 Request Time-out",
   409 : "409 Conflict",
   410 : "410 Gone",
   411 : "411 Length Required",
   412 : "412 Precondition Failed",
   413 : "413 Request Entity Too Large",
   414 : "414 Request-URI Too Large",
   415 : "415 Unsupported Media Type",
   500 : "500 Internal Server Error",
   501 : "501 Method Not Implemented",
   502 : "502 Bad Gateway",
   503 : "503 Service Temporarily Unavailable",
   504 : "504 Gateway Time-out",
   505 : "505 HTTP Version Not Supported",
   506 : "506 Variant Also Varies"
]);

/**
 * Where to save the status information.
 */
#define SAVE_FILE "/net/save/http"

#define status_drops_connection(x) (((x) == BAD_CMD)          || \
                                    ((x) == TIME_OUT)         || \
                                    ((x) == INTERNAL_ERROR)   || \
                                    ((x) == LENGTH_REQUIRED))

#undef DEBUG

#ifdef DEBUG
#define TP(STR) if (find_player("ceres")) tell_object(find_player("ceres"), STR)
#else
#define TP(STR)
#endif

#define RES_DIR    0
#define RES_FILE   1
#define RES_NORMAL 2

#define DAYS ({"Sun", "Mon", "Tue", "Wed", "Thu", \
         "Fri", "Sat"})

#define MONTHS ({"Jan", "Feb", "Mar", "Apr", "May", "Jun", \
     "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"})

class http_session {
  int fd;                       /* file descriptor */
  int time;                     /* request time */
  string address;               /* socket_address() of remote */
  string name;                  /* resolved() hostname of remote */
}

class http_body {
  int state;                    /* state of the body */
  int remaining;                /* remaining length of received body */
  int read_chunked;             /* reading a chunked body */
  int read_length;              /* length read so far */
  string buff;                  /* the body itself */
  mapping data;                 /* If a post for, the data */
}

class http_request_header {
  int noheaders;                     /* stupid mode (HTTP/0.9) */
  class mime_header headers_in;      /* incoming header */
  class mime_header headers_out;     /* outgoing header */
  class mime_header err_headers_out; /* outgoing error header */
  int header_only;                   /* the "HEAD" operation was requested */
}

class http_body_info {
  string content_type;          /* MIME content type of body */
  int chunked;                  /* chunked transfer mode */
  int clength;                  /* content length of body */
  int byterange;                /* type of byterange */
  string boundary;              /* boundary of range */
  string range;                 /* range string */ 
}

class http_request {
  class http_session sess;           /* connection info */
  string hostname;              /* expected hostname of server */
  int time;                     /* request time */
  string request;               /* the request so far */
  string status_line;           /* custom status of response */
  int status;                   /* status code */
  int proto_num;                /* protocol version (numerical) */
  string method;                /* requested operation */
  string uri;                   /* the uri */
  string args;                  /* arguments to the request */
  string filename;              /* the filename of the request */
  string dir;                   /* the directory of the request */
  string user;                  /* the user name for secure transactions */
  class http_request_header rh; /* header junk */
  string location;              /* the new location of the document */
  int id;                       /* call out id for timeout */
  string the_request;           /* first line of the request */
  int state;                    /* state of the request */
  int persistent;               /* is this connection persistent? */
  string compression;           /* does the client accept compression? */
  int proxyreq;                 /* this is a proxy request */
  int numredirect;              /* amount of internal redirects done */
  int cgi_request;              /* Is this a cgi bin request? */
  class http_body body;         /* incoming body */
  class http_body_info bi;      /* various junk about the body */
  class http_request prev;      /* previous request */
  int delayed;                  /* In a delayed response */
}

#endif /* __HTTP_H */
