/******************************************************************************
 * This file contains the personnel related functions
 *****************************************************************************/

/**
 * @ignore yes 
 * Adds a new employee.
 * Also prevent the employee from receiving a bonus until they have
 * worked a full month by setting EMP_NOBONUS to 1.
 * @param player the employee to add
 */
private void add_employee(string player)
{
    if (!_employees[player])
    {
        _employees += ([player:EMP_MAP]);
        _employees[player][EMP_NOBONUS] = 1;
        set_emp_time(player);
        _got_bonus += ({player});
        save_me();
        save_emps();
    }
}
/* add_employee() */

/**
 * @ignore yes 
 * Demote a manager or supervisor.
 * @param demoter the person doing the demoting
 * @param demotee the person being demoted
 */
private void demote(string demoter, string demotee)
{
    int points = _employees[demotee][EMP_POINTS] & CLOCKED_IN;
    if (_employees[demotee][EMP_POINTS] & MANAGER)
        points += (SUPER_POINTS * 32) + EMPLOYEE + SUPERVISOR;      
    else points += EMPLOYEE;
    _employees[demotee][EMP_POINTS] = points;
    save_emps();
    PLAYER_SHOP->auto_mail(demotee, _proprietor, "Demotion", "",
      "This is to advise you that you have today been demoted.\n"
      "This demotion will now stay on your employment record.\n");
    employee_log(demotee, "Demoted by "+ demoter);
    shop_log(PERSONNEL, demoter, "demoted "+ cap_name(demotee), PAID);
}
/* demote() */

/**
 * @ignore yes 
 * Managers' office.
 * Commend employees.  Adds 5% of their promotion target.
 */
int do_commend(string emp)
{
    string commender;

    if (!_employees[emp])
    {
        tell_object(this_player(), cap_name(emp)+
          " is not an active employee!\n");
        return 1;
    }
    if (_employees[emp][EMP_POINTS] & MANAGER)
    {
        tell_object(this_player(), "You can't commend a manager.\n");
        return 1;
    }
    if (_employees[emp][EMP_POINTS] & NPC)
    {
        tell_object(this_player(), "Don't be silly!  "
          "You can't commend $C$"+ emp +".\n");
        return 1;
    }
    commender = this_player()->query_cap_name();
    AUTO_MAILER->auto_mail(emp, lower_case(commender), "Commendation",
      "", "This is to advise you that you have today received a "
      "commendation for outstanding service.\nThis will now stay on "
      "your employment record.\n");
    employee_log(emp, "Received a commendation from "+ commender);
    shop_log(PERSONNEL, commender, "commended "+ cap_name(emp), PAID);
    _employees[emp][EMP_POINTS] += (_employees[emp][EMP_POINTS] & SUPERVISOR)?
    to_int(MANAGER_POINTS * 0.05 * 32) : to_int(SUPER_POINTS * 0.05 * 32);
    save_emps();
    tell_object(this_player(), "You commend "+ cap_name(emp)+ ".\n");
    return 1;
}
/* do_commend() */

/**
 * @ignore yes 
 * Managers' office.
 * Demote supervisors or managers.
 */
int do_demote(string emp)
{
    object tp = this_player();
    emp = lower_case(emp);
    if (!_employees[emp])
    {
        tell_object(tp, cap_name(emp)+ " is not an active employee!\n");
        return 1;
    }
    if (_employees[emp][EMP_POINTS] & MANAGER && !tp->query_creator())
    {
        tell_object(tp, "You don't have the authority to "
          "demote $C$"+ emp +".\n");
        return 1;
    }
    if (!( _employees[emp][EMP_POINTS] & SUPERVISOR))
    {
        tell_object(tp, "Don't be silly!  You can't demote $C$"+
          emp +".\n");
        return 1;
    }
    demote(tp->query_cap_name(), emp);
    tell_object(tp, "You demote "+ cap_name(emp)+ ".\n");
    return 1;
}
/* do_demote() */

/**
 * @ignore yes 
 * Managers' office.
 * Fire an employee.
 */
int do_fire(mixed *args)
{
    args[0] = lower_case(args[0]);
    if (!_employees[args[0]])
    {
        tell_object(this_player(), cap_name(args[0])+
          " doesn't work at the shop!\n");
        return 1;
    }
    if ((member_array(args[0], _retired) != -1) ||
      query_manager(args[0]) && !this_player()->query_creator())
    {
        tell_object(this_player(), "You don't have the authority "
          "to fire $C$"+ args[0] +".\n");
        return 1;
    }
    if (_employees[args[0]][EMP_POINTS] & NPC)
    {
        tell_object(this_player(), "Don't be silly!  You can't fire $C$"+
          args[0] +".\n");
        return 1;
    }
    fire_them(this_player()->query_name(), args[0], args[1]);
    tell_object(this_player(), "You fire "+ cap_name(args[0])+
      " for "+ args[1]+ ".\n" );
    return 1;
}
/* do_fire() */

