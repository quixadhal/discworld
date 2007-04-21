/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: fairy_godmothers.c,v 1.14 2002/09/02 11:39:59 taffyd Exp $
 * $Log: fairy_godmothers.c,v $
 * Revision 1.14  2002/09/02 11:39:59  taffyd
 * Changed non-existant set_social_points() method to new set_sp().
 *
 * Revision 1.13  2002/08/03 22:54:56  tilly
 * Changed it to query the player's start location instead of being hard coded for the Drum or CWC.
 *
 * Revision 1.12  2002/02/20 10:42:53  taffyd
 * Temporary tweak for Cwc.
 *
 * Revision 1.11  2002/01/25 13:24:12  tilly
 * Added a call_out to move the godmother to the rubbish room, because she vanished before moving the player and saying all her lines. Also added an exit message for when she vanishes.
 *
 * Revision 1.10  2001/12/27 01:06:48  drakkos
 * Added some logging and a playerinfo entry for when using the godmother costs a life.
 *
 * Revision 1.9  2001/03/24 00:08:47  ceres
 * Fixed death cost & GMs not speaking when they arrive.
 *
 * Revision 1.8  2001/03/09 21:55:47  ceres
 * Added entry message for godmothers
 *
 * Revision 1.6  2000/06/27 00:17:43  pinkfish
 * change to use the config.h file.
 *
 * Revision 1.5  2000/04/08 01:33:06  pinkfish
 * Fix up some errors in the godmother handler.
 *
 * Revision 1.4  1999/05/21 18:22:02  rywfol
 * Fixed bug/typo which prevented the "attacked godmother"
 * property from doing anything..
 *
 * Revision 1.3  1998/06/12 23:29:20  presto
 * Fixed typo
 *
 * Revision 1.2  1998/05/02 16:44:53  presto
 * Fixed typo, it's --> its
 *
 * Revision 1.1  1998/01/06 05:03:33  ceres
 * Initial revision
 * 
*/
/**
 * Keep track of all the fairy godmothers.  Make sure they don't do any
 * thing naughty.  This also controls the costs for the godmother
 * service.
 * @author Pinkfish
 */
#include <panic.h>
#include <config.h>
#include <playerinfo.h>
#include <cwc.h>

#define HARRY_SHADOW "/std/shadows/misc/harry_shadow"

object* _panicers;
object* _godmothers;
object* _queue;

#define FREE_NAME "free trip"
#define THRESHOLD 50
#define DAY (60*60*24)

/* Prototypes */

public  void   continue_panicing(string str, object panicer);
private object startup_godmother(object ob);
private void   finish_with_person(object ob, int ok);

/* Functions */

void create() {
  _panicers = ({ });
  _godmothers = allocate(NUM_GODMOTHERS);
  _queue = ({ });
} /* setup() */

/**
 * Return an array of all the godmothers.  This is the current set of
 * used godmothers.
 * @return an array of objects of godmothers
 */
object *query_godmothers() { return copy(_godmothers); }

/**
 * Check to see if there are any free godmothers.  Are you being
 * served?
 * @return 1 if there is a free one, 0 if there is not
 */
int free_godmothers() {
  int i;

  for (i=0;i<sizeof(_godmothers);i++) {
    if (!_godmothers[i]) {
      return 1;
    }
  }
  return 0;
} /* free_godmothers() */

/**
 * This method handles a player panicing.
 * <p>
 * This is the fairy godmother controller for panicing.
 * <p>
 * When someone panics it calls on this object to help fix them
 * up.
 * @param panicer the person panicing
 * @return 0 if they were unmable to panic, 1 if they were
 */
