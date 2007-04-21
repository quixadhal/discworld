/**
 * The standard inheritable object for player-run shop manager offices.
 *
 * <p><b>Description</b></p>
 * <p>The managers' office provides a front-end to the manager commands,
 * although these are mainly handled by the _office_. It also provides a
 * place of refuge for managers since only they have access to this room,
 * as well as allowing them to send and read their mail. Finally, it allows
 * them to claim a manager's handbook. 
 * </p>
 * <p>From here, managers can vote on employment applications and policy
 * suggestions. They can make personnel decisions such as commendations,
 * warnings, bonus suspensions, firing and demoting employees. They can
 * place employees on leave for a certain period if they know that employee
 * is away. They can view the ordinary, accounts, personnel and chart logs.
 * They can rent or remove additional stock cabinets. They can ban or unban
 * people from the shop. They can transfer monies between the profit account,
 * bonus account and cash register (see below). Finally, they can retire and
 * stay on as a retired manager, taking no further active role in the shop
 * other than voting on policy suggestions. 
 * </p>
 * 
 * <p><b>Accounts</b></p>
 * <p>There are three accounts in the shop, the first of which is the
 * register. This contains the 'float' - the money used for buying and
 * selling stock. The register is located behind the counter and is
 * accessible to every employee. 
 * </p>
 * <p>The profit account contains the funds used to pay operating expenses
 * such as cabinet rentals and employee pay. 
 * </p>
 * <p>The bonus account holds the funds used to pay the monthly bonus. 
 * </p>
 * <p>Monies can be transferred between all three accounts at any time, as
 * long as none would go negative as a result. 
 * </p>
 * 
 * @example
 * #include "path.h"
 * 
 * inherit "/std/shops/player_shop/mgr_office";
 * 
 * void setup()
 * {
 *    set_light(60);
 *    set_office( PATH+ "office" );
 * 
 *    set_short( "Tarnach Fendertwin's office" );
 *    set_long( "This room is the office of Tarnach Fendertwin himself.  "
 *     "Since he rarely visits this branch of the chain, it is more "
 *      "commonly used by the shop's managers.\n" );
 *    add_exit( "north", PATH +"office", "door" );
 * }
 * 
 * @see /include/player_shop.h
 * @see /std/shops/player_shop/office.c
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
#include <move_failures.h>
#include <money.h>
#include "patterns.h"

private nosave string _office = "", 
_place = "", 
_shop_front = "", 
_counter = "",
_storeroom = "";

private nosave object _notice = 0;

private string cap_name(string);
private int do_accounts();
private int do_ban(mixed *);
private int do_check();
private int do_claim();
private int do_commend(string);
private int do_demote(string);
private int do_fire(mixed *);
private int do_leave(mixed *);
private int do_list(mixed *);
private int do_logs(mixed *, string);
private int do_mail(string);
private int do_memo();
private int do_office();
private int do_policies();
private int do_policy(mixed *,string);
private int do_project(string);
private int do_query(string);
private int do_remove();
private int do_rent();
private int do_retire();
private int do_set(mixed *, string);
private int do_suspend(mixed *);
private int do_transfer(mixed *);
private int do_unban(string);
private int do_view(string,string);
private int do_vote(mixed *, string);
private int do_warn(mixed *);
private void end_memo(string,string);
private void end_policy_suggest(string,string,int);
private void send_memo(string, string, string);
protected void set_office(string);
private void write_memo(string);

/** @ignore yes */
void create()
{
    do_setup++;
    ::create();
    do_setup--;
    add_property("determinate", "");
    if (!do_setup)
    {
        this_object()->setup();
        this_object()->reset();
    }
    add_help_file("player_shop_mgr_office");
}
/* create() */


