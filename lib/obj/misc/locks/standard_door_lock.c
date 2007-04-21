inherit "/obj/misc/lock";

void setup() {
   set_name("lock");
   set_short("standard door lock");
   add_adjective( ({ "standard", "door" }));
   set_long("A standard brass door lock.\n");
   add_property("difficulty", 4);
   set_weight(19);
   set_value(40000);
}

void set_key_number(int num) {
   add_property("key prop", "door_lock_"+num);
   add_property("locktype", "door");
}
