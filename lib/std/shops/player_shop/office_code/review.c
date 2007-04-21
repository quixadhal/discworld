/******************************************************************************
 * This file contains the code to deal with the maintenance reviews
 *****************************************************************************/

/**
 * @ignore yes 
 * Called once a month to conduct the shop's monthly review.
 * This method is the monthly maintenance function for the shop.  This review
 * involves paying employees direct into their nominated bank account, and
 * awarding bonuses based on the current value of the bonus fund.  If there is
 * not enough money in the profit account to pay the correct amount, the
 * employees are paid on a pro-rata basis.
 */
private void monthly_review()
{  
   int amount = calc_pay(),
       bonus_divisor = 0,
       cabinet_cost = (_num_cabinets - MIN_CABINETS) * CABINET_COST,
       pay;
   float pay_multiplier = 1.0;
   string *emps = m_indices(_employees); 

   log_file("/log/PLAYER_SHOP", "%s: %s entered monthly review (review.c)\n",
      ctime(time()), _very_short); 

   if (cabinet_cost)
   {
      adjust_profit(_proprietor, -cabinet_cost);
      shop_log(ACCOUNTS, _proprietor,
         "paid "+ MONEY_HAND->money_value_string(cabinet_cost,
         _place) + " for the rent of " + (_num_cabinets - MIN_CABINETS) +
         " cabinets", UNPAID);
   }
   if (_accounts["profit"] < 0) adjust_bonus("Shop",_accounts["profit"]);
   if (amount > _accounts["profit"])
      pay_multiplier = _accounts["profit"] / amount;

   foreach (string word in emps)
   {
      if (!_employees[word][EMP_PAY]) continue;
      pay = to_int(_employees[word][EMP_PAY] * pay_multiplier);
      if (!(_employees[word][EMP_POINTS] & NPC))
      {
         BANK_HANDLER->adjust_account(word,
            BANKS[_employees[word][EMP_BANK]][1], pay );
         PLAYER_SHOP->auto_mail(word, _proprietor, "Pay advice for "+
            _last_month, "", sprintf("For your work during %s, you have been "
            "paid a total of %s.  Keep up the good work.\n--\n%s "
            "(proprietor)\n", _last_month, MONEY_HAND->money_value_string(pay,
            _place ), _proprietor));
      }
      else _employees[word][EMP_POINTS] = EMPLOYEE + NPC;
      shop_log(ACCOUNTS, _proprietor, "paid "+ MONEY_HAND->
        money_value_string(pay, _place)+ " to "+ cap_name(word), UNPAID);
      _employees[word][EMP_PAY] = 0;
      _accounts["profit"] -= pay;
   }
   _bonus += _accounts["bonus"];
   if (_bonus < 0)
   {
      _accounts["bonus"] = _bonus;
      _bonus = 0;
   } 
   else _accounts["bonus"] = 0;

   foreach (string word in m_indices(_employees))
   {
      if (_employees[word][EMP_NOBONUS]) continue;
      if (!test_prop(word, _very_short+ " handbook")) continue;

      if (_employees[word][EMP_POINTS] & MANAGER)
      {
         bonus_divisor += 4;
      }
      else if (_employees[word][EMP_POINTS] & SUPERVISOR ||
         _eom == word)
      {
         bonus_divisor += 3;
      }
      else
      {
         bonus_divisor += 2;
      }
   }
   if (!bonus_divisor) _bonus_val = _bonus;
   else _bonus_val = (_bonus * 2) / bonus_divisor;

   _got_bonus = ({});
   _eom = sort_array(get_employees(),
      (: _employees[$2][EMP_EOM] - _employees[$1][EMP_EOM] :))[0];
   foreach (string word in m_indices(_employees))
   {
      if (_employees[word][EMP_NOBONUS])
      {
         _employees[word][EMP_NOBONUS]--;
         _got_bonus += ({word});
      }
      _employees[word][EMP_EOM] = 0;
   }
   employee_log(_eom, _last_month+ "'s Employee Of The Month");
   add_board_message("Bonuses for "+ _last_month,
      sprintf("Based on the bonus fund of %s for %s, the following "
      "bonuses have been awarded:\n\n     Managers    - %s\n     Supervisors"
      " - %s\n     Employees   - %s\n\n%s's Employee Of The Month was %s.  "
      "Well done to you.\n", MONEY_HAND->money_value_string(_bonus, _place),
      _last_month, MONEY_HAND->money_value_string(_bonus_val * 2, _place),
      MONEY_HAND->money_value_string(to_int(_bonus_val * 1.5), _place),
      MONEY_HAND->money_value_string(_bonus_val, _place), _last_month,
      cap_name(_eom)));
   if (_bonus)
   {
      shop_log( ACCOUNTS, _proprietor, "paid out "+
        MONEY_HAND->money_value_string(_bonus, _place)+
        " in bonuses for "+ _last_month, UNPAID );
   }
   if (_eom == _shopkeeper->query_name())
   {
      _bonus -= to_int(_bonus_val * 1.5);
      shop_log(GENERAL, _eom, "claimed "+ 
         MONEY_HAND->money_value_string(to_int(_bonus_val * 1.5), _place),
         UNPAID);
   }
   else
   {
      _bonus -= _bonus_val;
      shop_log(GENERAL, _shopkeeper->query_name(), "claimed "+ 
         MONEY_HAND->money_value_string(_bonus_val, _place), UNPAID);
   }
   if (_bonus < 0) _bonus = 0;
   _last_month = _review_month;
   _call_review = 0;
   save_me();
   save_emps();
}
/* monthly_review() */

