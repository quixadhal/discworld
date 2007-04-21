/**
 * Peace NPC Inherit
 * NPCs inheriting this file will cause their environment to be
 * peaceful
 *
 * Action-trapping code was modified and improved (IMNSHO) from
 * the Brother David's code
 *
 * @author Dasquian
 * @started on 30 December 2000
 *
 */

inherit "/std/effect_shadow";

#define GCR peacenpc->get_combat_reason()
#define GER peacenpc->get_escape_reason()
#define GFR peacenpc->get_fighting_reason()
#define GMR peacenpc->get_magic_reason()
#define GOR peacenpc->get_other_reason()
#define GRR peacenpc->get_ritual_reason()
#define GSR peacenpc->get_stealth_reason()
#define TO this_object()
#define TP this_player()

object peacenpc;

void setup_shadow(object person, object thing) {
    shadow( person, 1 );
    peacenpc=thing;
} /* setup_shadow() */

void dest_peace_shadow() { destruct(TO); } /* dest_peace_shadow() */

void event_exit(object ob, string message, object to) {
    if (ob==peacenpc)
        dest_peace_shadow();
} /* event_exit() */

void event_move_object(mixed from, mixed to) {
    dest_peace_shadow();
} /* event_move_object() */

// This function should just stop players doing anything that could
// possibly harm each other.
int command_shadowed( string verb, string args ) {

    switch(verb) {
        case "attack":
        case "behead":
        case "berserk":
        case "bob":
        case "crush":     
        case "disarm": 
        case "fire":
        case "guard": 
        case "hack":    
        case "hurl":
        case "iai":
        case "impale":
        case "kill":
        case "punch":
        case "riposte":
        case "slash":
        case "throw":
        case "warcry":
            tell_object(TP, GFR);
            return 1;
        case "cast":
        case "circle":
        case "educe":
        case "forget":
        case "invoke":
        case "remember":
        case "scribe":
        case "scry":
        case "spellcheck":
        case "twist":
        case "use":
        case "zap":
            tell_object(TP, GMR);
            return 1;
        case "bodyguard":
        case "bury":
        case "drag":
        case "eye":
        case "fuel":
        case "hedgehog":
        case "judge":
        case "leatherwork":
        case "mock":
        case "tempt":
        case "scathe":
        case "vurdere":        
            tell_object(TP, GOR);
            return 1; 
        case "conflagrate":
        case "consecrate":
        case "decompose":
        case "ensumpf":
        case "envalise":
        case "imbue":
        case "perform":
        case "pray":
        case "pyroscipate":
        case "shroud":
        case "suffuse":     
        case "ventisepelate":     
            tell_object(TP, GRR);
            return 1; 
        case "abscond":
        case "ambush":
        case "case":
        case "conceal":
        case "crack":
        case "disable":
        case "filch":
        case "hide":
        case "inhume":
        case "lpick":
        case "palm":
        case "peek":
        case "plant":
        case "probe":
        case "scope":
        case "shoplift":
        case "slip":
        case "snatch":
        case "sneak":
        case "steal":
        case "unhide":
            tell_object(TP, GSR);
            return 1;
        default:
            return 0;
    }
} /* command_shadowed() */

// Too peaceful to run away...
int run_away() {
    tell_object(TO, GER);
}