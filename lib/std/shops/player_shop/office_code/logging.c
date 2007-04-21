/******************************************************************************
 * This file contains log handling functions
 *****************************************************************************/

/**
 * @ignore yes 
 * Display the shop's logs.
 */
int do_logs(mixed *args, string pattern)
{
    string start, end, file, *files, words;

    add_succeeded_mess("");
    switch (pattern)
    {   
    case LOG_BLANK :
        /* Display available logs */
        files = get_dir(_savedir +"general.log*");
        words = "Available logs:\n\n";
        if (sizeof(files))
            words += " 1: current log\n";
        for (int i = sizeof(files); i > 1; i--)
        {
            sscanf(unguarded((: read_file, _savedir + files[i-1], 2, 1 :)),
              "%*s, %s:", start);
            sscanf(unguarded((: read_file, _savedir + files[i-1],
                  file_length(_savedir + files[i-1]), 1 :)), 
              "%*s, %s: %*s", end );
            if (start == end)
                words += sprintf("%2d: %s\n",
                  sizeof(files)-(i-2), start);
            else
                words += sprintf("%2d: %s to %s\n",
                  sizeof(files)-(i-2), start, end);
        }
        words += "\nUse \"logs <number>\" to read one of them, or \"logs ";
        if ((_employees[this_player()->query_name()] && 
            _employees[this_player()->query_name()][EMP_POINTS] & MANAGER) ||
          this_player()->query_creator())
        {
            words += "{personnel|accounts|chart}\" to view "
            "those logs.\n";
        }
        else
        {
            words += "chart\" to view that log.\n";
        }
        tell_object(this_player(),"$P$Logs$P$"+ words);
        break;
    case LOG_NUMBER :
        /* Display specific log */
        files = get_dir(_savedir +"general.log*");
        if ((args[0] < 1) || (args[0] > sizeof(files)))
        {
            tell_object(this_player(), "There are "+ sizeof(files) +
              " logs.  Use \"logs\".\n");
            return 1;
        }
        if (args[0] == 1)
            sscanf(unguarded((: read_file, _savedir +
                  files[0] :)), "%*s\n%s", words);
        else
            sscanf(unguarded((: read_file, _savedir +
                  files[sizeof(files) - (args[0]-1)] :)), "%*s\n%s", words);
        tell_object(this_player(),"$P$Log "+ args[0]+ "$P$"+ words);
        break;
    case LOG_CHART :
        /* Display chart log */
        file = _savedir + "chart.log";
        if (file_size(file) > 0)
        {
            sscanf(unguarded((: read_file, file :)), "%s", words);
            tell_object(this_player(),"$P$Chart log$P$"+ words);
        }
        else
        {
            tell_object(this_player(), "The chart log is empty.\n");
            return 1;
        }
        break;
    case LOG_MGR :
        /* Display special log */
        file = _savedir + args[0] + ".log";
        if (file_size( file ) > 0)
        {
            sscanf(unguarded((: read_file, file :)), "%s", words);
            tell_object(this_player(),"$P$"+ capitalize(args[0])+
              " log$P$"+ words);
        }
        else
        {
            tell_object(this_player(), "The "+ args[0]+ " log is empty.\n");
            return 1;
        }
        break;
    }
    return 1;
}
/* do_logs() */

/**
 * @ignore yes 
 * Adds an entry to the employee's history.
 * This method is intended to log relevant personnel issues such as
 * applications, hirings, promotions etc.  The normal day-to-day
 * stuff is logged in the shop's logs themselves.
 * @param employee The employee.
 * @param note The note to log.
 */
private void employee_log(string employee, string note)
{
    load_history();
    if (!sizeof(_history)) _history = ([employee:({({}),({}),})]);
    else if (!_history[employee]) _history += ([employee:({({}),({}),})]);

    _history[employee][0] += ({time()});
    _history[employee][1] += ({note});
    save_hist();

    remove_call_out(_call_times);
    if (!sizeof(_times)) _times = ([employee:0]);
    else if (!_times[employee]) _times += ([ employee:0 ]);

    _times[employee] = time();   // Update employee record
    _call_times = call_out((: save_times() :), PERS_DELAY);
}
/* employee_log() */

/**
 * @ignore yes 
 * This function adds an entry to the logs & pays employees.
 * Logs record a full day's (3 DW days) activity, apart from the
 * chart, accounts & personnel logs which use log_file.<br>
 * @param logtype The type of log entry - see <player_shop.h>
 * @param word The employee making the entry.
 * @param words The log entry text.
 * @param paid Should the employee be paid for this entry
 */
