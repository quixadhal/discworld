/******************************************************************************
 * This file contains masked functions
 *****************************************************************************/

/**
 * @ignore yes 
 * Make sure all data is saved before desting this object.
 */
void dest_me()
{
   if (_very_short != UNSET_STR)
   {
      if (remove_call_out(_call_save) != -1) do_save();
      if (remove_call_out(_call_apps) != -1) do_save_applicants();
      if (remove_call_out(_call_hist) != -1) do_save_hist();
      if (remove_call_out(_call_newpols) != -1) do_save_new_policies();
      if (remove_call_out(_call_pols) != -1) do_save_policies();
      if (remove_call_out(_call_times) != -1) save_times();
      if (remove_call_out(_call_emps) != -1) do_save_emps();
   }
   if (_chart) _chart->dest_me();
   if (_board) _board->dest_me();
   if (_notice) _notice->dest_me();
   ::dest_me();
}
/* dest_me() */

/**
 * @ignore yes 
 * Someone has died.
 * This function will automatically fire an employee if they have
 * killed someone whilst on duty.  It will also make a note of anyone
 * who has killed an on-duty employee (including the npc shopkeeper).
 */
void event_death(object killed, object *others, object killer,
  string rmess, string kmess)
{
   /* Only look for players & the shopkeeper
    * We don't want to fire/ban people for killing a cabbage...
    */
   if (!killed->query_player() &&
     !(query_employee(killed->query_name()) & NPC)) return;
   if (!killer) return;

   if ((query_employee(killed->query_name()) & NPC)  || 
    (query_employee(killed->query_name())))
   {
      add_baddie(killer->query_name(), "the vicious assault on "+
        killed->query_name(), _proprietor);
      foreach(object baddie in others)
         if (baddie->query_player())
            add_baddie(baddie->query_name(),
              "the vicious assault on "+ killed->query_name(),
              _proprietor);
      return;
   }
   if (query_employee(killer->query_name()) & CLOCKED_IN)
      add_baddie(killer->query_name(), "the vicious assault on "+
        killed->query_name(), _proprietor);
   if (sizeof(others))
      foreach(object baddie in others)
      {
         if (!baddie->query_player()) continue;
         if (query_employee(baddie->query_name())& CLOCKED_IN)
            add_baddie(baddie->query_name(), "the vicious assault on "+
               killed->query_name(), _proprietor);
      }
}
/* event_death() */

/**
 * @ignore yes 
 * Someone has entered the room.
 * This function will automatically fire an employee if they have
 * teleported to this room.
 */
void event_enter(object ob, string message, object from)
{
   string room;

   if (!from || ob->query_creator() || from == find_object("/room/void") ||
     !ob->query_player()) return;
   room = file_name(from);
   if (room == _counter || room == _shop_front || room == _storeroom || 
     room == _mgr_office || from == this_object()) return;
#ifdef DEBUG
   tell_creator(CREATOR, "%s arrived from %O.\n", ob->query_name(), from);
#endif
   fire_them(_proprietor, ob->query_name(), "for teleporting into the shop");
}
/* event_enter() */

/**
 * @ignore yes 
 * init() - speaks for itself really
 */
void init()
{
   object tp = this_player();
   string word = tp->query_name();

   ::init();

   /*
    * Only employees & creators should have access to shop commands.
    */
   if (!tp->query_creator() && !_employees[word] &&
     (member_array(word, _retired) == -1)) return;
   add_command("claim", "{badge|handbook|bonus}", (: do_claim($4[0]) :));
   add_command("resign", "", (: do_resign() :));
   add_command("list", "", (: do_list() :));
   add_command("office", "", (: do_office() :));

   /*
    * If there is no board set up for this shop, all comms done by mail.
    */
   if (!_board) add_command("memo", "", (: do_memo() :));

   /*
    * Retired managers have no need for following commands.
    */
   if (member_array(word, _retired) != -1) return;
   add_command("clock", "{in|out}", (: do_clock($4[0]) :));
   add_command("bank", ({ "", "<number>" }), (: do_bank($4) :));
   if (!this_player()->query_property("no score"))
      add_command( "promotion", "{on|off}", (: do_promote($4[0]) :) );

   /*
    * Only supervisors & managers have access to following commands.
    */
   if (!tp->query_creator() &&  !(_employees[word][EMP_POINTS] & SUPERVISOR))
      return;
   add_command("chart", ({CHART_ADDITEM, CHART_REMITEM, CHART_MAXITEM,
      CHART_BUYITEM, CHART_SELLITEM, CHART_ASSIGN_ITEM, CHART_UNASSIGN_ITEM}),
     (: do_chart($4,$5) :));
   add_command("check", "cabinets", (: do_check() :));
   if (!tp->query_creator() && !(_employees[word][EMP_POINTS] & MANAGER))
      add_command( "logs", ({LOG_BLANK, LOG_NUMBER, LOG_CHART}),
      (: do_logs($4,$5) :));
   else 
      add_command("logs", ({LOG_BLANK, LOG_NUMBER,
        LOG_MGR}), (: do_logs($4,$5) :));
}
/* init() */

