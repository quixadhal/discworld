/**
 * This is the new bug database html error handler.  It uses the new
 * error handler to do the bug database lookups and avoids using the
 * blocking database calls.
 * @author Pinkfish
 * @started Fri Feb  7 12:31:59 PST 2003
 */
#include <error_handler.h>
#include <player_handler.h>
#include <http.h>
#include <db.h>

#undef SAVE_FILE
#define SAVE_FILE "/save/www/errors"

#define TIMEOUT_PERIOD (30 * 60)

class user_data {
   class error_query query;
   class error_summary* errors;
   int last_touched;
   string direction;
}

private mapping _user_query;

void load_me();

void create() {
   _user_query = ([ ]);
   call_out("flush_old", TIMEOUT_PERIOD);
   load_me();
}

void flush_old() {
   string name;
   class user_data data;

   call_out("flush_old", TIMEOUT_PERIOD);
   foreach (name, data in _user_query) {
      if (time() - data->last_touched > TIMEOUT_PERIOD) {
         map_delete(_user_query, name);
      }
   }
}

void load_me() {
   unguarded( (: restore_object(SAVE_FILE) :) );
}

void save_me() {
   unguarded( (: save_object(SAVE_FILE, 2) :) );
}

private varargs string create_header(class http_request req, string title,
                                     int no_timeout) {
  return "<HTML>\n<HEAD>\n"
    "<TITLE>" + title + "</TITLE>\n"
    "<META http-equiv=\"Refresh\" content=\"" + TIMEOUT_PERIOD +
        ";URL=http://" + req->hostname +
        "/secure/errors_new.c?action=timeout\">"
    "</HEAD>\n<BODY bgcolor=\"#ffffff\" text=\"#000030\" link=\"#4a529c\" "
    "vlink=\"#b57339\">\n<FONT face=\"arial,helvetica\">\n"
    //"<i>(Imagine a pretty picture here to save space)</i>\n<H2>Errors</H2>\n"
    //"<H3><I>When cabbages are just not enough.</I></H3>\n<BR clear=\"left\">\n"
    "<BR>\n";
} /* create_header() */

private string create_footer(class http_request req) {
  return ("/www/footer"->www_function("footer", ([ ]), req))+"</BODY>\n</HTML>\n";
}

private string htmlify_no_address(string str) {
  return replace(str, ({
    "&", "&amp;",
      "<", "&lt;",
      ">", "&gt;",
      "\n", "<BR>\n",
      /* " ", "&nbsp;", */
      }) );
}

/*
 * Makes all the '<' turn into $gt; and the '>' the same.  Turn
 * http addresses into real addresses...
 */
string htmlify(string  str) {
   string *bits;
   string start;
   string end;
   string extra;
   int i;

   str =  htmlify_no_address(str);
   if (strsrch(str, "http:") > 0) {
      bits = explode("#" + str, "http:");
      bits[0] = bits[0][1..];
      for (i = 1; i < sizeof(bits); i++) {
         if (sscanf(bits[i], "%s&nbsp;%s", start, end) == 2) {
            end = "&nbsp;" + end;
            if (sscanf(start, "%s<BR>%s", start, extra) == 2) {
                end = "<BR>" + extra + end;
            }
         } else if (sscanf(bits[i], "%s<BR>%s", start, end) == 2) {
            end = "<BR>" + end;
         } else {
            start = bits[i];
            end = "";
         }
         if (start[<1] == '.') {
            start = start[0..<2];
            end = "." + end;
         }
         bits[i] = "<A href=\"http:" + start + "\">http:" + start + "</A>" +
                     end;
      }
      str = implode(bits, "");
   }

   return str;
}

string html_error(class http_request req, string error) {
   return create_header(req, error) +
          "<h2>" + error + "</h2>" +
          create_footer(req);
}

int find_index(class http_request req, int id) {
   int i;
   class error_summary* errors;

   if (!_user_query[req->user]) {
      return 0;
   }
   _user_query[req->user]->last_touched = time();
   errors = _user_query[req->user]->errors;
   for (i = 0; i < sizeof(errors); i++) {
      if (id == errors[i]->id) {
         return i;
      }
   }
   return 0;
}

