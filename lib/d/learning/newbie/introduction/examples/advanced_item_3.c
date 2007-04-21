/*
    Ruler Of Death +2
    Wrytten by Drakkos Thee Creator
        
    21/10/2000
*/
    
inherit "/obj/weapon";
    
void setup() {
    set_name("ruler");
    set_short("wooden ruler");
    add_adjective("wooden");
    set_long("This is a thick, wooden ruler, with the inches "
        "inked out in black.\n");
    set_weight(1);
    set_value(0);
    new_weapon(5000);
    add_attack("rap", 50,({ 25, 10, 5 }), "blunt", "blunt", "do_shame");
    add_attack_message( "rap", "blunt", 
   ({
    0,({ "You rap $hcname$ smartly across the knuckles with your "
        "ruler.\n",
        "$mcname$ raps you smartly across the knuckles with $mposs$ "
            "ruler.\n",
        "$mcname$ raps $hcname$ smartly across the knuckles with "
            "$mposs$ ruler.\n"})
    }));
    set_wield_func( "test_creator", base_name( this_object() ) );     
}

int test_creator(object ob)  {

    // In case the function is called when there's no object to
    // be passed in.
    if(!ob) {
        return 1;
    }    
        
    // In case the object calling the function is a corpse attempting to
    // rehold the weapon after death.
    if(ob->query_corpse()) {
        return 1;           
    }
        
    // And now the test creator bit itself.
    if(ob->query_creator()) {
        tell_object(ob, "You have been deemed worthy, leetle creator!\n");
        return 1;
    }
    else {
        tell_object(ob, "You have been deemed unworthy, leetle player!\n");        
        return 0;
    }
}

void do_shame( int damage, object attack_ob, object attack_by, string
    type, string name ) {
        
    tell_object(attack_ob, "Your knuckles %^BOLD%^sting!%^RESET%^\n");
    tell_object(attack_by, "You feel strangely satisfied as " + 
        attack_ob->one_short() + " whimpers in pain!\n");
}