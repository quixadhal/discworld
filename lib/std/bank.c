#include "money.h"
inherit "/std/room";

mapping accounts;
string save_file;
int total_made;
static int total_account;

int account_cost,
    percentage;

void load_it();
void save_it();

void create() {
  int i;
  sscanf(file_name(this_object()), "%s#%d", save_file, i);
  accounts = ([ ]);
  account_cost = 0;
  percentage = 90;
  ::create();
  load_it();
} /* create() */

void set_save_file(string str) {
  save_file = str;
  load_it();
} /* set_save_file9) */

string query_save_file() { return save_file; }

void init() {
  add_action("balance", "balance");
  add_action("withdraw", "withdraw");
  add_action("create_account", "open");
  add_action("close_account", "close");
  add_action("deposit", "deposit");
  ::init();
} /* init() */

int get_account() {
  string name;
  int val;

  if (!undefinedp(this_player()->query_property("bank "+save_file)))
    return (int)this_player()->query_property("bank "+save_file);
  name = (string)this_player()->query_name();
  if (undefinedp(accounts[name]))
    return -1;
  return accounts[name];
} /* get_account() */

int set_account(int amt) {
  string name;

  name = (string)this_player()->query_name();
  map_delete(accounts, name);
  this_player()->add_property("bank "+save_file, amt);
} /* set_account() */

int balance() {
  int amt;

  amt = get_account();
  if (amt < 0) {
    notify_fail("You do not have an account here.\n");
    return 0;
  }
  if (!amt) {
    write("Your account is empty.\n");
    return 1;
  }
  write("You have "+MONEY_HAND->money_value_string(amt)+" in your account.\n");
  return 1;
} /* balance() */

int withdraw(string str) {
  int amt, i, val, total;
  string s1, type;
  mixed *values;

  total = get_account();
  if (total < 0) {
    notify_fail("You do not have an account here.\n");
    return 0;
  }
  notify_fail("Syntax: "+query_verb()+" <amount> <type> coin[s]\n");
  if (!str)
    return 0;
  if (sscanf(str, "%d %s coin%s", amt, type, s1) != 3 &&
      sscanf(str, "%d %s", amt, type) != 2)
    return 0;
  if (amt <= 0) {
    notify_fail("You must withdraw something.\n");
    return 0;
  }
  values = (mixed *)MONEY_HAND->query_values();
  if ((i=member_array(type, values)) == -1) {
    notify_fail("I do not know of any "+type+" coins.\n");
    return 0;
  }
  val = amt*values[i+1];
  if (val > total) {
    write("You do not have enough money in your account.\n");
    return 1;
  }
  this_player()->adjust_money(amt, type);
  set_account(total - val);
  total_account -= val;
  save_it();
  write("You withdraw "+amt+" "+type+" coins.\n");
  say(this_player()->query_cap_name()+" withdraws some money from "+
      this_player()->query_possessive()+" account.\n");
  return 1;
} /* withdraw() */

int deposit(string str) {
  object *obs, cont;
  int i, total_amt, amt, total;

  total = get_account();
  if (total < 0) {
    notify_fail("You do not have an account here.\n");
    return 0;
  }
  if (!str) {
    notify_fail("Syntax: "+query_verb()+" <money>\n");
    return 0;
  }
  cont = clone_object("/std/container");
  obs = find_match(str, this_player());
  for (i=0;i<sizeof(obs);i++)
    if (obs[i]->query_property("money"))
      obs[i]->move(cont);
  if (!sizeof(all_inventory(cont))) {
    cont->dest_me();
    notify_fail("You might want to deposit some money.\n");
    return 0;
  }
  obs = all_inventory(cont);
  for (i=0;i<sizeof(obs);i++)
    total_amt += (int)obs[i]->query_value();
  if (!total_amt) {
    notify_fail("Your money is not worth anything.\n");
    return 0;
  }
  amt = total_amt*percentage/100;
  set_account(total+amt);
  total_account += amt;
  total_made += total_amt - amt;
  save_it();
  write(capitalize((string)MONEY_HAND->money_value_string(total_amt))+
        " deposited to give a total account of "+
        MONEY_HAND->money_value_string(total+amt)+
        ".\n");
  say(this_player()->query_cap_name()+" deposits some money into "+
      this_player()->query_possessive()+" account.\n");
  return 1;
} /* deposit() */

int create_account() {
  int amt;

  amt = get_account();
  if (amt >= 0) {
    notify_fail("You already have an account here.\n");
    return 0;
  }
  if (account_cost) {
    if (this_player()->query_value() < account_cost) {
      write("You do not have enough money to open an account.\n");
      return 1;
    }
    write("It will cost you "+MONEY_HAND->money_value_string(account_cost)+
          " to open the account.\nDo you still want to ? ");
    input_to("check_open");
    return 1;
  }
  set_account(0);
  write("Account created.\n");
  save_it();
  return 1;
} /* create_account() */

