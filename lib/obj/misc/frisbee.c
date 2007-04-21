inherit "/std/object";

string me, him;

void setup() {
  set_name("frisbee");
  set_short("black frisbee");
  add_adjective("black");
  set_long("This is a small black frisbee.  It looks like you could "
           "throw it to "
           "people.\n");
  add_property("feeding_frenzy", "frisbee");
  set_weight(10);
  set_value(200);
  set_main_plural("black frisbees");
  add_plural("frisbees");
} /* setup() */

void init() {
  this_player()->add_command("throw", this_object(),
                             "<direct:object:me> to <indirect:player>");
} /* init() */

int do_throw(object *in_dir) {
  /* Make sure there really is someone in that array */
  if (!sizeof(in_dir))
    return 0;

  /*
   * Ok, move it to the destination.  Don't need to a print a message to
   * this_player as that is done automaticly.
   */
  if (move(in_dir[0])) {
    tell_object(in_dir[0], this_player()->one_short()+" throws "+
                this_object()->short()+" at you, but it falls on the floor.\n");
    tell_room(environment(in_dir[0]), "A frisbee arcs through the air, "+
              in_dir[0]->query_name()+" tries to catch it but fails and it "+
              "falls on the floor.\n", ({ in_dir[0], this_player() }) );
    move(environment(in_dir[0]));
  } else {
    tell_object(in_dir[0], this_player()->one_short()+" throws a frisbee "+
                "at you, you catch it... just.\n");
    tell_room(environment(in_dir[0]), in_dir[0]->one_short()+
          " fumbles the catch of the "+
          short(0)+" but manages to keep hold of it.\n",
          ({ in_dir[0], this_player() }));
  }
  this_player()->add_succeeded(in_dir[0..0]);
  return 1;
} /* do_throw() */
