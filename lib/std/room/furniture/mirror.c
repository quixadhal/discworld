/**
 * This is a mirror inheritable.
 * @author Ceres
 */
#include <effect.h>

inherit "/std/room/furniture/basic";

int _in_extra;

void create(){
	::create();
	add_extra_look(this_object());
}

string extra_look(object ob) {
	int dark;
	object env;

	env = environment(ob);
	if(living(env))
		return "You can see yourself in the mirror.";
	
	dark = (int) this_player()->check_dark((int)env->query_light());
	if(dark)
		return "It is too dark to see much in the room.";

	return "Reflected in the mirror is " +
		query_multiple_short(all_inventory(env) - ({ ob }), "a", 0, 1, dark) +
		".";
}
