/**
 * This is a handler to keep track of errors, make adding them and querying
 * them easier.
 * @author Pinkfish
 * @started Mon Jun 17 17:28:04 PDT 2002
 */
#include <db.h>
#include <error_handler.h>

#define SAVE_FILE "/save/handlers/error_handler"

#define ERROR_SUMMARY_FIELDS "id, entrydate, filename, type, category, name, reporter, status, directory"
#define ERROR_DETAILS_FIELDS ERROR_SUMMARY_FIELDS \
                      ", report, runtime"

// 4 weeks.
#define ERROR_REPLY_TIMEOUT (60 * 60 * 24 * 7 * 4)

void finish_update(int type, mixed data, function finished, string query);

private string _db;
private string _user;
private string _password;

void create() {
   unguarded( (: restore_object(SAVE_FILE) :) );
   // Do a call on the handler to clean up the old error replies.
   DB_HANDLER->make_sql_request(_db, _user, _password, 
                 "delete from error_replies where senddate < " +
                 (time() - ERROR_REPLY_TIMEOUT), (: 1 :));
}

/**
 * This method sets up the access permissions to use for accessing
 * the database.
 * @param user the user to use
 * @param db the database to use
 * @param password the password to use
 */
protected void setup_error_handler(string user, string db, string password) {
   _user = user;
   _db = db;
   _password = password;
   save_object(SAVE_FILE);
}

/**
 * This method builds an sql query from the input query.
 * @param query the input query
 * @return an sql string query
 */
protected string build_sql_query(string select_rows, class error_query query) {
   string* restrict;
   string name;
   string tmp;

   restrict = ({ });
   if (query->id != 0) {
      restrict += ({ "id = " + query->id });
   }
   if (query->dir) {
      if (query->dir[<1] == '/') {
         query->dir = query->dir[0..<2];
      }
      if (query->recursive) {
         restrict += ({ "directory like '" + query->dir + "%'" });
      } else {
         restrict += ({ "directory = '" + query->dir + "'" });
      }
   }

   if (query->assigned_to) {
      restrict += ({ "AssignedTo = '" + query->assigned_to + "'" });
   }

   if (query->reporter) {
      restrict += ({ "Reporter = '" + query->reporter + "'" });
   }

   if (query->file_name) {
      restrict += ({ "FileName = '" + query->file_name + "'" });
   }

   if (sizeof(query->type)) {
      tmp = "Type in (";
      foreach (name in query->type) {
         tmp += "'" + name + "',";
      }
      tmp = tmp[0..<2] + ")";
      restrict += ({ tmp });
   }

   if (sizeof(query->status)) {
      tmp = "Status in (";
      foreach (name in query->status) {
         tmp += "'" + name + "',";
      }
      tmp = tmp[0..<2] + ")";
      restrict += ({ tmp });
   }

   if (sizeof(query->category)) {
      tmp = "Category in (";
      foreach (name in query->category) {
         tmp += "'" + name + "',";
      }
      tmp = tmp[0..<2] + ")";
      restrict += ({ tmp });
   }
   if (sizeof(restrict)) {
      tmp = "select " + lower_case(select_rows) + " from errors where " +
             implode(restrict, " and ");
      switch (query->order_by) {
      case ERROR_ORDER_BY_FILENAME :
         tmp += " order by filename, entrydate";
         break;
      case ERROR_ORDER_BY_DATE :
         tmp += " order by entrydate";
         break;
      case ERROR_ORDER_BY_REPORTER :
         tmp += " order by reporter, entrydate";
         break;
      case ERROR_ORDER_BY_DIRECTORY :
      default :
         tmp += " order by directory, filename, entrydate";
         break;
      }
      return tmp + " limit 150";
   }
   return 0;
}

/**
 * This method builds an sql update from the given inputs.
 * @param id the id of the bug to update
 * @param field the field to update
 * @param value the new value of the field
 */
string build_sql_update(int id, string field, mixed value) {
   string ret;

   ret = "update errors set " + field + " = ";
   if (stringp(value)) {
      ret += "'" + value + "'";
   } else {
      ret += "" + value;
   }
   ret += " where id = " + id;
   return ret;
}

/**
 * This method creates the error forward from the input mapping.
 * @param map the mapping to create the forward from
 * @return the error forward class
 */
class error_forward create_error_forward(mapping map) {
   class error_forward newy;

   newy = new(class error_forward);
   newy->date = map["forwarddate"];
   newy->forwarder = map["forwarder"];
   newy->old_directory = map["olddirectory"];
   return newy;
}

