/******************************************************************************
 * This file contains saving-related functions
 *****************************************************************************/

/**
 * @ignore yes 
 * Save this object immediately.
 * Normal operations buffer saves to minimise disk activity.  This function is 
 * called to save immediately.
 */
private void do_save()
{
#ifdef DEBUG
   tell_creator(CREATOR,"Saving shop data to %s/.\n", _savedir);
#endif
   unguarded((: save_object, _savedir+ "shop_data" :));
}
/* do_save() */

/**
 * @ignore yes 
 * Saves the applications data.
 */
private void do_save_applicants()
{
   if (_very_short == UNSET_STR) return;
#ifdef DEBUG
   tell_creator(CREATOR,"Saving applications data to %s.\n",
     _savedir+ "applications");
#endif
   unguarded((: write_file, _savedir+ "applications",
     save_variable(_applicants), 1 :));
   clear_applicants();
}
/* save_applicants() */

/**
 * @ignore yes 
 * Saves the employee data.
 */
private void do_save_emps()
{
#ifdef DEBUG
   tell_creator(CREATOR,"Saving employee data to %s.\n",
     _savedir+ "employees");
#endif
   unguarded((: write_file, _savedir+ "employees",
     save_variable(_employees), 1 :));
}
/* do_save_emps() */

/**
 * @ignore yes 
 * Saves the employment history data.
 */
private void do_save_hist()
{
   if (_very_short == UNSET_STR) return;
#ifdef DEBUG
   tell_creator(CREATOR,"Saving history data to %s.\n",
     _savedir+ "history");
#endif
   unguarded((: write_file, _savedir+ "history",
     save_variable(_history), 1 :));
   clear_history();
}
/* save_hist() */

/**
 * @ignore yes 
 * Saves the new policies data.
 */
private void do_save_new_policies()
{
   if (_very_short == UNSET_STR) return;
#ifdef DEBUG
   tell_creator(CREATOR,"Saving new policies data to %s.\n",
     _savedir+ "new_policies");
#endif
   unguarded((: write_file, _savedir+ "new_policies",
     save_variable(_new_policies), 1 :));
   _call_newpols_clear = call_out((: _new_policies= 0 :), CLEAR_DELAY);
}
/* save_new_policies() */

/**
 * @ignore yes 
 * Saves the policies data.
 */
private void do_save_policies()
{
   if (_very_short == UNSET_STR) return;
#ifdef DEBUG
   tell_creator(CREATOR,"Saving policies data to %s.\n",
     _savedir+ "policies");
#endif
   unguarded((: write_file, _savedir+ "policies",
      save_variable(_policies), 1 :));
   clear_policies();
}
/* save_policies() */

/**
 * @ignore yes 
 * Saves the employment last-action times data.
 */
private void save_list()
{
   if (_very_short == UNSET_STR) return;
#ifdef DEBUG
   tell_creator(CREATOR,"Saving chart list data to %s.\n",
     _savedir+ "list");
#endif
   unguarded((: write_file, _savedir+ "list",
     save_variable(_list), 1 :));
}
/* do_save_list() */

/**
 * @ignore yes 
 * Saving the contents of the shop's register.
 * @param money the money array to save
 */
void save_register(mixed *money)
{
   _register = money;
   save_me();
}
/* save_register() */

/**
 * @ignore yes 
 * Saves the employment last-action times data.
 */
private void save_times()
{
   if (_very_short == UNSET_STR) return;
#ifdef DEBUG
   tell_creator(CREATOR,"Saving times data to %s.\n",
     _savedir+ "times");
#endif
   unguarded((: write_file, _savedir+ "times",
     save_variable(_times), 1 :));
}
/* save_times() */
