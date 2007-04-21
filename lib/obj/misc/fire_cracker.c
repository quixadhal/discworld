inherit "/std/object";

#define EFFECT "/w/dogbolter/cracker_eff"
#define DURATION 180

void setup() {
    set_name( "firecracker" );
    add_alias( "cracker" );
    add_adjective( "fire" );
    add_plural( "crackers" );
    set_short( "fire cracker" );
    set_long( "This is a large red fire cracker of Auriental manufacture"
      ".  It's the sort of thing that is thrown at weddings.  You may "
      "'light' it, and 'throw' it.\n" );
    set_weight( 2 ); // Velly Big Clackah!
    set_value( 500 );
    set_read_mess("ACME Firecracker", "agatean");
    //add_property( "no recycling", 1 );
}

void explosion() {
    object victim;
    if( !environment() )
        return;

    if( living( environment() ) ) {
        victim = environment();
        // A living is holding the clackah when it explodes.  Tell them
        // and tell the room.
        tell_object( environment(), "%^BOLD%^%^RED%^$C$"+ the_short()
          +" explodes in your face!\nMeep meep!%^RESET%^\n" );
        environment()->add_effect (EFFECT, DURATION);
            
        if( environment( environment() ) ) {
            tell_room( environment( environment() ), "%^BOLD%^%^RED%^$C$"+
              the_short() +" explodes in "+
              environment()->the_short() +"'s face!\nMeep meep!%^RESET%^\n",
              environment() );

            // Hurt them by adjusting heir hp downwards.
          //  victim->adjust_hp( -5000000 );
            // Check if they were killed
            // Make the cracker "attack" the victim to show up in death inform
          //  if( victim->query_hp() < 0 )
          //      victim->attack_by( this_object() );
                
        }
    } else {
        tell_room( environment(), "%^BOLD%^%^RED%^$C$"+ the_short()
          +" explodes!%^RESET%^\n" );
    }

    move( "/room/rubbish" );
}

int do_light() {
    call_out( (: explosion :), 10 );

    add_succeeded_mess( "$N $V $D and smoke begins to pour out.\n",
      ({ }) );
    return 1;
}





int do_throw() {
    if( environment() && environment( environment() ) ) {
        add_succeeded_mess( "$N $V $D on the ground.\n", ({ }) );
        move( environment( environment() ) );
        return 1;
    } else {
        add_failed_mess( "There is no ground to $V $D on.  How very odd\n",
          ({ }) );
        return 0;
    }
}

void init() {
    add_command( "light", "<direct:object:me>", (: do_light :) );
    add_command( "throw", "<direct:object:me>", (: do_throw :) );
}
string query_death_reason() {
    return "being blown up by a fire cracker, in true Road Runner Style.\n"
    "%^BOLD%^Meep meep!%^RED%^";
}




