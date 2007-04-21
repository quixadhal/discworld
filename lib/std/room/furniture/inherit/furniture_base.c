/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: furniture_base.c,v 1.23 2003/06/25 19:51:42 ceres Exp $
 *
 *
 */
/**
 * This is a furniture item for making all kinds of furniture.
 * @author Ceres
 * @changes Pinkfish Tue May 15 13:06:52 PDT 2001
 * Made is so that furniture can be bonded to someone so only that person
 * can move it and touch it.
 */
#include <move_failures.h>
#include <position.h>
#include <housing.h>
#include <room/placement.h>

//
// Predefintions.
//
void add_property(string name, mixed type);
mixed query_property(string name);
string one_short();
int query_light_needs_inform();
void inform_of_light_level_change(object ob);
//void set_max_cond(int);
//void set_cond(int);

private nosave mapping _allowed_room_verbs;
private nosave string *_allowed_positions;
private nosave string _bonded;

void create() {
  _allowed_positions = ({ });
  if (base_name(this_object()) + ".c" != __FILE__) {
    if ( !query_property( "shop type" ) ) {
      add_property( "shop type", "furniture" );
    }
    this_object()->set_max_cond(100000);
    this_object()->set_cond(100000);
  }
  if(clonep(this_object()) &&
     base_name(this_object())[0..14] != "/obj/furnitures" &&
     base_name(this_object())[0..2] != "/w/" && 
     strsrch (base_name(this_object()), "_dev") == -1 && 
     strsrch (base_name(this_object()), "_pt") == -1) {
    call_out("move", 2, "/room/void");
  }
}

/**
 * This method will return true if the item is an item of furniture.
 * @return always returns true
 */
int query_furniture() {
   return 1;
}

/**
 * Used to define what positions (sitting, lying etc.) are allowed for this
 * piece of furniture.
 * @param positions an array of strings.
 *
 * @see position.h
 */
void set_allowed_positions(string *positions) {
   _allowed_positions = positions;
}

/**
 * Removes the positions given to this function as the argument,
 * from the list of allowed positions for this piece of furniture
 */
void remove_allowed_positions( string *positions ){
  _allowed_positions -= positions;
}

/**
 * Returns the set of allowed positions for the furniture object.
 * @return the set of allowed positions
 */
string* query_allowed_positions() {
   return _allowed_positions;
}

/**
 * Set the allowed location verbs for this object.
 * @param allowed the allow mapping
 */
void set_allowed_room_verbs( mapping allowed ) {
  _allowed_room_verbs = allowed;
}

/**
 * This method returns the location verbs for this object.
 * @return the allowed mapping
 */
mapping query_allowed_room_verbs() {
   return _allowed_room_verbs;
}

/** @ignore yes */
string query_position_string(string position_type) {
  if(member_array(position_type, _allowed_positions) != -1) {
    return one_short();
  }
  return 0;
}

/**
 * This method sets the furniture as bonded to a specified person.
 * @param person the person to bond the furniture to
 */
void set_bonded(string person) {
   _bonded = person;
}

/**
 * This method returns the person who the furniture is currently
 * bonded to.
 * @return the person the furniture is bonded to
 */
string query_bonded() {
   return _bonded;
}

/**
 * Returns the person who this item is bonded to.
 * @return the person the furniture is bonded to
 */
string query_owner() {
   return _bonded;
}

/**
 * This method checks to see if the specified person can mess with this
 * furniture.
 * @param person the person to check
 */
int is_allowed_to_mess(object player) {
  mixed *stuff;
  string occupier;
  
  if(environment() && !function_exists("query_owner", environment()))
    return 1;
  if(player->query_creator())
    return 1;
  
  // Do the checks on the controller of a controlled monster
  // (dustdevil, fruitbat, portage cloud etc. etc.)
  if(player->query_caster())
    player = find_player(player->query_caster());
  if(player->query_owner())
    player = player->query_owner();

  // It's bonded to this person so they can move it.
  if (player && player->query_name() == _bonded)
    return 1;

  // If this person dropped it they can move it.
  stuff = this_object()->query_property("dropped");
  if(player && sizeof(stuff) && stuff[0] == player->query_name())
    return 1;

  if(player && environment()) {
    // If they control this room they can move it.
    if(environment()->test_occupier(player->query_name()))
      return 1;
    if(environment()->query_owner() == "For Sale" ||
       environment()->query_owner() == "Under Offer")
      return 1;
    
    occupier = HOUSING->query_owner(base_name(environment()));
    if(pk_check(player, occupier, 1))
      return 0;
  }
  
  // Otherwise succeed if not bonded, fail if bonded.
  return (!_bonded);
}

/**
 * This makes a move check to see if the person is allowed to move
 * the item or not.
 * @param dest where we are going to
 * @return 1 if can be moved, 0 otherwise
 */
int move_check(object dest) {
  if (living(dest)) {
    if (!is_allowed_to_mess(dest)) {
       return 0;
    }
  } else if (this_player() && environment()) {
    // Allow people to drop items that have managed to get that are bonded.
    if (!(living(environment()) && dest == environment(environment()))) {
       // Otherwise check this player, allow the initial setup move though.
       if (!is_allowed_to_mess(this_player())) {
          return 0;
       }
    }
  }
  return 1;
}

/**
 * This method returns the current verb the furniture object is using.  The
 * verb is something like 'sitting' or 'standing'.
 * @return the current verb
 */
string query_placement_verb() {
   return query_property(PLACEMENT_CURRENT_ROOM_VERB);
}

/**
 * This method returns the current other the furniture object is using.  The
 * other is somethign like 'against', 'beside'.
 * @return the current other
 */
string query_placement_other() {
   return query_property(PLACEMENT_CURRENT_ROOM_OTHER);
}

/**
 * This method returns the current position the furniture object is using.
 * The position is something like 'wall', 'ceiling' etc.
 * @return the current position
 */
string query_placement_position() {
   return query_property(PLACEMENT_CURRENT_ROOM_POSITION);
}

/**
 * This method returns if the furniture object can be moved or not.
 * @return 1 if it is immovable
 */
int query_placement_immovable() {
   return query_property(PLACEMENT_IMMOVABLE);
}

/**
 * This method checks to see if the furniture is placed or not.
 * @return 1 if placed, 0 if not
 */
int is_placed() {
   return query_placement_position() &&
     stringp(query_placement_verb()) &&
     stringp(query_placement_other());
}

/**
 * This method sets the current verb the furniture object is using.  The
 * verb is something like 'sitting' or 'standing'.
 * @param verb the new verb
 */
void set_placement_verb(string verb) {
   add_property(PLACEMENT_CURRENT_ROOM_VERB, verb);
}

/**
 * This method sets the current other the furniture object is using.  The
 * other is somethign like 'against', 'beside'.
 * @param other the new other
 */
void set_placement_other(string other) {
   add_property(PLACEMENT_CURRENT_ROOM_OTHER, other);
}

/**
 * This method sets the current position the furniture object is using.
 * The position is something like 'wall', 'ceiling' etc.
 * @param position the new position
 */
void set_placement_position(string position) {
   add_property(PLACEMENT_CURRENT_ROOM_POSITION, position);
}

/**
 * This method returns if the furniture object can be moved or not.
 * @return 1 if it is immovable
 */
int set_placement_immovable() {
   return query_property(PLACEMENT_IMMOVABLE);
}
