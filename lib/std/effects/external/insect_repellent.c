/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: insect_repellent.c,v 1.1 2001/11/16 09:09:10 siel Exp $
 *
 *
 */

#include "effect.h"

string query_classification() {
  return "herb.insect.repellent";
}

int beginning(object them, int power, int id) {
  if (power <= 0) {
    them->submit_ee(0, 1, EE_REMOVE);
    return 0;
  }
  them->add_property("insect", power);
  them->submit_ee("start_mess", 2, EE_ONCE);
  them->submit_ee("smell", ({30, 60}), EE_CONTINUOUS);
  return power;
}

int merge_effect(object them, int power, int adjust) {
  power += adjust;
  if (power <= 0) {
    them->submit_ee(0, 1, EE_REMOVE);
    return 0;
  }
  them->submit_ee(0, -1, EE_REMOVE);
  return power;
}

void start_mess(object them, int power, int id) {
  string start_mess;
  switch (power) {
  case 1:
    start_mess = "You smell rather pleasant, much as you might wish to for a "
           "date.";
    break;
  case 2..3:
    start_mess = "A pungent smell envelops you.";
    break;
  default:
    start_mess = "A horribly pungent smell envelops you, sufficient to make "
           "anyone with a sense of smell keep their distance.";
  }
  tell_object(them, start_mess+"\n");
}

void smell(object them, int power, int id) {
  string self_mess, other_mess;
  object env;

  if (--power <= 0) {
    tell_object(them, "The herbal odour around you fades away.\n");
    them->remove_property("insect");
    them->submit_ee(0, 1, EE_REMOVE);
    return 0;
  }
  switch (power) {
  case 1:
    self_mess = "A pleasant smell emanates from your body.";
    other_mess = "A pleasant smell emanates from "+them->one_short()+".";
    break;
  case 2..3:
    self_mess = "A pungent herbal smell emanates from your body.";
    other_mess = "A pungent herbal smell emanates from "+them->one_short()+".";
    break;
  default:
    self_mess = "You reek of pungent herbs.";
    other_mess = "$C$"+them->one_short()+" $V$0=reeks,reek$V$ of pungent "
           "herbs.";
  }
  tell_object(them, self_mess+"\n");
  if ((env = environment(them)) && env->query_property("location")) {
    tell_room(env, other_mess+"\n", them);
  }
}
