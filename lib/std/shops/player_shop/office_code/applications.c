/******************************************************************************
 * This file contains application-related functions
 *****************************************************************************/

/**
 * @ignore yes 
 * Used by the shop front to add this person to the applicants mapping.
 * @param player player to add to applicants mapping
 */
void add_applicant(string player, string message)
{
   if (previous_object() && previous_object() != find_object(_shop_front))
   {
      LOG_ERROR("office.c", "add_applicant("+player+","+message+")");
      return;
   }
   player = lower_case(player);
   if (!query_applicant(player))
   {
      load_applicants();
      if (!sizeof(_applicants)) _applicants = ([ player:APP_MAP ]);
      else _applicants += ([ player:APP_MAP ]);
      _applicants[player][APP_TIME] = time();
      _applicants[player][APP_MESSAGE] = message;
      employee_log( player, "Applied for employment" );
      save_applicants();
   }
}
/* add_applicant() */

/**
 * @ignore yes 
 * Add this person to the list of declined applicants.  Used by the shop front
 * to determine if this person can apply again yet.
 * @param applicant the declined applicant
 */
private void add_declined(string applicant)
{
   applicant = lower_case(applicant);
   if (!query_declined(applicant))
   {
      if (!sizeof(_declined)) _declined = ([applicant:0]);
      else _declined += ([applicant:0]);
      _declined[applicant] = time();
      save_me();
   }
}
/* add_declined() */

/**
 * @ignore yes 
 * Used by the managers' office whenever a manager casts a vote on an applicant
 * @param applicant the applicant to vote for
 * @param vote the vote type
 */
private void add_vote(string applicant, int vote)
{
   string voter = this_player()->query_name();
   int managers = sizeof(get_managers());

   if (!managers) managers = 1;
   load_applicants();
   if (vote == VABSTAIN) _applicants[applicant][APP_ABSTAIN] += ({voter});
   else if (vote == VFOR) _applicants[applicant][APP_FOR] += ({voter});
   else _applicants[applicant][APP_AGAINST] += ({voter});
   shop_log(GENERAL, voter, "voted on "+ cap_name(applicant), PAID);
   save_applicants();
   managers -= sizeof(_applicants[applicant][APP_ABSTAIN]);
   
   if (sizeof(_applicants[applicant][APP_FOR]) > managers / 2)
   {
     /* This applicant has the vote of over 50%
      * of managers so hire them. 
      */
      PLAYER_SHOP->auto_mail(applicant, _proprietor, _shop_name, "",
         "Congratulations!  You've been accepted to work at "+
         _shop_name+ ".  Please return to the shop within the next "
         "seven days to \"confirm\" that you wish to accept "
         "the position.\n");
      _applicants[applicant][APP_TYPE] = HIRED;
      _applicants[applicant][APP_TIME] = time();
      employee_log(applicant, "Application was accepted");
   }
   else if (sizeof(_applicants[applicant][APP_AGAINST]) >=
      managers / 2)
   {
     /* This applicant has been voted against by at least 50% of the 
      * managers so decline them. 
      */
      remove_applicant(applicant);
      PLAYER_SHOP->auto_mail(applicant, _proprietor, _shop_name, "",
         "Thank you for your recent application for employment "
         "with us.  Unfortunately, I have to inform you that you "
         "have been unsuccessful at this time.  Please feel free "
         "to re-apply again in the future, when your application "
         "will be re-considered.\n");
      employee_log(applicant, "Application was declined");
      add_declined(applicant);
   } 
}
/* add_vote() */

/**
 * @ignore yes 
 * Check to see if we should hire any more accepted applicants.
 * Called during reviews, as well as when firing/promoting employees.
 */
