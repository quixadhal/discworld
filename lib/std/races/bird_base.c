inherit "/std/races/base";

void create() {
   do_setup++;
   ::create();
   do_setup--;

   set_desc( "a general, unspecified bird.  This should not be used since "
      "it is a base race" );

   bits = ({
      "head", 0, ({ 0, 150, 0,
         "left eye", "right eye", "beak", "skull" }),
      "left eye", "eye", ({ "head", 1, 0 }),
      "right eye", "eye", ({"head", 1, 0 }),
      "beak", 0, ({ "head", 20, 0 }),
      "skull", "cranium", ({ "head", 30, 0, "brain" }),
      "brain", 0, ({ "skull", 10, 0 }),
      "neck", 0, ({ 0, 25, 0, "gizzard" }),
      "gizzard", "gullet", ({ "neck", 5, 0 }),
      "torso", "body", ({ 0, 500, 0,
         "heart", "liver", "left kidney", "right kidney", "left lung",
         "right lung", "spleen", "intestine", "pancreas", "wishbone",
         "body feather" }),
      "heart", 0, ({ "torso", 50, 0 }),
      "liver", 0, ({ "torso", 25, 0 }),
      "left kidney", "kidney", ({ "torso", 15, 0 }),
      "right kidney", "kidney", ({ "torso", 15, 0 }),
      "left lung", "lung", ({ "torso", 25, 0 }),
      "right lung", "lung", ({ "torso", 25, 0 }),
      "spleen", 0, ({ "torso", 15, 0 }),
      "intestine", "gut", ({ "torso", 50, 0 }),
      "pancreas", 0, ({ "torso", 15, 0 }),
      "wishbone", 0, ({ "torso", 5, 0 }),
      "body feather", "feather", ({ "torso", 1, ({"/std/bit", 6}) }),
      "tail", 0, ({ 0, 50, 0, "tail feather" }),
      "tail feather", "feather", ({ "tail", 1, ({"/std/bit", 4}) }),
      "left wing", "wing", ({ 0, 150, 0,
         "left wing feather" }),
      "left wing feather", "feather", ({ "left wing", 1,
         ({"/std/bit", 5}) }),
      "right wing", "wing", ({ 0, 150, 0,
         "right wing feather" }),
      "right wing feather", "feather", ({ "right wing", 1,
         ({"/std/bit", 5}) }),
      "left leg", "leg", ({ 0, 100, 0, "left claw" }),
      "left claw", "claw", ({ "left leg", 25, 0 }),
      "right leg", "leg", ({ 0, 100, 0, "right claw" }),
      "right claw", "claw", ({ "right leg", 25, 0 })
   });

   inedible = ({ "beak", "skull", "body feather", "left wing feather",
      "right wing feather", "tail feather", "left claw", "right claw",
      "wishbone" });
   unrottable = ({ "beak", "skull", "body feather", "left wing feather",
      "right wing feather", "tail feather", "left claw", "right claw",
      "wishbone" });
   pluckable = ({ "body feather", "left wing feather", "right wing feather",
      "tail feather" });
   set_skin( "feathers" );

   if( !do_setup )
      this_object()->setup();
} /* create() */


string *query_target_zones() {
  return ({ "head", "head", "neck",
              "breast", "breast", "breast", "breast", "back",
              "left wing", "left wing", "left wing", "left wing",
              "right wing", "right wing", "right wing", "right wing",
              "stomach", "stomach", "stomach",
              "left leg", "left leg", "left claw", 
              "right leg", "right leg", "right claw" });
}

string map_target_zone(string zone) {
  switch(zone) {
  case "chest":
    return "breast";
  case "abdomen":
    return "stomach";
  case "hands":
  case "arms":
    return (random(2) ? "left " : "right ") + "wing";
  case "feet":
    return (random(2) ? "left " : "right ") + "claw";
  default:
    return zone;
  }
}
