/*  -*- LPC -*-  */
/*
 * $Locker: pinkfish $
 * $Id: http.c,v 1.25 2000/06/27 19:00:39 turrican Exp pinkfish $
 */
/*    /daemon/http.c
 *    from Nightmare IV
 *    an http daemon that can talk to Mosaic and other WWW clients
 *    created by Descartes of Borg 940521
 *    callback fix by Robocoder 950117
 *    modified for discworld Pinkfish 10th of March 1995
 *    modified to allow 302 (diverted address) messages on 23rd of Feburary 1996
 *              - Pinkfish
 *    modified to use the new inheritable server object by Turrican, 24-4-96
 *    basically rewritten to be more HTTP/1.x compliant - Turrican, 31-1-97
 *    modified to add conditional gets and the like - Turrican, 11-2-97
 *    modified to be conditionally HTTP/1.1 compliant - Turrican, 24-6-97
 *    modified to add the "POST" method - Turrican, 27-6-97
 *    modified to add support for byteranges - Turrican, 2-7-97
 *    modified to add support for reading chunked bodies - Turrican, 22-7-97
 *    modified to support files larger than 200k - Pinkfish 11-11-97
 *    modified to add Basic authentication scheme - Turrican, 19-11-97
 *    modified to decode post attempts - Pinkfish, 20-11-97
 */

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

#include <network.h>
#include <localtime.h>
#include <runtime_config.h>
#include <login.h>
#include <config.h>

inherit SERVER;

#include <http.h>

#define IS_PATH 1
#define NOT_PATH 0

#define LOGGING 1

protected int www_resolve(class http_request req);
protected void resolve_incoming(string addr, string nom, int cle);
private void http_error(int fd, int status);
varargs private void add_activity(int fd, string str, mixed *args ...);
varargs protected void close_connection(int fd, int persist);
varargs private void get_file(class http_request req, string body);
string format_date(int x);
protected void send_file(int fd, string str);
protected void send_string(int fd, string str, int last_mod, string location,
                           int no_length);
protected void parse_headers(class http_request req, int i);

protected void send_string_array(int fd, string *str, int mod_date,
                           string location, int no_length);

private nosave mapping Sockets;
private nosave mapping Resolve; 
private nosave string current_file;
private nosave int max_file_size;

#ifdef TRACK_PAGE_STATS
mapping http_stats;
#endif

// stuff used for log caching
nosave string log_str;
nosave string cday;

protected void create() {
   server::create();
   SetSocketType(STREAM);
   SetDestructOnClose(1);
   Sockets = ([]);
   Resolve = ([]);

   max_file_size = get_config(__MAX_BUFFER_SIZE__);
   if (get_config(__MAX_STRING_LENGTH__) < max_file_size) {
      max_file_size =get_config(__MAX_STRING_LENGTH__);
   }
   if ( get_config(__MAX_READ_FILE_SIZE__) < max_file_size) {
      max_file_size = get_config(__MAX_READ_FILE_SIZE__);
   }
   if (get_config(__MAX_BYTE_TRANSFER__) < max_file_size) {
      max_file_size = get_config(__MAX_BYTE_TRANSFER__);
   }
   
   call_out("setup", 2);
   call_out("clean_sockets", 180);
   unguarded((: restore_object, SAVE_FILE :));
#ifdef TRACK_PAGE_STATS
   if (!mapp(http_stats)) {
      http_stats = ([]);
   }
#endif  
} /* create() */

protected void setup() {
   if (eventCreateSocket(PORT_HTTP) < 0) {
      if (this_object()) destruct(this_object());
   }
} /* setup() */

protected void reset() {
#ifdef TRACK_PAGE_STATS
   unguarded((: save_object, SAVE_FILE :));
#endif  
}

int hex_to_decimal(string str) {
   int ret;

   ret = ((str[0] >= 'A') ? ((str[0] & 0xdf) - 'A')+10 : (str[0] - '0'));
   ret *= 16;
   ret += (str[1] >= 'A' ? ((str[1] & 0xdf) - 'A')+10 : (str[1] - '0'));
   
   return ret;
} /* hex_to_decimal() */

/*
 * Replace all the mime codes in the address,
 * return HTTP_OK on success, error code on failure.
 */
int unmime(string ref url, int is_path) {
   int x, y, badesc, badpath, len, char, pos = -1;

   badesc = 0;
   badpath = 0;
   len = strlen(url);
   for (x = 0, y = 0; y < len; ++x, ++y) {
      if (url[y] != '%') {
         url[x] = url[y];
      } else {
         if (!isxdigit(url[y+1]) || !isxdigit(url[y+2])) {
            badesc = 1;
            url[x] = '%';
         } else {
            char = hex_to_decimal(url[y+1..]);
            y += 2;
            if (char) {
               url[x] = char;
            } else if (pos == -1) {
               pos = x;
            }
            if ((char == '/' && is_path) || !char) {
               badpath = 1;
            }
         }
      }
   }
   url = url[0..x-1];
   if (badesc) {
      return BAD_CMD;
   } else if (badpath) {
      url = url[0..pos-1];
      return NOT_FOUND;
   } else {
      return HTTP_OK;
   }
} /* unmime() */

string mime(string str) {
   return replace_string(str, " ", "%20");
} /* mime() */

string escape_html(string s) {
   return replace(s, ({ "&", "&amp;", "<", "&lt;", ">", "&gt;"}));
} /* escape_html() */

string next_token(string ref line) {
   int i, len = strlen(line);
   string ret, tspecials = " \t()<>@,;:\\/[]?={}";
   
   while ((i < len) && (iscntrl(line[i]) ||
                        (strsrch(tspecials, line[i]) != -1))) {
      if (line[i] == '"') {
         while ((i < len) && (line[i] != '"'))
            ++i;
      } else {
         ++i;
      }
   }
   if (i == len) {
      ret = 0;
   } else {
      ret = line[i..];
      
      i = 0;
      while ((i < len) && !iscntrl(line[i]) &&
             (strsrch(tspecials, line[i]) == -1)) {
         ++i;
      }
      
      if (i < len) {
         line = line[i+1..];
      } else {
         line = "";
      }
   }
   return (ret ? ret[0..i-1] : 0);
} /* next_token() */

int find_token(string line, string token) {
   string ltok;
  
   if (!line) {
      return 0;
   }

   while ((ltok = next_token(ref line))) {
      if (lower_case(ltok) == lower_case(token)) {
         return 1;
      }
   }

   return 0;
} /* find_token() */

int find_last_token(string line, string token) {
   int tlen;
   
   tlen = strlen(token);
   if (!line ||
       (strlen(line) <= tlen) ||
       (!isspace(line[<tlen+1]) && (line[<tlen+1] != ','))) {
      return 0;
   }
   return (lower_case(line[<tlen..]) == lower_case(token));
} /* find_last_token() */

protected void eventNewConnection(int fd) {
   class http_request t;
   string tmp;
   int x;

   server::eventNewConnection(fd);
   sscanf(socket_address(fd), "%s %*s", tmp);
   x = resolve(tmp, "resolve_callback");
   Resolve[x] = fd;
   t = new(class http_request, uri : "", request : "", status : TIME_OUT);
   t->sess = new(class http_session, fd : fd, address : tmp);
   t->rh = new(class http_request_header);
   t->rh->headers_in = new(class mime_header, header_m : ([ ]), header_k : ({ }));
   t->rh->headers_out = new(class mime_header, header_m : ([ ]), header_k : ({ }));
   t->rh->err_headers_out = new(class mime_header, header_m : ([ ]), header_k : ({ }));
   t->body = new(class http_body);
   t->bi = new(class http_body_info);
   Sockets[fd] = t;
} /* eventNewConnection */

protected int parse_byterange(string range, int clength, int ref start,
                              int ref end) {
   int len;
   int num;
   int dashpos = strsrch(range, '-');
   string dash;

   if (dashpos == -1) {
      return 0;
   }

   if (dashpos == 0) {
      /* In the form "-5" */
      sscanf(range[dashpos+1..], "%d", num);
      start = clength - num;
      end = clength - 1;
   } else {
      len = strlen(range);
      dash = range;
      range = range[0..dashpos-1];
      dashpos++;
      sscanf(range, "%d", start);
      if (dashpos < len) {
         sscanf(dash[dashpos..], "%d", end);
      } else {
         end = clength - 1;
      }
   }
   
   if (start > end) {
      return 0;
   }
   
   if (end >= clength) {
      end = clength - 1;
   }
   
   return 1;
} /* parse_byterange() */

