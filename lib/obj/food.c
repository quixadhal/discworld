/*  -*- LPC -*-  */
/*
 * $Id: food.c,v 1.51 2003/03/03 06:33:01 terano Exp tilly $
 */
/**
 * <BR>
 * This is the main inheritable for food objects.  It allows you to
 * create food and drink.  Virtual files may also be coded if given
 * the *.food extention.  There are many examples of these virtual
 * food files in the /obj/food directory.  Food does the following
 * things.
 * <BR><BR>
 *   - It can be eaten or drunk.
 * <BR>
 *   - Food can be changed from solid to liquid with set_liquid(),
 * and changed from liquid to solid with reset_liquid().
 * <BR>
 *   - The weight of each bite can be set with set_weight_per_bite().
 * <BR>
 *   - Effects can be added to the player when they are splashed with the food,
 * get food rubbed on them, or get food applied to them with
 * add_external_effect.
 * <BR>
 *   - Splashing, applying, and rubbing can be pk-checked with
 * set_external_pk_check()
 * <BR>
 *   - Splashing can be activated with set_splashable()
 * <BR>
 *   - Splashing can be deactivated with unset_splashable()
 * <BR>
 *   - Applying can be activated with set_applicable()
 * <BR>
 *   - Applying can be deactivated with unset_applicable()
 * <BR>
 *   - Effects can be added to the player when they consume the food
 * with add_eat_effect().
 * <BR>
 *   - Functions can be added to be carried out when the food is
 * consumed with add_eat_func().  This function can be stored
 * on another object if the set_eat_object() is used.
 * <BR>
 *   - An eat message can be set with set_eat_mess().
 * <BR>
 *   - Food can be cut up into pieces with sharp weapons.
 * <BR>
 *   - The description of the new 'pieces' can be set using
 * set_piece_description(), set_piece_short(), set_piece_plural(),
 * and set_piece_substance().
 * <BR>
 *   - Non-liquid food decays and will eventually crumble to dust.
 * <BR>
 *   - The decay speed can be set with set_decay_speed().
 * <BR>
 *   - Food can be 'pickled' to prevent decay using do_cure().
 */

#include <bits_controller.h>
#include <drinks.h>
#include <weapon.h>
#include <move_failures.h>
#include <playtesters.h>

inherit "/obj/cont_medium";
inherit "/std/basic/virtual_update";

#define DEFAULT_DECAY 7200      // Two hours for something to decay.
#define STATE_CHANGE ("/obj/handlers/state_change")

private int _liquid;
private int _dried, _cured;
private int _bites_gone;
private int _weight_per_bite;
private int _in_pieces;
private int _decay_level;
private int _decay_speed;
private int _splashable;
private int _applicable;
private int _external_pk_check;

private float _divisor = to_float(DEFAULT_DECAY) / to_float(6 * TIME_OUT);
// TIME_OUT is defined in /include/bits_controller.h

private mixed _eat_mess;

private string _eat_object;
private string _eat_func;
private string _piece_desc;
private string _piece_short;
private string _piece_plural;
private string _piece_substance;

/* These have been changed from string to mixed as they can either be 
 * a standard string or an array of strings.  
 */ 
private mixed _eat_piece_mess;
private mixed _eat_amt_mess;
private mixed _eat_last_mess;

private mapping _eat_effects = ([ ]);
private mapping _external_effects = ([ ]);


varargs int do_cut(int num);
varargs int do_eat(int no_mess);

int do_mince();
void set_decay_speed(int decay);
int check_for_container();

void create()
{
   do_setup++;
   ::create();
   do_setup--;
   reset_continuous();
   _eat_effects = ([ ]);
   set_decay_speed(DEFAULT_DECAY);
   add_help_file("food");
   if (!do_setup) {
      this_object()->setup();
   }
}                               /* create() */


/**
 * This function returns one if the food is a liquid.
 *
 * @return Return 1 if the food is a liquid, and return 0 if it is not.
 */
int query_liquid()
{
   return _liquid;
}                               /* query_liquid() */

/**
 * This tells us if the food is edible (ie. can be
 * consumed using 'eat' instead of 'drink'.  If it is
 * edible then it is not a liquid.
 *
 * @return Return 1 if the food is a liquid, and return 0 if it is not.
 *
 * @see set_liquid()
 * @see query_liquid()
 */
int query_edible()
{
   return !_liquid;
}                               /* query_edible() */

/**
 * This changes the food object into a liquid.  Liquids are
 * automatically a 'continuous' medium, which means it will
 * combine with other liquids of the same type.
 * <p>
 * This will automatically set the decay speed to 0.  Liquids
 * do not decay.
 *
 * @see reset_liquid()
 * @see set_decay_speed()
 * @see /obj/cont_medium->set_continuous()
 */
void set_liquid()
{
   set_continuous();
   _liquid = 1;
   if (environment()) {
      environment()->calc_liquid();
   }
   set_decay_speed(0);
}                               /* set_liquid() */

/**
 * This changes the food object into a solid.  Solids are not
 * 'continuous' and the decay speed will be set back to the
 * default.
 *
 * @see set_liquid()
 * @see set_decay_speed()
 */
void reset_liquid()
{
   reset_continuous();
   _liquid = 0;
   if (environment())
      environment()->calc_liquid();
   set_decay_speed(DEFAULT_DECAY);
   _divisor = to_float(DEFAULT_DECAY) / to_float(6 * TIME_OUT);
}                               /* reset_liquid() */

/**
 * This method returns 1 if the item is a food object.
 * @return 1 when it is a food object
 */
int query_food_object()
{
   return 1;
}                               /* query_food_object() */

/**
 * @ignore yes
 */
void init()
{
   if (query_continuous()) {
      add_command("rub", "<direct:object> <preposition> <indirect:living>");
      add_command("apply", "<direct:object> <preposition> <indirect:living>");
   }
   if (_liquid) {
      add_command("drink", "<direct:object>");
      add_command("quaff", "<direct:object>");
      add_command("splash",
                  "<direct:object> <preposition> <indirect:living>");
   } else {
      add_command("eat", "<direct:object>", (: do_eat(0) :));
      add_command("cut",
                  "<direct:object> into <number'number, eg: 3'> pieces",
                  (: do_cut($4[1]) :));
      add_command("cut", "<direct:object>", (: do_cut() :));
      add_command("slice", "<direct:object>", (: do_cut() :));
      add_command("mince", "<direct:object>", (: do_mince() :));
   }
}                               /* init() */

/**
 * This query tells us whether the food decays.  All objects
 * which decay should have this function on them.
 *
 * @return Return 1 if the object decays.
 * @see set_decay_speed()
 */
int query_decays()
{
   return _decay_speed != 0;
}                               /* query_decays() */

/**
 * This returns the number of bites that have been taken out
 * of the food object.
 *
 * @return The number of bites which have been taken from
 * the object.
 * @see set_bites_gone()
 * @see set_weight_per_bite()
 */
int query_bites_gone()
{
   return _bites_gone;
}                               /* query_bits_gone() */

/**
 * This sets the weight of each bite of the food object.
 * This is used along with the total weight of the object
 * to calculate how many bites of the food can be taken.
 *
 * @see /std/basic/misc->set_weight()
 * @see set_weight_per_bite()
 * @see weight
 * @return The weight of each bite.
 */