int check_open(string str) {
  str = lower_case(str);
  if (str[0] != 'y' && str[0] != 'n') {
    write("I don't understand.  Do you want to open an account ? ");
    input_to("check_open");
    return 1;
  }
  if (str[0] == 'n') {
    write("Ok, not opening the account.\n");
    return 1;
  }
  set_account(0);
  write("Account created.\n");
  save_it();
  total_made += account_cost;
  this_player()->pay_money((mixed *)MONEY_HAND->create_money_array(account_cost));
  return 1;
} /* check_open() */

int close_account() {
  int amt, total;
  object ob;

  total = get_account();
  if (total < 0) {
    notify_fail("You do not have an account here, so how can you close it?\n");
    return 0;
  }
  if ((amt = total)) {
    write("You get "+MONEY_HAND->money_value_string(amt)+" when you close "+
          "the account.\n");
    this_player()->adjust_money(MONEY_HAND->create_money_array(amt));
    total_account -= amt;
  } else {
    write("You close your account.\n");
  }
  say(this_player()->query_cap_name()+" closes " +
    this_player()->query_possessive() + " account.\n");
  set_account(0);
  this_player()->remove_property("bank "+save_file);
  save_it();
  return 1;
} /* close_account() */

void load_it() {
  if (save_file) {
    restore_object(save_file);
    if (!total_account) {
      string *str;
      int i;
      str = m_indices(accounts);
      for (i=0;i<sizeof(str);i++)
        total_account += accounts[str[i]];
    }
  }
} /* load_it() */

void save_it() {
  if (save_file)
    save_object(save_file);
} /* save_it() */

void set_percentage(int per) { percentage = per; }
int query_percentage() { return percentage; }

void set_account_cost(int i) { account_cost = i; }
int query_account_cost() { return account_cost; }

mixed *stats() {
  return ({ ({ "Percentage", percentage }),
            ({ "Account cost", account_cost }),
            ({ "Total made", total_made }),
            ({ "Total account", total_account }) });
} /* stats() */

/* this adds fun and excitement to the game, as well as
 * reducing LARGE accounts.
 */ 
int make_bank_robbers()
{
  object *robbers, *money, *bags, *players;
  int amount, i, ppl, loss, per, acc;
  mixed coins;
  string pname;

  per = random(4) + 2;
  
  players = users();
  for(i=0; i<sizeof(players); i++) {
    pname = (string)players[i]->query_name();
    if(acc = accounts[pname]) {
      switch(acc) {
        case 1..10: loss = 0; break;
        case 11..100: loss = acc * per / 100; break;
        case 101..1000: loss = acc * (per*2) / 100; break;
        case 1001..10000: loss = acc * (per*3) / 100; break;
        case 10001..100000: loss = acc * (per*4) / 100; break;
        default: loss = acc * (per*5) / 100;
      }
      accounts[pname] -= loss;
      amount += loss;
    }
  }
  if(!amount) return 0;

  ppl = amount / 10000;
  if(!ppl) ppl = 1;
  if(ppl > 5) ppl = 5;

  total_account -= amount;
  amount /= 3; /* well you have to _lose_ something */
  total_made += amount * 2; /* well ok, it isn't actually lost *8-)* */

  save_it();
  coins = (mixed)MONEY_HAND->create_money_array(amount / ppl);
  
  robbers = allocate(ppl);
  money = allocate(ppl);
  bags = allocate(ppl);
  
  for(i=0; i<ppl; i++) {
    robbers[i] = clone_object("/obj/monster");
    robbers[i]->set_name("robber");
    robbers[i]->add_adjective("bank");
    robbers[i]->set_short("Bank Robber");
    robbers[i]->set_main_plural("bank robbers");
    robbers[i]->add_plural("robbers");
    robbers[i]->set_long("The bank robber is wearing a stripey jersey.\n");
    robbers[i]->set_race("human");
    robbers[i]->set_guild("thief");
    robbers[i]->set_level(random(10)+20);

    bags[i] = clone_object("/std/container");
    bags[i]->set_name("bag");
    bags[i]->add_adjective("money");
    bags[i]->set_short("Money bag");
    bags[i]->set_main_plural("Money bags");
    bags[i]->add_plural("bags");
    bags[i]->set_long("A roughly sewn sacking bag with a big '$' sign on " +
      "it.\n");
    bags[i]->set_weight(10);
    
    money[i] = clone_object("/obj/money");
    money[i]->adjust_money(coins);

    money[i]->move(bags[i]);
    bags[i]->move(robbers[i]);
    robbers[i]->move(this_object());
  }
  return amount;
}