protected int internal_byterange(int realreq, int ref tlength,
                                 class http_request req, string ref r_range,
                                 int ref offset, int ref length) {
   int range_start;
   int range_end;
   int i;
   string range;

   if (!strlen(r_range)) {
      if (req->bi->byterange > 1) {
         if (realreq) {
            eventWrite(req->sess->fd, sprintf("\r\n--%s--\r\n",
                                              req->bi->boundary));
         } else {
            tlength += 4 + strlen(req->bi->boundary) + 4;
         }
      }
      return 0;
   }
   
   if ((i = strsrch(r_range, ',')) == -1) {
      range = r_range;
      r_range = "";
   } else {
      range = r_range[0..i-1];
      r_range = r_range[i+1..];
   }
   
   if (!parse_byterange(range, req->bi->clength, ref range_start,
                        ref range_end))
      return internal_byterange(realreq, ref tlength, req, ref r_range,
                                ref offset, ref length);
   
   if (req->bi->byterange > 1) {
      string ts;
      string ct;
      
      ct = req->bi->content_type ? req->bi->content_type : "text/html";
      ts = sprintf("%d-%d/%d", range_start, range_end, req->bi->clength);
      if (realreq) {
         eventWrite(req->sess->fd, sprintf("\r\n--%s\r\nContent-Type: %s\r\n"
                                           "Content-Range: bytes %s\r\n\r\n",
                                           req->bi->boundary, ct, ts));
      } else {
         tlength += 4 + strlen(req->bi->boundary) + 16 + strlen(ct) + 23 +
            strlen(ts) + 4;
      }
   }
   if (realreq) {
      offset = range_start;
      length = range_end - range_start;
   } else {
      tlength += range_end - range_start + 1;
   }
   return 1;
} /* internal_byterange() */

protected int each_byterange(class http_request req, int ref offset,
                             int ref length) {
   int tlength;
   
   return internal_byterange(1, ref tlength, req, ref req->bi->range,
                             ref offset, ref length);
} /* each_byterange() */

protected int set_byterange(class http_request req) {
   string range;
   string if_range;
   string match;
   int range_start;
   int range_end;
   
   if (!req->bi->clength || req->rh->noheaders) {
      return 0;
   }
  
   if (!(range = req->rh->headers_in->header_m["range"])) {
      range = req->rh->headers_in->header_m["request-range"];
   }

   if (!range || (range[0..5] != "bytes=")) {
      req->rh->headers_out->header_m["Accept-Ranges"] = "bytes";
      req->rh->headers_out->header_k += ({ "Accept-Ranges" });
      return 0;
   }

   if ((if_range = req->rh->headers_in->header_m["if-range"])) {
      if (if_range[0] == '"') {
         if (!(match = req->rh->headers_out->header_m["ETag"]) ||
             (lower_case(match) != lower_case(if_range))) {
            return 0;
         }
      } else if (!(match = req->rh->headers_out->header_m["Last-Modified"]) ||
                 (lower_case(match) != lower_case(if_range))) {
         return 0;
      }
   }

   if (strsrch(range, ',') == -1) {
      if (!parse_byterange(range[6..], req->bi->clength, ref range_start,
                           ref range_end)) {
         return 0;
      }

      req->bi->byterange = 1;
      req->rh->headers_out->header_m["Content-Range"] =
         sprintf("bytes %d-%d/%d", range_start, range_end, req->bi->clength);
      req->rh->headers_out->header_m["Content-Length"] =
         sprintf("%d", range_end - range_start + 1);
      req->rh->headers_out->header_k += ({ "Content-Range" });
   } else {
      int tlength, d1, d2;
      string r_range = range[6..];
      
      req->bi->byterange = 2;
      req->bi->boundary = sprintf("%x%x", req->time, req->sess->fd);
      while (internal_byterange(0, ref tlength, req, ref r_range, ref d1,
                                ref d2)) {
         ;
      }
      req->rh->headers_out->header_m["Content-Length"] =
         sprintf("%d", tlength);
   }
   
   req->status = PARTIAL_CONTENT;
   req->bi->range = range[6..];
   
   return 1;
} /* set_byterange() */

protected int set_persist(class http_request req) {
   int ka_sent;
   int wimpy;
   string conn = req->rh->headers_in->header_m["connection"];
   
   wimpy = find_token(req->rh->headers_out->header_m["Connection"], "close");
   if (((req->status == NOT_MODIFIED) ||
        (req->status == NO_CONTENT) ||
        req->rh->header_only ||
        !undefinedp(req->rh->headers_out->header_m["Content-Length"]) ||
        find_last_token(req->rh->headers_out->header_m["Transfer-Encoding"],
                        "chunked") ||
        ((req->proto_num >= 1001) && (req->bi->chunked = 1))) &&
       !status_drops_connection(req->status) &&
       !wimpy &&
       !find_token(conn, "close") &&
       (((ka_sent = find_token(conn, "keep-alive"))) ||
        (req->proto_num >= 1001))
       ) {
      req->persistent = 1;
      
      /* If they sent a Keep-Alive token, send one back */
      if (ka_sent) {
         req->rh->headers_out->header_m["Keep-Alive"] = "timeout=15";
         req->rh->headers_out->header_k += ({ "Keep-Alive" });
         if (!undefinedp(req->rh->headers_out->header_m["Connection"])) {
            req->rh->headers_out->header_m["Connection"] += ", Keep-Alive";
         } else {
            req->rh->headers_out->header_m["Connection"] = "Keep-Alive";
            req->rh->headers_out->header_k += ({ "Connection" });
         }
      }
      
      return 1;
   }
   
   if (!undefinedp(req->rh->headers_out->header_m["Connection"])) {
      req->rh->headers_out->header_m["Connection"] += ", close";
   } else {
      req->rh->headers_out->header_m["Connection"] = "close";
      req->rh->headers_out->header_k += ({ "Connection" });
   }
   
   req->persistent = 0;
   
   return 0;
} /* set_persist() */

protected void basic_header(class http_request req) {
   int fd = req->sess->fd;
   
   if (req->rh->noheaders) {
      return;
   }
   if (!req->status_line) {
      req->status_line = status_lines[req->status];
   }
   eventWrite(fd, sprintf("%s %s\r\n", HTTP_PROTOCOL, req->status_line));
   eventWrite(fd, sprintf("Date: %s\r\n", format_date(req->time)));
   eventWrite(fd, sprintf("Server: %s\r\n", HTTP_VERSION));
   
   map_delete(req->rh->headers_out->header_m, "Date");
   map_delete(req->rh->headers_out->header_m, "Server");
   req->rh->headers_out->header_k -= ({ "Date", "Server" });
} /* basic_header() */

protected void send_headers(class http_request req, mapping headers,
                            mixed *args ...) {
   string key;
   
   foreach (key in args) {
      if (undefinedp(headers[key])) {
         continue;
      }
      eventWrite(req->sess->fd, sprintf("%s: %s\r\n", key,
                                        headers[key]));
   }
   eventWrite(req->sess->fd, "\r\n");
} /* send_headers() */

protected int use_range_x(class http_request req) {
   string ua;
  
   return (!undefinedp(req->rh->headers_in->header_m["request-range"]) ||
           ((ua = req->rh->headers_in->header_m["user-agent"]) &&
            (strsrch(ua, "MSIE 3") != -1)));
} /* use_range_x() */

protected void http_header(class http_request req) {
   if (req->rh->noheaders)
      return;
   
   basic_header(req);
   
   set_persist(req);
   
   if (req->bi->chunked) {
      if (!undefinedp(req->rh->headers_out->header_m["Transfer-Encoding"])) {
         req->rh->headers_out->header_m["Transfer-Encoding"] += ", chunked";
      } else {
         req->rh->headers_out->header_m["Transfer-Encoding"] = "chunked";
      }
      req->rh->headers_out->header_k += ({ "Transfer-Encoding" });
      map_delete(req->rh->headers_out->header_m, "Content-Length");
      req->rh->headers_out->header_k -= ({ "Content-Length" });
   }
   
   if (req->bi->byterange > 1) {
      req->rh->headers_out->header_m["Content-Type"] =
         sprintf("multipart%sbyteranges; boundary=%s",
                 (use_range_x(req) ? "/x-" : "/"), req->bi->boundary);
   } else if (req->bi->content_type) {
      req->rh->headers_out->header_m["Content-Type"] = req->bi->content_type;
   } else {
      req->rh->headers_out->header_m["Content-Type"] = "text/plain";
   }
   req->rh->headers_out->header_k += ({ "Content-Type" });
   
   /* Not used yet 
      if (req->no_cache &&
      undefinedp(req->rh->headers_out->header_m["Expires"])) {
      req->rh->headers_out->header_m["Expires"] = format_date(req->time);
      req->rh->headers_out->header_k += ({ "Expires" });
      } */

   send_headers(req, req->rh->headers_out->header_m,
                req->rh->headers_out->header_k ...);
} /* http_header() */

