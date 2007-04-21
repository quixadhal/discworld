#include <http.h>

string www_function(string fname, mapping fields, class http_request req,
                    int dont_inc_date, int no_hr) {
  string ret;
  mixed *stuff;

  if (!no_hr) {
    ret = "<hr>\n";
  } else {
    ret = "";
  }
  ret += "<center>\
<font size=\"-1\"><a href=\"http://discworld.imaginary.com/login.html\">Discworld MUD</a>'s \
distribution world wide web pages.<br>\
brought to you by<br>\
<strong>Cut Me Own Throat Dibbler's \
<a href=\"http://discworld.imaginary.com/sausages.html\">Sensational Sausages</a>;\
&nbsp;buy one while they are hot.</strong><br>";
  if (fname == "footer") {
    fname = HTTPD->query_current_file();
  }
  if(fname[<2..] != ".c") {
    stuff = stat(fname);
  }

  if (stuff && sizeof ( stuff ) && !dont_inc_date && intp(stuff[1])) {
    ret += "File last modified: "+HTTPD->format_date(stuff[1]);
  }
  if (req && req->user) {
     ret += "<br><a href=\"/secure\">Secure section</a>, logged on as " + capitalize(req->user) + "\n";
  }
  ret += "<br>\n<hr>\
Lost?  Try Discworld's <a href=\"http://discworld.imaginary.com/\">home page</a>.\
</center>";
  return ret;
} /* www_function() */

