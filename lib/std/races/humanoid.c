inherit "/std/races/base";

void create() {
   do_setup++;
   ::create();
   do_setup--;

   set_desc( "a humanoid-type being.  This should not be used since it is a "
      "base race" );

   bits = ({
      "head", "head", ({ 0, 40, 0,
         "left ear", "right ear", "left eye", "right eye", "nose", "scalp",
         "tongue", "skull" }),
      "left ear", "ear", ({ "head", 1, 0 }),
      "right ear", "ear", ({ "head", 1, 0 }),
      "left eye", "eye", ({ "head", 1, 0 }),
      "right eye", "eye", ({ "head", 1, 0 }),
      "nose", "nose", ({ "head", 1, 0 }),
      "scalp", "scalp", ({ "head", 3, 0 }),
      "tongue", "tongue", ({ "head", 2, 0 }),
      "skull", "skull", ({ "head", 20, 0, "teeth", "brain" }),
      "brain", "brain", ({ "skull", 40, 0 }),
      "teeth", "teeth", ({ "skull", 2, ({"/std/bit_set", 1}), "tooth" }),
      "tooth", "tooth", ({ "teeth", 1, ({"/std/bit", 32}) }),
      "left arm", "arm", ({ 0, 80, 0, "left hand" }),
      "right arm", "arm", ({ 0, 80, 0, "right hand" }),
      "torso", "torso", ({ 0, 400, 0,
         "heart", "liver", "left kidney", "right kidney", "left lung",
         "right lung", "spleen", "rib" }),
      "left hand", "hand", ({ "left arm", 15, 0,
         "left little finger", "left third finger", "left index finger",
         "left ring finger", "left thumb" }),
      "right hand", "hand", ({ "right arm", 15, 0,
         "right little finger", "right third finger", "right index finger",
         "right ring finger", "right thumb" }),
      "left little finger", "finger", ({ "left hand", 1, 0 }),
      "left third finger", "finger", ({ "left hand", 1, 0 }),
      "left index finger", "finger", ({ "left hand", 1, 0 }),
      "left ring finger", "finger", ({ "left hand", 1, 0 }),
      "left thumb", "thumb", ({ "left hand", 1, 0 }),
      "right little finger", "finger", ({ "right hand", 1, 0 }),
      "right third finger", "finger", ({ "right hand", 1, 0 }),
      "right index finger", "finger", ({ "right hand", 1, 0 }),
      "right ring finger", "finger", ({ "right hand", 1, 0 }),
      "right thumb", "thumb", ({ "right hand", 1, 0 }),
      "heart", "heart", ({ "torso", 10, 0 }),
      "liver", "liver", ({ "torso", 5, 0 }),
      "left kidney", "kidney", ({ "torso", 5, 0 }),
      "right kidney", "kidney", ({ "torso", 5, 0 }),
      "left lung", "lung", ({ "torso", 5, 0 }),
      "right lung", "lung", ({ "torso", 5, 0 }),
      "spleen", "spleen", ({ "torso", 5, 0 }),
      "rib", "rib", ({ "torso", 1, ({"/std/bit", 16}) }),
      "genitals", "genitals", ({ 0 , 5, ({"/std/bit_set", 1}), "genitals" }),
      "left leg", "leg", ({ 0, 300, 0, "left foot", "left femur" }),
      "right leg", "leg", ({ 0, 300, 0, "right foot", "right femur" }),
      "left femur", "femur", ({ "left leg", 1, 0 }),
      "right femur", "femur", ({ "right leg", 1, 0 }),
      "left foot", "foot", ({ "left leg", 50, 0,
         "left big toe", "left second toe", "left third toe",
         "left fourth toe", "left little toe" }),
      "right foot", "foot", ({ "right leg", 50, 0,
         "right big toe", "right second toe", "right third toe",
         "right fourth toe", "right little toe" }),
      "right little toe", "toe", ({ "right foot", 1, 0 }),
      "right second toe", "toe", ({ "right foot", 1, 0 }),
      "right third toe", "toe", ({ "right foot", 1, 0 }),
      "right fourth toe", "toe", ({ "right foot", 1, 0 }),
      "right big toe", "toe", ({ "right foot", 1, 0 }),
      "left little toe", "toe", ({ "left foot", 1, 0 }),
      "left second toe", "toe", ({ "left foot", 1, 0 }),
      "left third toe", "toe", ({ "left foot", 1, 0 }),
      "left fourth toe", "toe", ({ "left foot", 1, 0 }),
      "left big toe", "toe", ({ "left foot", 1, 0 }),
      "skin", "skin", ({ 0, 175, 0 })
   });

   inedible = ({ "skull", "teeth", "tooth", "rib", "left femur", "right femur" });
   unrottable = ({ "skull", "teeth", "tooth", "rib", "left femur", "right femur" });
   set_skin( "skin" );

   if( !do_setup )
      this_object()->setup();
} /* create() */

/**
 * So that all races that inherit this one are marked as humanoids.
 * @return 1 if the race is a humanoid, 0 if not
 */
int query_humanoid() {
   return 1;
} /* query_humanoid() */

string *query_target_zones() {
  return ({ "head", "head", "neck",
              "chest", "chest", "chest", "chest", "back", 
              "left arm", "left arm", "left hand",
              "stomach", "stomach", "stomach",
              "right arm", "right arm", "right hand",
              "left leg", "left leg", "left foot",
              "right leg", "right leg", "right foot"});
}

string map_target_zone(string zone) {
  switch(zone) {
  case "abdomen":
    return "stomach";
  case "arms":
    return (random(2) ? "left " : "right ") + "arm";
  case "hands":
    return (random(2) ? "left " : "right ") + "hand";
  case "legs":
    return (random(2) ? "left " : "right ") + "leg";
  case "feet":
    return (random(2) ? "left " :"right ") + "foot";
  default:
    return zone;
  }
}
