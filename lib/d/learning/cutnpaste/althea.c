/*
 * $Locker:  $
 * $Id: althea.c,v 1.8 2000/10/27 02:13:48 twiggy Exp $
 *
 * $Log: althea.c,v $
 * Revision 1.8  2000/10/27 02:13:48  twiggy
 * Adjusted her to use queue_command instead of do_command when she drops or fumbles her flail, to make her a little more realistic.
 *
 * Revision 1.7  2000/10/17 05:57:39  taffyd
 * Whatever changes Taffyd made in 1999.
 *
 * Revision 1.6  1999/07/13 05:42:56  furcifer
 * fixed typo.
 *
 * Revision 1.5  1999/05/16 17:47:26  taffyd
 * Fixes!
 *
 * Revision 1.1  1998/02/08 08:44:56  terano
 * Initial revision
 *
*/
#define CREATOR "Ceres"
/*
 * This is Althea, the Archcardinal of Pishe. Another attempt by me to
 * create an interesting female npc.
*/
#include <armoury.h>
inherit "/obj/monster";

#define HEAL_COST 30
object flail;
int dickens_warned;

void do_heal(object);
void check_injured();
void maintain_self(object, object);
void fighting_stuff(object, object);
void ritual_stuff(object, object);

void setup() {
    set_name("althea");
    set_short("Althea of Pishe");
    set_main_plural("Altheas of Pishe");
    set_long("This is Althea, an Archcardinal of Pishe.  She is a beautiful, "
      "middle-aged woman with a kind face.  She almost seems to glow "
      "with a gentle power.\n");
    add_adjective(({"arch", "cardinal", "of", "pishe"}));
    add_alias(({"priestess", "archcardinal", "cardinal"}));
    add_property("determinate","");
    add_property("unique", 1);
    set_gender("female");
    set_race("human");
    set_guild("priest");
    set_deity("pishe");
    set_level(400);

    set_height(140);
    set_weight(1000);
    set_al(-2000 - random(501)); /* Max alignment is -2500 */
    set_con(15);
    set_dex(13);
    set_int(12);
    set_str(15);
    set_wis(20);

    add_skill_level("fighting", 300, 0);
    add_skill_level("fighting.combat.melee.blunt", 150, 0);
    add_skill_level("faith", 200);
    add_skill_level("faith.rituals.misc.area", 300);

    add_skill_level("other.perception", 400, 0);
    add_skill_level("other.health", 200, 0);
    add_known_command("strike");
    add_known_command("crush");

    add_spell("cure light wounds", "/obj/rituals/curelight", "cast_spell");
    add_spell("cure serious wounds", "/obj/rituals/cureserious", "cast_spell");
    add_spell("cure critical wounds", "/obj/rituals/curecritical", "cast_spell");
    add_spell("minor shield", "/obj/rituals/minorshield", "cast_spell");
    add_spell("dust devil", "/obj/rituals/dustdevil", "cast_spell");

    add_combat_action(100, "maintenance", (: maintain_self :)) ;
    add_combat_action(70, "fighting_stuff", (: fighting_stuff :));
    add_combat_action(50, "ritual_stuff", (: ritual_stuff :));

    ARMOURY->request_item("white cotton tunic", 100)->move(this_object());
    ARMOURY->request_item("rope belt", 100)->move(this_object());
    ARMOURY->request_item("soft leather shoes", 100)->move(this_object());
    clone_object("/obj/faith/pishe_symbol.ob")->move(this_object());

    do_command("tactics response parry");
    do_command("tactics attitude defensive");

    load_chat(20, ({ 2, ":smiles kindly.",
        1, "'May the countenance of Pishe rain upon you.",
        2, ":smiles serenely." }));

    load_a_chat(20, ({ 1, "'Do you know who I am?",
        1, "'May the power of Pishe strike you down.",
        1, "'Die unbeliever!",
        1, "'You fight me, you fight Pishe herself",
        1, "'Pishe! Aid your loyal servant!" }));

    set_virtual_move(1);
    set_move_after(30, 120);
    add_move_zone("Ankh-Morpork");
    add_move_zone("Temple");
    set_wimpy(10);

    add_respond_to_with(({ "@say",
        ({ ({ "heal", "healing" }), "please" }) }), (: do_heal :));
    add_respond_to_with(({ "@say",
        ({ "please", "heal", "me" }) }), (: do_heal :));

    add_respond_to_with(({ "@smile", "althea" }), "smile $hname$");
    add_respond_to_with(({ ({ "@bow", "@curtsey" }), "althea" }),
      "curtsey $hname$");

    add_enter_commands((: check_injured :));


    flail = ARMOURY->request_item("cleric rod", 100);
    flail->set_enchant(flail->query_max_enchant() / 2);
    flail->whichone(7);
    flail->move(this_object());
    call_out((: init_equip :), 2);

    move("/d/guilds/priests/Ankh-Morpork/small-gods/old/PriestGardenStatue", "$N walk$s into the "
      "room, smiling serenely.");
} /* setup() */

// A couple of things Althea does during fights just to spice her up a bit

