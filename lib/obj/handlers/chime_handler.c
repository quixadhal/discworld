/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: chime_handler.c,v 1.6 2002/03/16 14:56:23 wodan Exp $
 * 
 */
#define CREATOR "Ceres"

/**
 * This handler deals with clocks which chime. It uses a single 15 minute
 * callout to make all clocks on Discworld which choose to chime chime the
 * quarter, half or hours.
 *
 * @author Ceres
 */
#include <am_time.h>
#include <chime.h>

#define FIFTEEN_MINS (AM_SECONDS_PER_MINUTE * 15)

class chime {
  object ob;
  int type;
  string mess;
}

void do_chime(class chime record, int type);

class chime *chime_list;
int last_strike;

/** @ignore yes */
void create() {
  int next_chime;

  chime_list = ({ });

  // time to the next quarter
  next_chime = 301 - (time() % 300);
  remove_call_out("check_time");
  call_out("check_time", next_chime);
}

/**
 * Register a clock to do chimes. The num parameter specifes when to
 * chime.  If it is 1 then the clock chimes every hour, if it is
 * 2 the clock chimes ever half hour, if it is 4 the
 * clock chimes every quarter hour.  Anything else and
 * the clock never chimes.  NB: if you set it to chime the quarter hours
 * it will only chime the quarter hours, it will not chime the hours
 * or the half hours.  You need to call this function three times to
 * get your clock to chime the hour, half and quarter hours.
 * <p>
 * The function "chime" will be called on the clock
 * when the chime occurs.  The message will be passed in as the first
 * argument, the second will be the type of chime (CHIME_HOUR, CHIME_HALF_HOUR,
 * CHIME_QUARTER_HOUR).
 *
 * @param ob the object being registered
 * @param num when to chime
 * @param mess the message to chime with
 *
 * @return 1 for success, 0 for failure.
 */
int register_for_chimes(object ob, int num, string mess) {
  class chime record;

  // check if this object is already registered. If they use a new
  // message it's considered a new chime.
  foreach(record in chime_list) {
    if(record->ob == ob && record->type == num && record->mess == mess) {
      record->type = num;
      record->mess = mess;
      return 1;
    }
  }
  
  record = new(class chime);
  record->ob = ob;
  record->type = num;
  record->mess = mess;
  chime_list += ({ record });
  
  return 1;
}

/** @ignore yes
 *
 * This function checks the registered objects and sets up a call to
 * do_chime() for each one that needs a call.
 */
void check_time() {
  int next_chime, minute;
  class chime record;

  minute = AM_TIME_HANDLER->query_am_minute(time());
  
  foreach(record in chime_list) {
    catch{
      if(minute % 60 < 5)
        do_chime(record, CHIME_HOUR);
      else if(minute % 30 < 5 && record->type > CHIME_HOUR)
        do_chime(record, CHIME_HALF_HOUR);
      else if(minute % 15 < 5 && record->type == CHIME_QUARTER_HOUR)
        do_chime(record, CHIME_QUARTER_HOUR);
    };
  }

  next_chime = (FIFTEEN_MINS + 1) - (time() % FIFTEEN_MINS);
  remove_call_out("check_time");
  call_out("check_time", next_chime);
}

/** @ignore yes
 *
 * This actually calls the chime function in an object.
 */
void do_chime(class chime record, int type) {
  if(!record || !record->ob)
    return;
  (record->ob)->chime(record->mess, type);
}

/** @ignore yes
 * Restart the chime callout, just in case it got dropped on the floor.
 */
void reset() {
  int next_chime;
  class chime record;

  next_chime = (FIFTEEN_MINS + 1) - (time() % FIFTEEN_MINS);
  remove_call_out("check_time");
  call_out("check_time", next_chime);

  // Clean up the chime_list.
  foreach(record in chime_list) {
    if(!record->ob)
      chime_list -= ({ record });
  }
  
}

/** @ignore yes
 * these two are not here because the handler can be saved, but to keep
 * the info over updates.
 */
mapping query_dynamic_auto_load() {
   mapping tmp;
   tmp = ([ "chime_list" : chime_list ]);
   return tmp;
}

/** @ignore yes */
void init_dynamic_arg(mapping maps) {
   if (maps["chime_list"])
      chime_list = maps["chime_list"];
}

/** @ignore yes */
mixed stats() {
  return ({ ({ "chime count", sizeof(chime_list) }) });
}