/**
 * @ignore yes 
 * Managers' office.
 * Place an employee on leave.
 */
int do_leave(mixed *args)
{
    object tp = this_player();
    args[0] = lower_case(args[0]);
    if (!_employees[args[0]])
    {
        tell_object(tp, cap_name(args[0])+ " is not an employee!\n");
        return 1;
    }
    if (args[1] > MAX_LEAVE)
    {
        tell_object(tp, "You cannot place an employee on leave "
          "for more than "+ MAX_LEAVE+ " days at a time.\n");
        return 1;
    }
    _times[args[0]] = time() + (args[1] * 86400);
    remove_call_out(_call_times);
    _call_times = call_out((: save_times() :), PERS_DELAY);
    add_succeeded_mess(cap_name(args[0])+
      " is on leave until "+ ctime(time() + (args[1] * 86400))+ ".\n");
    shop_log(PERSONNEL, tp->query_name(), "placed "+
      cap_name(args[0])+ " on leave for " + args[1]+ " days", PAID);
    employee_log(args[0], "Placed on leave by "+ tp->query_cap_name()+
      " for "+ args[1]+ " days.");
    tell_object(tp, "You place "+ cap_name(args[0])+
      " on leave for " + args[1]+ " days.\n");
    return 1;
}
/* do_leave() */

/**
 * @ignore yes 
 * This employee has requested to be passed over (or not) for promotion.
 */
private int do_promote(string on)
{
    object tp = this_player();
    add_succeeded_mess("");
    if (tp->query_creator())
    {
        tell_object(tp, "Creators don't get promoted.\n");
        return 1;
    }
    switch (on)
    {
    case "off" :
        _employees[tp->query_name()][EMP_NOPROMOTE] = TRUE;
        tell_object(tp, "You have now requested to be "
          "passed over for promotion.\n");
        break;
    case "on" :
        if (tp->query_property("no score"))
        {
            tell_object(tp, "Sorry, you cannot be promoted.\n");
            return 1;
        }
        _employees[tp->query_name()][EMP_NOPROMOTE] = FALSE;
        tell_object(tp, "You have now requested to be "
          "considered for promotion.\n");
        break;
    }
    save_emps();
    return 1;
}
/* do_promote() */

/**
 * @ignore yes 
 * Employee wishes to terminate their employment with the shop.
 */
private int do_resign()
{
    string word = this_player()->query_name();

    add_succeeded_mess("$N $V.\n");
    remove_employee(word);
    shop_log(PERSONNEL, word, "resigned", UNPAID);
    employee_log(word, "Resigned");
    return 1;
}
/* do_resign() */

/**
 * @ignore yes 
 * Managers' office.
 * Retire from management.
 */
int do_retire()
{
    string manager = this_player()->query_name();
    if (!(_employees[manager][EMP_POINTS] & MANAGER)) return 0;
    remove_employee(manager);
    _retired += ({manager});
    shop_log(PERSONNEL, manager, "retired from management", UNPAID);
    employee_log(manager, "Retired from management");
    save_me();
    add_succeeded_mess("$N retire$s.\n");
    return 1;
}
/* do_retire() */

/**
 * @ignore yes 
 * Managers' office.
 * Suspend employee's bonus for x months.
 */
int do_suspend(mixed *args)
{
    string suspender;
    object tp = this_player();

    args[0] = lower_case(args[0]);
    if (!_employees[args[0]])
    {
        tell_object(tp, cap_name(args[0])+ " is not an active employee!\n");
        return 1;
    }
    if ((_employees[args[0]][EMP_POINTS] & MANAGER) && 
      (!tp->query_creator()))
    {
        tell_object(tp, "You don't have the authority to "
          "suspend $C$"+ args[0] +"'s bonus.\n");
        return 1;
    }
    if (_employees[args[0]][EMP_POINTS] & NPC)
    {
        tell_object(tp, "Don't be silly!  "
          "You can't suspend $C$"+ args[0] +"'s bonus.\n");
        return 1;
    }
    suspender = tp->query_cap_name();
    _employees[args[0]][EMP_NOBONUS] = args[1];
    save_emps();
    AUTO_MAILER->auto_mail(args[0], _proprietor, "Suspended bonus", "",
      sprintf( "This is to advise you that you have had your bonus "
        "entitlement suspended for %d month%s.\nThis suspension will "
        "now stay on your employment record.\n", args[1],
        (args[1] == 1)?"":"s"));
    employee_log(args[0], sprintf("Bonus suspended for %d month%s by %s",
        args[1], (args[1] == 1)?"":"s", suspender));
    shop_log(PERSONNEL, suspender,
      sprintf("suspended %s's bonus for %d month%s", args[0],
        args[1], (args[1] == 1)?"":"s"), PAID);
    tell_object(tp, "You suspend "+ cap_name(args[0])+
      "'s bonus for "+ args[1]+ " months.\n");
    return 1;
}
/* do_suspend() */

