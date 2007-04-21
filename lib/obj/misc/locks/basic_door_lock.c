inherit "/obj/misc/lock";

void setup() {
   set_name("lock");
   set_short("basic door lock");
   add_adjective( ({ "basic", "door" }));
   set_long("A basic brass door lock.\n");
   add_property("difficulty", 2);
   set_weight(19);
   set_value(20000);
}

void set_key_number(int num) {
   add_property("key prop", "door_lock_"+num);
   add_property("locktype", "door");
}
