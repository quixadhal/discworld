/**
 * This file contains all the basic configuration information for the
 * mudlib.  Controls all the bits which need to be setup for someone to get
 * somewhere.
 * @author Pinkfish
 */
#ifndef __SYS__CONFIG
/** @ignore yes */
#define __SYS__CONFIG

/**
 * A nice define for the configuration setting this place as discworld.
 */
#define __DISCWORLD__

#define __DISTRIBUTION_LIB__

/**
 * A define setting the start location.
 */
#define CONFIG_START_LOCATION "/d/dist/pumpkin/squash/squash5"

/**
 * The start location for newbies.
 */
#define CONFIG_NEWBIE_START_LOCATION "/d/liaison/NEWBIE/foyer"

/**
 * The players default race.
 */
#define CONFIG_DEFAULT_RACE "/std/races/human"

/**
 * The players default race.
 */
#define CONFIG_DEFAULT_GUILD "/std/guilds/standard"

/**
 * The user to use to connect to mysql.
 */
#define CONFIG_DB_USER "ddt"

/**
 * The web address to use for the site.
 */
#define CONFIG_WEB_ADDRESS "http://dead-souls.net/"

/**
 * The web address to use external for the site.
 */
#define CONFIG_EXTERNAL_WEB_ADDRESS "http://dead-souls.net/"

#endif /* __SYS__CONFIG */
