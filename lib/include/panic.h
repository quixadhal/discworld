/**
 * The include file for the panic handler.
 * @author Pinkfish
 */
#ifndef __PANIC_H
/** @ignore yes */
#define __PANIC_H

/**
 * How many godmothers are in the handler.
 */
#define NUM_GODMOTHERS 2
/**
 * The directory to find the godmothers.
 */
#define GODMOTHER_DIR "/obj/monster/godmother/"

/**
 * The amount of time the godmother will wait for the first response.
 */
#define FIRST_WAIT_TIME 120

/**
 * The amount of time the godmother will wait for the final response.
 */
#define FINISH_WAIT_TIME 240

/**
 * The location of the panic handler.
 */
#define PANIC_HANDLER "/obj/handlers/fairy_godmothers"

/**
 * The did not panic successfuly.
 */
#define PANIC_NOT_OK 0
/**
 * They paniced successful.
 */
#define PANIC_OK 1
#endif
