/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: glass.c,v 1.2 2002/08/28 16:46:00 carmine Exp $
 *
 *
 */

/** 
 * A nice little inheritable and constructible glass object thing.
 * 
 * @author Taffyd
 */

inherit "/obj/vessel";

nosave string description, actual_drink, actual_glass_size;

/** @ignore yes */
void create() {
    do_setup++;
    ::create();
    do_setup--;

    set_name("glass");
    set_weight(1);
    set_value(400);
    set_stuck(1);
    set_leak_rate(0);
    set_material("glass");
    add_property("fragile", 200);   

    if (!do_setup) {
        this_object()->setup();
    } 
} /* setup() */

/** 
 * This function is used to transform the glass into a new type of glass, changing the 
 * short descriptions and adding aliases and adjectives.
 *
 * If the 'contents' parameter is 1, then the glass is automatically filled
 * up with a drink made from /obj/reagents/generic_liquid.ob
 *
 * @param drink The name of the drink to be created
 * @param glass_description The description given when there is something in the glass.
 * @param glass_size The size of the glass, (in words), for example "large glass",
 * @param amount The amount of liquid the glass should hold.
 * @param has_contents Fill the glass up with 'drink'.
 * @see /obj/bottle
 * @example
 * // Create a large glass of tomato juice
 *
 * glass = clone_object("/obj/glass");
 * glass->make_glass("tomato juice", "Red, thick, delicious.\n",
 *      "large glass", 4000, 1);
 * glass->move(this_object());
 *
 * @example
 * inherit "/obj/glass";
 *
 * void setup() {
 * // Do your own configuration of the glass, then call make_glass
 *
 *     ...
 *     make_glass("chocolate milkshake", "Yumm...", "shot glass", 100, 0);
 *     ...
 * // Place your own contents in, do other funky stuff.
 * }
 */ 
void make_glass(string drink, string glass_description, string glass_size, 
   int amount, int has_contents) 
{ 
    string *glass_adjectives;
    object contents;
    
    description = glass_description;
    actual_drink = drink;
    actual_glass_size = glass_size;
    
    glass_adjectives = explode( glass_size, " " )[0..<2];

    set_short( glass_size );          
    set_long( (: this_object()->query_volume() ? description : "This is "
        "a dirty " + actual_glass_size + ", one that used to hold some "
        " " + actual_drink + ".\n" :) );

    glass_adjectives = explode( glass_size, " " );

    if (sizeof(glass_adjectives) > 1)
        add_alias(glass_adjectives[<1..]);
    else
        add_alias(glass_size);

    add_adjective(glass_adjectives);
    set_max_volume(amount);
        
    if (has_contents) {
        contents = clone_object("/obj/reagents/generic_liquid.ob");
        contents->set_name("drink");
        contents->set_short(drink);
        contents->set_long("This is some delicious looking " + drink + ".\n" );
        contents->add_adjectives(explode(drink, " ")[0..<2]);
        contents->add_alias( explode( drink, " " )[ sizeof( 
           explode( drink, " " ) ) - 1 ] );
        contents->set_amount(amount);
        contents->move(this_object());
    }        
} /* setup_drink() */    

/** @ignore yes */
mapping int_query_static_auto_load() {
    return ([
        "::": ::int_query_static_auto_load(),
        "description": description,
        "actual_drink" : actual_drink,
        "actual_glass_size" : actual_glass_size,
    ]);
} /* int_query_static_auto_load() */

void init_static_arg(mapping map) {
    if (map["::"])
        ::init_static_arg(map["::"]);
    if (!undefinedp(map["description"]))
        description = map["description"];
    if (!undefinedp(map["actual_drink"]))
        actual_drink = map["actual_drink"];        
    if (!undefinedp(map["actual_glass_size"]))
        actual_glass_size = map["actual_glass_size"];

    set_long((: this_object()->query_volume() ? description : "This is "
        "a dirty " + actual_glass_size + ", one that used to hold some "
        "sort of " + actual_drink + ".\n" :));

} /* init_static_arg() */

/** @ignore yes */
mixed query_static_auto_load() {
    if (base_name(this_object()) == "/obj/misc/glass" )
        return int_query_static_auto_load();
    return ([ ]);
} /* query_static_auto_load() */
