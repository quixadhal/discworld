/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: trap.c,v 1.5 2001/08/27 01:18:52 gototh Exp $
 * 
*/
/**
 * This is a basic trap system for adding traps to container type objects (or
 * anything that uses /std/basic/close_lock.
 *
 * To setup the trap use setup_trap(). Functions are available to find out
 * information about a trap on an object.
 *
 * If you wish to create a more advanced trap you can use the set_lock_trap and
 * set_open_trap functions in /std/basic/close_lock.
 *
 * @author ceres
 * @see /std/basic/close_lock
 * @see setup_trap
 */
#include <tasks.h>

#define SKILL "covert.lockpick.traps"

private nosave int _trap_difficulty;
private nosave string _trap_trigger;
private nosave string _trap_description;
private nosave mixed _trap_message;
private nosave mixed _trap_effect;
private nosave mixed _trap_armed;

/**
 * This sets up a basic trap on a lock.  The trap will be triggered when
 * someone locks/unlocks/picks the lock (depending on trigger). If
 * triggered the message will be displayed and the effect will be added
 * to the player.
 *
 * @param difficulty skill bonus needed to avoid triggering the trap
 * @param trigger "unlock", "pick, "open",  for which activities should
 * trigger the trap.
 * @param description a description of the trap for someone who looks at
   it.
 * @param message The message to be displayed to the player. If it's an array
 * the first index will be printed to the player the second
 * to the room.
 * @param effect the effect to be added to the trigerer. If the effect is an
 * int the players hps will be reduced by that amount. If it is
 * a string that effect will be added to the player, if it's an
 * array the first index will be treated as the effect the rest
 * as parameters to the effect.
 *
 * @example
 * setup_trap(300, "pick",
 *            "Hidden inside the lock is a small ampule which, if touched by "
 *            "an unwary tool would trigger a puff of poisonous gas.",
 *            ({"A small cloud of gas puffs out of the safe.\n",
 *                "A small cloud of gas puffs out of the safe.\n"}),
 *            ({"/std/effects/poisons/poison_gas", 600}));
 */
void setup_trap(int difficulty, string trigger, string description,
                     mixed message, mixed effect) {
  if(!trigger)
    return;
  
  if(trigger == "open")
    this_object()->set_open_trap(this_object(), "basic_trap");
  else
    this_object()->set_lock_trap(this_object(), "basic_trap");

  _trap_difficulty = difficulty;
  _trap_trigger = trigger;
  _trap_description = description;
  _trap_message = message;
  _trap_effect = effect;
}

/**
 * Get data on the basic lock trap. Data is returned as a mixed array of
 * stuff. This is really only used for the autoload. :)
 *
 * @return mixed array of trap data.
 */
mixed *query_trap_data() {
  return ({ _trap_difficulty, _trap_trigger, _trap_description,
              _trap_message, _trap_effect, _trap_armed });
}

/**
 * Find out the difficulty of this trap.
 *
 * @return int difficulty
 */
int query_trap_difficulty() { return _trap_difficulty; }

/**
 * Get the trigger condition for this trap.
 *
 * @return string the trigger condition.
 */
string query_trap_trigger() { return _trap_trigger; }

/**
 * Get the trap description text.
 *
 * @return string description of the trap.
 */
string query_trap_description() { return _trap_description; }

/**
 * Get the trap trigger messages
 *
 * @return mixed a string or array of messages
 */
mixed query_trap_message() { return _trap_message; }

/**
 * Get the trap effects.
 *
 * @return mixed either an int (for the damage) or a mixed variable for
 *   the effect to add to the triggerer.
 */
mixed query_trap_effect() { return _trap_effect; }

/**
 * Set the trap as armed or disarmed.
 */
void set_trap_armed(int i) { _trap_armed = i; }

/**
 * Get whether the trap is armed or not.
 */
int query_trap_armed() { return _trap_armed; }

/**
 * @ignore yes
 *
 * Internal function called to determine if the trap is triggered.
 */
int basic_trap(string action) {
  if(!_trap_armed || action != _trap_trigger)
    return 1;
  
  switch(TASKER->perform_task(this_player(), SKILL, _trap_difficulty,
                              TM_FREE)) {
  case AWARD:
    write("%^YELLOW%^You feel you have learned something about avoiding "
          "traps.%^RESET%^\n");
  case SUCCEED:
    return 1;
  default:
    if(arrayp(_trap_message)) {
      write(_trap_message[0]);
      say(_trap_message[1], this_player());
    } else
      write(_trap_message);
    
    if(intp(_trap_effect)) {
      if(_trap_effect > this_player()->query_hp())
        this_player()->do_death();
      else
        this_player()->adjust_hp(-(_trap_effect));
    } else if(arrayp(_trap_effect)) {
      if(sizeof(_trap_effect) == 2)
        this_player()->add_effect(_trap_effect[0], _trap_effect[1]);
      else
        this_player()->add_effect(_trap_effect[0], _trap_effect[1..]);
    } else
      this_player()->add_effect(_trap_effect);
    return 0;
  }
}
