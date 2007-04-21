/*  Arachnid  *
 *   Gototh   *
 *  23/11/97  */

inherit "/std/races/base";

void create() {
   do_setup++;
   ::create();
   do_setup--;

   set_desc( "A creature like scorpion, spider, daddy longlegs, mite, "
      "tick, and certain other invertebrate animals.  This file should "
      "be inherited by a more specialised race object." );

   bits = ({
      "cephalothorax", "head", ({ 0, 200, 0,
         "middle right upper eye", "middle right lower eye",
         "middle left upper eye", "middle left lower eye",
         "right upper eye", "right lower eye",
         "left upper eye", "left lower eye", "brain", "ganglion",
         "left fang", "right fang", "stomach", "mandible" }),
      "middle right upper eye", "eye", ({ "cephalothorax", 25, 0 }),
      "middle right lower eye", "eye", ({ "cephalothorax", 25, 0 }),
      "middle left upper eye", "eye", ({ "cephalothorax", 25, 0 }),
      "middle left lower eye", "eye", ({ "cephalothorax", 25, 0 }),
      "right upper eye", "eye", ({ "cephalothorax", 25, 0 }),
      "right lower eye", "eye", ({ "cephalothorax", 25, 0 }),
      "left upper eye", "eye", ({ "cephalothorax", 25, 0 }),
      "left lower eye", "eye", ({ "cephalothorax", 25, 0 }),
      "brain", "brain", ({ "cephalothorax", 25, 0 }),
      "ganglion", "ganglion", ({ "cephalothorax", 50, 0 }),
      "left fang", "fang", ({ "cephalothorax", 25, 0 }),
      "right fang", "fang", ({ "cephalothorax", 25, 0 }),
      "stomach", "stomach", ({ "cephalothorax", 50, 0 }),
      "mandible", "mandible", ({ "cephalothorax", 25, 0 }),
      "abdomen", "abdomen", ({ 0, 500, 0, "lung", "stomach", 
         "heart", "right front leg", "right middle front leg", 
         "left front leg", "left middle front leg", "right back leg", 
         "right middle back leg", "left back leg", "left middle back leg" }),
      "left middle back leg", "leg", ({ "abdomen", 25, 0 }),
      "left back leg", "leg", ({ "abdomen", 25, 0 }),
      "right middle back leg", "leg", ({ "addomen", 25, 0 }),
      "right back leg", "leg", ({ "abdomen", 25, 0 }),
      "right middle front leg", "leg", ({ "abdomen", 25, 0 }),
      "right front leg", "leg", ({ "abdomen", 25, 0 }),
      "left middle front leg", "leg", ({ "abdomen", 25, 0 }),
      "left front leg", "leg", ({ "abdomen", 25, 0 }),
      "lung", "lung", ({ "abdomen", 100, 0 }),
      "stomach", "stomach", ({ "abdomen", 100, 0 }),
      "heart", "heart", ({ "abdomen", 100, 0 }),
      "pedicel", "pedicel", ({ 0, 100, 0 })
   });

   inedible = ({ "left fang", "right fang", "mandible" });
   unrottable = ({ "left fang", "right fang", "mandible" });

   set_stats( ({ -6, 4, -6, -6, -6 }) );

   if( !do_setup )
      this_object()->setup();
} /* create() */
