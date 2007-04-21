/* -*- LPC -*- */
/**
 * This is the Refresh Handler.  When a player refreshes, this
 * handler is called so that all the miscellaneous bookkeeping
 * about that player can be reset.  This should be used
 * for taking someone out of the playtesters' group, resetting the
 * criminal ledger in KLK, etc.  Everything, in short,
 * which is not a property on the player object or otherwise
 * can be handled entirely by the player object without using any other
 * objects.
 *
 * Similarly, whenever a player gets deleted, this handler
 * gets called upon to call a number of deletion callbacks
 * to remove bookeeping about that player.
 *
 * If you have code that stores information about a player outside of
 * the player object, then you should call register_refresh()
 * and possibly register_delete()
 * on this handler.  That will then remember the function you
 * give it.  Then, whenever a player refreshes or deletes, the
 * function you specified will get called with the player object
 * and a flag to indicate if this is a total refresh or a partial
 * refresh (for refresh callbacks) or a string with the player
 * name (for player deletions).  Do the right thing.
 * will get called with the player object and a flag to indicate
 * if this is a total refresh or a partial refresh.  Do the
 * right thing.
 *
 * If your object moves to a different file, then you will
 * have to manually unregister the object from the refresh
 * handler.  This is very important!  To unregister, simply
 * call register_function() again, with the same object, but
 * give 0 for the function.
 *
 * Each object can have only one refresh function registered,
 * and clones can't have any!
 *
 * You can easily use the same function for both a deletion
 * and refresh callback.  All you do is declare the function
 *    void callback_func(mixed player, int flag)
 * and then test flag to see if it is PARTIAL_REFRESH,
 * TOTAL_REFRESH, or PLAYER_DELETED.  In the last case, player
 * will be a string.  Otherwise player is an object.
 *
 * @author Sin
 *
 * @change 17 Nov 1998, Sin
 *     Handles deletions now, too.
 */
#include <refresh.h>
#include <playerinfo.h>

#define SAVE_FILE "/save/refresh"

mapping refresh_callbacks;
mapping delete_callbacks;
nosave string loaded_by;
nosave string loaded_time;

/**
 * @ignore yes
 */
protected void load_me()
{
  refresh_callbacks = 0;
  delete_callbacks = 0;
  unguarded( (: restore_object, SAVE_FILE, 0 :) );
  if (!refresh_callbacks) {
    refresh_callbacks = ([ ]);
  }
  if (!delete_callbacks) {
    delete_callbacks = ([ ]);
  }
}

/**
 * @ignore yes
 */
protected void save_me()
{
  unguarded( (: save_object, SAVE_FILE, 0 :) );
}

/**
 * @ignore yes
 * This is used to make the player info handler happy.
 */
string query_name() {
   return "Refresh Handler";
} /* query_name() */

/**
 * @ignore yes
 */
void create()
{
  seteuid("/secure/master"->creator_file(file_name(this_object())));
  load_me();
  if (!previous_object()) {
    loaded_by = "The Masked Man";
  } else {
    if (previous_object()->query_creator()) {
      loaded_by = "Creator: " + previous_object()->query_name();
    } else if (previous_object()->query_interactive()) {
      loaded_by = "Player: " + previous_object()->query_name();
    } else {
      loaded_by = file_name(previous_object());
      if (this_player())
        loaded_by += sprintf(" (%s)", this_player()->query_name());
    }
  }
  loaded_time = ctime(time());
}

/**
 * @ignore yes
 */
void dest_me()
{
  save_me();
  destruct(this_object());
}

/**
 * @ignore yes
 */
nomask mixed dwep()
{
  efun::destruct(this_object());
  return "Destructed With Extreme Prejudice";
}

/**
 * Use this function to tell the refresh handler about functions that
 * you want to have called whenever any player refreshes or gets
 * deleted.
 *
 * Typically, you will call this by hand, with the 'call' command,
 * rather than coding a call to this function into your code.
 *
 * The first parameter is the object that contains the callback
 * to be called.  This can be either a pointer to the object, or
 * the filename to the object.
 *
 * The second parameter is the name of the function that should be
 * called.  This callback should take two parameters: an object for
 * the player who is affected, and an integer, which will be either
 * PARTIAL_REFRESH or TOTAL_REFRESH.
 *
 * This function will refuse to register a callback if either:
 * the object is a clone, the callback isn't defined in the object, or
 * the callback is a function pointer.
 */