int query_weight_per_bite()
{
   return _weight_per_bite;
}                               /* query_weight_per_bite() */

/**
 * This returns the eat object on which the eat function
 * of the food of the food is stored.
 *
 * @see set_eat_object()
 * @see set_eat_func()
 * @see set_eat_mess()
 * @return A string which is the the name of the eat object.
 */
string query_eat_object()
{
   return _eat_object;
}                               /* query_eat_object() */

/**
 * This returns the name of the eat function to carry out
 * when the object is eaten.
 *
 * @see set_eat_object()
 * @see set_eat_func()
 * @see set_eat_mess()
 * @return A string which is the name of the eat function.
 */
string query_eat_func()
{
   return _eat_func;
}                               /* query_eat_func() */

/**
 * This returns the eat message of the object.
 *
 * @see set_eat_object()
 * @see set_eat_func()
 * @see set_eat_mess()
 * @return A string of text which is the eat message.
 */
string query_eat_mess()
{
   return _eat_mess;
}                               /* query_eat_mess() */

/**
 * This returns the long description of the food object when
 * it is cut up into pieces.
 *
 * @see set_piece_description()
 * @see query_in_pieces()
 * @return A string of text which is the piece description.
 */
string query_piece_description()
{
   return _piece_desc;
}                               /* query_piece_description() */

/**
 * If the food has been cut up (using the 'cut' command)
 * then this will return true.
 *
 * @see do_cut()
 * @return Return 1 if it is in pieces, and return 0 otherwise.
 */
int query_in_pieces()
{
   return _in_pieces;
}                               /* query_in_pieces() */

/**
 * This returns the decay speed.  If this is 0 then the object
 * is not decaying.  This is the length of time it takes
 * for the whole object to decay.
 *
 * @see set_decay_speed()
 * @return The decay speed.
 */
int query_decay_speed()
{
   return _decay_speed;
}                               /* query_decay_speed() */

/**
 * This returns the level of the decay.  This will be a number
 * between 1 and 6.
 *
 * @see set_decay_speed()
 * @return The level of decay, which is between 1 and 6.
 */
int query_decay_level() {
   return _decay_level;
}                               /* query_decay_level() */

/**
 * This sets the number of bites gone.  This helps determine
 * the total weight of the object.
 *
 * @see set_weight_per_bite()
 * @param number The number of bites gone.
 */
void set_bites_gone(int number)
{
   _bites_gone = number;
}                               /* set_bites_gone() */

/**
 * This sets weight of each bite of the food object.  This is
 * used along with the total weight of the object to calculate
 * how many bites of the food can be taken.
 *
 * @see /std/basic/misc->set_weight()
 * @see weight
 * @param number The weight each bite should be.
 */
void set_weight_per_bite(int number)
{
   _weight_per_bite = number;
}                               /* set_weight_per_bite() */

/**
 * This sets the object on which the eat function is stored.
 * <p>
 * If the eat object is set, then the eat function needs to
 * be set as well.  If the function is not set then setting
 * the object has no effect.
 * <p>
 * The function will be called with two parameters, the first
 * being the object being eaten and the second being the
 * person eating the object.
 *
 * @see set_eat_object()
 * @see set_eat_mess()
 * @param word The object containing the eat function.
 * @example
 * void create_food() {
 *   object food;
 *   food = clone_object("/obj/food/apple.food");
 *   food->set_eat_object(this_object());
 *   food->set_eat_func("eat_apple");
 * } /\* create_food() *\/
 *
 * void eat_apple(object food, object eater) {
 *   /\* Do something wonderful! *\/
 * } /\* eat_rabbit() *\/
 */
void set_eat_object(string word)
{
   _eat_object = word;
}                               /* set_eat_object() */

/**
 * This sets the eat function of the food.  This is called when
 * the food is eaten.
 * <p>
 * If the eat object is set, then the eat function needs to be
 * set as well. If the function is not set then setting the
 * object has no effect.
 * <p>
 * The function will be called with two parameters, the first being the
 * object being eaten and the second being the person eating the object.
 *
 * @see set_eat_object()
 * @see set_eat_mess()
 * @param word The function to set as the eat function.
 * @example
 * See the example for set_eat_object()
 */
void set_eat_func(string word)
{
   _eat_func = word;
}                               /* set_eat_func() */

/**
 * This sets the eat function of the food.  The function is
 * carried out when the food is consumed.  The eat message,
 * set with set_eat_mess(), is displayed when this function
 * is carried out.
 * <p>
 * The eat mess should be treated exactly the same as a
 * add_succeeded_mess(), as it is processed later as
 * add_succeeded_mess(eat_mess, ({ }));
 *
 * @see set_eat_object()
 * @see set_eat_func()
 * @see /global/new_parse->add_succeeded_mess()
 * @param word The string or array to be used as
 * the add_succeeded_mess() when eating the food.
 */
void set_eat_mess(mixed word)
{
   _eat_mess = word;
}                               /* set_eat_mess() */

/**
 * This sets up the eat messages for pieces and stuff.  The amount message
 * will replace the $amt$ string in the message with the amount_string().  
 * @param piece the eat piece message. This can be either a string or an
 * array, it is passed to add_succeeded_mess().
 * @param amt the amount messages
 * @param last the message to print out when the last bit gets eaten
 */
void setup_eat_piece_messages(mixed piece,
                              mixed amt,
                              mixed last) {
   _eat_piece_mess = piece;
   _eat_amt_mess = amt;
   _eat_last_mess = last;
}                               /* setup_eat_piece_message() */

/**
 * This sets the long description of the food when it is has
 * 'cut' into pieces.
 * <p>
 * If this is set then the short of the object is changed
 * to 'piece of cake'.  If the piece description is not
 * set the short description of the object is not changed.
 *
 * @param word The long description to use when cut into pieces.
 * @see query_in_pieces()
 * @see set_piece_short()
 * @see set_piece_plural()
 * @see set_piece_substance()
 * @example
 * set_piece_description("The brown, soft, moist cake looks "
 *   "totally wonderful.\n");
 */
void set_piece_description(string word) {
   _piece_desc = word;
}                               /* set_piece_description() */

/**
 * This is the description used in the short for the pieces.
 * When the object is cut up this is used as part of the
 * short description.  For example, it can change
 * 'piece of cake' to 'slice of cake', 'piece of chocolate'
 * to 'square of chocolate', and so on.  If this the piece
 * short has not been set it is set to the default which is
 * simply "piece".
 *
 * @param word The short to use for the 'piece' when cut into
 * pieces.
 * @see query_in_pieces()
 * @see set_piece_description()
 * @see set_piece_plural()
 * @see set_piece_substance()
 * @example
 * set_piece_short("slice");
 */
void set_piece_short(string short) {
   _piece_short = short;
}                               /* set_piece_short() */

/**
 * This plural description used for the pieces.  It is only
 * really of use if the piece short pluralises in an unusual
 * manner.  If it is not set the piece plural is set to what
 * the pice short has been set to with an "s" stuck on the
 * end, for example, slice to slices.
 *
 * @param word The plural used for the 'pieces' when cut into
 * pieces.
 * @see query_in_pieces()
 * @see set_piece_description()
 * @see set_piece_short()
 * @see set_piece_substance()
 * @example
 * set_piece_plural("slices");
 */
