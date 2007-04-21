inherit "/std/races/humanoid";

#include <armoury.h>

void knock_out( object player, object golem );
void unarmed_hands( int damage, object him, object me, string a_type,
    string a_name );
object make_corpse();


void setup() {
    set_name( "golem" );
    set_long( "Golems are artifical shaped of clay to resemble humanoids and "
        "animated using religious rituals.  If they are actually alive is a "
        "question under constant debate.\n" );
    set_height( 200 );
    set_weight( 40000 );
    set_desc( "a tall, human-shaped creature made of clay" );

    add_property( "no scry", 1 );

    add_ac( "blunt", "pierce", ({150}) );
    add_ac( "pierce", "pierce", ({250}) );
    add_ac( "sharp", "sharp", ({250}) );
    add_ac( "fire", "fire", ({10000}) );
    add_ac( "cold", "cold", ({10000}) );
    add_ac( "acid", "acid", ({10000}) );
    add_ac( "magic", "magic", ({10000}) );

    set_sight( ({ 0, 5, 100, 125 }) );
    set_stats( ({ 30, 10, 0, 40, 0 }) );
} /* void setup() */


varargs int query_ac( string type, int damage, string zone, int amount ) {
    object attacker, weapon;

    if( type == "blunt" )
        return amount;

    attacker = (object)previous_object()->query_attacker();

    if( attacker ) {
        weapon = (object)attacker->query_weapon();
        if( weapon && weapon != attacker )
            weapon->adjust_cond( -(int)weapon->query_cond() / 15 );
    }

    return amount;
} /* varargs int query_ac() */


object make_corpse() {
    object corpse, owner;

    owner = previous_object();
    corpse = ARMOURY->request_item( "golem corpse", 100 );
    all_inventory( owner )->move( corpse );

    return corpse;
} /* object make_corpse() */


void set_unarmed_attacks( object thing ) {
    if( !thing )
        return;
    thing->remove_attack( "hands" );
    thing->remove_attack( "feet" );

    thing->add_attack( "hands", 75, ({ 50, 10, 15 }),
        "blunt", "unarmed", ({ "unarmed_hands", this_object() }) );
} /* void set_unarmed_attacks() */


void unarmed_hands( int damage, object him, object me, string a_type,
    string a_name ) {
    
    // Golems do not kill people.
    if( damage > 130 || damage >= him->query_hp() ) {
        damage = 0;
        knock_out( him, me );
    }
} /* void unarmed_hands() */



/* This should knock the player out of the room, if possible - if there is no
 * suitable exit, he stays in the room.  In any case, he should pass out for
 * 3 - 6 minutes. */
void knock_out( object player, object golem ) {
    object room, target;
    string *all_exits, *no_doors = ({ });
    string exit;
    int i;

    if( !player || !golem )
        return;

    player->stop_all_fight();
    golem->stop_fight( player );

    if( !( room = environment( golem ) ) )
        return;

    all_exits = room->query_dest_dir();
    for( i = 0; i < sizeof( all_exits ); i += 2 )
        if( !room->query_door( all_exits[i], "door" ) )
            no_doors += ({ all_exits[i + 1]  });
    target = load_object( no_doors[ random( sizeof( no_doors ) ) ] );

    tell_object( player,
        golem->the_short() + "' blow lifts you off your feet and hurls "
        "you through the air.\n"
        "You see the world race past you and then the ground approaching very "
        "fast.\n" );
    if( target ) {
        exit = all_exits[ member_array( file_name( target ), all_exits ) - 1 ];
        player->move( target,
            "$N come$s flying through the air from $F in a high arc and "
            "thump$s onto the ground.",
            "$N is lifted off " + player->query_possessive() + " feet by " +
            golem->the_short() + "'s blow and sail$s out of the room "
            "to the " + exit + "." );
    } else
        tell_room( room,
            player->the_short() + " is lifted off " +
            player->query_possessive() + " feet by " +
            golem->the_short() + "'s blow and smashes to the "
            "ground.\n", ({player}) );

    player->set_position( "lying on the ground unconscious" );
    player->add_property( "passed out", 1, ( 150 + random( 150 ) ) );
} /* void knock_out() */


int stone_skinned() {
    return 1;
} /* int stone_skinned() */


