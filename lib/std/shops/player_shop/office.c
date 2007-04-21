/**
 * The standard inheritable object for player-run shop offices.
 * 
 * <p><b>Description</b></p>
 * <p>This office is the nerve centre of the shop. Most of the data
 * structures are defined and maintained from here, and virtually all
 * of the managerial functions are forwarded here. It is this object
 * that passes the common variables across to other files as they call
 * for them. This minimises the complexity of setting up each object - as
 * long as they point to this object, and this object is correctly set up,
 * the objects will know which shop they belong to, who the employees are,
 * who the proprietor is etc. 
 * </p>
 * <p>Before the shop will function correctly, it must be registered with
 * the handler - see set_very_short()
 * </p>
 * <p>Most of the functions defined in the office are not needed by
 * creators. Those that are needed are shown in the examples. Other
 * functions are not necessary, but may be useful. 
 * </p>
 * <p>The source to this object is split into 15 different files. The
 * main file, office.c includes the other files under the office_code
 * directory. This is intended to ease code maintenance as the total
 * size of the office code (as of December 2000) is around 165k. 
 * </p>
 * <p>In general, the functions contained in office.c are those intended
 * to be used directly by creators. The other functions are called
 * internally by the shop. 
 * </p>
 * <p>The data itself is also saved into several files. The data is
 * arranged into these files to minimise the amount of disk activity
 * when changes are made. For example, employee data changes every time an
 * employee does something. The list of applicants, however, changes only
 * when an application status changes. They are, therefore, saved into
 * seperate files. Also, each save call is subject to a callout, and will
 * limit the amount of disk writing actually done. The only data not stored
 * by this file is the stock data, which is saved by each cabinet
 * individually. All data and logs are saved into several files within
 * a directory "/d/(domain)/save/player_shops/(shop_very_short)/".
 * In addition, some data are cleared if it hasn't been used for a while.
 * This will typically save up to 100k of memory for a store the size of
 * Tarnach's.
 * </p>
 * <p>The shop supports notice boards, posting all hirings, promotions,
 * bonus amounts in addition to any posts requested in the inheriting
 * object. In the absence of a board, all posts are mailed to each employee
 * and each employee will have access to an additional "memo" command in
 * the office. 
 * </p>
 * <p>Applications are handled automatically, with managers voting whether
 * to accept or reject an application. On gaining positive votes from at
 * least 50% of the managers, an applicant is hired. If they receive more
 * than 50% negative votes, they are rejected. If there are insufficient
 * votes to settle the decision, an applicant will be hired if more managers
 * have voted for than against. Policy suggestions are handled in a similar
 * way. 
 * </p>
 * <p>The shop's two main administration routines are run on a regular
 * basis. The first of these is run every day and is responsible for
 * checking that employees are still valid players (not deleted chars or
 * creators). It conducts automatic promotions, and handles demotions for
 * inactive employees. It also updates the lists of declined applicants
 * and banned people and removes that status if applicable. Finally, it
 * calls the check_hire_list() function to see if we can hire any new
 * employees. 
 * </p>
 * <p>The second is run every Discworld month. This review involves paying
 * employees directly into their nominated bank account, and awarding
 * bonuses based on the current value of the bonus fund. 
 * </p>
 * 
 * @example
 * #include "path.h"
 * 
 * inherit "/std/shops/player_shop/office";
 *  
 * void setup()
 * {
 *    set_light(60);
 *    set_place("Lancre");
 *    set_proprietor("Tarnach Fendertwin");
 *    set_shop_name("Tarnach Fendertwin's Quality Consumables (Creel Springs)");
 *    set_very_short("TFQC-CS");
 *    set_channel("tarnachcs", 0);
 *    set_shopkeeper(PATH + "shopkeeper");
 *    set_stock_policy("magical spell components");
 * 
 *    set_shop_front(PATH+ "front");
 *    set_counter(PATH+ "counter");
 *    set_storeroom(PATH+ "storeroom");
 *    set_directions("east", "east", "east");
 * 
 *    set_short("office of Tarnach's shop");
 *    set_long("This room is the office of the Creel Springs branch of "
 *      "Tarnach Fendertwin's Quality Consumables.  There is a door to the "
 *      "managers' office in the south wall.\n");
 *    add_sign("The sign is a small piece of paper stuck to the wall.\n",
 *      "This seems to be a handwritten note from Tarnach himself.  "
 *      "The handwriting, not to mention the spelling and the grammar, "
 *      "is appalling, so it's difficult to make out much.  All you "
 *      "can understand is something about what the \"office\" is for.",
 *      "sign", "sign" )->add_property( "there", "on one wall");
 *    add_exit("east", PATH+ "counter", "door");
 *    add_manager_exit("south", PATH + "mgr_office");
 * }
 * 
 * @see /include/player_shop.h
 * @see /std/shops/player_shop/mgr_office.c
 * @see /std/shops/player_shop/counter.c
 * @see /std/shops/player_shop/storeroom.c
 * @see /std/shops/player_shop/shop_front.c
 * @see /std/shops/player_shop/shopkeeper.c
 * @author Ringo
 * @started 1st August 1999
 */
