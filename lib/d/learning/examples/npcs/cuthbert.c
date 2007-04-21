/* Cuthbert requires more than the average NPC. He wants 
*cutting edge fashion.  If items are going to be truly 
*unique, add them this way, otherwise, add them to the armoury
*for all to share.  Don't make weapons unless you know what you are
*doing.
*/

#include <armoury.h>

#define TO this_object()
inherit "/obj/monster";

//We need to give him some items
object cape, trousers;




void setup() {

    set_name( "cuthbert" );
    set_short( "Cuthbert" );
    add_property("determinate", "");
    add_alias( ({"npc", "warrior", "man"}) );
    set_long("Cuthbert is a fashion victim.  Not content with off the peg "
    "stuff, he wants clothing unique to him.  Fair enough, let's give him"
    " some.\n"); 
    basic_setup ("human", "warrior", 50);
    set_gender( 1 );

    add_language( "common" );
    set_language( "common" );
    load_chat( 50, ({
                  1, ":checks his equipment.",
                  1, ":feels all snuggly and warm in his new clothes."                  
               }) );
    load_a_chat( 150, ({
                    1, "'Oof!",
                    1, ":is happy to die so you can learn.",
                    1, "You feel Someone is waiting in the wings."}) );
                    
         ARMOURY->request_item( "short sword",
         70+random(30))->move( TO );
         ARMOURY->request_item( "baggy leather loincloth",
         70+random(30))->move( TO );
         ARMOURY->request_item( "metal helmet",
         70+random(30))->move( TO );
         
         //Clone some clothes, make them trousers, give them all normal
         //properties, add some pockets, and some factors for condition
         //and resistance.
   trousers = clone_object( "/obj/clothing" );
   trousers->set_name( "britches" );
   trousers->add_adjective(({"pair", "of"}));
   trousers->set_short( "pair of britches" );
   trousers->add_alias("trousers");
   trousers->set_long( "These are funky purple flaired britches, made of camel"
   "skin with beaver fur lining.\n" );
   trousers->set_value( 100 );
   trousers->set_weight( 7 );
   //All clothing must have a type. Type
   //exec return m_indices("/obj/handlers/clothing_handler"
   //->query_all_clothing_types());
   //for a list of possibilities.
   trousers->set_type( "trousers" );
   trousers->add_pocket( "side", 100 );
   trousers->add_pocket( "side", 100 );
   trousers->set_damage_chance( 10 );
   trousers->set_max_cond( 800 );
   trousers->set_cond( 600 + random( 200 ) );
   trousers->move( TO );
         
         cape = clone_object( "/obj/clothing" );
   cape->set_name( "cape" );
   cape->set_short( "orange cape" );
   cape->add_adjective("orange");
   cape->set_long( "Orange velvet has been made into a large cape.  It is bright "
   "and garish.\n" );
   cape->set_value( 100 );
   cape->set_weight( 7 );
   cape->set_type( "cape" );
   cape->set_damage_chance( 10 );
   cape->set_max_cond( 800 );
   cape->set_cond( 600 + random( 200 ) );
   cape->move( TO );
      init_equip(); 
      
         give_money( 10, 30, "Ankh-Morpork pence" );

} /* ends setup() */
