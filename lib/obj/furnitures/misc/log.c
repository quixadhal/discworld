// Aquilo //

#define WEIGHT 25

inherit "/std/room/furniture/fuel_inherit";

void setup(){

  set_name("log");
  set_shorts( ({ "small log",
                 "slightly burnt log",
                 "blackened log",
                 "black, charred log" }) );

  set_longs( ({ 
    "This is a small dumpy log.  It looks as though it would burn well.\n",
    "This is a small dumpy log.  It has been charred around the edges "
      "but would still burn for a while yet.\n",
    "This is a black lump of charred wood, which is just about holding "
      "its shape together as a log.\n" }) );

  set_weight( WEIGHT );
  set_value( 400 );

}
