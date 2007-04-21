inherit "/std/races/base";

void create() {
   do_setup++;
   ::create();
   do_setup--;

   set_desc( "your standard insect.  This should not be used since it is a "
      "base race" );

   bits = ({ 
      "head", "head", ({ 0, 200, 0,
         "left feeler", "right feeler", "brain", "left eye", "right eye",
         "mandible" }),
      "left feeler", "feeler", ({ "head", 10, 0 }),
      "right feeler", "feeler", ({ "head", 10, 0 }),
      "brain", "brain", ({ "head", 50, 0 }),
      "left eye", "eye", ({ "head", 10, 0 }),
      "right eye", "eye", ({ "head", 10, 0 }),
      "mandible", "mouth", ({ "head", 25, 0 }),
      "thorax", "thorax", ({ 0, 500, 0,
         "left front leg", "right front leg", "left middle leg",
         "right middle leg", "left back leg", "right back leg" }),
      "left front leg", "leg", ({ "thorax", 50, 0 }),
      "right front leg", "leg", ({ "thorax", 50, 0 }),
      "left middle leg", "leg", ({ "thorax", 50, 0 }),
      "right middle leg", "leg", ({ "thorax", 50, 0 }),
      "left back leg", "leg", ({ "thorax", 50, 0 }),
      "right back leg", "leg", ({ "thorax", 50, 0 }),
      "abdomen", "abdomen", ({ 0, 500, 0 })
   });

   inedible = ({ "mandible" });
   unrottable = ({ "mandible" });
   set_skin( "carapace" );

   if( !do_setup )
      this_object()->setup();
} /* create() */

string *query_target_zones() {
  return ({ "head", "head",
              "thorax","thorax", "thorax", "thorax", "thorax", "thorax",
              "left front leg", "left front leg",
              "left middle leg", "left middle leg",
              "right front leg", "right front leg",
              "right middle leg", "right middle leg",
              "abdomen", "abdomen", "abdomen", "abdomen", "abdomen",
              "left back leg", "left back leg", 
              "right back leg", "right back leg" });
}

string map_target_zone(string zone) {
  switch(zone) {
  case "neck":
  case "chest":
    return "thorax";
  case "arms":
  case "hands":
    return (random(2) ? "left " : "right ") +
      (random(2) ? "front " : "middle ") + "leg";
  case "feet":
  case "legs":
    return (random(2) ? "left " : "right ") + "rear leg";
  default:
    return zone;
  }
}