/** @ignore yes */
void init()
{
    ::init();
    if (!_office || _office == "") return;
    if ( !_office->query_manager(this_player()->query_name()) &&
      !_office->query_retired(this_player()->query_name()) &&
      !this_player()->query_creator()) return;
    add_command("list", ({LIST_BLANK, LIST_CHOICE}),
      (: do_list($4) :));
    add_command("memo", "", (: do_memo() :));
    add_command("office", "", (: do_office() :));
    add_command("claim", "", (: do_claim() :));
    add_command("accounts", "", (: do_accounts() :));
    add_command("logs", ({LOG_BLANK, LOG_NUMBER, LOG_MGR}),
      (: do_logs($4,$5) :));
    add_command("mail", ({"", "<string>"}), (: do_mail($4) :));
    add_command("query", "{pay|employees}", (: do_query($4[0]) :));
    add_command("view", ({VIEW_EMP, VIEW_APP}), (: do_view($4[0],$5) :));
    add_command("project", "{pay|bonus}", (: do_project($4[0]) :));
    add_command("vote", VOTE_POLICY, (: do_vote($4,$5) :));
    add_command("policy", ({POLICY_SUGGEST, POLICY_REMOVE}),
      (: do_policy($4,$5) :));
    add_command("policies", "", (: do_policies() :));

    if (_office->query_retired(this_player()->query_name())) return;
    add_command("rent", "cabinet", (: do_rent() :));
    add_command("remove", "cabinet", (: do_remove() :));
    add_command("retire", "", (: do_retire() :));
    add_command("vote", VOTE_APPLICANT, (: do_vote($4,$5) :));
    add_command("check", "votes", (: do_check() :));
    add_command("commend", "<word'employee'>", (: do_commend($4[0]) :));
    add_command("fire", "<word'employee'> [for] <string'reason'>",
      (: do_fire($4) :));
    add_command("warn", "<word'employee'> [for] <string'reason'>",
      (: do_warn($4) :));
    add_command("demote", "<word'employee'>", (: do_demote($4[0]) :));
    add_command("suspend", "<word'employee'> [for] <number> [months]",
      (: do_suspend($4) :));
    add_command("leave", "<word'employee'> [for] <number> [days]",
      (: do_leave($4) :));
    add_command("ban", "<word'person'> [for] <string'reason'>",
      (: do_ban($4) :));
    add_command("unban", "<word'person'>", (: do_unban($4[0]) :));
    add_command("set", ({SET_EMPS, SET_PAY}), (: do_set($4,$5) :));
    add_command("transfer", "<number'amount'> <string'type'> [from] "
      "{register|bonus|profit} [to] {register|bonus|profit}",
      (: do_transfer($4) :));
}
/* init() */


/** @ignore yes */
private string cap_name(string name) 
{
    if (test_player(name))
        return PLAYER_HANDLER->query_cap_name(name);
    else return capitalize(name);
}

/** @ignore yes */
void dest_me()
{
    if (_notice) _notice->dest_me();
    ::dest_me();
}
/* dest_me() */


/** @ignore yes */
private int do_accounts()
{
    tell_object(this_player(), "The shop's accounts are as follows:\n"+
      sprintf("   Cash   : %s\n   Bonus  : %s\n   Profit : %s\n",
        MONEY_HAND->money_value_string(_counter->query_register(), _place),
        MONEY_HAND->money_value_string(_office->query_bonus(), _place),
        MONEY_HAND->money_value_string(_office->query_profit(), _place)));
    return 1;
}
/* do_accounts() */


/** @ignore yes */
private int do_ban(mixed *args) { return _office->do_ban(args); }


