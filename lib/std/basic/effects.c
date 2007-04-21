/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: effects.c,v 1.24 2003/07/20 06:59:27 ceres Exp $
 */
/* -*- LPC -*-
 */
/**
 * Basic effects module.
 * Extracted from Ember's living effects code by Deutha.
 * @change 4-6-1997, Turrican
 * to use arrays of classes, and to allow function pointers.
 * @change 17-10-1997 Olorin
 * fix a bug then adding an effect
 * in the end() function of another effect.
 * @see help::effects
 */

#include <effect.h>

class effect_shadow {
   object shad_ob;
   int idnum;
}

class effect_event {
   int inctime;
   mixed func;
   int eff_ob_num;
   mixed interval;
   int flags;
   mixed eff_ev_args;
}

#undef DEBUG
#ifdef DEBUG
nosave int logging_effects;
void set_logging_effects(int i) { logging_effects = i; }
int query_logging_effects() { return logging_effects; }
#endif

/**
 * This class stores all the data for the effects inheritable.
 * @member current_enum used to keep track of the effect being called;
 *         beginning/end etc.
 * @member next_id the next id to assign to an effect
 * @member next_event_time when the next effect is due to go off
 * @member shadows the shaodws on the object
 */
class effect_data {
   int current_enum;
   int next_id;
   int next_event_time;
   class effect_shadow* shadows;
}

void delete_effect(int i);

private nosave class effect_data _data;

private class effect *effs;
/* this stores effects currently active on player as ...
 * "effect_ob_name", arg
 */

private class effect_event *eeq;
/* effect event queue
 *
 *    This stores all effect events going to be happening...
 *
 *    time_from_previous_event, "function", eff_ob_num, interval, flags
 *
 * where flags can be
 *
 *   EE_REMOVE
 *   EE_CONTINUOUS
 *   EE_ONCE
 *
 * and interval can be
 *   number_of_secs
 *   ({ min_secs, max_secs })  ; "1" dice roll
 *   ({ min_secs, max_secs, n })   ; "n" dice rolls
 *
 * ee are added with
 *   submit_ee("function", interval, flags, event_args );
 *
 */

protected void create() {
  effs = ({ });
  eeq = ({ });
  _data = new(class effect_data);
  _data->shadows = ({ });
  _data->next_id = 0;
  _data->current_enum = -1;
  _data->next_event_time = 0;
#ifdef DEBUG
  logging_effects = 0;
#endif
} /* create() */


/**
 * This method removes all the current effects off the object
 * and sets everything back to the initial state.
 * @see add_effect()
 */
void reset_effects() {
   effs = ({ });
   eeq = ({ });
   _data->shadows = ({ });
   _data->current_enum = -1;
   _data->next_event_time = 0;
   remove_call_out("effect_timer");
#ifdef DEBUG
   if ( logging_effects )
      log_file( "EFFECT_WOBBLE",
               "%d:%s(%d) reset_effects:remove_call_out( \"effect_timer\" )\n",
               time() % 1000,
               this_object()->query_name(),
               _data->current_enum);
#endif
} /* reset_effects() */

/**
 * This method returns the outer most shadow on this object.  This is useful
 * for making sure functions get calklked right through the shadow
 * list.  If there are no objects being shadows, this_object()
 * is returned.
 * @return the outermost shadow object
 */
object affected_object() {
   object this, other;

   other = this_object();
   this = other;
   while (objectp(other)) {
      this = other;
      other = query_shadowing(this);
   }
   return this;
} /* affected_object() */

/**
 * This method freezes all the current effects so that they do
 * not continue to be processed.  Basicly it removes the call_out
 * for the effect_timer().  It also ensures the intrval for the
 * immediately pending effect
 * is correct.
 * @see effect_timer()
 * @see add_effect()
 */
void effect_freeze() {
   /* stop effect_timer call_out and put the remaining
    * time value into EEQ_INCTIME of first element */
   remove_call_out( "effect_timer" );
#ifdef DEBUG
   if ( logging_effects )
      log_file( "EFFECT_WOBBLE",
               "%d:%s(%d) effect_freeze:remove_call_out( \"effect_timer\" )=%d\n",
               time() % 1000,this_object()->query_name(),_data->current_enum,timeleft);
#endif
   if ( sizeof( eeq ) && _data->next_event_time) {
      if (_data->next_event_time <= time()) {
         eeq[ 0 ]->inctime = 1;
      } else {
         eeq[ 0 ]->inctime = _data->next_event_time - time(); //timeleft;
      }
   }
} /* effect_freeze() */

/**
 * Called when a player actually saves.  Calls the quitting method on
 * all the effects.
 * @ignore yes
 */
void effects_saving() {
   int i, neffs;
   string effn;
   mixed arg;

   neffs = sizeof( effs );
   for ( i = 0; i < neffs; i++ ) {
      effn = effs[ i ]->ob_name;
      arg = effs[ i ]->arg;
      if ( pointerp( arg ) && ( sizeof( arg ) == 3 ) &&
          ( arg[ 2 ] == BOGUS_OLD_ARG ) ) {
         arg = arg[ 1 ];
      }
      if ( effn ) {
         catch( (void)effn->saving( affected_object(), arg, i ) );
      }
   }
} /* effects_saving() */

