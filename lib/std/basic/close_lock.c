// /std/basic/close_lock
/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: close_lock.c,v 1.39 2003/03/06 11:04:14 runtime Exp $
 *
 */
/**
 * This class keeps track of whether things are open or closed,
 * locked or unlocked.  Trap functions that are triggered when
 * an object is opened or unlocked are also handled.  When
 * you need an object that is locked you should remember to
 * use set_key() and set_difficulty().  When cloning an object
 * and moving other objects inside it you should remember to
 * make sure the object is open before moving things into it,
 * and then close it if neccessary.
 *
 * @author Gototh
 */
#include <move_failures.h>
#include <tasks.h>
#include <clothing.h>

#define MAX_INVENT 40
#define C_CLOSED 1
#define C_TRANS 2
#define C_OPAQUE 1

#define LOCKED        1
#define CLOSED        2
#define CLOSED_LOCKED 3

#define SKILL "covert.manipulation.sleight-of-hand"

#define STEALTH_NONE        0
#define STEALTH_FAILED      1
#define STEALTH_SUCCEEDED   2

#define TP this_player()
#define TO this_object()

int do_open();
int do_close();
int do_lock(object *keys);
int do_unlock(object *keys);

private int _locked;
private int _closed;

private string _pick_skill;

private nosave int _stuck;
private nosave int _trans;
private nosave int _difficulty;
private nosave int _autolock;

private nosave string _trap_open_func;
private nosave string _trap_lock_func;

private nosave object _trap_open_ob;
private nosave object _trap_lock_ob;

private nosave mixed _key;

/**
 * @ignore yes
 */
void create() {
  _difficulty = 5;
  _pick_skill = "covert.lockpick.doors";
} /* create() */

/**
 * This sets the object as being open.  Note that to find out
 * if an object is open we use query_closed() which will return
 * 0 if the object is open.
 *
 * @see set_closed()
 * @see query_closed()
 */

void set_open() {
    if (!_closed)
        return;
    _closed = 0;
    _locked = 0;
    if (environment()) {
      event( ({ environment(), TO, }), "open", TP);
    } else if (TO->query_my_room()) {
      event( ({ TO->query_my_room(), TO, }), "open", TP);
    }
} /* set_open() */

/**
 * This sets the object as being closed.  If you are moving
 * objects into the object, make sure that you do not set
 * the object as closed and then try to move the objects in.
 * Move the objects in and then close the object.
 *
 * @see set_open()
 * @see query_closed()
 */

void set_closed() {
    if (_closed)
        return;
    _closed = 1;
    if (environment()) {
      event( ({ environment(), TO, }), "close", TP);
    } else if (TO->query_my_room()) {
      event( ({ TO->query_my_room(), TO, }), "close", TP);
    }
} /* set_closed() */

/**
 * This function tells you whether an object is open or closed.
 * It returns 1 if the object is closed, and 0 if the object is open.
 *
 * @return Return 1 if closed, 0 if open.
 * @see set_open()
 * @see set_closed()
 */
int query_closed() {
  return _closed;
} /* query_closed() */

/**
 * This function tells you whether an object is open or closed.
 * It returns 1 if the object is open and 0 if the object is closed.
 *
 * @return Return 1 if open, 0 if closed.
 * @see set_open()
 * @see set_closed()
 */
int query_open() {
  return !_closed;
}

/**
 * This sets the object as being locked.  Remember to set the
 * object as being closed before using this.  In most cases you
 * should probably use set_key() to set the key which opens the
 * lock and use set_difficulty() to set the skill level required
 * to pick the lock.
 *
 * @see set_open()
 * @see set_closed()
 * @see set_key()
 * @see set_difficulty()
 */
void set_locked() {
    if (!_closed)
        return;
    if (_locked)
        return;
    _locked = 1;
    if (environment()) {
        event( ({ environment(), TO, }), "lock", TP);
    } else if (TO->query_my_room()) {
        event( ({ TO->query_my_room(), TO, }), "lock", TP);
    }
} /* set_locked() */

/**
 * This sets the object as being unlocked.
 *
 * @see set_locked()
 */