/** @ignore yes */
private int do_check()
{
    string results,
    name = this_player()->query_name();
    mapping applicants;

    add_succeeded_mess("");
    applicants = _office->get_applicants();
    applicants = filter(applicants, (: $(applicants)[$1][APP_TYPE] == 1 :));
    if (this_player()->query_creator())
    {
        results = "The following votes have been cast:\n\n";
        foreach (string word in sort_array(keys(applicants), 1))
        {
            results += cap_name(word)+ " - applied " +
            ctime(applicants[word][APP_TIME] )+ "\n   Votes for:     ";
            foreach (string vote in applicants[word][APP_FOR])
            results += cap_name(vote) + "  ";
            results += "\n   Votes against: ";
            foreach (string vote in applicants[word][APP_AGAINST])
            results += cap_name(vote) + "  ";
            results += "\n   Abstentions:   ";
            foreach (string vote in applicants[word][APP_ABSTAIN])
            results += cap_name(vote) + "  ";
            results += "\n\n";
        }
        tell_object(this_player(), "$P$Votes cast$P$"+ results);
        return 1;
    }
    results = "You have cast the following votes:\n\nFor: ";
    foreach (string vote in sort_array(keys(applicants), 1))
    if (member_array(name, applicants[vote][APP_FOR]) != -1)
        results += cap_name(vote)+ "  ";
    results += "\nAgainst: ";
    foreach (string vote in sort_array(keys(applicants), 1))
    if (member_array(name, applicants[vote][APP_AGAINST]) != -1)
        results += cap_name(vote)+ "  ";
    results += "\nAbstentions: ";
    foreach (string vote in sort_array(keys(applicants), 1))
    if (member_array(name, applicants[vote][APP_ABSTAIN]) != -1)
        results += cap_name(vote)+ "  ";
    tell_object(this_player(), results + "\n");
    return 1;
}
/* do_check() */


/** @ignore yes */
private int do_claim()
{
    object thing;

    if (!_office || _office == "") return 0;
    thing = clone_object(MGR_BOOK);
    thing->set_read_mess("\n   "+ _office->query_shop_name()+ "\n\n"
      "   Manager's Handbook\n\n   This handbook belongs to: "+
      this_player()->query_short()+ "\n\n", "common", 100);
    thing->set_office(_office);
    if (thing->move(this_player()) != MOVE_OK)
    {
        thing->move(this_object());
        tell_object(this_player(),
          "You drop your handbook as you're carrying too much.\n");
    }
    add_succeeded_mess("$N $V a new handbook.\n");
    return 1;
}
/* do_claim() */


/** @ignore yes */
private int do_commend(string emp) { return _office->do_commend(emp); }


/** @ignore yes */
private int do_demote(string emp) { return _office->do_demote(emp); }


/** @ignore yes */
private int do_fire(mixed *args) { return _office->do_fire(args); }


/** @ignore yes */
private int do_leave(mixed *args) { return _office->do_leave(args); }


