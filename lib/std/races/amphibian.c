inherit "/std/races/base";

void create() {
   do_setup++;
   ::create();
   do_setup--;

   set_desc( "your standard amphibian" );
   set_height( 40 );
   set_weight( 600 );

   bits = ({ 
      "head", "head", ({ 0, 40, 0,
         "skull", "left eye", "right eye" }),
      "skull", "skull", ({ "head", 15, 0, "brain" }),
      "left eye", "eye", ({ "head", 1, 0 }),
      "right eye", "eye", ({ "head", 1, 0 }),
      "brain", "brain", ({ "skull", 5, 0 }),
      "torso", "guts", ({ 0, 400, 0,
         "heart", "liver", "left kidney", "right kidney", "spleen" }),
      "heart", "heart", ({ "guts", 10, 0 }),
      "liver", "liver", ({ "guts", 5, 0 }),
      "left kidney", "kidney", ({ "guts", 5, 0 }),
      "right kidney", "kidney", ({ "guts", 5, 0 }),
      "spleen", "spleen", ({ "guts", 5, 0 }),
      "genitals", "genitals", ({ 0, 5, 0 }),
      "right front leg", "leg", ({ 0, 200, 0 }),
      "left front leg", "leg", ({ 0, 200, 0 }),
      "right back leg", "leg", ({ 0, 200, 0 }),
      "left back leg", "leg", ({ 0, 200, 0 }),
      "skin", "skin", ({ 0, 175, 0 })
   });

   inedible = ({ "skull" });
   unrottable = ({ "skull" });
   set_skin( "skin" );

   set_stats( ({ -4, 10, -4, -4, -6 }) );

   if( !do_setup )
      this_object()->setup();
} /* create() */

int lives_in_water() { return 1; }