void set_unlocked() {
    if (!_locked)
        return;
    _locked = 0;
    if (environment()) {
        event( ({ environment(), TO, }), "unlock", TP);
    } else if (TO->query_my_room()) {
        event( ({ TO->query_my_room(), TO, }), "unlock", TP);
    }
} /* set_unlocked() */

/**
 * This tells us whether an object is locked.  It returns 1 if
 * the object is locked and 0 is the object is unlocked.
 *
 * @see set_locked()
 * @see set_unlocked()
 * @return Return 1 if locked and 0 if unlocked.
 */

int query_locked() {
  return _locked;
} /* query_locked() */

/**
 * This tells us whether an object is unlocked.  It returns 1 if
 * the object is unlocked and 0 is the object is locked.
 *
 * @see set_locked()
 * @see set_unlocked()
 * @return Return 1 if unlocked and 0 if locked.
 */

int query_unlocked() {
  return !_locked;
} /* query_unlocked() */

/**
 * This sets the object so that it will lock automatically when closed.
 *
 * @param number 1 for autolock, 0 for no autolock.
 */
void set_autolock(int number) { _autolock = number; }

/**
 * This tells us whether an object will lock automatically when closed.
 *
 * @see set_autolock()
 * @return 1 for true 0 for false;
 */
int query_autolock() { return _autolock; }

/**
 * This sets the property of the key that will unlock the object. This can
 * be a string, or a function pointer which will evaluate to a string.
 *
 * @see set_locked()
 * @param val The property the key must have to unlock the object.
 */
void set_key(mixed val) {
    if (!stringp(val) && !functionp(val))
        return;
    _key = val;
} /* set_key() */

/**
 * This returns the property of the key that will unlock the
 * object.
 *
 * @see set_key()
 */
string query_key() {
    if (stringp(_key))
        return _key;
    if (functionp(_key))
        return evaluate(_key);
    return 0;
} /* query_key() */

/**
 * This sets the skill that will be used when attempting to
 * pick the lock.  By default it is set to "covert.lockpick.safes"
 * so it need only be changed if you want to check a different
 * skill.
 *
 * @see query_pick_skill()
 * @see set_difficulty()
 * @param str The skill to use when picking the lock.
 */

void set_pick_skill(string str) {
  _pick_skill = str;
} /* set_pick_skill() */

/**
 * This returns the name of the skill which is used when
 * determining if the object can be unlocked.
 *
 * @see set_pick_skill()
 */

string query_pick_skill() {
  return _pick_skill;
} /* query_pick_skill() */

/**
 * This sets how difficult the lock on an object is to pick.
 * By default it is set to 200.
 *
 * @see query_difficulty()
 * @param i The difficulty of the lock to pick.
 */

void set_difficulty(int i) {
  _difficulty = i;
} /* set_difficulty() */

/**
 * This returns the difficulty picking the lock on the object.
 * By default it is set to 200 unless it has been changed with
 * set_difficulty().
 *
 * @see set_difficulty()
 */

int query_difficulty() {
  return _difficulty;
} /* query_difficulty() */

/**
 * This can set an object as being stuck, or make it unstuck.
 * Stuck objects cannot be opened, closed, locked or unlocked.
 * Using set_stuck(1) will make the object stuck, and set_stuck(0)
 * will unstick it.
 *
 * @see query_stuck()
 * @param i Use 1 to make the object stuck, and 0 to unstick it.
 */

void set_stuck(int i) {
  _stuck = i;
} /* set_stuck() */

/**
 * This tells you whether an object is stuck or not.  It will
 * return 1 if the object is stuck, and 0 if the object is not
 * stuck.
 *
 * @see set_stuck()
 * @return Return 1 if the object is stuck, and 0 if it is not.
 */

int query_stuck() {
  return _stuck;
} /* query_stuck() */

/**
 * This sets the object as being transparent.
 * A transparent object is one in which you can see the
 * inventory even if it is closed.
 *
 * @see reset_transparent()
 * @see query_transparent()
 */

void set_transparent() {
  _trans = C_TRANS;
} /* set_transparent() */