/**
 * This method creates the error comment from the input mapping.
 * @param map the mapping to create the comment from
 * @return the error comment class
 */
class error_comment create_error_comment(mapping map) {
   class error_comment newy;

   newy = new(class error_comment);
   newy->date = map["commentdate"];
   newy->commenter = map["commenter"];
   newy->comment = map["comment"];
   return newy;
}

/**
 * This method creates the error summary from the input mapping.
 * @param map the mapping to create the summary from
 * @return the error summary class
 */
class error_summary create_error_summary(mapping map) {
   class error_summary newy;

   newy = new(class error_summary);
   newy->id = map["id"];
   newy->entry_date = map["entrydate"];
   newy->filename = map["filename"];
   newy->category = map["category"];
   newy->assigned_to = map["assignedto"];
   newy->reporter = map["reporter"];
   newy->status = map["status"];
   newy->type = map["type"];
   newy->directory = map["directory"];
   return newy;
}

/**
 * This method creates the error details from the return data.
 */
class error_details create_error_details(mapping map) {
   class error_details newy;

   newy = new(class error_details);
   newy->summary = create_error_summary(map);
   newy->fix_date = map["fixdate"];
   newy->fixer = map["fixer"];
   newy->report = map["report"];
   newy->runtime = map["runtime"];
   return newy;
}

/**
 * This method creates the complete error class from the error details
 * passed in.
 * @param details the details to make the complete error from
 */
class error_complete create_error_complete(class error_details details) {
   class error_complete newy;

   newy = new(class error_complete);
   newy->details = details;
   newy->comments = ({ });
   newy->forwards = ({ });
   newy->replies = ({ });
   return newy;
}

/**
 * This method creates the error reply class from the sql query.
 * @param map the mapping to convert to the class
 * @return the nice new error reply class
 */
class error_replies create_error_replies(mapping map) {
   class error_replies reply;

   reply = new(class error_replies);
   reply->id = map["id"];
   reply->bug_id = map["bug_id"];
   reply->sender = map["sender"];
   reply->status = map["status"];
   reply->recipient = map["recipient"];
   reply->senddate = map["senddate"];
   reply->subject = map["subject"];
   reply->message = map["message"];
   return reply;
}

/**
 * This method finishes a summary based query.
 * @param type the type returned (error etc)
 * @param data the data returned
 * @param finished the function to call on finish
 */
void finish_summary(int type, mixed data, function finished, string query) {
   class error_summary* errors;
   mapping map;

   if (type != DB_SUCCESS) {
      evaluate(finished, type, data, query);
   } else {
      errors = ({ });
      foreach (map in data) {
         errors += ({ create_error_summary(map) });
      }
      evaluate(finished, type, errors, query);
   }
}

/**
 * This method updates the comments based on the query results.
 * @param type type type
 * @param fdata the return data from the query
 * @param function finsiished the finished function
 * @param errors the errors results we are updating
 * @param query the ids list we are using
 */
void finish_forwards(int type, mixed data, function finished,
                     class error_complete* errors, string query) {
   mapping map;
   class error_complete detail;

   if (type != DB_SUCCESS) {
      evaluate(finished, DB_SUCCESS, errors, query);
   } else {
      foreach (map in data) {
         foreach (detail in errors) {
            if (detail->details->summary->id == map["id"]) {
                 detail->forwards += ({ create_error_forward(map) });
            }
         }
      }
      evaluate(finished, type, errors, query);
   }
}

/**
 * This method updates the comments based on the query results.
 * @param type type type
 * @param fdata the return data from the query
 * @param function finsiished the finished function
 * @param errors the errors results we are updating
 * @param query the ids list we are using
 */
void finish_details_error_replies(int type, mixed data, function finished, 
                     class error_complete* errors, string ids, string query) {
   mapping map;
   class error_complete detail;

   if (type != DB_SUCCESS) {
      evaluate(finished, DB_SUCCESS, errors, query);
   } else {
      foreach (map in data) {
         foreach (detail in errors) {
            if (detail->details->summary->id == map["bug_id"]) {
               detail->replies += ({ create_error_replies(map) });
            }
         }
      }
      DB_HANDLER->make_sql_request(_db, _user, _password, 
                     "select id,forwarddate,forwarder,olddirectory from "
                     "forwards where id in " + ids +
                     " order by forwarddate",
                     (: finish_forwards($1, $2, $(finished), $(errors), $(query)) :));
      //evaluate(finished, type, errors, query);
   }
}
/**
 * This method updates the comments based on the query results.
 * @param type type type
 * @param fdata the return data from the query
 * @param function finsiished the finished function
 * @param errors the errors results we are updating
 * @param query the ids list we are using
 */