int do_startup(object panicer) {
  if (environment(panicer) &&
      environment(panicer)->query_property("no godmother")) {
    notify_fail(environment(panicer)->query_property("no godmother"));
    return 0;
  }

  if (panicer->query_property("attacked godmother") &&
      (panicer->query_property("attacked godmother") + DAY * 10) > time())
  {
    notify_fail("You attacked a godmother!  No help for you!\n");
    return 0;
  }

  if (sizeof(panicer->query_attacker_list())) {
    notify_fail("You cannot use a godmother whilst you are in a fight.\n");
    return 0;
  }

  panicer->remove_property("attacked godmother");

  _panicers -= ({ 0 });
  if (member_array(panicer, _panicers) != -1  ||
      member_array(panicer, _queue) != -1)
  {
    notify_fail("You are already requesting assistance.  Be patient.\n");
    return 0;
  }

  if (!panicer) {
    panicer = this_player();
  }

  if (panicer->query_level() < 10 ||
      !arrayp(environment(panicer)->query_exits()) ||
      !sizeof(environment(panicer)->query_exits()) ||
      !environment(panicer))
  {
    write("You feel help is on its way.\n");
    panicer->add_property("godmother", FREE_NAME);
    continue_panicing("y", panicer);
  }
  else {
    panicer->remove_property("godmother");
    if (panicer->query_level() < 50) {
      int max_gp;

      max_gp = panicer->query_max_gp();
      if (panicer->query_gp() < max_gp / 3) {
        notify_fail("You do not have enough guild points to summon "
                    "a godmother.  You need at least " +
                    (max_gp / 3) + ".\n");
        return 0;
      }
      write("This will cost you all of your current guild points and "
            "social points, and leave you feeling a little weak.  "
            "Do you wish to continue? ");
      input_to("continue_panicing", 0, panicer);
    }
    else {
      if (panicer->query_max_deaths() <= panicer->query_deaths()) {
        notify_fail("You do not have a life to spend on a godmother.\n");
        return 0;
      }
      write("This will cost you a life, do you wish to continue? ");
      input_to("continue_panicing", 0, panicer);
    }
  }
  return 1;
} /* do_startup() */


/** @ignore yes */
private void do_check_queue() {
   _queue -= ({ 0 });
   if (sizeof(_queue)) {
      if (startup_godmother(_queue[0])) {
         _queue = _queue[1..];
      }
   }
} /* do_check_queue() */


/** @ignore yes */
private void setup_harrass_callout() {
  int i;
  int check_queue;
  int waiting;
  int max_wait;

  for (i = 0; i < sizeof(_godmothers); i++) {
    if (_godmothers[i]) {
      waiting = _godmothers[i]->query_waiting();
      if (waiting > FIRST_WAIT_TIME && environment(_godmothers[i])) {
        _godmothers[i]->hurry_up();
      }
      if (!environment(_godmothers[i])) {
         _godmothers[i]->dest_me();
         check_queue = 1;
      }
      if (waiting > max_wait) {
        max_wait = waiting;
      }
    }
  }
  if (max_wait < FIRST_WAIT_TIME) {
    call_out("do_hurry_up", FIRST_WAIT_TIME-max_wait+2);
  }
  if (check_queue) {
    do_check_queue();
  }
} /* setup_harrass_callout() */


/** @ignore yes */
void continue_panicing(string str, object panicer) {
  str = lower_case(str);
  if (!str || str == "" || (str[0] != 'y' && str[0] != 'n')) {
    if (panicer->query_level() < 100) {
      write("This will cost you all of your current guild points and "
            "social points and leave you feeling a little weak.  "
            "Do you wish to continue? ");
      input_to("continue_panicing", 0, panicer);
      return ;
    }
    else {
      write("This will cost you a life, do you wish to continue? ");
      input_to("continue_panicing", 0, panicer);
      return ;
    }
  }

  if (str[0] == 'n') {
    write("Ok.  They do not show up to help you.  Good luck.\n");
    return ;
  }

  if (!startup_godmother(panicer)) {
    write("You hope a godmother will turn up sometime soon.\n");
    _queue += ({ panicer });
    setup_harrass_callout();
  }
} /* continue_panicing() */

void do_panic_callout(object godmother, object ob) {
  godmother->do_panic(ob);
}

private object startup_godmother(object ob) {
  object godmother;
  int    i, num;

  /* Which fairy godmother do you get? */
  /* Only done 2 so far... */
  i = random(NUM_GODMOTHERS);
  if (_godmothers[i]) {
    for (num = 1; num < NUM_GODMOTHERS; num++)  {
      i = (i + 1) % NUM_GODMOTHERS;
      if (!_godmothers[i])  break;
    }
  }

  /* See if they're all busy... */
  if (num == NUM_GODMOTHERS)
     return 0;

