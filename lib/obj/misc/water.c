inherit "/std/object";
#include "move_failures.h"

object my_ob;

void create() {
  set_name("liquid");
}

void set_my_ob(object ob) { my_ob = ob; }
object query_my_ob() { return my_ob; }

move(dest, str, str1) {
  if (!dest->query_property("watertight")) {
    write("The liquid drips all over the floor and disappears.\n");
    say(this_player()->short()+" trys to put some liquid into "+dest->short()+
        " and it dribbles all over the floor");
    call_out("dest_me", 0);
    return MOVE_OK;
  } else {
    dest->mix_up(my_ob);
    call_out("dest_me", 0);
  }
  return ::move(dest, str, str1);
}
