inherit "/std/effects/basic";

create()
{
   set_boundaries(
      ({ -10, -10 }), ({ 10, 10 }) );
}

backdrop() { return 1; }

/* defaults are fine for everything else */
