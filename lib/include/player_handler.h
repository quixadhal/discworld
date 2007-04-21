/**
 * The main include file for the player handler.
 * @see /obj/handlers/player_change
 * @author Pinkfish
 */
#ifndef __PLAYER_HANDLER_H
/** @ignore yes */
#define __PLAYER_HANDLER_H

/**
 * The define for the location of the player handler.
 */
#define PLAYER_HANDLER "/obj/handlers/player_handler"

/**
 * The directory under the player save directory in which to put the
 * deleted files.
 */
#define DELETE_DIR "delete"

/**
 * The directory under the player save directory in which to put the
 * players who are deleted pending appeal.
 */
#define APPEAL_DIR "appeal"

/**
 * The directory in which to put the player save files.
 */
#define PLAYER_SAVE_DIR "/save/players/"

#define PLAYER_RAM_DIR "/save/ramdisk/players/"

/**
 * The banish directory
 */
#define BANISH_DIR "/banish/"

#endif