/**
 * This method removes the current objects transparent
 * status.
 * A transparent object is one in which you can see the
 * inventory even if it is closed.  This is also
 * used for light propogration.
 *
 * @see set_transparent()
 * @see query_transparent()
 */

void reset_transparent() {
  _trans = 0;
} /* reset_transparent() */

/**
 * This method returns the current transparent
 * value of the object.
 * A transparent object is one in which you can see the
 * inventory even if it is closed.  This is also
 * used for light propogration.
 *
 * @see reset_transparent()
 * @see query_transparent()
 * @return 1 if the object is transparent
 */

int query_transparent() {
  return _trans;
} /* query_transparent() */

/**
 * This sets the function to be called when someone attempts to
 * open the object.  The function does not have to be in the
 * object's file, and can be specified using the 'ob' paramater.
 * In most cases the function will be stored in the same file,
 * so use this_object() in these cases.
 *
 * The function will be called with a single string parameter of "open" or
 * "close"
 *
 * @param ob The object in which the function is stored.
 * @param func The function to be called.
 * @see query_open_trap_func()
 * @see query_open_trap_ob()
 */

void set_open_trap(object ob, string func) {
  _trap_open_func =  func;
  _trap_open_ob = ob;
} /* set_open_trap() */

/**
 * This returns the name of the function to be called when
 * someone tries to open the object.
 *
 * @return The function to be called when the object is opened.
 * @see set_open_trap()
 * @see query_open_trap_ob()
 */

string query_open_trap_func() {
  return _trap_open_func;
} /* query_open_trap_func() */

/**
 * This returns the object on which the trap function is stored.
 * The trap function is called when someone tries to open the
 * the object.
 *
 * @return The function to be called when the object is opened.
 * @see set_open_trap()
 * @see query_open_trap_func()
 */

object query_open_trap_ob() {
  return _trap_open_ob;
} /* query_open_trap_ob() */

/**
 * This sets the function to be called when someone attempts to
 * unlock the object.  The function does not have to be in the
 * object's file, and can be specified using the 'ob' paramater.
 * In most cases the function will be stored in the same file,
 * so use this_object() in these cases.
 *
 * The function will be called with a single string parameter of "lock",
 * "unlock" or "pick"
 *
 * @param ob The object in which the function is stored.
 * @param func The function to be called.
 * @see query_lock_trap_func()
 * @see query_lock_trap_ob()
 */
void set_lock_trap(object ob, string func) {
  _trap_lock_func = func;
  _trap_lock_ob = ob;
} /* set_unlock_trap() */

/**
 * This returns the name of the function to be called when
 * some attempts to unlock the object.
 *
 * @return The function to be called when someone attempts
 * to unlock the object.
 * @see set_open_trap()
 * @see query_open_trap_ob()
 */

string query_lock_trap_func() {
  return _trap_lock_func;
} /* query_lock_trap_func() */

/**
 * This returns the object on which the trap function is stored.
 * The trap function is called when someone tries to unlock the
 * the object.
 *
 * @return The function to be called when the object is opened.
 * @see set_open_trap()
 * @see query_open_trap_func()
 */

object query_lock_trap_ob() {
  return _trap_lock_ob;
} /* query_lock_trap_ob() */

/**
 * This method returns the closed and locked status.  The status is
 * 0 if open and unlocked, 1 if locked, 2 if closed and 3 if closed
 * and locked.
 * @return closed and locked status
 */
int query_closed_locked_status() {
    if (_closed && _locked) {
        return CLOSED_LOCKED;
    }
    if (_closed) {
        return CLOSED;
    }
    if (_locked) {

        return LOCKED;
    }
    return 0;
} /* query_closed_locked_status() */

/**
 * This method adds the lock commands to the specified player.
 * It's been abstracted out of init() to make it easier for some
 * odd situations (like the t-shop).
 */
void add_close_lock_commands(object player) {
  if(!player)
    return;

    player->add_command("open", TO,
        "<direct:object:me-here>",
        (: do_open() :) );
    player->add_command("close", TO,
        "<direct:object:me-here>",
        (: do_close() :) );
    if (TO->query_key()) {
        player->add_command("lock", TO,
            "<direct:object:me-here>",
            (: do_lock(0) :) );
        player->add_command("lock", TO,
            "<direct:object:me-here> with <indirect:object:me>",
            (: do_lock($1) :) );
        player->add_command("unlock", TO,
            "<direct:object:me-here>",
            (: do_unlock(0) :) );
        player->add_command("unlock", TO,
            "<direct:object:me-here> with <indirect:object:me>",
            (: do_unlock($1) :) );
    }
} /* add_close_lock_commands() */

