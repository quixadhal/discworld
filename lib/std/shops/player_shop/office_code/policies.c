/******************************************************************************
 * This file contains policy-related functions
 *****************************************************************************/

/**
 * @ignore yes 
 * Update policies in force.
 * Add policy if doesn't exist, or remove it if it does.
 * @param policy the policy to update
 */
private void add_policy(string policy)
{
   load_policies();
   load_new_policies();
   if (!_policies[policy])
   {
      _policies += ([policy:({"",0})]);
      _policies[policy][0] = _new_policies[policy][POLICY_TEXT];      
      _policies[policy][1] = _new_policies[policy][POLICY_TYPE];      
      PLAYER_SHOP->auto_mail(_new_policies[policy][POLICY_MGR], _proprietor,
        "Policy proposition - "+ policy, "", "Your proposition has been "
        "accepted by majority vote and is now policy.\n" );
      shop_log(GENERAL, "Shop", "accepted the "+ policy + " policy.", UNPAID);
   }
   else
   {
      map_delete(_policies, policy);
      PLAYER_SHOP->auto_mail(_new_policies[policy][POLICY_MGR], _proprietor,
        "Policy proposition - "+ policy, "",
        "Your proposition to remove the above policy has been accepted "
        "by majority vote.\n");
      shop_log(GENERAL, "Shop", "removed the "+ policy + " policy.", UNPAID);
   }
   map_delete(_new_policies, policy);
   save_policies();
   save_new_policies();
}
/* add_policy() */

/**
 * @ignore yes 
 * Add a new policy suggestion.
 * @param name the name of the policy
 * @param text the body text of the policy
 * @param mgr the manager proposing the policy
 */
void add_policy_suggest(string name, string text, string mgr, int type)
{
   if (previous_object() && previous_object() != find_object(_mgr_office))
   {
      LOG_ERROR("office.c", "add_policy_suggest("+name+","+text+","+mgr+")");
      return;
   }
   load_new_policies();
   if (!sizeof(_new_policies)) _new_policies = ([ name:POLICY_MAP ]);
   else _new_policies += ([ name:POLICY_MAP ]);
   load_policies();
   if (sizeof(_policies) && _policies[name]) text = _policies[name][0];
   _new_policies[name][POLICY_TIME] = time();
   _new_policies[name][POLICY_MGR] = mgr;
   _new_policies[name][POLICY_TEXT] = text;
   _new_policies[name][POLICY_TYPE] = type;
   do_policy_vote(mgr, name, "y");
   clear_policies();
   save_new_policies();
} 
/* add_policy_suggest() */

/**
 * @ignore yes 
 * Add a vote
 * @param mgr the voting manager
 * @param policy the policy being voted upon
 */
private int do_policy_vote(string mgr, string policy, string decision)
{
   int managers;

   load_new_policies();
   if (!m_sizeof(_new_policies) || !_new_policies[policy])
   {
      tell_object(this_player(), "There is no such policy pending!\n");
      return 1;
   }
   if (member_array(mgr, _new_policies[policy][POLICY_FOR]) != -1 ||
     member_array(mgr, _new_policies[policy][POLICY_AGAINST]) != -1 )
   {
      tell_object(this_player(), "You have already voted on this policy!\n");
      return 1;
   }
   if (decision == "y")
   {
      _new_policies[policy][POLICY_FOR] += ({mgr});
      tell_object(this_player(), "You vote for the " + policy+ " policy.\n");
   }
   else
   {
      _new_policies[policy][POLICY_AGAINST] += ({mgr});
      tell_object(this_player(), "You vote against the "+
         policy+ " policy.\n");
   }
   shop_log(GENERAL, mgr, "voted on the "+ policy + " policy.", PAID);
   save_new_policies();
   managers = sizeof(get_managers()) + sizeof(get_retired());
   if (sizeof(_new_policies[policy][POLICY_FOR]) > managers / 2)
      add_policy(policy);
   else if (sizeof(_new_policies[policy][POLICY_AGAINST]) >= managers / 2)
      remove_policy(policy);
   return 1;
}
/* do_policy_vote() */

/**
 * @ignore yes
 * Query the shop's main policy - used for the notice.
 * @return the main policy of the shop
 */
string get_stock_policy() { return copy(_stock_policy); }

/**
 * @ignore yes 
 * Policy update has been unsuccessful.
 * @param the policy that was rejected
 */
private void remove_policy(string policy)
{
   load_new_policies();
   PLAYER_SHOP->auto_mail(_new_policies[policy][POLICY_MGR],
     "Tarnach Fendertwin", "Policy proposition - "+ policy, "",
     "Your proposition has been rejected by majority vote.\n");
   shop_log(GENERAL, "Shop", "rejected the "+ policy + " policy.\n", UNPAID);
   map_delete(_new_policies, policy);
   save_new_policies();
}
/* remove_policy() */