void shop_log(int logtype, string word, string words, int paid)
{
    string date, month, colour;

    sscanf(amtime(time()), "%*s %*s %*s %s %*s", month);
    if (file_size(_savedir +"general.log") > 0)
    {
        date = ctime(time())[0 .. 9];

        /*
         * Are we on a new day yet?  If so, do the daily review.
         */
        if (date != unguarded((: read_file,
              _savedir +"general.log", 1, 1 :))[0 .. 9])
        {
            string logfile = _savedir +"general.log-"+ time(),
            summary = sprintf( "%sFor the period ending %s:%s\n",
              "%^BOLD%^", amtime(time()), "%^RESET%^" ),
            sign = "";

            _storeroom->force_load();
            call_out((: update_averages() :), 60);
            unguarded((: rename, _savedir +"general.log", logfile :));
            call_out((: review_employees() :), 5 );
            foreach (string item in m_indices(_list))
            {
                summary += sprintf(" %s - Bought %d, Sold %d\n", capitalize(item),
                  _list[item][CHART_BOUGHT], _list[item][CHART_SOLD]);
                _list[item][CHART_AVESALE] = (_list[item][CHART_AVESALE] +
                  _list[item][CHART_SOLD] + random(2)) / 2;
                _list[item][CHART_SOLD] = 0;
                _list[item][CHART_BOUGHT] = 0;
            }
            save_list();
            if (_net_takings < 0)
            {
                _net_takings = -_net_takings;
                sign = "-";
            }
            summary += sprintf("%sThe net takings of the shop were %s%s.%s\n\n",
              "%^BOLD%^", sign, MONEY_HAND->money_value_string(_net_takings,
                _place), "%^RESET%^");
            _net_takings = 0;
            save_me();
            unguarded((: write_file, _savedir +"general.log",
                date +"\n" + summary :));

            /*
             * Remove old logs.
             */
            foreach (string file in get_dir(_savedir +"general.log-*"))
            {
                sscanf(file, "%*s-%s", date);
                if (time() - to_int(date) > (60*60*24*LOG_DAYS))
                    unguarded((: rm, _savedir + "general.log-"+ date :));
            }
        }
    }

    /*
     * Are we in a new month yet?  If so do the monthly review.
     */
    if (month != _last_month && !_call_review)
    {
        if (member_array(month, ({"Offle", "February", "March",
              "April", "May", "June", "Grune", "August", "Spune",
              "Sektober", "Ember", "December", "Ick"})) != -1)
        {
            _review_month = month;
            remove_call_out(_call_review);
            _call_review = call_out((: monthly_review() :), 60);
        }
    }

    /*
     * Only pay employees if clocked in & this is a paid action
     */
    if (_employees[word])
    {
        if (!(_employees[word][EMP_POINTS] & NPC))
            _employees[word][EMP_EOM]++;
        if (paid && _employees[word][EMP_POINTS] & CLOCKED_IN)
        {
            if (_employees[word][EMP_POINTS] & MANAGER)
                _employees[word][EMP_PAY] += (_pay_val * 2);
            else if (_employees[word][EMP_POINTS] & SUPERVISOR ||
              _eom == word)
            {
                _employees[word][EMP_PAY] += to_int(_pay_val * 1.5);
                _employees[word][EMP_POINTS] += 32;
            }
            else
            {
                _employees[word][EMP_PAY] += _pay_val;
                _employees[word][EMP_POINTS] += 32;
            }
        }
        set_emp_time(word);
        save_emps();
    }

    /*
     * Write entry to the appropriate log.
     */
    switch (logtype)
    {
    case PURCHASE :
        colour = "%^GREEN%^";
        break;
    case SALE :
        colour = "%^RED%^";
        break;
    case GENERAL :
        colour = "%^CYAN%^";
        break;
    case PERSONNEL :
        log_file(_savedir+ "personnel.log", "%s: %s - %s\n",
          amtime(time()), cap_name(word), words);
        return;
        break;
    case ACCOUNTS :
        log_file(_savedir+ "accounts.log", "%s: %s - %s\n",
          amtime(time()), cap_name(word), words);
        return;
        break;
    case CHARTLOG :
        log_file(_savedir+ "chart.log", "%s: %s - %s\n",
          amtime(time()), cap_name(word), words);
        return;
        break;
    default :
        colour = "%^RESET%^";
        break;
    }
    unguarded((: write_file, _savedir +"general.log", colour + amtime(time())+
        ": "+ cap_name(word) +"%^RESET%^ - "+ words +"\n" :));
}
/* shop_log() */