void set_piece_plural(string plural) {
   _piece_plural = plural;
}                               /* set_piece_plural() */

/**
 * This sets a new description for what there are pieces of.
 * For example, if you want the short of the food before it
 * is cut up to be 'bar of chocolate', without the piece
 * substance being set, when it is cut up it will become
 * 'a piece of bar of chocolate'.  If you set this in enables
 * you to change that unwieldly description to 'a piece of
 * chocolate'.
 *
 * @param word The description of what the 'piece' is of.
 * @see query_in_pieces()
 * @see set_piece_description()
 * @see set_piece_short()
 * @see set_piece_plural()
 * @example
 * set_piece_substance("goat cheese");
 */
void set_piece_substance(string substance) {
   _piece_substance = substance;
}                               /* set_piece_substance() */

/**
 * This sets the in_pieces flag.  This allows you to make the
 * food seem as though it has already been cut into pieces.
 *
 * @param number The number of pieces.
 * @see query_in_pieces()
 * @see set_piece_description()
 * @see set_piece_short()
 * @see set_piece_plural()
 * @see set_piece_substance()
 */
void set_in_pieces(int number) {
   int p;
   _in_pieces = p;
}                               /* set_in_pieces() */

/**
 * This sets the decay level.  This should be anumber
 * between 1 and 6.  It lets you make a food object seem
 * as though it is already decaying.
 *
 * @see query_decay_level()
 * @see set_decay_speed()
 * @param level The new decay level.
 */
void set_decay_level(int level) {
   _decay_level = level;
}                               /* set_decay_level() */

/**
 * This sets the speed at which a food object decays.  The
 * speed is set in seconds and is the total amount of time
 * before the food decays.
 * <p>
 * If the decay speed is set to 0, then the object will never decay.
 *
 * @param speed The speed at which the object will decays.
 * @see set_decay_level()
 * @see query_decay_speed()
 * @example
 * /\* This will make the object decay totaly in 30 minutes *\/
 * set_decay_speed(1800);
 *
 * /\* This will stop the object from ever decsaying *\/
 * set_decay_speed(0);
 */
void set_decay_speed(int decay) {
   float tmp;

   if(decay != _decay_speed && !_dried && !_cured) {
      BITS_CONTROLLER->remove_bit(this_object());
      _decay_speed = decay;
      if (decay && !_dried && !_cured) {
         tmp = _divisor;
         _divisor = to_float(decay) / to_float(6 * TIME_OUT);
         _decay_level = to_int(_decay_level * _divisor / tmp);
         BITS_CONTROLLER->add_bit(this_object());
      }
   }
}                               /* set_decay_speed() */

/**
 * @ignore yes
 * This is called by the bits controller.
 */
void do_decay() {
  if(_dried || _cured) {
    BITS_CONTROLLER->remove_bit(this_object());
    return;
  }

  if ( environment() && environment()->query_property( "no decay" ) )
    return;

  _decay_level++;

  if (_decay_level > (2 * _divisor) -1 ) {
    remove_adjective( "fresh" );
  }

  if (_decay_level > 6 * _divisor) {
    tell_room(environment(this_object()),
              capitalize(the_short()) + " crumbles to dust.\n");
    move("/room/rubbish");
  }
}

/**
 * @ignore yes
 */
int query_weight() {
  if (query_liquid())
    return 0;

  return ::query_weight() - _bites_gone * _weight_per_bite;
}

/** 
 * This method returns the current rotten adjective that's used 
 * in the short description for decaying objects.
 * @return a string like slightly rotten, mostly rotten, etc.
 */ 
string query_short_rotten_adjective() {
    string ret;

    ret = "";

    switch (to_int(_decay_level / _divisor)) {
    case 2:
      ret = "slightly rotten ";
      break;
    case 3:
      ret = "half rotten ";
      break;
    case 4:
      ret = "mostly rotten "; 
      break;
    case 5:
      ret = "almost rotten ";
      break;
    case 6:
      ret = "rotten ";
      break;
    }

    return ret; 
} /* query_short_rotten_adjective() */

/**
 * @ignore yes
 */
string short(int dark) {
  string ret;

  ret = ::short(dark);

  if (_decay_speed) {
     ret = query_short_rotten_adjective() + ret; 
  }
  return ret;
  
}

/** 
 * This method returns how much of the item has been nibbled on. 
 * @return a string used in long().
 */ 
string query_long_eat_level() { 
   int twelfths;
   string ret; 

    twelfths = (_bites_gone * _weight_per_bite * 12) / ::query_weight();

    switch (twelfths) {
        case 0..1:
            ret = "It has just been nibbled on.\n";
        break;
        
        case 2:
            ret = "It is about one sixth gone.\n";
        break;
        
        case 3:
            ret = "It is about one quarter gone.\n";
        break;
        
        case 4:
            ret = "It is about one third gone.\n";
        break;
        
        case 5:
            ret = "It is just under half gone.\n";
        break;
        
        case 6:
            ret = "It is about half gone.\n";
        break;
        
        case 7:
            ret = "It is just over half gone.\n";
        break;
        
        case 8:
            ret = "It is about two thirds gone.\n";
        break;
        
        case 9:
            ret = "It is about three quarters gone.\n";
        break;
        
        case 10:
            ret = "It is about five sixths gone.\n";
        break;

        case 11:   
            ret = "It is almost all gone.\n";
        break;

        default:
            ret = "Schroedinger's cat has been at it.\n";
        break;
    }

    return ret; 
} /* query_eat_decay_level() */ 

/** 
 * This method returns the decay level for the object as a string, 
 * it is used in the long() function.
 */ 
string query_long_decay_level() {
    string ret;

    ret = "";

    switch (to_int(_decay_level / _divisor)) {
    case 0..1:
      if(query_collective() && query_amount() > 1)
        ret += "They look nice and fresh.\n";
      else
       ret += "It looks nice and fresh.\n";
      break;
    case 2:
      if(query_collective() && query_amount() > 1)
        ret += "They look a bit rotten.\n";
      else
        ret += "It looks a bit rotten.\n";
      break;
    case 3:
      if(query_collective() && query_amount() > 1)
        ret += "They are half rotten.\n";
      else
        ret += "It is half rotten.\n";
      break;
    case 4:
      if(query_collective() && query_amount() > 1)
        ret += "They are mostly rotten.\n";
      else
        ret += "It is mostly rotten.\n";
      break;
    case 5:
      if(query_collective() && query_amount() > 1)
        ret += "They are almost completely rotten.\n";
      else
        ret += "It is almost completely rotten.\n";
       break;
    case 6:
      if(query_collective() && query_amount() > 1)
        ret += "They are completely rotten.\n";
      else
        ret += "It is completely rotten.\n";
      break;
    }

    return ret; 
} /* query_long_decay_level() */


/**
 * @ignore yes
 */