/** @ignore yes */
private int do_list(mixed *args)
{
    string results, word;
    mapping baddies, applicants;
    string *applied, *hired, *awaiting;

    if (!sizeof(args)) args = ({"employees"});
    add_succeeded_mess("");
    switch (args[0])
    {
    case "applicants" : 
        results = "      Applications for employment as at " +
        ctime(time()) + "\n\n";
        applicants = _office->get_applicants();
        if (!applicants || applicants == ([]))
        {
            tell_object(this_player(), results+
              "No applications outstanding.\n");
            return 1;
        }
        applied = sort_array(keys(filter(applicants,
              (: $(applicants)[$1][APP_TYPE] == APPLIED :))), 
          (: $(applicants)[$1][APP_TIME] - $(applicants)[$2][APP_TIME] :));
        hired = sort_array(keys(filter(applicants,
              (: $(applicants)[$1][APP_TYPE] == HIRED :))),
          (: $(applicants)[$1][APP_TIME] - $(applicants)[$2][APP_TIME] :));
        awaiting = sort_array(keys(filter(applicants, 
              (: $(applicants)[$1][APP_TYPE] == AWAITING :))),
          (: $(applicants)[$1][APP_TIME] - $(applicants)[$2][APP_TIME] :));
        if ( sizeof(awaiting))
        {
            results += "Applicants awaiting vacancies:\n";
            foreach (word in awaiting)
            results += "     "+ cap_name(word)+
            " - confirmed employment " +
            ctime(applicants[word][1])+ "\n";
            results += "\n";
        }
        if (sizeof(hired))
        {
            results += "Applicants yet to confirm their employment:\n";
            foreach (word in hired)
            results += "     "+ cap_name(word)+ " - voted in " +
            ctime(applicants[word][1])+ "\n";
            results += "\n";
        }
        if (sizeof(applied))
        {
            results += "Applicants yet to be hired or rejected.  Vote now!\n";
            foreach (word in applied)
            {
                results += "     "+ cap_name(word)+ " - applied " +
                ctime(applicants[word][1])+ "\n";
                results += sprintf("     %-12s%-5d%-16s%-5d%-13s%-5d\n",
                  "Votes for:", sizeof(applicants[word][APP_FOR]),
                  "Votes against:", sizeof(applicants[word][APP_AGAINST]),
                  "Abstentions:", sizeof(applicants[word][APP_ABSTAIN]));
            }
        }
        tell_object(this_player(), "$P$Applications list$P$"+ results);
        break;
    case "baddies" :
        if (!m_sizeof(baddies = _office->get_baddies()))
        {
            tell_object(this_player(),
              "There are no people on the list at the moment.\n");
            return 1;
        }
        results = "This is a list of people banned from the shop:\n\n";
        foreach (word in sort_array(keys(baddies), 1))
        {
            results += cap_name(word)+ " banned by "+ 
            baddies[word][BAD_BANNER]+ " on "+
            ctime(baddies[word][BAD_TIME] )+ ".\n   Banned for "+
            baddies[word][BAD_REASON]+ ".\n\n";
        }
        tell_object(this_player(), "$P$Baddies$P$\n"+ results);
        break;
    default :
        return _office->do_list();
    }
    return 1;
}
/* do_list() */


/** @ignore yes */
private int do_logs(mixed *args, string pattern)
{
    return _office->do_logs(args, pattern);
}
/* do_logs() */


/** @ignore yes */
private int do_mail(string words) { return MAIL_TRACK->mail(words); }


/** @ignore yes */
private int do_memo()
{
    tell_object(this_player(), "Subject: (hit enter for general memo)\n");
    input_to((: write_memo($1) :), 0);
    add_succeeded_mess("");
    return 1;
}
/* do_memo() */


/** @ignore yes */
private int do_office()
{
    int retired = _office->query_retired(this_player()->query_name());
    string result = (retired)?
    "As a retired manager, you can use the following commands:\n":
    "As a manager, you can use the following commands:\n";

    result +=
    "   accounts  - view register, bonus & profit accounts\n"
    "   claim     - claim a manager's handbook\n"
    "   list      - list employees, applicants or banned people\n"
    "   logs      - review the shop's logs\n"
    "   mail      - read / send mail\n"
    "   memo      - send a message to the other managers\n"
    "   policy    - suggest a new (or remove an old) policy for the shop\n"
    "   policies  - check on the proposed policies\n"
    "   project   - view anticipated paypacket & bonus values\n"
    "   query     - query pay amount or number of employees\n"
    "   view      - view a person's history with this shop\n"
    "               or an applicant's application form\n"
    "   vote      - vote for a policy to be implemented into the shop\n";
    if (!retired)
        result +=
        "               or for an applicant to work for the shop\n"
        "   (un)ban   - (un)ban a person from the shop for "+
        BAN_LENGTH+ " days\n"
        "   rent      - rent storeroom cabinets\n"
        "   remove    - remove storeroom cabinets\n"
        "   check     - check votes you have made on applications\n"
        "   commend   - reward an outstanding employee\n"
        "   demote    - demote an employee\n"
        "   fire      - end someone's employment\n"
        "   leave     - place an employee on leave for up to "+
        MAX_LEAVE+ " days\n"
        "   retire    - end your active employment at the shop\n"
        "   set       - set number of employees, and pay amount\n"
        "   suspend   - suspend an employee's bonus entitlement\n"
        "   transfer  - transfer money between register, bonus & profit\n"
        "   warn      - issue a formal warning to an employee\n";
    tell_object(this_player(), result+ "\n");
    add_succeeded_mess("");
    return 1;
}
/* do_office() */