/**
 * Called when a player actually quits.  Calls the quitting method on
 * all the effects.
 */
void effects_quiting() {
   int i, neffs;
   string effn;
   mixed arg;

   neffs = sizeof( effs );
   for ( i = 0; i < neffs; i++ ) {
      effn = effs[ i ]->ob_name;
      arg = effs[ i ]->arg;
      if ( pointerp( arg ) && ( sizeof( arg ) == 3 ) &&
          ( arg[ 2 ] == BOGUS_OLD_ARG ) ) {
         arg = arg[ 1 ];
      }
      if ( effn ) {
         catch( (void)effn->quiting( affected_object(), arg, i ) );
      }
   }
} /* effects_quiting() */

/**
 * Called when an object is dested.  Calls the desting method on
 * all the effects.
 */
void effects_desting() {
   int i, neffs;
   string effn;
   mixed arg;

   neffs = sizeof( effs );
   for ( i = 0; i < neffs; i++ ) {
      effn = effs[ i ]->ob_name;
      arg = effs[ i ]->arg;
      if ( pointerp( arg ) && ( sizeof( arg ) == 3 ) &&
          ( arg[ 2 ] == BOGUS_OLD_ARG ) ) {
         arg = arg[ 1 ];
      }
      if ( effn ) {
         catch( (void)effn->desting( affected_object(), arg, i ) );
      }
   }
}

/**
 * This restarts the internal call_out.
 * @see effects_freeze()
 * @see add_effect()
 */
void effect_unfreeze() {
   /* restart the timer, checking that it hasn't been restarted
    * already by a nested unfreeze
    */
   if ( sizeof( eeq ) ) {
#ifdef DEBUG
      if ( logging_effects )
         log_file( "EFFECT_WOBBLE",
                  "%d:%s(%d) effect_unfreeze:call_out( \"effect_timer\", %d)\n",
                  time() % 1000,this_object()->query_name(),_data->current_enum,eeq[ 0 ]->inctime);
#endif
      //time = find_call_out( "effect_timer" );
      //if ( time != -1 )
      remove_call_out( "effect_timer" );
      _data->next_event_time = time() + eeq[ 0 ]->inctime;
      call_out( "effect_timer", eeq[ 0 ]->inctime );
   }
} /* effect_unfreeze() */

/** @ignore yes */
protected int int_submit_ee( int eff_ob_num, mixed fun, mixed
      interval, int flags, mixed event_args ) {
/*
 * this calculates next occurence of this ee and inserts it into the eeq ...
 * note that it can only be called sandwiched between freeze and unfreeze calls
*/
   int i, ntime;

#ifdef DEBUG
   if ( logging_effects )
      log_file( "EFFECT_WOBBLE",
               "%d:%s(%d) int_submit_ee:( %d, %O, %O, %s, %O )\n",
               time() % 1000,this_object()->query_name(),_data->current_enum,
               eff_ob_num, fun, interval,
               ({"once","remove","continuous"})[flags], event_args );
#endif
   if (intp(interval))
      ntime = interval;
   else if (pointerp(interval)) {
      switch (sizeof(interval)) {
       case 0:
         return -1; /* bletch */
       case 1:
         ntime = interval[0];
         break;
       case 2:
         ntime = interval[0] + random(interval[1] - interval[0] + 1);
         break;
       case 3:
         ntime = 0;
         for (i = 0; i < interval[2]; i++) {
            ntime += interval[0] + random(interval[1] - interval[0] + 1);
         }
         ntime += (interval[2]/2);
         ntime /= interval[2];
         break;
       default:
         return -1;
      }
   } else return -1;

   /* if the flag is EE_REMOVE, and one exists for EEQ_EFF_OB already,
    * remove the old one.  This is for occasions when an EE_REMOVE is
    * called from merge_effect() */
   if (flags == EE_REMOVE) {
      for(i = 0; i < sizeof(eeq); i++) {
         if ((eeq[i]->eff_ob_num == eff_ob_num) &&
             (eeq[i]->flags == EE_REMOVE)) {
            eeq = eeq[0..i-1] + eeq[i+1..];
            /*
             for( j = i; j < sizeof( eeq ); j += EEQ_SIZE )
             eeq[j] += eeq[i];
             */
            break;
         }
      }
   }
   /*
    * This traps cases of negative scheduled times and, more
    * importantly, allows the removal of an EE_REMOVE without
    * submitting a new one.
    */
   if ( ntime < 0 ) {
      return 0;
   }

   for (i = 0; i < sizeof(eeq); i++) {
      if (ntime < eeq[i]->inctime) { /* goes in here! */
         eeq[i]->inctime -= ntime;
         eeq = eeq[0..i-1] +
            ({ new(class effect_event, inctime : ntime, func : fun,
                   eff_ob_num : eff_ob_num, interval : interval,
                   flags : flags, eff_ev_args : event_args ) }) + eeq[i..];
         break;
      }
      ntime -= eeq[i]->inctime;
   }
   if ((i == sizeof(eeq)) && eeq) {
      eeq += ({ new(class effect_event, inctime : ntime, func : fun,
                    eff_ob_num : eff_ob_num, interval : interval,
                    flags : flags, eff_ev_args : event_args ) });
   } else if ( !sizeof(eeq) ) {
     /* This shouldn't happen, but it does sometimes!
      * Note from Olorin: without testing for !sizeof(eeq), this
      * code mangles the cases where the queue exists and the event
      * already inserted by the for loop.
      */
      eeq = ({ new(class effect_event, inctime : ntime, func : fun,
                   eff_ob_num : eff_ob_num, interval : interval,
                   flags : flags, eff_ev_args : event_args ) });
   }
   return i;
} /* int_submit_ee() */

