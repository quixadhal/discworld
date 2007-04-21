inherit "/obj/misc/trap";

void setup() {
   set_name("trap");
   set_weight(1);
   set_short("needle trap");
   add_adjective("needle");
   set_long("This is a needle trap suitable for rigging inside "
            "the lock of a door. It uses a small needle that "
            "will inject poison into anyone clumsily picking "
            "the lock.\n");
   make_trap(200, "pick",
             "Hidden inside the lock is a small mechanism "
             "which, if touched by an clumsily wielded tool "
             "will cause a small needle to swing out and poison "
             "an unwary thief.\n",
             ({"A small needle swings out of the lock injecting "
                 "poison.\n",
                 "A small needle sings out of the lock.\n" }),
             ({ "/std/effects/poisons/spider_venom", 30 }));
   set_value(20000);
} /* setup() */