protected void handle_request(class http_request req) {
   int errno;
   int fd = req->sess->fd;
   string buff;
   string key;
   
   if (req->method == "HEAD") {
      if (req->rh->noheaders) {
         http_error(fd, BAD_CMD);
         return;
      }
      req->rh->header_only = 1;
   }
   if ((!req->hostname && (req->proto_num >= 1001)) ||
       ((req->proto_num == 1001) &&
        undefinedp(req->rh->headers_in->header_m["host"]))) {
      /* Client didn't send the hostname, HTTP/1.1 requires this. */
      TP("client sent HTTP/1.1 request without hostname\n");
      http_error(fd, BAD_CMD);
      return;
   }
   if (req->method != "TRACE") {
      if ((errno = unmime(ref req->uri, IS_PATH)) != HTTP_OK) {
         http_error(fd, errno);
         return;
      }
      if ((errno = www_resolve(req)) != HTTP_OK) {
         if (!((req->method == "OPTIONS") && (errno == NOT_FOUND))) {
            http_error(fd, errno);
            return;
         }
      }
   }
   switch (req->method) {
   case "POST":
      if (explode(req->filename, ".")[<1] != "c")
         http_error(fd, NOT_ALLOWED);
      return;
   case "TRACE":
      /* We want the original request here. */
      while (req->prev) {
         req = req->prev;
      }
      req->bi->content_type = "message/http";
      http_header(req);
      buff = sprintf("%s\r\n", req->the_request);
      foreach (key in req->rh->headers_in->header_k) {
         if (undefinedp(req->rh->headers_in->header_m[lower_case(key)])) {
            continue;
         }
         buff += sprintf("%s: %s\r\n", key,
                         req->rh->headers_in->header_m[lower_case(key)]);
      }
      buff += sprintf("\r\n");
      if (req->bi->chunked) {
         eventWrite(fd, sprintf("%x\r\n", strlen(buff)));
      }
      eventWrite(fd, buff);
      if (req->bi->chunked) {
         eventWrite(fd, "\r\n0\r\n\r\n");
      }
      return;
   case "OPTIONS":
      if (req->rh->noheaders) {
         http_error(fd, BAD_CMD);
         return;
      }
      basic_header(req);
      req->rh->headers_out->header_m["Content-Length"] = 0;
      if (explode(req->filename, ".")[<1] == "c") {
         req->rh->headers_out->header_m["Allow"] =
            "GET, HEAD, POST, OPTIONS, TRACE";
      } else {
         req->rh->headers_out->header_m["Allow"] = "GET, HEAD, OPTIONS, TRACE";
      }
      req->rh->headers_out->header_k += ({ "Content-Length", "Allow" });
      set_persist(req);
      send_headers(req, req->rh->headers_out->header_m,
                   req->rh->headers_out->header_k ...);
      return;
   case "HEAD":
   case "GET":
      get_file(req);
      return;
   default:
      http_error(fd, NOT_IMPLEMENTED);
      return;
   }
} /* handle_request() */

int handle_body(class http_request req) {
   class http_body body = req->body;
   string tenc;
   string len;
   string buf;
   int i;
   int slen;
   int len_to_read;

   if ((req->status != HTTP_OK) || (req->method != "POST")) {
      return HTTP_OK;
   }
  
   switch (body->state) {
   case BODY_START:
      tenc = req->rh->headers_in->header_m["transfer-encoding"];
      len = req->rh->headers_in->header_m["content-length"];
      if (tenc) {
         if (tenc != "chunked")
            return NOT_IMPLEMENTED;
         /* It's chunked, we don't allow that for POST requests yet.
            return (len) ? BAD_CMD : LENGTH_REQUIRED; */
         
         body->read_chunked = 1;
      } else if (len) {
         slen = strlen(len);
         while ((i < slen) && (isdigit(len[i]) || isspace(len[i]))) {
            ++i;
         }
         if (i != slen) {
            return BAD_CMD;
         }
         sscanf(len, "%d", body->remaining);
      }
      if (!body->read_chunked && !body->remaining) {
         get_file(req);
         return HTTP_OK;
      }
      if (req->proto_num >= 1001) {
         eventWrite(req->sess->fd, sprintf("%s %s\r\n\r\n", HTTP_PROTOCOL,
                                           status_lines[CONTINUE]));
      }
      body->buff = "";
      body->state = BODY_READ;
   case BODY_READ:
      if (!body->read_chunked) {
         if (strlen(req->request) < body->remaining) {
            return CONTINUE;
         } else {
            body->buff = req->request[0..body->remaining-1];
            req->request = req->request[body->remaining..];
            get_file(req);
            return HTTP_OK;
         }
      }
      /* Eek, chunked read. This is no fun :) */
      if (body->remaining == 0) {
         if (sscanf(req->request, "%s\r\n%s", buf, req->request) != 2) {
            return CONTINUE;
         }
         if (!strlen(buf) || !isxdigit(buf[0])) {
            req->persistent = 0;
            return HTTP_OK;
         }
         sscanf(buf, "%x", len_to_read);
         if (len_to_read == 0) { /* last chunk */
            /* Put the previous "\r\n" back in */
            req->request = sprintf("\r\n%s", req->request); 
            if ((i = strsrch(req->request, "\r\n\r\n")) != -1) {
               parse_headers(req, i-1);
               req->request = req->request[i+4..];
               map_delete(req->rh->headers_in->header_m, "transfer-encoding");
               req->rh->headers_in->header_k -= ({ "transfer-encoding" });
               req->rh->headers_in->header_m["content-length"] =
                  sprintf("%d", body->read_length);
               req->rh->headers_in->header_k += ({ "content-length" });
               get_file(req);
               return HTTP_OK;
            } else {
               /* Not complete yet. */
               body->state = BODY_CHUNK_FOOTER;
               return CONTINUE;
            }
         } else {
            body->remaining = len_to_read;
         }
      }
      if (strlen(req->request) < body->remaining) {
         return CONTINUE;
      } else {
         body->buff += req->request[0..body->remaining-1];
         req->request = req->request[body->remaining..];
         body->read_length += body->remaining;
         body->remaining = 0;
         body->state = BODY_CHUNK_END;
      }
   case BODY_CHUNK_FOOTER:
      if ((i = strsrch(req->request, "\r\n\r\n")) != -1) {
         parse_headers(req, i-1);
         req->request = req->request[i+4..];
         map_delete(req->rh->headers_in->header_m, "transfer-encoding");
         req->rh->headers_in->header_k -= ({ "transfer-encoding" });
         req->rh->headers_in->header_m["content-length"] =
            sprintf("%d", body->read_length);
         req->rh->headers_in->header_k += ({ "content-length" });
         get_file(req);
         return HTTP_OK;
      } else {
         return CONTINUE;
      }
   case BODY_CHUNK_END:
      if (strlen(req->request) < 2) {
         return CONTINUE;
      }
      if (req->request[0..1] != "\r\n") {
         req->persistent = 0;
         return HTTP_OK;
      }
      req->request = req->request[2..];
      body->state = BODY_READ;
      if (strlen(req->request)) {
         return handle_body(req);
      }
      return CONTINUE;
   }
} /* handle_body() */

protected void parse_headers(class http_request req, int i) {
   string *bits;
   string bit;
   string cont;
   int idx;
   int len;

   bits = explode(req->request[0..i], "\r\n") - ({ "", 0 });
   foreach (bit in bits) {
      if (cont && ((bit[0] == ' ') || (bit[0] == '\t'))) {
         req->rh->headers_in->header_m[cont] += bit[1..];
         continue;
      }
      len = strlen(bit);
      if ((idx = strsrch(bit, ':')) == -1) {
         continue;
      }
      cont = bit[0..idx-1];
      if (undefinedp(req->rh->headers_in->header_m[lower_case(cont)])) {
         req->rh->headers_in->header_k += ({ cont });
      }
      cont = lower_case(cont);
      ++idx;
      while ((idx < len) && isspace(bit[idx])) {
         ++idx;
      }
      if (undefinedp(req->rh->headers_in->header_m[cont])) {
         req->rh->headers_in->header_m[cont] = bit[idx..];
      } else {
         req->rh->headers_in->header_m[cont] += sprintf(", %s", bit[idx..]);
      }
   }
} /* parse_headers() */

