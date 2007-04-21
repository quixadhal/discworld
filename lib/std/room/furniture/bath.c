/**
 * This is a bath tub inheritable.  It will wash off stuff and also allow
 * people to splash each other and other exciting and wild things along
 * those lines.
 * @author Triana
 */
#include <effect.h>

inherit "/std/room/furniture/surface";
/* #include "path.h"; */

#define WET_EFFECT "/std/effects/other/wetness"
#define SOAP_EFFECT "/std/effects/healing/antiseptic_soap"

/* Is it full of water? */
int water = 0;
int dirt = 0;

string* dirt_messages = ({
    "clear",
    "almost clear",
    "soapy",
    "foamy",
    "slightly dirty",
    "fairly dirty",
    "dirty",
    "very dirty",
    "muddy"
});

void zap_bad_effects( object who );
int do_wash(object *indirect_obs );

void create(){
    ::create();
    add_property( "no recycling", 1 );
    this_object()->add_extra_look( this_object() );
    add_help_file("bath");
}
/* setup() */

/** @ignore yes */
string extra_look() {
    object *stuff;
    string answer = "";

    stuff = all_inventory( this_object() ); 
    if (water) {
        if (dirt >= sizeof(dirt_messages)) {
            dirt = sizeof(dirt_messages) - 1;
        }
        answer = answer + "It's about three quarters full of " + 
            dirt_messages[dirt] + " water.\n";
    }
    if( sizeof( stuff ) ) {
        answer = answer + "It has "+ query_multiple_short( stuff ) +" in it.\n"; 
    }

   return answer;
}
/* extra_look */

/** @ignore yes */
string long_status() {
    return "";
}

/** @ignore yes */
void init(){
    ::init();
    add_command( "lie", "in <direct:object>");
    add_command( "sit", "in <direct:object>");
    add_command( "kneel", "in <direct:object>");
    add_command( "stand", "in <direct:object>");
    add_command( "leave", "<direct:object>");
    add_command( "fill", "<direct:object>");
    add_command( "empty", "<direct:object>");
    add_command( "splash", "<indirect:living:here>");
    add_command( "wash", "<indirect:living:here>");
} /* init() */

/** @ignore yes */
int can_player_get_in_it() {
    if (environment(this_object())!=environment(this_player())) {
      this_player()->add_failed_mess(this_object(), "The " + this_object()->short() + " needs to "
      "be on the floor for you to get in it.\n");
      return 0;
    }
    /*
    if (this_player()->query_complete_weight()
        +this_object()->query_complete_weight()
        >this_object()->query_max_weight()) {
      this_player()->add_failed_mess(this_object(), "The bath tub is "
      "too full of other things.\n");
      return 0;
    }
    */
    if( environment( this_object() ) == this_player() ){
        this_object()->move(environment(this_player() ));
        tell_object(this_player(), "You drop the "+ 
            this_object()->short()+".\n");
        tell_room(environment(this_player()), this_player()->short()+" drops "
        "a "+this_object()->short()+".\n", this_player() );
    
    } 
    return 1;
}

/** @ignore yes */
void wet_everyone_in() {
    object *victims;
    if (!water) {
        return;
    }
    victims = filter(all_inventory(environment(this_object())),
        (: living($1) :) );
    foreach( object victim in victims ) {
        if (victim -> query_position_type() == "in" &&
            victim -> query_position_on() == "the "+this_object()->short()) {

            victim->add_effect( WET_EFFECT, 500 );
        }
    }
}

/** @ignore yes */
void reset() {
    wet_everyone_in();
}

/** @ignore yes */
int do_fill() {
    object player;
    player=this_player();
    if (water) {
        player->add_failed_mess(this_object(), "The " + this_object()->short() + " is "
            "already full of water.\n");
        return 0;
    }
    add_succeeded_mess( "", ({ }) );
    tell_object(player, "You fill the " + this_object()->short() + " with clean water.\n" );
    tell_room(environment(player), player->short()+" fills "
        "the " + this_object()->short() + " with clean water.\n", player );
    water=1;
    dirt=0;
    wet_everyone_in();
    return 1;
}

/** @ignore yes */
int do_empty() {
    object player;
    player=this_player();
    if (!water) {
        player->add_failed_mess(this_object(), "The " + this_object()->short() + " is "
            "already as empty as it gets.\n");
        return 0;
    }
    add_succeeded_mess( "", ({ }) );
    tell_object(player, "You reach into the " + this_object()->short() + " and "
                "pull the plug, allowing the water to drain away.\n" );
    tell_room(environment(player), player->short()+" reaches into "
        "the " + this_object()->short() + " and pulls the plug, allowing "
        "the water to flow away.\n", player );
    water=0;
    dirt=0;
    return 1;
}

