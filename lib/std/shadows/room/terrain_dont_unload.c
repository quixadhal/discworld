inherit "/std/effect_shadow";

#define BP tell_object( find_player("gerbil"), "BP.\n");

int i;

int dont_free() {return 1;}

void setup_shad() {
    i++;
}

string test_my_effect() {return "Yup!";}

void ditch_room() {
    if( !--i ) {
        remove_this_effect();
    }
}