/**
 * @ignore yes 
 * Managers' office.
 * Warn employees.  Removes 5% of their promotion target.
 */
int do_warn(mixed *args)
{
    string warner;
    object tp = this_player();
    int points;

    args[0] = lower_case(args[0]);
    if (!_employees[args[0]])
    {
        tell_object(tp, cap_name(args[0])+ " is not an active employee!\n");
        return 1;
    }
    if (_employees[args[0]][EMP_POINTS] & MANAGER && !tp->query_creator() )
    {
        tell_object( tp, "You don't have the authority to "
          "warn $C$"+ args[0] +".\n" );
        return 1;
    }
    if (_employees[args[0]][EMP_POINTS] & NPC)
    {
        tell_object(tp, "Don't be silly!  You can't warn $C$"+ args[0] +".\n");
        return 1;
    }
    warner = tp->query_cap_name();
    AUTO_MAILER->auto_mail(args[0], _proprietor, "Official warning", "",
      "This is to advise you that you have today received a formal "
      "warning for " + args[1] + ".\nThis warning will now stay on "
      "your employment record.\n");
    employee_log(args[0], "Received a warning from "+ warner+ 
      " for "+ args[1]);
    shop_log(PERSONNEL, warner, "warned "+
      cap_name( args[0] ) + " for "+ args[1], PAID);
    points = _employees[args[0]][EMP_POINTS] & CLOCKED_IN;
    if (_employees[args[0]][EMP_POINTS] & SUPERVISOR)
        _employees[args[0]][EMP_POINTS] -= to_int(MANAGER_POINTS * 0.05 * 32);
    else
    {
        _employees[args[0]][EMP_POINTS] -= to_int(SUPER_POINTS * 0.05 * 32) +
        EMPLOYEE;
        if (_employees[args[0]][EMP_POINTS] < 1)
            _employees[args[0]][EMP_POINTS] = EMPLOYEE + points;
    }
    save_emps();
    tell_object(tp, "You warn "+ cap_name(args[0])+ " for "+
      args[1]+ ".\n");
    return 1;
}
/* do_warn() */

/**
 * @ignore yes 
 * Used when employees are fired by managers, or automatically.
 * @param word the person doing the firing
 * @param them the person being fired
 * @param reason the reason for being fired
 */
private void fire_them(string word, string them, string reason)
{
    if (!_employees[them]) return;
    BANK_HANDLER->adjust_account(them, BANKS[_employees[them][EMP_BANK]][1],
      _employees[them][EMP_PAY]);
    shop_log(ACCOUNTS, _proprietor, "paid "+ 
      MONEY_HAND->money_value_string(_employees[them][EMP_PAY], _place)+
      " to "+ cap_name(them), UNPAID);
    shop_log(PERSONNEL, word, "fired "+ cap_name(them) +
      " for "+ reason, PAID);
    PLAYER_SHOP->auto_mail(them, word, _shop_name, "", 
      "Unfortunately, I have to inform you that you have today "
      "been fired for " + reason + ".  You have been paid the sum of "+
      MONEY_HAND->money_value_string( _employees[them][EMP_PAY], _place )+
      " for the work you have carried out to this date.\nIf you feel you "
      "have been unfairly dismissed, please refer to a manager.\n");
    employee_log(them, "Fired by "+ cap_name(word)+
      " for "+ reason);
    remove_employee(them);
}
/* fire_them() */

/**
 * @ignore yes 
 * Used when applicant has sufficient supporting votes to be accepted.
 * @param word the person to hire
 */
