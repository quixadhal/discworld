/**
 * This is the standard punishment file for a verdict of not guilty.
 * @author Pinkfish
 * @started Tue Sep 19 14:37:28 PDT 2000
 */
#include <nomic_system.h>

inherit "/std/room/inherit/punishment_inherit";

void setup() {
   set_short("not guilty");
   set_description_file(NOMIC_PUNISHMENT_DOCS_DIR + "not_guilty");
} /* setup() */