inherit "/std/room/basic_room";

#include <player_shop.h>
#include <mail.h>
#include <board.h>
#include <money.h>
#include <move_failures.h>
#include <refresh.h>
#include "office.h"


/*
 * Globals
 */
private nosave string _proprietor = UNSET_STR,
                      _shop_name = UNSET_STR,
                      _very_short = UNSET_STR, 
                      _place = UNSET_STR, 
                      _channel = UNSET_STR,
                      _storeroom = "",
                      _shop_front = "",
                      _counter = "",
                      _mgr_office = "",
                      _store_dir = "",
                      _counter_dir = "",
                      _shop_dir = "",
                      _shopkeeper = "",
                      _stock_policy = "",
                      _cab_name = "",
                      _review_month = "",
                      _savedir = "",
                      _creator = CREATOR;

private nosave object _chart = 0,
                      _board = 0,
                      _notice = 0;

private nosave mapping _history = 0,
                       _applicants = 0,
                       _policies = 0,
                       _new_policies = 0,
                       _list = ([]),
                       _times = ([]),
                       _employees = ([]);

/** Used to keep track of (and remove) call_outs */
private nosave int _call_save = 0,
                   _call_hist = 0,
                   _call_hist_clear = 0,
                   _call_times = 0,
                   _call_hire_list = 0,
                   _call_mail_hirees = 0,
                   _call_summon = 0,
                   _call_emps = 0,
                   _call_review = 0,
                   _call_apps = 0,
                   _call_apps_clear = 0,
                   _call_pols = 0,
                   _call_pols_clear = 0,
                   _call_newpols = 0,
                   _call_newpols_clear = 0;

private string *_retired = ({}),
               *_got_bonus = ({}),
               _last_month = "last month",
               _eom = CREATOR;

private mapping _baddies = ([]),
                _declined = ([]),
                _accounts = (["profit":0,"bonus":0]);

private int _max_emp = MAX_EMP,
            _bonus_val = 0, 
            _bonus = 0, 
            _pay_val = 4,
            _num_cabinets = MIN_CABINETS,
            _net_takings = 0;

private mixed *_register = ({});

/**
 * @ignore yes
 */
void create()
{  
   do_setup++;
   ::create();
   do_setup--;
   if (!do_setup)
   {
      this_object()->setup();
      this_object()->reset();
   }
   seteuid("Room");
   add_property("determinate", "");
   add_help_file("player_shop_office");
} /* create() */

/**
 * @ignore yes
 * Include rest of source - this is split up to keep filesizes within a
 * practical size limit.  This should make maintaining the source less of a
 * nightmare.  Only public functions will be found in this file.
 */