int max_index(class http_request req) {
   if (!_user_query[req->user]) {
      return 0;
   }
   _user_query[req->user]->last_touched = time();
   return sizeof(_user_query[req->user]->errors);
}

int id_at(class http_request req, int index) {
   if (!_user_query[req->user] || index < 0 ||
       index >= sizeof(_user_query[req->user]->errors)) {
      return 0;
   }

   _user_query[req->user]->last_touched = time();
   return _user_query[req->user]->errors[index]->id;
}

void finish_query(function callback,
                  class http_request req,
                  int type, mixed data, string query) {
   class error_summary summary;
   string ret;
   string fname;
   string line;

   if (type != DB_SUCCESS) {
      evaluate(callback, html_error(req, "Error: " + data));
      return ;
   }

   ret = create_header(req, "Error list");
   _user_query[req->user]->errors = data;
   save_me();
   if (!sizeof(data)) {
      ret += sprintf("No bugs found.");
   } else {
      line = "";
      if (sizeof(data) == 150) {
         line += "[ <a href=\"error_query.html\">Query</a> | "
                "<a href=\"errors_new.c?action=newoffset&offset=" +
                (_user_query[req->user]->query->bug_offset + 150) + "\">Next 150 bugs</a> ";
      } else {
         line += "[ <a href=\"error_query.html\">Query</a> ";
      }
      if (_user_query[req->user]->query->bug_offset != 0) {
         line += "| <a href=\"errors_new.c?action=newoffset&offset=" +
                (_user_query[req->user]->query->bug_offset - 150) +
                "\">Previous 150 bugs</a> ";
      }
      line += "]";
      ret += "<b>Offset #" + _user_query[req->user]->query->bug_offset +
             "</b> " + line + "<br>";
      ret += "<table width=100%><tr><th>Id</th>"
             "<th>File Name</th>"
             "<th>Status</th>"
             "<th>Date Reported</th><th>Reporter</th>"
             "<th>Assigned to</th>"
             "<th>Category</th><th>Type</th>"
             "<th>Directory</th></tr>";
      foreach (summary in data) {
         fname = explode(summary->filename, "/")[<1];
         ret += "<tr><td>" + 
                 "<a href=\"errors_new.c?action=bug&id=" + summary->id + "\">" +
                  summary->id + "</a></td><td>" +
                 fname + "</td><td>" +
                summary->status + "</td><td>" +
                 ctime(summary->entry_date)[4..15] + "</td><td>" +
                 summary->reporter + "</td><td>" +
                 summary->assigned_to + "</td><td>" +
                summary->category + "</td><td>" + summary->type + "</td><td>" +
                 summary->directory + "</td></tr>";
      }
      ret += "</table>";
      ret += line + "<br>";
   }
   ret += create_footer(req);
   evaluate(callback, ret);
}

int compare_query(class error_query q1, class error_query q2) {
   if (!q1 || !q2) {
      return 0;
   }

   if (q1->dir != q2->dir) {
      return 0;
   }

   if (q1->reporter != q2->reporter) {
      return 0;
   }

   if (q1->fixer != q2->fixer) {
      return 0;
   }

   if (q1->file_name != q2->file_name) {
      return 0;
   }

   if (q1->recursive != q2->recursive) {
      return 0;
   }

   if (q1->assigned_to != q2->assigned_to) {
      return 0;
   }

   if (sizeof(q1->status) != sizeof(q2->status) ||
       sizeof(q1->status - q2->status)) {
      return 0;
   }

   if (sizeof(q1->type) != sizeof(q2->type) ||
       sizeof(q1->type - q2->type)) {
      return 0;
   }

   if (sizeof(q1->category) != sizeof(q2->category) ||
       sizeof(q1->category - q2->category)) {
      return 0;
   }
   return 1;
}

