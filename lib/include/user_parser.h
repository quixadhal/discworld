#ifndef __SYS_PARSER_H
#define __SYS_PARSER_H

/* Position of things in the commands array */
#define PATTERN_WEIGHT 0
#define PATTERN_STRING 1
#define OLD_PATTERN_STRING 2
#define OBJS 3

/* Type... */
#define WORD_LIST 1
#define STRING 2
#define OPTIONAL 3
#define DIRECT_OBJECT 4
#define INDIRECT_OBJECT 5
#define NUMBER 6
#define LIVING 7
#define SINGLE_WORD 8
#define DISTANT_LIVING 9
#define ANY_LIVING 10
#define ANY_OBJECT 11
#define SHORT_STRING 12
#define FRACTION 13
#define WIZ_PRESENT_TARGET 14
#define WORD_LIST_SPACES 15
#define OPTIONAL_SPACES 16
#define TARGET_PLAYER 17
#define QUOTED_STRING 18

#define PATTERN ({ "","","<string> ","[","<direct> ","<indirect> ", \
                   "<number> ", "","<word> ","","","","<string:short> ", \
                   "<fraction> ", "", "", "[", "", "<string:quoted>" })

/* Defines used when searching the string. */
#define FIND_NONE 0
#define FIND_FIRST 1
#define FIND_LAST 2

/* Environments */
#define ENV_HERE       0
#define ENV_ME         1
#define ENV_HERE_ME    2
#define ENV_DIRECT_OBS 3
#define ENV_ME_HERE    4

#define MASTER "/secure/master"
#define PATTERN_OB "/obj/handlers/pattern_compiler"

#endif /* __SYS_PARSER_H */
