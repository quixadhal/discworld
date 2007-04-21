inherit "/obj/monster";
#include <panic.h>
#include <armoury.h>

void finish_up();

int state, start_time, prodded;
object lost;
string safeplace;

void setup() {
  start_time = time();
  set_name("garlick");
  set_short("fairy godmother");
  add_alias("godmother");
  add_adjective("fairy");
  set_long( "In the realm of Fairy Godmothers, this one must certainly "
    "take honours in a Division Yet To Be Announced.  Grimly determined "
    "to do things properly, she has woven wild flowers in her "
    "windswept, golden locks, powdered her face to a fashionable pallor "
    "and has attired herself in numerous fluttering shawls, bangles, "
    "and a vivid green gown.  The overall effect is that of a maypole "
    "with a flower box emptied over its head that happens to jingle like "
    "a tinker's wagon with a broken wheel.  With one hand she clutches her "
    "regulation wand and with the other she dabs at her red and dripping "
    "nose with a damp scrap of lace.\n");

  set_race("human");
  set_guild("witch");
  set_level(200);
  set_gender(2);
  set_language("general");
  set_response_mon_understand_anything(1);
  add_property( "anchor", 1);

  ARMOURY->request_item("startling green dress", 100)->move( this_object() );
  ARMOURY->request_item("spiderweb shawl", 100)->move( this_object() );
  ARMOURY->request_item("pumps", 50)->move( this_object() );
  init_equip();

  add_respond_to_with(({ "yes" }), "#yes_response");
  add_respond_to_with(({ "@nod" }), "#yes_response");
  add_respond_to_with(({ "no" }), "#no_response");
  add_respond_to_with(({ "@shake" }), "#no_response");
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
 
  queue_command( "say Umm.  Hi.  You are lost?  Would you like me to get "
             "you back to " + safeplace + "?" );
  queue_command( "emote waves her wand around a bit." );
  tell_room(environment(),
            "A puff of smoke drifts away from where something else used to "
            "be.  Now residing there is a pumpkin.\n");

  ARMOURY->request_item("pumpkin", 100)->move(environment());
  queue_command( "say Whoops." );
  lost = pl;
  start_time = time();
  prodded = 0;
  call_out("do_prod", 30);

} /* do_panic() */


int query_waiting() { return time()-start_time; }


void yes_response(object ob) {
  if (ob != lost)
    return ;
  queue_command( "say Oh, ahhh.  Good." );
  queue_command( "peer wor" );
  tell_object( lost, the_short() + " waves her wand in your direction.\n");
  tell_room( environment(), the_short() + " waves her wand in " +
    lost->the_short() + "'s direction.\n", lost );
  PANIC_HANDLER->finish_panic(lost, PANIC_OK);
  /* A couple of pumpkins for good luck. */
  ARMOURY->request_item("pumpkin", 100)->move(lost);
  ARMOURY->request_item("pumpkin", 100)->move(environment(lost));
  lost = 0;
} /* yes_response() */


void no_response(object ob) {
  if(ob != lost)
    return;
  queue_command( "say Oh, I must have got lost somewhere then." );
  queue_command( "wave" );
  queue_command( "say Oh dear." );
  ARMOURY->request_item("pumpkin", 100)->move(lost);
  ARMOURY->request_item("pumpkin", 100)->move(environment(lost));
  PANIC_HANDLER->finish_panic(lost, PANIC_NOT_OK);
  lost = 0;
} /* no_response() */


void do_prod() {
  if(!lost)
    return;
  if (prodded < 3 ) {
    queue_command( "say Umm....so....what do you think?  Do you want "
      "my help?" );
    queue_command( "expect "+lost->query_name() );
    prodded ++ ;
    call_out("do_prod", 60);
  } else {
    queue_command( "say Oh well.  I guess it wasn't me you wanted after all" );
    queue_command( "sigh" );
    PANIC_HANDLER->finish_panic(lost, PANIC_NOT_OK);
    lost = 0;
  }
} /* do_prod() */


void event_exit(object ob, string message, object to) {
    if (ob == lost) {
    queue_command( "say Bye..." );
    queue_command( "wave" );
    queue_command( "say Whoops." );
    queue_command( "tell "+lost->query_name()+" Sorry to have bothered you." );
    PANIC_HANDLER->finish_panic(lost, PANIC_NOT_OK);
    ARMOURY->request_item("pumpkin", 100)->move(environment(lost));
    lost = 0;
  }
} /* event_exit() */


void hurry_up() {
  if (!lost) {
    /* If the player has buggered off, we give up */
    queue_command( "blink twice" );
    queue_command( "say I guess my help wasn't wanted, then..." );
    PANIC_HANDLER->finish_panic(lost, PANIC_NOT_OK);
    lost = 0;
  }
  else do_prod();
} /* hurry_up() */


void event_fight_in_progress(object attacker, object attackee) {
  if (attackee == this_object()) {
    queue_command( "say Look, this is really not very nice." );
    queue_command( "frown" );
    queue_command( "say I know the art of self defence you know!" );
    set_hp(query_max_hp(), attacker);
    attackee->adjust_tmp_str(-2);
    attackee->adjust_tmp_dex(-2);
    attackee->adjust_tmp_wis(-2);
    attackee->adjust_tmp_int(-2);
    attackee->adjust_tmp_con(-2);
    attackee->add_property("attacked godmother", time());
    PANIC_HANDLER->finish_panic(lost, PANIC_NOT_OK);
    lost = 0;
  }
} /* event_fight_in_progress() */


void finish_up() {
  queue_command( "shuffle" );
  queue_command( "say Umm.  I have to go." );
  queue_command( "smile appol" );
  queue_command( "wave" );
  PANIC_HANDLER->finish_panic(lost, PANIC_NOT_OK);
  lost = 0;
} /* finish_up() */


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
