/**
 *  Playtester header file
 *  @author Drakkos
 *  @started 23/07/2002
 *
 */
 
#ifndef __SYS__PLAYTESTER
#define __SYS__PLAYTESTER

// Domain define
#define PLAYTESTERS               "/d/playtesters/"


// General defines
#define PT_LOGS                   "/log/secure/playtesters"
#define PT_SAVE                   "/save/playtesters"
#define PLAYTESTER_HAND           "/obj/handlers/playtesters"


// Playtester domain defines
#define PLAYTESTER_CHARS          PLAYTESTERS + "chars/"
#define PLAYTESTER_DOCS           PLAYTESTERS + "doc/"
#define PLAYTESTER_ITEMS          PLAYTESTERS + "items/"
#define PLAYTESTER_PALACE         PLAYTESTERS + "palace_rooms/"
#define PLAYTESTER_INHERITS       PLAYTESTERS + "inherits/"
#define PLAYTESTER_SECURE         PLAYTESTERS + "secure/"
#define PLAYTESTER_HANDLERS       PLAYTESTERS + "handlers/"

// Doc Defines

#define PLAYTESTER_HELP           PLAYTESTER_DOCS + "help_dir/"

// Palace defines

#define PLAYTESTER_ARMOURY        (PLAYTESTER_PALACE + "armoury")
#define PLAYTESTER_ARENA          PLAYTESTER_PALACE + "arena/"
#define PLAYTESTER_COURSE         PLAYTESTER_PALACE + "course/"


// General domain object defines

#define PT_OBSTACLE_COURSE        (PLAYTESTER_INHERITS + "obstacle_course")
#define PLAYTESTER_INTERVIEW      (PLAYTESTER_PALACE + "interview")
#define PT_APPLICATION_HANDLER    (PLAYTESTER_HANDLERS + "applications")

#endif
