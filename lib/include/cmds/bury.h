/**
 * Contains information specific for the bury command.
 * @author Pinkfish
 * @started Mon Feb  7 01:12:21 PST 2000
 */
#ifndef __BURY_H
/** @ignore yes */
#define __BURY_H

/**
 * This is the path to the burial effect.
 */
#define BURY_EFFECT "/std/effects/room/burial"

/**
 * This is the amount of time to leave the thing buried before really
 * destroying it. (5 minutes)
 */
#define BURY_TIMEOUT (5 * 60)

/**
 * This is the property that's used to track the environment of the object
 * when it's buried.
 */
#define BURY_PROPERTY "bury location"

#endif
