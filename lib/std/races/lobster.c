inherit "/std/races/base";

void setup() {
    set_name( "lobster" );
    set_long( "A cranky lobster.\n" );
    set_height( 10 );
    set_weight( 18 );
    set_desc( "a cranky lobster" );

    bits = ({ 
        "head", "head", ({ 0, 200, 0,
            "left antenna", "right antenna", "left antennule", "right "
            "antennule", "brain", "left eye", "right eye", "left eyestalk",
            "right eyestalk", "jaw", "skull" }),
        "left antenna", "antenna", ({ "head", 2, 0 }),
        "right antenna", "antenna", ({ "head", 2, 0 }),
        "left antennule", "antennule", ({ "head", 1, 0 }),
        "right antennule", "antennule", ({ "head", 1, 0 }),
        "brain", "brain", ({ "head", 50, 0 }),
        "skull", "skull", ({ "head", 50, 0 }),
        "left eye", "eye", ({ "head", 5, 0 }),
        "right eye", "eye", ({ "head", 5, 0 }),
        "left eyestalk", "eyestalk", ({ "head", 2, 0 }),
        "right eyestalk", "eyestalk", ({ "head", 2, 0 }),
        "jaw", "mouth", ({ "head", 25, 0 }),
        "carapace", "carapace", ({ 0, 25, 0 }),
        "thorax", "thorax", ({ 0, 500, 0,
            "left cheliped", "right cheliped", "left second leg",
            "right second leg", "left third leg", "right third leg", "left "
            "fourth leg", "right fourth leg", "left back leg", "right back "
            "leg", "heart", "hepatopancreas", "intestine", "stomach" }),
        "left cheliped", "cheliped", ({ "thorax", 75, 0 }),
        "right cheliped", "cheliped", ({ "thorax", 75, 0 }),
        "left second leg", "leg", ({ "thorax", 50, 0 }),
        "right second leg", "leg", ({ "thorax", 50, 0 }),
        "left third leg", "leg", ({ "thorax", 50, 0 }),
        "right third leg", "leg", ({ "thorax", 50, 0 }),
        "left fourth leg", "leg", ({ "thorax", 50, 0 }),
        "right fourth leg", "leg", ({ "thorax", 50, 0 }),
        "left back leg", "leg", ({ "thorax", 50, 0 }),
        "right back leg", "leg", ({ "thorax", 50, 0 }),
        "heart", "heart", ({ "thorax", 50, 0 }),
        "hepatopancreas", "hepatopancreas", ({ "thorax", 10, 0 }),
        "intestine", "intestine", ({ "thorax", 30, 0 }),
        "stomach", "stomach", ({ "thorax", 30, 0 }),
        "apron", "apron", ({ 0, 100, 0, "left pleopod", "right pleopod" }),
        "left pleopod", "pleopod", ({ "apron", 20, 0 }),
        "right pleopod", "pleopod", ({ "apron", 20, 0 })
    });

    inedible = ({ "jaw", "carapace", "skull" });
    unrottable = ({ "jaw", "carapace", "skull" });
    set_skin( "shell" );

    set_stats( ({ -6, 4, -6, -4, -6 }) );
} /* setup() */
 
int query_eat( string type ) {
   return ( member_array( type, inedible ) == -1 );
} /* query_eat() */


void new_set_level( mixed *args ) {
   int level;
   object thing;

   if( sizeof(args) < 2 || !args[1] )
      return;

   level = args[ 0 ];
   thing = args[ 1 ];

   thing->add_skill_level( "other.health", level * 2 );
   thing->add_ac( "blunt", "blunt", level / 2 );
   thing->add_ac( "pierce", "pierce", level / 4 );
   thing->add_ac( "sharp", "sharp", level / 3 );
} /* new_set_level() */


void set_unarmed_attacks( object thing ) {
    int number;

    number = (int)thing->query_skill_bonus( "fighting.combat.melee.unarmed" );
    number = 4 + sqrt( number );

    thing->remove_attack( "hands" );
    thing->remove_attack( "feet" );

    thing->remove_attack( "pincers" );

    thing->add_attack( "pincers", 100, ({ number, 10, number }),
        "unarmed", "unarmed", 0 );

    thing->add_attack_message( "pincers", "unarmed", ({
       20, "$N bat$s $I with $p pincer.\n",
       60, "$N nip$s $P $z with $p pincer.\n",
       100, "$N scratch$es $I deeply with $p pincer.\n",
       140, "$N cut$s $P $z deeply with $p pincer.\n",
       180, "$N dig$s $p pincer into $P $z.\n",
       220, "$N gouge$s $I with $p pincer.\n",
       0, "$N slice$s $P $z up with $p pincer.\n" }) );

   thing->tactics( "response dodge" );
   
} /* set_unarmed_attacks() */

int lives_in_water() { return 1; }