  switch (i) {
    case 0:
      godmother = clone_object(GODMOTHER_DIR + "granny");
      break;

    case 1:
      godmother = clone_object(GODMOTHER_DIR + "magrat");
      break;

    default:
      write("Something is hosed.  Please file a bug report.\n");
      break;
  }
  _godmothers[i] = godmother;
  godmother->move(environment(ob), "As if by magic $N appears.", "");
  call_out("do_panic_callout", 1, godmother, ob);
  _panicers += ({ ob });
  return godmother;
} /* startup_godmother() */

/**
 * Called by the godmother when the person has completed their
 * paniced transaction.
 * @param person the person who paniced
 * @param ok did they complete it ok?
 */
void finish_panic(object person, int ok) {
  int i;

  if (person)  {
     finish_with_person(person, ok);
  }

  _panicers -= ({ person });

  for (i = 0; i < sizeof(_godmothers); i++)  {
    if (_godmothers[i] == previous_object()) {
      call_out("do_move", 10, _godmothers[i] );
      break;
    }
  }

  do_check_queue();

  if (!sizeof(_queue)) {
     remove_call_out("do_hurry_up");
  }
} /* finish_panic() */


void do_move (object fairy) {
      fairy->move("/room/rubbish", "",
          "With a quick flick of her wand, the fairy godmother is gone.");
      fairy = 0;
} /* do_move */


private void finish_with_person(object person, int ok)  {
  if (member_array(person, _panicers) == -1) {
    log_file("GODMOTHER_ERROR", "person == %O, godmother == %s\n", person, previous_object()->query_short());
    write("Consistancy error, they never paniced.\n");
    return ;
  }
  if (ok) {
    if (person->query_property("godmother") == FREE_NAME) {
    }
    else if (person->query_level() < THRESHOLD) {
      person->adjust_tmp_con(-2);
      person->set_gp(0);
      person->set_sp(0);
    }
    else if (person->query_level() >= THRESHOLD &&
             person->query_max_deaths() > person->query_deaths()) {
      PLAYERINFO_HANDLER->add_entry(person,
        person->query_name(), "misc", "Lost a life to the Godmother");
      log_file("GODMOTHER_DEATH", "%s: %s lost a life to the Godmother\n", 
        ctime(time()), person->query_name());              
      person->adjust_deaths(1);
    }

    
    call_out(function(object person) {
      string destination;
      
      destination = person->query_nationality()->query_default_start_location();

      person->move_with_look(destination,
                             "$N appears out of nowhere.",
                             "$N disappears with a pop.");
      if (person->query_ghost()) {
        person->remove_ghost();
        clone_object(HARRY_SHADOW)->setup_shadow(person,
                                                 "a fairy godmother" );
      }
    }, 2, person);
  }

  person->remove_property("godmother");
} /* finish_with_person() */


/**
 * Give the players a nudge if they are taking to long.
 */
void do_hurry_up() {
  int i;
  int waiting;
  int max_wait;

  if (!sizeof(_queue)) {
    return ;
  }

  for (i = 0; i < sizeof(_godmothers); i++) {
    if (_godmothers[i]) {
      if (!environment(_godmothers[i])) {
        _godmothers[i]->dest_me();
        _godmothers[i] = 0;
      }
      else {
        waiting = _godmothers[i]->query_waiting();
        if (waiting > FIRST_WAIT_TIME) {
          if (waiting > FINISH_WAIT_TIME) {
            _godmothers[i]->finish_up();
          } else {
            _godmothers[i]->hurry_up();
          }
        }
        if (waiting > max_wait) {
          max_wait = waiting;
        }
      }
    }
  }
  if (max_wait > 0) {
    if (FINISH_WAIT_TIME - max_wait < 10) {
      call_out("do_hurry_up", 10);
    } else {
      call_out("do_hurry_up", FINISH_WAIT_TIME-max_wait);
    }
  }
} /* do_hurry_up() */


/** @ignore yes */
void dest_me() {
  int i;

  for (i = 0; i < sizeof(_godmothers); i++) {
    if (_godmothers[i]) {
      _godmothers[i]->dest_me();
    }
  }
  destruct(this_object());
} /* dest_me() */
