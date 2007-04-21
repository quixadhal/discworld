/**
 * This is the main include file for the pub_shop.c inheritable.
 * The file reaction.h contains a series of defines for sizes of
 * containers.
 * @author Lemming
 * @started 23/10/1999
 * @see /include/reaction.h
 */

#ifndef __SYS__PUB_SHOP

/** @ignore */
#define __SYS__PUB_SHOP

/**
 * This tells add_menu_item() that the new item is an appetiser.
 */
#define PUB_APPETISER   0
/**
 * This tells add_menu_item() that the new item is a main course.
 */
#define PUB_MAINCOURSE  1
/**
 * This tells add_menu_item() that the new item is a dessert.
 */
#define PUB_DESSERT     2
/**
 * This tells add_menu_item() that the new item is an alcoholic drink.
 */
#define PUB_ALCOHOL     3
/**
 * This tells add_menu_item() that the new item is a soft drink.
 */
#define PUB_SOFTDRINK   4
/**
 * This tells add_menu_item() that the new item is a hot drink.
 */
#define PUB_HOTDRINK    5

/**
 * This is the path where the menu file is held.
 */
#define PUB_MENU_FILE     "/obj/misc/menu"
/**
 * This is the path where the counter file is held.
 */
#define PUB_COUNTER_FILE  "/std/shops/objs/counter"

/**
 * This is the path where the standard glasses are held.
 */
#define PUB_GLASS_PATH    "/obj/misc/glasses/"

/**
 * This is the path for the standard tankard.
 * Volume 4000.
 */
#define PUB_STD_TANKARD   PUB_GLASS_PATH + "std_tankard.ob"
/**
 * This is the path for the standard pint glass.
 * Volume 2200.
 */
#define PUB_STD_PINT      PUB_GLASS_PATH + "std_pint_glass.ob"
/**
 * This is the path for the standard half-pint glass.
 * Volume 1100.
 */
#define PUB_STD_HALFPINT  PUB_GLASS_PATH + "std_half_glass.ob"
/**
 * This is the path for the standard shot glass.
 * Volume 200.
 */
#define PUB_STD_SHOT      PUB_GLASS_PATH + "std_shot_glass.ob"
/**
 * This is the path for the standard wine glass.
 * Volume 960.
 */
#define PUB_STD_WINE      PUB_GLASS_PATH + "std_wine_glass.ob"
/**
 * This is the path for the standard brandy glass.
 * Volume 700.
 */
#define PUB_STD_BRANDY    PUB_GLASS_PATH + "std_brandy_glass.ob"
/**
 * This is the path for the standard tumbler.
 * Volume 1100.
 */
#define PUB_STD_TUMBLER   PUB_GLASS_PATH + "std_tumbler.ob"
/**
 * This is the path for the standard cocktail glass.
 * Volume 750.
 */
#define PUB_STD_COCKTAIL  PUB_GLASS_PATH + "std_cocktail_glass.ob"
/**
 * This is the path for the standard flute glass (champagne, etc).
 * Volume 750.
 */
#define PUB_STD_FLUTE     PUB_GLASS_PATH + "std_flute_glass.ob"
/**
 * This is the path for the standard teacup.
 * Volume 550.
 */
#define PUB_STD_TEACUP    PUB_GLASS_PATH + "std_teacup.ob"
/**
 * This is the path for the standard mug.
 * Volume 800.
 */
#define PUB_STD_MUG       PUB_GLASS_PATH + "std_mug.ob"

/**
 * This is the path where the standard plates are held.
 */
#define PUB_PLATE_PATH    "/obj/misc/plates/"

/**
 * This is the path for the standard plate.
 * Maximum weight 50.
 */
#define PUB_STD_PLATE         PUB_PLATE_PATH + "std_plate.ob"
/**
 * This is the path for the standard dish.
 * Maximum weight 30.
 */
#define PUB_STD_DISH          PUB_PLATE_PATH + "std_dish.ob"
/**
 * This is the path for the standard bowl.
 * Maximum weight 30.
 */
#define PUB_STD_BOWL          PUB_PLATE_PATH + "std_bowl.ob"
/**
 * This is the path for the standard soup dish.
 * Maximum weight 30.
 */
#define PUB_STD_SOUPDISH      PUB_PLATE_PATH + "std_soup_dish.ob"
/**
 * This is the path for the standard dinner plate.
 * Maximum weight 50.
 */
#define PUB_STD_DINNERPLATE   PUB_PLATE_PATH + "std_dinner_plate.ob"
/**
 * This is the path for the standard side plate.
 * Maximum weight 30.
 */
#define PUB_STD_SIDEPLATE     PUB_PLATE_PATH + "std_side_plate.ob"
/**
 * This is the path for the standard dessert plate.
 * Maximum weight 30.
 */
#define PUB_STD_DESSERTPLATE  PUB_PLATE_PATH + "std_dessert_plate.ob"
/**
 * This is the path for the standard dessert dish.
 * Maximum weight 30.
 */
#define PUB_STD_DESSERTDISH   PUB_PLATE_PATH + "std_dessert_dish.ob"

/**
 * This is the class used for all the menu items.
 * @element type The type of the item, as defined in this file
 * @element cost The cost of the item
 * @element item The path to use to get the item from
 * @element container The path to use to get the container from
 * @element volume The volume of the liquid to put in the glass
 * @element intox The intoxication parameter, this is used by the
 * npcs for drinking themselves silly
 */
class menu_item {
   int type;
   int cost;
   string item;
   string container;
   int volume;
   int intox;
}

/**
 * This is the event called when a specific item is not on the menu.  The
 * parameters to this are the shop, the player and the item the player asked
 * for.
 */
#define PUB_EVENT_NOT_AVAILABLE "not_available"

/**
 * This is the event called when the shop is not open.
 */
#define PUB_EVENT_NOT_OPEN "not_open"

/**
 * This is the event called when some stuff is bought.  The parameter to this
 * are the shop, the player and the people (object*) for who the stuff was
 * bought and the second (string)
 * parameter is
 * the stuff that was bought.
 */
#define PUB_EVENT_BOUGHT_STUFF "bought_stuff"

/**
 * This is the event called when someonthing was attempted to be sold to some
 * dead people. The parameter to this
 * are the shop, the player and the people (object*) for who the stuff was
 * bought and the second (string)
 * parameter is
 * the stuff that was bought.
 */
#define PUB_EVENT_DEAD_PEOPLE "dead_people"

/**
 * This is the event called when the people were unable to be sold something
 * The parameter to this
 * are the shop, the player and the people (object*) for who the stuff was
 * bought and the second (string)
 * parameter is
 * the stuff that was bought.
 */
#define PUB_EVENT_FAILED_PEOPLE "failed_people"

/**
 * This is the event called when you are too poor to buy the stuff.
 * The parameter to this
 * are the shop, the player and the people (object*) for who the stuff was
 * bought and the second (string)
 * parameter is
 * the stuff that was bought.
 */
#define PUB_EVENT_POOR_PERSON "poor_people"


#endif /* __SYS__PUB_SHOP */
