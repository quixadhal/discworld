inherit "/std/races/reptile_base";

void setup() {
   set_name( "snake" );
   set_long( "Snake, Serpent, Legless thingy that crawls on its belly all "
      "day long.\n" );
   set_height( 5 );
   set_weight( 60 );
   set_desc( "a serpent, the type with no legs but with scales." );
  
   /* throw away reptilian bits and replace totally */
   bits = ({ 
      "head", "head", ({ 0, 50, 0,
         "left eye", "right eye", "poison sac", "tongue", "skull" }),
      "left eye", "eye", ({ "head", 5, 0 }),
      "right eye", "eye", ({ "head", 5, 0 }),
      "poison sac", "sac", ({ "head", 10, 0 }),
      "skull", "skull", ({ "head", 40, 0, "tooth", "brain" }),
      "tooth", "tooth", ({ "skull", 5, ({"/std/bit", 2}) }),
      "brain", "brain", ({ "skull", 10, 0 }),
      "torso", "torso", ({ 0, 600, 0,
         "heart", "liver", "left kidney", "right kidney", "spleen",
         "bladder", "intestine" }),
      "heart", "heart", ({ "torso", 20, 0 }),
      "liver", "liver", ({ "torso", 20, 0 }),
      "left lung", "lung", ({ "torso", 20, 0 }),
      "right lung", "lung", ({ "torso", 20, 0 }),
      "left kidney", "kidney", ({ "torso", 10, 0 }),
      "right kidney", "kidney", ({ "torso", 10, 0 }),
      "spleen", "spleen", ({ "torso", 10, 0 }),
      "bladder", "bladder", ({ "torso", 10, 0 }),
      "intestine", "gut", ({ "torso", 100, 0 }),
      "genitals", "genitals", ({ 0, 5, 0 }),
      "tail", "tail", ({ 0, 500, 0 }),
      "skin", "skin", ({ 0, 175, 0 })
   });

   inedible = ({ "poison sac", "tooth" });
   unrottable = ({ "tooth" });

   add_ac( "blunt", "blunt", 40 );
   add_ac( "sharp", "sharp", 20 );
   add_ac( "pierce", "pierce", 10 );

   set_stats( ({ 0, 0, -4, 4, -4 }) );
} /* setup() */
     
void set_unarmed_attacks( object thing ) {
   int number;

   number = (int)thing->query_skill_bonus( "fighting.combat.melee.unarmed" );
   number = sqrt( number );

   thing->remove_attack( "hands" );
   thing->remove_attack( "feet" );

   thing->remove_attack( "bite" );
   thing->remove_attack( "crush" );

   thing->add_attack( "bite", 80, ({ ( 3 * number ) / 2, 5, number * 2 }),
      "pierce", "unarmed", ({ "poison", this_object() }) );
   thing->tactics( "response dodge" );
} /* set_unarmed_attacks() */

void poison( int damage, object him, object me, string a_type,
             string a_name ) {
   tell_room( me->one_short() + " visciously sinks its fangs into " +
      him->one_short() + " who seethes in agony!\n" );
   tell_object( him, me->one_short() + " sinks its fangs into you!  You "
      "seethe in agony.\n" );

   him->add_effect( POISON_EFFECT, random(200) );
}

string *query_target_zones() {
  return ({ "head", "body", "body", "body", "tail", "tail", "tail" });
}

string map_target_zone(string zone) {
  switch(zone) {
  case "head":
    return "head";
  case "neck":
  case "chest":
    return "body";
  default:
    return "tail";
  }
}