#include "office_code/admin.c"         // misc admin functions
#include "office_code/applications.c"  // application-related functions
#include "office_code/baddies.c"       // banning-related functions
#include "office_code/cabinets.c"      // cabinet-related functions
#include "office_code/emp_cmd.c"       // employee commands
#include "office_code/lists.c"         // listing functions
#include "office_code/logging.c"       // log-related functions
#include "office_code/masked.c"        // masked functions
#include "office_code/memo.c"          // memo functions
#include "office_code/personnel.c"     // personnel-related functions
#include "office_code/policies.c"      // policy-related functions
#include "office_code/review.c"        // daily & monthly review functions
#include "office_code/save.c"          // saving-related functions
#include "office_code/stock.c"         // stock-related functions

/******************************************************************************
 ******************************************************************************
 *                       START OF PUBLIC FUNCTIONS                            *
 ******************************************************************************
 ******************************************************************************/

/**
 * Set the exit to the managers' office.
 * This method also modifies the exit to allow only managers to enter the
 * managers' office.
 * @example add_manager_exit( "east", PATH + "tarnach's_man_office" );
 * @param dir the direction of the exit.
 * @param path the full path and filename to the managers' office of the shop.
 */
protected void add_manager_exit(string dir, string path)
{
   add_exit(dir, path, "door");
   modify_exit(dir, ({"function", ({this_object(),
     "check_manager"}), "door short", "office door", "door long",
     "There is a sign on the door which reads: \""+ _proprietor+ 
     "- Private\".\n"}));
   _mgr_office = path;
}
/* set_mgr_office() */


/**
 * Query the list of applicants.
 * This method returns the list of applicants as a mapping formatted
 * as follows:<br>
 * ([ "name": type, time, ({ for }), ({ against }), ({ abstain }), ])
 * @return the applicants mapping formatted as above.
 */
mapping get_applicants()
{
   load_applicants();
   clear_applicants();
   return copy(_applicants + ([]));
}

/**
 * Query the people banned from the shop.
 * This method returns a mapping of the people banned from this shop, along
 * with the time that they were banned, the person banning them, and the 
 * reason they were banned.  The format for this mapping is:<br>
 * ([ person:({ reason, banner, time }) ])
 * @return the mapping, formatted as above.
 */
mapping get_baddies() { return copy(_baddies + ([])); }

/**
 * Query the list of employees.
 * This method returns the list of employees, sorted alphabetically.
 * @see get_supervisors()
 * @see get_managers()
 * @see get_retired()
 * @return the sorted array of employees.
 */
string *get_employees()
{
   string *employees = m_indices(_employees);

   /* Don't include people with the supervisor bit set */
   foreach (string word in employees)
      if (_employees[word][EMP_POINTS] & SUPERVISOR)
         employees -= ({word});
   return copy(sort_array(employees, 1));
}
/* get_employees() */

/**
 * Query the list of managers.
 * This method returns the list of managers, sorted alphabetically.
 * @see get_employees()
 * @see get_supervisors()
 * @see get_retired()
 * @return the sorted array of managers.
 */
string *get_managers()
{
   return copy(sort_array(keys(filter(_employees,
        (: _employees[$1][EMP_POINTS] & MANAGER :))), 1));
}
/* get_managers() */

/**
 * Query the policy suggestions.
 * These are the policies that have been proposed but not yet implemented.
 * @see get_policies()
 * @see query_policy()
 * @return a mapping of the suggested shop policies.
 */
mapping get_new_policies()
{ 
   load_new_policies();
   clear_new_policies();
   return copy(_new_policies);
}

/**
 * Query the policies.
 * These are the policies currently in effect.
 * @see get_new_policies()
 * @see query_policy()
 * @return a mapping of the shop policies.
 */
mapping get_policies(int type)
{
   load_policies();
   clear_policies();
   if (type)
      return copy(filter(_policies, (: _policies[$1][1] :)));
   else
      return copy(filter(_policies, (: !_policies[$1][1] :)));
}
/* get_policies() */

