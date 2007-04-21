inherit "/obj/misc/lock";

void setup() {
   set_name("lock");
   set_short("chubby ultra door lock");
   add_adjective( ({ "ultra", "chubby", "door" }));
   set_long("A chubby ultra brass door lock.  These high-quality "
                     "locks are made by Grabthroat Shinkicker and Sons.  "
                     "Contrary to popular belief the locks are called "
                     "chubby's not because of their thickness but because "
                     "Grabthroat's real name is Charles and his "
                     "nickname is Chubby.\n");
   add_property("difficulty", 10);
   set_weight(19);
   set_value(300000);
}

void set_key_number(int num) {
   add_property("key prop", "door_lock_"+num);
   add_property("locktype", "door");
}
