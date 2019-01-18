.DT
refresh.c
Disk World autodoc help
refresh.c

.SH Description
.SP 5 5

This is the Refresh Handler.  When a player refreshes, this
handler is called so that all the miscellaneous bookkeeping
about that player can be reset.  This should be used
for taking someone out of the playtesters' group, resetting the
criminal ledger in KLK, etc.  Everything, in short,
which is not a property on the player object or otherwise
can be handled entirely by the player object without using any other
objects.

Similarly, whenever a player gets deleted, this handler
gets called upon to call a number of deletion callbacks
to remove bookeeping about that player.

If you have code that stores information about a player outside of
the player object, then you should call register_refresh()
and possibly register_delete()
on this handler.  That will then remember the function you
give it.  Then, whenever a player refreshes or deletes, the
function you specified will get called with the player object
and a flag to indicate if this is a total refresh or a partial
refresh (for refresh callbacks) or a string with the player
name (for player deletions).  Do the right thing.
will get called with the player object and a flag to indicate
if this is a total refresh or a partial refresh.  Do the
right thing.

If your object moves to a different file, then you will
have to manually unregister the object from the refresh
handler.  This is very important!  To unregister, simply
call register_function() again, with the same object, but
give 0 for the function.

Each object can have only one refresh function registered,
and clones can't have any!

You can easily use the same function for both a deletion
and refresh callback.  All you do is declare the function
   void callback_func(mixed player, int flag)
and then test flag to see if it is PARTIAL_REFRESH,
TOTAL_REFRESH, or PLAYER_DELETED.  In the last case, player
will be a string.  Otherwise player is an object.

.EP
.SP 10 5


Written by Sin

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/refresh.h and /include/playerinfo.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^player_deleted%^RESET%^
.EI
.SI 5
varargs void player_deleted(string player)
.EI
.SP 7 5

This, like player_refreshed(), goes through and calls all the
registered deletion callbacks.  In this case, however, the
deletion callbacks get called with a _string_ rather than
an object.  That string is the name of the player who is being
deleted.

.EP

.SI 3
* %^BOLD%^player_refreshed%^RESET%^
.EI
.SI 5
varargs void player_refreshed(object player, int totally)
.EI
.SP 7 5

This function gets called by the player object whenever
a player refreshes, or by any of the various objects that delete
players when they get deleted.  You should never call
this function directly.

.EP

.SI 3
* %^BOLD%^query_funcs%^RESET%^
.EI
.SI 5
mapping query_funcs()
.EI
.SP 7 5

Return the current list of callback functions.

.EP

.SI 3
* %^BOLD%^register_delete%^RESET%^
.EI
.SI 5
string register_delete(mixed ob, string func)
.EI
.SP 7 5

This function does almost exactly what register_refresh() does,
but it is used for registering delete handlers.

Delete callbacks are called with two parameters as well, but
the first parameter is a string containing the player's name,
while the second parameter is the integer PLAYER_DELETED.

.EP

.SI 3
* %^BOLD%^register_refresh%^RESET%^
.EI
.SI 5
string register_refresh(mixed ob, string func)
.EI
.SP 7 5

Use this function to tell the refresh handler about functions that
you want to have called whenever any player refreshes or gets
deleted.

Typically, you will call this by hand, with the 'call' command,
rather than coding a call to this function into your code.

The first parameter is the object that contains the callback
to be called.  This can be either a pointer to the object, or
the filename to the object.

The second parameter is the name of the function that should be
called.  This callback should take two parameters: an object for
the player who is affected, and an integer, which will be either
PARTIAL_REFRESH or TOTAL_REFRESH.

This function will refuse to register a callback if either:
the object is a clone, the callback isn't defined in the object, or
the callback is a function pointer.

.EP

.SI 3
* %^BOLD%^stats%^RESET%^
.EI
.SI 5
mixed * stats()
.EI
.SP 7 5

This function supports the 'stat' command.  If you stat the
refresh handler, you'll see up to 4 things: the number of
refresh callbacks registered, the number of delete callbacks registered,
the person or object who loaded the refresh handler, and the
time that the handler got loaded.

.EP


