.DT
pub_shop.h
Disk World autodoc help
pub_shop.h

.SH Description
.SP 5 5

This is the main include file for the pub_shop.c inheritable.
The file reaction.h contains a series of defines for sizes of
containers.
.EP
.SP 10 5


Written by Lemming

Started 23/10/1999
.EP



.SH See also
.SP 5 5
/include/reaction.h

.EP
.SH Classes

.SI 3
* %^BOLD%^menu_item%^RESET%^
class menu_item {
int type;
int cost;
string item;
string container;
int volume;
int intox;
}

.EI
.SP 7 5

This is the class used for all the menu items.
.EP


.SH Defines
.SI 3
* %^BOLD%^PUB_ALCOHOL%^RESET%^
.EI
.SP 7 5

This tells add_menu_item() that the new item is an alcoholic drink.

.EP

.SI 3
* %^BOLD%^PUB_APPETISER%^RESET%^
.EI
.SP 7 5

This tells add_menu_item() that the new item is an appetiser.

.EP

.SI 3
* %^BOLD%^PUB_COUNTER_FILE%^RESET%^
.EI
.SP 7 5

This is the path where the counter file is held.

.EP

.SI 3
* %^BOLD%^PUB_DESSERT%^RESET%^
.EI
.SP 7 5

This tells add_menu_item() that the new item is a dessert.

.EP

.SI 3
* %^BOLD%^PUB_EVENT_BOUGHT_STUFF%^RESET%^
.EI
.SP 7 5

This is the event called when some stuff is bought.  The parameter to this
are the shop, the player and the people (object*) for who the stuff was
bought and the second (string)
parameter is
the stuff that was bought.

.EP

.SI 3
* %^BOLD%^PUB_EVENT_DEAD_PEOPLE%^RESET%^
.EI
.SP 7 5

This is the event called when someonthing was attempted to be sold to some
dead people. The parameter to this
are the shop, the player and the people (object*) for who the stuff was
bought and the second (string)
parameter is
the stuff that was bought.

.EP

.SI 3
* %^BOLD%^PUB_EVENT_FAILED_PEOPLE%^RESET%^
.EI
.SP 7 5

This is the event called when the people were unable to be sold something
The parameter to this
are the shop, the player and the people (object*) for who the stuff was
bought and the second (string)
parameter is
the stuff that was bought.

.EP

.SI 3
* %^BOLD%^PUB_EVENT_NOT_AVAILABLE%^RESET%^
.EI
.SP 7 5

This is the event called when a specific item is not on the menu.  The
parameters to this are the shop, the player and the item the player asked
for.

.EP

.SI 3
* %^BOLD%^PUB_EVENT_NOT_OPEN%^RESET%^
.EI
.SP 7 5

This is the event called when the shop is not open.

.EP

.SI 3
* %^BOLD%^PUB_EVENT_POOR_PERSON%^RESET%^
.EI
.SP 7 5

This is the event called when you are too poor to buy the stuff.
The parameter to this
are the shop, the player and the people (object*) for who the stuff was
bought and the second (string)
parameter is
the stuff that was bought.

.EP

.SI 3
* %^BOLD%^PUB_GLASS_PATH%^RESET%^
.EI
.SP 7 5

This is the path where the standard glasses are held.

.EP

.SI 3
* %^BOLD%^PUB_HOTDRINK%^RESET%^
.EI
.SP 7 5

This tells add_menu_item() that the new item is a hot drink.

.EP

.SI 3
* %^BOLD%^PUB_MAINCOURSE%^RESET%^
.EI
.SP 7 5

This tells add_menu_item() that the new item is a main course.

.EP

.SI 3
* %^BOLD%^PUB_MENU_FILE%^RESET%^
.EI
.SP 7 5

This is the path where the menu file is held.

.EP

.SI 3
* %^BOLD%^PUB_PLATE_PATH%^RESET%^
.EI
.SP 7 5

This is the path where the standard plates are held.

.EP

.SI 3
* %^BOLD%^PUB_SOFTDRINK%^RESET%^
.EI
.SP 7 5

This tells add_menu_item() that the new item is a soft drink.

.EP

.SI 3
* %^BOLD%^PUB_STD_BOWL%^RESET%^
.EI
.SP 7 5

This is the path for the standard bowl.
Maximum weight 30.

.EP

.SI 3
* %^BOLD%^PUB_STD_BRANDY%^RESET%^
.EI
.SP 7 5

This is the path for the standard brandy glass.
Volume 700.

.EP

.SI 3
* %^BOLD%^PUB_STD_COCKTAIL%^RESET%^
.EI
.SP 7 5

This is the path for the standard cocktail glass.
Volume 750.

.EP

.SI 3
* %^BOLD%^PUB_STD_DESSERTDISH%^RESET%^
.EI
.SP 7 5

This is the path for the standard dessert dish.
Maximum weight 30.

.EP

.SI 3
* %^BOLD%^PUB_STD_DESSERTPLATE%^RESET%^
.EI
.SP 7 5

This is the path for the standard dessert plate.
Maximum weight 30.

.EP

.SI 3
* %^BOLD%^PUB_STD_DINNERPLATE%^RESET%^
.EI
.SP 7 5

This is the path for the standard dinner plate.
Maximum weight 50.

.EP

.SI 3
* %^BOLD%^PUB_STD_DISH%^RESET%^
.EI
.SP 7 5

This is the path for the standard dish.
Maximum weight 30.

.EP

.SI 3
* %^BOLD%^PUB_STD_FLUTE%^RESET%^
.EI
.SP 7 5

This is the path for the standard flute glass (champagne, etc).
Volume 750.

.EP

.SI 3
* %^BOLD%^PUB_STD_HALFPINT%^RESET%^
.EI
.SP 7 5

This is the path for the standard half-pint glass.
Volume 1100.

.EP

.SI 3
* %^BOLD%^PUB_STD_MUG%^RESET%^
.EI
.SP 7 5

This is the path for the standard mug.
Volume 800.

.EP

.SI 3
* %^BOLD%^PUB_STD_PINT%^RESET%^
.EI
.SP 7 5

This is the path for the standard pint glass.
Volume 2200.

.EP

.SI 3
* %^BOLD%^PUB_STD_PLATE%^RESET%^
.EI
.SP 7 5

This is the path for the standard plate.
Maximum weight 50.

.EP

.SI 3
* %^BOLD%^PUB_STD_SHOT%^RESET%^
.EI
.SP 7 5

This is the path for the standard shot glass.
Volume 200.

.EP

.SI 3
* %^BOLD%^PUB_STD_SIDEPLATE%^RESET%^
.EI
.SP 7 5

This is the path for the standard side plate.
Maximum weight 30.

.EP

.SI 3
* %^BOLD%^PUB_STD_SOUPDISH%^RESET%^
.EI
.SP 7 5

This is the path for the standard soup dish.
Maximum weight 30.

.EP

.SI 3
* %^BOLD%^PUB_STD_TANKARD%^RESET%^
.EI
.SP 7 5

This is the path for the standard tankard.
Volume 4000.

.EP

.SI 3
* %^BOLD%^PUB_STD_TEACUP%^RESET%^
.EI
.SP 7 5

This is the path for the standard teacup.
Volume 550.

.EP

.SI 3
* %^BOLD%^PUB_STD_TUMBLER%^RESET%^
.EI
.SP 7 5

This is the path for the standard tumbler.
Volume 1100.

.EP

.SI 3
* %^BOLD%^PUB_STD_WINE%^RESET%^
.EI
.SP 7 5

This is the path for the standard wine glass.
Volume 960.

.EP

