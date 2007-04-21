/**
 * The include file for the chime handler.  Spcifies the location of the
 * chime handler and the types used in the fucntion call when chiming.
 * @see /obj/handlers/chime_handler.c
 * @author Pinkfish
 */
#ifndef __CHIME_H
/** @ignore yes */
#define __CHIME_H

/** The chime handler object. */
#define CHIME_HANDLER "/obj/handlers/chime_handler"

/** The type for the hour chime. */
#define CHIME_HOUR 1
/** The type of the half hour chime. */
#define CHIME_HALF_HOUR 2
/** The type for the quarter hour chime. */
#define CHIME_QUARTER_HOUR 4

#endif
