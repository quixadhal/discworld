/******************************************************************************
 * This file contains employee commands
 *****************************************************************************/


/**
 * @ignore yes 
 * Display or update the employee's bank details.
 */
private int do_bank(mixed *args)
{
   object tp = this_player();
   string word = tp->query_name();

   if (tp->query_creator())
   {
      tell_object(tp, "Creators don't get paid.\n");
      return 1;
   }
   add_succeeded_mess( "" );

   if ( !sizeof( args ) )
   {      
      string message = "You are currently being paid into your account at "+
        BANKS[_employees[word][EMP_BANK]][0]+ ".  To change which bank "
        "you would like your pay to be deposited at, use the command "
        "\"bank <number>\" where <number> is one of the following:\n\n";
      for (int x = 0; x < sizeof(BANKS); x++)
         message += sprintf("     %d: %s\n", x+1, BANKS[x][0]);
      tell_object(tp, message + "\n");
      return 1;
   }
   if (args[0] < 1 || args[0] > sizeof(BANKS))
   {
      tell_object(tp,  "There are "+ sizeof(BANKS)+ 
        " banks to choose from.\n");
      return 1;
   }
   _employees[word][EMP_BANK] = args[0] - 1;
   save_emps();
   tell_object(tp, "You change your bank details.\n");
   return 1;
}
/* do_bank() */


/**
 * @ignore yes 
 * Claim a badge, handbook, or bonus.
 */
private int do_claim(string item)
{
   object thing,
          tp = this_player();
   int give_bonus;
   string tp_name = tp->query_name();

   switch(item)
   {
     case "badge" :
        thing = clone_object(BADGE);
        thing->set_props(file_name(this_object()), tp->query_name());
        break;
     case "handbook" :
        thing = clone_object(STAFF_BOOK);
        thing->set_read_mess("\n   "+ _shop_name+ "\n\n"
          "   Staff Handbook\n\n   This handbook belongs to: "+
          tp->query_short()+ "\n\n", "common", 100);
        thing->set_office(file_name(this_object()));
        tp->add_property(_very_short+ " handbook",1);
        break;
     case "bonus" :
        /* Already claimed, new employee, or had bonus suspended. */
        if (member_array(tp_name, _got_bonus) != -1)
        {
           tell_object(tp, "You are not entitled to a "
             "bonus this month!\n");
           return 1;
        }

        /* Value depends on status */
        if (_employees[tp_name][EMP_POINTS] & MANAGER)
        {
           give_bonus = _bonus_val * 2;
        }
        else if (_employees[tp_name][EMP_POINTS] & SUPERVISOR ||
           _eom == tp_name)
        {
           give_bonus = to_int(_bonus_val * 1.5);
        }
        else
        {
           give_bonus = _bonus_val;
        }

        /* Nothing to give */
        if (!give_bonus)
        {
           tell_object(tp,  "There is no money in the bonus "
             "fund this month.\n");
           return 1;
        }

        /* Prevent them from claiming again this month */
        _got_bonus += ({tp_name});
        _bonus -= give_bonus;

        /* Update value of unclaimed bonus */
        if (_bonus < 0) _bonus = 0;

        thing = MONEY_HAND->make_new_amount(give_bonus, _place);
        shop_log(GENERAL, tp_name, "claimed "+ 
          MONEY_HAND->money_value_string(give_bonus, _place), UNPAID);
        if (thing->move(tp) != MOVE_OK)
        {
           thing->move(environment(tp));
           tell_object(tp, "You drop "+ thing->query_short()+
             " as you're carrying too much.\n");
        }
        save_me();
        add_succeeded_mess("$N $V "+ MONEY_HAND->
          money_value_string(give_bonus, _place )+ ".\n");
        return 1;
        break;
   }
   if (thing->move(tp) != MOVE_OK)
   {
      thing->move(this_object());
      tell_object(tp, "You drop your "+ item+
        " as you're carrying too much.\n");
   }
   add_succeeded_mess("$N $V a new "+ item+ ".\n");
   return 1;
}
/* do_claim() */

/**
 * @ignore yes 
 * This function is used by employees to clock in and out of work.
 * Clocking out may also summon the npc shopkeeper.
 */
private int do_clock(string clock)
{
   object tp = this_player();
   string word = tp->query_name();
   int pay_them = UNPAID;

   if (tp->query_creator())
   {
      tell_object(tp, "Creators don't clock in or out.\n");
      return 1;
   }

   switch(clock)
   {
     case "in" :
        if (_employees[word][EMP_POINTS] & CLOCKED_IN)
        {
           tell_object(tp, "You are already clocked in!\n");
           return 1;
        }

        /* Stop the shopkeeper appearing if on their way */
        remove_call_out(_call_summon);

        /* Is this person a cre alt? If so, prevent promotion. */
        if (tp->query_property("no score") && 
           !_employees[word][EMP_NOPROMOTE])
        {
           _employees[word][EMP_NOPROMOTE] = TRUE;
           save_emps();
        }
        pay_them = PAID;
        set_employee(word, CLOCKED_IN);
        break;
     case "out" :
        if (!(_employees[word][EMP_POINTS] & CLOCKED_IN))
        {
           tell_object(tp, "You are already clocked out!\n");
           return 1;
        }

        /* Call npc shopkeeper to work */
        remove_call_out(_call_summon);
        _call_summon = call_out((: summon_shopkeeper() :), 60);
        reset_employee(word, CLOCKED_IN);
        break;
   }
   add_succeeded_mess("$N $V "+ clock+ ".\n");
   shop_log(GENERAL, word, "clocked "+ clock, pay_them);
   return 1;
}
/* do_clock() */
