// Only for use within CWC - Una 07.02.02


inherit "/std/room/furniture/basic";

int folded;

int do_unfold(object ob);
int do_fold(object ob);

void setup() {

    folded = 0;
    set_name( "futon" );
    set_short( "traditional red futon" );
    add_adjective( ({ "traditional", "red" } ));
    add_alias( "couch" );
    add_alias( "sofa" );
    set_long( "This red mattressed futon is made of a pad of tufted "
        "cotton batting covered with red cotton cloth, set in a "
        "lacquered oaken frame.  The frame allows the futon to rest "
        "comfortably a few inches above the ground, its hard mattress "
        "a must for those craving a little punishment combined with "
        "a good night's sleep.  It looks comfortable and is the perfect "
        "thing to have if you are into the whole minimalistic trend "
        "for your home.\n");
    set_allowed_positions(({"sitting", "lying", "kneeling", "meditating"}));
    set_allowed_room_verbs((["sitting" : "sits" ]));
    set_weight(150);
    set_value(650000);
    add_extra_look( this_object() );

} /* setup() */

void init() {

    add_command("unfold", "<indirect:object'futon'>" );
    add_command("fold", "<indirect:object'futon'>" );
  
    ::init();
  
} /* init() */

int do_unfold(object ob) {
   
    if(folded ) {
        add_failed_mess("$D is already folded.\n", ({ }) );
        return 0;
    }

    add_succeeded_mess("$N $V $D back into a sofa.\n", ({ }) );
    folded = 1;
    return 1;
    
} /* do_unfold() */

int do_fold(object ob) {
	
    if(!folded) {
        add_failed_mess("$D is already folded.\n", ({ }) );
	    return 0;
    }
    
    add_succeeded_mess("$N $V $D out into a bed.\n", ({ }) );
    folded = 0; 
    return 1;
    
} /* do_fold */

string extra_look() {

    if(!folded) {
        return "The futon has been folded out into a bed.\n";
    }
    return "The futon is folded into a sofa.\n";
    
} /* extra_look() */
