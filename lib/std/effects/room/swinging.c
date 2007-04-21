/* 28 NOV 00 - Awful's swinging effect.  A very simple little
 * effect, useful for playground swings.
 * Allow for a simple "stop" swinging as well as being able to
 * "jump" from the swing in the traditional playground daredevil
 * method.
 */

/*                        72 column spacing                           */

#include <effect.h>

#define SWINGS "/d/sur/Sto_Lat/hill_street/playground/playground04"

string query_classification() { return "other.swinging"; }


void beginning(object player, int arg) {
  int st, tt;
  player->submit_ee("swinging", ({15, 30}), EE_CONTINUOUS);
  player->submit_ee( 0, arg, EE_REMOVE );
  tt = (int)player->expected_tt();
  st = (tt - 15);
  player->submit_ee("slowing", (st), EE_ONCE);

  if (tt < 61) {
    player->add_succeeded_mess(previous_object(), ({
      "You push off with your feet and start to swing back and "
        "forth.\n",
      "$N push$s off with $p feet and start$s to swing back and "
        "forth.\n"}));
    player->set_position("swinging back and forth");
    player->set_position_type("on");
  }
} /* void beginning(object player, mixed arg) */


void merge_effect(object player, int old_arg, int new_arg) {
  int tt, st;
  tt = (int)player->expected_tt();
  tell_room(environment(player), capitalize(player->a_short() ) +
    " leans back and pulls on the swing to gain momentum.\n", player);
  tell_object(player, "You lean back and pull on the swing "
    "to gain momentum.\n");
  player->submit_ee(0, new_arg + tt, EE_REMOVE);
  st = (tt -15);
  player->submit_ee("slowing", (st), EE_ONCE);
}


void end(object player) {
  if(file_name(environment(player)) != SWINGS)
      return;
  tell_object(player, "You stop swinging.\n");
  tell_room(environment(player), capitalize(player->a_short()) +
    " stops swinging.\n", player);
  player->set_position("sitting");
  player->set_position_type("in");
} /* void end(object player) */


void swinging(object player) {
   if(file_name(environment(player)) != SWINGS){
      player->submit_ee(0, 0, EE_REMOVE);
      return;
   }
   
  switch (random(3)) {
  case 1 :
    if (player->query_gender() == 2) {
      tell_room(environment(player), capitalize(player->a_short() ) +
        " seems very happy as she reaches the bottom of her "
        "arc on the swing.\n", player);
      tell_object(player, "You feel a pleasant tickling sensation "
        "in your stomach as you reach the bottom of your arc on "
        "the swing.\n");
    }
    else {
      tell_room(environment(player), capitalize(player->a_short() ) +
        " swings back and forth happily, just like a child again.\n",
        player);
      tell_object(player, "You swing back and forth happily, just "
        "like a child again.\n");
    }
  break;
  case 2 :
    tell_room(environment(player), capitalize(player->a_short() ) +
      " looks at if "+player->query_pronoun()+" could just "
      "fly right off into the sky as "+player->query_pronoun()+
      " reaches the top of "+player->query_possessive()+" arc on "
      "the swing.\n", player);
    tell_object(player, "You feel as if you could just fly right "
      "off into the sky as you reach the top of your arc on the "
      "swing.\n");
  break;
  case 3 :
    tell_room(environment(player), capitalize(player->a_short() ) +
      " arcs through the air like a pendlum.\n", player);
    tell_object(player, "You arc through the air like a "
      "pendlum.\n");
  break;
  default :
    tell_room(environment(player), capitalize(player->a_short() ) +
      " swings back and forth.\n", player);
    tell_object(player, "You swing back and forth.\n");
  }
} /* void swinging(object player) */


void slowing(object player) {
  if ( (int)player->expected_tt() < 20 ) {
    tell_object(player, "Your swinging is slowing down as you "
      "gradually lose momentum.\n");
    tell_room(environment(player), capitalize(player->a_short()) +
      " slows down a bit as "+player->query_pronoun()+" loses "
      "momentum.\n", player);
  }
}  /* void slowing(object player) */