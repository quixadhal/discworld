/* Edded to put inside rather than outside. Deutha 23/8/93
 * Changed to make it more safe, also removes ran_talk. Olorin 7/4/96
 * Patch added to stop the few killings that seem to get through. Terano 10/3/98
 * "Attack" command wasn't blocked. Weird. Fixed it. Red Scharron will be
 *   unable to use it here. *sigh* Also, I added some "violent" souls to the
 *   list. - Fiona 7/6/98
 * Added restriction against "iai" command.  Dragonkin 98-07-18
 * Rewritten - 21 July, 1998. Terano
 * Added restriction against "hurl" and "guard", fixed bug with magic smoke.
 *    Aragorn 98-09-06
 * Added restriction against "plant" - Aragorn 98-09-30
 * Added restriction against "invoke" - Aragorn 98-11-07
 * Protection from range attacks (longshot) and KLK insects added.
 *    Aragorn 98-11-08
 * Made Brother David unique, hopefully this will protect him from fleas.
 *    Aragorn 98-11-08
 * Added restriction against "twist" and "hedgehog" - Aragorn 99-01-18
 * Added protection from longshot that actually works  -  Gerbil 13-3-1999
 * Added restriction against "berserk" - Saffra June 3, 2000
 * Added restrictions against "perform", which is used in some rituals - Twiggy,
 *    Dec 24, 2000
 * Added restriction against "backstab" - Saffra April 16, 2001
 *
 * Made it into an inheritable, filed into /std/room/ for common use throughout 
 * the Disc on April 2002 - Siel
 */


#include <armoury.h>

#define TP this_player()
#define SHAD "/std/shadows/misc/davidshad"

inherit "/std/room/basic_room";

object keeper;

void create() {
    do_setup++;
    ::create();
    do_setup--;
    set_light(90);
    
    /* Dry them up a bit */
    add_property("warmth", 3);
    add_property( "los", "closed" );
    
    /* Special property for things to check when attempting to complete. */
    add_property("calm room", 1);
  
    if(!do_setup) {
      this_object()->setup();
      this_object()->reset();
  }
} /* create() */

int trap_action( string str ) {
    string st1;

    if( sscanf( str,"%s %s",str,st1 ) );
    switch(str) {
    /* these are in alphabetical order for ease of reference */
    case "ambush":
    case "attack":
    case "backstab":
    case "behead":
    case "bury":
    case "cast":
    case "circle":
    case "conflagrate":
    case "crush":
    case "decompose":
    case "drag":
    case "eat":
    case "ensumpf":
    case "fade":
    case "filch":
    case "fire":
    case "fuel":
    case "guard":
    case "hedgehog":
    case "hide":
    case "hurl":
    case "iai":
    case "inhume":
    case "inpale":
    case "invoke":
    case "kill":
    case "mock":
    case "order":
    case "tempt":
    case "palm":
    case "perform":
    case "plant":
    case "pyroscipate":
    case "scathe":
    case "scribe":
    case "scry":
    case "shroud":
    case "slip":
    case "snatch":
    case "sneak":
    case "steal":
    case "recharge":
    case "rifle":
    case "throw":
    case "twist":
    case "use":
    case "ventisepelate":
    case "zap":

    /* Here go some violent souls */
    case "skick":
    case "sburn":
    case "slap":
    case "spunch":
    case "berserk":
        tell_object( this_player(), "You feel too peaceful to do that.\n");
        return 1;

    default:
        return 0;
    }
} /* trap_action */

void event_exit(object ob, string message, object to);

void init() {

    this_player()->remove_hide_invis( "hiding" );
    clone_object( SHAD )->setup_shadow(TP);
    this_player()->command_override( (: trap_action :) );
    this_player()->add_command( "dream", this_object(), "" );
    this_player()->add_command( "rest", this_object(), "" );

    ::init();
} /* init() */

/**
 * This is the object that is the keeper of the safe room. This should "
 * be an NPC.
 * @param keeper The keeper NPC object.
 */
void set_keeper(object ob) { keeper = ob; }

void event_exit(object ob, string message, object to){
    ob->dest_shadow();
}

void event_enter( object ob, string stringy, string stringy2 ) {
    object thing;
    if ( base_name( ob ) == "/d/guilds/priests/summoned/dust_devil" ) {
        ob->do_leave();
    }

    if ( base_name( ob ) == "/d/am/chars/herrena" ) {
            int *enums = ob->effects_matching( "fighting.combat" );
            foreach( int i in enums ) ob->delete_effect( i );
    }

    foreach( thing in all_inventory(ob)) {
        if( base_name( thing ) == "/d/sur/items/crystal_ball" )
            thing->do_rub();
    }

    if ( living( ob ) )
        call_out("make_calm", 1, ob );
        call_out( "stop_sneaking", 1, ob );

} /* event_enter */

/* Rest function used by players in this room.  aids healing but costs GP */
int do_rest( string str ) {
    this_player()->adjust_hp( 4 );
    this_player()->adjust_gp( -( 2 + random( 4 ) ) );
    /* well nothing's free is it ? */
    this_player()->adjust_social_points( -( 2 + random(8)));
    tell_object( this_player() ,
      "You rest on one of the pillows, and feel much "
      "better for it.\n");
    return 1;
} /* do_rest */

/* Dream function used by players. Aids healing but costs GP */

int do_dream( string str ) {
    this_player()->adjust_hp( 4 );
    this_player()->adjust_gp( -( 2 + random( 4 ) ) );
    /* well nothing's free is it ? */
    this_player()->adjust_social_points( -( 2 + random(8)));
    tell_object( this_player(), ({
      "You dream of a world where cabbages roam free.\n",
      "You have a vision of penguin waiters serving giraffes in white leisure "
         "suits.\n",
      "You dream of imp-powered sheep leaping over fences.\n",
      "You dream uneasily of creators with cuddly teddy-bears.\n",
      "Visions of sugar-plums dance through your head.\n",
      "You count lemons as you try to get to sleep.\n" }) [ random(4) ] );
    return 1;
} /* do_dream */

/* Stops all fighting here ....  */

void event_fight_in_progress( object ob1, object ob2 ) {
    if ( keeper ) { tell_room ( this_object(),
    keeper->the_short() + " hums a peaceful tune softly and calm fils the "
      "room." ); }
    ob2->stop_all_fight();
    ob1->stop_all_fight();
} /* event_fight_in_progress */

void make_calm( object blue ) {

    int *enums;
    int wibble;

    enums = blue->effects_matching("fighting.combat");

    if ( sizeof( enums ) ) {
        foreach( wibble in enums ) { blue->delete_effect( wibble ); }
        keeper->do_command("smile "+ blue->query_name() );
        tell_object( blue, "You feel a lot calmer.\n");
    }

} /* end do_calm */

void stop_sneaking( object sneaker ) {

    int *enums;
    int wibble;

    enums = sneaker->effects_matching( "covert.sneaking" );

    if ( sizeof( enums ) ) {
        foreach( wibble in enums ) { sneaker->delete_effect( wibble ); }
        keeper->do_command("hug "+ sneaker->query_name() );
        tell_object( sneaker, "You feel less sneaky.\n");
    }

} /* end stop_sneaking */
