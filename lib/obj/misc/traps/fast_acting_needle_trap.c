inherit "/obj/misc/trap";

void setup() {
   set_name("trap");
   set_weight(1);
   set_short("fast-acting needle trap");
   add_adjective(({ "fast-acting", "fast", "acting", "needle" }));
   set_long("This is a needle trap suitable for rigging inside "
            "the lock of a door. It uses a small needle that "
            "will inject a fast acting poison into anyone "
            "clumsily picking "
            "the lock.\n");
   make_trap(250, "pick",
             "Hidden inside the lock is a small mechanism "
             "which, if touched by an clumsily wielded tool "
             "will cause a small needle to swing out and poison "
             "an unwary thief.\n",
             ({"A small needle swings out of the lock injecting "
                 "poison.\n",
                 "A small needle sings out of the lock.\n" }),
             500);
   set_value(80000);
} /* setup() */