/**
 * This submits an effect event schedule.  It will call "function name" in your
 * effect object after the interval specified in interval_spec, with behaviour
 * modified by flags.  Addtional args may optionally be sent to "function name"
 * as well.  If none are passed, args == 0.
 * interval_spec can be one of:
 * <dl><dt>n
 * <dd>time in seconds
 * <dt>({ m, n })
 * <dd>minimum time m seconds, maximum time n seconds, simple random
 *     distribution
 * <dt>({ m, n, r })
 * <dd>as for the ({ m, n }) case, except the random factor is effectively
 *     "rolled" r times ... the higher r is, the more likely the interval
 *     is to be close to (m + n)/2.
 * </dl>
 * flags may be:
 * <dl>
 * <dt>EE_REMOVE
 * <dd> remove the effect from the player aftert the function call
 * <dt>EE_CONTINUOUS
 * <dd>do it repeatedly.  Default (EE_ONCE) is only do it once.
 * <dt>EE_ONCE
 * <dd>do it once
 * </dl>
 * These are defined in /include/effect.h
 * <p>
 * Only one EE_REMOVE can be in place at a time.  Subsequent EE_REMOVEs
 * will wipe previous ones.
 * <p>
 * NB:  submit_ee can ONLY be called from the effect object itself in the
 * course of a beginning/handler/end call, or from the effect shadow.
 * @see submit_ee2()
 * @see /include/effect.h
 * @see add_effect()
 * @param fun the function to call
 * @param interval the interval specifier
 * @param flags the flags for the event
 * @param event_args additional (optional) args passed to fun
 */
varargs void submit_ee(mixed fun, mixed interval, int flags, mixed
    event_args ) {
  /* public access point for int_submit_ee ... it uses current_enum or
     previous_object (for shadows) to determine what effect it refers to */
  int enumb;

  if( undefinedp( event_args ) )
     event_args = 0;

#ifdef DEBUG
   if ( logging_effects )
      log_file( "EFFECT_WOBBLE",
               "%d:%s(%d) submit_ee:( %O, %O, %s, %O )\n",
               time() % 1000,this_object()->query_name(),_data->current_enum,
               fun, interval, ({"once","remove","continuous"})[flags],
               event_args );
#endif
  enumb = _data->current_enum;
  if (enumb < 0) {
    enumb = member_array(1,
        map(_data->shadows, (: $1 && (((class effect_shadow)$1)->shad_ob == previous_object()) :)));
    if (enumb < 0) {
      error("Can not submit from that object");
    }
  }

  effect_freeze();
  int_submit_ee(enumb, fun, interval, flags, event_args );
  effect_unfreeze();
} /* submit_ee() */

/**
 * This submits an effect event for a specific effect.
 * Apart from this it is the same as submit_ee()
 * @param enum the enum of the effecrt
 * @param fun the function to call
 * @param interval the interval specifier
 * @param flags the flagds associated with the event
 * @param event_args additional (optional) args passed to fun
 */
varargs void submit_ee2(int enum, mixed fun, mixed interval, int flags,
    mixed event_args ) {
  /* public access point for int_submit_ee ... it uses enum
   * to determine what effect it refers to (for those cases where
   * current_enum isn't defined) */

  if( undefinedp( event_args ) )
    event_args = 0;

#ifdef DEBUG
   if ( logging_effects )
      log_file( "EFFECT_WOBBLE",
               "%d:%s(%d) int_submit_ee:( %d, %O, %O, %s, %O )\n",
               time() % 1000,this_object()->query_name(),_data->current_enum,
               enum, fun, interval, ({"once","remove","continuous"})[flags],
               event_args );
#endif
  if (enum < 0)
    error("Can't submit from that object");

  effect_freeze();
  int_submit_ee(enum, fun, interval, flags, event_args );
  effect_unfreeze();
} /* submit_ee() */

