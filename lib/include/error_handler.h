/**
 * This is the include file to handler all the basic error handler
 * code.
 * @author Pinkfish
 * @started Thu Feb  6 17:40:26 PST 2003
 */
#ifndef __ERROR_HANDLER_H
/** @ignore yes */
#define __ERROR_HANDLER_H

/**
 * This is the return value when the query is bad.
 */
#define ERROR_BAD_QUERY -1000

/** This is the path to the error handler.  */
#define ERROR_HANDLER "/obj/handlers/error_handler"

/** This is the list of valid categories. */
#define ERROR_CATEGORIES ({ "ROOM", "OBJECT", "HELP", "RITUAL", "SPELL", "COMMAND", "GENERAL" })

/** Error status open */
#define ERROR_STATUS_OPEN "OPEN"
/** Error status fixing, so it is in the middle of being fixed. */
#define ERROR_STATUS_FIXING "FIXING"
/** Error status considering the idea, not yet fixed. */
#define ERROR_STATUS_CONSIDERING "CONSIDERING"
/** Error status fixed.  The bug has been resolved and closed. */
#define ERROR_STATUS_FIXED "FIXED"
/** Error status denied.  The bug was not fixed and is marked as not being
 * a bug */
#define ERROR_STATUS_DENIED "DENIED"
/** This is the list of valid statuses. */
#define ERROR_STATUS ({ ERROR_STATUS_FIXED, ERROR_STATUS_DENIED, \
                        ERROR_STATUS_FIXING, ERROR_STATUS_CONSIDERING, \
                        ERROR_STATUS_OPEN })
/** This is the list of valid types. */
#define ERROR_TYPE ({ "BUG", "TYPO", "IDEA", "COMMENT" })

/** This define is used to order by the filename. */
#define ERROR_ORDER_BY_FILENAME "filename"
/** This define is used to order by the directory. */
#define ERROR_ORDER_BY_DIRECTORY "directory"
/** This define is used to order by the date. */
#define ERROR_ORDER_BY_DATE "date"
/** This define is used to order by the reporter. */
#define ERROR_ORDER_BY_REPORTER "reporter"

/** This is the define for the user name to use for assigned to 'no one'. */
#define ERROR_ASSIGNED_NO_ONE "nobody"

/**
 * The summary plus the details is the complete
 * error.
 * @member id the unique id of the bug
 * @member entry_date the date the bug was added
 * @member filename the filename the bug was put against
 * @member assigned_to who the bug is assigned to
 * @member reporter the person who reported the bug
 * @member category is one of ROOM | OBJECT etc
 * @member type is one of TYPO | BUG | IDEA
 * @member status is one of OPEN | CLOSED | etc
 * @member directory the directory the bug is in
 */
class error_summary {
   int id;
   int entry_date;
   string filename;
   string category;
   string assigned_to;
   string reporter;
   string type;
   string status;
   string directory;
}

/**
 * The details of the bug.
 * @member summary the summary class for the basic sumamry details
 * @member fix_date the date the bug was fixed
 * @member fixer the person who fixed the bug
 * @member report the report associated with the bug
 * @member runtime the runtime error associated with the bug
 */
class error_details {
   class error_summary summary;
   int fix_date;
   string fixer;
   string report;
   string runtime;
}

/**
 * The comments associated with the bug.
 * @member date the date the comment was added
 * @member commenter the person who commented on the bug
 * @member comment the comment about the bug
 */
class error_comment {
   int date;
   string commenter;
   string comment;
}

/**
 * The forwards associated with the bug.
 * @member date the date the forward was done
 * @member forwarder the person who forwared the bug
 * @member old_directory the old directory of the bug
 */
class error_forward {
   int date;
   string forwarder;
   string old_directory;
}

/**
 * The reply data for the error.  This is where the bug replies are
 * stored.
 * @member id the id of the bug reply
 * @member bug_id the id of the bug
 * @member sender who sent the bug reply
 * @member recipient the recipient of the bug reply
 * @member status the status of the bug reply
 * @member senddate when the bug was sent
 * @member subject the subject of the reply
 * @member message the message associated with the bug
 */
class error_replies {
   int id;
   int bug_id;
   string sender;
   string recipient;
   string status;
   int senddate;
   string subject;
   string message;
}

/**
 * All the details of the error.
 * @member details the details of the error
 * @member comments the comments associated with the error
 * @member forwards the forwards associated with the error
 * @member replies the replies associated with the error
 */
class error_complete {
   class error_details details;
   class error_comment* comments;
   class error_forward* forwards;
   class error_replies* replies;
}

/**
 * This is the class used to generate queries through the error handler.
 * You do not need to fill in all the fields, only the filled in ones will
 * be used as restrictions on the query.
 * @member no_extra_details this only works with the bug details queries
 *                          and it means that only the main data is fetched,
 *                          not comments etc.
 * @member recursive do recursive directory lookups
 * @member id look up this id specifically
 * @member id_list look up this list of ids, if this is specified it
 *                 overrides id
 * @member bug_offset the offset to use in the limit command
 * @member dir the directory to use
 * @member assigned_to who it is assigned to
 * @member file_name specific file name to lookup
 * @member reporter the reporter to lookup
 * @member fixer the person who fixed the bug
 * @member status the status of the bug
 * @member category the category of the bug
 * @member type the type of the bug
 * @member order_by one of the ordering methods
 */
class error_query {
   int no_extra_details;
   int recursive;
   int id;
   int bug_offset;
   int* id_list;
   string dir;
   string assigned_to;
   string file_name;
   string reporter;
   string fixer;
   string* status;
   string* category;
   string* type;
   string order_by;
}

#endif