/** @ignore yes */
private int do_policies()
{
    mapping policies = _office->get_new_policies();
    if (!m_sizeof(policies))
        tell_object(this_player(), "There are no policies awaiting sanction.\n");
    else
    {
        int i = 1;

        tell_object(this_player(), "The following policies have been suggested."
          "  Policies in existence are proposed to be overturned.\n");
        foreach(string policy in m_indices(policies))
        tell_object(this_player(), sprintf("%d. %s - %s [%s]. %s\n", i++,
            (policies[policy][POLICY_TYPE])?"MGR":"EMP", policy,
            policies[policy][POLICY_MGR], policies[policy][POLICY_TEXT]));
    }
    return 1;
}
/* do_policies() */


/** @ignore yes */
private int do_policy(mixed *args, string pattern)
{
    string *managers, mgr;

    if (pattern == POLICY_SUGGEST)
    {
        if (_office->query_policy(args[1]))
        {
            tell_object(this_player(), "This name is already in use.\n");
            return 1;
        }
        this_player()->do_edit(0, (: end_policy_suggest($1,
              $(lower_case(args[1])),$((args[0] == "manager")?1:0)) :));
        add_succeeded_mess("");
        return 1;
    }
    if (_office->query_policy(args[0]) < 2)
    {
        tell_object(this_player(), "This policy does not exist.\n");
        return 1;
    }
    args[0] = lower_case(args[0]);
    tell_object(this_player(), "You suggest that the "+ args[0]+
      " policy should be removed.\n");
    mgr = this_player()->query_name();
    managers = _office->get_managers();
    managers += _office->get_retired();
    managers += ({_office->query_creator()});
    managers -= ({mgr});
    AUTO_MAILER->auto_mail(implode(managers, ","),
      mgr, "TFQC policy suggestion",
      "", "Please note that I would like you to vote on removing the "+
      args[0]+ " policy.\n", 0, 0);
    _office->add_policy_suggest(args[0], "", mgr);
    return 1;
}   
/* do_policy() */


/** @ignore yes */
private int do_project(string projection)
{
    add_succeeded_mess("");
    switch (projection)
    {
    case "bonus" :
        _office->calc_bonus();
        break;
    case "pay" :
        tell_object(this_player(),
          "For the month to date, the staff wages total "+
          MONEY_HAND->money_value_string( _office->calc_pay(), 
            _place)+ ".\n");
        break;
    }
    return 1;
}
/* do_project() */


/** @ignore yes */
private int do_query(string query)
{
    add_succeeded_mess("");
    switch(query)
    {
    case "pay" :
        tell_object(this_player(), "The pay is currently set at "+
          MONEY_HAND->money_value_string(_office->query_pay(),
            _place)+ ".\n");
        break;
    case "employees" :
        tell_object(this_player(),
          "The maximum number of employees is currently "+ 
          _office->query_maxemp()+ ".\n");
        break;
    default :
    }
    return 1;
}
/* do_query() */


/** @ignore yes */
private int do_remove() { return _office->do_remove(); }


/** @ignore yes */
private int do_rent() { return _office->do_rent(); }


/** @ignore yes */
private int do_retire() { return _office->do_retire(); }


/** @ignore yes */
private int do_set(mixed *args, string pattern)
{ 
    return _office->do_set(args, pattern);
}
/* do_set() */


/** @ignore yes */
private int do_suspend(mixed *args) { return _office->do_suspend(args); }


/** @ignore yes */
private int do_transfer(mixed *args) { return _office->do_transfer(args); }


/** @ignore yes */
private int do_unban( string person ) { return _office->do_unban(person); }


/** @ignore yes */
private int do_view(string person, string pattern)
{
    _office->view_record(person, pattern);
    return 1;
}
/* do_view() */


