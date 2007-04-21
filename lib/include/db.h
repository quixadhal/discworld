/* $Id: db.h,v 1.2 2003/02/11 20:10:15 pinkfish Exp $ */

/**
 * The header file for the database package.  Include this if your
 * object is going to make SQL queries to the database.
 */
#ifndef DB_H
#define DB_H

/**
 * Use this macro to properly escape text that you wish to use in an
 * SQL query.
 */
#define db_escape(query) replace_string((query), "'", "''")

/**
 * This is the handler to the database object.
 */
#define DB_HANDLER "/net/daemon/mysql"

/**
 * This should be the value returned if we have success.
 */
#define DB_SUCCESS 0

/**
 * THis is the error value if a socket was unable to be opened for
 * some reason.
 */
#define DB_ERROR_BAD_SOCKET -1

#endif /* DB_H */
