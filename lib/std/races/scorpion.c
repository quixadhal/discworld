/* scorpion race object Thought .. 19th Oct 1996
 * Fixed by Sin to do the poisoning correctly. */

#define POISON_EFFECT "/std/effects/poisons/spider_venom"

inherit "/std/races/arachnid";
 
void poison( int damage, object him, object me, string atype, string aname );

void setup() {
   set_name( "scorpion" );
   set_long( "A nasty little poisonous creature, of the arachnid family.\n" );
   set_height( 5 );
   set_weight( 2 );
   set_desc( "a nasty little poisonous creature, of the arachnid family" );

   bits = ({
      "head", "head", ({ 0, 200, 0,
           "left eye", "right eye", "mandible", "brain" }),
      "left eye", "eye", ({ "head", 10, 0 }),
      "right eye", "eye", ({ "head", 10, 0 }),
      "mandible", "mouth", ({ "head", 50, 0 }),
      "brain", "brain", ({ "head", 25, 0 }),
      "thorax", "thorax", ({ 0, 500, 0, "heart", "poison sac" }),
      "heart", "heart", ({ "thorax", 50, 0 }),
      "poison sac", "sac", ({ "thorax", 50, 0 }),     
      "tail", "tail", ({ 0, 300, 0, "sting" }),
      "sting", "sting", ({ "tail", 25, 0 }),
      "left claw", "claw", ({ 0, 100, 0 }),
      "right claw", "claw", ({ 0, 100, 0 }),
      "left front leg", "leg", ({ 0, 50, 0 }),
      "left middle leg", "leg", ({ 0, 50, 0 }),
      "left back leg", "leg", ({ 0, 50, 0 }),
      "right front leg", "leg", ({ 0, 50, 0 }),
      "right middle leg", "leg", ({ 0, 50, 0 }),
      "right back leg", "leg", ({ 0, 50, 0 })
   });

   inedible = ({ "mandible", "left claw", "right claw" });
   unrottable = ({ "mandible", "left claw", "right claw" });

   set_stats( ({ -6, 4, -6, -6, -6 }) );
} /* setup() */
 
void new_set_level( mixed *args ) {
   int level;
   object thing;

   if( sizeof(args) < 2 || !args[1] )
      return;

   level = args[ 0 ];
   thing = args[ 1 ];

   thing->add_skill_level( "other.health", level * 3 );
   thing->add_ac( "blunt", "blunt", level / 2 );
   thing->add_ac( "pierce", "pierce", level / 4 );
   thing->add_ac( "sharp", "sharp", level / 3 );
   thing->add_ac( "fire", "fire", level );

/* Not quite sure about these levels, I know the ordering is right, but
 * the actual values could probably do with tweaking
 */ 
} /* new_set_level() */

int query_dark( int light ) { return 0; }

void set_unarmed_attacks( object thing ) {
   int number;

   number = (int)thing->query_skill_bonus( "fighting.combat.melee.unarmed" );
   number = 4 + sqrt( number );

   thing->remove_attack( "hands" );
   thing->remove_attack( "feet" );

   thing->remove_attack( "poison" );
   thing->remove_attack( "bite" );

   thing->add_attack( "sting", 70, ({ 5 * number, 10, number }),
      "pierce", "unarmed", ({ "poison", this_object() }) );
   thing->add_attack( "bite",  20, ({ 3 * number, 10, number }),
      "pierce", "unarmed", 0 );

   thing->add_attack_message( "sting", "pierce", ({
      0, "$N sting$s $P $z.\n" }) );

   thing->tactics( "response dodge" );
} /* set_unarmed_attacks() */

void poison( int damage, object him, object me, string a_type,
             string a_name ) {
   tell_object( me, "You stab " + him->one_short() + " with your tail, "
      "injecting " + him->query_objective() + " with venom.\n" );
   tell_object( him, me->one_short() + " stabs you with " +
      me->query_possessive() + " tail!  You seethe in agony.\n" );
   tell_room( environment( me ), me->one_short() + " visciously stabs " +
      him->one_short() + " with " + me->query_possessive() + " tail!  " +
      him->one_short() + " seethes in agony.\n", ({ me, him }) );

   him->add_effect( POISON_EFFECT, 100 );
} /* poison() */



