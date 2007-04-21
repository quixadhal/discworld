#ifndef __SYS__DIRS
#define __SYS__DIRS

#define STD_ORDERS ({ \
  "north", ({ -1, 0, 0 }), "south", ({ 1, 0, 0 }), \
  "east", ({ 0, -1, 0 }), "west", ({ 0, 1, 0 }), \
  "northeast", ({ -1, -1, 0 }), "southwest", ({ 1, 1, 0 }), \
  "southeast", ({ 1, -1, 0 }), "northwest", ({ -1, 1, 0 }), \
  "up", ({ 0, 0, -1 }), "down", ({ 0, 0, 1 }) })
#define REL_DIRS ({ \
   "forward", "up ahead", "in front of you", \
   "forward-right", "ahead and to the right", "in front and to your right", \
   "right", "the right", "to your right", \
   "backward-right", "behind you and to the right", "behind you and to your right", \
   "backward", "behind you", "behind you", \
   "backward-left", "behind you and to the left", "behind you and to your left", \
   "left", "the left", "to your left", \
   "forward-left", "ahead and to the left", "in front and to your left" })
#define ABS_FACING ([ \
  "north":8, "northeast":1, \
  "east":2, "southeast":3, \
  "south":4, "southwest":5, \
  "west":6, "northwest":7 ])
#define SHORTEN ([ \
  "north":"n", "south":"s", \
  "east":"e", "west":"w", \
  "northeast":"ne", "southwest":"sw", \
  "southeast":"se", "northwest":"nw", \
  "up":"u", "down":"d", \
  "hubward":"hw", "rimward":"rw", \
  "widdershins":"ws", "turnwise":"tw", \
  "forward":"fw", "forward-right":"fr", \
  "right":"rt", "backward-right":"br", \
  "backward":"bw", "backward-left":"bl", \
  "left":"lt", "forward-left":"fl" ])
#define LENGTHEN ([ \
  "n":"north", "s":"south", \
  "e":"east", "w":"west", \
  "ne":"northeast", "sw":"southwest", \
  "se":"southeast", "nw":"northwest", \
  "u":"up", "d":"down", \
  "hw":"hubward", "rw":"rimward", \
  "ws":"widdershins", "tw":"turnwise", \
  "fw":"forward", "fr":"forward-right", \
  "rt":"right", "br":"backward-right", \
  "bw":"backward", "bl":"backward-left", \
  "lt":"left", "fl":"forward-left" ])

#define EXPAND_EXIT(x) LENGTHEN[x]?LENGTHEN[x]:x

#endif /* __SYS__DIRS */