protected string check_fulluri(class http_request req, string uri) {
   string name;
   string host;
   int i;
   int port;
   
   if (lower_case(uri[0..6]) != "http://") {
      return uri;
   }
   name = uri[7..];
   i = strsrch(name, '/');
   name = name[0..i-1];
   if (sscanf(name, "%s:%d", host, port) != 2) {
      port = 80;
   }
   if (port != PORT_HTTP) {
      return uri;
   }
   req->hostname = host;
   /* XXX - should be done with resolve() */
   if (lower_case(host) == "discworld.imaginary.com" ||
       lower_case(host) == "discworld.lspace.org" ||
       lower_case(host) == "atuin.icepack.com" ||
       lower_case(host) == "localhost" ||
       lower_case(host) == "208.202.120.111") {
      return uri[7+i..];
   }
   return uri;
} /* check_fulluri() */

void parse_uri(class http_request req, string uri) {
   int len;
   int i;

   len = strlen(uri);
   for (i = 0; i < len; i++) {
      if (!isalnum(uri[i]) && uri[i] != '+' && uri[i] != '-' &&
          uri[i] != '.') {
         break;
      }
   }
   
   if (uri[i] == ':' && i) {
      req->proxyreq = 1;
      req->uri = uri;
      req->args = 0;
   } else if (req->method == "TRACE") {
      req->proxyreq = 0;
      req->uri = uri;
      req->args = 0;
   } else {
      req->proxyreq = 0;
      if (sscanf(uri, "%s?%s", req->uri, req->args) != 2) {
         req->uri = uri;
         req->args = 0;
      }
   }
} /* parse_uri() */

protected void eventRead(int fd, string str) {
   string uri;
   string rest;
   string protocol;
   int major;
   int minor;
   int i;
   int ret;
   class http_request req = (class http_request)Sockets[fd];

   if (req->id) {
      remove_call_out(req->id);
   }
   if (str) {
      TP("Received: " + replace(str, ({ "\r", "<CR>", "\n", "<LF>\n"})));
      req->request += str;
      if ((req->state != REQ_BODY) && (strsrch(str, '\n') == -1))
         return;
   }
   switch (req->state) {
   case REQ_REQUEST:
      /* Skip leading newlines */
      if (str == "\r\n") {
         return;
      }
      sscanf(req->request, "%*(^(\r\n)*)%s\r\n%*s", req->the_request);
      if (!req->the_request) {
         return;
      }
      req->time = time();
      if ((sscanf(req->request,
                  "%*(^(\r\n)*)%s%*(( |\t)+)%s%*(( |\t)+)%s%((\r\n).*)",
                  req->method, uri, protocol, rest) != 7) &&
          (sscanf(req->request, "%*(^(\r\n)*)%s%*(( |\t)+)%s\r\n%*s",
                  req->method, uri) != 5)) {
         req->rh->noheaders = 1;
         http_error(fd, BAD_CMD);
         return;
      }
      req->request = rest;
      uri = check_fulluri(req, uri);
      parse_uri(req, uri);
      if (!stringp(protocol) || !strlen(protocol)) {
         req->rh->noheaders = 1;
         protocol = "HTTP/0.9";
      }
      sscanf(protocol, "HTTP/%d.%d", major, minor);
      req->proto_num = 1000 * major + minor;
      req->state = REQ_HEADERS;
   case REQ_HEADERS:
      if (!req->rh->noheaders &&
          ((i = strsrch(req->request, "\r\n\r\n")) != -1)) {
         parse_headers(req, i-1);
         req->request = req->request[i+4..];
         req->status = HTTP_OK;
         if (!(req->hostname ||
               undefinedp(req->rh->headers_in->header_m["host"]))) {
            req->hostname = req->rh->headers_in->header_m["host"];
         } else {
            req->hostname = "discworld.imaginary.com:"+ PORT_HTTP;
         }

				 // Does the client support compression?
				 if(!undefinedp(req->rh->headers_in->header_m["ACCEPT_ENCODING"]) &&
						strsrch(req->rh->headers_in->header_m["ACCEPT_ENCODING"],
										"gzip, deflate") != -1)
					 req->compressed = 1;
				 
      } else if (!req->rh->noheaders) {
         /* No full set of headers received yet */
         return;
      } else if (!req->hostname) {
         req->hostname = "discworld.imaginary.com:"+ PORT_HTTP;
      }
      handle_request(req);
      req->state = REQ_BODY;
   case REQ_BODY:
      if ((ret = handle_body(req)) != HTTP_OK) {
         if (ret == CONTINUE)
            return;
         else
            http_error(fd, ret);
      }
      if (!req->persistent) {
         TP("Closing connection\n");
         eventWrite(fd, 0, 1);
         return;
      }
      req->id = call_out("close_connection", 15, fd, 1);
      req->status = TIME_OUT;
      req->rh->headers_in->header_m = ([ ]);
      req->rh->headers_in->header_k = ({ });
      req->rh->headers_out->header_m = ([ ]);
      req->rh->headers_out->header_k = ({ });
      req->rh->err_headers_out->header_m = ([ ]);
      req->rh->err_headers_out->header_k = ({ });
      req->hostname = 0;
      req->rh->noheaders = 0;
      req->rh->header_only = 0;
      req->proto_num = 0;
      req->persistent = 0;
      req->numredirect = 0;
      req->bi->chunked = 0;
      req->status_line = 0;
      req->bi->content_type = 0;
      req->bi->clength = 0;
      req->bi->byterange = 0;
      req->bi->boundary = 0;
      req->bi->range = 0;
      req->method = 0;
      req->args = 0;
      req->filename = 0;
      req->dir = 0;
      req->location = 0;
      req->the_request = 0;
      req->uri = "";
      req->body->state = BODY_START;
      req->state = REQ_REQUEST;
      if (strlen(req->request))
         eventRead(fd, 0);
   }
} /* eventRead() */

protected void eventSocketClosed(int fd) {
   class http_request req = (class http_request)Sockets[fd];
   
   if (req) {
      map_delete(Sockets, fd);
   }
} /* eventSocketClosed() */
     
protected void resolve_callback(string name, string addr, int key) {
   class http_request req;
   
   if (undefinedp(Resolve[key])) {
      return;
   }
   req = (class http_request)Sockets[Resolve[key]];
   if (req && req->sess->address == addr) {
      req->sess->name = (name ? name : addr);
   }
   map_delete(Resolve, key);
} /* resolve_callback() */

string uudecode(string bufcoded) {
   int nbytesdecoded;
   string bufin;
   string prevbufin;
   string bufout;
   int nprbytes;
   int i;
   int nbufcoded;
   int *pr2six = ({
      64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
      64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,62,64,64,64,63,
      52,53,54,55,56,57,58,59,60,61,64,64,64,64,64,64,64,0,1,2,3,4,5,6,7,8,9,
      10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,64,64,64,64,64,64,26,27,
      28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
      64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
      64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
      64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
      64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
      64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
      64,64,64,64,64,64,64,64,64,64,64,64,64 });

  /* Strip leading whitespace. */

   while (bufcoded[nbufcoded] == ' ' || bufcoded[nbufcoded] == '\t') {
      nbufcoded++;
   }

   /* Figure out how many characters are in the input buffer.
    * Allocate this many from the per-transaction pool for the result.
    */
   bufin = bufcoded[nbufcoded..];
   i = 0;
   while (pr2six[bufin[i++]] <= 63);
   nprbytes = i - nbufcoded - 1;
   nbytesdecoded = ((nprbytes+3)/4) * 3;
   
   bufout = "";
   while (nprbytes > 0) {
      bufout += sprintf("%c%c%c",
                        (pr2six[bufin[0]] << 2 | pr2six[bufin[1]] >> 4),
                        (pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2),
                        (pr2six[bufin[2]] << 6 | pr2six[bufin[3]]));
      prevbufin = bufin;
      bufin = bufin[4..];
      nprbytes -= 4;
   }
   
   if (nprbytes & 03) {
      if ((strlen(prevbufin) >= 2) && (pr2six[prevbufin[<2]] > 63)) {
         nbytesdecoded -= 2;
      } else {
         nbytesdecoded -= 1;
      }
   }
   return bufout[0..nbytesdecoded-1];
} /* uudecode() */
     
