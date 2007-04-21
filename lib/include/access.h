#ifndef __SYS__ACCESS
#define __SYS__ACCESS

#define ERROR     0
#define ACCESS    1
#define NO_NEW    2
#define NO_ACCESS 3
#define AUTH_NEW  4

#define DEFAULT ACCESS

#define ACCESS_LEVEL   0
#define ACCESS_REASON  1
#define ACCESS_TIMEOUT 2

#define MULTI_LEVEL 0
#define MULTI_TIMEOUT 1

#define SUSPEND_TIME   0
#define SUSPEND_REASON 1

#define PERM_NAMES ({ "error", "normal access", "no new characters", \
                      "no access", "new requires authorisation" })
#endif /* __SYS__ACCESS */