/**
 * Query the list of retired managers.
 * This method returns the list of retired managers, sorted alphabetically.
 * @see get_employees()
 * @see get_supervisors()
 * @see get_managers()
 * @return the sorted array of retired managers.
 */
string *get_retired() { return copy(sort_array(_retired, 1)); }

/**
 * Query the list of supervisors.
 * This method returns the list of supervisors, sorted alphabetically.
 * @see get_employees()
 * @see get_managers()
 * @see get_retired()
 * @return the sorted array of supervisors.
 */
string *get_supervisors()
{
   string *supervisors = keys(filter(_employees,
        (: _employees[$1][EMP_POINTS] & SUPERVISOR :)));

   /* Managers also have the supervisor bit set, so don't
    * include them in this list.
    */
   foreach (string word in supervisors)
      if (_employees[word][EMP_POINTS] & MANAGER)
         supervisors -= ({word});
   return copy(sort_array(supervisors, 1));
}
/* get_supervisors() */

/**
 * Query the number of employees currently clocked in.
 * This function will also clock out any employees that are no longer on DW.
 * @return the number of employees clocked in. 
 */
int num_employees_in()
{
   int any = 0;

   foreach (string word in m_indices(_employees))
      if (_employees[word][EMP_POINTS] & CLOCKED_IN)
      {
         if (_employees[word][EMP_POINTS] & NPC) continue;
         if (!find_player(word) || !interactive(find_player(word)))
         {
            reset_employee(word, CLOCKED_IN);
            shop_log(GENERAL, word, "was clocked out", UNPAID);
         }
         else any++;
      }
   return any;
}
/* num_employees_in() */

/**
 * Determine if this person has applied for a job.
 * @param player the player to query.
 * @return APPLIED if applied, HIRED if voted in, AWAITING if awaiting 
 * a vacancy.  Otherwise, will return FALSE.
 */
int query_applicant(string player)
{
   load_applicants();
   clear_applicants();
   if (!sizeof(_applicants)) return FALSE;
   if (_applicants[player]) return copy(_applicants[player][APP_TYPE]);
   return FALSE;
}
/* query_applicant() */

/**
 * Determine if this person is banned from the shop.
 * @param player the player to query.
 * @return the time of the ban if banned, FALSE if not banned.
 */
int query_baddie(string player)
{
   if (!m_sizeof(_baddies)) return FALSE;
   if (_baddies[player]) return copy(_baddies[player][BAD_TIME]);
   return FALSE;
}
/* query_baddie() */

/**
 * Query the value of the bonus account.
 * This is the current value of the bonus account.  Will need converting to 
 * local currency if it is to be displayed.
 * @see query_profit()
 * @return the value of the bonus account.
 */
int query_bonus() { return copy(_accounts["bonus"]); }

/**
 * Query the channel used by the shop.
 * @see set_channel()
 * @return the channel in use by this shop
 */
string query_channel() { return copy(_channel); }

/**
 * Query the path to the shop counter.
 * @see set_counter()
 * @return the path to the counter.
 */
string query_counter() { return copy(_counter); }

/**
 * Query the maintainer of this shop's files.
 * @see set_creator()
 * @return the person responsible for this shop.
 */
void query_creator(string creator) { return copy(_creator); }

/**
 * Determine if a player was declined for a job.
 * This method is used to determine if there is a declined application
 * registered for a player.
 * @param player the player to query.
 * @return FALSE, or the time at which the applicant was declined. 
 */
int query_declined(string player)
{
   if (!sizeof(_declined)) return FALSE;
   if (_declined[player]) return copy(_declined[player]);
   return FALSE;
}
/* query_declined() */


/**
 * Determine if this person is an employee of the shop.
 * @see query_supervisor()
 * @see query_manager()
 * @see query_retired()
 * @param player the player to query.
 * @return the employee's points, or FALSE if not an employee.
 */
