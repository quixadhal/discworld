inherit "/obj/misc/lock";

void setup() {
   set_name("lock");
   set_short("standard furniture lock");
   add_adjective( ({ "standard", "furniture" }));
   set_long("A standard brass furniture lock.\n");
   add_property("difficulty", 2);
   set_weight(19);
   set_value(10000);
}

void set_key_number(int num) {
   add_property("key prop", "furniture_lock_"+num);
   add_property("locktype", "furniture");
}
