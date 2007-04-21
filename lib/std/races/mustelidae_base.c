inherit "/std/races/base";

void create() {
   do_setup++;
   ::create();
   do_setup--;

   set_desc( "the usual mustelidae.  This should not be used as it is a "
      "base race" );

   bits = ({ 
      "head", "head", ({ 0, 40, 0,
         "left eye", "right eye", "left ear", "right ear", "skull",
         "tongue", "nose" }),
      "left eye", "eye", ({ "head", 1, 0 }),
      "right eye", "eye", ({ "head", 1, 0 }),
      "left ear", "ear", ({ "head", 1, 0 }),
      "right ear", "ear", ({ "head", 1, 0 }),
      "skull", "skull", ({ "head", 20, 0, "brain", "teeth" }),
      "brain", "brain", ({ "skull", 20, 0 }),
      "teeth", "teeth", ({ "skull", 1, ({"/std/bit_set", 1}), "tooth" }),
      "tooth", "tooth", ({ "teeth", 1, ({"/std/bit", 20}) }),
      "tongue", "tongue", ({ "head", 2, 0 }),
      "nose", "nose", ({ "head", 1, 0 }),
      "torso", "torso", ({ 0, 350, 0,
         "heart", "liver", "left kidney", "right kidney", "left lung",
         "right lung", "spleen", "intestine" }),
      "heart", "heart", ({ "torso", 10, 0 }),
      "liver", "liver", ({ "torso", 5, 0 }),
      "left kidney", "kidney", ({ "torso", 5, 0 }),
      "right kidney", "kidney", ({ "torso", 5, 0 }),
      "left lung", "lung", ({ "torso", 10, 0 }),
      "right lung", "lung", ({ "torso", 10, 0 }),
      "spleen", "spleen", ({ "torso", 5, 0 }),
      "intestine", "gut", ({ "torso", 15, 0 }),
      "genitals", "genitals", ({ 0, 5, 0 }),
      "left front leg", "leg", ({ 0, 5, 0 }),
      "right front leg", "leg", ({ 0, 5, 0 }),
      "left rear leg", "leg", ({ 0, 5, 0 }),
      "right rear leg", "leg", ({ 0, 5, 0 }),
      "tail", "tail", ({ 0, 5, 0 }),
      "skin", "skin", ({ 0, 175, 0 })
   });

   inedible = ({ "skull", "teeth", "tooth", "tail", "skin" });
   unrottable = ({ "skull", "teeth", "tooth" });
   set_skin( "fur" );

   if( !do_setup )
      this_object()->setup();
} /* create() */
