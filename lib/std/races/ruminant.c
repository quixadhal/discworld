inherit "/std/races/base";

void create() {
   do_setup++;
   ::create();
   do_setup--;

   set_weight( 800 );
   set_height( 50 );
   set_desc( "a four-legged ruminant.  It shouldn't be used since this is "
      "just a base race" );

   bits = ({ 
      "head", "head", ({ 0, 100, 0,
         "left eye", "right eye", "nose", "skull", "tongue", "left ear",
         "right ear" }),
      "left eye", "eye", ({ "head", 2, 0 }),
      "right eye", "eye", ({ "head", 2, 0 }),
      "skull", "skull", ({ "head", 40, 0, "brain", "teeth" }),
      "brain", "brain", ({ "skull", 20, 0 }),
      "left ear", "ear", ({"head", 5, 0 }),
      "right ear", "ear", ({"head", 5, 0 }),
      "nose", "nose", ({ "head", 15, 0 }),
      "teeth", "teeth", ({ "skull", 5, ({"/std/bit_set", 1}), "tooth" }),
      "tooth", "tooth", ({ "teeth", 1, ({"/std/bit", 18}) }),
      "tongue", "tongue", ({ "head", 10, 0 }),
      "neck", "neck", ({ 0, 100, 0 }),
      "torso", "chest", ({ 0, 300, 0,
         "heart", "liver", "left kidney", "right kidney", "left lung",
         "right lung", "spleen", "first stomach", "second stomach",
         "third stomach", "fourth stomach", "intestine", "pancreas" }),
      "heart", "heart", ({ "torso", 15, 0 }),
      "liver", "liver", ({ "torso", 15, 0 }),
      "left kidney", "kidney", ({ "torso", 10, 0 }),
      "right kidney", "kidney", ({ "torso", 10, 0 }),
      "left lung", "lung", ({ "torso", 15, 0 }),
      "right lung", "lung", ({ "torso", 15, 0 }),
      "spleen", "spleen", ({ "torso", 10, 0 }),
      "intestine", "gut", ({ "torso", 50, 0 }),
      "pancreas", "pancreas", ({ "torso", 10, 0 }),
      "first stomach", "stomach", ({ "torso", 20, 0 }),
      "second stomach", "stomach", ({ "torso", 20, 0 }),
      "third stomach", "stomach", ({ "torso", 20, 0 }),
      "fourth stomach", "stomach", ({ "torso", 20, 0 }),
      "genitals", "genitals", ({ 0, 5, 0 }),
      "left front leg", "leg", ({ 0, 150, 0, "left front hoof" }),
      "left front hoof", "hoof", ({ "left front leg", 30, 0 }),
      "right front leg", "leg", ({ 0, 150, 0, "right front hoof" }),
      "right front hoof", "hoof", ({ "right front leg", 30, 0 }),
      "left rear leg", "leg", ({ 0, 150, 0, "left rear hoof" }),
      "left rear hoof", "hoof", ({ "left rear leg", 30, 0 }),
      "right rear leg", "leg", ({ 0, 150, 0, "right rear hoof" }),
      "right rear hoof", "hoof", ({ "right rear leg", 30, 0 }),
      "skin", "skin", ({ 0, 175, 0 })
   });

   inedible = ({ "skull", "neck", "genitals", "teeth", "left front hoof",
      "right front hoof", "left rear hoof", "right rear hoof", "tooth",
      "skin" });
   unrottable = ({ "skull", "teeth", "tooth" });
   set_skin( "hide" );

   set_stats( ({ 0, 0, -4, 2, -6 }) );

   if( !do_setup )
      this_object()->setup();
} /* create() */

string *query_target_zones() {
  return ({ "head", "head", "neck",
              "chest", "back", "back", "back", "lower back", 
              "left front leg", "left front leg", "left front hoof",
              "stomach", "stomach", "tail",
              "right front leg", "right front leg", "right front hoof",
              "left rear leg", "left rear leg", "left rear hoof",
              "right rear leg", "right rear leg", "right rear hoof"});
}

string map_target_zone(string zone) {
  switch(zone) {
  case "chest":
    return random(4) ? "back" : "chest";
  case "abdomen":
    return random(4) ? "stomach" : "lower back";
  case "arms":
    return (random(2) ? "left " : "right ") + "front leg";
  case "hands":
    return (random(2) ? "left " : "right ") + "front hoof";
  case "legs":
    return (random(2) ? "left " : "right ") + "rear leg";
  case "feet":
    return (random(2) ? "left " :"right ") + "rear hoof";
  default:
    return zone;
  }
}
