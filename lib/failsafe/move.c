#include "move_failures.h"
#define DROP 1
#define GET 2
int move_flag;

varargs int move(mixed dest, mixed messin, mixed messout) {
  if (!dest)
    return MOVE_EMPTY_DEST;
  if (environment() && !environment()->test_remove(this_object(),
                                                  move_flag & DROP))
    return MOVE_NO_DROP;
  if (!dest->test_add(this_object(), move_flag & GET))
    return MOVE_NO_GET;
  if (environment() && messout)
    say(messout);
  move_object(this_object(), dest);
  if (messin)
    say(messin);
  return MOVE_OK;
}

/* default to being gettable and dropable...
 * modify as you will...
 */
reset_get() { move_flag |= GET; }
set_get() { move_flag &= ~GET; }
reset_drop() { move_flag |= DROP; }
set_drop() { move_flag &= ~DROP; }

void dest_me() {
  efun::destruct(this_object());
}

/* Do not I repeat do not mask this function.
 * is used when all else fails to dest the object...
 * If we find you masking this function... We will cut you up
 * into little bits... slowly
 */
mixed dwep() {
  efun::destruct(this_object());
  return "Destruct With Extreme Prejuce";
}

drop() { return move_flag & DROP; }
get() { return move_flag & GET; }