/**
 * @ignore yes
 */

void init() {
    add_close_lock_commands(this_player());
} /* init() */

/**
 * @ignore yes
 */
string short_status() {
    return "";
} /* short_status() */

/**
 * @ignore yes
 */
string long_status() {
    // If it is stuck in one state don't bother telling us the state.
    if (TO->query_stuck())
        return "";

    switch (query_closed_locked_status()) {
        case CLOSED_LOCKED:
            return "It is closed and locked.\n";
        case CLOSED:
            return "It is closed.\n";
        default:
            return "It is open.\n";
    }
} /* long_status() */

/**
 * @ignore yes
 */
int check_stealth() {
    mapping hide_invis;
    int hiding, sneaking, difficulty, light, my_light, items_blocking;

    hide_invis = (mapping) TP->query_hide_invis();
    hiding = undefinedp(hide_invis["hiding"]) ? 0 : 1;
    sneaking = TP->query_sneak_level() ? 1 : 0;

    if (!hiding && !sneaking)
        return STEALTH_NONE;

    my_light = TP->query_light();
    light = environment(TP)->query_light();

    difficulty = light + (4 * my_light) / (light + 1);

    items_blocking = sizeof(CLOTHING_HANDLER->query_items_blocking(TO, TP));

    if (items_blocking) {
        difficulty += 50 * items_blocking;
    }

    debug_printf( "Difficulty = %d.\n Skill = %s\n Bonus = %d\n",
                  difficulty, SKILL, TP->query_skill_bonus( SKILL ) );
    switch (TASKER->perform_task(TP, SKILL, difficulty, TM_FREE)) {
        case AWARD:
            write("%^YELLOW%^" + ({
                "You discover something that lets your fingers move more nimbly.",
                "You find yourself capable of deceiving the eye with greater ease "
                "than before.",
                "You realise how to deceive the eye more effectively."
                })[ random(3) ] + "%^RESET%^\n" );
        case SUCCEED :
            return STEALTH_SUCCEEDED;
        default :
            return STEALTH_FAILED;
    }
} /* check_stealth() */

/**
 * @ignore yes
 */
int do_unlock(object *keys) {
    object *gkeys = ({ });
    string key = TO->query_key();
    int inv = 0;
    if (!_locked) {
        TP->add_failed_mess(TO,
            "$D $V$0=isn't,aren't$V$ locked.\n",
            ({ }) );
        return 0;
    }
    if (_stuck) {
        TP->add_failed_mess(TO,
            "$D $V$0=is,are$V$ stuck.\n",
            ({ }) );
        return 0;
    }
    if (!_closed) {
        TP->add_failed_mess(TO,
            "$D $V$0=isn't,aren't$V$ closed.\n",
            ({ }) );
        return 0;
    }
    if (!key) {
        TP->add_failed_mess(TO,
            "$D $V$0=doesn't,don't$V$ have a lock.\n",
            ({ }) );
        return 0;
    }

    if (key != "by hand") {
        if (!keys || !sizeof(keys)) {
            keys = all_inventory(TP);
            inv = 1;
        }
        gkeys = filter(keys, (: $1->query_property( $(key) ) :) );
        if (!sizeof(gkeys)) {
            if (!inv) {
                TP->add_failed_mess(TO,
                    "You cannot $V $D with $I.\n",
                        keys);
                return 0;
            }
            TP->add_failed_mess(TO,
                "You cannot $V $D.\n",
                ({ }) );
            return 0;
        }
        gkeys[0]->use_key(TO);
    }

    if (_trap_lock_func && _trap_lock_ob) {
        if (!call_other(_trap_lock_ob, _trap_lock_func, "unlock", TO, TP)) {
            return 0;
        }
    }

    set_unlocked();
    switch (check_stealth()) {
        case STEALTH_SUCCEEDED:
          if (sizeof (gkeys)) {
            TP->add_succeeded_mess(TO, ({
                "$N $V $D with $I, managing to stay unnoticed.\n",
                "", }),
                ({ ( gkeys[0] ) }) );
          } else {
            TP->add_succeeded_mess(TO, ({
                "$N $V $D, managing to stay unnoticed.\n",
                "", }),
                ({ }) );
          }

            break;
        case STEALTH_FAILED:
            TP->add_succeeded_mess(TO,
                "$N unsuccessfully tr$y to " + query_verb() + " $D while "
                "staying unnoticed.\n", ({ }) );
            break;
        default:
            if (sizeof (gkeys)) {
              TP->add_succeeded_mess(TO,
                 "$N $V $D with $I.\n", ({ gkeys[0] }) );
            } else {
              TP->add_succeeded_mess (TO, "$N $V $D.\n", ({ }));
            }
            break;
    }
    return 1;
} /* do_unlock() */

