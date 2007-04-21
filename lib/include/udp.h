#ifndef __SYS__UDP
#define __SYS__UDP

#ifndef __UDP_H
#define __UDP_H
#define NAMESERVER_CD "/net/nameserver_cd"
#define UDP_PATH "/net/udp/"
#define NETMAIL_D "/net/daemon/netmaild"

#define PING_Q (UDP_PATH+"ping_q")
#define PING_A (UDP_PATH+"ping_a")

/* #define MUDLIST_UDP ({ "milou.cd.chalmers.se", 2500 }) */
/* #define MUDLIST_UDP ({ "199.199.122.10", 1709 })
 * We need the ip number, the current socket efuns don't handle using names.
 */
#define MUDLIST_UDP ({ "165.95.7.122", 5678, "199.199.122.10", 1709, "129.16.117.21", 2500, "198.69.82.2", 5563, "129.10.114.86", 5563 })


/* The currently known types */
#define UDP_STARTUP "startup"
#define UDP_SHUTDOWN "shutdown"
#define UDP_BOARD_Q "board_q"
#define UDP_PING_Q "ping_q"
#define UDP_PING_A "ping_a"
#define UDP_SUPPORT_Q "support_q"
#define UDP_SUPPORT_A "support_a"
#define UDP_RWHO_Q "rwho_q"
#define UDP_RWHO_A "rwho_a"
#define UDP_MUDLIST_Q "mudlist_q"
#define UDP_MUDLIST_A "mudlist_a"
#define UDP_GFINGER_Q "gfinger_q"
#define UDP_GFINGER_A "gfinger_a"
#define UDP_WARNING "warning"
#define UDP_GTELL "gtell"
#define UDP_GWIZMSG "gwizmsg"
#define UDP_AFFIRMATION_A "affirmation_a"
#define UDP_MAIL_A "mail_a"
#define UDP_MAIL_Q "mail_q"
#define UDP_LOCATE_Q "locate_q"
#define UDP_LOCATE_A "locate_a"

/*
 * This is the number of retrys we ping the mud for before we decide
 * it has gone down.
 */
#define MAX_RETRYS 2
#define UDP_NO_CONTACT "_udp_no_contact"
#endif /* __UDP_H */

#endif /* __SYS__UDP */
