#include <http.h>
#include <player_handler.h>

string this_file;

void create(){
    this_file = __FILE__[strsrch(__FILE__, "/", - 1) + 1..];
}

string www_request(string, mapping args, class http_request req){
  if(!req->user)
    return "Error!";

  if(!PLAYER_HANDLER->test_creator(req->user))
    return "You are not a creator!";

  if(args["file"] && file_size(args["file"]) > 0){
    if(!unguarded((: master()->valid_read($(args["file"]), $(req->user), "cat"):)))
      return "You don't have access to that file!";

    if(req->body->data["body"]){
      mixed stats;
      if(arrayp(stats = stat("/tmp/"+req->user)) && sizeof(stats))
        unguarded((:rm, "/tmp/"+req->user:));
      
      if(arrayp(stats))
        unguarded((:mkdir, "/tmp/"+req->user:));

      unguarded((:write_file, "/tmp/" + req->user + "/" +
                 explode(args["file"], "/")[<1], req->body->data["body"], 1:));

      return "saved in /tmp/" + req->user + "/" + 
        explode(args["file"], "/")[<1];
    }
    return sprintf(@END
<form method=post action="%s?file=%s">
<textarea name="body" rows="20" cols="80" wrap="hard">
END, this_file, args["file"]) + HTTPD->escape_html(read_file(args["file"])) + @END
</textarea><br>
<input type=submit value="save"></form>
END;
  } else {
    return sprintf("No such file, or no file selected."
                   "<hr><form action=\"%s\" METHOD=GET>"
                   "<b>File:</b> <input type=\"text\" name=\"file\" size=62><p>"
                   "<p>"
                   "<input type=\"submit\" value=\"Get file\">"
                   "<input type=\"reset\" value=\"Reset\">"
                   "</form><br><b><a HREF=\"file_browser.c?"
                   "action=/secure/%s,check_file,"
                   "%s,file\">Browse</a>", allocate(3, this_file) ...);
  }
}
      
int check_file(string file){
  return 1;
}
