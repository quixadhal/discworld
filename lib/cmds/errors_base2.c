/* Hey Emacs, this is -*- LPC -*- ! */
/* $Id: errors_base.c,v 1.5 2001/06/02 03:28:17 presto Exp $ */

/**
 * This is the low level error database inheritable.  It handles all such
 * annoying things as connecting to the server and sending the queries.
 * There are 2 levels of functions, you can do your own queries or
 * leave the details to this object.
 * The errors database contains the following fields in the errors table:
 * <UL>
 * <LI>Id - The unique identifier for a report
 * <LI>DirEntryDate - The date the report was entered into the directory
 *     (changes when forwarded)
 * <LI>EntryDate - The date the report was made in time() format
 * <LI>FixDate - The date the report was fixed or marked otherwise in time()
 *     format
 * <LI>Directory - The directory the report belongs to
 * <LI>Filename - The filename of the object the report was made on
 * <LI>Category - The category the report belongs to, one of 'ROOM', 'OBJECT',
 *     'RITUAL', 'SPELL', 'HELP', or 'COMMAND'
 * <LI>Type - The type of the report, one of 'TYPO', 'BUG' or 'IDEA'
 * <LI>Name - The name of the object
 * <LI>Reporter - The name of the person who made the report
 * <LI>Fixer - The name of the person who fixed the report or otherwise marked
 *     it
 * <LI>Status - The status of the report, one of 'OPEN', 'FIXING',
 *     'CONSIDERING', 'DENIED', or 'FIXED'
 * <LI>Report - The text of the report as typed by the reporter
 * <LI>Runtime - The runtime error that may or may not(!) relate to the
 *     report
 * </UL>
 * There are is also a separate table for forwards, which has the following
 * fields:
 * <UL>
 * <LI>Id - The bug report id the forward belongs to
 * <LI>ForwardDate - The date the report was forwarder in time() format
 * <LI>Forwarder - The name of the person who forwarded the report
 * <LI>OldDirectory - The directory the report belonged to before forwarding
 * </UL>
 * Finally, a table for comments exists with the following fields:
 * <UL>
 * <LI>Id - The bug report id the comment belongs to
 * <LI>CommentDate - The date someone made a comment on the report in time()
 *     format
 * <LI>Commenter - The name of the person who commented on the report
 * <LI>Comment - The comment about the report
 * </UL>
 * @author Turrican
 */

#include <db.h>
#include <log.h>
#include <config.h>

/** @ignore yes */
private class report {
  int row;
  string newstatus;
  string newdir;
  string newtype;
  int changed;
}

/** @ignore yes */
private class bugs {
  //int fd;
  mixed* errors;
  string user;
  int changed;
}

private nosave mapping _globvars;

protected varargs mixed get_row(mixed key, int row, int nomap);

/**
 * This method initializes some state variables and connects to the errors
 * database.  It doesn't do very much now we do not use the
 * internal database methods.
 * @param key the unique key to use for the global variables
 * @param user the username used to connect to the database
 * @param replace set to 1 if an existing key should be replaced
 * @return 0 for succes, an error string for failure
 * @see save_changes()
 * @see finish_errors()
 */
protected varargs string init_errors(mixed key, string user, int replace) {
  string ret;

  if (!mapp(_globvars)) {
    _globvars = ([ ]);
  } else {
    if (!(replace || undefinedp(_globvars[key]))) {
      return "key already in use";
    }
  }
  _globvars[key] = new(class bugs);
  _globvars[key]->user = user;
  _globvars[key]->errors = ([ ]);
  return 0;
} /* init_errors() */

/**
 * This method ends access to the database.  It doesn't save any pending
 * changes.
 * @param key the unique key for the global variables
 * @return 0 for succes, an error string for failure
 * @see init_errors()
 * @see save_changes()
 */
protected string finish_errors(mixed key) {
  string ret;

  if (!classp(_globvars[key])) {
    return "unknown key";
  }
  map_delete(_globvars, key);
  return ret;
} /* finish_errors() */