string long(string words, int dark) {
   string ret;

   ret =::long(words, dark);

   if (_liquid) {
      return ret;
   }
   if (_weight_per_bite && _bites_gone) {
       ret += query_long_eat_level(); 
   }
   if (_decay_speed) {
       ret += query_long_decay_level(); 
   } 
   else {
       if(_dried) {
           if(query_collective() && query_amount() > 1) {
              ret += "They are all dry and shrivelled up.\n";
           }
           else {
              ret += "It is all dry and shrivelled up.\n";
           }
        } 
        else { 
            if(_cured) {
                if(query_collective() && query_amount() > 1) { 
                    ret += "They seem to have been pickled.\n";
                }
                else {
                    ret += "It seems to have been pickled.\n";
                }
            }
        }
   }
   return ret;
}

/**
 * This sets up the eat object and function.  This is used when
 * the food is eaten.  The eat function is called on the eat
 * object.  If successful the eat message is added to the player.
 *
 * @see set_eat_object()
 * @see set_eat_func()
 * @param ob The object to use.
 * @param func The function to use.
 */
void setup_eat(string ob, string func) {
   _eat_object = ob;
   _eat_func = func;
}                               /* setup_eat() */

/**
 * The eat effects of the food.  The mapping contains a set of
 * strings (the effect) and a number associated with the
 * effect.
 *
 * @see add_eat_effect()
 * @return A mapping containing the eat effects.
 */
mapping query_eat_effects() {
   if (undefinedp(_eat_effects)) {
      _eat_effects = ([ ]);
   }
   return copy( _eat_effects );
}                               /* query_eat_effects() */

/**
 * This sets all the eat effects.  This function should NOT
 * be used.  Please use add_eat_effect.
 *
 * @param map The eat effects mapping.
 */
void set_eat_effects(mapping map) {
   if (mapp(map)) {
      _eat_effects = map;
   }
}                               /* set_eat_effects() */

/**
 * This adds an eat effect to the food.  This will be added to
 * the player or NPC when the food is eaten.
 * <p>
 * The effect is added with a number as the argument.  The number
 * is based on the amount which is eaten (in weight units), and
 * usually represents the strength of the effect.
 * If the object is continuous, the weight is calculated from
 * the amount and weight_unit array...
 * <p>
 * <br>
 * <code>
 * eff_num = (amount * number * weight_unit[0]) / weight_unit[1]
 * </code>
 * <p>
 * ...where the number is passed into the add_eat_effect() function.
 * If the effect already exists, then the number is added onto
 * the existing number.
 *
 * @see /obj/cont_medium->set_weight_unit()
 * @see remove_eat_effect()
 * @param word The file name of the effect to add.
 * @param number The number to set to the effect to.
 * @return The current value of the effect in the mapping
 */
int add_eat_effect(mixed word,
                   int number)
{
   if (undefinedp(_eat_effects)) {
      _eat_effects = ([ ]);
   }

   if (mapp(word)) {
      int num;
      string name;

      foreach(name, number in word) {
         num += add_eat_effect(name, number);
      }
      return num;
   }

   if (_eat_effects[word]) {
      _eat_effects[word] += number;
   } else {
      _eat_effects[word] = number;
   }
   return _eat_effects[word];
}                               /* add_eat_effect() */

/**
 * This removes the eat effect of the given name.
 *
 * @param word The name of the effect to delete.
 * @see add_eat_effect()
 */
void remove_eat_effect(string word)
{
   if (!undefinedp(_eat_effects[word])) {
      _eat_effects = m_delete(_eat_effects, word);
   }
}                               /* remove_eat_effect() */

/**
 * This makes the external effects pk checked.
 * @see add_external_effect(), set_applicable(), set_splashable()
 * @see unset_external_pk_check()
 */
void set_external_pk_check()
{
   _external_pk_check = 1;
}

/**
 * This makes the external effects not pk checked.
 * @see add_external_effect(), set_applicable(), set_splashable()
 * @see set_external_pk_check()
 */
void unset_external_pk_check()
{
   _external_pk_check = 0;
}


/**
 * The external (splash,apply,rub) effects of the food.  The
 * mapping contains a set of strings (the effect) and a number
 * associated with the effect.
 *
 * @see set_splashable(), set_applicable()
 * @return A mapping containing the external effects.
 */
mapping query_external_effects()
{
   if (undefinedp(_external_effects))
      _external_effects = ([ ]);
   return _external_effects + ([ ]);
}                               /* query_external_effects() */

/**
 * This sets all the external effects.  This function should NOT
 * be used.  Please use add_external_effect.
 *
 * @param map The external effects mapping.
 */
void set_external_effects(mapping map)
{
   _external_effects = map;
}                               /* set_splash_effects() */

/**
 * This adds an external effect to the food.  This will be added to
 * the player or NPC when the liquid is splashed, applied, or rubbed on them.
 * <p>
 * The effect is added with a number as the argument.  The number
 * is based on the amount which is splashed (in weight units), and
 * usually represents the strength of the effect.
 * If the object is continuous, the weight is calculated from
 * the amount and weight_unit array...
 * <p>
 * <br>
 * <code>
 * eff_num = (amount * number * weight_unit[0]) / weight_unit[1]
 * </code>
 * <p>
 * ...where the number is passed into the add_external_effect() function.
 * If the effect already exists, then the number is added onto
 * the existing number.
 *
 * @see /obj/cont_medium->set_weight_unit()
 * @see set_splashable()
 * @see unset_splashable()
 * @see set_applicable()
 * @see unset_applicable
 * @see remove_external_effect()
 * @see set_external_pk_check()
 * @param word The file name of the effect to add.
 * @param number The number to set to the effect to.
 * @return The current value of the effect in the mapping
 */
int add_external_effect(string word,
                        int number)
{
   if (undefinedp(_external_effects))
      _external_effects = ([ ]);
   if (_external_effects[word])
      _external_effects[word] += number;
   else
      _external_effects[word] = number;
   return _external_effects[word];
}                               /* add_external_effect() */

/**
 * This removes the external effect of the given name.
 *
 * @param word The name of the effect to delete.
 * @see add_external_effect()
 */
void remove_external_effect(string word)
{
   if (!undefinedp(_external_effects[word])) {
      _external_effects = m_delete(_external_effects, word);
   }
}                               /* remove_external_effect() */

/**
 * This sets it so the food can be splashed.
 * Note that the food can only be splashed when liquid in any case.
 * Note this is NOT the default case.
 * @see unset_splashable(), set_applicable(), unset_applicable(),
 * add_external_effect()
 */
void set_splashable()
{
   _splashable = 1;
}

/**
 * This sets it so the food cannot be splashed.
 * Note that the food can only be splashed when liquid in any case.
 * @see set_splashable()
 */
void unset_splashable()
{
   _splashable = 0;
}

/**
 * This queries splashability of the food.
 * Note that the food can only be splashed when liquid and when
 * set_splashable() has been called
 * @param splasher - Who splashes.
 * @param splashee - Who is splashed.
 * @return 1 if it can splash, 0 if it can't
 * @see set_splashable(), unset_splashable(), set_liquid(),
 * set_external_pk_check()
 */
int query_splashable(object splasher,
                     object splashee)
{
   if (splasher == splashee || !_external_pk_check) {
      return (_splashable && _liquid);
   }
   return (_splashable && _liquid && !(pk_check(splasher, splashee)));
}



/**
 * This sets it so the food can be applied.
 * Note this is the default case.
 * @see unset_applicable()
 */
void set_applicable()
{
   _applicable = 1;
}

/**
 * This sets it so the food cannot be applied.
 * @see set_applicable()
 */
