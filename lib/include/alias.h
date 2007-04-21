/**
 * The defines for all the aliases.
 * @author Pinkfish
 */
#ifndef __SYS__ALIAS
/** @ignore yes */
#define __SYS__ALIAS

/**
 * The mask for the type.
 */
#define ALIAS_MASK 255
/**
 * A new line in the alias stream.
 */
#define NEW_LINE 256
/**
 * This means include all the arguments.
 */
#define ALL_ARGS 512
#define ALL_ARG 1024
#define ARG_THING 2048
#define IFARG_THING 4096
#define ALL_IFARG 8192
#define ONE_ARG 16384
#define TO_ARG 32768
#define FROM_ARG 65536
#define ELSE_THING 131072
#define CURR_LOC (ELSE_THING+256)
#define END_IF (ELSE_THING+512)

/**
 * The alias command object.
 */
#define ALIAS_CMD "/cmds/player/al_ias"
/**
 * The edit alias command object.
 */
#define EDIT_ALIAS_CMD "/cmds/player/ea_lias"
/**
 * The unalias command object.
 */
#define UNALIAS_CMD "/cmds/player/un_alias"

#endif /* __SYS__ALIAS */
