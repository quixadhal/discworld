/**
 * This is a fireplace inheritable for the playerhouses.
 * There are lots of functions for you to customise it. They all
 * have default settings so you don't have to override all of them
 * but try to make each fireplace look and feel different.
 * @author Aquilo
 */

#include <fuel_handler.h>
#include <move_failures.h>

#define WIZARD "/std/guilds/wizard"
#define CMD "/cmds/"
#define BURN_PER_TICK   1
#define BRIGHT 30
#define K      1  // A constant to relate weight and fuel

inherit "/std/room/furniture/surface";

private nosave int _light_produced;
private nosave int _is_lit;
private nosave int _frequency;
private nosave string _lit_mess;
private nosave string _unlit_mess;
private nosave string *_lit_chats;
private nosave string *_dying_chats;
private nosave string _turn_on_mess;
private nosave string _turn_off_mess;
private nosave string _out_of_fuel_mess;

void set_light_produced(int b);

void set_lit_mess(string str);
void set_unlit_mess(string str);

void set_frequency(int i);

void set_lit_chats(string *str);

void add_lit_chat(string str);
string request_lit_chat();

void set_dying_chats(string *str);
void add_dying_chat(string str);
string request_dying_chat();

void set_turn_on_mess(string mess);
void set_turn_off_mess(string mess);

void set_out_of_fuel_mess(string mess);

  // Internal functions
int do_light(object *obs);
int do_zap();
int turn_on();
int do_dowse(object *obs);
int turn_off();
int calc_fuel();

/** @ignore yes */
void create() {
  do_setup++;
  surface::create();
  do_setup--;

  set_name("fireplace");
  add_alias( ({ "fireplace", "furniture", "hearth", "fire" }) );
  add_plural( "fireplaces" );
  add_property( "shop type", "furniture" );
  add_property( "degrees", 0 );

  set_light_produced( BRIGHT );

  set_lit_mess("A fire blazes happily in the hearth, providing "
      "a soft red glow and pleasant warmth. It crackles and spits "
      "occasionally, giving the room that homely feel.\n");

  set_unlit_mess("The hearth is blackened with ashes and old embers.\n");

  set_frequency(5);

  set_lit_chats( ({
      "The fire crackles and spits cheerfully.",
      "The warm fire soothes your weariness.",
      "An enchanting orange glow radiates from the hearth.",
      "Small glowing embers dance over the fire.",
      "The fire blazes in the grate." }) );

  set_dying_chats( ({
      "The fire splutters.",
      "The warm red glow flickers.",
      "A whisp of smoke from the fire trails around the room." }) );

  set_turn_on_mess("Soon a roaring fire is blazing in the grate.\n");

  set_turn_off_mess("The embers smoke slightly and their glow dims.\n");

  set_out_of_fuel_mess("The fire flickers and dies away.\n");

  if ( !do_setup ) {
      this_object()->setup();
  }

  this_object()->add_extra_look( this_object() );

  add_help_file("fireplace");

}

/** @ignore yes */
void init() {

  this_player()->add_command("light", this_object(), "<direct:object> with <indirect:object>");
  this_player()->add_command("extinguish", this_object(), "<direct:object>", (: do_dowse :) );
  this_player()->add_command("dowse", this_object(), "<direct:object>");

  if( this_player()->query_guild_ob() == WIZARD ){
    this_player()->add_command("zap", this_object(), "<direct:object>");
    this_player()->add_command("point", this_object(), "[finger] at <direct:object>", (: do_zap :) );
  }

}

/**
 * Tell us the amount of the fuel the fireplace has left to burn.
 * @return the amount of fuel the fireplace has left to burn
 */
int query_fuel_left(){  return calc_fuel();  }

/**
 * This sets the amount of light produced by the fireplace (when it is on).
 * @param b the amount of light produced by the fireplace
 */
void set_light_produced(int b){  _light_produced = b;  }

/**
 * The amount of light produced by the fireplace (when it is on).
 * @return the amount of light produced by the fireplace
 */
int query_light_produced(){  return _light_produced;  }