void check_hire_list()
{
   int count;
   string *hirees = ({});
   
   /* Applicant has not confirmed employment after being accepted 
    * so remove them. 
    */
   load_applicants();
   foreach (string word in m_indices(filter(_applicants,
      (: _applicants[$1][APP_TYPE] == HIRED :))))
      if (time() - _applicants[word][APP_TIME] > HIRE_TIMEOUT)
      {
         PLAYER_SHOP->auto_mail(word, _proprietor,
            _shop_name, "", "Since you have not returned to confirm "
            "your employment with us, we have assumed that you are no "
            "longer interested, and removed your name from our files.  "
            "If you wish to re-apply at any time in the future, please "
            "return to the shop to do so.\n");
         employee_log(word, "Lapsed their application");
         remove_applicant(word);
      }
   
   foreach (string word in m_indices(filter(_applicants,
      (: _applicants[$1][APP_TYPE] == APPLIED :))))
      if (sizeof(_applicants[word][APP_FOR]) >
         sizeof(get_managers()) / 2)
      {
         PLAYER_SHOP->auto_mail(word, _proprietor, _shop_name, "",
            "Congratulations!  You've been accepted to work at "+
            _shop_name+ ".  Please return to the shop within the next "
            "seven days to \"confirm\" that you wish to accept "
            "the position.\n");
         _applicants[word][APP_TYPE] = HIRED;
         _applicants[word][APP_TIME] = time();
         employee_log(word, "Application was accepted");
      }
      else if (time() - _applicants[word][APP_TIME] > VOTE_TIMEOUT)
      {
         if (sizeof(_applicants[word][APP_FOR]) <=
            sizeof(_applicants[word][APP_AGAINST]))
         {
           /* Applicant has at least as many 'no' votes than 'yes' votes
            * so decline them.
            */
            remove_applicant(word);
            PLAYER_SHOP->auto_mail(word, _proprietor, _shop_name, "",
               "Thank you for your recent application for employment "
               "with us.  Unfortunately, I have to inform you that you "
               "have been unsuccessful at this time.  Please feel free "
               "to re-apply again in the future, when your application "
               "will be re-considered.\n");
            employee_log(word, "Application was declined");
            add_declined(word);
         }
         else
         {
           /* Applicant has more 'yes' votes as 'no' votes so accept them.
            */
            PLAYER_SHOP->auto_mail(word, _proprietor, _shop_name, "",
               "Congratulations!  You've been accepted to work at "+
               _shop_name+ ".  Please return to the shop within the next "
               "seven days to \"confirm\" that you wish to accept "
               "the position.\n");
            _applicants[word][APP_TYPE] = HIRED;
            _applicants[word][APP_TIME] = time();
            employee_log(word, "Application was accepted");
         }
      }
   
   /* Number of places available */
   count = _max_emp - sizeof(_employees);
   
   if (count < 1) return;
   hirees = sort_array(keys(filter(_applicants,
      (: _applicants[$1][APP_TYPE] == AWAITING :))),
      (: _applicants[$1][APP_TIME] - _applicants[$2][APP_TIME] :));
   
   /* See if we can hire all people awaiting a vacancy */
   if (count > sizeof(hirees)) count = sizeof( hirees );
   
   /* Hire as many people as we can */
   for (int n = 0; n < count; n++) hire(hirees[n]);
   save_applicants();
}
/* check_hire_list() */

/**
 * @ignore yes 
 * Shop front.
 * Cancel application.
 */
int do_cancel()
{
   object tp = this_player();

   remove_applicant(tp->query_name());
   employee_log(tp->query_name(), "Cancelled application");
   tell_object(tp, "You cancel your application.\n");
   return 1;
}
/* do_cancel() */

/**
 * @ignore yes 
 * Shop front.
 * Confirm employment.
 */