/**
 * This method is called when the request finishes.
 * @param key the key of the thingy which finished
 */
void event_finished_get_fields(string key) {
} /* event_finished_get_fiels() */

private void finished_get_fields(mixed key, int type, mixed* data) {
   _globvars[key]->errors = data;
   event_finished_get_fields(key);
} /* finished_get_fields() */

/**
 * This method fetches the specified fields from the database.  You should
 * not call this too often as this is a very expensive operation.  Typically
 * it's called once, at the beginning of your object.  This will finish
 * immeditately, a function called 'event_finished_get_fields' will be
 * called that you can do whatever you want with.
 * @param key the unique key for the global variables
 * @param directory the directory to get the reports from
 * @param fields a comma separated list of fields you wish to fetch
 * @return the number of reports that were found or an error string
 * @see get_bug()
 * @see get_forwards()
 * @see get_comments()
 */
protected void get_fields(mixed key, string directory,
                          string fields, string type) {
  string query;
  string temp;
  mixed ret;

  if (!strlen(type)) {
    type = "OPEN";
  }

  _globvars[key]->errors = ({ });

  fields += ", Directory";
  query = sprintf("SELECT %s FROM errors WHERE Directory = '%s' AND "
                  "Status = '%s' ORDER BY DirEntryDate;", fields, directory,
                  type);
  MYSQL_HANDLER->make_sql_request("errors", USER, "", query,
                                  (: finished_get_fields($(key), $1, $2) :));
} /* get_fields() */

void event_finish_get_forwards(mixed key, int row) {
} /* event_finish_get_forwards() */

protected void finish_get_forwards(mixed key, int row, int type, mixed* data) {
   _globvars[key]->forwards[row] = data;
   event_finish_get_forwards(key, row);
} /* finish_get_forwards() */

/**
 * This method gets the forwarding info for a report in the database.
 * The info is given in a mapping indexed by column name where the
 * values are arrays of the values for the columns, ordered by
 * ForwardDate.
 * @param key the unique key for the global variables
 * @param row the row number to get the forwards for
 * @return a mapping with the forwards or an error string
 * @see get_fields()
 * @see get_comments()
 */
protected void get_forwards(mixed key, int row) {
  string query, err;
  mixed ret;
  mapping forwards;

  ret = _globvars[key]->errors[row];
  if (_globvars[key]->forwards[row]) {
     return _globvars[key]->forwards[row];
  }
  query = sprintf("SELECT ForwardDate, Forwarder, OldDirectory FROM forwards "
                  "WHERE Id = %d ORDER BY ForwardDate;", ret["Id"]);
  MYSQL_HANDLER->make_sql_request("errors", USER, "", query,
                                  (: finished_get_forwards($(key), $(row),
                                     $1, $2) :));
/*
  if (ret) {
    err = catch {
      int *dates, i;
      string *forwarders, *dirs;
      mixed *vals;

      dates = allocate(ret);
      forwarders = allocate(ret);
      dirs = allocate(ret);
      for (i = 0; i < ret; i++) {
        vals = db_fetch(fd, i + 1);
        dates[i] = vals[0];
        forwarders[i] = vals[1];
        dirs[i] = vals[2];
      }
      forwards = allocate_mapping(3);
      forwards["ForwardDate"] = dates;
      forwards["Forwarder"] = forwarders;
      forwards["OldDirectory"] = dirs;
    };
    if (err) {
      //catch(db_close(fd));
      return err;
    }
  }
  //catch(db_close(fd));
  return forwards;
 */
} /* get_forwards() */

void event_finish_get_comments(mixed key, int row) {
} /* event_finish_get_comments() */

protected void finish_get_comments(mixed key, int row, int type, mixed* data) {
   _globvars[key]->comments[row] = data;
   event_finish_get_comments(key, row);
} /* finish_get_comments() */