/**
 * This function sets the frequency at which roomchats from the fireplace
 * are printed to the room. ( i * 15 ) is the average time in seconds
 * inbetween each roomchat.
 * @param i the frequency ( 0 no roomchats are printed )
 */
void set_frequency(int i){  _frequency = i + 1;  } /* +1 because
                                                    random(1) still = 0
                                                   */

/**
 * The current frequency (*15 seconds) at which roomchats are
 * printed from the fireplace.
 * @return the current frequency of the room chats
 */
int query_frequency(){  return _frequency;  }

/** @ignore yes */
int do_light(object *obs){
  string q_n;
  q_n = obs[0]->query_name();

  if( _is_lit ){
    this_player()->add_failed_mess( this_object(),
         "$D is already ablaze.\n", ({ }) );
    return 0;
  }

  if( q_n != "flint" && q_n != "match" && q_n != "lighter" ){
    this_player()->add_failed_mess(this_object(),
         "You can't light $D with $I.\n", ({ obs[0] }) );
    return 0;
  }

  //Bring in the fact that matches and lighters are always fiddly :)
  if( this_player()->query_dex() < random(18) ){
    this_player()->add_failed_mess(this_object(),
      "You strike $I, but can't quite get the spark to catch.\n", ({ obs[0] }) );
    return 0;
  }

  if ( calc_fuel() <= 0 ) {
    this_player()->add_failed_mess( this_object(),
        "There is nothing in the fireplace to burn.\n", ({ }) );
    return 0;
  }

  this_player()->add_succeeded_mess(this_object(),
    "$N $V $D with $I.\n", ({ obs[0] }) );

  return turn_on();

}

/** @ignore yes */
int do_zap(){
  object tp;
  tp = this_player();

  if(tp->query_gp() < 10){
    tell_object(tp, "You don't even have the energy for such a small task!\n");
    return 1;
  }

  if(_is_lit){
    tell_object(tp, "There is already a merry fire burning in the fireplace.\n");
    return 1;
  } else {

    tp->add_succeeded_mess(this_object(),
      "$N extend$s an arm towards $D.\nA small fireball leaps from $p "
      "index finger and throws itself onto the hearth.\n", ({ }) );
    tp->adjust_gp( -10 );
    return turn_on();
  }

}

/**
 * This function sets the message that is printed to the room containing
 * the fireplace, a few seconds after it has been turned on.
 * @param mess the message. Use "" for no message.
 */
void set_turn_on_mess(string mess){  _turn_on_mess = mess;  }

/**
 * This function returns the message that is printed to the room containing
 * the fireplace after is has been turned on.
 * @return the message.
 */
string query_turn_on_mess(){  return _turn_on_mess;  }

/**
 * This function sets the message that is printed to the room containing
 * the fireplace, a few seconds after it has been turned off.
 * @param mess the message. Use "" for no message.
 *
 *
 */
void set_turn_off_mess(string mess){  _turn_off_mess = mess;  }

/**
 * This function returns the message that is printed to the room
 * containing the fireplace a few seconds after is has been turned off.
 * @return the message.
 */
string query_turn_off_mess(){  return _turn_off_mess;  }

/** @ignore yes */
void msg_on(){
  if(_is_lit){
    tell_room(environment( this_object() ), query_turn_on_mess() );
  }
}

/** @ignore yes */
int turn_on(){
  int x;
  FUEL_HANDLER->add_burner( this_object() );
  _is_lit = 1;
  adjust_light( query_light_produced() );

    x = query_light_produced() / 4;
    add_property("degrees", query_light_produced() * 4);
    environment( this_object() )->add_property("warmth", x );

  call_out( "msg_on", 7 );
  return 1;
}

/** @ignore yes */
int do_dowse(object *obs){

  if( !_is_lit ){
    this_player()->add_failed_mess( this_object(),
         "There is no fire to put out.\n", ({ }) );
    return 0;
  } else {
    this_player()->add_succeeded_mess(this_object(),
      "$N $V $D and the room loses its warm glow.\n", ({ }) );
    return turn_off();
  }

}

/** @ignore yes */
void msg_off() {
  if(!_is_lit){
    tell_room(environment( this_object() ), query_turn_off_mess() );
  }
}

