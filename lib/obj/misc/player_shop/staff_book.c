/**
 * Player-run shop staff handbook object.
 * This object is claimable by staff of player-run shops.
 * It is intended to describe the duties & offer guidance to staff.
 * This object should not be inherited by anything, but is created by the
 * player_shop office.
 *
 * @author Ringo
 * @started 1st August 1999
 */
#include <player_shop.h>

inherit "/std/book";

private string _office = "";

void setup()
{
   set_name( "book" );
   add_alias( "handbook" );
   set_short( "staff handbook");
   set_main_plural( "staff handbooks" );
   add_adjective( "staff" );
   set_weight( 5 );
   set_value( 5 );
   set_no_pages( 12 );
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
void set_office( string office )
{
   _office = office;
   if ( !query_property( "office" ) )
   {
      add_property( "office", office );
   }
   set_long( "This is a handbook for staff of "+ 
     office->query_shop_name()+ ", " + office->query_place()+ 
     ".  It covers all aspects of the job.\n"
     "You may <read book>, <open book to page #> and "
     "<turn a page of book>.\n" );

   set_open_page( 1 );
   set_read_mess( "\n"     
     "                            TABLE OF CONTENTS                    Page 1\n"
     "                    .o0o.o0o.o0o.o0o.o0o.o0o.o0o.o0o.\n\n"
     "       Introduction............................................2-4\n"
     "       The job.................................................5-6\n"
     "       The office..............................................6-7\n"
     "       The counter.............................................8\n"
     "       The storeroom...........................................9\n"
     "       The shopkeeper..........................................10\n"
     "       Promotion and pay.......................................11\n"
     "       Termination of employment...............................12\n\n",
     "common", 100 );

   set_open_page( 2 );
   set_read_mess( "\n"
     "                               Introduction                      Page 2\n"
     "                           -=-=-=-=-==-=-=-=-=-\n\n"
     " Welcome to "+ office->query_shop_name()+ ".\n"
     " This is a player-run shop, designed to operate interactively with its\n"
     " employees with minimal input from creators.\n"
     " Players are able to sell items to and buy them from the shop, just like\n"
     " a regular shop, but prices should be set such that it is advantageous\n"
     " to deal with us,  rather than some other general shop.\n\n"
     " A list of items in which the shop deals is maintained in the shop's\n"
     " office, where a chart shows the buying and selling prices of items,\n"
     " as well as how many are in stock, the maximum number that the shop\n"
     " will keep in stock and the average number that has recently been in\n"
     " stock; the stock itself is saved over a reboot.        (Continued...)",
     "common", 100 );

   set_open_page( 3 );
   set_read_mess( "\n"
     "                                                                 Page 3\n\n"
     " The most important feature of the shop is that it is run by players,\n"
     " with a non-player employee who stands in when there are no player\n"
     " employees about.  Players who work faithfully for the shop will be\n"
     " promoted to supervisor and then to manager, as well as receiving a\n"
     " regular wage which increases with employment level.\n\n"
     " As well as hiring new employees, managers are also responsible for\n"
     " firing employees who break trust.  Supervisors (and managers) are\n"
     " responsible for keeping the shop running efficiently by adding new\n"
     " items to those in which the shop deals, by adjusting selling and\n"
     " buying prices and inventory limits to reflect supply and demand and\n"
     " by removing items in which it is no longer reasonable to deal.\n\n"
     " Supervisors are also given access to the shop's log books which\n"
     " contain details of everything that happens.           (Continued...)",
     "common", 100 );

   set_open_page( 4 );
   set_read_mess( "\n"
     "                                                                 Page 4\n\n"
     " Finally, while it is intended, and hoped, that the shop will run\n"
     " autonomously under player control, it is inevitable that problems\n"
     " will arise from time to time which cannot be solved by the shop's\n"
     " player managers and supervisors.  Should such problems arise, please\n"
     " don't hesitate to contact myself or another creator.\n"
     " Enjoy your time with "+ _office->query_shop_name()+ ".\n "+
     cap_name( _office->query_creator() )+ " (caretaker)\n", "common", 100 );  

   set_open_page( 5 );
   set_read_mess( "\n"
     "                                 The Job                         Page 5\n"
     "                               -=-=-=-=-=-\n\n"
     " It is the responsibility of employees to ensure that the shop is\n"
     " kept as fully stocked as possible.  This may be achieved by buying\n"
     " items from players, or by collecting the items yourself.  Although\n"
     " you are not expected to come running to the shop every time a\n"
     " customer walks in, once you are clocked in, you should not leave\n"
     " the shop until you clock out again.\n\n"
     " All of the commands that are available to the shop's employees\n"
     " should be obvious from the various rooms of the shop; those available\n"
     " in the office are described by giving the command %^B_RED%^office%^RESET%^.\n"
     " Possibly the most important command is %^B_RED%^log%^RESET%^, which must be "
     "used to record\n"
     " each transaction.  All activity within the shop is logged, and so it is\n"
     " vital to record each sale and purchase in the correct manner so that the\n"
     " shop's supervisors and managers can see what happened when they examine\n"
     " the logs.  Examples of the \"log\" command are given in the section\n"
     " dealing with the counter.\n",
     "common", 100 );  

   set_open_page( 6 );
   set_read_mess( "\n"
     "                               The Office                          Page 6\n"
     "                             -=-=-=-==-=-=-\n\n"
     " The office of the shop is the nerve-centre.  This is where you can\n"
     " obtain most of your information, and is where you clock in and out.\n"
     " A description of the \"office\" commands you can use from here is\n"
     " given below.\n\n"
     " clock - Clock in or out of work.  You are only paid for the work\n"
     "         you do whilst you are clocked in.\n"
     " claim - Claim a staff badge, a copy of this book, or your monthly\n"
     "         bonus.  The badge itself has some nifty functions - look\n"
     "         at one for further information.\n"
     " list - List the employees of the shop, and find out your current\n"
     "        standing for promotion.  This shows the percentage of points\n"
     "        you have achieved towards your next promotion.\n"
     " bank - Change, or check your banking details.  Your pay can be paid\n"
     "        into the bank of your choice.  Your bonus is given in cash when\n"
     "        you \"claim\" it.\n"
     " promotion - Allows you to specify whether to be automatically promoted\n"
     "             or not.  If not, you will not be promoted once gaining the\n"
     "             required points until you specify otherwise.\n" 
     " resign - End your employment with the shop.             (Continued...)\n",
     "common", 100 );

   set_open_page( 7 );
   set_read_mess( "\n"
     "                                                                 Page 7\n\n"
     " Supervisors receive two additional commands - \"chart\" and \"logs\".\n"
     " The \"logs\" command allows a supervisor to review the logs, whilst the\n"
     " \"chart\" command allows them to add and remove items from the shop's\n"
     " list, set the maximum stock level, buy & sell prices, and assign them\n"
     " to a cabinet.  If an item is not assigned to a cabinet, it cannot be\n"
     " stored.  The \"check\" command allows supervisors to check which items\n"
     " are allocated to each cabinet.\n"
     " It is important to note that an item can be assigned to more than one\n"
     " cabinet, and that a cabinet can be assigned more than one item.  It is\n"
     " the supervisor's responsibility to ensure that items are judiciously\n"
     " assigned to ensure that the most efficient use is made of the available\n"
     " storage space.\n"
     " To view details of items stocked by the shop, simply \"look\" at the\n"
     " chart in the office.\n\n"
     " In addition, if there is no notice board in the shop, employees may use\n"
     " the \"memo\" command to send a mail to all the employees of the shop.\n",
     "common", 100 );

   set_open_page( 8 );
   set_read_mess( "\n"
     "                               The Counter                        Page 8\n"
     "                             -=-=-=-=-=-=-=-\n\n"
     " The counter houses the shop's cash register, and allows employees to\n"
     " \"log\" transactions.  There is also a \"calc\"ulator for use in those\n"
     " difficult situations.  All monies entering or exiting the register are\n"
     " logged, so if you are tempted, please bear in mind that supervisors can\n"
     " see exactly who took that $1000 from the till ;-)\n\n"
     " The log command can be confusing, so the following examples have been\n"
     " provided.  All entries are made from the point of view of the shop with\n"
     " the syntax: %^B_RED%^log shop bought|sold <quantity> <item> for <amount>%^RESET%^.\n"
     " Examples:\n\n"
     " A customer sells 5 gizmos to the shop at 20 pence each. The log entry is:\n"
     " %^B_RED%^log shop bought 5 gizmos for 100p%^RESET%^ (or $1 instead of 100p)\n\n"
     " A customer buys 20 elephants from the shop at 10 pence each.\n"
     " The log entry is:\n"
     " %^B_RED%^log shop sold 20 elephants for 200p%^RESET%^ (or $2 instead of 200p)\n\n"
     " If you do happen to make an error whilst logging a transaction, you\n"
     " may use the \"correction\" facility to note the error.  This will\n"
     " not erase the error, but will clarify that an error has been made.\n"
     " The syntax is the same as that of \"log\".\n\n"
	  " Please note that these are examples only, and you may be using a different\n"
	  " currency.\n\n",
     "common", 100 );

   set_open_page( 9 );
   set_read_mess( "\n"
     "                               The Storeroom                      Page 9\n"
     "                             -=-=-=-=-=-=-=-=-\n\n"
     " The storeroom, unsurprisingly, is where the shop's stock is stored.\n"
     " The stock itself is stored in several store cabinets, located in this\n"
     " room.  Each cabinet has a maximum stock capacity of "+ STOCK_PER_CABINET+ ".\n"
     " There are three commands available for dealing with the stock:\n\n"
     "   add - add items to the stock\n"
     "   remove - remove items from the stock\n"
     "   list - list all stock, or individual items\n\n"
     " These commands can be used relative to the whole stock, or by an\n"
     " individual cabinet.  For example, to add 10 beans to the stock, you\n"
     " could let the storeroom decide where to put the items by saying:\n"
     "   \"add 10 beans\"\n"
     " or specify a cabinet in which to place the beans:\n"
     "   \"add 10 beans to cabinet 1\"\n"
     " Generally, it is easier to use the first syntax and let the storeroom\n"
     " decide things for you.  Be aware that it will not let you place an item\n"
     " in the wrong cabinet, or in a cabinet that is full.\n"
     " For more information, see \"syntax <command>\"\n\n", "common", 100 );
     

    set_open_page( 10 );
    set_read_mess( "\n"
     "                              The Shopkeeper                     Page 10\n"
     "                           -=-=-=-=-=-=-=-=-=-\n\n"
     " With regard to the shopkeeper, when you leave the shop for other\n"
     " activities, it is important to clock out in order that they can be\n"
     " summoned from their home to staff the shop.  Conversely, when you clock\n"
     " in for work, say to them that you'll take over, and they will return\n"
     " home. Please also make sure that the cash register always contains\n"
     " plenty of the lowest denomination coins as, for religious reasons,\n"
     " they are incapable of handling the larger denominations of coinage.\n",
     "common", 100 );

   set_open_page( 11 );
   set_read_mess( "\n"
     "                              Promotion and pay                  Page 11\n"
     "                           -=-=-=-=-=-=-=-=-=-=-=-\n\n"
     " Promotion within the shop depends entirely on how much work an\n"
     " employee does.  Once an employee has done the required amount,\n"
     " they will be promoted automatically unless they have requested\n"
     " otherwise.  When you are promoted to supervisor, you will gain\n"
     " access to the shop's logs and also be able to add/remove items\n"
     " from the chart as well as adjust the buy/sell and maximum stock\n"
     " level of those items.  After even more work, the dedicated employees\n"
     " will be promoted to manager.\n"
     " Pay is also based on the amount of work that you do.  You will be\n"
     " paid each month directly into your bank account at whichever bank\n"
     " you specify in the office.  As a supervisor or manager, you will\n"
     " enjoy a higher rate of pay.\n"
     " Managers are responsible for allocating funds towards the monthly\n"
     " bonus fund.  This fund will be divided up between employees at the\n"
     " end of each month as long as those employees have been employed for\n"
     " the full month, and have not had their bonus suspended.\n"
     " It is up to each individual to claim their bonus during that month.\n"
     " All unclaimed bonuses will be added to the next month's fund.\n", 
     "common", 100 );

   set_open_page( 12 );
   set_read_mess( "\n"
     "                         Termination of employment                Page 12\n"
     "                      -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n"
     " If you no longer wish to work at the shop then you can resign from\n"
     " your position.  This can be done from the shop office.  If you do\n"
     " resign, please remember to return your employee badge.\n\n"
     " Managers have the power to fire employees for breach of trust.\n"
     " Reasons for dismissal include (but are not restricted to):\n"
     "     Dishonesty (Stealing items or cash from the shop)\n"
     "     Inactivity (Not working in the shop for prolonged periods)\n"
     "     The killing of any employee on duty\n"
     "     The killing of anybody whilst on duty\n"
     "     Not returning to the shop whilst on duty\n"
     "     Teleporting into or encoding the location of employee areas\n\n"
     " If you are fired without good reason, then you can appeal to any\n"
     " manager.  Bear in mind though, that all decisions taken with regard\n"
     " to the hiring and firing of employees will normally have been\n"
     " discussed by the managers beforehand and will usually have been\n"
     " preceded by a warning.\n", "common", 100 );

   set_open_page( 0 );
}
/* set_office() */


void init ()
{
   set_open_page( 0 );
   if ( !_office || _office == "" )
   {
      if ( !query_property( "office" ) )
      {
         return;
      }
      _office = query_property( "office" );
      set_office( _office );
   }
   if ( !_office->query_employee( environment( this_object() )->query_name() ) &&
     !_office->query_retired( environment( this_object() )->query_name() ) &&
     !environment( this_object() )->query_creator() )
   {
      return;
   }
   ::init();
}
/* init() */
