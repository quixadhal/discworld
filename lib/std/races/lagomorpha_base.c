/* This is the base race inherit for lagomorpha, that is rabbits,
 * hares and pikas.
 * Danbala, 2003
 */
inherit "/std/races/base";

void create() {
   do_setup++;
   ::create();
   do_setup--;

   set_desc( "a generic lagomorph animal.  It shouldn't be used since "
      "this is just a base race" );

   bits = ({ 
      "head", "head", ({ 0, 40, 0,
         "left eye", "right eye", "left ear", "right ear", "skull" }),
      "left eye", "eye", ({ "head", 1, 0 }),
      "right eye", "eye", ({ "head", 1, 0 }),
      "left ear", "ear", ({ "head", 20, 0 }),
      "right ear", "ear", ({ "head", 20, 0 }),
      "skull", "skull", ({ "head", 20, 0, "brain", "teeth" }),
      "brain", "brain", ({ "skull", 10, 0 }),
      "teeth", "teeth", ({ "skull", 5, ({"/std/bit_set", 1}), "tooth" }),
      "tooth", "tooth", ({ "teeth", 1, ({"/std/bit", 20}) }),
      "torso", "torso", ({ 0, 100, 0,
         "heart", "liver", "left kidney", "right kidney", "spleen",
         "right lung", "left lung", "intestine" }),
      "heart", "heart", ({ "torso", 15, 0 }),
      "liver", "liver", ({ "torso", 15, 0 }),
      "left kidney", "kidney", ({ "torso", 10, 0 }),
      "right kidney", "kidney", ({ "torso", 10, 0 }),
      "spleen", "spleen", ({ "torso", 10, 0 }),
      "right lung", "lung", ({ "torso", 15, 0 }),
      "intestine", "gut", ({ "torso", 100, 0 }),
      "genitalia", "genitalia", ({ 0, 5, 0 }),
      "left front leg", "leg", ({ 0, 100, 0, "left front paw" }),
      "left front paw", "paw", ({ "left front leg", 30, 0 }),
      "right front leg", "leg", ({ 0, 100, 0, "right front paw" }),
      "right front paw", "paw", ({ "right front leg", 30, 0 }),
      "left rear leg", "leg", ({ 0, 300, 0, "left rear paw" }),
      "left rear paw", "paw", ({ "left rear leg", 50, 0 }),
      "right rear leg", "leg", ({ 0, 300, 0, "right rear paw" }),
      "right rear paw", "paw", ({ "right rear leg", 50, 0 }),
      "tail", "tail", ({ 0, 25, 0 }),
      "skin", "skin", ({ 0, 175, 0 })
   });

   inedible = ({ "skull", "teeth", "tooth", "tail", "skin" });
   unrottable = ({ "skull", "teeth", "tooth" });
   set_skin( "fur" );

   if( !do_setup )
      this_object()->setup();
} /* create() */

string *query_target_zones() {
  return ({ "head", "head", "neck",
              "chest", "back", "back", "back", "lower back", 
              "left front leg", "left front leg", "left front paw",
              "stomach", "stomach", "tail",
              "right front leg", "right front leg", "right front paw",
              "left rear leg", "left rear leg", "left rear paw",
              "right rear leg", "right rear leg", "right rear paw"});
} /* query_target_zones() */

string map_target_zone(string zone) {
  switch(zone) {
  case "chest":
    return random(4) ? "back" : "chest";
  case "abdomen":
    return random(4) ? "stomach" : "lower back";
  case "arms":
    return (random(2) ? "left " : "right ") + "front leg";
  case "hands":
    return (random(2) ? "left " : "right ") + "front paw";
  case "legs":
    return (random(2) ? "left " : "right ") + "rear leg";
  case "feet":
    return (random(2) ? "left " :"right ") + "rear paw";
  default:
    return zone;
  }
} /* map_target_zone() */
