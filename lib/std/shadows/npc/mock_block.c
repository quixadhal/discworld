/**
* This shadow goes with the "mock.block.npc" effects and adds functions
* that allow its args to be manipulated.
* 
* @author Tannah
**/

inherit "/std/effect_shadow";

/**
* Removes a skill from the list of those blocked from mocking and returns
* the list of those still blocked.
* @param skills - the skills to remove from the list of those blocked.
* @return skills - those skills still blocked from mocking
**/

string *unblock_skills( string *skills ) {
  set_arg( arg() - skills );
  
  /* was that the last skill?  If so, remove the effect */
  if( !sizeof( arg() ) ) {
    remove_this_effect();
    return ({});
  }
  return arg();
} /* unblock_skills() */

/**
* Determines whether or not a skill is blocked from being mocked.  mocked 
* can be a leaf (ie: "magic.methods.mental.charming") or a branch (ie: 
* "magic" or "magic.spells").  If all skills are blocked, this always 
* returns 1.  
* @param mocked - the skill to compare to the list of those blocked.
* @return 1 if the skill is blocked from mocking, 0 if not.
**/

int query_mock_block( string mocked ) {
  
  /* Are all skills blocked from mocking?  Easy! */
  
  if( member_array( "all", arg() ) != -1 ) {
    return 1;
  }
  
  /* check each skill that's blocked */
  foreach( string skill in arg() ) {
    if( !strsrch( mocked, skill ) ) {
      return 1;
    }
  }
} /* query_mock_block() */