int query_employee(string player)
{
   if (_employees[player]) return copy(_employees[player][EMP_POINTS]);
   return FALSE;
}
/* query_employee() */

/**
 * Return the employee data.
 * This includes only active employees - retired managers are not included.
 * The data is formatted as:<br>
 * ([ employee:({ points, time, bank, pay, inactive, nobonus, nopromote }) ])<br>
 * @return the employee mapping, formatted as above.
 */
mapping query_employees() { return copy(_employees + ([])); }

/**
 * Query the list of items sold by the shop.
 * Generates an array of the keys to the list mapping.
 * @see query_list_mapping()
 * @see query_list_string()
 * @return the list of items in array form.
 */
string *query_list_array() { return copy(m_indices(_list) + ({})); }

/**
 * Query the list of items sold by the shop.
 * @see query_list_array()
 * @see query_list_string()
 * @return the list of items in mapping form.
 */
mapping query_list_mapping() { return copy(_list) + ([]); }

/**
 * Query the list of items sold by the shop.
 * Generates a list of all items bought & sold by this shop, and outputs a
 * multiple short string for use in displays.
 * @see query_list_array()
 * @see query_list_mapping()
 * @return the list of items.
 */
string query_list_string()
{
   if (!m_sizeof(_list)) return "absolutely nothing at the moment";
   return query_multiple_short(m_indices(_list));
}
/* query_list_string() */

/**
 * Determine if this person is a manager of this shop.
 * @see query_employee()
 * @see query_supervisor()
 * @see query_retired()
 * @param player the player to query.
 * @return TRUE or FALSE
 */
int query_manager(string player)
{
   if (_employees[player]) return (_employees[player][EMP_POINTS] & MANAGER);
   return FALSE;
}
/* query_manager() */

/**
 * Query the maximum number of employees.
 * @return the maximum number of employees allowed at this shop.
 */
int query_maxemp() { return copy(_max_emp); }

/**
 * Query the path to the managers' office.
 * @return the path to the managers' office.
 */
string query_mgr_office() { return copy(_mgr_office); }

/**
 * Query the number of storeroom cabinets.
 * @return the number of cabinets in the storeroom.
 */
int query_num_cabinets() { return copy(_num_cabinets); }

/**
 * Query the base pay rate.
 * This is the base pay rate set by the managers; the amount each employee
 * will receive for a single transaction.
 * @return the value of the base pay rate
 */
int query_pay() { return copy(_pay_val); }

/**
 * Query the location of this shop.
 * @see set_place()
 * @return the location of the shop
 */
string query_place() { return copy(_place); }

/**
 * Determine if a shop policy exists.
 * @see get_policies()
 * @see get_new_policies()
 * @param policy The name of the policy to query.
 * @return 2 if it is already policy, 1 if it is a proposal, else 0
 */
int query_policy(string policy)
{
   load_policies();
   clear_policies();
   if (m_sizeof(_policies) && _policies[policy]) return 2;
   load_new_policies();
   clear_new_policies();
   if (m_sizeof(_new_policies) && _new_policies[policy]) return 1;
   return 0;
}
/* query_policy() */

/**
 * Query the value of the profit account.
 * This is the current value of the profit account.  Will need converting to 
 * local currency if it is to be displayed.
 * @see query_bonus()
 * @return the value of the profit account
 */
int query_profit() { return copy(_accounts["profit"]); }

/**
 * Query the name of the proprietor.
 * @see set_proprietor()
 * @return the name of the proprietor
 */
string query_proprietor() { return copy(_proprietor); }

/**
 * Determine if this person is a retired manager of this shop.
 * @see query_employee()
 * @see query_supervisor()
 * @see query_manager()
 * @param player The player to query.
 * @return TRUE or FALSE
 */
int query_retired(string player)
{
   return (member_array(player, _retired) == -1)?FALSE:TRUE;
}
/* query_retired() */

/** @ignore yes */
string query_savedir() { return copy(_savedir); }

