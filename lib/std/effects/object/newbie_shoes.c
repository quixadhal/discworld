/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: newbie_shoes.c,v 1.2 1998/10/27 23:27:30 sin Exp $
 * $Log: newbie_shoes.c,v $
 * Revision 1.2  1998/10/27 23:27:30  sin
 * Silenced the shoes on net dead players.
 *
 * Revision 1.1  1998/01/06 04:19:27  ceres
 * Initial revision
 * 
*/
/*
 * Effect for the evil orthopaedic shoes.
 * by Sin
 */
#include "path.h"

#include <effect.h>

#define CLASS "misc.worn.shoes.orthopaedic"

string query_classification() { return CLASS; }
int query_indefinite() { return 1; }
string query_shadow_ob() { return SHADOWS + "newbie_shoes"; }

void chat(object player, mixed arg, int id)
{
  string *mechats = ({
    "Something's crawling inside the shoes.\n",
    "The Cobblers of Sek must've made these things!\n",
    "Did that vice just screw down tighter?\n",
    "*OUCH* that stung!\n",
    "You not-so-idly wonder who came up with these shoes.\n"
    });
  string *youchats = ({
    " winces in pain.\n",
    " mutters something about shoes.\n",
    " stumbles.\n"
    });

  if (!interactive(player)) return;

  tell_object(player, mechats[random(sizeof(mechats))]);
  tell_room(environment(player),
    player->query_cap_name() + youchats[random(sizeof(youchats))],
    ({ player }) );
}

void notify(object player, mixed arg, int id)
{
  tell_object(player,
    "A fiery pain shoots up your legs from your feet.\n");
}

void beginning( object player, int amount, int id )
{
  player->add_extra_look( this_object() );
  player->adjust_bonus_dex(-1);
  player->submit_ee("chat", ({ 10, 30 }), EE_CONTINUOUS);
  player->submit_ee("notify", 1, EE_ONCE);
} /* beginning() */

void restart( object player, int amount, int id )
{
  beginning(player, amount, id);
} /* restart() */

void merge_effect(object player, mixed oldarg, mixed newarg, int id)
{
  return;
}

void end( object player, int amount, int id )
{
  player->remove_extra_look( this_object() );
  player->adjust_bonus_dex(1);
  tell_object(player, "Ahhhh!  Free from those evil shoes!\n");
} /* end() */

string extra_look( object player )
{
  int *enums;
  enums = (int *)player->effects_matching( CLASS );
  if ( !sizeof( enums ) )
     return "";
  return(capitalize(player->query_pronoun()) +
    " looks embarrassed and uncomfortable.\n");
} /* extra_look() */
