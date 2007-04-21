/******************************************************************************
 * This file contains memo functions
 *****************************************************************************/

/**
 * @ignore yes 
 * Send a memo to each employee.  Only used if there is no board.
 */
private int do_memo()
{
   tell_object(this_player(), "Subject: (hit enter for general memo)\n");
   input_to((: write_memo($1) :), 0);
   add_succeeded_mess("");
   return 1;
}
/* do_memo() */


/** @ignore yes */
private void end_memo(string text, string subject)
{
   if (!text)
   {
      tell_object(this_player(), "Aborted.\n");
      return;
   }
   tell_object(this_player(), "Do you want to keep a copy of the memo? ");
   input_to((: send_memo($1,$(subject),$(text)) :), 0);
}
/* end_memo() */


/**
 * @ignore yes 
 * Send the memo
 */
private void send_memo(string ans, string subject, string text)
{
   object tp = this_player();
   string *employees = _retired,
          tp_name = tp->query_name();

   tp = this_player();
   tp_name = tp->query_name();
   ans = lower_case(ans);
   if (strlen(ans) < 1 || (ans[0] != 'y' && ans[0] != 'n'))
   {
      tell_object(tp,
        "Do you want to keep a copy of the memo? (Yes or No)? ");
      input_to((: send_memo($1,$(subject),$(text)) :), 0);
      return;
   }
   if (tp->query_creator() && tp_name != _creator)
      employees += ({tp_name});
   employees += ({_creator});
   foreach(string employee in m_indices(_employees))
      if (!(_employees[employee][EMP_POINTS] & NPC))
         employees += ({employee});
   if (ans[0] == 'n') employees -= ({tp_name});
   if (!sizeof(employees))
   {
#ifdef DEBUG
      tell_creator(CREATOR, "No employees to send mail to.\n");
#endif
      tell_object(tp, "There is no-one to send a memo to!\n");
      return;
   }
#ifdef DEBUG
   tell_creator(CREATOR, "Sending employee memo to %s.\n", 
     implode(employees, "," ));
#endif
   tell_object( tp, "Sending your memo.\n" );
   if (subject && subject != "") subject = " - " + subject;
   else subject = "";
   AUTO_MAILER->auto_mail(implode(employees, ","),
     tp_name, _very_short+ " employee memo" + subject, "", text, 0, 0);
}
/* send_memo() */

/** @ignore yes */
private void write_memo(string text)
{
   this_player()->do_edit(0, (: end_memo($1,$(text)) :));
}
/* write_memo() */
