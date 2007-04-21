/**
 * This lists all the replies to bugs and lets you go through and delete/read
 * them.
 * @author Pinkfish
 * @started Mon Feb 17 14:56:20 PST 2003
 */
#include <http.h>
#include <error_handler.h>
#include <player_handler.h>
#include <db.h>

#define TIMEOUT_PERIOD (6 * 60 * 60)

private mapping _replies;

void create() {
   _replies = ([ ]);
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

string html_error(class http_request req, string error) {
   return create_header(req, error) +
          "<h2>" + error + "</h2>" +
          create_footer(req);
}

private string htmlify(string str) {
  return replace(str, ({
    "&", "&amp;",
      "<", "&lt;",
      ">", "&gt;",
      "\n", "<BR>\n",
      /* " ", "&nbsp;", */
      }) );
}

string list_replies(string user) {
   class error_replies reply;
   string ret;

   if (!sizeof(_replies[user])) {
      ret = "<font size=-1>[ <b><a href=\"bug_replies.c\">Refresh</a></b> ]<p>";
      return ret + "No bug replies.<br>";
   }

   ret = "<ol>";
   foreach (reply in _replies[user]) {
      ret += "<li>" + ctime(reply->senddate)[4..15] + " " +
             (reply->status != "READ" ? "<B>" + reply->status + "</b> " : "") +
             capitalize(reply->sender) +
             " '<a href=\"bug_replies.c?action=read&id=" + reply->id + "\">" +
             reply->subject + "</a>'";
      if (PLAYER_HANDLER->test_creator(user)) {
         ret += "<font size=-2>(bug <a href=\"errors_new.c?action=bug&id=" +
                reply->bug_id + "\">#" + reply->bug_id + "</a>)</font>";
      }
   }
   ret += "</ol>";
   ret += "<font size=-1>[ <b><a href=\"bug_replies.c\">Refresh</a></b> | " +
          "<b><a href=\"bug_replies.c?action=deleteread\">Delete all "
          "read</a></b> ]<p>";
   return ret;
}

string reply_details(class error_replies reply, class http_request req) {
   string ret;
   int i;

   ret = "<table>" +
         (PLAYER_HANDLER->test_creator(req->user)?
          "<tr><td><b>Bug #</b></td><td>"
             "<a href=\"errors_new.c?action=bug&id=" +
             reply->bug_id + "\">#" + reply->bug_id + "</a>"
         "</td></tr>":"") +
         "<tr><td><b>Date Sent:</b></td><td>" +
         ctime(reply->senddate) + "</td></tr>"
         "<tr><td><b>Status:</b></td><td>" +
         reply->status + "</td></tr>"
         "<tr><td><b>From:</b></td><td>" +
         reply->sender + "</td></tr>"
         "<tr><td><b>To:</b></td><td>" +
         reply->recipient + "</td></tr>"
         "<tr><td><b>Subject:</b></td><td>" +
         reply->subject + "</td></tr>"
         "</td></tr></table>";
   ret += "<p><div align=right><font size=-1>";
   if (reply->status != "DELETED") {
      ret += "<p><b>[ <a href=\"bug_replies.c?action=reply&id=" + reply->id +
                  "\">Reply</a> | "
                  "<a href=\"bug_replies.c?action=list\">List</a> | "
                  "<a href=\"bug_replies.c?action=delete&id=" + reply->id +
                  "\">Delete</a> ";
   } else {
      ret += "<p><b>[ <a href=\"bug_replies.c?action=reply&id=" + reply->id +
                  "\">Reply</a> | "
                  "<a href=\"bug_replies.c?action=list\">List</a> | "
                  "<a href=\"bug_replies.c?action=undelete&id=" + reply->id +
                  "\">Undelete</a>";
   }
   for (i = 0; i < sizeof(_replies[req->user]); i++) {
      if (((class error_replies)_replies[req->user][i])->id == reply->id) {
         if (i > 0) {
            ret += "| <a href=\"bug_replies.c?action=read&id=" +
      ((class error_replies)_replies[req->user][i - 1])->id + "\">Previous</a>";
         }
         if (i < sizeof(_replies[req->user]) - 1) {
            ret += "| <a href=\"bug_replies.c?action=read&id=" +
         ((class error_replies)_replies[req->user][i + 1])->id + "\">Next</a>";
         }
      }
   }
   ret += " ]</b></font></div><p>" + htmlify(reply->message);
   if (reply->status == "NEW") {
      ERROR_HANDLER->do_status_error_reply(reply->id, "READ", (: 1 :));
   }
   return ret;
}

void finish_replies_list(function callback, class http_request req,
                         int type, class error_replies* data) {
   if (type != DB_SUCCESS) {
      evaluate(callback, html_error(req, "Unable to get bug replies.\n"));
   } else {
      _replies[req->user] = data;
      evaluate(callback, create_header(req, "Bug Reply List") +
                         "<h2>Your bug reply list</h2>" +
                         list_replies(req->user) +
                         create_footer(req));
   }
}

string get_reply_details(int id, class http_request req) {
   class error_replies reply;

   foreach (reply in _replies[req->user]) {
      if (reply->id == id) {
         return create_header(req, "Bug reply #" + reply->id) +
                reply_details(reply, req) +
                create_footer(req);
      }
   }
   return html_error(req, "Unable to find the bug reply.\n");
}

string delete_bug_reply(int id, class http_request req) {
   class error_replies reply;

   foreach (reply in _replies[req->user]) {
      if (reply->id == id) {
         reply->status = "DELETED";
         ERROR_HANDLER->do_status_error_reply(reply->id, "DELETED", (: 1 :));
         return create_header(req, "Deleted bug reply #" + reply->id) +
                reply_details(reply, req) +
                create_footer(req);
      }
   }
   return html_error(req, "Unable to find the bug reply.\n");
}

string undelete_bug_reply(int id, class http_request req) {
   class error_replies reply;

   foreach (reply in _replies[req->user]) {
      if (reply->id == id) {
         reply->status = "READ";
         ERROR_HANDLER->do_status_error_reply(reply->id, "READ", (: 1 :));
         return create_header(req, "Deleted bug reply #" + reply->id) +
                reply_details(reply, req) +
                create_footer(req);
      }
   }
   return html_error(req, "Unable to find the bug reply.\n");
}

string reply_to_bug_reply(int id, class http_request req) {
   string ret;
   class error_replies reply;

   foreach (reply in _replies[req->user]) {
      if (reply->id == id) {
         ret = create_header(req, "Deleted bug reply #" + reply->id);
         ret = "<form action=\"bug_replies.c?action=replymessage&id=" + reply->id +
               "\" method=post>"
                "Subject: <input name=subject type=text size=60 value=\"Re: " +
                htmlify(reply->subject) + "\">\n"
                "<textarea name=message rows=15 cols=80>" +
                "> " + htmlify(implode(map(explode(reply->message, "\n"),
                                   (: "> " + $1 :)), "\n")) +
                "</textarea>"
                "<br><input type=submit name=rabbit value=\"Send Message\">" +
                create_footer(req);
         return ret;
      }
   }
   return html_error(req, "Unable to find the bug reply.\n");
}

string reply_message_bug_reply(int id, class http_request req) {
   class error_replies reply;

   foreach (reply in _replies[req->user]) {
      if (reply->id == id) {
         reply->status = "READ";
         ERROR_HANDLER->do_add_error_reply(reply->bug_id, req->user,
                     reply->sender, req->body->data["subject"],
                     req->body->data["message"]);
         return create_header(req, "Deleted bug reply #" + reply->id) +
                "<h2>Reply sent</h2>" +
                reply_details(reply, req) +
                create_footer(req);
      }
   }
   return html_error(req, "Unable to find the bug reply.\n");
}

string delete_all_read(class http_request req) {
   string str;

   str = create_header(req, "Delete all read.");
   str += "<b>Are you sure you want to delete all the read replies?</b><p>";
   str += "<a href=\"bug_replies.c?action=deleteallreadconfirm\">Yes</a> "
          "<a href=\"bug_replies.c\">No</a>";
   return str + create_footer(req);
}

string delete_all_confirmed(class http_request req) {
   class error_replies reply;

   foreach (reply in _replies[req->user]) {
      if (reply->status == "READ") {
         // Do the delete.
         reply->status = "DELETED";
         ERROR_HANDLER->do_status_error_reply(reply->id, "DELETED", (: 1 :));
      }
   }
   return create_header(req, "Deleted all read") +
                         list_replies(req->user) +
                         create_footer(req);
}

mixed www_delayed(function callback, string str, mapping args,
    class http_request req ) {
   if (!args["action"]) {
      // List the current replies.
      ERROR_HANDLER->do_error_replies(req->user, 0,
                      (: finish_replies_list($(callback), $(req),
                                    $1, $2) :));
      return 1;
   }
}

string www_request(string str, mapping args, class http_request req) {
   switch (args["action"]) {
   case "delete" :
      return delete_bug_reply(to_int(args["id"]), req);
   case "undelete" :
      return undelete_bug_reply(to_int(args["id"]), req);
   case "read" :
      return get_reply_details(to_int(args["id"]), req);
   case "reply" :
      return reply_to_bug_reply(to_int(args["id"]), req);
   case "replymessage" :
      return reply_message_bug_reply(to_int(args["id"]), req);
   case "deleteallread" :
      return delete_all_read(req);
   case "deleteallreadconfirm" :
      return delete_all_confirmed(req);
   case "list" :
      return create_header(req, "Bug Reply List") +
                         list_replies(req->user) +
                         create_footer(req);
   default :
      return html_error(req, "Unknown action.\n");
   }
}

mapping query_dynamic_auto_load() {
   return ([ "replies" : _replies ]);
}

void init_dynamic_arg(mapping map) {
   _replies = map["replies"];
   if (!_replies) {
      _replies = ([ ]);
   }
}
