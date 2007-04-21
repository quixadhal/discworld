/**
 * The include file for the login handler.
 * @author Pinkfish
 */
#ifndef __SYS__LOGIN_HANDLER
/** @ignore yes */
#define __SYS__LOGIN_HANDLER

#include <player_handler.h>

/**
 * The path to the login handler.
 */
#define LOGIN_HANDLER "/obj/handlers/login_handler"

/** The name of the login event.  Used in the static/dynamic login calls. */
#define LOGIN "login"
/** The name of the logout event.  Used in the static/dynamic login calls. */
#define LOGOUT "logout"
/** The name of the netdeath event.  Used in the static/dynamic login calls. */
#define NETDEATH "netdeath"
/** The name of the reconnect event.  Used in the static/dynamic login calls. */
#define RECONNECT "reconnect"

/** The maximum number of compressed stream players. */
#define MAX_COMPRESSED_PLAYERS 200
/** The maximum number of uncompressed stream players.  */ 
#define MAX_UNCOMPRESSED_PLAYERS 100
/** The maximum number of players online at Discworld at once. */
#define MAX_PLAYERS (MAX_COMPRESSED_PLAYERS + MAX_UNCOMPRESSED_PLAYERS)

/** Flag to the is_discworld_full method. */
#define WITHOUT_LOGINS_NOT_IN_QUEUE 3
/** Flag to the is_discworld_full method. */
#define WITHOUT_ANY_LOGINS 2
/** Flag to the is_discworld_full method. */
#define WITHOUT_LOGIN_QUEUE 1
/** Flag to the is_discworld_full method. */
#define WITH_LOGIN_QUEUE    0

/** Returned from is_discworld_full if Discworld is full. */ 
#define DISCWORLD_IS_EMPTY 0 
#define COMPRESSED_FULL 1
#define UNCOMPRESSED_FULL 2 
#define DISCWORLD_IS_FULL (COMPRESSED_FULL | TELNET_FULL)

#endif /* __SYS__LOGIN_HANDLER */
