/**
 * This is used in conjunction with the placement inherit to handle
 * how things are placed into the room.
 */
#ifndef __ROOM_PLACEMENT_H__
/** @ignore yes */
#define __ROOM_PLACENENT_H__

/**
 * This property is used for handling the current room verb.  This is used
 * after the item has been placed.
 */
#define PLACEMENT_CURRENT_ROOM_VERB "current room verb"
/**
 * This property is used for handling the current object 'other'.  The
 * other is the 'against', 'beside' etc things.
 * This is used
 * after the item has been placed.
 */
#define PLACEMENT_CURRENT_ROOM_OTHER "current room other"
/**
 * This property is used for handling the current object position.  The
 * position is what it is beside, like 'wall', 'ceiling' etc.
 * This is used
 * after the item has been placed.
 */
#define PLACEMENT_CURRENT_ROOM_POSITION "current room position"
/**
 * This property is used to make items in the room as fixed in their current
 * place.  This way they cannot be removed or replaced.
 */
#define PLACEMENT_IMMOVABLE "immovable"

#endif