private void http_error(int fd, int status) {
   class http_request req = (class http_request)Sockets[fd];
   string buff;
   
   if (undefinedp(status_lines[status])) {
      status = 500;
   }
   req->status = status;
   
   if (status == NOT_MODIFIED) {
      basic_header(req);
      set_persist(req);
      send_headers(req, req->rh->headers_out->header_m,
                   "Connection",
                   "Keep-Alive",
                   "ETag",
                   "Content-Location",
                   "Expires",
                   "Cache-Control",
                   "Vary",
                   "Warning",
                   "WWW-Authenticate");
      return;
   }
   
   if (!req->rh->noheaders) {
      req->rh->headers_out->header_m = req->rh->err_headers_out->header_m;
      req->rh->headers_out->header_k = req->rh->err_headers_out->header_k;
      req->rh->err_headers_out->header_m = ([ ]);
      req->rh->err_headers_out->header_k = ({ });
      if (req->location && (status == MOVED_PERMANENTLY ||
                            status == MOVED_TEMP)) {
         req->rh->headers_out->header_m["Location"] = req->location;
         req->rh->headers_out->header_k += ({ "Location" });
      }
      req->bi->content_type = "text/html";
      if ((status == NOT_ALLOWED) || (status == NOT_IMPLEMENTED)) {
         if (explode(req->filename, ".")[<1] == "c") {
            req->rh->headers_out->header_m["Allow"] =
               "GET, HEAD, POST, OPTIONS, TRACE";
         } else {
            req->rh->headers_out->header_m["Allow"] = "GET, HEAD, OPTIONS, TRACE";
         }
         req->rh->headers_out->header_k += ({ "Allow" });
      }
      http_header(req);
      if (req->rh->header_only) {
         return;
      }
   }
   buff = sprintf("<HTML><HEAD>\n<TITLE>%s</TITLE>\n</HEAD>"
                  "<BODY>\n<H1>%s</H1>\n", status_lines[status],
                 status_lines[status][4..]);
   switch (status) {
   case MOVED_PERMANENTLY:
   case MOVED_TEMP:
      buff += sprintf("The document has moved <A HREF=\"%s\""
                      ">here</A>.<P>\n", escape_html(req->location));
      break;
   case BAD_CMD:
      buff += "Your browser sent a request that\n"
         "this server could not understand.<P>\n";
      break;
   case AUTH_REQUIRED:
      buff += "This server could not verify that you\n"
         "are authorized to access the document you\n"
         "requested.  Either you supplied the wrong\n"
         "credentials (e.g., bad password), or your\n"
         "browser doesn't understand how to supply\n"
         "the credentials required.<P>\n";
      break;
   case ACCESS_DENIED:
      buff += sprintf("You don't have permission to access %s\n"
                      "on this server.<P>\n"
                      "This is either because you do not have the correct "
                      "user permissions or because your site has been "
                      "<b>temporarily</b> locked out due to excessive "
                      "login failures.<p>\n"
                      "If you believe it is the latter wait 15 minutes and "
                      "then try again.<p>\n", escape_html(req->uri));
      break;
   case NOT_FOUND:
      buff += sprintf("The requested URL %s was not found on this "
                      "server.<P>\n", escape_html(req->uri));
      break;
   case NOT_ALLOWED:
      buff += sprintf("The requested method %s is not allowed for the "
                      "URL %s.<P>\n", escape_html(req->method),
                      escape_html(req->uri));
      break;
   case TIME_OUT:
      buff += "Please, I don't have all day. Goodbye.\n";
      break;
   case LENGTH_REQUIRED:
      buff += sprintf("A request of the requested method %s requires a valid "
                      "Content-length.<P>\n", escape_html(req->method));
      break;
   case PRECONDITION_FAILED:
      buff += sprintf("The precondition on the request for the URL %s "
                      "evaluated to false.<P>\n", escape_html(req->uri));
      break;
   case NOT_IMPLEMENTED:
      buff += sprintf("%s to %s not supported.<P>\n",
                      escape_html(req->method),
                      escape_html(req->uri));
      break;
   default:
      buff += "This is the end of the line. The tomatoes have finally "
         "expired.<P>\n";
      break;
   }
   buff += "</BODY></HTML>\n";
   if (req->bi->chunked) {
      eventWrite(fd, sprintf("%x\r\n", strlen(buff)));
   }
   eventWrite(fd, buff);
   if (req->bi->chunked) {
      eventWrite(fd, "\r\n0\r\n\r\n");
   }
} /* http_error() */
     
varargs protected void close_connection(int fd, int persist) {
   class http_request req = (class http_request)Sockets[fd];

   if (!req) {
      return;
   }
   if (!req->sess->name) {
      req->sess->name = req->sess->address;
   }
   if (!persist) {
      http_error(fd, TIME_OUT);
   }
   eventWrite(fd, 0, 1);
} /* close_connection() */
     
protected void clean_sockets() {
   class http_request req;
       
   foreach (req in (class http_request *)values(Sockets)) {
      if (!req->time) {
         req->time = time();
         continue;
      }
      if (time() - req->time > 180) {
         close_connection(req->sess->fd);
      }
   }
   call_out("clean_sockets", 180);
} /* clean_sockets() */

protected string auth_name(class http_request req) {
   return "Discworld-login";
} /* auth_name() */

protected void auth_failure(class http_request req) {
   string mess;

   if (req->user) {
      mess = capitalize(req->user) + " failed to login, web, " +
         (req->sess->name ? req->sess->name + " (" +
          req->sess->address : req->sess->address);
      event(users(), "inform", mess, "bad-password");
      log_file("BAD_PASSWORD", ctime(time()) + ": " + mess + "\n");
      LOGIN_HANDLER->failed_login(req->sess->address);
   }

   req->rh->err_headers_out->header_m["WWW-Authenticate"] =
      sprintf("Basic realm=\"%s\"", auth_name(req));
   req->rh->err_headers_out->header_k += ({ "WWW-Authenticate" });
} /* auth_failure() */

private int is_secure_dir(string dir) {
   if (dir[0..strlen(DIR_WWW+"/secure/")-1] == DIR_WWW+"/secure/") {
      return 1;
   }

   // Check to see if it is a creator or domain directory.
   if (dir[0..2] == "/w/" ||
       dir[0..1] == "w/" ||
       dir[0..2] == "/d/" ||
       dir[0..1] == "d/") {
      if (strsrch(dir, "/secure/") != -1) {
         return 1;
      }
   }

   return 0;
} /* is_secure_dir() */

private int is_secure_creator_dir(string dir) {
   if (dir[0..strlen(DIR_WWW+"/secure/creator/")-1] == 
       DIR_WWW+"/secure/creator/") {
      return 1;
   }

   // Check to see if it is a creator or domain directory.
   if (dir[0..2] == "/w/" ||
       dir[0..1] == "w/" ||
       dir[0..2] == "/d/" ||
       dir[0..1] == "d/") {
      if (strsrch(dir, "/secure/creator/") != -1) {
         return 1;
      }
   }

   return 0;
} /* is_secure_dir() */

protected int www_authenticate(class http_request req) {
   string auth_req = req->rh->headers_in->header_m["authorization"];
   int i;
   string decoded;
   string *bits;
   string mess;
   
   if (!is_secure_dir(req->dir)) {
      return HTTP_OK;
   }

   if (!auth_req) {
      auth_failure(req);
      return AUTH_REQUIRED;
   }

   i = strsrch(auth_req, ' ');
   if ((i == -1) || (auth_req[0..i-1] != "Basic")) {
      auth_failure(req);
      return AUTH_REQUIRED;
   }

   decoded = uudecode(auth_req[i+1..]);
   bits = explode(decoded, ":");
   if (sizeof(bits) < 2) {
      auth_failure(req);
      return AUTH_REQUIRED;
   }

   /* Fill in the user for logging and lockdown purposes */
   req->user = lower_case(bits[0]);

   if(LOGIN_HANDLER->site_lockeddown(req->sess->address)) {
     mess = bits[0] + " attempted login from locked down site, "
       "web, " + (req->sess->name ? req->sess->name + " (" +
                  req->sess->address : req->sess->address);
     event(users(), "inform", mess, "bad-password");
     log_file("BAD_PASSWORD", ctime(time()) + ": " + mess + "\n");
     return ACCESS_DENIED;
   }
   
   if (!PLAYER_HANDLER->test_password(lower_case(bits[0]), bits[1])) {
      auth_failure(req);
      return AUTH_REQUIRED;
   }

   if (is_secure_creator_dir(req->dir)) {
      if (!PLAYER_HANDLER->test_creator(bits[0])) {
         return ACCESS_DENIED;
      }
   }

   req->rh->headers_out->header_m["Cache-Control"] = "private";
   return HTTP_OK;
} /* www_authenticate() */

