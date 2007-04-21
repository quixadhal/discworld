
inherit "/std/object";

string me, him;
void setup() {
     set_name("football");
     set_short("leather football");
     add_adjective("leather");
     add_alias("ball");
     set_long("This is a leather football, pumped up hard.  Looks like you could"
             " kick it to people.\n");
     set_weight(10);
     set_value(1);
     add_plural("footballs");
     add_plural("balls");
} /* setup() */

void init() {
  this_player()->add_command("kick", this_object(),
        "<direct:object:me> to <indirect:player>");
  add_command( "kickup", "<direct:object>" );
  /*  ::init(); */
} /* init() */

int do_kickup()  {
   write( "You kick the football into the air a few times, stopping it from "
   "hitting the floor by bouncing it off your knee, your chest and your feet."
   "  A rare display of skill!\n" );
tell_room( environment( this_player() ),
     this_player()->query_cap_name() + " expertly kicks the ball into the "
     "air, heads it back up, bounces it a few times on their chest and displays "
     "an uncanny level of skill.\n", this_player());
   return 1;
}

int do_kick(object *in_dir) {
  if (!sizeof(in_dir))
    return 0;

  if (move(in_dir[0])) {
    tell_object(in_dir[0], this_player()->query_cap_name()+" kicks "+
        this_object()->query_short()+" at you, but it falls on the floor.\n");
    tell_room(environment(in_dir[0]), "A leather football flies "
        "through the air, "+ in_dir[0]->query_name()+" tries to trap it "
        "but fails and it lands next to them.", ({ in_dir[0], this_player() }) );
    move(environment(in_dir[0]));
  } else {
    tell_object(in_dir[0], this_player()->query_cap_name()+" kicks a "
           "leather football at you.  It curls towards you at speed but you "
           "manage to " +
           ({ "trap it with your foot.",
           "bounce it off your chest and trap it underfoot.",
           "head it to the ground, juggle it a few times and eventually stand"
           " on it",
           "catch it. HANDBALL!",
           "look like the famous footballer Fele and trap it.",
           "trap it easily.  You should sign up for Ankh Morpork Wanderers."})
           [random (6) ]+"\n"); 
    tell_room(environment(in_dir[0]), "A leather football curls "
           "gracefully towards "+ in_dir[0]->query_cap_name()+", who "+
                "manages to "+
                ({ "trap it with their foot.",
           "bounce it off their chest and trap it underfoot.",
           "head it to the ground, juggle it a few times and eventually stands"
           " on it",
           "catches it. HANDBALL!",
           "look like the famous footballer Fele and trap it.",
           "trap it easily.  They should sign up for Ankh Morpork Wanderers."})
           [random (6) ]+
                "\n",
          ({ in_dir[0], this_player() }));
  }
  this_player()->add_succeeded(in_dir[0..0]);
  return 1;
} /* do_kick() */