/**
 * This method gets the comments about a report in the database.
 * The info is given in a mapping indexed by column name where the
 * values are arrays of the values for the columns, ordered by
 * CommentDate.
 * @param key the unique key for the global variables
 * @param row the row number to get the comments for
 * @return a mapping with the comments or an error string
 * @see get_fields()
 * @see get_forwards()
 */
protected void get_comments(mixed key, int row) {
  string query, err;
  mixed ret;
  mapping comments;
  int fd;

  ret = _globvars[key]->errors[row];
  if (_globvars[key]->comments[row]) {
     return _globvars[key]->comments[row];
  }
  query = sprintf("SELECT CommentDate, Commenter, Comment FROM comments "
                  "WHERE Id = %d ORDER BY CommentDate;", ret["Id"]);
  MYSQL_HANDLER->make_sql_request("errors", USER, "", query,
                                  (: finished_get_comments($(key), $(row),
                                     $1, $2) :));
} /* get_comments() */

/**
 * This method returns a row of information from the database.  Note that
 * you get the original, not a copy, so any changes you make to the row
 * will also be reflected in a later get_row().  If you don't want this,
 * just make a copy(). The row consists of a mapping with the column names
 * being the keys.
 * @param key the unique key for the global variables
 * @param number the row number of the bug report
 * @return a row from the database or an error string
 * @see get_fields()
 * @see efun::copy()
 */
protected void get_row(mixed key, int row) {
  mixed *res;
  string ret, *keys;
  mapping rowvals;

  return _globvars[key]->errors[row];  
} /* get_row() */

/**
 * This method changes the status of a bug report.  In the old system
 * it would delete the bug.
 * @param key the unique key for the global variables
 * @param row the row number to change the status of
 * @param status the new status of the report
 * @return 1 for succes, 0 for failure
 */
protected int set_status(mixed key, int row, string status) {
  int realrow = ROWS[row];
  
  /* Currently, this can't fail because of missing commit/rollback
     facilities in MySQL. We only actually save this later on. */
  if (!classp(_globvars[key]->changes[row])) {
    _globvars[key]->changes[row] = new(class _report, row : realrow);
  }
  _globvars[key]->changes[row]->newstatus = status;
  _globvars[key]->changes[row]->changed = 1;
  return 1;
} /* set_status() */

/**
 * This method changes the type of a bug report.
 * Possible types are 'IDEA', 'BUG' and 'TYPO'.
 * @param key the unique key for the global variables
 * @param row the row number to change the type of
 * @param type the new type of the report
 * @return 1 for succes, 0 for failure
 */
protected int set_type(mixed key, int row, string type) {
  /* Currently, this can't fail because of missing commit/rollback
     facilities in MySQL. We only actually save this later on. */
  if (!classp(_globvars[key]->changes[row])) {
    _globvars[key]->changes[row] = new(class _report, row : realrow);
  }
  _globvars[key]->changes[row]->newtype = status;
  _globvars[key]->changes[row]->changed = 1;
  return 1;
} /* set_type() */

/**
 * This method forwards the bug report to a different directory.
 * @param key the unique key for the global variables
 * @param row the row number of the bug to forward
 * @param directory the name of the new directory
 * @return 1 for succes, 0 for failure
 */
protected int forward_bug(mixed key, int row, string directory) {
  int realrow = ROWS[row];

  /* Currently, this can't fail because of missing commit/rollback
     facilities in MySQL. We only actually save this later on. */
  while (directory[<1] == '/') {
    directory = directory[0..<2];
  }
  if (!classp(_globvars[key]->changes[row])) {
    _globvars[key]->changes[row] = new(class _report, row : realrow);
  }
  _globvars[key]->changes[row]->newdir = directory;
  _globvars[key]->changes[row]->changed = 1;
  return 1;
} /* forward_bug() */

/**
 * This method stores a comment about the report.
 * Unlike most other methods, this is immediately saved in the
 * comments table!
 * @param key the unique key for the global variables
 * @param row the row number of the bug to forward
 * @param who the name of the commenter
 * @param comment the text of the comment
 * @return 0 for succes, an error string for failure
 */
