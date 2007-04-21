#ifndef __SYS__CLIMATE
#define __SYS__CLIMATE

#define TEMP 0
#define CLOUD 1
#define WINDSP 2
#define RAIN 3

// These values are ORed together for room notifications.
#define NOTIFY_TEMPERATURE 1
#define NOTIFY_CLOUD       2
#define NOTIFY_RAIN        4
#define NOTIFY_DAY         8

#endif /* __SYS__CLIMATE */