void unset_applicable()
{
   _applicable = 0;
}

/**
 * This queries applicability of the food. (rubbing on someone)
 * Note that the food can only be applied when
 * set_applicable() has been called
 * @param applier - Who applies it
 * @param appliee - Who it is applied to.
 * @return 1 if it can be applied, 0 if it can't
 * @see set_applicable(), unset_applicable(), set_external_pk_check()
 */
int query_applicable(object applier,
                     object appliee)
{
   if (applier == appliee || !_external_pk_check) {
      return _applicable;
   }
   return (_applicable && !(pk_check(applier, appliee)));
}




/**
 * @ignore yes
 */
void being_joined_by(object thing)
{
   int i,
     that,
     this;
   string *words;
   mapping new_effects;
   that = (int) thing->query_amount();
   this = query_amount();
   if (!(this + that))
      return;
   if (thing->query_splashable(0, 0) && !_splashable)
      set_splashable();
   if (thing->query_applicable(0, 0) && !_applicable)
      set_applicable();
   if (thing->query_external_pk_check() && !_external_pk_check)
      set_external_pk_check();
   new_effects = (mapping) thing->query_eat_effects();
   // If new_effects isn't a mapping, then there's something
   // wrong, so it should give an error so that someone knows
   // and can get it fixed.
   words = m_indices(_eat_effects);
   for (i = 0; i < sizeof(words); i++) {
      if (new_effects[words[i]]) {
         new_effects[words[i]] *= that;
         new_effects[words[i]] += this * _eat_effects[words[i]];
      } else {
         new_effects[words[i]] = this * _eat_effects[words[i]];
      }
   }
   words = m_indices(new_effects);
   for (i = 0; i < sizeof(words); i++) {
      new_effects[words[i]] /= this + that;
   }
   _eat_effects = new_effects;

   new_effects = (mapping) thing->query_external_effects();
   // If new_effects isn't a mapping, then there's something
   // wrong, so it should give an error so that someone knows
   // and can get it fixed.
   words = m_indices(_external_effects);
   for (i = 0; i < sizeof(words); i++) {
      if (_external_effects[words[i]]) {
         new_effects[words[i]] *= that;
         new_effects[words[i]] += this * _external_effects[words[i]];
      } else {
         new_effects[words[i]] = this * _external_effects[words[i]];
      }
   }
   words = m_indices(new_effects);
   for (i = 0; i < sizeof(words); i++) {
      new_effects[words[i]] /= this + that;
   }
   _external_effects = new_effects;

}                               /* being_joined_by() */

/**
 * This consumes some of the food.
 *
 * @param consumer The person doing the consumption.
 * @param amount The amount being consumed.
 * @param type Use "splash" or "apply" or "external" to use external
 *  effects, anything else for eat effects.
 *
 */
varargs void consume(object consumer, int amount, string type) {
  int i, denominator, numerator, wholes;
  string *words;
  
  switch (type) {
  case "splash":
  case "apply":
  case "external":
    words = m_indices(_external_effects);
    break;
  default:
    words = m_indices(_eat_effects);
    break;
  }
  if (!amount && query_continuous()) {
    amount = query_amount();
  }
  if (amount) {
    // Make sure we don't take more of it than there is.
    if (query_continuous() && (amount > query_amount()))
      amount = query_amount();
    numerator = amount * query_weight_unit()[0];
    denominator = query_weight_unit()[1];
    if (type != "apply" && type != "splash" && type != "external") {
      if (_liquid) {
        consumer->adjust_volume(D_DRINK, (50 * numerator) / denominator);
      } else {
        consumer->adjust_volume(D_FOOD, (50 * numerator) / denominator);
      }
    }
    if (!this_object()->query_property("playtester") ||
        PLAYTESTER_HAND->query_playtester(consumer->query_name()) ||
        consumer->query_creator()) {
      for (i = 0; i < sizeof(words); i++) {
        switch (type) {
        case "splash":
        case "apply":
        case "external":
          numerator = _external_effects[words[i]] * amount *
            query_weight_unit()[0];
          break;
        default:
          numerator = _eat_effects[words[i]] * amount *
            query_weight_unit()[0];
          break;
        }
        wholes = numerator / denominator;
        numerator %= denominator;
        if (random(denominator) < numerator) {
          wholes++;
        }
        consumer->add_effect(words[i], wholes);
      }
    }
    adjust_amount(-amount);
  } else {
    if (_weight_per_bite) {
      _bites_gone++;
      if (type != "apply" && type != "splash" && type != "external") {
        consumer->adjust_volume(D_FOOD, 50 * _weight_per_bite);
      }
      if (!this_object()->query_property("playtester") ||
          PLAYTESTER_HAND->query_playtester(consumer->query_name()) ||
          consumer->query_creator()) {
        for (i = 0; i < sizeof(words); i++) {
          switch (type) {
          case "splash":
          case "apply":
          case "external":
            this_player()->add_effect(words[i],
                                      _external_effects[words[i]] *
                                      _weight_per_bite);
            break;
          default:
            this_player()->add_effect(words[i], _eat_effects[words[i]] *
                                      _weight_per_bite);
            break;
          }
        }
      }
    } else {
      consumer->adjust_volume(D_FOOD, 50 * weight);
      if (!this_object()->query_property("playtester") ||
          PLAYTESTER_HAND->query_playtester(consumer->query_name()) ||
          consumer->query_creator()) {
        for (i = 0; i < sizeof(words); i++) {
          switch (type) {
          case "apply":
          case "splash":
          case "external":
            consumer->add_effect(words[i],
                                 _external_effects[words[i]] * weight);
            break;
          default:
            consumer->add_effect(words[i],
                                 _eat_effects[words[i]] * weight);
            break;
          }
        }
      }
    }
  }
  if (type != "apply" && type != "splash" && type != "external") {
    if (_eat_func) {
      if (_eat_object) {
        call_other(_eat_object, _eat_func, this_object(), consumer);
      } else {
        call_other(this_object(), _eat_func, this_object(), consumer);
      }
    }
  }
}

/**
 * This is the eat command call.  This is setup with add_command()
 * and does the actual eating.
 * @param no_mess don't use success messages
 * @return Return 1 on success, 2 on success + last bit eaten and return 0 on failure.
 * @see /global/new_parse->add_command()
 * @see /global/new_parse->add_succeeded_mess()
 * @see set_eat_mess()
 */
varargs int do_eat(int no_mess) {
   int ret;

   
   if ((weight <= 0) && !query_continuous()) {
      if (_eat_mess && !no_mess) {
         add_succeeded_mess(_eat_mess, ({ }));
      }
      move("/room/rubbish");
      return 1;
   }
   if ((weight <= (_bites_gone + 1) * _weight_per_bite) || !_weight_per_bite) {
      if (!no_mess) {
         if (!query_continuous()) {
            if (_weight_per_bite) {
               if (_eat_last_mess) {
                  add_succeeded_mess(_eat_last_mess, ({ }));
               } else {
                  add_succeeded_mess("$N $V the last of $D.\n", ({ }));
               }
            } else if (_eat_mess) {
               // This only gets done if the whole thing is eaten.
               add_succeeded_mess(_eat_mess, ({ }));
            }
         } else {
            if (_eat_amt_mess) {
               add_succeeded_mess(replace_string(_eat_amt_mess, "$amt$",
                                                 amount_size() ), ({ }));
            } else {
               add_succeeded_mess("$N $V " + amount_size() + " of $D.\n",
                                  ({ }));
            }
         }
      }
      ret = 2;
      move("/room/rubbish");
   } else {
      if (!no_mess) {
         if (_eat_piece_mess) {
            add_succeeded_mess(_eat_piece_mess, ({ }));
         } else {
            add_succeeded_mess("$N $V a piece of $D.\n", ({ }));
         }
      }
      ret = 1;
   }
   consume(this_player(), 0);
   return ret;
}                               /* do_eat() */