mixed do_query(function callback, mapping args, class http_request req) {
   class error_query query;
   string bing;

   if (args["dir"] == "" && args["filename"] == "" &&
       args["reporter"] == "" && args["assigned"] == "") {
      evaluate(callback, html_error(req, "One of 'dir', 'filename' or 'reporter' must be not blank."));
      return 1;
   }

   query = new(class error_query);
   if (args["dir"] != "") {
      query->dir = args["dir"];
   }

   if (args["recurse"]) {
      query->recursive = 1;
   }

   // Check each of the statuses to see if they were set.
   query->status = ({ });
   foreach (bing in ({ "fixing", "considering", "open", "fixed", "denied" })) {
      if (args[bing]) {
         query->status += ({ upper_case(bing) });
      }
   }

   query->type = ({ });
   foreach (bing in ({ "bug", "typo", "idea", "comment" })) {
      if (args[bing]) {
         query->type += ({ upper_case(bing) });
      }
   }

   query->category = ({ });
   foreach (bing in ({ "room", "help", "object", "command", "general", "ritual", "spell" })) {
      if (args[bing]) {
         query->category += ({ upper_case(bing) });
      }
   }

   if (args["reporter"] != "") {
      query->reporter = args["reporter"];
   }

   if (args["fixer"] != "") {
      query->fixer = args["fixer"];
   }

   if (args["filename"] != "") {
      query->file_name = args["filename"];
   }

   if (args["assigned"] != "") {
      query->assigned_to = args["assigned"];
   }

   if (args["bug_offset"] != "") {
      query->bug_offset = to_int(args["bug_offset"]);
   }

   query->order_by = args["order_by"];

   if (!_user_query[req->user]) {
      _user_query[req->user] = new(class user_data);
   }

   _user_query[req->user]->direction = "next";
   _user_query[req->user]->last_touched = time();
   // Redo the query each time.
   //if (!compare_query(_user_query[req->user]->query, query)) {

      _user_query[req->user]->query = query;
      if (!ERROR_HANDLER->do_query_bug_summary(query, (: finish_query, callback, req :))) {
         evaluate(callback, html_error(req, "Error doing stuff."));
      }
   //} else {
      //finish_query(callback, req, DB_SUCCESS,
                   //_user_query[req->user]->errors, "some select thing");
   //}
   return 1;
}

int do_new_offset(function callback, int offset, class http_request req) {
   class error_query query;

   query = _user_query[req->user]->query;
   if (query->bug_offset == offset) {
      finish_query(callback, req, DB_SUCCESS, _user_query[req->user]->errors, "");
   } else {
      query->bug_offset = offset;
      if (!ERROR_HANDLER->do_query_bug_summary(query, (: finish_query, callback, req :))) {
         evaluate(callback, html_error(req, "Error doing stuff."));
      }
   }
   return 1;
}

