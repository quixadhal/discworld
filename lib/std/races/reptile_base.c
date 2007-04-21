inherit "/std/races/base";

void create() {
   do_setup++;
   ::create();
   do_setup--;

   set_desc( "your standard reptile.  This should not be used as it is a "
      "base race" );

   bits = ({ 
      "head", "head", ({ 0, 50, 0,
         "left eye", "right eye", "skull", "tongue", "snout" }),
      "left eye", "eye", ({ "head", 2, 0 }),
      "right eye", "eye", ({ "head", 2, 0 }),
      "skull", "skull", ({ "head", 50, 0, "brain", "teeth" }),
      "brain", "brain", ({ "skull", 25, 0 }),
      "teeth", "teeth", ({ "skull", 5, ({"/std/bit_set", 1}), "tooth" }),
      "tooth", "tooth", ({ "teeth", 1, ({"/std/bit", 48}) }),
      "tongue", "tongue", ({ "head", 10, 0 }),
      "snout", "snout", ({ "head", 10, 0 }),
      "neck", "neck", ({ 0, 50, 0 }),
      "torso", "body", ({ 0, 400, 0,
         "heart", "liver", "left kidney", "right kidney", "left lung",
         "right lung", "spleen", "intestine", "pancreas" }),
      "heart", "heart", ({ "torso", 15, 0 }),
      "liver", "liver", ({ "torso", 15, 0 }),
      "left kidney", "kidney", ({ "torso", 10, 0 }),
      "right kidney", "kidney", ({ "torso", 10, 0 }),
      "left lung", "lung", ({ "torso", 15, 0 }),
      "right lung", "lung", ({ "torso", 15, 0 }),
      "spleen", "spleen", ({ "torso", 10, 0 }),
      "intestine", "gut", ({ "torso", 100, 0 }),
      "pancreas", "pancreas", ({ "torso", 10, 0 }),
      "left front leg", "leg", ({ 0, 180, 0, "left front foot" }),
      "left front foot", "foot", ({ "left front leg", 27, 0 }),
      "right front leg", "leg", ({ 0, 180, 0, "right front foot" }),
      "right front foot", "foot", ({ "right front leg", 27, 0 }),
      "left rear leg", "leg", ({ 0, 180, 0, "left rear foot" }),
      "left rear foot", "foot", ({ "left rear leg", 27, 0 }),
      "right rear leg", "leg", ({ 0, 180, 0, "right rear foot" }),
      "right rear foot", "foot", ({ "right rear leg", 27, 0 }),
      "tail", "tail", ({ 0, 100, 0 }),
      "skin", "skin", ({ 0, 175, 0 })
   });

   inedible = ({ "skull", "left front foot", "right front foot",
      "left rear foot", "right rear foot", "tail", "teeth", "tooth",
      "skin" });
   unrottable = ({ "skull", "teeth", "tooth" });
   set_skin( "scales" );

   if( !do_setup )
      this_object()->setup();
} /* create() */