void finish_comments(int type, mixed data, function finished, 
                     class error_complete* errors, string ids, string query) {
   mapping map;
   class error_complete detail;

   if (type != DB_SUCCESS) {
      evaluate(finished, DB_SUCCESS, errors, query);
   } else {
      foreach (map in data) {
         foreach (detail in errors) {
            if (detail->details->summary->id == map["id"]) {
                 detail->comments += ({ create_error_comment(map) });
            }
         }
      }
      DB_HANDLER->make_sql_request(_db, _user, _password, 
                     "select * from "
                     "error_replies where bug_id in " + ids +
                     " order by senddate",
                     (: finish_details_error_replies($1, $2, $(finished), $(errors), $(ids), $(query)) :));
      //evaluate(finished, type, errors, query);
   }
}

/**
 * This method finishes a details based query.
 * @param type the type returned (error etc)
 * @param data the data returned
 * @param finished the function to call on finish
 */
void finish_details(int type, mixed data, function finished, string query) {
   class error_complete* errors;
   mapping map;
   string str;

   if (type != DB_SUCCESS) {
      evaluate(finished, type, data, query);
   } else {
      errors = ({ });
      str = "(";
      foreach (map in data) {
         errors += ({ create_error_complete(create_error_details(map)) });
         str += map["id"] + ",";
      }
      str = str[0..<2] + ")";
      DB_HANDLER->make_sql_request(_db, _user, _password, 
                     "select commentdate,commenter,comment,id "
                     "from comments where id in " + str +
                     " order by commentdate desc",
                     (: finish_comments($1, $2, $(finished), $(errors), $(str), $(query)) :));
      //evaluate(finished, type, errors, query);
   }
}

/**
 * This method finishes a details based query.
 * @param type the type returned (error etc)
 * @param data the data returned
 * @param finished the function to call on finish
 */
void finish_forward_query(int type, mixed data, function finished,
                          string user, string newdir) {
   class error_complete error;
   mapping map;
   string str;

   if (type != DB_SUCCESS) {
      evaluate(finished, type, data);
   } else {
      map = data[0];
      error = create_error_complete(create_error_details(map));
      if (error->details->summary->directory != newdir) {
         str = "insert into forwards (id,forwarddate,forwarder,olddirectory) " +
               "values (" + error->details->summary->id + ", " + time() + ", '" +
               db_escape(user) + "', '" +
               db_escape(error->details->summary->directory) +
               "')";
         DB_HANDLER->make_sql_request(_db, _user, _password, 
                     str, (: 1 :));


         str = build_sql_update(error->details->summary->id, "directory", newdir);
         DB_HANDLER->make_sql_request(_db, _user, _password, str,
                                 (: finish_update($1, $2, $(finished), $(str)) :));
      } else {
         // Don't forward again if the bug is already there.
         evaluate(finished, type, data);
      }
   }
}

void finish_status_query(int type, mixed data, function finished,
                         string status,
                         int nomail, string user, string subject, 
                         string message) {
   class error_complete error;
   mapping map;
   string str;

   if (type != DB_SUCCESS) {
      evaluate(finished, type, data);
   } else {
      map = data[0];
      error = create_error_complete(create_error_details(map));

      // Send the mail if needed.
      if (!nomail) {
         // Send mail.
         str = "insert into error_replies (bug_id,status,sender,recipient,senddate,message,subject) "
               "values (" +
               error->details->summary->id + ", 'NEW', '" + user + "', '" +
               error->details->summary->reporter + "', " +
               time() + ", '" +
               "Change status from " + error->details->summary->status +
               " to " + status +
               "\n" + db_escape(message) + "', '" + db_escape(subject) + "')";
         DB_HANDLER->make_sql_request(_db, _user, _password, str,
                 (: 1 :) );
      }
   
      str = build_sql_update(error->details->summary->id, "status", status);
      if (!str) {
         return 0;
      }

      DB_HANDLER->make_sql_request(_db, _user, _password, str,
                            (: finish_update($1, $2, $(finished), $(str)) :));
   }
}

/**
 * This method finishes an update based query.
 * @param type the type returned (error etc)
 * @param data the data returned
 * @param finished the function to call on finish
 */
void finish_update(int type, mixed data, function finished, string query) {
   evaluate(finished, type, data, query);
}

