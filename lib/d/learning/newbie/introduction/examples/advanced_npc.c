/*
    The Laughing Gnome!
    With teefs and spells!
        
    Wrytten by Drakkos Thee Creator
    18/10/2000
*/

#include <armoury.h>

inherit "/obj/monster";

void do_bunny(object, object);
void do_maintain(object, object);
    
void setup() {
    set_name("gnome");
    set_short("laughing gnome");
    set_long("This is a little, friendly looking gnome.  Well... "
        "friendly looking aside from the vicious sharp teefs and the "
        "wicked razor-like claws.  He has laughter lines all over "
        "his face, tho', so he can't be all bad.\n");
    add_property("unique", 1);
    basic_setup("gnome", "wizard", 50);
    set_gender("male");
        
    set_int(23);
    set_str(18);
    set_wis(18);
        
    add_skill_level("magic", 100);
    add_skill_level("fighting", 50);

    load_chat(20,({ 
        1, "' Ha ha ha!.",
        1, "' He he he!.",
        2 , "' I'm the laughing gnome, and you can't catch me!",
        1 , "#charm_women",
    }) );

    load_a_chat(20,({ 
        2, ": bares his teeth.",
        1, "' I'll get my brother Fred onto you!.",
        2 , "' I'll call out the Gnome Guard!",
    }) );
    
    add_spell("bunnies", "/obj/spells/fire_bunny", "cast_spell");
    add_spell("flowers", "/obj/spells/flowers", "cast_spell");
    add_spell("shield", "/obj/spells/small_shield", "cast_spell");    

    add_combat_action(25, "bunny_them",(: do_bunny :));
    add_combat_action(25, "maintain_me",(: do_maintain :));
    
    add_attack( "claws", 88,({ 10, 8, 20 }), "sharp", "sharp", 0 );
    add_attack( "teeth", 88,({ 5, 5, 25 }), "pierce", "pierce", 0 );
    
    add_attack_message( "claws", "sharp", 
   ({
    100, 
       ({ "You slice $hcname$ with your claws.\n",
            "$mcname$ slices you with $mposs$ claws.\n",
            "$mcname$ slices $hcname$ with $mposs$ claws.\n"}),
    200,
       ({ "You rip $hcname$ apart with your claws.\n",
            "$mcname$ rips you apart with $mposs$ claws.\n",
            "$mcname$ rips $hcname$ apart with $mposs$ claws.\n"}),
    0, // The default message set.
       ({ "You scratch $hcname$ viciously with your claws.\n",
            "$mcname$ scratches you viciously with $mposs$ claws.\n",
            "$mcname$ scratches $hcname$ viciously with $mposs$ claws.\n"})
    }));      
    
    add_attack_message( "teeth", "pierce", 
   ({
    100, 
       ({ "You nibble $hcname$ with your teeth.\n",
            "$mcname$ nibbles you with $mposs$ teeth.\n",
            "$mcname$ nibbles $hcname$ with $mposs$ teeth.\n"}),
    200,
       ({ "You chew on $hcname$ with your teeth.\n",
            "$mcname$ chews on you with $mposs$ teeth.\n",
            "$mcname$ chews on $hcname$ with $mposs$ teeth.\n"}),
    0, // The default message set.
       ({ "You sink your teeth into $hcname$.\n",
            "$mcname$ sinks $mposs$ teeth into you.\n",
            "$mcname$ sinks $mposs$ teeth into $hcname$.\n"})
    }));       
        
}    


void charm_women() {        
    object player;
        
    foreach(player in all_inventory(environment(this_object()))) {
        if(interactive(player) && player->query_gender() == 2) {
            queue_command("cast flowers");
            queue_command("give flowers to " + player->query_name());
            queue_command("bow with a flourish");
            return;
        }
    }
}

void do_bunny(object attacker, object target) {
        
    object carrot,torch;
        
    if(target != this_object() ) {
        
        if(!sizeof(match_objects_for_existence("carrot", this_object()))) {
                
            carrot =ARMOURY->request_item ("carrot");
            carrot->move(this_object());
        }
            
        if(!sizeof(match_objects_for_existence("torch", this_object()))) {
            torch = ARMOURY->request_item ("torch");
            torch->move(this_object());
        }
            
        do_command("cast bunnies on " + target->query_name());
    }
}

void do_maintain(object attacker, object target) {
    
    object temp;
    
    if(!sizeof(match_objects_for_existence("shields", this_object()))) {
        temp = ARMOURY->request_item("wooden djelian shield", 100);    
        temp->move(this_object());                        
    }
                    
    do_command("cast shield on " + this_object()->query_name());
}

void adjust_hp(int number) {
    return;
}