/**
 * @ignore yes 
 * Called once a day to conduct the shop's daily review.
 * This method is the daily maintenance function for the shop.  It checks that
 * employees are still valid players, and not creators.  It conducts automatic
 * promotions, and handles demotions for inactive employees.  It also updates
 * the lists of declined applicants and banned people and removes that status
 * if applicable.  Finally, it calls the check_hire_list function to see if 
 * we can hire any new employees.
 */
private void review_employees()
{
   int prom = FALSE,
       prom_number,
       managers,
       time = time();
   string *promos = ({}),
          promopost,
          *emps = _retired + m_indices(_employees);

   log_file("/log/PLAYER_SHOP", "%s: %s entered review_employees (review.c)\n",
      ctime(time()), _very_short); 

   /* Fire non-users, creators and demote creator alts */
   foreach (string word in emps)
   {
      if (!test_player(word))
      { 
         /* Make sure we are not firing the npc shopkeeper */
         if (_employees[word][EMP_POINTS] & NPC) continue;
         fire_them(_proprietor, word, "not existing");
      }

      else if (test_cre(word))
         fire_them(_proprietor, word, "being a creator");

      else if (test_prop(word,"no score") &&
         _employees[word][EMP_POINTS] & SUPERVISOR )
      {
         _employees[word][EMP_POINTS] =
            _employees[word][EMP_POINTS] & CLOCKED_IN + EMPLOYEE;
         save_emps();
         PLAYER_SHOP->auto_mail(word, _proprietor, "Demotion", "",
         "This is to advise you that, due to you having a creator alt, "
         "you have today been demoted.\n");
         employee_log(word, "Demoted by "+ _proprietor);
         shop_log(PERSONNEL, _proprietor, "demoted "+ cap_name(word), UNPAID);
      }

      if (!test_prop(word, _very_short+ " handbook"))
      {
         _employees[word][EMP_PAY] = 0;
         _employees[word][EMP_POINTS] =
            _employees[word][EMP_POINTS] & CLOCKED_IN + EMPLOYEE;
      }
   }


   /* Check for inactive managers */
   foreach (string word in get_managers())
      if ((time - _times[word]) > (60*60*24*MGR_DEMOTE) &&
         _employees[word][EMP_INACTIVE] &&
         last_login(word) - _times[word] > (60*60*24*2))
            demote( _proprietor, word );
      else if (time - _times[word] > (60*60*24*MGR_WARN) &&
         !_employees[word][EMP_INACTIVE] && 
         last_login(word) - _times[word] > (60*60*24*2))
      {
         PLAYER_SHOP->auto_mail(word, _proprietor, "Poor attendance",
           "", "It has come to my attention that you have now been "
           "inactive for over " + MGR_WARN+ " days.  As you are a manager, "
           "you are required to meet certain levels of attendance.  "
           "You are now in serious danger of being demoted without "
           "further warning.\n---\n" + _proprietor+ " (proprietor)\n");
         _employees[word][EMP_INACTIVE] = TRUE;
         employee_log(word, "Warned about inactivity");
         shop_log(PERSONNEL, _proprietor, "warned "+
               cap_name(word) + " about inactivity", UNPAID);
      }

   /* Check supervisors for inactivity or promotion.  Sorted by points so
    * people promoted in order.
    */
   foreach(string word in sort_array(get_supervisors(), 
     (: _employees[$1][EMP_POINTS] - _employees[$2][EMP_POINTS] :)))
      if (time - _times[word] > (60*60*24*SPR_DEMOTE) &&
         _employees[word][EMP_INACTIVE] &&
         last_login(word) - _times[word] > (60*60*24*2))
            demote( _proprietor, word );
      else if (time - _times[word] > (60*60*24*SPR_WARN) &&
        !_employees[word][EMP_INACTIVE] &&
         last_login(word) - _times[word] > (60*60*24*2))
      {
         PLAYER_SHOP->auto_mail(word, _proprietor, "Poor attendance",
            "", "It has come to my attention that you have now been "
            "inactive for over "+ SPR_WARN+ " days.  As you are a supervisor, "
            "you are required to meet certain levels of attendance.  "
            "You are now in serious danger of being demoted without "
            "further warning.\n---\n" + _proprietor+ " (proprietor)\n");
         _employees[word][EMP_INACTIVE] = TRUE;
         employee_log(word, "Warned about inactivity");
         shop_log(PERSONNEL, _proprietor, "warned "+
            cap_name(word) + " about inactivity", UNPAID);
      }
      else
      {
         /* See if any managerial vacancies.  If so, and this person has
          * sufficient points & is not being ignored for promotion,
          * promote them.
          */
         prom_number = (_max_emp * PERCENT_M) / 100;
         if ((_employees[word][EMP_POINTS] > 32 * MANAGER_POINTS) && 
           sizeof(get_managers()) < prom_number &&
           !_employees[word][EMP_NOPROMOTE])
         {
            set_employee(word, MANAGER);
            shop_log(PERSONNEL, _proprietor, "promoted "+
               cap_name(word)+ " to manager", UNPAID);
            employee_log(word, "Promoted to manager");
            PLAYER_SHOP->auto_mail(word, _proprietor, "Promotion!", 
              "", "Congratulations!  You've been promoted to manager "
              "of "+ _shop_name+ ".  You'll find that you can now enter "
              "the managers' office.  Please remember to use the \"memo\" "
              "facility from there to discuss any major admin points with "
              "other managers.  This includes hiring, firing, and so on.\n");
            promos += ({word});
            prom = TRUE;
         }
      }

   /* Check employees for inactivity or promotion.  Sorted by points so
    * people promoted in order.
    */
   foreach(string word in sort_array(get_employees(), 
     (: _employees[$1][EMP_POINTS] - _employees[$2][EMP_POINTS] :)))
   {
      if (_employees[word][EMP_POINTS] & NPC) continue;
      if (time - _times[word] > (60*60*24*EMP_FIRE) &&
         _employees[word][EMP_INACTIVE] &&
         last_login(word) - _times[word] > (60*60*24*2))
      {
         fire_them(_proprietor, word, "serious inactivity");
         continue;
      }
      if (time - _times[word] > (60*60*24*EMP_WARN))
      {
         if (!_employees[word][EMP_INACTIVE] &&
         last_login(word) - _times[word] > (60*60*24*2))
         {
            PLAYER_SHOP->auto_mail(word, _proprietor, "Inactivity",
              "", "It has come to my attention that you have now been "
              "inactive for over "+ EMP_WARN+ " days.  Unless this "
              "situation is resolved, the management may have no option "
              "but to terminate your employment.\n---\n"+ _proprietor+ 
              " (proprietor)\n");
            _employees[word][EMP_INACTIVE] = TRUE;
            shop_log(PERSONNEL, _proprietor, "warned "+ 
              cap_name(word)+ " about inactivity", UNPAID);
            employee_log(word, "Warned about inactivity");
         }
      }
      else
      {
         /* See if any supervisory vacancies.  If so, and this person has
          * sufficient points & is not being ignored for promotion,
          * promote them.
          */
         prom_number = (_max_emp * PERCENT_S) / 100;
         if ((_employees[word][EMP_POINTS] > 32 * SUPER_POINTS) &&
           sizeof( get_supervisors() ) < prom_number &&
           !_employees[word][EMP_NOPROMOTE])
         {
            if(_employees[word][EMP_POINTS] & CLOCKED_IN)
               _employees[word][EMP_POINTS] = (SUPER_POINTS * 32) +
                 EMPLOYEE+ SUPERVISOR + CLOCKED_IN;
            else
               _employees[word][EMP_POINTS] = (SUPER_POINTS * 32) + 
                 EMPLOYEE+ SUPERVISOR;
            shop_log(PERSONNEL, _proprietor, "promoted "+
              cap_name(word)+ " to supervisor", UNPAID);
            employee_log(word, "Promoted to supervisor");
            PLAYER_SHOP->auto_mail(word, _proprietor, "Promotion!", "",
              "Congratulations!  You've been promoted to supervisor "
              "of "+ _shop_name+ ".  You will now be able to use your "
              "newly acquired supervisor commands.\n");
            promos += ({word});
            prom = TRUE;
         }
      }
   }

   /* Post about promotions */
   if (prom)
   {
      promopost = "The following employees have been promoted:\n\n";
      foreach (string word in promos)
         promopost += sprintf("     %s has been promoted to %s\n",
           cap_name(word), (_employees[word][EMP_POINTS] & MANAGER)?
           "manager":"supervisor");
      promopost += "\nCongratulations!\n";
      add_board_message("Promotions", promopost);
   }

   /* Check the list of banned people */
   foreach (string word in m_indices(_baddies))
      if (time - _baddies[word][BAD_TIME] > (60*60*24*BAN_LENGTH))
         remove_baddie( word );

   /* Check the list of declined applicants */
   foreach (string word in m_indices(_declined))
      if (time - _declined[word] > (60*60*24*DECLINE_LENGTH))
         remove_declined(word);

   /* See if anyone can be hired */
   remove_call_out(_call_hire_list);
   _call_hire_list = call_out((: check_hire_list() :), 5);
   save_emps();

   /* Update policies */
   managers = sizeof(get_managers()) + sizeof(get_retired());
   load_new_policies();
   if (sizeof(_new_policies))
   {
      foreach (string word in m_indices(_new_policies))
      {
         if (sizeof(_new_policies[word][POLICY_FOR]) > managers / 2)
         {
            add_policy(word);
         }
         else if (time - _new_policies[word][POLICY_TIME] > VOTE_TIMEOUT)
         {
            if (sizeof(_new_policies[word][POLICY_FOR]) >=
               sizeof(_new_policies[word][POLICY_AGAINST]))
            {
               add_policy(word);
            }
            else remove_policy(word);
         }
      }
   }

   clear_new_policies();

   /* Update the player history data */
   load_history();
   foreach (string word in m_indices(_history))
   {
      if (!test_player(word) || test_cre(word) ||
        !_times[word] || _times[word] < (time - HIST_TIMEOUT))
      {
         map_delete(_times, word);
         map_delete(_history, word);      
      }
   }
   save_hist();
   save_times();
}
/* review_employees() */