/**
 * This method finishes doing a reply lookup.
 */
void finish_error_replies(int type, mixed data, function finished, string query) {
   class error_replies* replies;
   mapping map;

   if (type != DB_SUCCESS) {
      evaluate(finished, type, data);
   } else {
      replies = ({ });
      foreach (map in data) {
         replies += ({ create_error_replies(map) });
      }
      evaluate(finished, type, replies);
   }
}

/**
 * This method sets up a query for the specific data, this returns the
 * bug summary for all the found bugs.
 * @param query the class to use for the query
 * @param finished the function to call when things are finished
 * @return 1 on success, 0 on failure
 */
int do_query_bug_summary(class error_query query, function finished) {
   string str;

   str = build_sql_query(ERROR_SUMMARY_FIELDS,
                         query);
   if (!str) {
      return 0;
   }

   DB_HANDLER->make_sql_request(_db, _user, _password, str,
                                   (: finish_summary($1, $2, $(finished), $(str)) :));
   return 1;
}

/**
 * This method sets up a call to return the details of the error.
 * @param bug_id the id of the bug to get the details from
 * @param finished the function to call when things are finished
 * @return 1 on success, 0 on failure
 */
int do_query_bug_details(int bug_id, function finished) {
   string str;
   class error_query query;

   query = new(class error_query);
   query->id = bug_id;
   str = build_sql_query(ERROR_DETAILS_FIELDS, query);
   if (!str) {
      return 0;
   }

   DB_HANDLER->make_sql_request(_db, _user, _password, str,
                               (: finish_details($1, $2, $(finished), $(str)) :));
   return 1;
}

/**
 * This method finds the bug summary for all the open bugs in the
 * specified directory.
 * @param dir the directory to find the open bugs in
 * @param finished the function to call when things are finished
 * @return 1 on success, 0 on failure
 */
int do_query_open_bugs_in_dir(string dir, function finished) {
   class error_query query;

   query = new(class error_query);
   query->dir = dir;
   query->status = ({ "OPEN", "FIXING", "CONSIDERING" });
   return do_query_bug_summary(query, finished);
}

/**
 * This returns the bug summary for all the open bugs associated with
 * the specified file.
 * @param fname the file name
 * @param finished the function to call when things are finished
 * @return 1 on success, 0 on failure
 */
int do_query_open_bugs_for_fname(string fname, function finished) {
   class error_query query;

   query = new(class error_query);
   query->file_name = fname;
   query->status = ({ "OPEN", "FIXING", "CONSIDERING" });
   return do_query_bug_summary(query, finished);
}

/**
 * This returns the bug summary for all the open bugs associated with
 * the specified reporter.
 * @param reporter the reporter to find the bugs for
 * @param finished the function to call when things are finished
 * @return 1 on success, 0 on failure
 */
int do_query_open_bugs_by_reporter(string reporter, function finished) {
   class error_query query;

   query = new(class error_query);
   query->reporter = reporter;
   query->status = ({ "OPEN", "FIXING", "CONSIDERING" });
   return do_query_bug_summary(query, finished);
}

/**
 * This method changes the type of the bug.  The type is in the set
 * 'bug', 'typo', 'idea'.
 * @param id the id of the bug to update
 * @param type the new type of the bug
 * @return 1 on success, 0 on failure
 */
int do_change_type(int id, string type, function finished) {
   string str;

   str = build_sql_update(id, "type", type);
   if (!str) {
      return 0;
   }
   DB_HANDLER->make_sql_request(_db, _user, _password, str,
                               (: finish_update($1, $2, $(finished), $(str)) :));
   return 1;
}

/**
 * This method changes the category of the bug.  The category is in the set
 * 'room', 'object', 'help', 'ritual', 'spell', 'command', 'general'.
 * @param id the id of the bug to update
 * @param category the new category of the bug
 * @return 1 on success, 0 on failure
 */
int do_change_category(int id, string category, function finished) {
   string str;

   str = build_sql_update(id, "category", category);
   if (!str) {
      return 0;
   }
   DB_HANDLER->make_sql_request(_db, _user, _password, str,
                               (: finish_update($1, $2, $(finished), $(str)) :));
   return 1;
}

/**
 * This method changes who the bug is assigned to.
 * @param id the id of the bug to change
 * @param assigned_to the person it is assigned to
 * @return 1 on success, 0 on failure
 */
