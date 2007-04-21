/*****************************************************************************
 * This file contains some list functions
 *****************************************************************************/

/**
 * @ignore yes 
 * A nicely formatted employee list for the employee badge.
 */
string badge_list()
{
   string *results = ({}),
          *creators = ({});
 
   foreach(object ob in children(BADGE))
   {
      if (ob == find_object(BADGE)) continue;
      if (ob->query_channel() != _channel) continue;
      if (environment(ob)->query_creator())
         creators += ({ environment(ob)->query_name() });
   }
   if (member_array(CREATOR, creators) != -1)
   {
      object ob = find_player(CREATOR);
      creators -= ({CREATOR});
      if (ob && !ob->query_invis())
      {
         string temp_result = ob->query_cap_name() +
            "%^YELLOW%^ (C)%^RESET%^";
         results += ({temp_result});
      }
   }
   if (sizeof(creators))
      foreach(string word in sort_array(creators, 1))
      {
         string temp_result = cap_name(word) +
            "%^YELLOW%^ (C)%^RESET%^";
         results += ({temp_result});
      }
   foreach (string word in sort_array(_retired, 1))
   {
      object ob;

      if (ob = find_player(word))
      {
         string temp_result = "%^CYAN%^"+ ob->query_cap_name() +
            "%^RESET%^ (R)";
         int idle = 0;

         if (interactive(ob))
            idle = query_idle(ob);
         else temp_result += " [net-dead]";

         if (idle > 90)
            temp_result += sprintf(" [idle: %d:%02d]",
               idle  / 60, idle % 60);
         results += ({temp_result});
      }
   }
   foreach (string word in get_managers())
   {
      string temp_result = "";
      object ob;

      if (ob = find_player(word))
      {
         int idle = 0;

         if (interactive(ob))
            idle = query_idle(ob);
         else temp_result += " [net-dead]";

         if (_employees[word][EMP_POINTS] & CLOCKED_IN)
            temp_result = "%^YELLOW%^";
         temp_result += ob->query_cap_name()+ "%^RESET%^ (m)";
         if (idle > 90)
            temp_result += sprintf(" [idle: %d:%02d]",
               idle  / 60, idle % 60);
         results += ({temp_result});
      }
   } 
   foreach (string word in get_supervisors())
   {
      string temp_result = "";
      object ob;

      if (ob = find_player(word))
      {
         int idle = 0;

         if (interactive(ob))
            idle = query_idle(ob);
         else temp_result += " [net-dead]";

         if (_employees[word][EMP_POINTS] & CLOCKED_IN)
            temp_result = "%^YELLOW%^";
         temp_result += ob->query_cap_name()+ "%^RESET%^ (s)";
         if (idle > 90)
            temp_result += sprintf(" [idle: %d:%02d]",
               idle  / 60, idle % 60);
         results += ({temp_result});
      }
   } 
   foreach (string word in get_employees())
   {
      string temp_result = "";
      object ob;

      if (ob = find_player(word))
      {
         int idle = 0;

         if (interactive(ob))
            idle = query_idle(ob);
         else temp_result += " [net-dead]";

         if (_employees[word][EMP_POINTS] & CLOCKED_IN)
            temp_result = "%^YELLOW%^";
         temp_result += ob->query_cap_name()+ "%^RESET%^";
         if (idle > 90)
            temp_result += sprintf(" [idle: %d:%02d]",
               idle  / 60, idle % 60);
         results += ({temp_result});
      }
   }
   if (!sizeof(results))
      /* Will only occur if no employees logged on, and CREATOR is invis.
       */
      return "\nJust you, I'm afraid.\n";
   return sprintf("\n%s\n", query_multiple_short(results));
}
/* badge_list() */

/**
 * @ignore yes 
 * The list of employees in the shop.
 * This list is displayed sorted primarily by status (Retired, manager,
 * supervisor, riff-raff) and then alphabetically within those groups.
 * It also highlights those employees currently logged on (as long as they
 * are visible).
 */