private void hire(string word)
{
    int gender;

    remove_applicant(word);

    /* Do not hire if not a user, already an employee, or banned */
    if (!test_player(word) || _employees[word] || query_baddie(word)) return;

    add_employee(word);
    employee_log(word, "Hired");
    shop_log(PERSONNEL, _proprietor, "hired "+ cap_name(word), UNPAID);
    PLAYER_SHOP->auto_mail(word, _proprietor, _shop_name, "", 
      "Congratulations!  You've been hired to work at "+ _shop_name+
      ".  You'll find that you can now move through the counter "
      "to the back areas of the shop.  The first things you should "
      "do are \"claim\" a new badge and staff handbook.\n");
    gender = PLAYER_HANDLER->test_gender(word);
    add_board_message("New employee", sprintf("%s has today been employed to "
        "work for the shop.  Please make %s feel welcome, and assist %s while "
        "%s gets started in %s new position.\n", cap_name(word),
        ({"it", "him", "her"})[gender] ,({"it", "him", "her"})[gender],
        ({"it", "he", "she"})[gender], ({"its", "his", "her"})[gender]));

    /* Update the other accepted applicants */
    remove_call_out(_call_mail_hirees);
    _call_mail_hirees = call_out((: mail_hirees() :), 5);
}
/* hire() */

/**
 * @ignore yes 
 * Used by the employee list to show managers & creators the last
 * time an employee was active.  Employees will be highlighted yellow
 * if they are currently on an inactivity warning, and red if they are
 * within 7 days of being fired/demoted
 * @param emp the employee to query
 */
private string query_worked(string emp)
{
    string blurb;

    /* Clocked in */
    if (_employees[emp][EMP_POINTS] & CLOCKED_IN)
        return " is currently clocked in";

    /* NPC */
    if (_employees[emp][EMP_POINTS] & NPC)
        return " has gone home for tea";

    /* On leave */
    if (_times[emp] > time())
        return " - %^CYAN%^on leave until "+ ctime(_times[emp])+ "%^RESET%^";

    blurb = " - last action ";

    if (_employees[emp][EMP_POINTS] & MANAGER)
    {
        if ((time() - _times[emp]) > ((60*60*24*MGR_DEMOTE)-7))
            blurb += "%^RED%^"; 
        else if ((time() - _times[emp]) > (60*60*24*MGR_WARN))
            blurb += "%^RED%^"; 
    }
    else if (_employees[emp][EMP_POINTS] & SUPERVISOR)
    {
        if (( time() - _times[emp]) > ((60*60*24*SPR_DEMOTE)-7))
            blurb += "%^RED%^"; 
        else if ((time() - _times[emp]) > (60*60*24*SPR_WARN))
            blurb += "%^YELLOW%^"; 
    }
    else if (( time() - _times[emp]) > ((60*60*24*EMP_FIRE)-7))
        blurb += "%^RED%^";
    else if ((time() - _times[emp]) > (60*60*24*EMP_WARN))
        blurb += "%^YELLOW%^";
    return blurb + ctime(_times[emp])+ "%^RESET%^";
}
/* query_worked() */

/**
* @ignore yes 
* Sets the last action time of an employee.
* This time is the last time an employee did something worth
* recording and is used to determine if they are inactive.
* @param employee The employee.
*/
private void set_emp_time(string employee)
{
    if (!_employees[employee]) return;
    if (_employees[employee][EMP_INACTIVE])
    {
        _employees[employee][EMP_INACTIVE] = 0;   // Reset inactivity flag
        save_emps();
    }
    if (!sizeof(_times)) _times = ([employee:0]);
    else if (!_times[employee]) _times += ([employee:0]);
    _times[employee] = time();
    remove_call_out(_call_times);
    _call_times = call_out((: save_times() :), PERS_DELAY);
}
/* set_emp_time() */

/**
 * @ignore yes 
 * View an employee's history or an applicant's application.
 * This method displays a formatted display of the employee's history
 * with a particular shop, and is viewable by managers of that shop.
 * If passed the name of an applicant, it will view the relevant application.
 * @param person The employee or applicant.
 */
void view_record(string person, string pattern)
{   
    if (pattern == VIEW_EMP)
    {
        string text = sprintf("Employment history of %s:\n\n", cap_name(person));
        load_history();
        if (!sizeof(_history) || !_history[person])
        {
            tell_object(this_player(), "There is no history for that person.\n");
            return;
        }
        for(int i = 0; i < sizeof(_history[person][0]); i++)
            text += sprintf("%s: %s\n", ctime(_history[person][0][i]),
              _history[person][1][i]); 
        tell_object(this_player(), sprintf("$P$%s's history$P$%s",
            cap_name(person), text));
        clear_history();
    }
    else
    {
        if (!query_applicant(person))
        {
            tell_object(this_player(), "That person has no application form "
              "on file.\n");
            return;
        }
        load_applicants();
        tell_object(this_player(),
          sprintf("$P$%s's history$P$Application of %s:\n\n%s", cap_name(person),
            cap_name(person), _applicants[person][APP_MESSAGE]));
        clear_applicants();
    }
}
/* view_record() */
