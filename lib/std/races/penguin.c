inherit "/std/races/standard";

string *inedible;

void setup() {
   set_long("Penguins are always rather formal.\n");
   set_name("penguin");
   /* throw away human bits and replace totally */
   bits = ({ 
"head", 0, ({ 0, 2, 0, "left eye", "right eye", "beak", "skull" }),
"left eye", "eye", ({ "head", 1, 0 }),
"right eye", "eye", ({ "head", 1, 0 }),
"beak", "pecker", ({ "head", 1, 0 }),
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
"tail", "", ({ 0, 1, 0 }),
"left wing", "wing", ({ 0, 2, 0 }),
"right wing", "wing", ({ 0, 2, 0 }),
"left leg", "leg", ({ 0, 3, 0, "left claw" }),
"right leg", "leg", ({ 0, 3, 0, "right claw" }),
"left claw", "claw", ({ "left leg", 1, 0 }),
"right claw", "claw", ({ "right leg", 1, 0 })
});
  inedible = ({ "beak", "skull", "left claw", "right claw" });
}

string query_desc(object ob) {
   return "Penguins are always rather formally dressed.\n";
}

int query_eat(string type) {
  return (member_array(type, inedible) == -1);
}
