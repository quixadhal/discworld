inherit "/obj/misc/lock";

void setup() {
   set_name("lock");
   set_short("flimsy door lock");
   add_adjective( ({ "flimsy", "door" }));
   set_long("A flimsy brass door lock.\n");
   add_property("difficulty", 1);
   set_weight(19);
   set_value(10000);
}

void set_key_number(int num) {
   add_property("key prop", "door_lock_"+num);
   add_property("locktype", "door");
}
