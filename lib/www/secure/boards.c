#include <http.h>

string www_request(string str, mapping args, class http_request req) {
  return "/www/boards"->www_request(str, args, req);
} /* www_request() */
