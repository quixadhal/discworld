/**
 * This is the header file for the player information database handler.
 *
 * @author Fiona
 */

#ifndef __PLAYERINFO_H
#define __PLAYERINFO_H

/**
 * Structure containing one database entry. Fields are:
 *    time    - entry's creation timestamp
 *    creator - the name of the creator who added the event
 *    event   - the name of the event
 *    comment - arbitrary comment string
 *    extra   - arbitrary array of arbitrary objects
 * @ignore
 */
class dbentry {
    int     time;
    string  creator;
    string  event;
    string  comment;
    mixed   *extra;
}

/**
 * Structure containing a single player's information. Fields are:
 *    name        - the name of the player (always lowercased)
 *    last_check  - timestamp of the last timeout check
 *    alts        - array containing this player's alt character names
 *    data        - array containing playerinfo entries
 * @ignore
 */
class playerinfo {
    string          name;
    int             last_check;
    string          *alts;
    class dbentry   *data;
    string          main_alt;
}

/** This is the handler for all the player information. Lucky it. */
#define PLAYERINFO_HANDLER "/obj/handlers/playerinfo"

#define VERBOSE_PI "verbose playerinfo"

#endif                         // __PLAYERINFO_H