int do_lie() {
    object player;
    if (!can_player_get_in_it()) {
        return 0;
    }
    player=this_player();
    add_succeeded_mess( "", ({ }) );
    if (player->query_position_on() == "the "+this_object()->short()) {
        if (player->query_position() == "lying") {
            tell_object(player, "You are already lying in the pool.\n" );
            return 1;
        }
        tell_object(player, "You lie in the " + this_object()->short() + ".\n" );
        tell_room(environment(player), player->short()+" lies "
            "in the " + this_object()->short() + ".\n", player );
    }
    else {
        tell_object(player, "You step into the " + this_object()->short() + " and lie down.\n" );
        tell_room(environment(player), player->short()+" steps "
            "into the " + this_object()->short() + " and lies down.\n", player );
        if (water) {
            player->add_effect( WET_EFFECT, 500 );
        }
    }
    player->set_position( "lying" );
    player->set_position_type( "in" );
    player->set_position_on( "the "+this_object()->short() );
    return 1;
}
/* lie() */

/** @ignore yes */
int do_sit() {
    object player;
    if (!can_player_get_in_it()) {
        return 0;
    }
    player=this_player();
    add_succeeded_mess( "", ({ }) );
    if (player->query_position_on() == "the "+this_object()->short()) {
        if (player->query_position() == "sitting") {
            tell_object(player, "You are already sitting in the " + this_object()->short() + ".\n" );
            return 1;
        }
        tell_object(player, "You sit in the " + this_object()->short() + ".\n" );
        tell_room(environment(player), player->short()+" sits "
            "in the " + this_object()->short() + ".\n", player );
    }
    else {
        tell_object(player, "You step into the " + this_object()->short() + " and sit down.\n" );
        tell_room(environment(player), player->short()+" steps "
            "into the " + this_object()->short() + " and sits down.\n", player );
        if (water) {
            player->add_effect( WET_EFFECT, 500 );
        }
    }
    player->set_position( "sitting" );
    player->set_position_type( "in" );
    player->set_position_on( "the "+this_object()->short() );
    return 1;
}
/* sit() */

int do_kneel() {
    object player;
    if (!can_player_get_in_it()) {
        return 0;
    }
    add_succeeded_mess( "", ({ }) );
    player=this_player();
    if (player->query_position_on() == "the "+this_object()->short()) {
        if (player->query_position() == "kneeling") {
            tell_object(player, "You are already kneeling in the " + this_object()->short() + ".\n" );
            return 1;
        }
        tell_object(player, "You kneel in the " + this_object()->short() + ".\n" );
        tell_room(environment(player), player->short()+" kneels "
            "in the " + this_object()->short() + ".\n", player );
    }
    else {
        tell_object(player, "You step into the " + this_object()->short() + " and kneel down.\n" );
        tell_room(environment(player), player->short()+" steps "
            "into the " + this_object()->short() + " and kneel down.\n", player );
        if (water) {
            player->add_effect( WET_EFFECT, 500 );
        }
    }
    player->set_position( "kneeling" );
    player->set_position_type( "in" );
    player->set_position_on( "the "+this_object()->short() );
    return 1;
}
/* kneel() */

/** @ignore yes */
int do_stand() {
    object player;
    if (!can_player_get_in_it()) {
        return 0;
    }
    add_succeeded_mess( "", ({ }) );
    player = this_player();
    if (player->query_position_on() == "the "+this_object()->short()) {
        if (player->query_position() == "standing") {
            tell_object(player, "You are already standing in the " + this_object()->short() + ".\n" );
            return 1;
        }
        tell_object(player, "You stand in the " + this_object()->short() + ".\n" );
        tell_room(environment(player), player->short()+" stands "
            "in the " + this_object()->short() + ".\n", player );
    }
    else {
        tell_object(player, "You step into the " + this_object()->short() + ".\n" );
        tell_room(environment(player), player->short()+" steps "
            "into the " + this_object()->short() + ".\n", player );
        if (water) {
            player->add_effect( WET_EFFECT, 500 );
        }
    }
    player->set_position( "lying" );
    player->set_position_type( "in" );
    player->set_position_on( "the "+this_object()->short() );
    return 1;
}
/* lie() */

/** @ignore yes */
void event_exit(object ob, string message, object to) {
  if (ob==this_player() && this_player()->query_position_on() == this_object()) {
    this_player()->return_to_default_position(1);
  }
}
/* event_exit() */

/** @ignore yes */
int do_leave(){
    
    if (this_player()->query_position_on() == "the "+this_object()->short()) {
        this_player()->return_to_default_position(1);
        tell_object(this_player(), "You clamber out of the " + this_object()->short() + ".\n" );
        tell_room(environment(this_object()), this_player()->short()+" clambers "
            "out of the " + this_object()->short() + ".\n", this_player() );
    }
    else {
        tell_object(this_player(), "Umm... You're not in the " + this_object()->short() + " in the "
            "first place.\n" );
    }
    add_succeeded_mess( "", ({ }) );
    return 1;
}
/* do_leave() */

