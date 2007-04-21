/**
 * This contains all the defines for the options command and the options
 * handler.
 * @author Pinkfish
 * @started Thu Jun  8 17:59:30 PDT 2000 
 */
#ifndef __CMDS_OPTIONS_H
/** @ignore yes */
#define __CMDS_OPTIONS_H

/** An integer type. */
#define OPTIONS_TYPE_INTEGER  1
/** A string type. */
#define OPTIONS_TYPE_STRING   2
/** A boolean type. */
#define OPTIONS_TYPE_BOOLEAN  3
/** A brief/verbose type. */
#define OPTIONS_TYPE_BRIEF    4
/** A colour type. */
#define OPTIONS_TYPE_COLOUR   5
/** A dynamic group option.  This will expand out to a bunch more options. */
#define OPTIONS_TYPE_DYNAMIC_GROUP 6
/** A percentage type. */
#define OPTIONS_TYPE_PERCENTAGE 7
/** A terminal type. */
#define OPTIONS_TYPE_TERMINAL   8
/** A normal group. */
#define OPTIONS_TYPE_GROUP 9

/**
 * This is the path to the option handler.
 */
#define OPTION_HANDLER "/obj/handlers/options_handler"

/**
 * The property for determining the include file to exec.
 */
#define OPTION_EXEC_INCLUDE "exec include"

/**
 * The property to use for determining tab stop information.
 */
#define TABSTOP_PROP "more tabstop"

/**
 * The property to use for determining shorthand expansion.
 */
#define SHORTHAND_PROP "expand shorthands"
#define SHORTHAND_OUTPUT_PROP "expand output shorthands"


/** The monitor options */
#define MONITOR_OPTIONS ({ "off", "on", "slow", "snails pace" })

/** Type of option, everyone's option. */
#define OPTIONS_TYPE_ALL 0
/** Type of option, creator only. */
#define OPTIONS_TYPE_CRE_ONLY 1
/** Type of option, lord only. */
#define OPTIONS_TYPE_LORD_ONLY 2
/** Type of option, playtester only. */
#define OPTIONS_TYPE_PLAYTESTER_ONLY 3

#endif
