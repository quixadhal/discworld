inherit "/obj/monster";
#include <panic.h>
#include <armoury.h>

void finish_up();

int state, start_time, prodded;
object lost;
string safeplace;

void setup() {
  start_time = time();
  set_name( "godmother" );
  set_short( "fairy godmother" );
  add_adjective( "fairy" );
  set_long( "This fairy godmother isn't quite what you expected.  She is "
    "tall and forbidding; not in the slightest bit cuddly or smiley.  She "
    "looks slightly uncomfortable and is currently glaring at her wand, "
    "which appears to be malfunctioning.  If you cross her, you might find "
    "yourself turned into a wombat or worse.\n" );

  set_race( "human" );
  set_guild( "witch" );
  set_level( 300 );
  set_gender( 2 );
  set_language("general");
  set_response_mon_understand_anything(1);
  add_property( "anchor", 1);

  ARMOURY->request_item("hobnailed boots", 100)->move( this_object() );
  ARMOURY->request_item("antique black dress", 100)->move( this_object() );
  ARMOURY->request_item("witchs pointy hat", 100)->move( this_object() );
  ARMOURY->request_item("black witches cloak", 100)->move( this_object() );
  init_equip();

  add_respond_to_with(({ "yes" }), "#yes_response");
  add_respond_to_with(({ "@nod" }), "#yes_response");
  add_respond_to_with(({ "no" }), "#no_response");
  add_respond_to_with(({ "@shake" }), "#no_response");
  add_respond_to_with(({ "@think", "somewhere", "safe" }), "#think_safe");
  add_respond_to_with(({ "@think", "safe" }), "#think_safe");
  add_respond_to_with(({ "@think", "safety" }), "#think_safe");
  add_respond_to_with(({ "@womble" }), "#think_mended_drum");
} /* setup() */


void do_panic(object pl) {

  string home;
  string nationality;

  nationality = pl->query_nationality();
   
  if ( !nationality ) { 
      queue_command( "say Oops.. you haven't got a nationality, I can't "
        "send you home!");
      finish_up();
      return;
  }

  home = nationality->query_default_start_location();
  safeplace = home->query_short();
 
  queue_command( "say I hope you're not foreign.  You know, where "
    "you gabble at me in heathen lingo and eat foreign muck and "
    "worship objects." );
  queue_command( "say So.  You want some help to get back to the "
    + safeplace + "?" );
  queue_command( "glare "+pl->query_name() );
  lost = pl;
  start_time = time();
} /* do_panic() */


int query_waiting() { return time()-start_time; }


void yes_response(object ob) {
  if (ob != lost)
    return ;
  queue_command( "say Right then.  Close your eyes and concentrate." );
  queue_command( "say Think of somewhere safe." );
  prodded = 0;
  call_out("check_think", 30);
} /* yes_response() */


void no_response(object ob) {
  if (ob != lost)
    return ;
  queue_command( "say So why did you call me up then?", 1 );
  queue_command( "roll eyes "+lost->query_name() );
  queue_command( "pinch "+lost->query_name()+" cheek" );
  PANIC_HANDLER->finish_panic(lost, PANIC_NOT_OK);
  lost = 0;
} /* no_response() */


void think_safe(object ob) {
  if (ob != lost)
    return ; 
  
  queue_command( "say Now the world will just rearrange itself "
    "to suit you." );
  PANIC_HANDLER->finish_panic(lost, PANIC_OK);
  lost = 0;
} /* think_safe() */


void check_think() {
  if(!lost)
    return;
  if (prodded < 3 ) {
    queue_command( "say Come on, I haven't got all day.  Just think of "
      "safety." );
    queue_command( "tap" );
    prodded ++ ;
    call_out("check_think", 60);
  } else {
    queue_command( "say Young people of today..." );
    queue_command( "mutter no manners" );
    PANIC_HANDLER->finish_panic(lost, PANIC_NOT_OK);
    lost = 0;
  }
} /* check_think() */


void event_exit(object ob, string message, object to) {
  if (ob == lost) {
    queue_command("hrmph");
    queue_command("tell "+lost->query_name()+" Thats right!  Just walk "
      "out on me.");
    queue_command("glare "+lost->query_name() );
    PANIC_HANDLER->finish_panic(lost, PANIC_NOT_OK);
    lost = 0;
  }
} /* event_exit() */


void event_fight_in_progress(object attacker, object attackee) {
  if (attackee == this_object()) {
/* Someone is attacking us! */
    attacker->adjust_tmp_str(-4);
    attacker->adjust_tmp_con(-4);
    attacker->adjust_tmp_dex(-4);
    attacker->adjust_tmp_int(-4);
    attacker->adjust_tmp_wis(-4);
    queue_command( "say What is this travesty!  Attacking your fairy "
               "godmother, Well I never." );
    queue_command( "say "+attacker->one_short()+" I here by curse you!" );
    queue_command( "say Don't expect me to help you again." );
    queue_command( "glare "+attacker->query_name() );
    attacker->add_property("attacked godmother", time());
    PANIC_HANDLER->finish_panic(lost, PANIC_NOT_OK);
    lost = 0;
  }
} /* event_fight_in_progress() */


void hurry_up() {
  if (!lost) {
    /* If the player has buggered off, we give up */
    queue_command( "blink" );
    queue_command( "say Where'd that bugger go?  Oh well.");
    PANIC_HANDLER->finish_panic(lost, PANIC_NOT_OK);
    lost = 0;
  }
  else  check_think();
} /* hurry_up() */


void finish_up() {
  if (lost) {
    queue_command( "say Well.  You took too long.  What a complete "
      "waste of time." );
    queue_command( "glare " + lost->query_name() );
    lost = 0;
  }
  else  {
    queue_command( "peer" );
    queue_command( "say Damn, they buggered off somewhere.  I hate "
      "it when they do that." );
    queue_command( "scowl" );
  }
  PANIC_HANDLER->finish_panic(lost, PANIC_NOT_OK);
} /* finish_up() */


void event_dest(object ob) {
  if (ob == lost) {
    PANIC_HANDLER->finish_panic(lost, PANIC_NOT_OK);
    lost = 0;
  }
} /* event_dest() */


void dest_me() {
  if (lost) {
    PANIC_HANDLER->finish_panic(lost, PANIC_NOT_OK);
    lost = 0;
  }
  ::dest_me();
} /* dest_me() */


int unambushable() {
  return 1;
}