string register_refresh(mixed ob, string func)
{
  object real;

  if (objectp(ob)) {
    ob = base_name(ob);
  }
  if (!ob) {
    return "Please supply an object";
  }

  real = load_object(ob);
  if (!real) {
    return "Couldn't find object";
  }
  if (func && !function_exists(func, real)) {
    return "Couldn't find function";
  }

  if (!func) {
    if (refresh_callbacks[ob]) {
      map_delete(refresh_callbacks, ob);
    }
  } else {
    if (refresh_callbacks[ob]) {
      refresh_callbacks[ob] = func;
    } else {
      refresh_callbacks += ([ ob : func ]);
    }
  }
  save_me();
}

/**
 * This function does almost exactly what register_refresh() does,
 * but it is used for registering delete handlers.
 * 
 * Delete callbacks are called with two parameters as well, but
 * the first parameter is a string containing the player's name,
 * while the second parameter is the integer PLAYER_DELETED.
 */
string register_delete(mixed ob, string func)
{
  object real;

  if (objectp(ob)) {
    ob = base_name(ob);
  }
  if (!ob) {
    return "Please supply an object";
  }

  real = load_object(ob);
  if (!real) {
    return "Couldn't find object";
  }
  if (func && !function_exists(func, real)) {
    return "Couldn't find function";
  }

  if (!func) {
    if (delete_callbacks[ob]) {
      map_delete(delete_callbacks, ob);
    }
  } else {
    if (delete_callbacks[ob]) {
      delete_callbacks[ob] = func;
    } else {
      delete_callbacks += ([ ob : func ]);
    }
  }
  save_me();
}

/**
 * This function gets called by the player object whenever
 * a player refreshes, or by any of the various objects that delete
 * players when they get deleted.  You should never call
 * this function directly.
 */
varargs void player_refreshed(object player, int totally) {
  string ob;

  if (base_name(file_name(previous_object())) != "/global/player") {
    return;
  }

  foreach (ob in keys(refresh_callbacks)) {
    object obj;

    obj = load_object(ob);
    if (obj) {
      mixed *vals;

      vals = ({ refresh_callbacks[ob], player, totally });
      call_out( (: call_other, obj, vals :), 1);
    }
  }
}

/**
 * This, like player_refreshed(), goes through and calls all the
 * registered deletion callbacks.  In this case, however, the
 * deletion callbacks get called with a _string_ rather than
 * an object.  That string is the name of the player who is being
 * deleted.
 */
varargs void player_deleted(string player)
{
  string ob;

  if (file_name(previous_object()) != "/secure/delete_clear" &&
      file_name(previous_object()) != "/cmds/lord/rmp_layer" &&
      !master()->high_programmer(previous_object(-1))) {
    unguarded( (: write_file, "/log/CHEAT", ctime( time() ) +
                ": illegal attempt to delete player files using "+
                "refresh_handler\nTrace: "+ back_trace() :) );
    return;
  }

  foreach (ob in keys(delete_callbacks)) {
    object obj;

    obj = load_object(ob);
    if (obj) {
      mixed *vals;

      vals = ({ delete_callbacks[ob], player, PLAYER_DELETED });
      call_out( (: call_other, obj, vals :), 1);
    }
  }
  "/secure/related_files"->delete_related_files(player, 1, 0);
}

/**
 * Return the current list of callback functions.
 */
mapping query_funcs()
{
  return ([
    "refresh callbacks" : refresh_callbacks,
    "delete callbacks" : delete_callbacks,
    ]);
}

/**
 * This function supports the 'stat' command.  If you stat the
 * refresh handler, you'll see up to 4 things: the number of
 * refresh callbacks registered, the number of delete callbacks registered,
 * the person or object who loaded the refresh handler, and the
 * time that the handler got loaded.
 */
mixed *stats()
{
  return ({
    ({ "refreshes", sizeof(refresh_callbacks) }),
    ({ "deletes", sizeof(delete_callbacks) }),
    ({ "loaded by", loaded_by }),
    ({ "loaded time", loaded_time }),
    });
}
