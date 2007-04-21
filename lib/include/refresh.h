/**
 * Definition so we can reach the refresh handler, and some
 * other definitions for the arguments passed to the callback
 * function.
 * @author Ceres
 */
#ifndef __REFRESH_H
/** @ignore yes */
#define __REFRESH_H

/**
 * The path to the refresh handler itself.
 */
#define REFRESH_HANDLER "/obj/handlers/refresh"

/**
 * The flag to the method that tells us that the refresh is a partial
 * one.
 */
#define PARTIAL_REFRESH 0
/**
 * The flag to the method that tells us that the refresh is a total
 * one.
 */
#define TOTAL_REFRESH 1
/**
 * The flag to the method that tells us that the refresh is actually
 * the player being deleted.
 */
#define PLAYER_DELETED 2

#endif
