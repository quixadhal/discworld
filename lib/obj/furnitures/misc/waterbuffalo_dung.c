
inherit "/std/room/furniture/fuel_inherit";

void setup() {

  set_name("dung");
  set_shorts( ({ "small lump of dried waterbuffalo dung",
                 "slightly burnt lump of waterbuffalo dung",
                 "blackened lump of waterbuffalo dung",
                 "black, charred lump of dung" }) );

  set_longs( ({ 
    "This is a small lump of dried waterbuffalo dung.  It looks as "
        "though it would burn well.\n",
    "This is a small lump of dried waterbuffalo dung.  It has been "
        "charred around the edges but would still burn for a while yet.\n",
    "This is a black lump of charred dung, which is barely able to still "
        "hold itself together.\n" }) );

  set_weight( 9 );
  set_value( 200 );

} /* setup() */