/**
 * Query the path to the shop front.
 * @see set_shop_front()
 * @return the path to the shop front
 */
string query_shop_front() { return copy(_shop_front); }

/**
 * Query the full name of the shop.
 * @see set_shop_name()
 * @return the full name of the shop
 */
string query_shop_name() { return copy(_shop_name); }

/**
 * Query the shopkeeper name.
 * @see set_shopkeeper()
 * @return the name of the shopkeeper
 */
string query_shopkeeper() { return copy(_shopkeeper); }


/**
 * Query number of items in stock.
 * Determines the number of a specific item currently held by this shop.
 * @param items the item to query
 * @return the number of 'items' in stock
 */
int query_stock(string items) { return _storeroom->query_num_items(items, 0); }

/**
 * Query the path to the storeroom.
 * @see set_storeroom()
 * @return the path to the storeroom
 */
string query_storeroom() { return copy(_storeroom); }

/**
 * Determine if this person is a supervisor of this shop.
 * @see query_employee()
 * @see query_manager()
 * @see query_retired()
 * @param player The player to query.
 * @return TRUE or FALSE
 */
int query_supervisor(string player)
{ 
   if (_employees[player])
      return (_employees[player][EMP_POINTS] & SUPERVISOR);
   return FALSE;
}
/* query_supervisor() */

/**
 * Save the employee data file.
 * This method uses a call_out to help minimise the amount of disk activity
 * during normal operations of the shop.  The very short name of the shop
 * must have been set previously.
 * @see save_me()
 * @see set_very_short()
 */
protected void save_emps()
{
   if (_very_short == UNSET_STR) return;
   remove_call_out(_call_emps);
   _call_emps = call_out((: do_save_emps() :), SAVE_DELAY);
}
/* save_emps() */

/**
 * Save the shop data file.
 * This method uses a call_out to help minimise the amount of disk activity
 * during normal operations of the shop.  The very short name of the shop
 * must have been set previously.
 * @see save_emps()
 * @see set_very_short()
 */
protected void save_me()
{
   if (_very_short == UNSET_STR) return;
   remove_call_out(_call_save);
   _call_save = call_out((: do_save() :), SAVE_DELAY);
}
/* save_me() */

/**
 * Set the channel used by the shop.
 * This sets the channel used by the employees' badges, and also the
 * name of the board.  If a board has been set-up for this shop, this
 * function will also add the board into the room.
 * @example set_channel( "tarnachcs", 0 );
 * @see query_channel()
 * @param name the name of the channel.
 * @param board non-zero if a board exists for this shop.
 */
protected void set_channel(string name, int board)
{
   _channel = lower_case(name);
   if (board)
   {
      _board = clone_object("/obj/misc/board");
      _board->set_datafile(name);
      _board->move(this_object());
   }
}
/* set_channel() */

/**
 * Set the path to the counter.
 * This is the full path and filename of the counter object to be used by this
 * shop.
 * @example set_counter( PATH + "counter" );
 * @see query_counter()
 * @param path The full path and filename to the shop's counter.
 */
protected void set_counter(string path) { _counter = path; }

/**
 * Set the creator of this shop.
 * This person will receive all applications, complaints, suggestions
 * etc in the absence of any managers.  Default is set by CREATOR in
 * <player_shop.h>
 * @example set_creator( "ringo" );
 * @see query_creator()
 * @param creator The person responsible for this shop.
 */
protected void set_creator(string creator) { _creator = creator; }

/**
 * Set the directions to other parts of the shop.
 * This function is used by the npc shopkeeper to navigate around the shop, 
 * using the exits at the given directions.  These directions should be the 
 * standard "north", "south", "up" etc.
 * @example set_directions( "southeast", "southeast", "southeast" );
 * @param store the direction to the storeroom.
 * @param counter the direction to the counter.
 * @param shop the direction to the shop front.
 */
