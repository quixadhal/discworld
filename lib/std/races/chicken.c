inherit "/std/races/standard";

string *inedible;

void setup() {
   set_long("Chickens.  That's all there is to say.  Chickens, chickens, "+
      "chickens, chickens, chickens.  Thank you.\n");
   set_name("chicken");
   /* throw away human bits and replace totally */
   bits = ({ 
"head", 0, ({ 0, 2, 0, "left eye", "right eye", "beak", "comb", "skull" }),
"left eye", "eye", ({ "head", 1, 0 }),
"right eye", "eye", ({ "head", 1, 0 }),
"beak", "pecker", ({ "head", 1, 0 }),
"comb", 0, ({ "head", 1, 0 }),
"skull", 0, ({ "head", 1, 0 }),
"neck", 0, ({ 0, 2, 0 }),
"torso", "chest", ({ 0, 400, 0, "heart", "liver", "left kidney",
    "right kidney", "left lung", "right lung", "spleen" }),
"heart", "", ({ "torso", 10, 0 }),
"liver", "", ({ "torso", 5, 0 }),
"left kidney", "kidney", ({ "torso", 5, 0 }),
"right kidney", "kidney", ({ "torso", 5, 0 }),
"left lung", "lung", ({ "torso", 5, 0 }),
"right lung", "lung", ({ "torso", 5, 0 }),
"spleen", "", ({ "torso", 5, 0 }),
"genitals", "", ({ 0, 5, 0 }),  /* sex ????  ohhh yes*/
"left wing", "wing", ({ 0, 5, 0, "feather" }),
"right wing", "wing", ({ 0, 5, 0, "feather" }),
"feather", 0, ({ "left wing", 1, 0 }),
"tail feather", "feather", ({ 0, 1, 0 }),
"left leg", "leg", ({ 0, 3, 0, "left claw" }),
"right leg", "leg", ({ 0, 3, 0, "right claw" }),
"left claw", "claw", ({ "left leg", 1, 0 }),
"right claw", "claw", ({ "right leg", 1, 0 })
});
  inedible = ({ "beak", "skull", "feather", "tail feather",
                "comb", "neck", "genitals", "left claw", "right claw" });
}

string query_desc(object ob) {
   return "An ordinary brain-dead chicken\n";
}

int query_eat(string type) {
  return (member_array(type, inedible) == -1);
}

void new_set_level(mixed *bing) {
   bing[1]->add_attack("beak", 0, 50, 20, 0, 0, "pierce-beak");
   bing[1]->add_attack("claw", 0, 30, 30, 0, 0, "sharp-claws");
   bing[1]->add_ac("feather_blow", "blunt", 10);
   bing[1]->add_ac("feather_slash", "sharp", 2);
   bing[1]->add_ac("feather_pierce", "pierce", 2);
}