protected int www_resolve(class http_request req) {
   string *parts;
   string file;
   string dir;
   string args;
   int fsize;
   
   file = req->uri;
   if (file[0] != '/' && (file != "*")) {
      if (req->proxyreq) {
         /* Er. This was a proxy request. We aren't a proxy. */
         return ACCESS_DENIED;
      } else {
         return BAD_CMD;
      }
   }
   parts = explode(file, "/") - ({ ".", "..", "" });
   if (!sizeof(parts)) {
      req->uri += "index.html";
      req->filename = sprintf("%s/index.html", DIR_WWW);
      req->dir = DIR_WWW+ "/";
      return HTTP_OK;
   }
   
   // So that external references are moved to the external httpd..
#ifdef CONFIG_REWRITE_EXTERNAL_ADDRESS
   if (parts[0] == "external") {
      req->location = sprintf("http://discworld.imaginary.com%s",
                              req->uri);
      return MOVED_PERMANENTLY;
   }
#endif
   
   if (parts[0][0] == '~' && strlen(parts[0]) > 2) {
      if (parts[0][1] >= 'a' && parts[0][1] <= 'z') {
         parts[0] = sprintf("/w/%s/public_html", parts[0][1..]);
      } else {
         parts[0] = sprintf("/d/%s/public_html", lower_case(parts[0])[1..]);
      }
      file = implode(parts, "/");
      dir = implode(explode(file, "/")[0..<2], "/");
   } else {
      file = DIR_WWW+"/"+implode(parts, "/");
      dir = DIR_WWW+"/"+implode(parts[0..<2], "/");
   }
   sscanf(file, "%s___%s", file, args);
   fsize = file_size(file);
   
   if(fsize == -2) {
      if (req->uri[<1] == '/') {
         dir = file;
         file += "/index.html";
         fsize = file_size(file);
      } else {
         req->location = sprintf("http://%s%s/", req->hostname, req->uri);
         return MOVED_PERMANENTLY;
      }
   }
   if (args) {
      req->filename = file+"___"+args;
   } else {
      req->filename = file;
   }
   req->dir = (dir[<1] == '/' ? dir : dir + "/");
   if (fsize <= 0) {
      return NOT_FOUND;
   }
   return www_authenticate(req);
} /* www_resolve() */
   
/* This handles the @@ stuff in the files... */ 
private string parse_file_string(string str, class http_request req, mapping fields) {
   string *bits;
   string old_uri;
   string old_dir;
   string old_fname;
   int i;
   string fname;
   string args;

   bits = explode(str, "@@");
   str = "";
   old_uri = req->uri;
   old_fname = req->filename;
   old_dir = req->dir;
   for (i = 0; i < sizeof(bits); i += 2) {
      str += bits[i];
      if (i+1 < sizeof(bits)) {
        sscanf(bits[i+1], "%s:%s", fname, args);
        if (!fname) {
           fname = bits[i + 1];
           args = "";
        }
        if (fname[0] != '/') {
          fname =  old_dir + fname;
        } else {
          req->uri = fname;
          req->dir = old_dir;
          www_resolve(req);
          fname = req->filename;
        }
        bits[i+1] = "Oook!  Error!";
        TP("Calling "+ fname+ "->www_function( \""+ args+ "\" )\n");
        if (catch(bits[i+1] = 
                  call_other(fname, "www_function", args, fields, req))) {
          str += "Error!";
        } else {
          str += bits[i+1];
        }
      }
   }
   req->uri = old_uri;
   req->filename = old_fname;
   req->dir = old_dir;
   return str;
} /* parse_file_string() */

private void get_file(class http_request req) {
   string fname;
   string *bits;
   string bit;
   string id;
   string *parts;
   string location;
   int fsize;
   int errno;
   int mod_date;
   int i;
   int fd = req->sess->fd;
   mapping fields;
   mixed str;
   object cgibin;
       
   TP("Started get.\n");
   fields = ([ ]);
   if (req->args) {
      bits = explode(req->args, "&");
      foreach (bit in bits) {
         sscanf(bit, "%s=%s", id, str);
         
         if (!id) {
            continue;
         }
         if (!str) {
            str = "";
         }
         str = replace_string(str, "+", " ");
         if (((errno = unmime(ref id, NOT_PATH)) != HTTP_OK) ||
             ((errno = unmime(ref str, NOT_PATH)) != HTTP_OK)) {
            http_error(fd, errno);
            return;
         }
         fields[id] = str;
      }
      req->args = 0;
   }
   sscanf(req->filename, "%s___%s", req->filename, req->args);
   
   if (!req->args) {
      req->args = "";
   }
   TP("File = "+req->filename+" Args = "+req->args+"\n");
  
   current_file = req->filename;
#ifdef TRACK_PAGE_STATS 
   http_stats[req->filename]++;
#endif  
    
   TP("Found file "+req->filename+"\n");
   parts = explode(req->filename, ".");

   switch (parts[<1]) {
   case "c":
      TP("Attempting a call.\n");
      if (!(cgibin = find_object(req->filename))) {
         if (catch(cgibin = load_object(req->filename))) {
            TP("error compiling it.\n");
            http_error(fd, INTERNAL_ERROR);
            return;
         }
      }
      if (!function_exists("query_content_type", cgibin)) {
         req->bi->content_type = "text/html";
      } else if (catch(req->bi->content_type =
                       (string)cgibin->query_content_type())) {
         TP("error getting content type.\n");
         http_error(fd, INTERNAL_ERROR);
         return;
      }
      /* If it is a post request we parse the body here */
      bit = req->body->buff;
      req->body->data = ([ ]);
      if (bit && strlen(bit) > 0) {
         bits = explode(bit, "&");
         foreach (bit in bits) {
            string name;
            string arg;
            
            if (sscanf(bit, "%s=%s", name, arg) == 2) {
               /* We will ignore errors here... */
               arg = replace_string(arg, "+", " ");
               unmime(ref arg, NOT_PATH);
               unmime(ref name, NOT_PATH);
               req->body->data[name] = arg;
            }
         }
      }
      if (catch(str = (mixed)cgibin->www_request(req->args, fields, req))) {
         TP("error handling request.\n");
         http_error(fd, INTERNAL_ERROR);
         return;
      }
      mod_date = 0;
      if (pointerp(str) && intp(str[0])) {
         if (sizeof(str) < 2) {
            http_error(fd, INTERNAL_ERROR);
            return;
         }
         mod_date = str[0];
         str = str[1];
      }
      if (mapp(str)) {
         location = str["location"];
         mod_date = str["mod_date"];
         req->status_line = str["status"];
         if (stringp(req->status_line)) {
            sscanf(req->status_line, "%d", req->status);
         }
         str = str["response"];
         if (req->status == AUTH_REQUIRED) {
            req->rh->headers_out->header_m["WWW-Authenticate"] =
               sprintf("Basic realm=\"%s\"", auth_name(req));
            req->rh->headers_out->header_k += ({ "WWW-Authenticate" });
         }
      }
      if (str && (stringp(str) || bufferp(str))) {
         send_string(fd, str, mod_date, location, 1);
      } else {
         TP("invalid result from handler.\n");
         http_error(fd, INTERNAL_ERROR);
         return;
      }
      break;
   case "html":
   case "htm":
      /* This should be done only for .shtml files, the s in shtml means
         server parsed. But we'll leave it for now. */
      TP("Reading buffer.\n");
      fname = req->filename;
      
      TP("Found html file.\n");
      req->bi->content_type = "text/html";
      if ((fsize = file_size(req->filename)) > max_file_size) {
         str = ({ });
         for (i = 0; i < fsize; i += max_file_size) {
            str += ({ parse_file_string(read_bytes(req->filename, i,
                                                   max_file_size),
                                        req, fields) });
         }
         send_string_array(fd, str, stat(req->filename)[1], 0, 0);
         str = "";
      } else {
         str = parse_file_string(read_file(req->filename), req, fields);
         send_string(fd, str, stat(req->filename)[1], 0, 0);
      }
      break;
   default:
      send_file(fd, req->filename);
      return;
   }
   if (!str || (!stringp(str) && !bufferp(str))) {
      http_error(fd, NOT_FOUND);
   }
} /* get_file() */
 
