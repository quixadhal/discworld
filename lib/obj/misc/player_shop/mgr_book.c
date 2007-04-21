/**
 * Player-run shop manager handbook object.
 * This object is claimable by managers of player-run shops.
 * It is intended to describe the duties & offer guidance to these managers.
 * This object should not be inherited by anything, but is created by the
 * player_shop office.
 *
 * @author Ringo
 * @started 1st August 1999
 */
#include "player_shop.h"

inherit "/std/book";

private string _office;

void setup() {
    set_name( "book" );
    add_alias( "handbook" );
    set_short( "manager handbook");
    set_main_plural( "manager handbooks" );
    add_adjective( "manager" );
    set_weight( 5 );
    set_value( 5 );
    set_no_pages( 11 );
    add_property( "no recycling", 1 );
}
/* setup() */

/** @ignore yes */
private string cap_name(string name) 
{
   if (test_player(name))
      return PLAYER_HANDLER->query_cap_name(name);
   else return capitalize(name);
}

/** @ignore yes */
void set_office( string office ) { 
   _office = office;
   if ( !query_property( "office" ) ) {
      add_property( "office", office );
   }
   set_long( "This is a handbook for Managers of "+ office->query_shop_name()+
     ", "+ office->query_place()+ ".  It covers those aspects of "
     "the managers' job not covered by the staff handbook.\n"
     "You may <read book>, <open book to page #> and "
     "<turn a page of book>.\n" );

    set_open_page( 1 );
    set_read_mess( "\n"     
      "                            TABLE OF CONTENTS                    Page 1\n"
      "                    .o0o.o0o.o0o.o0o.o0o.o0o.o0o.o0o.\n\n"
      "       Introduction............................................2\n"
      "       The job.................................................3\n"
      "       The way things work.....................................4\n"
      "       Specific commands.......................................5-7\n"
      "       Hiring new employees....................................8-9\n"
      "       Disciplinary procedures.................................10-11\n",
      "common", 100 );

    set_open_page( 2 );
    set_read_mess( "\n"
      "                               Introduction                      Page 2\n"
      "                           -=-=-=-=-==-=-=-=-=-\n\n"
      " This handbook has been written specifically for managers to detail\n"
      " your role in the shop.  You will find details of commands specific\n"
      " to managers, and a more detailed explanation of the workings of the\n"
      " shop.  This book is to be treated as a confidential document, and its\n"
      " contents only to be discussed with other managers.\n",
      "common", 100 );

    set_open_page( 3 );
    set_read_mess( "\n"
      "                                 The Job                         Page 3\n"
      "                               -=-=-=-=-=-\n\n"
      " It is the responsibility of managers to ensure the smooth operation\n"
      " of the shop.  This includes staff motivation, advertising, pay and\n"
      " staffing levels, accounts, discipline and personnel.\n"
      " Motivation and advertisements are left to the judgement of managers,\n"
      " whilst the personnel issues should follow strict guidelines detailed\n"
      " later.\n\n"
      " As you should be aware, since you now hold the highest position in the\n"
      " store, there is nothing to work towards regarding your promotion.  You\n"
      " are paid at a higher rate, however, so there are still benefits to be\n"
      " obtained from carrying out the more 'mundane' tasks within the shop.\n\n"
      " Finally, managers are allowed to \"retire\" and continue in an advisory\n"
      " capacity.  Whilst having very limited functional control over the\n"
      " shop, retired managers are still able to keep their badge, and use\n"
      " the notice board and \"memo\" facility.\n",
      "common", 100 );  

    set_open_page( 4 );
    set_read_mess( "\n"
      "                           The way things work                   Page 4\n"
      "                         -=-=-=-=-=-=-=-=-=-=-=-\n\n"
      " On being hired, employees are paid according to the amount of work\n"
      " they do.  Along with their pay, they accumulate points towards\n"
      " promotion in the shop.  The pay and points depend on the amount of\n"
      " work done, and not on the amount of time spent in the shop.  Precisely,\n"
      " employees receive a set amount per transaction, with a maximum of one\n"
      " paid transaction every minute.\n"
      " Each employee's status is reviewed every 3 DW days, their points added,\n"
      " and their pay awarded.  This pay is transferred into their bank account\n"
      " every month, and a written confirmation of the amount is sent.\n"
      " Employees are paid according to their status.  This is done in a ratio\n"
      " of 2:3:4 so that managers will be paid twice as much as employees, with\n"
      " supervisors halfway in between.  The bonuses are calculated in the same\n"
      " way.\n",
      "common", 100 );

    set_open_page( 5 );
    set_read_mess( "\n"
      "                              Specific commands                  Page 5\n"
      "                           -=-=-=-=-=-=-=-=-=-=-=-\n\n"
      " There are a number of additional commands once promotion to manager\n"
      " is gained.  The majority of these are rather self-explanatory, and\n"
      " so will only be covered briefly.  The remainder merit an explanation.\n\n"
      " accounts - Check the current value of the accounts\n"
      " list - As in the main office, with the addition of baddies & applicants\n"
      " mail - Yes, you now have access to your mail from the managers' office\n"
      " memo - Send a memo to the other managers of the shop\n"
      " retire - The alternative to resignation\n"
      " view - View a person's record with this shop.  This is extremely useful\n"
      "        when deciding on disciplinary matters, or before hiring applicants\n"
      " query - Check the current settings for maximum number of employees and\n"
      "         pay rates\n"
      " project - Gives a projection of the months' pay packet based on the\n"
      "           employees' activity so far, and also the projected bonus amounts\n"
      "           based on the current value of the bonus fund.  Useful when setting\n"
      "           these values\n"
      " transfer - Transfer money between two accounts                (Continued...)\n",
      "common", 100 );

    set_open_page( 6 );
    set_read_mess( "\n"
      "                                                                 Page 6\n\n"
      " set - This command sets the maximum number of employees (speaks for\n"
      "       itself), or the base pay rate.  The base pay rate is the amount\n"
      "       that each employee will receive per transaction.  This rate is\n"
      "       then adjusted accordingly for supervisors and managers.\n"
      "       Changing this amount will not affect transactions already made\n"
      "       by employees, but will affect all subsequent transactions.\n"
      " buy/sell - These commands are used to buy and sell storeroom cabinets.\n"
      "            Cabinets are sold for 50% of the buying price, so make sure\n"
      "            you really do want a cabinet before buying it as they can be\n"
      "            expensive.\n"
      " (un)ban - These commands are used to ban or unban a person from the shop.\n"
      "           A person can be banned automatically, and will also be unbanned\n"
      "           automatically after 28 days.  If, however, you wish to unban\n"
      "           someone sooner, you may do so with this command.\n"
      " vote - Allows you to vote for, against, or abstain on an applicant.  This\n"
      "        will be your only chance to vote on this applicant, so make sure it\n"
      "        is the right decision.\n"
      " check - Allows you to check your current votes               (Continued...)\n",
      "common", 100 );

    set_open_page( 7 );
    set_read_mess( "\n"
      "                                                                 Page 7\n\n"
      " leave - Allows you to place an employee on leave for up to 14 days\n"
      " commend - Allows you to commend an outstanding employee.  This will\n"
      "           place an entry in their employment record, and award them\n"
      "           extra promotion points.\n"
      " warn - Officially reprimand an employee.  Will place an entry in their\n"
      "        record, and deduct promotion points.\n"
      " suspend - Suspend an employee's bonus entitlement for a specified\n"
      "           number of months following the current month.\n"
      " demote - Demote a supervisor to employee.  Will start them off as if\n"
      "          they were a new employee, but with their employment record\n"
      "          reflecting their history.\n"
      " fire - Terminate someone's employment with this shop.\n",
      "common", 100 );


    set_open_page( 8 );
    set_read_mess( "\n"
      "                            Hiring new employees                Page 8\n"
      "                        -=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n"
      " This is a brief description of the process involved in hiring a\n"
      " new employee.\n\n"
      " 1) Prospective employees \"apply\" in the shop and a mail is sent\n"
      "    to each manager.  The application is saved and viewable from\n"
      "    within the managers' office.\n"
      " 2) Managers check the list in the office which will show applicants\n"
      "    waiting for employment, applicants yet to confirm their offer,\n"
      "    and new applicants yet to be voted upon (with votes so far).\n"
      " 3) Managers \"vote\" on each prospective employee.  Only one vote\n"
      "    per employee per manager is allowed, and the first vote counts.\n"
      " 4) As soon as the number of votes for employment equals 50% of the\n"
      "    current manager base, the candidate is accepted.  If votes against\n"
      "    exceeds 50%, the candidate is rejected.\n"
      " 5) If 50% hasn't been reached after 7 days, then the candidate is\n"
      "    hired/rejected on the basis of votes received so far.  (Continued...)\n",
      "common", 100 );

    set_open_page( 9 );
    set_read_mess( "\n"
      "                                                                Page 9\n\n"
      " 6) The approved candidate has 7 days from being accepted to confirm\n"
      "    their offer, otherwise they are removed from the list.\n"
      " 7) Once confirmed, the employee is kept advised of their current\n"
      "    position in the waiting list, and hired as soon as a position\n"
      "    becomes vacant in the shop.\n"
      " 8) The candidate can \"cancel\" their application at any time until\n"
      "    they are hired.\n\n"
      " Since the candidates are hired in order of being accepted, please make\n"
      " sure you vote for the oldest applications first.\n",
      "common", 100 );

    set_open_page( 10 );
    set_read_mess( "\n"
      "                           Disciplinary procedures              Page 10\n"
      "                         -=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n"
      " This section is intended as a guide to disciplinary procedures.  It\n"
      " is ultimately down to managers to set their own policies within the\n"
      " shop.\n"
      " The first step in any action is to verbally warn an employee before\n"
      " taking any formal action.  This avoids any unpleasant comebacks on\n"
      " the manager involved.\n"
      " The next stage is either a warning, or suspending an employee's bonus.\n"
      " Whilst a warning reduces an employee's promotion points, bonuses can\n"
      " be suspended indefinitely.\n"
      " If the employee is a supervisor, the next stage is probably demotion.\n"
      " Following demotion, or if the employee is not a supervisor, dismissal\n"
      " may be the only course of action open to you.\n"
      " It is important to check the employee's history before taking any\n"
      " action as this will allow you to see what action has already been\n"
      " taken, and when.                                       (Continued...)\n",
      "common", 100 );

    set_open_page( 11 );
    set_read_mess( "\n"
      "                                                                Page 11\n\n"
      " Again, it is the responsibility of managers to develop a policy as\n"
      " to what merits disciplinary action.  A few guidelines are set out in\n"
      " the staff handbook, but more can be added.\n\n"
      " Finally, if there are any changes that need making to this, or indeed\n"
      " the staff handbook, please let me know.  I am open to comments and\n"
      " suggestions on content as well as shop policy.\n"
      " (c) "+ cap_name(CREATOR)+ " 2001",
      "common", 100 );

    set_open_page( 0 );
}
/* setup() */


void init () {
   set_open_page( 0 );
   if ( !_office || _office == "" ) {
      if ( !query_property( "office" ) ) {
         return;
      }
      _office = query_property( "office" );
      set_office( _office );
   }
   if ( !_office->query_manager( environment( this_object() )->query_name() ) &&
     !_office->query_retired( environment( this_object() )->query_name() ) &&
     !environment( this_object() )->query_creator() ) {
      return;
   }
   ::init();
}
/* init() */
