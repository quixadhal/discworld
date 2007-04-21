/**
 * This file contains all the needed defines for the plans.  This is part
 * of the npc inteligence system.
 * @author Pinkfish
 * @started Thu Aug 13 07:35:51 EDT 1998
 */
#ifndef __NPC_PLAN_H__
/** @ignore yes */
#define __NPC_PLAN_H__

/**
 * The directory all the default plans are stored in.
 */
#define PLAN_DIR "/std/npc/plans/"

/**
 * This is the file to use as a base class for all plans.  It defines
 * all the default plan behaviour and makes sure you do not declare your
 * control functions with the wrong arguments.
 */
#define PLAN_BASE_INHERIT (PLAN_DIR "base")

/**
 * This should be returned if the plan is not at all sutabile for the
 * specified situation.
 */
#define PLAN_INVALID_SUTABILITY -1

/**
 * This is the lowest sutability used for a plan.
 */
#define PLAN_LOWEST_SUTABILITY 0

/**
 * This is the low sutability for a plan.
 */
#define PLAN_LOW_SUTABILITY 100

/**
 * This is the medium sutability for a plan.
 */
#define PLAN_MEDIUM_SUTABILITY 200

/**
 * This is the high sutability for a plan.
 */
#define PLAN_HIGH_SUTABILTIY 300

#endif
