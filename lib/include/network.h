/**
 * This header includes all the network defines.
 */
#ifndef __NETWORK_H 
/** @ignore yes */
#define __NETWORK_H 
 
#include <socket_errors.h> 

/**
 * The parameter to the socket_create efun, makes a mud type socket.
 */ 
#define MUD                       0 
/**
 * The parameter to the socket_create efun, makes a stream type socket.
 */ 
#define STREAM                    1 
/**
 * The parameter to the socket_create efun, makes a datagram type socket.
 */ 
#define DATAGRAM                  2 
/**
 * The parameter to the socket_create efun, makes a binary stream type socket.
 */ 
#define STREAM_BINARY             3
/**
 * The parameter to the socket_create efun, makes a binary datagram type socket.
 */ 
#define DATAGRAM_BINARY           4

/**
 * This is the port the mud is on.
 */
#define PORT_MUD (mud_name() == "Discworld") ? 23 : 6677
/**
 * This is the port the ftp server runs on.
 */
#define PORT_FTP (mud_name() == "Discworld") ? 21 : 4444

/**
 * This is the port the http server runs on.
 */
#define PORT_HTTP                 80
/** 
 * This is the port the MUD's internal LPC web server runs on
 */
#define PORT_INTERNAL_HTTP                  5678
/**
 * This is the port the out of band data for the intermud services runs on.
 */
#define PORT_OOB                  8765
/**
 * This is the port the authorisation stuff for the ftp server runs on.
 */
#define PORT_FTPAUTH              2227
#undef  PORT_RCP
#undef  PORT_UDP

/**
 * This is the inherit to use for client based network inheritables.
 */
#define CLIENT                    "/net/inherit/client"
/**
 * This is the inherit to use for server based network inheritables.
 */
#define SERVER                    "/net/inherit/server"
/**
 * This is the path to the intermud 3 daemon for doing intermud requests.
 */
#define INTERMUD_D                "/net/intermud3/intermud"
/**
 * This is the path to the intermud 3 services handler.
 */
#define SERVICES_D                "/net/intermud3/services"
/**
 * This is the path to the intermud 3 out of band services handler.
 */
#define OOB_D                     "/net/intermud3/oob_server"
/**
 * This is the path to the dictionary handler.
 */
#define NETWORK_DICTIONARY_HANDLER "/net/daemon/dictionary"

/**
 * The successful return of a network thing.
 */
#define NETWORK_SUCCESS 0

/**
 * The Discworld version string
 */
#define MUDLIB_VERSION "Discworld 17.0a"

/**
 * Admin Address.
 */
#define ADMIN_ADDRESS (mud_name() == "Discworld") ? "trustees@discworld.imaginary.com" : "none"

/**
 * Allows the real Discworld to show 'open for players' while mirrors
 * automatically show 'Development'
 */
#define STATUS (mud_name() == "Discworld") ? "open for players" : "Development"

#endif /* __NETWORK_H */ 
