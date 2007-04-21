// Rue 5.2000 from Nassar's idea

inherit "/std/room/furniture/clock";

void setup() {
  set_name("clock");
  set_short("owl clock");
  add_adjective("owl");
  set_long("This is a wooden clock carved roughly into the shape of an owl.  "
    "The tail twitches back and forth, somewhat like a pendulum, and the owl's "
    "eyes roll cheerfully left and right with each tick of the clock.  A small "
    "set of tiles, strung on a metal rod and affixed to the owl's feet, tells "
    "the date.\n"+
    "According to the clock it is ");
  set_allowed_positions(({}));
  set_allowed_room_verbs((["hanging" : "hangs" ]));
  set_weight(50);
  set_value(42000);
  add_property("no recycling", 1); 
  set_chimes(2, "The owl clock opens its beak and hoots twice.\n");
  set_chimes(1, "The owl clock opens its beak and hoots once.\n");
  set_show_date();
}

void chime(string mess) {
  int *co_ords;

  if(environment(this_object())) {

    co_ords = (int *)environment(this_object())->query_co_ord();
    
    "/obj/handlers/broadcaster"->broadcast_event(users(), co_ords,
                                                 mess, 20, 1, 0 );
    tell_room(environment(this_object()), mess);
  }
}

