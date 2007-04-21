/**
* This is a very boring effect that does nothing but add a shadow to
* the npc, preventing one or more of his skills from being mocked.
* The skills need not be leaves - branches are allowed.  Ie: "magic"
* will block all ma.* skills, "magic.spells" will block ma.sp.*, etc.
* Also, if "all" is included in the array, all skills will be blocked
* from being mocked.
*
* The shadow has a few functions in to query the list.
*
* Pass add_effect one arg: string *skills - the skills to block
* 
* This effect has one arg: *string skills - the skills blocked.
* This effect is indefinite.
* This effect has a shadow associated with it.
* This effect has a classification of "mock.block.npc"
*
* @author Tannah
**/

#include "path.h"

/** @ignore yes */
string *beginning( object player, string *skills ) { 
  return skills;
} /* beginning() */

/* restart or end needed */

/** @ignore yes */
string *merge_effect( object player, string *old, string *arg ) {

  /* Might as well take the duplicates out, even though it's not really 
   * necessary. */
  arg = filter( arg, (: member_array( $1, $2 ) == -1 :), old );

  return old + arg;
} /* merge_effect() */

/** @ignore yes */
string query_classification() { return "mock.block.npc"; }

/** @ignore yes */
string query_shadow_ob() { return SHADOWS "mock_block"; }

/** @ignore yes */
int query_indefinite() { return 1; }
