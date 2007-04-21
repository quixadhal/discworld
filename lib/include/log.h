/**
 * This include file for use with bug, idea and typo report logging.
 */
#ifndef __SYS__LOG
/** @ignore yes */
#define __SYS__LOG


/**
 * This is the file name into which the errors used to saved.  Depreciated.
 * @ignore yes
 */
#define ERROR_LOG "ERROR_REPORTS"
/**
 * This is the file name into which the errors used to saved.  Depreciated.
 * @ignore yes
 */
#define PLAYER_ERROR_LOG "BUGS"
/**
 * Where the errors are logged to.
 */
#define LOG_FILE "/log/ERRORS"
/**
 * Used to keep track of the mailing list to use when you exit the
 * error system.
 */
#define TEMP_MAIL_LIST ("/w/" + this_player()->query_name() + "/TEMP_MAIL_LIST")
/**
 * Used to keep track of the forward list to use when you exit the
 * error system.
 */
#define TEMP_FORWARD_LIST ("/w/" + this_player()->query_name() + "/TEMP_FORWARD_LIST")
/**
 * The smart logger.
 */
#define SMART_LOG "/std/smart_log"

/**
 * Used to keep track of the mail list to use when you exit the
 * error system on the web.
 */
#define WWW_TEMP_MAIL_LIST "/w/" + user + "/TEMP_MAIL_LIST"
/**
 * Used to keep track of the forward list to use when you exit the
 * error system on the web.
 */
#define WWW_TEMP_FORWARD_LIST "/w/" + user + "/TEMP_FORWARD_LIST"

#endif /* __SYS__LOG */