void finish_details(function callback, class http_request req,
                    int type, mixed data) {
   string ret;
   string bing;
   class error_details details;
   class error_complete complete;
   class error_comment comment;
   class error_forward forward;
   class error_replies reply;
   int index;
   int max;

   if (type != DB_SUCCESS) {
      evaluate(callback, html_error(req, "Error: " + data));
      return ;
   }

   complete = data[0];
   details = complete->details;
   ret = create_header(req, "Bug #" + details->summary->id);

   ret += "<table width=100%>";
   ret += "<tr><td><b>Date Reported</b>:</td><td>" + ctime(details->summary->entry_date) + "</td></tr>";
   ret += "<tr><td><b>Assigned To</b>:</td><td>" +
          "<table border=0 width=100%><tr><td>" +
          details->summary->assigned_to + "</td><td align=right>"
          "<form action=\"errors_new.c?action=assign&id=" +
          details->summary->id +
          "\" method=get><input type=text size=12 name=\"assigned\">"
          "<input type=hidden name=id value=" + details->summary->id + ">"
          "<input type=hidden name=action value=assign>"
          "<input type=submit name=rabbit value=assign></form>"
          "</td></tr></table>";
   ret += "<tr><td><b>Reporter</b>:</td><td>" + details->summary->reporter + "</td></tr>";
   ret += "<tr><td><b>Directory</b>:</td><td>" + details->summary->directory + "</td></tr>";
   ret += "<tr><td><b>File name</b>:</td><td>" + details->summary->filename + "</td></tr>";
   ret += "<tr><td><b>Category</b>:</td><td>"
          "<table border=0 width=100%><tr><td>" +
          details->summary->category +
          "</td><td align=right><font size=-2>Change to:";
   foreach (bing in ERROR_CATEGORIES) {
      if (bing != details->summary->category) {
         ret += "<a href=\"errors_new.c?action=changecategory&id=" +
                details->summary->id +
                "&category=" + bing +
                "\">" + bing + "</a> ";
      }
   }
   ret += "</i></font></td></tr></table></td></tr>";
   ret += "<tr><td><b>Type</b>:</td><td>"
          "<table border=0 width=100%><tr><td>" +
          details->summary->type +
          "</td><td align=right><font size=-2>Change to:";
   foreach (bing in ERROR_TYPE) {
      if (bing != details->summary->type) {
         ret += "<a href=\"errors_new.c?action=changetype&id=" +
                details->summary->id +
                "&type=" + bing +
                "\">" + bing + "</a> ";
      }
   }
   ret += "</i></font></td></tr></table></td></tr>";
   ret += "<tr><td><b>Status</b>:</td><td>"
          "<table border=0 width=100%><tr><td>" +
          details->summary->status +
          "</td><td align=right><font size=-2>Change to:";
   foreach (bing in ERROR_STATUS) {
      ret += "<a href=\"errors_new.c?action=changestatus&id=" +
             details->summary->id +
             "&status=" + bing +
             "\">" + bing + "</a> ";
   }
   ret += "<br><a href=\"errors_new.c?action=changestatus&id=" +
          details->summary->id +
          "&status=THANKS\">Thanks</a> ";
   ret += "<a href=\"errors_new.c?action=changestatus&id=" +
          details->summary->id +
          "&status=NOTREPRO\">Not reproducable</a> ";
   ret += "<a href=\"errors_new.c?action=changestatus&id=" +
          details->summary->id +
          "&status=NOTPOSSIBLE\">Not possible to fix</a> ";
   ret += "<a href=\"errors_new.c?action=changestatus&id=" +
          details->summary->id +
          "&status=NOINFO\">Not enough Information</a> ";
   ret += "<a href=\"errors_new.c?action=changestatus&id=" +
          details->summary->id +
          "&status=TEMPORARY\">Temporary</a> ";
   ret += "</i></font></td></tr></table></td></tr>";

   if (details->summary->status == "FIXED" ||
       details->summary->status == "DENIED") {
      ret += "<tr><td><b>Fixed By</b></td><td>" + details->fixer +
             " (" + ctime(details->fix_date) + ")</td></tr>";
   }

   if (sizeof(complete->forwards)) {
      ret += "<tr><td><b>Forwards</b>:</td><td>";
      // Print out the forwards.
      foreach (forward in complete->forwards) {
         ret += forward->forwarder + " from " +
                forward->old_directory + " at " +
                ctime(forward->date)[4..15] + "<br>";
      }
      ret += "</td></tr>";
   }

   ret += "</table>";

   ret += "<div align=right><form action=\"errors_new.c?action=forward&id=" +
           details->summary->id + "\" method=get>";
   index = find_index(req, details->summary->id);
   if (index != -1) {
      // Update the summary details, just in case they changed.
      _user_query[req->user]->errors[index] = details->summary;
   }
   max = max_index(req);
   ret += (index + 1) + " of " + max + " ";
   ret += "<b>[ <a href=\"errors_new.c?action=index\">Index</a> | ";
   if (index + 1 < max) {
      ret += "<a href=\"errors_new.c?action=next&id=" + id_at(req, index + 1) + "\">Next</a> | ";
   }
   if (index > 0) {
      ret += "<a href=\"errors_new.c?action=prev&id=" + id_at(req, index - 1) + "\">Prev</a> | ";
   }
   ret += "<a href=\"errors_new.c?action=comment&id=" + details->summary->id + "\">Add Comment</a> | ";
   ret += "<a href=\"error_query.html\">Query</a> ]</b><br>";
   ret += "\n<input type=text size=30 name=\"forward\">"
          "<input type=hidden name=id value=" + details->summary->id + ">"
          "<input type=hidden name=action value=forward>"
          "<input type=submit name=rabbit value=forward></form></div>\n";
   ret += htmlify(details->report);
   ret += htmlify(details->runtime);

   ret += "<blockquote>";
   // Show the error replies before the comments.
   foreach (reply in complete->replies) {
      ret += "<p><b>Date Sent:</b> " + ctime(reply->senddate)[4..15] + 
             "<br><b>From:</b> " + reply->sender +
             "<br><b>To:</b> " + reply->recipient +
             "<br><b>Subject:</b> " + reply->subject + "<br>" +
             htmlify(reply->message);
   }
   if (sizeof(complete->replies) && sizeof(complete->comments)) {
      ret += "<hr width=30%>";
   }
   foreach (comment in complete->comments) {
      ret += "<p><b>Comment by " + comment->commenter + " at " +
             ctime(comment->date)[4..15] + "</b><br>" +
             htmlify(comment->comment);
   }
   ret += "</blockquote>";
   ret += create_footer(req);
   evaluate(callback, ret);
}