private string log_time(int time) {
   mixed *stuff;
   int offset;
   int sign;
   string month;
  
   stuff = localtime(time);
   offset = stuff[LT_GMTOFF];
   if (offset < 0) {
      offset = -offset;
      sign = '+'; /* Looks strange, but it is correct! */
   } else {
      sign = '-';
   }

   month = ({"Jan", "Feb", "Mar", "Apr", "May",
             "Jun","Jul","Aug","Sep","Oct","Nov","Dec"})[stuff[LT_MON]];
   
   return sprintf("[%02d/%03s/%04d:%02d:%02d:%02d %c%02d%02d]",
                  stuff[LT_MDAY],
                  month,
                  stuff[LT_YEAR],
                  stuff[LT_HOUR],
                  stuff[LT_MIN],
                  stuff[LT_SEC],
                  sign,
                  (offset / 3600),
                  (offset % 60));
} /* log_time() */

private void log_request(class http_request req, int len) {
#ifdef LOGGING
   mixed *stuff;
   
   if(!log_str)
      log_str = "";
   if(!cday)
      cday = "";
   
   stuff = localtime(time());
   
   // Flush the log if the day has changed since the last write.
   if(cday != stuff[LT_MDAY] && sizeof(log_str)) {
      write_file("/log/http/log-" + cday + "-" + (stuff[LT_MON]+1) +
                 "-" + stuff[LT_YEAR], log_str);
      log_str = "";
   }
   
   cday = stuff[LT_MDAY];
   
   log_str += (req->sess->name?req->sess->name:req->sess->address) + 
      " - " + (req->user?req->user:"-") + " " + log_time(time()) + " \"" +
      req->the_request + "\" " + req->status + " " +
      len + "\n";
   
   // Flush the log when it's 4k in size, used to reduce disk seeking.
   if(sizeof(log_str) > 4096) {
      /* Do a log of it... */
      write_file("/log/http/log-" + stuff[LT_MDAY] + "-" + (stuff[LT_MON]+1) +
                 "-" + stuff[LT_YEAR], log_str);
      log_str = "";
   }
#endif
} /* log_request() */

int dest_me() {
   unguarded((: save_object, SAVE_FILE :));
   server::dest_me();
} /* dest_me() */

int ltime2sec(mixed *ltime) {
   int year;
   int days;
   int *dayoffset =
      ({306, 337, 0, 31, 61, 92, 122, 153, 184, 214, 245, 275});
   
   year = ltime[LT_YEAR];
   
   if (year < 70 || year >= 138) {
      return 0;
   }
   
   /* shift new year to 1st March in order to make leap year calc easy */
   
   if (ltime[LT_MON] < 2) {
      year--;
   }
   
   /* Find number of days since 1st March 1900 (in the Gregorian calendar). */
   
   days  = year * 365 + year/4 - year/100 + (year/100 + 3)/4;
   days += dayoffset[ltime[LT_MON]] + ltime[LT_MDAY] - 1;
   days -= 25508; /* 1 jan 1970 is 25508 days since 1 mar 1900 */
   
   days = ((days * 24 + ltime[LT_HOUR]) * 60 + ltime[LT_MIN]) * 60 +
      ltime[LT_SEC];
   if (days < 0) {
      return 0;       /* must have overflowed */
   } else {
      return days;           /* must be a valid time */
   }
} /* ltime2sec() */

/*
 * Parses an HTTP date in one of three standard forms:
 *
 *     Sun, 06 Nov 1994 08:49:37 GMT  ; RFC 822, updated by RFC 1123
 *     Sunday, 06-Nov-94 08:49:37 GMT ; RFC 850, obsoleted by RFC 1036
 *     Sun Nov  6 08:49:37 1994       ; ANSI C's asctime() format
 *
 * and returns the number of seconds since 1 Jan 1970 GMT, or
 * 0 if this would be out of range or if the date is invalid.
 */
int parse_date(string date) {
   string month;
   int day;
   int year;
   int hours;
   int minutes;
   int seconds;
   int mon;
   mixed *ltime = allocate(10);

   if (sscanf(date, "%*s, %d %s %d %d:%d:%d %*s", day, month, year, hours,
              minutes, seconds) == 8) {
      ltime[LT_YEAR] = year - 1900;
      if (ltime[LT_YEAR] < 0) {
         return 0;
      }
   } else if (sscanf(date, "%*s, %d-%s-%d %d:%d:%d %*s", day, month, year, 
                     hours, minutes, seconds) == 8) {
      ltime[LT_YEAR] = year;
      if (ltime[LT_YEAR] < 70)
         ltime[LT_YEAR] += 100;
   } else if (sscanf(date, "%*s %s %d %d:%d:%d %d", month, day, hours, minutes,
                   seconds, year) == 7) {
      ltime[LT_YEAR] = year - 1900;
      if (ltime[LT_YEAR] < 0)
         return 0;
   } else if (sscanf(date, "%*s %s  %d %d:%d:%d %d", 
                     month, day, hours, minutes,
                   seconds, year) == 7) {
      ltime[LT_YEAR] = year - 1900;
      if (ltime[LT_YEAR] < 0)
         return 0;
   } else {
      return 0;
   }
   ltime[LT_MDAY] = day;
   if ((ltime[LT_MDAY] <= 0) || (ltime[LT_MDAY] > 31)) {
      return 0;
   }
   ltime[LT_HOUR] = hours;
   ltime[LT_MIN] = minutes;
   ltime[LT_SEC] = seconds;
   if ((ltime[LT_HOUR] > 23) || (ltime[LT_MIN] > 59) || (ltime[LT_SEC] > 61)) {
      return 0;
   }
   mon = member_array(month[0..2], MONTHS);
   if (mon == -1) {
      return 0;
   }
   if ((ltime[LT_MDAY] == 31) && (mon == 3 || mon == 5 || mon == 8 ||
                                  mon == 10)) {
      return 0;
   }
   
   /* February gets special check for leapyear */
   
   if ((mon == 1) && ((ltime[LT_MDAY] > 29) ||
                      ((ltime[LT_MDAY] == 29) && ((ltime[LT_YEAR] & 3) ||
                                                  (((ltime[LT_YEAR] % 100) == 0) && (((ltime[LT_YEAR] % 400) != 100))))))) {
      return 0;
   }
   
   ltime[LT_MON] = mon;
   return ltime2sec(ltime);
} /* parse_date() */
     
string format_date(int x) {
   string str;
   string mon;
   mixed *tm;
   int offset;
       
   if (x<0 || !intp(x)) {
      return "Bad time";
   }
   
   tm = localtime(x);
   offset = tm[LT_GMTOFF];
   tm = localtime(x + offset);
   str = DAYS[tm[LT_WDAY]];
   mon = MONTHS[tm[LT_MON]];
   str = sprintf("%s, %02d %s %d %02d:%02d:%02d GMT", str, tm[LT_MDAY], mon,
                 tm[LT_YEAR], tm[LT_HOUR], tm[LT_MIN], tm[LT_SEC]);
   return str;
} /* format_date() */

protected int set_last_modified(class http_request req, int mtime) {
   string etag;
   string weak_etag;
   string if_modified_since = req->rh->headers_in->header_m["if-modified-since"];
   string if_unmodified     = req->rh->headers_in->header_m["if-unmodified-since"];
   string if_nonematch      = req->rh->headers_in->header_m["if-none-match"];
   string if_match          = req->rh->headers_in->header_m["if-match"];
   int now = time();
   mixed size;

   if (now < 0) {
      now = req->time;
   }
   req->rh->headers_out->header_m["Last-Modified"] =
      format_date((mtime > now) ? now : mtime);
   req->rh->headers_out->header_k += ({ "Last-Modified" });
   if (intp((size = stat(req->filename)[0]))) {
      weak_etag = sprintf("W/\"%x-%x\"", size, mtime);
   } else {
      weak_etag = sprintf("W/\"%x\"", mtime);
   }
   etag = weak_etag[((req->time - mtime > 1) ? 2 : 0)..];
   req->rh->headers_out->header_m["ETag"] = etag;
   req->rh->headers_out->header_k += ({ "ETag" });
   
   if (if_match) {
      if ((if_match[0] != '*') && !find_token(if_match, etag)) {
         return PRECONDITION_FAILED;
      }
   } else if (if_unmodified) {
      int utime = parse_date(if_unmodified);
      
      if (utime && mtime > utime) {
         return PRECONDITION_FAILED;
      }
   }
   if (if_nonematch) {
      if ((if_nonematch[0] == '*') || find_token(if_nonematch, etag)) {
         return (req->method == "GET") ? NOT_MODIFIED : PRECONDITION_FAILED;
      }
   }
   else if (if_modified_since && (req->method == "GET")) {
      int itime = parse_date(if_modified_since);
      
      if ((itime >= mtime) && (itime <= req->time)) {
         return NOT_MODIFIED;
      }
   }
   
   return HTTP_OK;
} /* set_last_modified() */