/**
 * This checks to see if a weapon is sharp.  This is used later
 * to determine whather an object can be cut up with the weapon.
 *
 * @param weapon The weapon to check to see if it is sharp.
 * @return Return 1 if it is sharp, or 0 otherwise.
 */
int check_sharp(object weapon)
{
   mixed *data;
   int i;
   if (!weapon) {
      return 0;
   }
   data = weapon->query_attack_data();
   for (i = W_TYPE; i < sizeof(data); i += W_ARRAY_SIZE) {
      if (data[i] == "sharp") {
         return 1;
      }
   }
   return 0;
}                               /* check_sharp() */

/**
 * This cuts the food up into bits.  This is the command called
 * with add_command() and does the actual cutting up into bits.  If
 * the num_pieces parameter is undefined, the food is cut in half.
 * If the food is continuous, the cutting is handled by
 * the state_change handler (all continuous objects must be cut this
 * way).
 *
 * @see set_piece_description()
 * @see query_in_pieces()
 * @see query_piece_short()
 * @see query_piece_plural()
 * @see query_piece_substance()
 * @param num_pieces The number of pieces to cut the food into.
 * @return 1 if it succeeded, 0 if not.
 */
varargs int do_cut(int num_pieces) {
  object bing, *obs, with;
  string name, *exploded_short;
  int i, j, k, size_of_each, gone;
  int portion_of_whole;
  int portion_of_parent;
  string temp_short, temp_plural;
  mixed rabbit;
  
  if (query_liquid()) {
    add_failed_mess("You cannot $V $D.\n");
    return -1;
  }

  // First, check to see if they have a sharp weapon.
  obs = this_player()->query_holding();
  for (i = 0; i < sizeof(obs); i++) {
    if (check_sharp(obs[i])) {
      if (!with || (with && with->query_weight() > obs[i]->query_weight())) {
        with = obs[i];
      }
    }
  }

  if (!with) {
    add_failed_mess("You have to be holding a sharp object to $V $D.\n");
    return -1;
  }

  if (undefinedp(num_pieces))
    num_pieces = 2;
  
  if (query_continuous())
    return (do_mince());
  
  if (num_pieces <= 1 || !_weight_per_bite) {
    add_failed_mess("$D cannot be cut up.\n");
    return -1;
  } else if (num_pieces >
             ((query_weight() / _weight_per_bite) - _bites_gone)) {
    
    if (num_pieces == 2 || ((query_weight() / _weight_per_bite) -
                            _bites_gone) < 2) {
      add_failed_mess("$D cannot be cut up.\n");
    } else
      add_failed_mess("You can't $V $D into that many pieces.\n");
    return -1;
  }

  // Now, do the cutting.
  name = file_name(this_object());
  sscanf(name, "%s#%*d", name);
  size_of_each = (query_weight() - _weight_per_bite * _bites_gone) /
    num_pieces;
  gone = (::query_weight() - size_of_each) / _weight_per_bite;
  for (i = 0; i < num_pieces; i++) {
    // Duplicate ourselves.
    bing = clone_object(name);
    bing->init_static_arg(query_static_auto_load());
    bing->init_dynamic_arg(query_dynamic_auto_load());
    bing->set_weight(size_of_each);
    bing->set_value(0);
    
    bing->add_property("whole:weight", query_property("whole:weight")
                       || query_weight());
    bing->add_property("whole:short", query_property("whole:short")
                       || this_player()->convert_message(a_short()));
    
    portion_of_whole = bing->query_property("whole:weight") / size_of_each;
    
    portion_of_parent = query_weight() / size_of_each;
    
    if (!_in_pieces) {        // is _in_pieces *ever* not 0??
      
      if (_piece_desc) {
        bing->set_long(_piece_desc);
        if (!_in_pieces) {  //  redundant check?
          if (!_piece_short) {
            if (portion_of_whole == 1)
              temp_short = "piece";
            else if (portion_of_whole == 2)
              temp_short = "half";
            else if (portion_of_whole == 4)
              temp_short = "quarter";
            else
              temp_short = word_ordinal(portion_of_whole);
          }
          
          if (!_piece_plural) {
            if (_piece_short)
              _piece_plural = _piece_short + "s";
            else {
              if (temp_short == "half")
                temp_plural = "halves";
              else
                temp_plural = temp_short + "s";
            }
          }
          
          if (_piece_substance)
            rabbit = _piece_substance;
          else
            rabbit = bing->query_property("whole:short");
          bing->set_short((_piece_short || temp_short) +
                          " of " + rabbit);
          bing->set_main_plural((_piece_plural || temp_plural) +
                                " of " + rabbit);
        }
      }
    } else {     // Is this ever evaluated?  _in_pieces is always 0!
      bing->set_bites_gone(gone);
    }
    
    /* These next two might not be valid if _in_pieces isn't 0.  Good
     * thing it always is! */
    
    bing->add_alias(_piece_short || temp_short);
    bing->add_plural(_piece_plural || temp_plural);
    
    exploded_short = explode(bing->query_short(), " ");
    j = sizeof(exploded_short);
    bing->set_name(exploded_short[j - 1]);
    for (k = 0; k < sizeof(exploded_short) - 1; k++) {
      bing->add_adjective(exploded_short[k]);
    }
    
    bing->set_in_pieces(1);   // This doesn't do anything!
    if (bing->move(environment((this_object())))!= MOVE_OK ) {
      bing->move(environment(this_player()));
    }
  }

  move("/room/rubbish");
  
  // Use the default message.
  
  if (with->query_weight() < 20) {
    add_succeeded_mess(({ sprintf("$N $V $D into %s pieces with %s%s.\n",
                                  query_num(num_pieces), with->poss_short(),
                                  (num_pieces * size_of_each) <
                                  query_weight()?
                                  ", carelessly dribbling a bit while you do "
                                  "so" : ""),
                            sprintf("$N $V $D into %s pieces with %s.\n",
                                    query_num(num_pieces),
                                    with->poss_short()) }));
    
  } else {
    add_succeeded_mess(({sprintf("$N stand back and hack $D into %s pieces "
                                 "with %s%s.\n", query_num(num_pieces),
                                 with->poss_short(),
                                 (num_pieces * size_of_each) < query_weight()?
                                 ", carelessly obliterating a bit when you do "
                                 "so" : ""),
                           sprintf("$N stands back and hacks $D into %s "
                                   "pieces with %s.\n",
                                   query_num(num_pieces),
                                   with->poss_short()) }));
    
  }
  return 1;
}

