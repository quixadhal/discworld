inherit "/obj/misc/trap";

// Note: This trap is not suitable to be sold for players to add to their
// doors since it requires a hold in the floor and so would have to be
// custom installed!
// Ceres (no, I did not write it!)

void setup() {
   set_name("trap");
   set_weight(1);
   set_short("leg trap");
   add_adjective("leg");
   set_long("This trap has a trigger suitable for rigging inside "
            "the lock of a door. It uses a small portion of the floor "
            "in front of the door to conceal a blunt leg trap that will "
            "shatter the femurs of any clumsy thief that attempt to "
            "pass through.\n");
   make_trap(350, "pick",
             "Hidden inside the lock is a trigger that leads down to a "
             "concealed panel on the floor. Carefully lifting the panel "
             "you see a vicious blunt man trap, designed to break any "
             "unskilled questionable entrant.\n",
             ({"A small click is heard, and the floor beneath your feet "
               "snaps away to reveal a leg trap which snaps together "
               "violently, shattering both your legs at once!\n",
               "A small click is heard and the jaws of a man trap burst out "
               "of the ground below " + this_player()->query_short() + "'s "
               "feet and shatters both of their legs!\n" }),
             ({ "/std/effects/disease/broken_leg", ({ "left", "right"}) }));
   set_value(1000000);
} /* setup() */