/**
 * @ignore yes 
 * Called when someone is refreshed or deleted
 * Will delete the employee in case of deletion or full refresh.  In case
 * of partial refresh, will reset employee to new employee status.
 */
void refresh_function(mixed employee, int flag)
{
   string emp_name;

   switch (flag)
   {
      case PLAYER_DELETED :
        emp_name = employee;
        break;
      case TOTAL_REFRESH :
      case PARTIAL_REFRESH :
        emp_name = employee->query_name();
        break;
   }

   if (query_applicant(emp_name))
   {
      remove_employee(emp_name);
      switch (flag)
      {
         case PLAYER_DELETED :
           shop_log(PERSONNEL, emp_name, "deleted character", UNPAID);
           employee_log(emp_name, "Deleted character");
           break;
         case TOTAL_REFRESH :
         case PARTIAL_REFRESH :
           tell_object(employee, "Your application at "+ _shop_name+ 
             " has been removed.\n");
           shop_log(PERSONNEL, emp_name, "refreshed character", UNPAID);
           employee_log(emp_name, "Refreshed character");
           break;
      }   
   }

   if (member_array(emp_name, _retired) != -1)
   {     
      remove_employee(emp_name);
      switch (flag)
      {
         case PLAYER_DELETED :
           shop_log(PERSONNEL, emp_name, "deleted character", UNPAID);
           employee_log(emp_name, "Deleted character");
           break;
         case TOTAL_REFRESH :
         case PARTIAL_REFRESH :
           tell_object(employee, "Your employment at "+ _shop_name+ 
             " has been terminated.\n");
           shop_log(PERSONNEL, emp_name, "refreshed character", UNPAID);
           employee_log(emp_name, "Refreshed character");
           break;
      }   
   }

   if (!_employees[emp_name]) return;

   switch (flag)
   {
      case PLAYER_DELETED :
        shop_log(PERSONNEL, emp_name, "deleted character", UNPAID);
        employee_log(emp_name, "Deleted character");
        remove_employee(emp_name);
        break;
      case TOTAL_REFRESH :
        tell_object(employee, "Your employment at "+ _shop_name+ 
          " has been terminated.\n");
        shop_log(PERSONNEL, emp_name, "refreshed totally", UNPAID);
        employee_log(emp_name, "Refreshed totally");
        remove_employee(emp_name);
        break;
      case PARTIAL_REFRESH :
        tell_object(employee, "Your employment level at "+ _shop_name+ 
          " has been reset.\n");
        shop_log(PERSONNEL, emp_name, "refreshed character", UNPAID);
        employee_log(emp_name, "Refreshed character");
        _employees[emp_name] = EMP_MAP;
        set_emp_time(emp_name);
        _got_bonus += ({emp_name});
        save_me();
        break;
   }
}
/* refresh_function() */   

/**
 * @ignore yes 
 * Add standard stuff
 */
protected void set_long(string long_desc)
{
   string ob_file = file_name(this_object());

   /***************************
    * Add the shop's item chart
    **************************/
   _chart = clone_object(SHOP_CHART);
   _chart->set_office(ob_file);
   add_hidden_object(_chart);

   /******************************
    * Add the shop's policy notice
    *****************************/
   _notice = clone_object(SHOP_NOTICE);
   _notice->set_office(ob_file);
   add_hidden_object(_notice);
   long_desc += "Employees may \"clock\" in and out of work here "
     "and examine the items chart.  There is also a policy notice "
     "located next to the chart.\n";
   ::set_long(long_desc);
}
/* set_long() */