/** @ignore yes */
private int do_vote(mixed *args, string pattern)
{
    return _office->do_vote(args, pattern);
}
/* do_vote() */


/** @ignore yes */
private int do_warn(mixed *args) { return _office->do_warn(args); }


/** @ignore yes */
private void end_memo(string text, string subject)
{
    if (!text)
    {
        tell_object(this_player(), "Aborted.\n");
        return;
    }
    tell_object(this_player(), 
      "Do you want to keep a copy of the memo? ");
    input_to((: send_memo($1,$(subject),$(text)) :), 0);
}
/* end_memo() */


/** @ignore yes */
private void end_policy_suggest(string text, string name, int type)
{
    string mgr, *managers;

    if (!text)
    {
        tell_object(this_player(), "Aborted.\n");
        return;
    }
    /* Remove newlines */
    text = replace_string(text, "\n", " ");
    text = replace_string(text, "  ", " ");
    mgr = this_player()->query_name();
    managers = _office->get_managers();
    managers += _office->get_retired();
    managers += ({_office->query_creator()});
    managers -= ({lower_case(mgr)});
    AUTO_MAILER->auto_mail(implode(managers, ","),
      mgr, _office->shop_very_short()+ " policy suggestion",
      "", "Please note that there is a new policy, "+ name+ 
      ", on which I would like you to vote.\n", 0, 0);
    _office->add_policy_suggest(name, text, mgr, type);
}
/* end_policy_suggest() */


/** @ignore yes */
void event_death(object k, object *o, object k2, string r, string k3)
{
    _office->event_death(k, o, k2, r, k3);
}
/* event_death() */


/** @ignore yes */
void event_enter(object ob, string message, object from)
{
    _office->event_enter(ob, message, from);
}
/* event_enter() */


/** @ignore yes */
private void send_memo(string ans, string subject, string text)
{
    string *managers;

    ans = lower_case(ans);
    if (strlen(ans) < 1 || (ans[0] != 'y' && ans[0] != 'n'))
    {
        tell_object(this_player(),
          "Do you want to keep a copy of the memo? (Yes or No)? ");
        input_to((: send_memo($1,$(subject),$(text)) :), 0);
        return;
    }
    managers = _office->get_managers();
    managers += _office->get_retired();
    if ( this_player()->query_creator() &&
      this_player()->query_name() != _office->query_creator())
        managers += ({lower_case(this_player()->query_name())});
    managers += ({_office->query_creator()});
    if ( ans[0] == 'n' ) 
        managers -= ({lower_case(this_player()->query_name())});
    if (!sizeof(managers))
    {
        tell_object(this_player(), "\nNo recipients!\n");
        return;
    }
    tell_object(this_player(), "\nSending your memo.\n");
    subject = (subject && subject != "")?" - "+ subject:"";
    AUTO_MAILER->auto_mail(implode(managers, ","),
      this_player()->query_name(), _office->shop_very_short()+
      " manager memo" + subject, "", text, 0, 0);
}
/* send_memo() */


/** @ignore yes */
protected void set_long(string long_desc)
{
    long_desc += "Managers can use their own \"office\" commands "
    "from in here.  There is a management policy notice on the wall.\n";
    ::set_long( long_desc );
}
/* set_long() */

/**
 * Set the path of the main office.
 * @example set_office( PATH+ "office" );
 * @param path The full path & filename.
 */
protected void set_office(string path)
{
    _office = path;
    _place = _office->query_place();
    _shop_front = _office->query_shop_front();
    _counter = _office->query_counter();
    _storeroom = _office->query_storeroom();

    /******************************
     * Add the shop's policy notice
     *****************************/
    _notice = clone_object(SHOP_NOTICE);
    _notice->set_office(_office);
    _notice->set_type(1);
    add_hidden_object(_notice);
}
/* set_office() */

/** @ignore yes */
private void write_memo(string text)
{
    this_player()->do_edit(0, (: end_memo($1,$(text)) :));
}
/* write_memo() */
