/* 
 *  Inherit for hot food.
 *  Sightblinder, February 2002.
 * 
 * $Id: hotfood.c,v 1.3 2003/03/25 15:32:15 sightblinder Exp sightblinder $
 * $Log: hotfood.c,v $
 * Revision 1.3  2003/03/25 15:32:15  sightblinder
 * added $Log$
 *
*/

/**
 *
 * <BR>
 * This is an inherit for hot food which cools down, 
 * based on the temperature of the room in which it 
 * finds itself and any containers it might be in. 
 * <P>
 * It performs just as normal food, apart from the following:
 * <P>
 * - If eaten while still hot it will cause the consumer to warm up for 
 *   a period of time.
 * <P>
 * - It only decays once it has finished cooling.
 * <P>
 * It's cooling level can be checked by query_cool_level().  This
 * starts at a value of twenty times the original weight of the 
 * hot food and determines how long it takes to cool down.  Larger items
 * cool more slowly.
 * <P>
 * The level of cooling is shown in the long and short of the object.
 * These are created using query_short_rotten_adjective and 
 * query_long_decay_level, and are based on the percentage by which it has 
 * cooled.
 * <P>
 * 
 * Differant messages can be displayed when the food is eaten hot or cold
 * These can be set using set_hot_eat_message() and set_cold_eat_message()
 * Messages can also be set for eating bites of food where appropriate
 * using setup_cold_eat_piece_messages() and setup_hot_eat_piece_messages()
 * 
 * @author Sightblinder
 * 
 */

#include <bits_controller.h>
#include <weather.h>

inherit "/obj/food";

#define WARM 10        /* By how much eating this will warm you */
#define WARM_TIME 60   /* For how long eating this will warm you */
#define COOL_RATE_DEFAULT 10  /* The default cool rate, initially corresponding
                               * to 100 seconds for something of weight 1 to 
                               * cool away to nothing in a 20 deg. C 
                               * environment 
                               */                            
/* GLOBAL VARIABLES */
private int _cool_level;          /* this represents the amount left to cool.  
                                     A value of 1 = stone cold. */
private int _cool_rate;           /* This determines how quickly the item cools
                                    by controlling the frequency of the cooling 
                                    call.*/
private int _purchase_time;       /* The time the food was bought.  Used to
                                   * calculate how much it has cooled by. */
private mixed _hot_eat_message;   /* The messeage returned when the food is 
                                   * eaten while still hot. */
private mixed _cold_eat_message;  /* The messeage returned when the food is 
                                   * eaten while cold. */
private mixed _hot_eat_piece_message;  /* Messages returned by           */
private mixed _hot_eat_amt_message;    /* setup_eat_piece_messages when  */
private mixed _hot_eat_last_message;   /* dealing with hot or cold food. */
private mixed _cold_eat_piece_message; /*                                */
private mixed _cold_eat_amt_message;   /*                                */
private mixed _cold_eat_last_message;  /*                                */
 
/* FUNCTION PROTOTYPES */
void create();
void set_cool_rate( int rate );
void set_cool_level(int level);
int query_cool_rate();
int query_cool_level();
protected void do_cool();
void set_hot_eat_mess(mixed messages);
void set_cold_eat_mess(mixed messages);
mixed query_hot_eat_mess();
mixed query_cold_eat_mess();
void setup_cold_eat_piece_messages(mixed piece, mixed amt, mixed last);
void setup_hot_eat_piece_messages(mixed piece, mixed amt, mixed last);
mixed query_hot_eat_amt_message;
mixed query_hot_eat_piece_message;
mixed query_hot_last_message;
mixed query_cold_eat_amt_message;
mixed query_cold_eat_piece_message;
mixed query_cold_last_message;
/**
 * @ignore yes
 */
void create() {
   do_setup++;
   ::create();
   do_setup--;
   _cool_rate = COOL_RATE_DEFAULT;
   set_decay_speed( 1 );
   if ( !do_setup ) this_object()->setup();
   _cool_level = ( this_object()->query_weight() ) * 20;
   _purchase_time = time();
}/* create() */

/**
 * This function sets the current cooling rate for the food.  This will
 * have no real affect on the cooling of the food, as the cool rate is 
 *recalculated regularly.
 *
 * @param rate The new rate of cooling
 *
 * @see query_cool_rate()
 *
 */
void set_cool_rate( int rate ) {
   _cool_rate = rate;
} /* set_cool_rate */

/**
 * This function returns the current rate of cooling of the hot food.
 *
 * @return The current cooling rate of the food.
 *
 * @see set_cool_rate()
 *
 */
int query_cool_rate() {
   return _cool_rate;
} /* query_cool_rate */

/**
 * This function returns the current cool level of the food.  This value
 * starts at twenty times the original weight of the object, and decreases
 * in time, as the food cools. 1 = stone cold.
 *
 * @return The current cool level of the food.
 */
int query_cool_level() {
   return _cool_level;
} /* query_cool_level */

