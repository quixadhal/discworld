/*
 * Some useful macros
 */
#define clear_applicants() _call_apps_clear = \
                         call_out((: _applicants = 0 :), CLEAR_DELAY)
#define clear_history() _call_hist_clear = \
                         call_out((: _history = 0 :), CLEAR_DELAY)
#define clear_new_policies() _call_newpols_clear = \
                             call_out((: _new_policies = 0 :), CLEAR_DELAY)
#define clear_policies() _call_pols_clear = \
                         call_out((: _policies = 0 :), CLEAR_DELAY)

#define save_applicants() _call_apps = \
                          call_out((: do_save_applicants() :), SAVE_DELAY);
#define save_hist() _call_hist = \
                          call_out((: do_save_hist() :), SAVE_DELAY);
#define save_new_policies() _call_newpols = \
                          call_out((: do_save_new_policies() :), SAVE_DELAY);
#define save_policies() _call_pols = \
                          call_out((: do_save_policies() :), SAVE_DELAY);

/*
 * Prototypes
 * Where a function exists outside of office.c, the relevant .c file is indicated
 */
void     add_applicant(string,string);   //applications.c
private void add_baddie( string, string, string);   //baddies.c
private void add_board_message(string, string);   //admin.c
private void add_chart_cabinet(string, int);   //stock.c
private void add_declined(string);   //applications.c
private void add_employee(string);   //personnel.c
protected void add_manager_exit(string, string);
private void add_policy(string);   //policies.c
void     add_policy_suggest(string, string, string, int);   //policies.c
private void add_vote(string, int);   //applications.c
private void adjust_bonus(string, int);   //admin.c
void     adjust_bought(string, int);   //stock.c
private void adjust_profit(string, int);   //admin.c
void     adjust_sold(string, int);   //stock.c
void     adjust_takings(int);   //admin.c
string   badge_list();   //lists.c
void     calc_bonus();   //admin.c
int      calc_pay();   //admin.c
private string cap_name(string); //admin.c
void check_hire_list();   //applications.c
int      check_manager(string);   //admin.c
void     confirm_cabinet(string, int);   //cabinets.c
private void demote(string, string);   //personnel.c
string   directions_to(string);   //admin.c
private int do_ban(mixed *);   //baddies.c
private int do_bank(mixed);   //emp_cmd.c
int do_cancel();   //applications.c
private int do_chart(mixed *, string);   //stock.c
private int do_check();   //cabinets.c
private int do_claim(string);   //emp_cmd.c
private int do_clock(string);   //emp_cmd.c
int      do_commend(string);  //personnel.c
private int do_confirm();   //applications.c
int      do_demote(string);   //personnel.c
int      do_fire(mixed *);   //personnel.c
int      do_leave(mixed *);   //personnel.c
int      do_list();   //lists.c
int      do_logs(mixed *, string);   //logging.c
private int do_memo();   //memo.c
private int do_office();   //lists.c
private int do_policy_vote(string, string, string);   //policies.c
private int do_promote(string);   //personnel.c
int      do_remove();   //cabinets.c
int      do_rent();   //cabinets.c
private int do_resign();   //personnel.c
int      do_retire();   //personnel.c
private void do_save();   //save.c
private void do_save_applicants(); //save.c
private void do_save_emps();   //save.c
private void do_save_hist();   //save.c
private void do_save_new_policies(); //save.c
private void do_save_policies(); //save.c
int      do_set(mixed *, string);   //admin.c
int      do_suspend(mixed *);   //personnel.c
int      do_transfer(mixed *);   //admin.c
int      do_unban(string);   //baddies.c
int      do_vote(mixed *, string);   //applications.c
int      do_warn(mixed *);  //personnel.c
private void employee_log(string, string);   //logging.c
string   employees_clocked_in();   //lists.c
private void end_memo(string,string);   //memo.c
private void fire_them(string, string, string);  //personnel.c
mapping  get_applicants();
mapping  get_baddies();
string  *get_employees();
string  *get_managers();
mapping  get_new_policies();
mapping  get_policies(int);
string  *get_retired();
string   get_stock_policy();   //policies.c
string  *get_supervisors();
private void hire(string);   // personnel.c
private string list_stuff(string);   //lists.c
private void load_applicants(); //admin.c
private void load_history(); //admin.c
private void load_new_policies(); //admin.c
private void load_policies(); //admin.c
private void mail_hirees();   //applications.c
private void monthly_review();   //review.c
int      num_employees_in();
private void put_on_list(string);   //stock.c
int      query_applicant(string);
int      query_ave(string);   //stock.c
int      query_baddie(string);
int      query_bonus();
int      query_buy(string);   //stock.c
int     *query_cabinet(string);   //cabinets.c
int      query_cabinet_used(int);   //cabinets.c
string   query_channel();
string   query_counter();
void     query_creator(string);
int      query_declined(string);
int      query_employee(string );
mapping  query_employees();
string  *query_list_array();
string   query_list_string();
int      query_manager(string);
int      query_max(string);   //stock.c
int      query_maxemp();
string   query_mgr_office();
int      query_num_cabinets();
int      query_on_list(string);   //stock.c
int      query_pay();
string   query_place();
int      query_policy(string);
int      query_profit();
string   query_proprietor();
int      query_retired(string);
int      query_sell(string);   //stock.c
string   query_shop_front();
string   query_shop_name();
string   query_shopkeeper();
int      query_stock(string);
string   query_storeroom();
int      query_supervisor(string);
private string query_worked(string);   //personnel.c
private void remove_applicant(string);   //applications.c
private void remove_baddie(string);   //baddies.c
private void remove_chart_cabinet(string, int);   //cabinets.c
private void remove_declined(string);   //applications.c
private void remove_employee(string);   //admin.c
private void remove_policy(string);   //policies.c
private void remove_retired(string);   //admin.c
private void reset_employee(string, int);   //admin.c
mixed   *restore_register();   //admin.c
private void review_employees();   //review.c
private void rm_cab();   //cabinets.c
protected void save_emps();
private void save_list();   //save.c
protected void save_me();
void     save_register(mixed *);   //save.c
private void save_times();   //save.c
private void send_memo(string, string, string);   //memo.c
private void set_buy(string, int);   //stock.c
protected void set_channel(string, int);
protected void set_counter(string);
protected void set_creator(string);
protected void set_directions(string, string, string);
private void set_emp_time(string);   //personnel.c
private void set_employee(string, int);   //admin.c
private void set_max(string, int);   //stock.c
protected void set_place(string);
protected void set_proprietor(string);
private void set_sell(string, int);   //stock.c
protected void set_shop_front(string);
protected void set_shop_name(string);
protected void set_shopkeeper(string);
protected void set_stock_policy(string);
protected void set_storeroom(string);
protected void set_very_short(string);
void     shop_log(int, string, string, int);   // logging.c
string   shop_very_short();
void     summon_shopkeeper();   //admin.c
private void take_off_list(string);   //stock.c
private void update_averages();   //stock.c
void     view_record(string,string);   //personnel.c
private void write_memo(string); //memo.c


