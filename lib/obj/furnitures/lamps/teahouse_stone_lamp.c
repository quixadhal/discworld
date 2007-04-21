// Only for use within CWC - Una 07.02.02


inherit "/std/room/furniture/basic";

#define BRIGHTNESS      60
#define FUEL_PER_TICK  FUEL_TIME

int     fuel_left;
int     is_lit;
int     has_tealight;
mapping vars = (["is lit": 0, "fuel left": 20, "has tealight" : 1 ]);
void    dest_me();
int     do_dowse();
int     do_extinguish();
int     do_light();
void    init();
void    init_arg( mixed arg );
void    out_of_fuel();
string  pretty_plural(string str);
mixed   query_auto_load();
void    setup();
string  short( int i );
int     do_replace();
 
void setup() {

    fuel_left = 3600;
    is_lit    = 0;
    has_tealight = 0;
    
    set_name("lamp");
    set_short("stone teahouse lamp");
    set_weight( 40 );
    set_value( 50000 );
    add_adjective( ({ "stone", "teahouse" }) );
    add_alias( ({ "teahouse lamp", "stone lamp" }) );
    
    set_long( function() {
    string str;
    str = "Shaped like an Agatean teahouse, this lantern creates "
        "a peaceful ambiance of Auriental tradition in your home as "
        "it casts a beautiful soft glow through its calligraphed rice "
        "paper windows.  Exquisitely handcrafted in the shape of an "
        "Agatean teahouse, the front door of the lantern swings open "
        "to reveal the tealight inside.  ";
        
        if(has_tealight && is_lit) {
            fuel_left = find_call_out("out_of_fuel");
            return str + "The tealight is burning with a steady yellow "             
                "light.";
        }
    
        if(has_tealight) {
            return str + "An unlit tealight is sitting in the base.";
        }
    
        if(!fuel_left) {
            return str + "The tealight has burned itself out.";
        }
    
        else {
            return str + "There is no tealight in the lamp.";
        
        } 
    } );   
    
    set_allowed_positions(({"sitting","standing"}));
    set_allowed_room_verbs((["sitting" : "sits", "standing" : "stands" ]));
    set_weight(9);
    set_value(20500);
        
    init_dynamic_arg( query_dynamic_auto_load() );
  
} /* setup() */


void init() {
    
    add_command("replace", "tealight in <direct:object:me>");
    this_player()->add_command("light", this_object());
    this_player()->add_command("extinguish", this_object());
} /* init() */


void dest_me() {
    set_light( 0 );
    ::dest_me();
} /* set_light() */
 
 
int do_extinguish() {

    if ( !has_tealight ) {
        this_player()->add_failed_mess( this_object(), "There is no "
            "tealight in $D\n", ({ }) );
        return 0;
    }
    
    if ( !is_lit ) {
        this_player()->add_failed_mess( this_object(), "$D is not "
        "lit.\n", ({ }));
        return 0;
    }
   
    this_player()->add_succeeded_mess( this_object(), "$N $V the "
        "remnants of the tealight in the $De.\n");
    fuel_left = remove_call_out( "out_of_fuel" );
    is_lit = 0;
    has_tealight = 1;
    set_light( 0 );
    return 1;
} /* do_extinguish() */

int do_light() {

    if ( is_lit ) {
        this_player()->add_failed_mess( this_object(), "$D is already "
            "lit.\n", ({ }) );
        return 0;
    }
   
    if( !has_tealight && fuel_left <= 0 ) {
        return this_player()->add_failed_mess( this_object(), "The "
            "tealight has burned itself out and needs to be replaced.\n",
            ({ }) );
        return 0;
    }
    
    if( !has_tealight ) {
        this_player()->add_failed_mess( this_object(), "There is no tealight "
            "in $D.\n", ({ }) );
        return 0;
    }
 
    call_out( "out_of_fuel" , fuel_left );
    is_lit = 1;
    has_tealight = 1;
    set_light( BRIGHTNESS );
    return 1;
    
} /* do_light() */
 
int do_replace() {
    int i;
    object *things;
    things = all_inventory( this_player() );
    for( i = 0; i < sizeof( things ); i++ ){
        if( !has_tealight && things[ i ]->query_name() == "tealight" ) {
            fuel_left = 3600;
            is_lit = 0;
            has_tealight = 1;
            destruct( things[ i ] );
            this_player()->add_succeeded_mess( this_object(), "$N $V the "
                "tealight in the $D.\n", ({ }) );
            return 1;
        }
        
        if( has_tealight && things [ i ]->query_name() == "tealight" ) {
            return this_player()->add_failed_mess( this_object(), "There is "
                "already a tealight in $D.\n", ({ }) );
            return 1;
        }
    }        
    return notify_fail( "You do not have a tealight to put in the lamp.\n" );
}

mapping query_dynamic_auto_load() {
    /*
    if (is_lit) {
        fuel_left = find_call_out("out_of_fuel");
        return ([ "::" : ::query_dynamic_auto_load(),
            "fuel left" : fuel_left,
            "is lit" : is_lit,
            "has tealight" : has_tealight
        ]);
    }
    else if (has_tealight) {
        return ([ "::" : ::query_dynamic_auto_load(),
            "fuel left" : fuel_left,
            "is lit" : is_lit,
            "has tealight" : has_tealight
        ]);
    }
    */
    return ([ "::" : ::query_dynamic_auto_load(),
            "fuel left" : fuel_left,
            "is lit" : is_lit,
            "has tealight" : has_tealight
        ]);
       
} /* query_dynamic_auto_load() */

void init_dynamic_arg(mapping arg) {

    if (arg["::"]) ::init_dynamic_arg(arg["::"]);

    if (arg["fuel left"]) {
        fuel_left = arg["fuel left"];
    }
    else {
        fuel_left = 0;
    }
    
    if (arg["is lit"]) {
        call_out("out_of_fuel", fuel_left);
        is_lit = 1;
        set_light(BRIGHTNESS);
    }   
    if (arg["has tealight"]) { 
        has_tealight = 1;
        set_light(0);
    }    
    else {
        has_tealight = 0;
        is_lit = 0;
    }

} /* init_dynamic_arg() */   
 
void out_of_fuel() {
    object env;
    env = environment(this_object());
    
    is_lit = 0;
    has_tealight = 0;
    fuel_left = 0;
    set_light( 45 );
    tell_room(env, "The tealight in the lamp burns out.\n" );
    
} /* out_of_fuel() */
 
string pretty_plural(object str) {
    if ( is_lit )
        return ::pretty_plural( str) + " (lit)";
    return ::pretty_plural( str);
}

string short( int i ) {
    if ( is_lit )
        return ::short( 0 ) + " (lit)";
    return ::short( 0 );
}