void maintain_self(object thing1, object thing2) {
    if (thing2 != this_object())
        return;

    /* Try and pick up our flail if we've been disarmed. */
    if (member_array(flail, query_holding()) == -1) {
        queue_command("get flail");
        queue_command("hold flail");
    }

    /* Althea always has enough guild points to cast her healing and
       defensive rituals */

    if (query_gp() < 200)
        adjust_gp(200);

    if (query_hp() < query_max_hp() / 2) {
        do_command("cast cure serious wounds on althea");
    }

/*
    if (query_hp() < query_max_hp() / 4) {
        do_command("use flail to cast sanctuary");
    } */

    if(query_hp() < query_max_hp() - 1000) {
        do_command("cast dust devil on althea");
    }

    if (query_hp() < query_max_hp() - 500) {
        do_command("cast minor shield on althea");
    }

    if (query_hp() < query_max_hp() - 250) {
        do_command("cast cure light wounds on althea");
    } 
} /* maintain_self() */

void fighting_stuff(object thing1, object thing2) {
    if (thing2 != this_object())
        return;

    if ( random( 10 ) )
        return;
   do_command("concentrate " + thing1->query_name());
    do_command("tactics attitude offensive" );
    do_command("crush "+ thing1->query_name() + " with flail");
}

void ritual_stuff(object thing1, object thing2) {
    if (thing2 != this_object())
        return;

    switch (random(6)) {
    case 0..1:
        do_command( "cast minor shield on althea" );
        break;
/*    case 2 :
        do_command( "use flail to cast barrier" );
        break; */

    case 2 .. 4 :
        do_command("cast dust devil on althea");
        do_command("cast dust devil on althea");
        do_command("cast dust devil on althea");
        break;

    case 5 :
        do_command("use flail to cast fear on " + thing1->query_name());
        break;

    }
    do_command("tactics attitude defensive");
} /* ritual_stuff() */

void do_heal(object victim) {
    if (member_array(victim, query_attacker_list()) != -1) {
        do_command("say I'm not going to heal you!");
        return;
    }

    if (victim->query_hp() > (victim->query_max_hp() / 2)) {
        do_command("say You don't need healing from me, " + 
          victim->short()+".");
        return;
    }

    if (victim->query_gp() < HEAL_COST) {
        do_command("say Sorry you're not strong enough for me to "
          "heal at the moment.\n");
        return;
    }

    if (victim->query_guild_ob() == "/std/guilds/wizard") {
        do_command("say I thought wizards didn't believe in gods?");
        do_command("smirk");
        return;
    }

    if (victim->query_al() > random(300)) {
        do_command("say You are far too wicked for Pishe's healing.");
        return;
    }

    do_command("say May the power of Pishe heal you.");
    tell_room(environment(), "Althea places her hands upon " +
      victim->the_short() + " and starts to pray softly.\n");

    tell_room(environment(), "As she does so her hands start to glow with a "
      "gentle silver light.\nThe light gradually intensifies until "
      "everything is bathed in it.\n");

    tell_object(victim, "As Althea prays you feel warmth spreading from her "
      "hands throughout your body.\n");
    tell_room(environment(), "Gradually the light fades.\n");
    tell_object(victim, "You feel much better.\n");
    tell_room(environment(), victim->the_short()+" looks much better.\n",
      victim);
    victim->set_hp(victim->query_max_hp());
    victim->adjust_gp(-HEAL_COST);
} /* do_heal() */


void check_injured() {
    object *people, person;

    people = filter(all_inventory(environment()), (: living($1) &&
        ($1->query_hp() < ($1->query_max_hp() / 2)) &&
        $1->query_al() < -300 :));

    people -= ({ this_object() });

    if (!sizeof(people))
        return;

    person = people[random(sizeof(people))];    

    do_command("say Oh my, " + person->short()+ " you don't look so good.");
    do_command("say Allow me to heal you.");
    call_out((: do_heal($1) :), 1, person);
} /* check_injured() */

void event_fight_in_progress(object attacker, object victim) {

    if ( attacker != this_object() && victim->query_name() == "dickens" ) {
        if ( !dickens_warned ) {
            do_command( "say Fear not, Dickens, Pishe"
                " shall protect you!" );
            dickens_warned = 1;
            do_command( "protect dickens" );
            attack_ob( attacker );
        }
    }

    ::event_fight_in_progress( attacker, victim );
} /* event_fight_in_progress() */
     
int adjust_hp(int hp, object attacker) {
    int result;

    result = ::adjust_hp(hp, attacker);

    if(hp < -1500)
        call_out("eek", 0);

    return result;
} /* adjust_hp() */

void eek() {
    init_command("use flail to cast sanctuary", 1);
    init_command("cast cure critical wounds on althea", 2);
    run_away();
} /* eek() */

void check_fumble( object ob ) {
    if ( environment( ob ) == environment() ) {
        do_command( "say Whoops, I must have dropped my precious flail!" );
        do_command( "get " + file_name( ob ) );
        do_command( "hold " + file_name( ob ) );
    }
} /* check_fumble() */
        
void event_enter( object ob, string mess, object from ) {
    ::event_enter( ob, mess, from );

    if ( environment( ob ) == environment() && 
        ob->id( "flail" ) && ob->query_cloned_by() == "althea" ) {
        call_out( (: check_fumble :), 1, ob );
    }
}  /* event_enter() */