/*
 * Patterns used within add_commands
 */

/* Office chart command */
#define CHART_ADDITEM       "add <string'item'>"
#define CHART_REMITEM       "remove <string'item'>"
#define CHART_MAXITEM       "max <string'item'> <number>"
#define CHART_BUYITEM       "buy <string'item'> <number>"
#define CHART_SELLITEM      "sell <string'item'> <number>"
#define CHART_ASSIGN_ITEM   "assign <string'item'> <number'cabinet number'>"
#define CHART_UNASSIGN_ITEM "unassign <string'item'> <number'cabinet number'>"

/* Offices logs command */
#define LOG_BLANK  ""
#define LOG_NUMBER "<number>"
#define LOG_MGR    "{personnel|accounts|chart}"
#define LOG_CHART  "chart"

/* Mgr Office list command */
#define LIST_BLANK  ""
#define LIST_CHOICE "{employees|applicants|baddies}"

/* Mgr Office set command */
#define SET_EMPS "maximum <number> [employees]"
#define SET_PAY  "pay <number'amount'> <string'type'>"

/* Mgr Office vote command */
#define VOTE_APPLICANT "<word'applicant'> {y|n|a}"
#define VOTE_POLICY    "policy <word'name'> {y|n}"

/* Mgr Office view command */
#define VIEW_EMP "<string'employee'>"
#define VIEW_APP "application [by] <string'applicant'>"

/* Mgr Office policy command */
#define POLICY_SUGGEST "suggest {manager|shop} <word'policy'>"
#define POLICY_REMOVE  "remove <word'policy'>"

/* Stockroom list command */
#define LIST_CABINET      "cabinet <number'cabinet'>"
#define LIST_ITEM         "<string'item'>"
#define LIST_ITEM_CABINET "<string'item'> in cabinet <number'cabinet'>"
