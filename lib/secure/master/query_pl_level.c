/*
 * There are several occasions when the game driver wants to check if
 * a player has permission to specific things.
 *
 * These types are implemented so far:
 * "error messages":	If the player is allowed to see runtime error
 *			messages.
 * "trace":		If the player is allowed to use tracing.
 * "wizard":		Is the player considered at least a "minimal" wizard ?
 * "error messages":	Is the player allowed to get run time error messages ?
 */
int query_player_level(string what) {
  int level;

  if (this_player() == 0)
    return 0;
  switch(what) {
  case "wizard":
    return (int)this_player()->query_creator();
  case "error messages":
    return (int)this_player()->query_app_creator();
  case "trace":
    return (int)this_player()->query_creator();
  }
} /* query_player_level() */
