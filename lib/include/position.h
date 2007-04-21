#ifndef __POSITION_H
#define __POSITION_H

/**
 * the swimming position, used in the underwater room.
 * @see /std/uwater.c
 */
#define SWIMMING "swimming"
#define STANDING "standing"
#define SITTING "sitting"
#define KNEELING "kneeling"
#define LYING "lying"
#define MEDITATING "meditating"
#define CROUCHING "crouching"

/**
 * The property which should be set on objects to allow them to
 * be used for positions.
 */
#define CAN_POSITION_PROPERTY "position on"
/**
 * The property to be set on objhects which tells them to work as
 * multiple objects.
 */
#define MULTIPLE_POSITION_PROPERTY "position multiple"
/**
 * The property to be set on rooms to store the default position in.
 */
#define DEFAULT_POSITION_PROPERTY "default position"

#define POSITION_DEFAULT STANDING

/**
 * The path to the object to use when trying to make someone stand up.
 */
#define STANDING_CMD "/cmds/living/stand"

/**
 * The path to the object to use when trying to make someone stand up.
 */
#define LYING_CMD "/cmds/living/lie"

/**
 * The path to the object to use when trying to make someone stand up.
 */
#define SITTING_CMD "/cmds/living/sit"

/* The position array stuff on the player object */
#define POS_ON_OBJECT 0
#define POS_TYPE      1
#define POS_MULTIPLE  2
#define POSITION_ARRAY_SIZE 3

/* Ok, the type thingies, on, at etc... */
#define ON_TYPE "on"
#define AT_TYPE "at"
#define BESIDE_TYPE "beside"

/* This is te stuff for the default position array. */
/**
 * The size of the array if there is only a type.
 */
#define POSITION_ONLY_TYPE_SIZE 1
/**
 * The size of the array with all the stuff in i it.
 */
#define POSITION_MESS_SIZE 3
/**
 * The posiition in the array fopr the position type.
 */
#define POSITION_TYPE_INDEX 0
/**
 * The index fort the position message to tell others.
 */
#define POSITION_ME_MESS_INDEX 1
/**
 * The index whith referese to the message to tell others.
 */
#define POSITION_REST_MESS 2
#endif