int do_bug_details(function callback, mapping args, class http_request req,
                   int update_position) {
   int index;
   int max;
   int val;

   if (update_position) {
      index = find_index(req, to_int(args["id"]));
      max = max_index(req);
      val = to_int(args["id"]);
      switch (_user_query[req->user]->direction) {
      case "prev" :
         if (index - 1 >= 0) {
            val = id_at(req, index - 1);
         }
      case "next" :
      default :
         if (index + 1 < max) {
            val = id_at(req, index + 1);
         }
         break;
      }
tell_creator("pinkfish", "%O %O %O\n", args["id"], index, max);
      args["id"] = val + "";
   }

   if (!ERROR_HANDLER->do_query_bug_details(to_int(args["id"]), (: finish_details, callback, req :))) {
      evaluate(callback, html_error(req, "Error doing stuff."));
   }
   return 1;
}

int do_change_type(function callback, mapping args, class http_request req) {
   if (!ERROR_HANDLER->do_change_type(to_int(args["id"]), args["type"],
                (: do_bug_details($(callback), $(args), $(req), 0) :))) {
      evaluate(callback, html_error(req, "Error doing stuff."));
   }
   return 1;
}

int do_change_category(function callback, mapping args, class http_request req) {
   if (!ERROR_HANDLER->do_change_category(to_int(args["id"]), args["category"],
                (: do_bug_details($(callback), $(args), $(req), 0) :))) {
      evaluate(callback, html_error(req, "Error doing stuff."));
   }
   return 1;
}

int do_forward(function callback, mapping args, class http_request req) {
   if (!ERROR_HANDLER->do_forward(to_int(args["id"]), req->user,
                args["forward"],
                (: do_bug_details($(callback), $(args), $(req), 1) :))) {
      evaluate(callback, html_error(req, "Cannot forward to a non-existant "
                                   "directory or to '/'"));
   }
   return 1;
}

int do_assign(function callback, mapping args, class http_request req) {
   if (!ERROR_HANDLER->do_assign_bug_to(to_int(args["id"]),
                args["assigned"],
                (: do_bug_details($(callback), $(args), $(req), 1) :))) {
      evaluate(callback, html_error(req, "Error doing stuff."));
   }
   return 1;
}

