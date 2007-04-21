
inherit "/obj/clothing";

//defines various locations taliswoman will go to
#include "/d/guilds/witches/include/taliswoman.h"

/* start of function prototypes */
void setup();
void init();
void high_level();
void set_destination(string city);
int go_granny( string *args );

/* end of function prototypes */

//Global Variables
nosave mapping _city_destinations = ([
 
   "default" : AM,
   "klk" : KLK,
   "stolat" : STO_LAT,
// "stohelit" : STO_HELIT   
// "genua" : GENUA    

]);
 
nosave private string set_destination;

string _city;


void setup() {

    set_name( "taliswoman" );
    add_alias( ({ "stone", "ass", "thong" }) );
    set_short( "taliswoman" );
    add_adjective( ({ "small", "surly", "ass", "smooth", "leather", "thin",
        "witches'", "witches", "guild", "stone" }) );
    add_property( "talisman", "no one" );
    add_property( "no recycling", 1 );
    set_main_plural( "taliswomen" );

    set_long( "This small stone shaped like a small and surly ass has been "
      "worn smooth by frequent handling.  A thin, leather thong has been "
      "threaded through its hole.  " );
    add_extra_look( this_object() );

    set_value(1);
    set_weight(2);
    set_type( "necklace" );
    set_max_cond( 70 );
    set_cond( 70 );
    
    init_dynamic_arg(query_dynamic_auto_load());
    
} /* setup() */


string extra_look() {

    if( this_player()->query_name() != query_property( "talisman" ) ) {
        return "\n";
    }
    
    return "This taliswoman will get you to the Witches Guild to see Granny "
        "Weatherwax if you rub the tail of the ass, and back to "
        "Ankh-Morpork if you rub its head. You can only use it once in "
        "each direction, but you can get a new one from Gennie if you "
        "haven't outgrown them.\n";
        
} /* extra_look() */


void init() {
    add_command( "rub" , "{head|tail} of <direct:object'ass'>",
      (: go_granny( $4[0] ) :) );
} /* init() */


void high_level() {
    string gender;

    switch( this_player()->query_gender() ) {
        case 0:
            gender = "thing";
            break;
        case 1:
            gender = "boy";
            break;
        case 2:
            gender = "girl";
            break;
    }
    add_failed_mess( "$D says \"You are a big " + gender + " now, so you'll "
        "have to manage without me.  Good luck on your adventures!\"\n", );
    set_worn_by(0);
    this_player()->remove_property( "given taliswoman" );
    move( "/room/rubbish", "", "$N vanishes in a puff of smoke." );
    
} /* high_level() */


void set_destination( string city ) { 
    _city = "city";
} /* set_destination() */


mapping query_dynamic_auto_load() {
    return ([
        "::" : ::query_dynamic_auto_load(),
        "new_destination" : _city,
    ]);
} /* int_query_static_auto_load() */


void init_dynamic_arg( mapping map, object ) {
 
    if (map["::"]) ::init_dynamic_arg(map["::"]);
   
    if (map[ "city" ] ]) _city = map["city"];
        else {
            _city = "default";
        }

} /* init_dynamic_arg() */

int go_granny( string where ) {
    mixed destination;

    destination = _city;


    if( !query_property( "talisman" ) ) {
        add_failed_mess( "$D blinks rapidly and vanishes in a sudden fit "
            "of quantum insecurity.\n" );
        set_worn_by(0);
        move( "/room/rubbish" );
        return -1;
    }

    if( query_property( "talisman" ) != this_player()->query_name() ) {
        this_player()->add_failed_mess( this_object(), "$D isn't yours!\n"
            "It thumbs its metaphorical nose at you and refuses to do "
            "anything.\n" );
        return -1;
    }

    if( where == "tail" ) {

    /*  Gennie won't give you a taliswoman if you're above level 20, so if 
        you've been sneaky and kept one, it shouldn't work. */
    
        if( this_player()->query_level() > 20 ) {
            high_level();
            return -1;
        }
        if( query_property( "been to bad ass" ) ) {
            add_failed_mess( "You have already used $D to travel to the "
                "witches once.\n" );
            return -1;
        }
    
        if( file_name( environment( this_player() ) ) == destination ) {
            add_failed_mess( "But you are already here!\n" );
            return -1;
        }

        add_succeeded_mess( ({ "$D starts to glow.  The bright glow makes "
            "you blink and when you open your eyes, you're in a new place.\n",
            "$N $V $D.\n" }) );

        if( this_player()->query_guild_ob() != "/std/guilds/witch" ) {
            call_out( (: tell_object( $(this_player()), "The taliswoman "
                "says: To the northwest, then north of here is the home of "    
                "Granny Weatherwax.  You need to speak with her about "
                "becoming a witch.  To get into the cottage you will need "
                "the key to the back door; it's hidden in the privy.  To get "
                "back to Ankh-Morpork, you can rub my head or catch the "
                "carriage in Bad Ass to the southeast of here.  "
                "Good luck!\n" ) :), 2 );
            add_property( "been to bad ass", 1 );
        }
    }
    
    else {
   
    /*  Gennie won't give you a taliswoman if you're above level 20, so if 
        you've been sneaky and kept one, it shouldn't work. */
    
        if( this_player()->query_level() > 20 ) {
            high_level();
            return -1;
        }
        if( query_property( "been to city" ) ) {
            add_failed_mess( "You have already used $D to travel to the "
                "city once.\n" );
            return -1;
        }
    
        if( file_name( environment( this_player() ) ) == destination ) {
            add_failed_mess( "But you are already here!\n" );
            return -1;
        }

        add_succeeded_mess( ({ "$D starts to glow.  The bright glow makes "
            "you blink and when you open your eyes, you're in a new place.\n",
            "$N $V $D.\n" }) );
        add_property( "been to city", 1 );
    }

    call_out( (: $(this_player())->move_with_look( $(destination),
        "You notice a glimmering light to the side of the road.  As you "
        "watch, the light begins to take the shape of $N as she "
        "arrives from somewhere else.",
        "A bright light surrounds $N.  It dims suddenly, then "
        "vanishes.  It seems to have taken $N with it." ) :), 0 );

    if( query_property( "been to bad ass" ) && 
        query_property( "been to city" ) ) {
        
        call_out( (: tell_object( $(this_player()), the_short() +
            " says: You have now used me to travel once in each direction, "
            "so our ways part here.  If you need me again, just go see "
            "Gennie.\n" ) :), 1 );
        // so she can get another without waiting 30 minutes...
        this_player()->remove_property( "given taliswoman" );
        call_out( (: set_worn_by(0), move( "/room/rubbish", "",
            "$N vanishes in a puff of smoke." ) :), 1 );
    }
    return 1;
}  /*  go_granny()  */


varargs int add_property( string var, mixed val, int tim ) {

    if( var == "talisman" ) {
        
        if( query_property( "talisman" ) && 
            query_property( "talisman" ) != "no one" ) {
                
            call_out( (: tell_object( $(environment()), "The taliswoman "
                "blinks rapidly and vanishes in a sudden fit of quantum "
                "insecurity.\n" ) :), 1 );
            call_out( (: set_worn_by(0), move( "/room/rubbish" ) :), 1 );
        }
    }
    return ::add_property( var, val, tim );
    
} /* add_property() */



string query_destination() {
    return _city;
} /* query_destination() */


