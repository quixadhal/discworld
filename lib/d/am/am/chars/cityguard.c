#include "path.h"
inherit defmonster;
 
setup() {
        set_level(8);
        set_name("guard");
        set_short("City guard");
        set_long("This is a city guard.  He is supposed to protect the city from "+
                 "undesirables.  Sadly Anhk Morpork isn't very fussy and he is almost "+
                 "asleep.\n");
        set_race("human");
	set_al(50);
        give_money(150,100);
        load_chat(10, ({
          1, "@snore",
          1, ":snoozes.\n",
          1, ":moves in his sleep.\n"
          }) );
        load_a_chat(50,({
                1, "'Ouch!\n",
                1, "'Come on.  I don't need this!\n",
                1, "'Leave me alone!\n",
                1, "'Just Rack off!\n",
                1, "'I gave at the office.\n"
        }));
}