/**
 * This method returns the time until any EE_REMOVE effect
 * is expected to occur.  It will return --1 if no remove
 * event is scheduled.  If the flag is not
 * set to true, the the current enum is checked, otherwise
 * the new_enum is checked.
 * @param flag itrue, use new_enum not current_enum
 * @return the time until the remove event will occur
 * @see submit_ee()
 * @see add_effect()
 * @see expected_ee()
 * @see all_expected_ees()
 */
int expected_tt(int flag, int new_enum) {
  /* If -1 is returned, no EE_REMOVE for this effect is expected. */

  /* This is a little kludgy, but if flag is true, it uses new_enum
   * instead of current_enum (current_enum is unchanged).  I did it
   * like this for backwards compatibility -- Jeremy, 31-May-96
   */
  int i, timeleft, enum;

  if (flag)
    enum = new_enum;
  else
    enum = _data->current_enum;
  if (enum < 0)
    return -1;
  if (!sizeof(eeq))
    return -1;
  effect_freeze();   /* stop the event timer */
  /* Add up the incremental times in the queue until you get to the
   * EE_REMOVE for the one your asking about */
  for (i = 0; i < sizeof(eeq); i++) {
    timeleft += eeq[i]->inctime;
    if ((eeq[i]->eff_ob_num == enum) &&
        (eeq[i]->flags == EE_REMOVE)) {
      effect_unfreeze();   /* restart the event timer */
      return timeleft;
    }
  }
  effect_unfreeze();  /* restart the event timer */
  return -1;
} /* expected_tt() */

/**
* Returns the time until the next event that will call func_name.
* If enum is undefined, the event is searched for in this effect,
* otherwise the event is searched for in the specified effect.
*
* If no outstanding calls to the function are found, -1 is returned.
*
* @param func_name - the name of the function the ee will call.
* @param enum - the enum of the effect to check in.
* @return time - the seconds until the next call to func_name for
* the effect specified.
* @see all_expected_ees()
* @see expected_tt()
* @see submit_ee()
**/

varargs int expected_ee( string func_name, int enum ) {
  int timeleft = 0;          /* time left until func_name is called */

  /* If enum hasn't been passed, look for the current effect */
  if( undefinedp( enum ) ) {
    enum = _data->current_enum;
  }

  /* If there's still no enum, never mind */
  if( !enum ) {
    return -1;
  }

  /* If the function name isn't valid, don't bother */
  if( !stringp( func_name ) ) {
    return -1;
  }

  /* Ditto if there's no queueue */
  if( !sizeof( eeq ) ) {
    return -1;
  }

  effect_freeze();   /* stop the event timer */

  /* Add up the incremental times in the queue until you get to the
   * function in question for the effect in question. Return the
   * time left. */

  for( int i = 0; i < sizeof( eeq ); i++) {
    timeleft += eeq[i]->inctime;

    if( eeq[i]->eff_ob_num == enum &&
        eeq[i]->func == func_name ) {
      effect_unfreeze();  /* restart the event timer */
      return timeleft;
    }
  }

  /* No event for the give function was found in the given in the
   * given enum. */

  effect_unfreeze();  /* restart the event timer */
  return -1;          /* wasn't found */
} /* expected_ee() */

/**
* Returns the time until the next event that will call func_name.
* If enums is undefined, the event is searched for in this effect,
* otherwise the event is searched for in the specified effect(s).
*
* This returns the a list of the time in seconds until the function
* will be called in the effect(s) specified.  NB: this is not the
* time *between* calls, but the time from Right Now until each call.
* If no outstanding calls to the function are found, an empty array
* is returned.
*
* @param func_name - the name of the function the ee will call.
* @param enum - the enums of the effect to check in.
* @return time - array containing the seconds until each outstanding
* call to func_name for the effect(s) specified.
* @see expected_ee()
* @see expected_tt()
* @see submit_ee()
**/

varargs int *all_expected_ees( string func_name, int *enums ) {
  int timeleft = 0;          /* time left until func_name is called */
  int *alltimesleft = ({});  /* timeleft for all outstanding calls */

  /* If enum hasn't been passed, look for the current effect */
  if( undefinedp( enums ) ) {
    enums = ({ _data->current_enum });
  }

  /* if it's a single int, make it a pointer */
  if( intp( enums ) ) {
    enums = ({ enums });
  }

  /* If there's still no enums, never mind */
  if( !enums ) {
    return ({});
  }

  /* If the function name isn't valid, don't bother */
  if( !stringp( func_name ) ) {
    return ({});
  }

  /* Ditto if there's no queueue */
  if( !sizeof( eeq ) ) {
    return ({});
  }

  effect_freeze();   /* stop the event timer */

  /* Add up the incremental times in the queue until you get to the
   * function in question for the effect(s) in question, add the
   * time to the array, then keep looking for the next one. */

  for( int i = 0; i < sizeof( eeq ); i++) {
    timeleft += eeq[i]->inctime;

    if( member_array( eeq[i]->eff_ob_num , enums ) != -1 &&
        eeq[i]->func == func_name ) {
      alltimesleft += ({ timeleft });
    }
  }
  effect_unfreeze();  /* restart the event timer */
  return alltimesleft;
} /* all_expected_ees() */