protected void set_directions(string store, string counter, string shop)
{
   _store_dir = store;
   _counter_dir = counter;
   _shop_dir = shop;
}
/* set_directions() */

/**
 * Set the location of the shop.
 * This is used by the money handling functions to determine which 
 * currency to use and therefore should be one of the locations returned
 * by the query_all_places() function in /obj/handlers/money_handler
 * @example set_place( "Lancre" );
 * @see query_location()
 * @param place the location of this shop.
 */
protected void set_place(string place) { _place = place; }

/**
 * Set the name of the proprietor.
 * A fictional name; the owner of this establishment.  All administration
 * board posts & mudmails will be sent by this name.
 * @example set_proprietor( "Tarnach Fendertwin" );
 * @see query_proprietor()
 * @param name the name of the proprietor.
 */
protected void set_proprietor(string name) { _proprietor = name; }

/**
 * Set the path to the customer area.
 * @example set_shop_front( PATH + "shopfront" );
 * @see query_shop_front()
 * @param path the full path and filename to the customer area of the shop.
 */
protected void set_shop_front(string path) { _shop_front = path; }

/**
 * Set the full name of the shop.
 * This is used throughout the shop, and passed to the shop front as the
 * short description for the shop.
 * @example set_shop_name( "Tarnach Fendertwin's Quality Consumables (Creel Springs)" );
 * @see query_shop_name()
 * @param name the name of the shop.
 */
protected void set_shop_name(string name) { _shop_name = name; }

/**
 * Set the npc shopkeeper object.
 * @example set_shopkeeper( PATH + "shopkeeper" );
 * @see /std/shops/player_shop/shopkeeper.c
 * @param path the full path to the shopkeeper.
 */
protected void set_shopkeeper(string path)
{ 
   _shopkeeper = path;
   path=_shopkeeper->query_name();
   if (!_employees || !_employees[path])
   {
      _employees += ([path:EMP_MAP]);
      _employees[path][EMP_POINTS] = EMPLOYEE + NPC; 
   }
}

/**
 * Set the path to the storeroom.
 * @example set_storeroom( PATH + "storeroom" );
 * @see query_storeroom()
 * @param path the full path and filename to the storeroom of the shop.
 */
protected void set_storeroom(string path) { _storeroom = path; }

/**
 * Set the stock's main policy.
 * This is a general description of the items that this store deals in.
 * @example set_stock_policy( "magical spell components" );
 * @param desc the main stock description.
 */
protected void set_stock_policy(string desc) { _stock_policy = desc; }

/**
 * Set the very short name of the shop.
 * This is used in many places including save-file names, setting up 
 * player-titles, and mail headers.  It should be no more than around
 * 4 or 5 chars in length.  This function also restores all saved data.
 * You must register this name by calling add_shop() in the handler.
 * @example set_very_short( "TFQC-CS" );
 * @see shop_very_short()
 * @see /obj/handlers/player_shop
 * @param name the very short name.
 */
protected void set_very_short(string name)
{
   if (PLAYER_SHOP->query_shop(name) != file_name(this_object()))
      return;
   _very_short = name;
   _savedir = sprintf("/save/player_housing/%s/player_shops/%s/",
      lower_case(geteuid(this_object())), name);
   if (file_size(_savedir+ "shop_data.o") > 0)
      unguarded((: restore_object, _savedir+ "shop_data" :));
   if (file_size(_savedir+ "employees") > 0)
      _employees = restore_variable(unguarded((: read_file,
        _savedir+ "employees" :)));
   if (file_size(_savedir+ "times") > 0)
      _times = restore_variable(unguarded((: read_file,
      _savedir+ "times" :)));
   if (file_size(_savedir+ "list") > 0)
      _list = restore_variable(unguarded((: read_file, _savedir+ "list" :)));
}
/* set_very_short() */

/**
 * Query the very short name of the shop.
 * @see set_very_short()
 * @return the very short name of the shop.
 */
string shop_very_short() { return copy(_very_short); }
