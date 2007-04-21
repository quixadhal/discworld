/**
 *We're going to make a rat, with a different description 
 *each time.  Also, their sex and level will be random.
 *The long() will reflect this.
 */

inherit "/obj/monster";

void setup() {
        
        //A list of possible adjectives to use and select from.
    string *name_adjectives = ({ "furry", "manky", 
      "flea-ridden", "obnoxious", "sharp-toothed", "stinky", "dirty, stinking",
      "brown", "beady-eyed", "corpse-eating" });
    string adjective;

    adjective = name_adjectives[random(sizeof(name_adjectives))];


        //Normal basic information, short() uses a random adjective.
    set_name("rat");
    add_alias("rodent");
        //Gender = 1 + (zero or one)
    set_gender(1 + random(2));    
    set_short(adjective + " rat");
    add_adjective(adjective);
        //Note the query_possessive() in the long() to get his, or her right
    set_long("The learning domain is full of rats.  This one appears "
    "not to have eaten for some hours and has a look"
    " of hunger in " + query_possessive()
      + " eyes.\n");

        //Level 2 plus a random figure upto and including 4 (including 0)
    basic_setup ("rat", "fighter", 2 + random(5));
    
  
} /* setup() */