/**
 * This method adds an effect onto the object.  RThe arg is passed
 * directly onto the function 'begining' on the effect
 * object.  If the result is non-zero then the return
 * value of the begining function is used instead of the
 * arg and stored away.  If an effect of the same
 * type is already on the object then the function 'merge_effects'
 * will be called.  If it returns a non-zero value then the arg for the
 * initial effect will be updated to the new value, otherwise both effects
 * will run
 * with different arguments.
 * @see help::effects
 * @see submit_ee()
 */
void add_effect(string eff, mixed arg) {
  string shad;
  object ob, effob;
  mixed hbf, res;
  int i;
  int old_current_enum;
  int enum;

#ifdef DEBUG
   if ( logging_effects )
      log_file( "EFFECT_WOBBLE",
               "%d:%s(%d) add_effect:(%s,%O)\n",
               time() % 1000,this_object()->query_name(),current_enum,eff,arg);
#endif
  hbf = (mixed)eff->query_secs_between_hb();
  if (!hbf) {
    hbf = (mixed) eff->query_heart_beat_frequency();
    if (hbf) {
      if (intp(hbf)) hbf *= 60;
    }
  }

  old_current_enum = _data->current_enum;

  effob = find_object(eff);
  if (!effob)
    error("bad effect object");

  for (i=0; i<sizeof(effs); i++) {
    if (effs[i]->ob_name == eff) {
      /*
       * Hmm.  Well. if they have a function on them we join up, otherwise we
       * add it again.
       */
      if (function_exists("merge_effect", effob)) {
        _data->current_enum = i;
        if (hbf) {  /* old fudged system */
          res = (mixed) effob->merge_effect(affected_object(),
                                            effs[i]->arg[1], arg);
          if (res)
            effs[i]->arg[1] = res;
        } else {
          res = (mixed) effob->merge_effect(affected_object(),
                                            effs[i]->arg, arg);
          if (res)
            effs[i]->arg = res;
        }
        _data->current_enum = old_current_enum;
        return ;
      }
    }
  }

  shad = effob->query_shadow_ob();
  if (shad) {
    ob = clone_object(shad);
    if(!ob) {
      error("Failed to clone shadow object " + shad + ".\n");
      _data->shadows += ({ new(class effect_shadow, shad_ob : 0, idnum : _data->next_id) });
    } else {
      _data->shadows += ({ new(class effect_shadow, shad_ob : ob, idnum : _data->next_id) });
      ob->attach_to_player(affected_object(), _data->next_id);
    }
  } else {
    _data->shadows += ({ new(class effect_shadow, shad_ob : 0, idnum : _data->next_id) });
  }
  _data->current_enum = sizeof(effs);
  enum = _data->current_enum;

  if(!effs)
    effs = ({ });
  
  effs += ({ new(class effect, ob_name : eff, arg : arg) });

  if (function_exists("beginning", effob)) {
    res = (mixed) effob->beginning(affected_object(), arg, _data->next_id);
    if (res) {
      effs[enum]->arg = res;
    }
  }

  _data->next_id++;

  if (hbf) {
    submit_ee("effect_heart_beat", hbf, EE_CONTINUOUS | EE_OLD);
    effs[enum]->arg = ({ 0, effs[<1]->arg, BOGUS_OLD_ARG });
  }

  _data->current_enum = old_current_enum;
} /* add_effect() */

#define EFF_OB_NAME 0
#define EFF_ARG 1

#define EEQ_INCTIME 0
#define EEQ_FUNC 1
#define EEQ_EFF_OB 2
#define EEQ_INTERVAL 3
#define EEQ_FLAGS 4
#define EEQ_ARGS 5

#define EFF_SIZE 2
#define EEQ_SIZE 5

/** @ignore yes */
protected void convert_arrays() {
  int i, neffs, neeq;
  mixed *old;

  if (sizeof(effs) && !classp(effs[0])) {
    old = effs;
    neffs = sizeof(old) / EFF_SIZE;
    effs = allocate(neffs);
    for (i = 0; i < neffs; i++)
      effs[i] = new(class effect,
                    ob_name : old[i*EFF_SIZE+EFF_OB_NAME],
                    arg : old[i*EFF_SIZE+EFF_ARG]);
  }
  if (sizeof(eeq) && !classp(eeq[0])) {
    old = eeq;
    neeq = sizeof(eeq) / EEQ_SIZE;
    eeq = allocate(neeq);
    for (i = 0; i < neeq; i++)
      eeq[i] = new(class effect_event,
                   inctime : old[ i * EEQ_SIZE + EEQ_INCTIME ],
                   func : old[ i * EEQ_SIZE + EEQ_FUNC ],
                   eff_ob_num : old[ i * EEQ_SIZE + EEQ_EFF_OB ],
                   interval : old[ i * EEQ_SIZE + EEQ_INTERVAL ],
                   flags : old[ i * EEQ_SIZE + EEQ_FLAGS ],
                   eff_ev_args : old[ i * EEQ_SIZE + EEQ_ARGS ] );
  }
} /* convert_arrays() */

