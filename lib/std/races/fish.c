inherit "/std/races/standard";

void setup() {
   set_long("Fish,  yes.  A fish.  Nice generic standard fish thing.\n");
   set_name("fish");
   /* throw away human bits and replace totally */
   bits = ({ 
"head", 0, ({ 0, 2, 0, "left eye", "right eye", "left gill", "right gill" }),
"left eye", "eye", ({ "head", 1, 0 }),
"right eye", "eye", ({ "head", 1, 0 }),
"left gill", "gill", ({ "head", 1, 0 }),
"right gill", "gill", ({ "head", 1, 0 }),
"torso", "guts", ({ 0, 400, 0, "heart", "liver", "left kidney",
    "right kidney", "spleen" }),
"heart", "", ({ "guts", 10, 0 }),
"liver", "", ({ "guts", 5, 0 }),
"left kidney", "kidney", ({ "guts", 5, 0 }),
"right kidney", "kidney", ({ "guts", 5, 0 }),
"spleen", "", ({ "guts", 5, 0 }),
"genitals", "", ({ 0, 5, 0 }),  /* sex ????  ohhh yes*/
"left petral fin", "fin", ({ 0, 5, 0, }),
"right petral fin", "fin", ({ 0, 5, 0, }),
"dorsal petral fin", "fin", ({ 0, 5, 0 }),
"tail", 0, ({ 0, 5, 0 }),
});
}

string query_desc(object ob) {
   return "A fish, the type with fins.\n";
}

/*
 * Need to create a shadow to handle multipleing the armour class by
 * an obscene value when underwater...
 */
void start_player(object ob2) {
  object ob;

  ob = clone_object("/std/races/shadows/fish_shadow");
  ob->init_shadow(ob2);
}
