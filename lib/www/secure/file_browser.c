/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: file_browser.c,v 1.4 2003/03/21 04:50:28 ceres Exp $
 *
 *
 */

//Source browser by Wodan
#include <http.h>
#include <assert.h>
#include <player_handler.h>

string this_file;
string *action_info, action;

void create(){
  this_file = __FILE__[strsrch(__FILE__, "/", - 1) + 1..];
}

int done_dirs;

string make_dir_list(string done, mixed *file_info){
  int size;
  string file = file_info[1];
  if((size = file_info[0]) == -1)
    return done;
  if(size == 0)
    return sprintf("%s<B><A HREF=\"%s?path=%s/&action=%s\">%s/</A></B><br> ", done,
                 this_file, file, action, file);

  if(!done_dirs){
    done += "<br>";
    done_dirs = 1;
  }

  if(!call_other(action_info[0], action_info[1], file))
    return done;
  
  return sprintf("%s<B><A HREF=\"%s?%s=%s\"><strong>%s</strong></A></B><br> ", done,
                 action_info[2], action_info[3], file, file);

}

mixed *get_full_dir(string path){
  mixed *ret = map(get_dir(path, -1), (:({$1[1] >= 0, $(path) + $1[0]}):));
  ret = sort_array(ret, function(mixed *a, mixed *b){
    if(a[0] < b[0])
      return -1;
    if(a[0] > b[0])
      return 1;
    if(a[1] < b[1])
      return -1;
    if(a[1] > b[1])
      return 1;
    return 0;
  });
  return ret;
}

string www_request(string, mapping args, class http_request req){
  string ret;

  if(!args["path"])
    args["path"] = "/";
  
  if(!(req->user && args["action"]))
    return "Something bad is wrong.";

  action_info = explode(args["action"], ",");
  action_info[0] = "/www" + action_info[0];
  action = args["action"];

  if(!PLAYER_HANDLER->test_creator(req->user))
    return "You are not a creator!";
  
  done_dirs = 0;

  if(!unguarded((:master()->valid_read($(args["path"]), $(req->user), "cat"):)))
    return sprintf("You don't have read permission for %s", args["path"]);

  ret = sprintf(@END
<html><head><title> Browsing %s</title></head>
<body bgcolor="#ffffff" TEXT="#000030" LINK="#4a529c" VLINK="#b57339">
<strong>Path:</strong> %s<BR><BR>
%s<BR>
%s<br>
<i><a
  href="mailto:wodan.discworld@discworld.imaginary.com">wodan.discworld@discworld.imaginary.com</font></a></i><br>
</body>
</html>

END
  , args["path"], args["path"]
  , unguarded((: implode(get_full_dir($(args["path"])), 
       (:make_dir_list:), "<br>"):))
  , "/www/footer"->www_function("footer", ([ ]), req, 0, 1));
  return ret;
}



