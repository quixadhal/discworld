inherit "/std/object";

string me, him;

void setup() {
  set_name("frisbee");
  set_short("black frisbee");
  add_adjective("black");
  set_long("A small black frisbee.  Looks like you could throw it to "+
           "people.\n");
  add_property("feeding_frenzy", "frisbee");
  set_weight(20);
  set_value(200);
  set_main_plural("black frisbees");
  add_plural("frisbees");
}

void init() {
  add_action("throw", "throw");
  ::init();
}

int throw(string str) {
  object *from, *dest;
  int i, j;

  notify_fail("What are you going to throw?\n");
  if (!str)
    return 0;
  if (sscanf(str, "%s at %s", me, him ) != 2 &&
      sscanf(str, "%s to %s", me, him ) != 2 &&
      sscanf(str, "%s the %s", him, me) != 2 &&
      sscanf(str, "%s %s", me, him) != 2)
    return 0;
  him = (string)this_player()->expand_nickname(him);
  me = (string)this_player()->expand_nickname(me);
  from = find_match(me, this_player()) +
         find_match(me, environment(this_player()));
  dest = find_match(him, this_player()) +
         find_match(him, environment(this_player()));
  if (!sizeof(dest))
    if (!find_living(him))
      return 0;
    else
      dest = ({ find_living(him) });
  for (i=0;i<sizeof(from);i++)
    for (j=0;j<sizeof(dest);j++)
      if (!from[i]->do_throw(dest[j]))
        write("You cannot throw "+from[i]->short()+".\n");
  return 1;
}

int do_throw(object dest) {
  if (environment() != this_player()) {
    write("You must have the "+short(0)+" to throw it.\n");
    return 1;
  }
  say(this_player()->query_cap_name()+" throws "+me+" to "+him+"\n",
      ({ dest, this_player() }));
  write("You throw the frisbee to "+him+".\n");
  if (move(dest)) {
    tell_object(dest, this_player()->query_cap_name()+" throws "+
                this_object()->short()+" at you, but it falls on the floor.\n");
    tell_room(environment(dest), "A frisbee arcs through the air, "+
              dest->query_name()+" trys to catch it but fails and it "+
              "fall on the floor", ({ dest, this_player() }) );
    move(environment(dest));
  } else {
    tell_object(dest, this_player()->query_cap_name()+" throws a frisbee "+
                "at you, you catch it... just.\n");
    tell_room(environment(dest), dest->query_cap_name()+
          " fumbles the catch of the "+
          short(0)+" but manages to keep hold of it.\n",
          ({ dest, this_player() }));
  }
  return 1;
}