/** @ignore yes */
int do_splash( object *indirect_obs ) {
    if (environment(this_object())!=environment(this_player())) {
      this_player()->add_failed_mess(this_object(), "The " + this_object()->short() + " needs to "
      "be on the floor for you to splash with it.\n");
      return 0;
    }
    if (!water) {
        this_player()->add_failed_mess(this_object(), "It's not a bad idea,  "
            " but maybe you'd need some water in the " + this_object()->short() + 
            " before you can splash.\n");
        return 0;
    }
    if ( sizeof( indirect_obs ) ) {
        add_succeeded_mess( "$N splash$es $I.\n", indirect_obs );
        return 1;
    }
    
    this_player()->add_succeeded_mess( this_object(),
        "$N splash$es $oself.\n", ({ }) );
    return 1;
}

int do_wash( object *indirect_obs ) {   

    if (!can_player_get_in_it()) {
        return 0;
    }
    if (this_player() -> query_position_type() != "in" &&
        this_player()->query_position_on() != "the "+this_object()->short()) {
      this_player()->add_failed_mess(this_object(), "You need to get "
                                     "into the " + this_object()->short() +
                                     " first.\n");
      return 0;
    }
    if (!water) {
        this_player()->add_failed_mess(this_object(), "It's not a bad idea,  "
            " but maybe you'd need some water in the " + this_object()->short() + 
            " before you can wash.\n");
        return 0;
    }
    if (dirt >= sizeof(dirt_messages) - 1) {
        this_player()->add_failed_mess(this_object(), "The water in the " +
            this_object()->short() +
            " looks so muddy, it probably would just soil anyone.  Maybe " +
            " it would be a better idea to empty it and fill it again " +
            " with fresh water.\n");
        return 0;
    }

    if ( sizeof( indirect_obs ) ) {
        foreach( object victim in indirect_obs ) {
            call_out( (: zap_bad_effects( $1 ) :), 0, victim );
        }
        
        add_succeeded_mess( "$N wash$es $I using a bar of pleasant smelling soap.\n", indirect_obs );
        return 1;
    }
    
    call_out( (: zap_bad_effects( $1 ) :), 0, this_player() );
    this_player()->add_succeeded_mess( this_object(),
        "$N wash$es $oself using a bar of pleasant smelling soap.\n", ({ }) );
    return 1;
} /* wash_self() */

/**
 * This method returns 1 if it is a bath.
 * @return 1 if it is a bath
 */
int query_baths() {
    return 1;
} /* query_baths() */

/**
 * Since we have a limited quantity of water here, it may wash an effect
 * right away, or it may need more washing.  It also means the water
 * itself will become dirty in the process.  The dirtier the water is,
 * the less likely it is to wash anything out.  Fresh water has about
 * 50% chance of washing anything out.
 */
void zap_bad_effects( object who ) {
    int i, flag, *enums;
    class effect *effs;
    
    dirt++;

    effs = (class effect *)who->query_effs();
    enums = (int *)who->effects_matching( "body.smell" );
    i = sizeof( enums );
    if ( !random(dirt) && i ) {
        i = random( i );
        flag = 1;
        write( "The antiseptic power of the soap defeats "+
          (string)effs[ enums[ i ] ]->ob_name->smell_string( who,
            effs[ enums[ i ] ]->arg ) +".\n" );
        who->delete_effect( enums[ i ] );
        dirt ++;
    }

    enums = who->effects_matching("pie.cream");
    if (!random(dirt) && sizeof(enums)) {
        tell_object(who, "The cream washes right off your body.\n");
        who->delete_effect(enums[0]);
        dirt ++;
    }

    enums = (int *)who->effects_matching( "misc.ketchup" );
    i = sizeof( enums );
    if ( !random(dirt) && i ) {
        i = random( i );
        flag = 1;
        write( "The soap rinses the ketchup off your face.\n" );
        who->delete_effect( enums[ i ] );
        dirt ++;
    }

    enums = who->effects_matching("exploded.apples");
    if (!random(dirt) && sizeof(enums)) {
        tell_object(who, "The apples wash right off your body.\n");
        who->delete_effect(enums[0]);
        dirt ++;
    }
    
    enums = (int *)who->effects_matching("object.label");
    if (!random(dirt) && sizeof(enums)) {
        // They cannot have this effect if they have no labels...
        tell_object(who, "The labels stuck on you peel off in "
          "the water.\n");
        who->delete_effect(enums[0]);
        dirt ++;
    }

    enums = who->effects_matching("body.frog-adhesive");
    if (!random(dirt) && sizeof(enums)) {
        tell_object(who, "The frog adhesive washes off in the water.\n");
        who->delete_effect(enums[0]);
        dirt ++;
    }

    enums = who->effects_matching("body.writing.soap");
    if (!random(dirt) && sizeof(enums)) {
        tell_object(who, "The rest of the words wash off without a single "
            "bubble in sight.\n");
        who->delete_effect(enums[0]);
        dirt ++;
    }

    if (!random(dirt)) {
        /* cure the fleas */
        who->add_effect( SOAP_EFFECT, 10000 );
    }
    who->add_effect( WET_EFFECT, 250 );
} /* zap_bad_effects() */