/** @ignore yes */
int turn_off(){

  FUEL_HANDLER->remove_burner( this_object() );
  _is_lit = 0;
  adjust_light( -query_light_produced() );
  add_property( "degrees", 0 );
  environment( this_object() )->remove_property("warmth");

  call_out( "msg_off", 10 );
  return 1;

}

/**
 * This function sets the message that is printed to the room containing
 * the fireplace, when the fuel runs out.
 * @param mess the message.
 */
void set_out_of_fuel_mess(string mess){  _out_of_fuel_mess = mess;  }

/**
 * This function returns the message that is printed to the room
 * containing the fireplace when it runs out of fuel.
 * @return the message.
 */
string query_out_of_fuel_mess(){  return _out_of_fuel_mess;  }

/** @ignore yes */
void out_of_fuel() {

  turn_off();
  tell_room(environment( this_object() ), query_out_of_fuel_mess() );
}


/**
 * This function sets the array of room chats that will be printed
 * when the fireplace is lit. One chat is chosen randomly every
 * 15 * query_frequency() seconds.
 * @param str an array containing the desired chats
 * @see query_frequency()
 * @see set_frequency()
 * @see set_dying_chats()
 */
void set_lit_chats(string *str){  _lit_chats = str;  }

/**
 * This functions adds a string to the array of room chats that
 * will be printed
 * when the fireplace is lit. One chat is chosen randomly every
 * 15 * query_frequency() seconds.
 * @param str the string to be added to the array
 * @see query_frequency()
 * @see set_frequency()
 * @see set_dying_chats()
 * @see set_lit_chats()
 */
void add_lit_chat(string str){
  if( !_lit_chats ){
    _lit_chats = ({ });
  }
  _lit_chats += ({ str });
}

/** @ignore yes */
string request_lit_chat(){
  return _lit_chats[random( sizeof(_lit_chats) - 1 )];
}

/**
 * This functions sets the array of room chats that will be printed
 * when the fire is dying away. One chat is chosen randomly every
 * 15 * query_frequency() seconds.
 * @param str an array containing the desired chats
 * @see query_frequency()
 * @see set_frequency()
 * @see set_lit_chats()
 */
void set_dying_chats(string *str){  _dying_chats = str;  }

/**
 * This functions adds a string to the array of room chats that
 * will be printed
 * when the fire is dying away. One chat is chosen randomly every
 * 15 * query_frequency() seconds.
 * @param str the string to be added to the array
 * @see query_frequency()
 * @see set_frequency()
 * @see set_dying_chats()
 * @see set_lit_chats()
 */
void add_dying_chat(string str){
  if( !_dying_chats ){
    _dying_chats = ({ });
  }
  _dying_chats += ({ str });
}

/** @ignore yes */
string request_dying_chat(){
  return _dying_chats[random( sizeof(_dying_chats) - 1 )];
}

/**
 * This function sets the message that will be appended to the fireplace's
 * long when it is lit
 * @param str the message to be displayed
 */
void set_lit_mess(string str){  _lit_mess = str;  }

/** @ignore yes */
string query_lit_mess(){  return _lit_mess;  }

/**
 * This function sets the message that will be appended to the fireplace's
 * long when it is not lit
 * @param str the message to be displayed
 */
void set_unlit_mess(string str){  _unlit_mess = str;  }

/** @ignore yes */
string query_unlit_mess(){  return _unlit_mess;  }


/* This function is called by the burner handler even few seconds.
   It sorts out the burning of fuel and printing of messages
 */