/** @ignore yes */
void init_after_save() {
  /* called after the restore_object is done to init shadows etc */
  int i;
  int neffs;
  string shad;
  string effn;
  object ob;
  mixed arg;
  int *ee_exists;

  convert_arrays();

  /* convert old size == 5 classes into the new size == 6 ones */

  for( i = 0; i < sizeof( eeq ); i++ ) {
    if( classp( eeq[i] ) && sizeof( eeq[i] ) < 6 ) {
      eeq[i] = new( class effect_event,
                    inctime : eeq[i]->inctime ,
                    func : eeq[i]->func ,
                    eff_ob_num : eeq[i]->eff_ob_num ,
                    interval : eeq[i]->interval ,
                    flags : eeq[i]->flags ,
                    eff_ev_args : 0 );
    }
  }

  neffs = sizeof(effs);

  _data->shadows = allocate(neffs);

  for (i = 0; i < neffs; i++) {
    effn = effs[i]->ob_name;
    _data->shadows[i] = new(class effect_shadow, idnum : _data->next_id);
    if (catch(shad = (string)
              effn->query_shadow_ob())) {
      continue;
    }
    if (shad) {
      ob = clone_object(shad);
      _data->shadows[i]->shad_ob = ob;
      ob->attach_to_player(affected_object(), _data->next_id);
    }

    _data->current_enum = i;
    arg = effs[i]->arg;
    if (pointerp(arg) && sizeof(arg) == 3 && arg[2] == BOGUS_OLD_ARG) {
      arg = arg[1];
    }
    catch(effn->restart(affected_object(), arg, _data->next_id++));
  }

  ee_exists = allocate(sizeof(effs));
  for (i = 0; i < sizeof(eeq); i++) {
    ee_exists[eeq[i]->eff_ob_num] = 1;
  }
  for (i = sizeof(ee_exists) - 1; i >= 0; i--) {
    if (catch(ob = load_object(effs[i]->ob_name)) || !ob) {
      delete_effect(i);
      continue;
    }
    if (!ee_exists[i] && !ob->query_indefinite()) {
      delete_effect(i);
    }
  }
  _data->current_enum = -1;

  effect_unfreeze();
} /* init_after_save() */

/**
 * This method returns an array of effects matching the
 * classification.  The start part of the classification is
 * used to match, so passing "npc." into this function will
 * match all the effects which modify how an
 * npc functions.
 * @param eff the classification to match
 * @return the array of enums corresponding to the effects
 * @see add_effect()
 */
int *effects_matching(string eff) {
  /* return an array of all effect indexes matching the partial string 'eff' */
  int i, neffs;
  int *match;
  string clas;
  object ob;

  match = ({ });

  neffs = sizeof(effs);
  for (i = 0; i < neffs; i++) {
    if (catch(ob = load_object(effs[i]->ob_name)) || !ob) {
      continue;
    }
    clas = (string)ob->query_classification();
    if (!stringp(clas)) {
      continue; /* bleccch */
    }
    if (clas[0..strlen(eff)-1] == eff) {
      match += ({ i });
    }
  }
  return match;
} /* effects_matching() */

/**
 * This removes an effect from the player.  It uses the enum as
 * returned from sid to enum.  This is the number you see in
 * the stat of the object next to the effect.
 * @param i the enum to remove
 * @see add_effect()
 */
void delete_effect(int i) {
   /* delete shadow and remove effect from arrays */
   int id, j;
   object effect_object, shadow_object;
   string effect_file;
   mixed arg;

#ifdef DEBUG
   if ( logging_effects )
      log_file( "EFFECT_WOBBLE",
               "%d:%s(%d) delete_effect:(%d) sizeof(eeq)=%d\n",time() % 1000,
               this_object()->query_name(),current_enum,i, sizeof(eeq));
#endif
   if ((i < 0) || (i >= sizeof( effs ))) {
      return;
   }

   id = _data->shadows[i]->idnum;
   arg = effs[i]->arg;

   effect_freeze();
   for (j = 0; j < sizeof(eeq); j++) {
      if (eeq[j]->eff_ob_num == i) {
         if (sizeof(eeq)-1 > j)
            eeq[j+1]->inctime += eeq[j]->inctime;
         eeq = delete(eeq, j--, 1);
      } else {
         if (eeq[j]->eff_ob_num > i)
            eeq[j]->eff_ob_num--;
      }
   }
   effect_file = effs[i]->ob_name;
   effs = delete(effs, i, 1);

   shadow_object = _data->shadows[i]->shad_ob;
   _data->shadows = delete(_data->shadows, i, 1);

   effect_unfreeze();

   /* once all bookkeeping's done, it's safe to call end(), since we're
    * essentially outside the effects system.
    */
#ifdef DEBUG
   if ( logging_effects )
      log_file( "EFFECT_WOBBLE",
               "%d:%s(%d) delete_effect: before end\n",time() % 1000,
               this_object()->query_name(),current_enum);
#endif
   effect_object = find_object(effect_file);
   if (!effect_object)
      catch(effect_object = load_object(effect_file));

   if (effect_object)
      if (pointerp(arg) && (sizeof(arg) == 3) &&
          (arg[2] == BOGUS_OLD_ARG))
         effect_object->end(affected_object(), arg[1], id);
   else
      effect_object->end(affected_object(), arg, id);
#ifdef DEBUG
   if ( logging_effects )
      log_file( "EFFECT_WOBBLE",
               "%d:%s(%d) delete_effect: after end\n",time() % 1000,
               this_object()->query_name(),_data->current_enum);
#endif
   /* kill the shadow if it exists */
   if (objectp(shadow_object))
      shadow_object->remove_effect_shadow(id);

} /* delete_effect() */