varargs int do_mince()
{
   object *obs,
     with,
     transformed;
   int i,
     rem_pieces;

   if (query_liquid()) {
      add_failed_mess("You cannot $V $D.\n");
      return -1;
   }
   // First, check to see if they have a sharp weapon.
   obs = this_player()->query_holding();
   for (i = 0; i < sizeof(obs); i++) {
      if (check_sharp(obs[i])) {
         if (!with || (with && with->query_weight() > obs[i]->query_weight())) {
            with = obs[i];
         }
      }
   }
   if (!with) {
      add_failed_mess("You have to be holding a sharp object to $V " "$D.\n");
      return -1;
   }

   transformed = STATE_CHANGE->transform(this_object(), "slice");

   // if it doesn't mince, cut into the remaining number of pieces

   if (!objectp(transformed)) {
      if (query_continuous()) {
         add_failed_mess("$D cannot be cut up any further.\n");
         return -1;
      }
      if (_weight_per_bite) {
         rem_pieces = (query_weight() / _weight_per_bite) - _bites_gone;
         return (do_cut(rem_pieces));
      }
      return -1;
   }

   this_object()->move("/room/rubbish");

   if (transformed->move(this_player()) != MOVE_OK) {
      call_out("tell_object", 0, this_player(), "You drop " +
               transformed->the_short() + ".\n");
      call_out("tell_room", 0, environment(this_player()),
               this_player()->the_short() + " drops " +
               transformed->the_short() + ".\n");
      transformed->move(environment(this_player()));
   }
   add_succeeded_mess("$N $V $D with " + with->the_short() +
                      " into $I.\n", ({ transformed }));
   return 1;
}

/**
 * This gives the appropriate message for drinking things.
 *
 * @see do_eat()
 * @see consume(*)
 * @see set_liquid()
 * @return Return 1 if it succeeded, or return 0 if it failed.
 */
int do_drink()
{

   if (!check_for_container()) {
      add_failed_mess("You go to try and $V $D, but it suddenly "
                      "remembers what happens when it is not in a "
                      "container and drains away to dust.\n");
      move("/room/rubbish");
      return 0;
   }

   this_player()->add_succeeded_mess(this_object(), "$N $V " +
                                     amount_size() + " of $D.\n", ({ }));
   consume(this_player(), 0);
   return 1;
}                               /* do_drink() */

/**
 * This gives the appropriate message for quaffing things. Its
 * rather like drinking but a lot of it gets splashed on you.
 *
 * @see do_eat()
 * @see consume(*)
 * @see set_liquid()
 * @return Return 1 if it succeeded, or return 0 if it failed.
 */
int do_quaff()
{

   if (!check_for_container()) {
      add_failed_mess("You go to try and $V $D, but it suddenly "
                      "remembers what happens when it is not in a "
                      "container and drains away to dust.\n");
      move("/room/rubbish");
      return 0;
   }

   this_player()->add_succeeded_mess(this_object(), "$N $V " +
                                     amount_size() +
                                     " of $D spilling most of "
                                     "it of course.\n", ({ }));

   consume(this_player(), query_amount() * 6 / 10, "splash");
   consume(this_player(), query_amount() * 4 / 10);

   return 1;
}                               /* do_quaff() */

/**
 * This handles applying things
 *
 * @see set_applicable()
 * @see consume(*)
 * @return Return 1 if it succeeded, or return 0 if it failed.
 */
int do_apply(object * things)
{
   if (sizeof(things) > 1) {
      add_failed_mess("You must $V $D to one person alone.\n", ({ }));
      return 0;
   }
   if (!_applicable) {
      add_failed_mess("You cannot $V $D to anybody.\n");
      return 0;
   }
   if (_external_pk_check && pk_check(this_player(), things[0])) {
      add_failed_mess("You feel it would be wrong to rub $D on $I.\n",
                      things);
      return 0;
   }
   this_player()->add_succeeded_mess(this_object(), "$N $V " +
                                     amount_size() + " of $D to $I.\n",
                                     ({ things[0] }));
   consume(things[0], 0, "apply");
   return 1;
}                               /* do_apply() */

/**
 * @ignore yes
 */
int do_rub(object * things)
{
   /*identical to do_apply, except for one little syntax thing =) */
   if (sizeof(things) > 1) {
      add_failed_mess("You must $V $D on one person alone.\n", ({ }));
      return 0;
   }
   if (!_applicable) {
      add_failed_mess("You cannot $V $D on anybody.\n");
      return 0;
   }
   if (_external_pk_check && pk_check(this_player(), things[0])) {
      add_failed_mess("You feel it would be wrong to rub $D on $I.\n",
                      things);
      return 0;
   }

   this_player()->add_succeeded_mess(this_object(), "$N $V " +
                                     amount_size() + " of $D on $I.\n",
                                     ({ things[0] }));
   consume(things[0], 0, "apply");
   return 1;
}                               /* do_rub() */

int do_splash(object * things)
{
   if (sizeof(things) > 1) {
      add_failed_mess("You must $V $D on one person alone.\n", ({ }));
      return 0;
   }
   if (!_splashable || !_liquid) {
      add_failed_mess("You cannot $V $D on anybody.\n", ({ }));
      return 0;
   }
   if (_external_pk_check && pk_check(this_player(), things[0])) {
      add_failed_mess("You feel it would be wrong to splash $D on $I.\n",
                      things);
      return 0;
   }
   this_player()->add_succeeded_mess(this_object(), "$N $V " +
                                     amount_size() + " of $D on $I.\n",
                                     ({ things[0] }));
   consume(things[0], 0, "splash");
   return 1;
}                               /* do_splash() */


/**
 * @ignore yes
 */
mapping int_query_static_auto_load() {
   return ([ "::" : ::int_query_static_auto_load(),
             "liquid" : _liquid,
             "weight_per_bite" : _weight_per_bite,
             "eat_object" : _eat_object,
             "eat_func" : _eat_func,
             "eat_mess" : _eat_mess,
             "splashable" : _splashable,
             "applicable" : _applicable,
             "external_pk_check" : _external_pk_check ]);
}                               /* int_query_static_auto_load() */

/**
 * @ignore yes
 */
mapping query_static_auto_load()
{
   if ((explode(file_name(this_object()), "#")[0] != "/obj/food")
       && !query_continuous()) {
      return 0;
   }
   return int_query_static_auto_load();
}                               /* query_static_auto_load() */

/**
 * @ignore yes
 */
void init_static_arg(mapping map)
{
   if (!mapp(map)) {
      return;
   }
   if (map["::"]) {
      ::init_static_arg(map["::"]);
   }
   _liquid = map["liquid"];

   _weight_per_bite = map["weight_per_bite"];
   _eat_object = map["eat_object"];
   _eat_func = map["eat_func"];
   _eat_mess = map["eat_mess"];
   _splashable = map["splashable"];
   _applicable = map["applicable"];
   _external_pk_check = map["external_pk_check"];
}                               /* init_static_arg() */

/**
 * @ignore yes
 */
mapping query_dynamic_auto_load()
{
   return ([ "::" : ::query_dynamic_auto_load(),
             "bites_gone" : _bites_gone,
             "eat_effects" : _eat_effects,
             "external_effects" : _external_effects,
             "in pieces" : _in_pieces,
             "piece desc" : _piece_desc,
             "piece short" : _piece_short,
             "piece plural" : _piece_plural,
             "piece substance" : _piece_substance,
             "decay speed" : _decay_speed,
             "decay level" : _decay_level,
             "dried" : _dried,
             "cured" : _cured,
             "divisor" : _divisor, ]);
}                               /* query_dynamic_auto_load() */

