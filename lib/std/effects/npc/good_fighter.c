/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: good_fighter.c,v 1.3 1998/04/11 23:55:16 sin Exp $
 * $Log: good_fighter.c,v $
 * Revision 1.3  1998/04/11 23:55:16  sin
 * Changed documentation to describe USE_UNARMED
 *
 * Revision 1.2  1998/02/06 22:18:18  sin
 * Change in preparation for the skills rearrange.
 *
 * Revision 1.1  1998/01/06 04:16:50  ceres
 * Initial revision
 * 
*/
/**
 * The Good Fighter effect makes the npc fight a lot better.  It has a
 * classification of "npc.fighter".
 *
 * <p>Many NPCs need to be able to fight well.  Generally "fight well"
 * simply means using the special combat commands that their guild
 * offers.  Of course, since the use of those commands requires that an
 * NPC have the requisite skills, the NPC will also have to include
 * logic to set those skills to an appropriate level.  Since many
 * NPCs need to be good fighters, this kind of code is duplicated
 * all over the Disc.
 *
 * <p>This shadow does everything necessary to turn a plain, stupid,
 * NPC into a semi-intelligent fighter.
 *
 * <p>To be more specific, when this shadow is applied to an NPC, it'll
 * do everything necessary to make sure the NPC has the commands
 * appropriate to their level, has the skills to use those commands,
 * and has the intelligence to use the commands in a useful way.
 *
 * <p>Currently, this shadow is only really useful to members of the
 * warriors' guild.  It should be relatively straightforward to
 * generalize this shadow to the needs of other guilds, or to write
 * customized shadows based on this one but tuned for those others.
 * But that is a later project.
 *
 * <p>To use this shadow, you must first set the NPC's guild, race,
 * and level, and their stats (strength, constitution, &c.).  Then
 * add the effect to the NPC.  Defines are provided in good_fighter.h
 * to simplify this.  Generally, this would be done in the setup()
 * function for the NPC, but it can also be done later, in response
 * to some event, or a player's statement, or what have you.  The
 * only thing that is important is that the effect needs to get added
 * after the NPC's stats are set, and before the NPC enters combat.
 *
 * <p>When you add the effect, you need to give an argument.  This
 * argument tunes the effect to one of 15 different variants, depending
 * on how the fighter should use blunt, sharp, and pierce attacks, and
 * how the fighter defends itself.  Defines for the attacks are
 * USE_SHARP, USE_PIERCE, USE_BLUNT, USE_UNARMED, and USE_BALANCED.
 * Defines for the defenses are DEFEND_DODGE, DEFEND_PARRY, and 
 * DEFEND_BALANCED.  As an example, if you want a fighter who uses 
 * piercing attacks, while defending with parry, then use the following 
 * code:
 * <pre>
 *   add_effect(GOOD_FIGHTER, ({ USE_PIERCE, DEFEND_PARRY }) );
 * </pre>
 * Always put the USE_xxx before the DEFEND_xxx!
 *
 * <p>Obviously, it doesn't make a whole lot of since to set an NPC
 * to USE_UNARMED and DEFEND_PARRY unless you also give them a
 * shield.
 *
 * <p>The skills and intelligence of the NPC are driven largely by
 * the NPC's level.  The higher level an NPC is, the more special
 * commands will be available to the NPC, and the more often the NPC
 * will use these commands.
 *
 * <p>The effect applies a shadow to the NPC.  It is in that shadow
 * that nearly all the intelligence resides.
 *
 * @see /std/shadows/fighting/combat
 * @see /std/race->set_guild()
 * @see /std/race->set_level()
 * @see clone_object()
 *
 * @author Sin
 * @created 12 November 1997
 * @changed 13 November 1997 -- Sin
 *     Converted it from a pure shadow to a shadow/effect pair.
 * @changed 6 Feb 1998 -- Sin
 *     Fixed documentation, and modified effect to support the skill
 *     changeover (USE_PIERCE).
 * @changed 11 April 1998 -- Sin
 *     Changed documentation to discuss USE_UNARMED.
 */

#include "path.h"

#include <effect.h>

#define CLASS "npc.fighter"

/** @ignore yes */
string query_classification() { return CLASS; }

/** @ignore yes */
int query_indefinite() { return 1; }

/** @ignore yes */
string query_shadow_ob() { return SHADOWS "good_fighter"; }

/** @ignore yes */
void gfs(object player)
{
  player->good_fighter_setup();
}

/** @ignore yes */
int *beginning(object player, int *arg)
{
  if (!arg || !arrayp(arg) || sizeof(arg) != 2)
    arg = ({ 0, 0 });
  player->submit_ee("gfs", 1, EE_ONCE);
  return arg;
} /* beginning() */

/** @ignore yes */
void restart(object player, int *arg)
{
  beginning(player, arg);
} /* restart() */

/** @ignore yes */
int *merge_effect(object player, int *oldarg, int *newarg)
{
  return oldarg;
} /* merge_effect() */

/** @ignore yes */
void end(object player, int *arg)
{
} /* end() */