/**
 * This method returns the effect number for the given effect id.  The
 * effect number is an internal nhandle used by delete_effect()
 * arg_of() set_arg_of() functions.  "id" is passed into the
 * begining/end etc functions on the effect object.
 * @param sid the effect id
 * @return the effect number
 * @see submit_ee2()
 * @see delete_effect()
 * @see add_effect()
 * @see arg_of()
 * @see set_arg_of()
 */
int sid_to_enum(int sid) {
   return member_array(1,
      map(_data->shadows, (: classp($1) &&
                      ((class effect_shadow)$1)->idnum == $(sid) :)));
} /* sid_to_enum() */

/**
 * This method turns an effect number into an effect id.
 * @see sid_to_enum()
 * @see add_effect()
 * @param enum the effect number
 * @return the effect id
 */
int enum_to_sid(int enum) {
  return _data->shadows[enum]->idnum;
} /* enum_to_sid() */

/**
 * This method returns the effect object for the given effect number.
 * @param enum the effect number
 * @return the effect object
 * @see arg_of()
 * @see add_effect()
 * @see sid_to_enum()
 */
string eff_of(int enum) {
  return effs[enum]->ob_name;
} /* eff_of() */

/**
 * This method returns the current argument associated with the
 * given effect number.
 * @param enum the effect number
 * @return the argument associated with the effect
 * @see add_effect()
 * @see set_arg_of()
 * @see sid_to_enum()
 */
mixed arg_of(int enum) {
  mixed arg;

  if (sizeof(effs) <= enum)
    return 0;

  arg = effs[enum]->arg;
  if (arrayp(arg) && sizeof(arg) == 3)
    if (arg[2] == BOGUS_OLD_ARG)
      return copy(arg[1]);
  return copy(arg);
} /* arg_of() */

/**
 * This sets the argument of the given effect to a new value.
 * @param enum the effect number to change
 * @param newarg the new argument value
 * @see sid_to_enum()
 * @see arg_of()
 * @see add_effect()
 */
void set_arg_of(int enum, mixed newarg) {
  effs[enum]->arg = newarg;
} /* set_arg_of */

/** @ignore yes */
class effect *query_effs() { return effs; }
/** @ignore yes */
class effect_shadow *query_effect_shadows() { return _data->shadows; }
/** @ignore yes */
class effect_event *query_eeq() { return eeq; }

/** @ignore yes */
void effect_timer() {
   int enum;
   object effect_object;
   mixed res, last_flags;
   class effect_event thisee;

#ifdef DEBUG
   if ( logging_effects )
      log_file( "EFFECT_WOBBLE",
               "%d:%s(%d) effect_timer:() sizeof(eeq)=%d\n",time() % 1000,
               this_object()->query_name(),current_enum,sizeof(eeq));
#endif
   _data->next_event_time = 0;
   if (!sizeof(eeq)) {
      return;
   }

   thisee = eeq[0];
   eeq = eeq[1..];

   enum = _data->current_enum = thisee->eff_ob_num;
   if (enum >= sizeof(effs)) {
      effect_unfreeze();
      if (this_object()->query_creator()) {
         tell_object(this_object(), "Effect number out of range, "
                     "ignoring.\n");
      }
      return;
   }

   if (thisee->flags & EE_CANCELLED) {
      effect_unfreeze();
      return;
   }

   if (thisee->flags & EE_CONTINUOUS) {
      last_flags = int_submit_ee(enum, thisee->func, thisee->interval,
                                 thisee->flags, thisee->eff_ev_args );
   }

   effect_unfreeze(); /* restart timer */

   effect_object = find_object(effs[enum]->ob_name);
   if (!effect_object) {
      catch {
         effect_object = load_object(effs[enum]->ob_name);
      };
   }
   if (!effect_object) {
      if (this_object()->query_creator() ) {
         tell_object( this_object(), "Non-existent effect "+
                     effs[enum]->ob_name +", deleting.\n" );
      }
      delete_effect(enum);
      _data->current_enum = -1;
      return;
   }

/*
   if (this_object()->query_name() == "pinkfish") {
      tell_creator("pinkfish", "%O %O %O", effect_object, enum, thisee);
   }
*/

   if (thisee->flags & EE_OLD) {
      res = (mixed)effect_object->
         effect_heart_beat(this_object(), ++effs[enum]->arg[0],
                           effs[enum]->arg[1], _data->shadows[enum]->idnum);
      if (res == REMOVE_THIS_EFFECT) {
         delete_effect(enum);
      } else {
         effs[enum]->arg[1] = res;
      }
   } else {
      if (stringp(thisee->func)) {
         res = call_other(effect_object, thisee->func, affected_object(),
                          effs[enum]->arg, _data->shadows[enum]->idnum, enum,
                          thisee->eff_ev_args );

         if (res == REMOVE_THIS_EFFECT) {
            delete_effect(enum);
         } else {
            if (res == CANCEL_THIS_EE) {
               eeq[last_flags]->flags |= EE_CANCELLED;
            }
         }
      } else if (functionp(thisee->func)) {
         res = evaluate(thisee->func, affected_object(), effs[enum]->arg,
                     _data->shadows[enum]->idnum, enum, thisee->eff_ev_args );
         if (res == REMOVE_THIS_EFFECT) {
            delete_effect(enum);
         } else if (res == CANCEL_THIS_EE) {
               eeq[last_flags]->flags |= EE_CANCELLED;
         }
      }
   }
   if (thisee->flags & EE_REMOVE)
      delete_effect(enum);

   _data->current_enum = -1;
} /* effect_timer() */

