/**
 * The include file for all the bits of door setup stuff.
 * @author Pinkfish
 */
#ifndef __SYS__NEW_DOOR
/** @ignore yes */
#define __SYS__NEW_DOOR
/*
 * These are indexes into the arrays in the door_types mapping defined
 * in the room handler.
 */
/** The index into the arrat showing if it is closed. */
#define D_CLOSED 0
/** The index into the arrat showing if it is locked. */
#define D_LOCKED 1
/** The index into the arrat showing what sort of key is needed. */
#define D_KEY 2
/** The index into the arrat showing the difficulty of picking the lock. */
#define D_DIFFICULTY 3
/** The index into the arrat showing if the door is hidden from the long. */
#define D_SECRET 4
/** The index into the arrat showing if the door is transparent. */
#define D_TRANSPARENT 5
/** The index into the arrat giving the name of the door. */
#define D_NAME 6

#endif /* __SYS__NEW_DOOR */