/**
 * @ignore yes
 */
int do_lock(object *keys) {
    object *gkeys = ({ });
    string key = TO->query_key();
    int inv = 0;
    if (_locked) {
        TP->add_failed_mess(TO,
            "$D $V$0=is,are$V$ already locked.\n",
            ({ }) );
        return 0;
    }
    if (_stuck) {
        TP->add_failed_mess(TO,
            "$D $V$0=is,are$V$ stuck.\n",
            ({ }) );
        return 0;
    }
    if (!_closed && !do_close()) {
        TP->add_failed_mess(TO,
            "$D $V$0=isn't,aren't$V$ closed.\n",
            ({ }) );
        return 0;
    }
    if (!key) {
        TP->add_failed_mess(TO,
            "$D $V$0=doesn't,don't$V$ have a lock.\n",
            ({ }) );
        return 0;
    }

    if (key != "by hand") {
        if (!keys || !sizeof(keys)) {
            keys = all_inventory(TP);
            inv = 1;
        }
        gkeys = filter(keys, (: $1->query_property( $(key) ) :) );
        if (!sizeof(gkeys)) {
            if (!inv) {
                TP->add_failed_mess(TO, "You cannot lock $D with $I.\n", keys);
                return 0;
            }
            TP->add_failed_mess(TO, "You cannot lock $D.\n", ({ }) );
            return 0;
        }
        gkeys[0]->use_key(TO);
    }

    if (_trap_lock_func && _trap_lock_ob) {
        if (!call_other(_trap_lock_ob, _trap_lock_func, "lock", TO, TP)) {
            return 0;
        }
    }

    set_locked();
    switch (check_stealth()) {
    case STEALTH_SUCCEEDED:
          if (sizeof (gkeys)) {
            TP->add_succeeded_mess(TO, ({
                "$N lock$s $D with $I, managing to stay unnoticed.\n",
                "", }),
                ({ ( gkeys[0] ) }) );
          } else {
            TP->add_succeeded_mess(TO, ({
                "$N lock$s $D, managing to stay unnoticed.\n",
                "", }),
                ({ }) );
          }
        break;
      case STEALTH_FAILED:
        TP->add_succeeded_mess(TO, "$N unsuccessfully tr$y to " +
                               "lock $D while staying unnoticed.\n",
                               ({ }) );
        break;
      default:
        if (sizeof (gkeys)) {
          TP->add_succeeded_mess(TO,
                                 "$N lock$s $D with $I.\n", ({ gkeys[0] }) );
        } else {
          TP->add_succeeded_mess (TO, "$N lock$s $D.\n", ({ }));
        }
        break;
    }
    return 1;
} /* do_lock() */

/**
 * @ignore yes
 */
int pick_lock(object player) {
    if (_trap_lock_ob
     && !call_other(_trap_lock_ob, _trap_lock_func, "pick", TO, player)) {
        return 0;
    }
    set_locked();
    return 1;
} /* pick_lock() */

/**
 * This is called when someone successfully unlocks the object.
 * If there is a trap, and the trap function returns true, the
 * object will not be opened.  If the trap function returns
 * false, the object will still be opened even if the trap is
 * sprung.
 *
 * @param player The player who is unlocking the object.
 *
 * @return Returns 1 if the object was unlocked, and 0 otherwise.
 */
