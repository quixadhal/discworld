#ifndef __SYS__LIVING
/** @ignore yes */
#define __SYS__LIVING

/** The property used to determing if someone is passed out. */
#define PASSED_OUT "passed out"
#define TRANCE "trance"
#define UNKNOWN_MOVE "unknown move"
#define RUNNING_MOVE "running move"
#define MAX_INVEN 10
#define MAX_CREATOR_INVEN 25
#define WILL_POWER 338
#define MIN_TIMES 20
#define GP_ADVANCE_MOD 5

#define OUT_COLD 1
#define ASLEEP 2

/**
 * This is the class used for nationality data.
 * @member nationality the path to the nationality object
 * @member region the region specialisation for the nationality
 * @member data any extra data associated with the nationality
 */
class living_nationality {
   string nationality;
   string region;
   mixed data;
}

#endif /* __SYS__LIVING */
