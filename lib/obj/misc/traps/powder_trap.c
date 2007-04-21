inherit "/obj/misc/trap";

void setup() {
   set_name("trap");
   set_weight(1);
   set_short("anthrax trap");
   add_adjective("anthrax");
   set_long("This is a anthrax trap suitable for rigging inside "
            "the lock of a door. It uses a small envelope that "
            "will open in the face of anyone clumsily picking "
            "the lock.\n");
   make_trap(200, "pick",
             "Hidden inside the lock is a small envelope "
             "which, if touched by an clumsily wielded tool "
             "will open up and infect an unwary thief.\n",
             ({"A small envelope opens up and a puff of powder "
                "explodes into your face, which you inadvertantly "
                "inhale. The envelope has \"Congratulations! You've "
                "Been Infected With Anthrax!\" written in bold "
                "cheery letters.\n",
                 "A cloud of powder puffs out of the lock.\n" }),
             ({ "/std/effects/disease/common_cold", 6000 }));
   set_value(10000);
} /* setup() */
