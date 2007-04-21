/**
 * This is the include file for the craft shop inheritable.
 * @author Pinkfish
 * @started Thu Sep 10 21:13:26 PDT 1998
 */
#ifndef __CRAFT_SHOP_H
/** @ignore yes */
#define __CRAFT_SHOP_H

/**
 * The path of the craft shop inheritable.
 */
#define CRAFT_SHOP_INHERIT "/std/shops/craft_shop"
/**
 * The path of the craft shop controller.
 */
#define CRAFT_SHOP_CONTROLLER "/std/shops/craft_shop_controller"

/**
 * This method defines the maximum number of letters a name in a 
 * craft shop can be.
 */
#define CRAFT_SHOP_MAX_NAME_LENGTH 50

/**
 * This means the main data file is being save.
 * This is the control type for the save/load functions. 
 */
#define CRAFT_SHOP_MAIN_SAVE_FILE 1
/**
 * This means the individual shop item is being saved.
 * This is the control type for the save/load functions. 
 */
#define CRAFT_SHOP_DATA_SAVE_FILE 2
/**
 * This means the individual shop item is being removed because it is no
 * longer used.  This will only be passed to the save function.
 * This is the control type for the save/load functions. 
 */
#define CRAFT_SHOP_REMOVE_DATA_SAVE_FILE 3

/**
 * The state of the category when it is being nominated as a new
 * position.
 */
#define CRAFT_CATEGORY_STATE_NOMINATING 1
/**
 * The state of the category when it is being voted into a real category
 * position.
 */
#define CRAFT_CATEGORY_STATE_VOTING     2
/**
 * The state of the category when it has been accepted and is happy about
 * it.
 */
#define CRAFT_CATEGORY_STATE_ACCEPTED   3
/**
 * The state of the category when it is being setup for deletion.
 */
#define CRAFT_CATEGORY_STATE_DELETEING  4

/**
 * This is the arguement for a yes vote.
 */
#define CRAFT_CATEGORY_CHOICE_YES 1
/**
 * This is the argument for a no vote.
 */
#define CRAFT_CATEGORY_CHOICE_NO 2
/**
 * This is the argument for a abstain vote.
 */
#define CRAFT_CATEGORY_CHOICE_ABSTAIN 3

/**
 * This is the value returned as an error from methods that normally return
 * nice happy values.
 */
#define CRAFT_CATEGORY_ERROR -1

/**
 * This is the directory to use to get help on the function names.
 */
#define CRAFT_SHOP_EXPRESSION_HELP_DIR "/doc/furniture/expressions/"

#endif
