/**
 * This contains the definitions of the situation class
 * and the when defines for situations.
 * The when defines consist of (1>>hour) combined with or
 * where hour is an hour of the day during which a 
 * situation may occur (25 hour clock, AM time).
 * For example, WHEN_WEE_HOURS allows the situation
 * to occur between 12 midnite and 4 am.
 */

class situation { 
  string start_mess;
  string end_mess;
  string extra_look;
  function start_func;
  function end_func;
  int *chat_rate;
  string *chats;
  mixed *add_items;
  mixed *random_words;
}

#define WHEN_WEE_HOURS     ((1<<0) | (1<<1) | (1<<2) | (1<<3))
#define WHEN_EARLY_MORNING ((1<<4) | (1<<5) | (1<<6))
#define WHEN_LATE_MORNING  ((1<<7) | (1<<8) | (1<<9) | (1<<10) | (1<<11))
#define WHEN_AFTERNOON     ((1<<12) | (1<<13) | (1<<14) | (1<<15) | (1<<16))
#define WHEN_EVENING       ((1<<17) | (1<<18) | (1<<19))
#define WHEN_LATENIGHT     ((1<<20) | (1<<21) | (1<<22) | (1<<23))

#define WHEN_MIDDAY        ((1<<11) | (1<<12))
#define WHEN_MORNING       (WHEN_EARLY_MORNING | WHEN_LATE_MORNING)

#define WHEN_NIGHT         (WHEN_LATENIGHT | WHEN_WEE_HOURS)
#define WHEN_DAY           (WHEN_MORNING | WHEN_AFTERNOON)

#define WHEN_ANY_TIME      ((1<<24)-1)
