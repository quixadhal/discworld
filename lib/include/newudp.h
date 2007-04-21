#ifndef __SYS__NEWUDP
#define __SYS__NEWUDP

#ifndef __NEWUDP_H
#define __NEWUDP_H
/* --- User Configuration. --- */

#ifdef __UDP_H
#define NEW_INETD "/net/intermud/inetd"
#else
#define INETD		"/net/intermud/inetd"
#endif
#define UDP_CMD_DIR	"/net/intermud/udp/"
#define HOST_FILE	"/net/intermud/adm/INETD_HOSTS"
#define INETD_LOG_FILE	"INETD"

#define REPLY_TIME_OUT	12
#define RETRIES		2

/* #define LOCAL_NAME	SECURITY->get_mud_name()	// CD */
#define LOCAL_NAME	MUD_NAME

/* #define LOCAL_UDP_PORT	SECURITY->do_debug("udp_port");  // CD */
/* #define LOCAL_UDP_PORT	query_imp_port() */
#define LOCAL_UDP_PORT 5756

#define INETD_DIAGNOSTICS

/* If you are running another intermud system concurrently and there is a
 * clash with the udp.h filename, rename the old udp.h file as appropriate
 * and include this line. */
/* #include <cdudp.h>	// CD */

/* Include these definitions for CD muds. */
/* #define CDLIB */
/* #define CD_UDP		"/d/Standard/obj/udp" */

/* --- End of config. --- */

#define INETD_VERSION	"0.61"

/* --- Standard header macros. --- */

#define RECIPIENT	"RCPNT"
#define REQUEST		"REQ"
#define SENDER		"SND"
/* The DATA field should be used to store the main body of any packet. */
#define DATA		"DATA"

/* These headers are reserved for system use only. */
#define HOST		"HST"
#define ID		"ID"
#define NAME		"NAME"
#define PACKET		"PKT"
#define UDP_PORT	"UDP"
#define SYSTEM		"SYS"

/* Reserved headers for diagnostics. */
#define PACKET_LOSS	"PKT_LOSS"
#define RESPONSE_TIME	"TIME"

/* --- Standard REQUEST macros. --- */

#define PING		"ping"
#define QUERY		"query"
#define REPLY		"reply"

/* --- Standard SYSTEM macros. --- */
#define TIME_OUT	"TO"

/* --- Index macros for host arrays. --- */

#define HOST_NAME	0
#define HOST_IP		1
#define HOST_UDP_PORT	2
#define LOCAL_COMMANDS	3
#define HOST_COMMANDS	4
#define HOST_STATUS	5

#endif /* __NEWUDP_H */

#endif /* __SYS__NEWUDP */
