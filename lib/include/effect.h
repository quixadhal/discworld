#ifndef __SYS__EFFECT
#define __SYS__EFFECT

#define REMOVE_THIS_EFFECT 2000000000
#define REMOVE_THIS_EFFECT_NEXT_HB 2000000001
#define CANCEL_THIS_EE 2000000002

#define EE_REMOVE 1
#define EE_CONTINUOUS 2
#define EE_OLD 4
#define EE_CANCELLED 8
#define EE_ONCE 0

#define BOGUS_OLD_ARG 20495621

class effect {
  string ob_name;
  mixed arg;
}

#endif /* __SYS__EFFECT */
