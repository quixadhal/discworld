/* handles the indeirection needed for living objects...
 */
#include "money.h"

varargs int adjust_money(mixed i, string type) {
  object ob, p_o;
  int val;

  p_o = previous_object();
  ob = present("Some Money For Me", this_object());
  if (!ob) {
    ob = clone_object(MONEY_OBJECT);
    ob->move(this_object());
  }
  val = (int)ob->query_value();
  ob->adjust_money(i, type);
  val = (int)ob->query_value()-val;
/*
  if (interactive(this_object()))
    add_worth(val, (string)"/secure/master"->creator_file(file_name(p_o)));
*/
}

mixed *query_money_array() {
  object ob;

  ob = present("Some Money For Me", this_object());
  if (!ob) {
    ob = clone_object(MONEY_OBJECT);
    ob->move(this_object());
  }
  return (mixed *)ob->query_money_array();
}

void set_money_array(mixed *arr) {
  object ob;

  ob = present("Some Money For Me", this_object());
  if (!ob) {
    ob = clone_object(MONEY_OBJECT);
    ob->move(this_object());
  }
  ob->set_money_array(arr);
}

int query_value() {
  object ob;

  ob = present("Some Money For Me", this_object());
  if (!ob) {
    ob = clone_object(MONEY_OBJECT);
    ob->move(this_object());
  }
  return (int)ob->query_value();
}

int query_money(string type) {
  object ob;

  ob = present("Some Money For Me", this_object());
  if (!ob) {
    ob = clone_object(MONEY_OBJECT);
    ob->move(this_object());
  }

  return (int)ob->query_money(type);
}

int pay_money(mixed *m_array) {
  int i, j, val;
  mixed *m_a;
  object ob, p_o;

  p_o = previous_object();
  ob = present("Some Money For Me", this_object());
  if (!ob) {
    ob = clone_object(MONEY_OBJECT);
    ob->move(this_object());
  }
  val = (int)ob->query_value();
  for (i=0;i<sizeof(m_array);i+=2) {
    m_a = (mixed *)MONEY_HAND->make_payment(m_array[i], m_array[i+1],
                                            this_object());
    for (j=0;j<sizeof(m_a[0]);j+=2)
      ob->adjust_money(-m_a[0][j+1], m_a[0][j]);
    for (j=0;j<sizeof(m_a[1]);j+=2)
      ob->adjust_money(m_a[1][j+1], m_a[1][j]);
  }
  val = (int)ob->query_value()-val;
/*
  if (interactive(this_object()))
    add_worth(val, (string)"/secure/master"->creator_file(file_name(p_o)));
*/
}