int do_list()
{
   int emps_in;
   string results = "   Employees of "+ _shop_name + "\n"
                    "     As at " + ctime(time()) + "\n\n";

   /* Retired managers */
   foreach (string word in sort_array(_retired, 1))
   {
      object ob = find_player(word);
      if (ob)
         results += "%^GREEN%^";
      results += "     "+ cap_name(word)+ 
      "%^RESET%^ (retired manager)\n";
   }

   /* Managers */
   foreach (string word in get_managers())
   {
      object ob = find_player(word);
      if (ob)
      {
         emps_in++;
         results += "%^GREEN%^";
      }
      results += "     "+ cap_name(word)+ "%^RESET%^ (manager)"+
        list_stuff(word);
   }

   /* Supervisors */
   foreach (string word in get_supervisors())
   {
      object ob = find_player(word);
      if (ob)
      {
         emps_in++;
         results += "%^GREEN%^";
      }
      results += "     "+ cap_name(word)+ "%^RESET%^ (supervisor)"+
        list_stuff(word);
   }

   /* Riff-raff */
   foreach (string word in get_employees())
   {
      object ob = find_player(word);
      if (ob)
      {
         emps_in ++;
         results += "%^GREEN%^";
      }
      results += "     "+ cap_name(word)+ "%^RESET%^"+ list_stuff(word);
   }

   results += "\n";

   /* If viewed by manager or creator, display number of employees logged
    * on and number clocked in.
    */
   if (this_player()->query_creator() || 
     _employees[this_player()->query_name()][EMP_POINTS] & MANAGER)
   {
      results += (emps_in == 1)?"   There is one employee ":
                 "   There are "+ query_num(emps_in,0)+ " employees "; 
      results += "out of "+ query_num(sizeof(_employees) - 1)+
        " on " + mud_name() + " at the moment, with "+ query_num(num_employees_in(),
          0) + " currently clocked in.\n\n";
   }

   tell_object(this_player(), "$P$Employees$P$"+ results);
   add_succeeded_mess("");
   return 1;
}
/* do_list() */

/**
 * @ignore yes 
 * Displays the list of commands available to each employee.
 */
private int do_office()
{
   int employee;
   object tp = this_player();
   string tp_name = tp->query_name(),
          board_mess = "";

   if (!_board)
      board_mess = "   memo      - send a memo to the other employees\n";
   add_succeeded_mess("");
   tp = this_player();
   tp_name = tp->query_name();
   if (member_array(tp_name, _retired) != -1)
   {
      tell_object( tp, "As a retired manager, you can use the following "
        "commands:\n"
        "   claim     - claim a badge or handbook\n"
        "   list      - list the other employees of the shop\n"
        "   logs      - review the shop's logs\n"
        "   resign    - terminate your association with Tarnach's\n"+
        board_mess+
        "You may also enter the managers' office for more commands.\n" );
      return 1;
   }
   employee = (tp->query_creator())?SUPERVISOR + MANAGER:
              _employees[tp_name][EMP_POINTS];
   tell_object(tp, "As an employee, you can use the following commands:\n"
     "   clock     - start or finish your working day\n"+
     "   claim     - claim a badge, handbook, or this month's bonus\n"
     "   bank      - set to which bank account you are paid\n"
     "   list      - list the other employees of the shop\n"
     "   promotion - request to be ignored for promotion\n"
     "   resign    - end your employment at the shop\n"+
     board_mess );
   if (employee & SUPERVISOR)
      tell_object( tp, "\nAs a supervisor, you can also use:\n"
        "   chart   - change the information on the sales chart\n"
        "   check   - check cabinet assignments\n"
        "   logs    - review the shop's logs\n" );
   if (employee & MANAGER)
      tell_object( tp, "\nAs a manager you can enter the managers' "
        "office and use the commands listed there.\n" );
   add_succeeded_mess("");
   return 1;
}
/* do_office() */

/**
 * @ignore yes 
 */
string employees_clocked_in()
{
   object *words = ({}), ob;

   if (!num_employees_in()) words = ({"No employees"});
   if (find_object(_shopkeeper) &&
      _employees[_shopkeeper->query_name()][EMP_POINTS] & CLOCKED_IN)
      words = ({_shopkeeper->query_short()});
   foreach (string word in m_indices(_employees))
   {
      ob = find_player(word);
      if (ob && _employees[word][EMP_POINTS] & CLOCKED_IN)
            words += ({ob->query_cap_name()});
   }
   return query_multiple_short(words);
}
/* employees_clocked_in() */

/**
 * @ignore yes 
 * Displays an employee's percentage towards promotion, if applicable.
 * Displays all employees' scores to creators.
 * Displays last-action (leave) times to managers.
 */
private string list_stuff(string word)
{
   string results = "",
          tp = this_player()->query_name();

   /* Nothing to display for retired managers */
   if (member_array(word, _retired) != -1) return "\n";

   /* Npc shopkeeper */
   if (_employees[word][EMP_POINTS] & NPC) return query_worked(word) + "\n";

   /* Creator viewing - display all scores */
   if (this_player()->query_creator())
      results = " ["+ (_employees[word][EMP_POINTS] / 32) +"]";

   else if ((word == tp) && !(_employees[word][EMP_POINTS] & MANAGER))
      if (_employees[word][EMP_POINTS] & SUPERVISOR)
         results = " ["+ ((100 * (_employees[word][EMP_POINTS] /
           32 - SUPER_POINTS)) /
           (MANAGER_POINTS - SUPER_POINTS)) +"%]";
      else
         results = " ["+ ((100 * (_employees[word][EMP_POINTS] / 32)) /
           SUPER_POINTS) +"%]";

   /* Last-action times displayed to managers & creators */
   if (this_player()->query_creator() || (_employees[tp][EMP_POINTS] & MANAGER))
      results += query_worked(word);
   return results + "\n";
}
/* list_stuff() */