int do_assign_bug_to(int id, string assigned_to, function finished) {
   string str;

   str = build_sql_update(id, "assignedto", assigned_to);
   if (!str) {
      return 0;
   }
   DB_HANDLER->make_sql_request(_db, _user, _password, str,
                               (: finish_update($1, $2, $(finished), $(str)) :));
   return 1;
}

/**
 * This method changes the status of the bug, it also automaticaly
 * sends a message to the person who reported the bug and who the bug
 * is assigned to if someone else changes it.
 * @param id the id of the bug to change
 * @param status the new status of the bug
 * @param nomail dont send any mail
 * @param user the user who is making the change
 * @param subject the subject of the mail
 * @param message the main body of the mail
 * @return 0 on failure and 1 on success
 */
int do_change_status(int id, string status, int nomail, string subject,
                     string user, string message, function finished) {
   string str;
   class error_query query;

   query = new(class error_query, id : id);
   str = build_sql_query(ERROR_DETAILS_FIELDS, query);
   if (!str) {
      return 0;
   }

   DB_HANDLER->make_sql_request(_db, _user, _password, str,
             (: finish_status_query($1, $2, $(finished), $(status),
                                    $(nomail), $(user), $(subject),
                                     $(message)) :));
   return 1;
}

/**
 * This method changes the status of the bug, it also automaticaly
 * sends a message to the person who reported the bug and who the bug
 * is assigned to if someone else changes it.
 * @param id the id of the bug to change
 * @param user the user
 * @param dir the directory
 * @return 0 on failure and 1 on success
 */
int do_forward(int id, string user, string dir, function finished) {
   string str;
   class error_query query;

   if (!strlen(dir)) {
      return 0;
   }
   if (dir[0] != '/') {
      dir = "/" + dir;
   }
   if (dir[<1] == '/') {
      dir = dir[0..<2];
   }
   // Don't let people forward to root.
   if (file_size(dir) != -2 || dir == "/") {
      return 0;
   }

   query = new(class error_query, id : id);
   str = build_sql_query(ERROR_DETAILS_FIELDS, query);
   if (!str) {
      return 0;
   }

   // Need to get the details to find the old directory.
   DB_HANDLER->make_sql_request(_db, _user, _password, str,
             (: finish_forward_query($1, $2, $(finished), $(user), $(dir)) :));
   return 1;
}

/**
 * This method adds a comment into the database.
 * @param id the bug to comment on
 * @param user the user making the comment
 * @param comment the comment to make
 * @param finished the finished function
 */
int do_comment(int id, string user, string comment, function finished) {
   string str;

   str = "insert into comments (id,commentdate,commenter,comment) values (" +
         id + ", " + time() + ", '" + user + "', '" + db_escape(comment) +
         "')";
   DB_HANDLER->make_sql_request(_db, _user, _password, str,
                             (: finish_update($1, $2, $(finished), $(str)) :));
   return 1;
}

/**
 * This method looks for all the error replies for the specific recipient.
 * @param recipient the recipient
 * @param only_new only new messages
 * @param finished the function to call on finishing
 */
int do_error_replies(string recipient, int only_new, function finished) {
   string str;

   str = "select * from error_replies where recipient = '" + recipient +
         "'";
   if (only_new) {
      str += " and status = 'NEW'";
   } else {
      str += " and status != 'DELETED'";
   }
   str += " order by senddate";
   DB_HANDLER->make_sql_request(_db, _user, _password, str,
                      (: finish_error_replies($1, $2, $(finished), $(str)) :));
   return 1;
}

/**
 * This method sets the error reply status to something useful.
 * @param id the id of the error reply
 * @param status the new status
 * @param finished the finished fuinction
 * @return 1 if the setup is successful, 0 if not
 */
int do_status_error_reply(int id, string status, function finished) {
   string str;

   str = "update error_replies set status = '" + status + "' where id = " +
         id;
   DB_HANDLER->make_sql_request(_db, _user, _password, str,
                      (: finish_update($1, $2, $(finished), $(str)) :));
   return 1;
}

/**
 * THis method adds an error reply into the system.  THis is a debug message
 * and only useful until all the error setups are connected together properly.
 */
int do_add_error_reply(int bug_id, string from, string to, string subject, string message) {
   string str;

   str = "insert into error_replies (bug_id,status,sender,recipient,senddate,message,subject) "
         "values (" + bug_id + ", 'NEW', '" + from + "', '" + to + "', " +
         time() + ", '" +
         db_escape(message) + "', '" + db_escape(subject) + "')";
   DB_HANDLER->make_sql_request(_db, _user, _password, str,
           (: 1 :) );
   return 1;
}
