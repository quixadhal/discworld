inherit "/obj/handlers/inherit/player_council";
#define NOMIC_SYSTEM_NO_CLASSES
#include <nomic_system.h>

//#define SAVE_FILE "/save/nomic/main.o"

void load_me();
void save_me();

void create() {
   player_council::create();

   load_me();
} /* create() */

void save_me() {
   unguarded( (: save_object(NOMIC_SYSTEM_SAVE_FILE) :) );
} /* save_me() */

void load_me() {
   unguarded( (: restore_object(NOMIC_SYSTEM_SAVE_FILE, 1) :) );
} /* load_me() */

void dest_me() {
   destruct(this_object());
} /* dest_me() */