protected void internal_redirect(class http_request req, string new_uri) {
   class http_request nreq = new(class http_request);

   nreq->sess = req->sess;
   parse_uri(nreq, new_uri);
   nreq->prev = req;
   nreq->the_request = req->the_request;
   nreq->method = req->method;
   nreq->status = req->status;
   nreq->rh = new(class http_request_header);
   nreq->rh->noheaders = req->rh->noheaders;
   nreq->rh->header_only = req->rh->header_only;
   nreq->proto_num = req->proto_num;
   nreq->hostname = req->hostname;
   nreq->time = req->time;
   nreq->numredirect = req->numredirect + 1;
   nreq->rh->headers_in = req->rh->headers_in;
   nreq->rh->headers_out = new(class mime_header, header_m : ([ ]),
                               header_k : ({ }));
   nreq->rh->err_headers_out = new(class mime_header, header_m : ([ ]),
                                   header_k : ({ }));
   nreq->bi = new(class http_body_info);
   Sockets[nreq->sess->fd] = nreq;
   if (nreq->numredirect > 5) {
      /* recursion ... */
      http_error(nreq->sess->fd, INTERNAL_ERROR);
      return;
   }
   handle_request(nreq);
} /* internal_redirect() */

protected void send_string(int fd, string str, int mod_date, string location,
                           int no_length) {
   class http_request req = (class http_request)Sockets[fd];
   int errstatus;
   int rangestatus;

   if (!req) {
      return;
   }
   if (strlen(location) && location[0] == '/' && req->status == 200) {
      req->location = location;
      req->method = "GET";
      map_delete(req->rh->headers_in->header_m, "content-length");
      req->rh->headers_in->header_k -= ({ "Content-Length" });
      internal_redirect(req, location);
      return;
   } else if (location && req->status == 200) {
      req->location = location;
      http_error(fd, MOVED_TEMP);
      return;
   }
   if (!req->rh->noheaders) {
      if (mod_date) {
         if ((errstatus = set_last_modified(req, mod_date)) != HTTP_OK) {
            http_error(fd, errstatus);
            return;
         }
      }
      if (!no_length) {
         req->bi->clength = strlen(str);
         req->rh->headers_out->header_m["Content-Length"] = ""+strlen(str);
         req->rh->headers_out->header_k += ({ "Content-Length" });
      }
      rangestatus = set_byterange(req);
      http_header(req);
   }
   
   /* Log it... */
   log_request(req, strlen(str));
   
   if (!req->rh->header_only) {
      if (req->bi->chunked) {
         eventWrite(fd, sprintf("%x\r\n", strlen(str)));
      }
      if (!rangestatus) {
         eventWrite(fd, str);
      } else {
         int offset;
         int length;
         
         while (each_byterange(req, ref offset, ref length))
            eventWrite(fd, str[offset..offset + length]);
      }     
      if (req->bi->chunked) {
         eventWrite(fd, "\r\n0\r\n\r\n");
      }
   }
} /* send_string() */
     
protected void send_string_array(int fd, string *str, int mod_date,
                           string location, int no_length) {
   class http_request req = (class http_request)Sockets[fd];
   int errstatus;
   int rangestatus;
   string frog;

   if (!req) {
      return; 
   }      
   if (strlen(location) && location[0] == '/' && req->status == 200) {
      req->location = location;
      req->method = "GET";
      map_delete(req->rh->headers_in->header_m, "content-length");
      req->rh->headers_in->header_k -= ({ "Content-Length" });
      internal_redirect(req, location);
      return;
   } else if (location && req->status == 200) {
      req->location = location;
      http_error(fd, MOVED_TEMP);
      return;
   }
   req->bi->clength = 0;
   foreach (frog in str) {
      req->bi->clength += strlen(frog);
   }
   if (!req->rh->noheaders) {
      if (mod_date) {
         if ((errstatus = set_last_modified(req, mod_date)) != HTTP_OK) {
            http_error(fd, errstatus);
            return;
         }
      }
      if (!no_length) {
         
         req->rh->headers_out->header_m["Content-Length"] = ""+req->bi->clength;
         req->rh->headers_out->header_k += ({ "Content-Length" });
      }
      rangestatus = set_byterange(req);
      http_header(req);
   }

   /* Log it... */
   log_request(req, req->bi->clength);
   
   if (!req->rh->header_only) {
      if (req->bi->chunked) {
         eventWrite(fd, sprintf("%x\r\n", req->bi->clength));
      }
      if (!rangestatus) {
         string rabbit;
         
         foreach (rabbit in str) {
            eventWrite(fd, rabbit);
         }
      } else {
         int offset;
         int length;
         int index;
         int off_min;
         
         while (each_byterange(req, ref offset, ref length)) {
            TP("Ummm " + offset + " + " + length + "\n");
            while (offset - off_min > strlen(str[index])) {
               off_min += strlen(str[index]);
               index++;
            }
            while (offset + length - off_min > strlen(str[index])) {
               TP("Sending " + index + " [" + offset + " (" + off_min + ")..]\n");
               eventWrite(fd, str[index][offset - off_min..]);
               off_min += strlen(str[index]);
               index++;
            }
            TP("Sending " + index + " [" + offset + " (" + off_min + ").." + length + "]\n");
            eventWrite(fd, str[index][offset - off_min..offset + length - off_min]);
         }
      }     
      if (req->bi->chunked) {
         eventWrite(fd, "\r\n0\r\n\r\n");
      }
   }
} /* send_string_array() */

protected void send_file(int fd, string str) {
   string *bits;
   class http_request req = (class http_request)Sockets[fd];
   int errstatus;
   int rangestatus;
   int fsize;

   if (!req) {
      return;
   }
   if (req->rh->noheaders) {
      /* Stupid mode, no header */
      eventWrite(fd, str, 1, 1);
      return;
   }
   if ((errstatus = set_last_modified(req, stat(str)[1])) != HTTP_OK) {
      http_error(fd, errstatus);
      return;
   }
   fsize = file_size(str);
   
   req->bi->clength = fsize;
   req->rh->headers_out->header_m["Content-Length"] = ""+fsize;
   req->rh->headers_out->header_k += ({ "Content-Length" });

   /* Log it... */
   log_request(req, req->bi->clength);
   
   bits = explode(req->uri, ".");
   switch (lower_case(bits[<1])) {
   case "html":
   case "htm":
      req->bi->content_type = "text/html";
      break;
   case "wrl":
   case "vrml":
      req->bi->content_type = "x-world/x-vrml";
      break;
   case "jpeg":
   case "jpg":
   case "jpe":
      req->bi->content_type = "image/jpeg";
      break;
   case "gif":
      req->bi->content_type = "image/gif";
      break;
   case "txt":
      req->bi->content_type = "text/plain";
      break;
   case "class":
      req->bi->content_type = "application/octet-stream";
      break;
   case "wav":
      req->bi->content_type = "audio/x-wav";
      break;
   default:
      req->bi->content_type = "text/plain";
      break;
   }
   rangestatus = set_byterange(req);
   http_header(req);
   if (!req->rh->header_only) {
      if (!rangestatus) {
         eventWrite(fd, str, 0, 1);
      } else {
         int offset;
         int length;
         
         while (each_byterange(req, ref offset, ref length)) {
            eventWrite(fd, str, 0, 1, offset, offset + length);
         }
      }
   }
}  /* send_file() */

string query_current_file() {
   return current_file;
} /* query_current_file() */

/* Number of times a file has been accessed */
int query_current_no_reads(string file) {
#ifdef TRACK_PAGE_STATS
   return http_stats[file];
#else
   return 0;
#endif    
} /* query_current_no_reads() */

#ifdef TRACK_PAGE_STATS
void tidy_files_read() {
   int i;
   string tmp;
   
   foreach(tmp in keys(http_stats)) {
      if(tmp[<5..] != ".html" && tmp[<4..] != ".htm" && tmp[<3..] != ".c") {
         map_delete(http_stats, tmp);
      } else if(tmp[0..3] != "/www") {
         map_delete(http_stats, tmp);
      } else if(tmp[0..11] == "/www/autodoc") {
         //      map_delete(http_stats, tmp);
         write("[" + tmp[0..12] + "] " + tmp + "\n");
      }
   }
} /* tidy_files_read() */
    
int query_files_read() {
   return sizeof(keys(http_stats));
} /* query_files_read() */
#endif