/** @ignore yes */
void consume_fuel() {
  int x;
  int q;
  int z;
  int w;
  int amount;
  string r;
  object *inv;
  object ashes;
  object thing;
  object env;

  inv = filter(all_inventory( this_object() ),
              (:$1->query_property("fuel") :));

  if( !inv || inv == ({ }) || calc_fuel() < 1){
    out_of_fuel();
    return;
  }

  x = sizeof(inv);

  foreach(thing in inv){
      q = thing->query_amount_left();
      w = thing->query_weight();

      z = q - ( ( BURN_PER_TICK * 100 / x ) / w );

      if ( z < 1 ){
          thing->move("/room/rubbish");
          amount = roll_MdN(4, 20);
          ashes = clone_object("/obj/cont_medium");
          ashes->set_name("ash");
          ashes->set_short("fine grey ash");
          ashes->add_property("determinate", "some ");
          ashes->add_adjective(({"some", "fine", "grey", "pinch", "pinches",
           "handful", "handfuls", "bucketful", "bucketfuls", "of"}));
          ashes->add_plural("ash");
          ashes->set_long("This is $pile_size$ of fine grey ash, "
            "$amount_size$. It looks like it was the leftovers "
            "from someone's camp fire.\n");
          ashes->set_weight_unit(({ 1, 50 }));
          ashes->set_medium_alias("FiNeGrEyAsH");
          ashes->set_amount_types((["pinch" : ({1, "pinches"}),
           "handful" : ({50, "handfuls"}),
           "bucketful" : ({1000, "bucketfuls"})]));
          ashes->set_pile_types(({50, "small", 100, "medium",
            200, "large", "huge"}));
          ashes->set_continuous();
          ashes->set_amount(amount);
          ashes->move(this_object());
      } else {
        thing->set_amount_left( z );
      }

  }

  // Re calculate fuel left
  calc_fuel();

  // Now sort out the printing of messages
  env = environment( this_object() );

  /* Adding and removing room chats was a hassle, and things didn't go
   * smoothly. Using tell_room is much easier
   */
  if( random( query_frequency() ) == 1 ){
    if( calc_fuel() > 10 ){

      r = request_lit_chat();
      tell_room( env, r + "\n");

    } else {

      r = request_dying_chat();
      tell_room( env, r + "\n");

    }
  }

}

/** @ignore yes */
int calc_fuel(){
  object *inv;
  object thing;
  int tot_fuel = 0;
  int w;
  int amt;

  inv = all_inventory( this_object() );

  if( !inv || inv == ({ }) ) { return 0; }

  foreach(thing in inv){
    if( thing->query_property("fuel") ){
      w =  thing->query_weight();
      amt = thing->query_amount_left();
      tot_fuel += w * amt * K / 100 ;
    }
  }

  return tot_fuel;

}

/** @ignore yes */
int test_remove( object thing, int flat, mixed dest){
  object tp;
  object *prev;
  tp = this_player();
  prev = previous_object(-1);

  /* Below: make sure this func is being called by a person trying to get
   * items from here, not by a move call made in earlier functions.
   */
  if(_is_lit && tp && strsrch( file_name(prev[1]), CMD ) != -1 ){
    tell_object(tp, "You reach out towards the fire but "
       "you can't brave the flames and quickly pull away.\n");

    tell_room( environment(tp), tp->query_short() + " tries to take something "
      "from the fire but pulls back sucking " + tp->query_possessive() +
      " fingers.\n", tp);
    tp->adjust_hp( -random(120) );
    return 0;
  }
  return ::test_remove(thing, flat, dest);
}

/** @ignore yes */
int test_add( object thing, int flag ) {

  if( !thing->query_property("fuel") && environment(thing) ){
    tell_object(this_player(), "You don't think " + thing->the_short() +
      " would be a suitable fuel.\n");
    return 0;
  }

  if( _is_lit ){
    tell_room(environment( this_object() ),
      ({ "The fire crackles.\n",
         "A shower of glowing embers tumble around the hearth.\n" })[random(2)] );
  }


  call_out("calc_fuel",1);
  return 1;
}

/** @ignore yes */
string extra_look( object ob ){

  if( _is_lit ) {
      return query_lit_mess();
  }
  else {
      return query_unlit_mess();
  }

}

void pre_dest_me() {
  int tmp;

  if(_is_lit && environment()) {
    FUEL_HANDLER->remove_burner( this_object() );
    _is_lit = 0;
    adjust_light( -query_light_produced() );
    tmp = environment()->query_property("warmth");
    if(tmp > query_light_produced() / 4)
      environment()->add_property("warmth", tmp -
                            (query_light_produced() / 4));
    else
      environment()->remove_property("warmth");
  }

  return;
}

/** @ignore yes */
void dest_me() {
  FUEL_HANDLER->remove_burner( this_object() );
  _is_lit = 0;
   ::dest_me();
}

