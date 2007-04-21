/*  Wearble Pet Inherit  *
 *        Wobin          *
 *       13/12/00        */

/**
 * Note, expand_mon_string will possibly be required to be modified
 */

#include <gossip.h>
#include <armoury.h>
#define CHAT_FREQ 200

inherit "/obj/monster";
inherit "/std/basic/wearable";

varargs int wear_remove_function(object);

void expand_mon_string( mixed str ) {
    object room;
    string *args, *stuff;

    if ( functionp( str ) ) {
        evaluate( str, this_object() );
    }

    if(strsrch(str, "$name$") > 0) {
        stuff = GOSSIP_HANDLER->query_random_gossip();
        str = replace(str, ({ "$name$", stuff[0],
            "$mess$", stuff[1] }));
    }
    str = expand_string(str, 0);

    switch( str[0] ) {
    case '#':
        args = explode(str[ 1..], ":");
        switch (sizeof(args)) {
        case 1 :
            call_other( this_object(), args[0] );
            return;
        case 2 :
            call_other( this_object(), args[0], args[1] );
            return;
        case 3 :
            call_other( this_object(), args[0], args[1], args[2] );
            return;
        case 4 :
            call_other( this_object(), args[0], args[1], args[2],
              args[3] );
            return;
        default :
            call_other( this_object(), args[0], args[1], args[2],
              args[3], args[4] );
            return;
        }

    case ':' :
        str = str[1..];
        break;
    case '\'' :
        str = "says " + str;
        break;
    case '"' :
        str = "says loudly " + str;
        break;
    }
    if(living(environment())) {
        room = environment(environment());
        str = capitalize(environment()->poss_short()) +" "
        + this_object()->query_short()  + " " + str;
        tell_room( room,  str +"\n" );
    } else {
        room = environment();
        str = "The " + this_object()->query_short() + " " + str;
        tell_room( room, expand_string( str, 0 ) +"\n" );
    }


} /* expand_mon_string() */

int check_anyone_here() {
object holder, *people;

    if(!(::check_anyone_here()))
        if ( living(holder = environment(this_object())) ) {
            if(environment(holder) && 
                    sizeof(people = all_inventory(environment(holder))))
            if(sizeof(filter(people, (: userp($1) :))))
                return 1;
            else
                return 0;
    }
    return ::check_anyone_here();
} /* check_anyone_here() */

varargs int wear_remove_function(object pet, object thing) {
    object *wearing;
    object worn;

    if(pet && thing) {
        wearing = this_player()->query_wearing();
        foreach(worn in wearing) {
            if( (worn != previous_object()) 
                && inherits("/obj/misc/wearable_pet", worn)) {
                previous_object()->init_command(sprintf("kill %O",worn));
            }
        }
    }
}

/**		
 * Inheritance overriding to organise the shared functions
 */

void create() {
    wearable::create();
    monster::create();
} /* create() */

string long(string str, int dark) {
    return monster::long(str, dark);
} /* long() */

void init_dynamic_arg(mapping map) {
    wearable::init_dynamic_arg(map);
    monster::init_dynamic_arg(map);
} /* init_dynamic_arg() */

mapping query_dynamic_auto_load() {
    return
    wearable::query_dynamic_auto_load() +
    monster::query_dynamic_auto_load();
} /* query_dynamic_auto_load() */

void init_static_arg(mapping map) {
    wearable::init_static_arg(map);
    monster::init_static_arg(map);
} /* init_static_arg() */

mixed query_static_auto_load() {
    wearable::query_static_auto_load() +
    monster::query_static_auto_load();
} /* query_static_auto_load() */

// Let them have a value.
int query_value() {
    return query_base_value();
}

mixed stats() {
    return wearable::stats() + monster::stats();
} /* stats() */

/** 
 *Now to add in the Living/Clothing interfaces. We want all our pets to act as 
 * clothing worn and take damage, but respond as a living and die if 'broken'
 */

void break_me() {
    do_death();   
} /* break_me() */

string cond_string() {
   health_string();
} /* cond_string() */

int adjust_cond( int i ) {
    adjust_hp( i );
} /* adjust_cond() */

/* Now the tricky back de doody */
varargs int adjust_hp( int number, object attacker, object weapon, 
    string attack ) {
    if(::adjust_cond( number ))
      return ::adjust_hp( number, attacker, weapon, attack);
}

int set_max_hp( int number ) {
    set_max_cond(number);
    set_cond(number);
    set_damage_chance( 15 );
    set_lowest_cond(1);
    return ::set_max_hp(number);
}