int pick_unlock(object player) {
    if (_trap_lock_ob
     && !call_other(_trap_lock_ob, _trap_lock_func, "pick", TO, player)) {
        return 0;
    }
    set_unlocked();
    return 1;
} /* pick_unlock() */

/**
 * @ignore yes
 */
int do_open() {
    if (!_closed) {
        TP->add_failed_mess(TO,
            "$D $V$0=is,are$V$ already open.\n",
            ({ }) );
        return 0;
    }
    if (_stuck) {
        TP->add_failed_mess(TO,
            "$D $V$0=is,are$V$ stuck.\n",
            ({ }) );
        return 0;
    }
    if (_locked && !do_unlock(0)) {
        TP->add_failed_mess(TO,
            "$D $V$0=is,are$V$ locked.\n",
            ({ }) );
        return 0;
    }

    if (_trap_open_func && _trap_open_ob) {
        if (!call_other(_trap_open_ob, _trap_open_func, "open", TO, TP)) {
            return 0;
        }
    }

    set_open();
    if (!TO->query_door_name()) {
        switch (check_stealth()) {
            case STEALTH_SUCCEEDED:
                TP->add_succeeded_mess(TO, ({
                    "$N $V $D, managing to stay unnoticed.\n",
                    "", }),
                    ({ }) );
                break;
            case STEALTH_FAILED:
                TP->add_succeeded_mess(TO,
                    "$N unsuccessfully tr$y to " + query_verb() + " $D while "
                    "staying unnoticed.\n", ({ }) );
                break;
            default:
                TP->add_succeeded_mess(TO,
                    "$N $V $D.\n",
                    ({ }) );
                break;
        }
    }
    return 1;
} /* do_open() */

/**
 * @ignore yes
 */
int do_close() {
    if (_closed) {
        TP->add_failed_mess(TO,
            "$D $V$0=is,are$V$ already closed.\n",
            ({ }) );
        return 0;
    }
    if (_stuck) {
        TP->add_failed_mess(TO,
            "$D $V$0=is,are$V$ stuck.\n",
            ({ }) );
        return 0;
    }
    if (_trap_open_func && _trap_open_ob) {
        if (!call_other(_trap_open_ob, _trap_open_func, "close", TO,TP)) {
            return 0;
        }
    }

    set_closed();
    if (_autolock)
        set_locked();
    if (!TO->query_door_name()) {
        switch (check_stealth()) {
            case STEALTH_SUCCEEDED:
                TP->add_succeeded_mess(TO, ({
                    "$N $V $D, managing to stay unnoticed.\n",
                    "", }),
                    ({ }) );
                break;
            case STEALTH_FAILED:
                TP->add_succeeded_mess(TO,
                    "$N unsuccessfully tr$y to " + query_verb() + " $D while "
                    "staying unnoticed.\n", ({ }) );
                break;
            default:
                TP->add_succeeded_mess(TO,
                    "$N $V $D.\n",
                    ({ }) );
                break;
        }
    }
    return 1;
} /* do_close() */

/**
 * @ignore yes
 */
string *parse_command_adjectiv_id_list() {
    switch( query_closed_locked_status() ) {
        case CLOSED_LOCKED:
            return ({ "closed", "locked" });
        case CLOSED:
            return ({ "closed", "unlocked" });
        case LOCKED:
            return ({ "open", "locked" });
        default:
            return ({ "open", "unlocked" });
    }
} /* parse_command_adjectiv_id_list() */

/**
 * @ignore yes
 */
mixed *stats() {
  return ({
    ({"closed", query_closed()}),
    ({"transparent", query_transparent()}),
    ({"key", query_key()}),
    ({"difficulty", query_difficulty()}),
    ({"locked", query_locked()}),
    ({"stuck", query_stuck()}),
    ({"open trapped", _trap_open_func != 0}),
    ({"lock trapped", _trap_lock_func != 0}),
    ({"pick skill", query_pick_skill()}),});
} /* stats() */
/* EOF */