int do_confirm()
{
   object tp = this_player();
   string applicant = lower_case(tp->query_name());
   
   if (previous_object() && previous_object() != find_object(_shop_front))
   {
      LOG_ERROR("office.c", "do_confirm()");
      return 0;
   }
   load_applicants();
   _applicants[applicant][APP_TYPE] = AWAITING;
   _applicants[applicant][APP_TIME] = time();
   tell_object(tp, "You have now been added to "
      "our waiting list.  You will be notified of your position in the "
      "list as it changes.  You may \"cancel\" your application at "
      "any time.  You are currently at position " + 
      sizeof(m_indices(filter(_applicants,
      (: _applicants[$1][APP_TYPE] == AWAITING :))))+ 
      " in the waiting list.\n");
   save_applicants();
   employee_log(applicant, "Confirmed employment");
   tell_object(tp, "You confirm your employment.\n");
   return 1;
}
/* do_confirm() */

/**
 * @ignore yes 
 * Managers' office.
 * Vote on an applicant.
 */
int do_vote(mixed *args, string pattern)
{
   int query_app;
   string tp = this_player()->query_name();
   
   if (pattern == VOTE_APPLICANT)
   {
      args[0] = lower_case(args[0]);
      if (!(query_app = query_applicant(args[0])))
      {
         tell_object(this_player(),cap_name(args[0])+
            " hasn't applied!\n");
         return 1;
      }
      if (query_app != APPLIED)
      {
         tell_object(this_player(), cap_name( args[0])+ 
            " has already been hired!\n");
         return 1;
      }
      load_applicants();
      if (member_array(tp, _applicants[args[0]][APP_FOR]) != -1 ||
         member_array(tp, _applicants[args[0]][APP_AGAINST]) != -1 ||
         member_array(tp, _applicants[args[0]][APP_ABSTAIN]) != -1)
      {
         tell_object(this_player(),
            "You have already voted for this person!\n");
         return 1;
      }
      switch (args[1][0])
      {
      case 'y' :
         add_vote(args[0], VFOR);
         tell_object(this_player(), "You vote for " +
            cap_name( args[0] ) + " to work for the shop.\n");
         break;
      case 'a' :
         add_vote(args[0], VABSTAIN);
         tell_object(this_player(), "You abstain on " + 
            cap_name(args[0] ) + ".\n");
         break;
      default :
         add_vote(args[0], VAGAINST);
         tell_object(this_player(), "You vote against " +
            cap_name( args[0] ) + " working for the shop.\n");
      }
      clear_applicants();
   }
   else do_policy_vote(tp, args[0], args[1]);
   return 1;
}
/* do_vote() */

/**
 * @ignore yes 
 * Send a mail to each applicant awaiting a vacancy to let them know
 * their position in the queue.
 */
private void mail_hirees()
{
   string *hirees;
   
   /* Make sure we are mailing the correct person with the correct position
    * by sorting the list by time.
    */
   load_applicants();
   hirees = sort_array(keys(filter(_applicants,
      (: _applicants[$1][APP_TYPE] == AWAITING :))),
      (: _applicants[$1][APP_TIME] - _applicants[$2][APP_TIME] :));
   for (int x = 0; x < sizeof(hirees); x++)
      PLAYER_SHOP->auto_mail(hirees[x], _proprietor, _shop_name, "",
         "I am writing to inform you that you have now moved to position "+
         (x + 1)+ " in the employment waiting list.  If you are not near "
         "the top of the list, please be patient.\nThank you.\n");
   clear_applicants();
}
/* mail_hirees() */

/**
 * @ignore yes 
 * Remove this person from the applicants mapping.
 * Used when an applicant is hired, declined, or cancels their application.
 * @param applicant the applicant to remove
 */
private void remove_applicant(string applicant)
{
   applicant = lower_case(applicant);
   if (query_applicant(applicant))
   {
      load_applicants();
      map_delete(_applicants, applicant);
      save_applicants();
   }
}
/* remove_applicant() */

/**
 * @ignore yes 
 * Remove this person from the list of declined applicants, allowing them to 
 * re-apply.
 * @param declined the declinee to remove
 */
private void remove_declined(string declined)
{
   declined = lower_case(declined);
   if (query_declined(declined))
   {
      map_delete(_declined, declined);
      save_me();
   }
}
/* remove_declined() */
