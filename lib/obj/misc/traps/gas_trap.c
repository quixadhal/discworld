inherit "/obj/misc/trap";

void setup() {
   set_name("trap");
   set_weight(1);
   set_short("gas trap");
   add_adjective("gas");
   set_long("This is a gas trap suitable for rigging inside "
            "the lock of a door. It uses a small ampule of "
            "poison gas that is ejected by someone attempting "
            "to pick the lock.\n");
   make_trap(300, "pick",
             "Hidden inside the lock is a small ampule which, "
             "if touched by an unwary tool would trigger a "
             "puff of poisonous gas.",
             ({"A small cloud of gas puffs out of the lock.\n",
                 "A small cloud of gas puffs out of the lock.\n"}),
             ({"/std/effects/poisons/poison_gas", 600}));

   set_value(120000);
} /* setup() */