void finish_change_status_details(function callback, mapping args,
                                  class http_request req,
                                  int type, mixed data) {
   class error_complete complete;
   class error_details details;
   string str;
   string internal_status;

   if (type != DB_SUCCESS) {
      evaluate(callback, html_error(req, "Error: " + data));
      return ;
   }

   complete = data[0];
   details = complete->details;

   internal_status = args["status"];
   if (internal_status == "TEMPORARY") {
      internal_status = "FIXED";
   }
   if (internal_status == "NOTREPRO" || internal_status == "NOINFO" ||
       internal_status == "NOTPOSSIBLE" || internal_status == "THANKS") {
      internal_status = "DENIED";
   }

   str = create_header(req, "Change status of #" + details->summary->id);
   str += "New status: " + internal_status;
   str += "<form action=\"errors_new.c?action=changestatusmessage\" method=post>\n";
   switch (args["status"]) {
   case "TEMPORARY" :
      str += "<input type=text size=60 name=subject value=\"" +
             lower_case(details->summary->category) + " " +
             lower_case(details->summary->type) +
             " bug #" + args["id"] + " was a temporary bug\"><br>";
      break;
   case "NOINFO" :
      str += "<input type=text size=60 name=subject value=\"Not enough " +
             "information on " +
             lower_case(details->summary->category) + " " +
             lower_case(details->summary->type) +
             " bug #" + args["id"] + "\"><br>";
      break;
   case "NOTPOSSIBLE" :
      str += "<input type=text size=60 name=subject value=\"" +
             lower_case(details->summary->category) + " " +
             lower_case(details->summary->type) +
             " bug #" + args["id"] + " was not possible to fix\"><br>";
      break;
   case "NOTREPRO" :
      str += "<input type=text size=60 name=subject value=\"" +
             lower_case(details->summary->category) + " " +
             lower_case(details->summary->type) +
             " bug #" + args["id"] + " was not reproducable\"><br>";
      break;
   case "THANKS" :
      str += "<input type=text size=60 name=subject value=\"Thanks for " +
             lower_case(details->summary->category) + " " +
             lower_case(details->summary->type) +
             " bug #" + args["id"] + "\"><br>";
      break;
   default :
      str += "<input type=text size=60 name=subject value=\"Change status of " +
             lower_case(details->summary->category) + " " +
             lower_case(details->summary->type) +
             " bug #" + args["id"] + " to " +
             lower_case(args["status"]) + "\"><br>";
   }
   str += "<textarea name=message cols=80 rows=20>";
   switch (args["status"]) {
   case "FIXED" :
      str += "Thank you for your " + lower_case(complete->details->summary->type) +
             " report, this " + lower_case(complete->details->summary->type) +
             " has now been fixed.\n\nGood luck!\n" +
             capitalize(req->user) + "\n";
      break;
   case "DENIED" :
      str += "Thank you for your " + lower_case(complete->details->summary->type) +
             " report."
             "\n\nThanks!\n" +
             capitalize(req->user) + "\n\n";
      break;
   case "THANKS" :
      str += "Thank you for your " + lower_case(complete->details->summary->type) +
             " report.\n\nThanks!\n" +
             capitalize(req->user) + "\n\n";
      break;
   case "NOTREPRO" :
      str += "The " + lower_case(complete->details->summary->type) +
             " report was unable to be reproduced.  Please submit a new bug "
             "report with more specific details if it happens again.\n\n"
             "Thanks!\n" +
             capitalize(req->user) + "\n\n";
      break;
   case "NOINFO" :
      str += "The " + lower_case(complete->details->summary->type) +
             " report did not contain enough information to fix the bug.  "
             "Please make a new report with more details.\n\n"
             "Good luck!\n" +
             capitalize(req->user) + "\n\n";
      break;
   case "NOTPOSSIBLE" :
      str += "Sadly, this " + lower_case(complete->details->summary->type) +
             " is cannot be fixed due to coding "
             "constraints.\n\n"
             "Good luck!\n" +
             capitalize(req->user) + "\n\n";
      break;
   case "TEMPORARY" :
      str += "The " + lower_case(complete->details->summary->type) +
             " in question was a temporary problem in the game and has now "
             "been solved.  Please report this if it happens again.\n\n"
             "Good luck!\n" +
             capitalize(req->user) + "\n\n";
      break;
   }
   str += "> Bug #" + details->summary->id + " reported at " +
          ctime(details->summary->entry_date)[4..15] + "\n>\n";
   str += "> " + replace_string(htmlify_no_address(sprintf("%79-=s", details->report)), "<BR>\n", "\n> ");
   str += "</textarea>";
   str += "<br>";
   str += "<input type=hidden name=status value=\"" + internal_status + "\">";
   str += "<input type=hidden name=id value=\"" + details->summary->id + "\">";
   str += "<input type=submit name=submit value=\"Change Status\">";
   str += "<input type=checkbox name=nomail value=yes> Don't send mail<br>";
   str += "</form>";
   str += create_footer(req);
   evaluate(callback, str);
}

