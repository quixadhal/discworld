/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: gossip.c,v 1.1 1998/01/06 04:16:50 ceres Exp $
 * $Log: gossip.c,v $
 * Revision 1.1  1998/01/06 04:16:50  ceres
 * Initial revision
 * 
*/
/**
 * This is the effect skelton docs.  This effect
 * has a classification of "npc.gossip".
 * <p>
 * Describe the arguments in here.
 * <p>
 * This effect has a shadow associated with it.
 * @classification npc.gossip
 * @see help::effects
 */
#include <effect.h>
#include "path.h"

/*
 * Gossip effect...  For those gossiping beggers etc.
 *
 * Simple to use.  Add it to the thingy you want to gossip...  It
 * then calls do_gossip_say(str);
 * to do the gossping.  The str is the pattern to use.  The string
 * $name$ is replaced with the name of the person who gossiped
 * and $mess$ is replaced with the message they said.
 *
 * You can do this with the standard chat strings by placing a
 * "#do_gossip_say:$name$ said: $mess$"
 * or whatever in place of a say or emote or whatever...
 *
 * See the beggars in ankh-morpork for an example.
 *
 * The specified arguement is the chance of it getting the gossip from
 * the central gossip pool.  Its one in the given number, so the higher
 * the number the less the chance...
 */
/** @ignore yes */
string query_classification() { return "npc.gossip"; }

/** @ignore yes */
string query_shadow_ob() { return SHADOWS + "gossip"; }