/**
 * @ignore yes
 */
void init_dynamic_arg(mapping map, object)
{
   if (!mapp(map)) {
      return;
   }
   if (map["::"]) {
      ::init_dynamic_arg(map["::"]);
   }
   _bites_gone = map["bites_gone"];
   if (undefinedp(_eat_effects)) {
      _eat_effects = ([ ]);
   } else {
      if (mapp(map["eat_effects"])) {
         _eat_effects = map["eat_effects"];
      }
   }
   if (undefinedp(_external_effects)) {
      _external_effects = ([ ]);
   } else {
      _external_effects = map["external_effects"];
   }
   _in_pieces = map["in pieces"];
   _piece_desc = map["piece desc"];
   _piece_short = map["piece short"];
   _piece_plural = map["piece plural"];
   _piece_substance = map["piece substance"];
   if (_in_pieces && _piece_desc) {
      set_long(_piece_desc);
   }
   set_decay_speed(map["decay speed"]);
   _decay_level = map["decay level"];
   _divisor = map["divisor"];
   _dried = map["dried"];
   _cured = map["cured"];
   
   init_virtual_object();
}                               /* init_dynamic_arg() */

/**
 * @ignore yes
 */
mixed *stats()
{
   int i;
   string *words;
   mixed *args;
   args =::stats() + ({ ({ "liquid", _liquid }),
                        ({ "bites gone", _bites_gone }),
                        ({ "weight per bite", _weight_per_bite }),
                        ({ "eat object", _eat_object }),
                        ({ "eat function", _eat_func }),
                        ({ "eat message", _eat_mess }),
                        ({ "in pieces", _in_pieces }),
                        ({ "decay speed", _decay_speed }),
                        ({ "decay level", _decay_level }),
                        ({ "dried", _dried }),
                        ({ "cured", _cured }) });
   words = m_indices(_eat_effects);
   for (i = 0; i < sizeof(words); i++) {
      args += ({ ({ "eat effect#" + i, words[i] + " (" +
                    _eat_effects[words[i]] + ")" }) });
   }
   args += ({ ({ "applicable", _applicable }) });
   args += ({ ({ "splashable", _splashable }) });
   if (_splashable && !_liquid) {
      args += ({ ({ "***not splashable***", "because not a liquid" }) });
   }
   args += ({ ({ "external_pk_check", _external_pk_check }) });
   if (_external_effects) {
      words = keys(_external_effects);
      for (i = 0; i < sizeof(words); i++) {
         args += ({ ({ "external effect#" + i, words[i] + " (" +
                       _external_effects[words[i]] + ")" }) });
      }
   }
   return args;
}                               /* stats() */

/**
 * This checks the outside of us for a container and then destroys ourselves
 * if it is not a container.
 */
int check_for_container()
{
   if (query_liquid()) {
      if (!environment()->query_max_volume() || living(environment())) {
         return 0;
      }
   }
   return 1;
}                               /* check_for_container() */

private void move_check_for_container()
{
   if (!check_for_container()) {
      //
      // Opps, not a container.  Destroy ourselves.
      //
      tell_room(environment(),
                the_short() + " dribbles all over the place and disappears "
                "into the dust.\n");
      if(move("/room/rubbish") != MOVE_OK) {
        debug_printf("Oops, failed to move us to the rubbish room.\n");
      }
   }
}                               /* check_for_container() */

/** Check to see if the thing outside us can handle liquids...
 * @ignore yes
 */
varargs int move(mixed dest, string mess1, string mess2) {
   int ret;

   ret =::move(dest, mess1, mess2);
   if (ret == MOVE_OK && dest != "/room/rubbish" && query_liquid()) {
      move_check_for_container();
   }
   return ret;
}                               /* move() */

/** This tells us if the food has started to rot
 *
 * @return Return 1 if the food has started to rot, 0 otherwise.
 *
 * @see set_decay_speed()
 * @see query_decay_speed()
 * @see set_decay_level()
 * @see query_decay_level()
 * @see query_decay_divisor()
 */ 
int query_rotten() { 
    if ( _divisor ) { 
        return to_int( _decay_level / _divisor ) > 1; 
    }

    return 0; 
} /* query_rotten() */

/** Add the "rotten" adjective if appropriate
 * @ignore yes
 */
 
string *query_adjectives() {
  if( query_rotten() ) {
    return ::query_adjectives() + ({ "rotten" });
  }
  return ::query_adjectives();
}

/** This tells us the current divisor used to determine decay.  This can
 *  be modified with set_decay_speed.
 * 
 * @return float the divisor
 * @see set_decay_speed()
 * @see query_decay_speed()
 * @see set_decay_level()
 * @see query_decay_level()
 */
 
float query_decay_divisor() { return _divisor; }

/**
 * This is called by the pickling stick.  It stops the food
 * object decaying.
 *
 * @see set_decay_speed()
 * @return returns 1 for suceess and 0 for failure.
 */
int do_cure() {
  if(_dried || _liquid || _cured)
    return 0;

  _decay_speed = 0;
  _decay_level = 0;
  BITS_CONTROLLER->remove_bit(this_object());
  set_main_plural( "cured " + query_plural() );
  set_short( "cured " + query_short());
  add_adjective("cured");
  remove_adjective("fresh");
  _cured = 1;

  if( query_collective() ) {
    merge();
  }

  return 1;
}

/**
 * This returns the cured state of the bit.
 *
 * @return 1 if it is cured, 0 if not
 */
int query_cured() { return _cured; }

/**
 * This method dries the food.  Dried food no longer decays.
 *
 * @see set_decay_speed()
 * @return returns 1 for suceess and 0 for failure.
 */
int do_dry() {
  int new_weight;

  if(_dried || _liquid || _cured)
    return 0;

  _decay_speed = 0;
  _decay_level = 0;
  BITS_CONTROLLER->remove_bit( this_object() );
  set_main_plural("dried " + query_plural(0));
  set_short( "dried " + query_short());
  add_adjective("dried");
  remove_adjective("fresh");

  new_weight = ::query_weight() / 10;

  if ( new_weight ) { 
    set_weight( new_weight );
  }
  else {
    set_weight( 1 );
  }

  if( query_collective() ) {
    merge();
  }

  _dried = 1;
  return 1;
}

/**
 * This returns the dried state of the bit.
 *
 * @return 1 if it is dried, 0 if not
 */
int query_dried() { return _dried; }

/** @ignore yes.
 *
 * Define some food-specific merge criteria for collective food objects.
 */
int merge_criteria(object ob) {
  return ::merge_criteria(ob) &&
    _decay_speed == ob->query_decay_speed() &&
    _bites_gone == ob->query_bites_gone() &&
    ((to_int(_decay_level / _divisor) < 2 &&
     to_int(ob->query_decay_level() / ob->query_decay_divisor()) < 2) ||
     to_int(_decay_level / _divisor) ==
     to_int(ob->query_decay_level() / ob->query_decay_divisor()));
}