/**
 * This function returns the adjective appended to the food's
 * short description, describing how cooled or decayed it is.
 *
 * @return The adjectives added to the food's short describing its
 * state of cooling or decay if already cold.
 * @see query_long_decay_level()
 */
string query_short_rotten_adjective() {
   string ret;
   if (this_object()->query_cool_level() == 7){
       return ("cold " + ::query_short_rotten_adjective());
   }
   _cool_level = (time() - _purchase_time)/_cool_rate;
   ret = "";
   switch ( _cool_level ) {
     case 0..1:
        ret="piping hot " + ret;
        break;
     case 2:
        ret = "slightly cooled " + ret;
        break;
     case 3:
        ret = "partially cooled " + ret;
        break;
     case 4:
        ret = "half cooled " + ret;
        break;
     case 5:
        ret = "mostly cooled " + ret;
        break;
     case 6:
        ret = "almost completely cooled " + ret;
        break;
     default:
        ret ="stone cold " + ret;
        break;
        }
   return ret; 
} /* query_short_rotten_adjective */


/**
 * This function provides words for the food's long description, 
 * showing how cooled or decayed the object is.
 * 
 * @return The words appended to the food's long, showing its state of 
 *  cooling, or decay if it's already cold.
 *
 * @see query_short_rotten_adjective()
 */
string query_long_decay_level() {
   string ret;
   int flag;   
   if(query_collective() && query_amount() > 1 ) flag = 1;  
   if (this_object()->query_cool_level() == 7){
       return ("It is stone cold. " + ::query_long_decay_level());
   }
   ret = "";
  _cool_level = (time() - _purchase_time)/_cool_rate;
   
   switch ( _cool_level) {
     case 0..1:
          ret += (flag?
          "They have not yet begun to cool.\n":
          "It has not yet begun to cool.\n");
        break;
     case 2:
           ret += (flag?
           "They have cooled slightly.\n":
           "It has cooled slightly.\n");
        break;
     case 3:
           ret += (flag?
           "They have partially cooled.\n":
           "It has partially cooled.\n");
        break;
     case 4:
           ret += (flag?
           "They are half cooled.\n":
           "It is half cooled.\n");
        break;
     case 5:
           ret += (flag?
           "They have mostly cooled.\n":
           "It has mostly cooled.\n");
        break;
     case 6:
           ret += (flag?
           "They are almost completely cold.\n":
           "It is almost completely cold.\n");
        break;
     default:
           ret += (flag?
           "They are stone cold.\n":
           "It is stone cold.\n");
        break;
        }
    return ret; 
} /* query_long_decay_level */

/*
 *  If the food is still hot make the person warm for a while when they eat
 *  it.  Then inherit normal eat stuff.
 */
/**
 * @ignore yes
 */
varargs int do_eat( int no_mess ) {
  int current_warmth;
  int remaining_time;
  int new_warmth;
  int new_time;
  
  if(this_object()->query_cool_level() >= 7 ){ 
    if (this_object()->query_weight_per_bite()){
      setup_eat_piece_messages(this_object()->query_cold_eat_piece_message(),
                               this_object()->query_cold_eat_amt_message(),
                               this_object()->query_cold_eat_last_message());
      }
    else {
      set_eat_mess( this_object()->query_cold_eat_message() );
      }
     return( ::do_eat( no_mess ));
  }
  if(this_player()->query_property("warmth")){
    current_warmth = this_player() -> query_property("warmth");
    remaining_time = this_player() -> query_property_time_left("warmth");
    new_warmth = current_warmth+WARM;
    new_time = (remaining_time + WARM_TIME)*0.75;
    this_player()->add_property( "warmth",new_warmth,new_time);
    }
  else{
     this_player()->add_property("warmth",WARM,WARM_TIME);
     }
  if(this_object()->query_weight_per_bite()){
    setup_eat_piece_messages(this_object()->query_hot_eat_piece_message(),
                                 this_object()->query_hot_eat_amt_message(),
                                 this_object()->query_hot_eat_last_message());
      }
  else{
    set_eat_mess( this_object()->query_hot_eat_message() );
    }
  return( ::do_eat( no_mess ) );
} /* do_eat() */


/* This function sets up the message seen by the player when they eat the food
 * while it is still hot.
 * <P>
 * @param The message displayed when the food is eaten hot.
 * <P>
 * @see query_hot_eat_message(), set_cold_eat_message(), 
 * @see query_cold_eat_message().
*/
void set_hot_eat_message(mixed messages){
   _hot_eat_message = messages;
}

/* This function sets up the message seen by the player when they eat the food
 * when it is cold.
 * <P>
 * @param The message displayed when the food is eaten cold.
 * <P>
 * @see query_cold_eat_message(), set_hot_eat_message(), 
 * @see query_hot_eat_message().
*/
void set_cold_eat_message(mixed messages){
   _cold_eat_message = messages;
}

/* This function sets up the message seen by the player when they eat the food
 * while it is still hot.
 * <P>
 * @return The message displayed when the food is eaten hot.
 * <P>
 * @see set_hot_eat_message(), set_cold_eat_message(), 
 * @see query_cold_eat_message().
*/
mixed query_hot_eat_message(){
  return _hot_eat_message;
}

