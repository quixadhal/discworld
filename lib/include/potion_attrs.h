#ifndef __SYS__POTION_ATTRS
#define __SYS__POTION_ATTRS

/* constants for potion attributes */

#define POTION_SPACE_HANDLER "/obj/handlers/potion_space"

/* number of elements in the potion_attribute struct */
#define POTION_ATTRS_SIZE 6

#define POTION_CONSISTENCY 0     /* 0 = waterlike, 50 = honey, 100 = solid */
#define POTION_TRANSPARENCY 1    /* 0 = opaque, 50 = cloudy, 100 = water */
#define POTION_NAMES 2           /* ({ ({ "chunky soup", 100 }) }) */
#define POTION_COLOURS 3         /* ({ ({ "green", 20 }), ({ "yellow", 80 }) }) */
#define POTION_SMELLS 4          /* ({ ({ "fish", 80 }), ({ "onion", 40 }) }) */
#define POTION_FLAVOURS 5        /* ({ ({ "fish", 60 }), ({ "garlic", 30 }), ({ "onion", 20 }) }) */

#define VERY_SMALL_AMOUNT 5
#define SMALL_AMOUNT 10
#define MODERATE_AMOUNT 25

#define OC_VOL 0
#define OC_MVOL 1
#define OC_WVOL 2
#define OC_MAXVOL 3
#define OC_LEAKRATE 4
#define OC_PSQUANT 5
#define OC_MISCATTRS 6
#define OC_PSCOORD 7

#endif /* __SYS__POTION_ATTRS */