/** @ignore yes */
mixed *stats() {
   mixed *sr;
   int i;
   object ob;

   sr = ({ });

   for (i = 0; i < sizeof(effs); i++) {
      if (catch(ob = load_object(effs[i]->ob_name)) || !ob) {
         sr += ({ ({ "effect#" + i, " (buggered)" }) });
         continue;
      }
      if (intp(effs[i]->arg) || stringp(effs[i]->arg)) {
         sr += ({ ({ "effect#" + i, 
                       effs[i]->ob_name->query_classification() +
                       " (" + effs[i]->ob_name + ") (" + 
                       effs[i]->arg + ")" }) });
      } else {
         sr += ({ ({ "effect#"+i, effs[i]->ob_name->query_classification() +
               " (complex)" }) });
      }
   }
   return sr;
} /* stats() */

/**
 * This method removes all the effects as we die.  It calls the function
 * survive_death() on the effect objects themselves to determine if
 * they should be kept when the living dies.
 */
void effects_thru_death() {
   int i;
   int id; 
   object ob;

   i = sizeof( effs );

   while ( i-- ) {
      id = _data->shadows[i]->idnum;

      if ( catch( ob = load_object(effs[i]->ob_name)) || !ob ||
          !ob->survive_death( affected_object(), effs[i]->arg, id) ) { 
         delete_effect(i);
      } 
   }
} /* effects_thru_death() */

/**
 * This method returns the current enum.
 */
int query_current_effect_enum() {
   return _data->current_enum;
} /* query_current_effect_enum() */

/**
 * This method is used to force the effects setup in the object to be a
 * specified value.  THis is used in the effects effect which sets up
 * effects on someone using a shadow.  This should only be used on startup.
 * @param args the effects array to overwrite the old one with.
 */
protected void set_effs( mixed *args ) {
  int i, neffs;

  if ( sizeof( args ) && !classp( args[ 0 ] ) ) {
    neffs = sizeof( args ) / EFF_SIZE;
    effs = allocate( neffs );
    for (i = 0; i < neffs; i++) {
      effs[ i ] = new( class effect,
                       ob_name : args[ i * EFF_SIZE + EFF_OB_NAME ],
                       arg : args[ i * EFF_SIZE + EFF_ARG ] );
    }
  } else {
    effs = args;
  }
} /* set_effs() */

/**
 * This method is used to force the effects setup in the object to be a
 * specified value.  THis is used in the effects effect which sets up
 * effects on someone using a shadow.  This should only be used on startup.
 * @param args the effects array to overwrite the old one with.
 */
protected void set_eeq( mixed *args ) {
  int i, neeq;

  if ( sizeof( args ) && !classp( args[ 0 ] ) ) {
    neeq = sizeof( args ) / EEQ_SIZE;
    eeq = allocate( neeq );
    for (i = 0; i < neeq; i++) {
      eeq[ i ] = new( class effect_event,
                      inctime : args[ i * EEQ_SIZE + EEQ_INCTIME ],
                      func : args[ i * EEQ_SIZE + EEQ_FUNC ],
                      eff_ob_num : args[ i * EEQ_SIZE + EEQ_EFF_OB ],
                      interval : args[ i * EEQ_SIZE + EEQ_INTERVAL ],
                      flags : args[ i * EEQ_SIZE + EEQ_FLAGS ],
                      eff_ev_args : args[ i * EEQ_SIZE + EEQ_ARGS ] );
    }
  } else {
    eeq = args;
  }
} /* set_eeq() */