int do_change_status(function callback, mapping args, class http_request req) {
   if (!ERROR_HANDLER->do_query_bug_details(to_int(args["id"]), (: finish_change_status_details, callback, args, req :))) {
      evaluate(callback, html_error(req, "Error doing stuff."));
   }
   return 1;
}

int do_change_status_message(function callback, mapping args, class http_request req) {
   int nomail;

   args = req->body->data;
   if (args["nomail"] == "yes") {
      nomail = 1;
   }

   args["message"] = replace_string(args["message"], "\r\n", "\n");

   if (!ERROR_HANDLER->do_change_status(to_int(args["id"]), args["status"],
                                  nomail, args["subject"],
                                  req->user, args["message"],
                (: do_bug_details($(callback), $(args), $(req), 1) :))) {
      evaluate(callback, html_error(req, "Error doing stuff."));
   }

   return 1;
}

string do_comment(mapping args, class http_request req) {
   string str;

   str = create_header(req, "Comment on #" + args["id"]);
   str += "<h2>Comment to add to #" + args["id"] + "</h2>"
          "<form action=\"errors_new.c?action=commentmessage&id=" + args["id"] +
         "\" method=post>";
   str += "<textarea name=message cols=80 rows=20>"
          "</textarea><br>"
          "<input type=submit name=\"Add Comment\" value=\"Add Comment\">"
          "</form>";
   str += create_footer(req);
   return str;
}

int do_comment_message(function callback, mapping args, class http_request req) {
   string mess;

   mess = replace_string(req->body->data["message"], "\r\n", "\n");
   if (!ERROR_HANDLER->do_comment(to_int(args["id"]), req->user,
                                  req->body->data["message"],
                (: do_bug_details($(callback), $(args), $(req), 0) :))) {
      evaluate(callback, html_error(req, "Error doing stuff."));
   }
   return 1;
}

mixed www_delayed(function callback, string str, mapping args,
    class http_request req ) {
   if (!PLAYER_HANDLER->test_creator(req->user)) {
      return 0;
   }
   switch (args["action"]) {
   case "query" :
      return do_query(callback, args, req);
   case "next" :
   case "prev" :
      _user_query[req->user]->direction = args["action"];
   case "bug" :
      return do_bug_details(callback, args, req, 0);
   case "index" :
      finish_query(callback, req, DB_SUCCESS,
                   _user_query[req->user]->errors, "some select thing");
      return 1;
   case "changetype" :
      return do_change_type(callback, args, req);
   case "changecategory" :
      return do_change_category(callback, args, req);
   case "changestatus" :
      return do_change_status(callback, args, req);
   case "changestatusmessage" :
      return do_change_status_message(callback, args, req);
   case "forward" :
      return do_forward(callback, args, req);
   case "assign" :
      return do_assign(callback, args, req);
   case "commentmessage" :
      return do_comment_message(callback, args, req);
   case "newoffset" :
      return do_new_offset(callback, to_int(args["offset"]), req);
   }
   return 0;
}

mixed www_request(string str, mapping args, class http_request req) {
   if (!PLAYER_HANDLER->test_creator(req->user)) {
      return 0;
   }
   switch (args["action"]) {
   case "timeout" :
      map_delete(_user_query, req->user);
      str = create_header(req, "Session timed out.", 1);
      str += "<h2>Your session timed out</h2>"
             "<a href=\"error_query.html\">Query page</a>";
      return str + create_footer(req);
   case "comment" :
      return do_comment(args, req);
   }
}

mapping query_dynamic_auto_load() {
   return _user_query;
}

void init_dynamic_arg(mapping details) {
   if (mapp(details)) {
      _user_query = details;
   }
}