protected string comment_bug(mixed key, int row, string who, string comment) {
  string query, err;
  int fd;
  mixed ret;

  query = sprintf("INSERT LOW_PRIORITY INTO comments VALUES "
                  "(%d, %d, '%s', '%s');",
                  ret["Id"], time(), who, comment);
  MYSQL_HANDLER->make_sql_request("errors", USER, "", query);
  return 0;
} /* comment_bug() */

/** @ignore yes */
private string save_status(mixed key, int *ids, string user, string status) {
  string query, ret, err;
  mixed res;

  ret = "";
  if (sizeof(ids)) {
    if (sizeof(ids) == 1) {
      query = sprintf("UPDATE LOW_PRIORITY errors SET Status = '%s', "
                      "FixDate = %d, Fixer = '%s' WHERE Id = %d;", status,
                      time(), user, ids[0]);
    } else {
      query = sprintf("UPDATE LOW_PRIORITY errors SET Status = '%s', "
                      "FixDate = %d, Fixer = '%s' WHERE Id IN (%s);", status,
                      time(), user, implode(ids, (: "" + $1 + ", " + $2 :)));
    }
    MYSQL_HANDLER->make_sql_request("errors", USER, "", query);
  }
  return ret;
}

/**
 * This method saves any pending changes into the database.
 * @param key the unique key for the global variables
 * @param user the name of the person who made the changes
 * @return 0 for succes, an error string for failure
 * @see finish_errors()
 * @see init_errors()
 */
protected string save_changes(mixed key, string user) {
  class _report *reports, report;
  string query, *queries, ret, err;
  int *ids_fixed, *ids_denied, *ids_fixing, *ids_considering, *ids_open, ftime;
  mixed res, row;

  if (!CHANGED) {
    return 0;
  }
  reports = filter(values(ERRORS), (: ((class _report)$1)->changed :));
  ids_fixed = ids_denied = ids_fixing = ids_considering = ids_open = queries = ({ });
  ret = "";
  foreach (report in reports) {
    row = get_row(key, report->row, 1);
    if (stringp(row)) {
      ret += row;
      continue;
    }
    switch (report->newstatus) {
    case "FIXED":
      ids_fixed += ({ row["Id"] });
      break;
    case "DENIED":
      ids_denied += ({ row["Id"] });
      break;
    case "FIXING":
      ids_fixing += ({ row["Id"] });
      break;
    case "CONSIDERING":
      ids_considering += ({ row["Id"] });
      break;
    case "OPEN":
      ids_open += ({ row["Id"] });
      break;
    case 0:
    case "":
      if (report->newdir) {
        ftime = time();
        query = sprintf("UPDATE LOW_PRIORITY errors SET Directory = '%s', "
                        "DirEntryDate = %d WHERE Id = %d;",
                        report->newdir, ftime, row["Id"]);
        queries += ({ query });
        query = sprintf("INSERT LOW_PRIORITY INTO forwards VALUES "
                        "(%d, %d, '%s', '%s');",
                        row["Id"], ftime, user, row["Directory"]);
        queries += ({ query });
      }
      if (report->newtype) {
        query = sprintf("UPDATE LOW_PRIORITY errors SET Type = '%s' "
                        "WHERE Id = %d;", report->newtype, row["Id"]);
        queries += ({ query });
      }
      
      break;
    default:
      ret += sprintf("Unknown report status: %s\n", report->newstatus);
      break;
    }
  }
  ret += save_status(key, ids_fixed, user, "FIXED");
  ret += save_status(key, ids_denied, user, "DENIED");
  ret += save_status(key, ids_fixing, user, "FIXING");
  ret += save_status(key, ids_considering, user, "CONSIDERING");
  ret += save_status(key, ids_open, user, "OPEN" );
  foreach (query in queries) {
    err = catch(db_exec(FD, query));
    if (err) {
      ret += err;
    }
    if (stringp(res)) {
      ret += res;
    }
  }
  if (ret == "") {
    ret = 0;
  }
  return ret;
} /* save_changes() */
