#ifndef __SYS_SOUL_H
#define __SYS_SOUL_H

#define PATTERNS 0
#define SINGLE 1
#define TARGET 2

#define NO_ARGUMENTS 0
#define ARGUMENTS 1

#define SELF     0
#define REST     1
#define TARG     2
#define ARGS     3
#define FORCE    4
#define POSITION 5
/* Reuse the TARG for this... */
#define POSITION_SINGLE 2

/* targeted arg size... */
#define ARG_SIZE 6
/* Untargeted arg size... */
/* We don't need force or position for this :) */
#define SMALL_ARG_SIZE 4

#define SOUL_OBJECT "/obj/handlers/new_soul"
#define SOUL_COMPILER "/obj/handlers/soul_compiler"
#define SOUL_DIR "/soul/"

#define ONLY_ARGUMENT 1
#define NO_ARGUMENT 2
#define ARGUMENT_FIRST 3
#define ARGUMENT_SECOND 4
#define ONLY_TARGET 5

#endif /* __SYS_SOUL_H */
