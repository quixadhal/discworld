#define PATH "/w/ceres/mtf-v1.1/"

/*MISCELLANEOUS */

#define VERSION v1.1

/* ERROR CODES */

#define NO_MAP_FILE -1
#define NO_FILE_DIR -2
#define NO_KEY_FILE -3

/* DEFAULT FILES */

#define DEFAULT_KEYS PATH+"key_dft.c"

#define SPECIALS ({ "*", "+", "x", "-", "|", "\\", "/" })

/* DIRECTION STUFF FOR EXITS */

#define EXITS_NS ({ "north", "", "south" })
#define EXITS_WE ({ "west", "", "east" })

#define DIRECTIONS ({ ({ "northwest", "north", "northeast", }), ({ "west", "", "east", }), ({ "southwest", "south", "southeast" }) })
#define LIST ({"north", "northeast", "east", "southeast", "south", "southwest", "west", "northwest" })