/* This function sets up the message seen by the player when they eat the food
 * when it is cold.
 * <P>
 * @return The message displayed when the food is eaten cold.
 * <P>
 * @see set_cold_eat_message(), set_hot_eat_message(), 
 * @see query_hot_eat_message().
*/
mixed query_cold_eat_message(){
  return _cold_eat_message;
}
void setup_hot_eat_piece_messages(mixed piece, mixed amt, mixed last) {
   _hot_eat_piece_message = piece;
   _hot_eat_amt_message = amt;
   _hot_eat_last_message = last;
}    

void setup_cold_eat_piece_messages(mixed piece, mixed amt, mixed last) {
   _cold_eat_piece_message = piece;
   _cold_eat_amt_message = amt;
   _cold_eat_last_message = last;
}            

mixed query_cold_eat_piece_message(){
  return _cold_eat_piece_message;
}

mixed query_cold_eat_amt_message(){
  return _cold_eat_amt_message;
}

mixed query_cold_eat_last_message(){
  return _cold_eat_last_message;
}
mixed query_hot_eat_piece_message(){
  return _hot_eat_piece_message;
}

mixed query_hot_eat_amt_message(){
  return _hot_eat_amt_message;
}

mixed query_hot_eat_last_message(){
  return _hot_eat_last_message;
}

/**
 * @ignore yes
 */
mixed *stats(){
   mixed *args;
   args =::stats() + ({ ({ "cool level", _cool_level }),
                        ({ "cool rate", _cool_rate }),
                        ({ "purchase time", _purchase_time }) });
   return args;
}/* stats() */

/*
 * This section covers all the loading of global variables
 */
/**
 * @ignore yes
 */
mapping int_query_static_auto_load(){
   return ([ "::" : ::int_query_static_auto_load(),
             "cool_level" : _cool_level,
             "cool rate" : _cool_rate,
             "purchase time" : _purchase_time ]);
} /* int_query_static_auto_load() */

/**
 * @ignore yes
 */
mapping query_static_auto_load(){
   if ( ( base_name( this_object() ) != "/obj/food" )
       && !query_continuous() ) {
      return 0;
   }
   return int_query_static_auto_load();
} /* query_static_auto_load() */

/**
 * @ignore yes
 */
void init_static_arg( mapping map ){
   if ( !mapp( map ) ) return; 
   if ( map[ "::" ] ) ::init_static_arg( map[ "::" ] );
   _cool_level = map[ "cool_level" ];
   _cool_rate = map[ "cool rate" ];
   _purchase_time = map [ "purchase time" ];
} /* init_static_arg() */

/**
 * @ignore yes
 */
mapping query_dynamic_auto_load(){
   return ([ "::" : ::query_dynamic_auto_load(),
             "cool_level" : _cool_level,
             "cool rate" : _cool_rate,
             "purchase time" : _purchase_time ]);
} /* query_dynamic_auto_load() */

/**
 * @ignore yes
 */
void init_dynamic_arg( mapping map, object ){
   if ( !mapp( map ) ) return;
   if ( map[ "::" ] ) ::init_dynamic_arg( map[ "::" ] );
   _cool_level = map[ "cool_level" ];
   _cool_rate = map[ "cool rate" ];
   _purchase_time = map [ "purchase time" ];
} /* init_dynamic_arg() */

/*
 * This is all masked stuff from /obj/food.c which stops
 * hot food from decaying while it's still hot.
 */

/*
 *  Food wont decay untill it's cold.  Then it decays in normal fashion.
 */
/**
 * @ignore yes
 */
void do_decay() {
   if (this_object()->query_cool_level()>1) return;
   ::do_decay();
} /* do_decay */


/**
 * @ignore yes
 */
int query_decays() {
   if (this_object()->query_cool_level()>1) return 0;
   return 1;
} /* query_decays() */

/**
 * @ignore yes
 */
int query_decay_speed() {
   if (this_object()->query_cool_level()>1) return 0;
   return (::query_decay_speed());
} /* query_decay_speed() */

/**
 * @ignore yes
 */
int query_decay_level() { 
   if (this_object()->query_cool_level()>1) return 0;
   return (::query_decay_level());
} /* query_decay_level() */

/**
 * @ignore yes
 */
void set_decay_speed( int decay ) {
  if (this_object()->query_cool_level()>1) return( ::set_decay_speed( 0 ) );
  return( ::set_decay_speed( 7200 ) );
} /* set_decay_speed() */

/* Mask set_main_plural() cos it breaks the short descriptions if its been 
 * set, so we leave it to the pluralizer to sort out.  If it turns out this 
 * messes things up too much for object names then it will need to be modified
 * to mask the values returned by query_multiple_short().  Yes it's a hack, 
 * but the best I could do at the time.
 */

/**
 * @ignore yes
 */
void set_main_plural(mixed str) {
   return;
}


