/* this object keeps track of how much each type of coin is worth
 * So be nice to it ;)
 */
#define DEF_VALUE 1
mixed *values;

void create() {
  values = ({ "brass", 1, "copper", 10,
              "silver", 100, "gold", 2000, "platinum", 6000 });
}

void add_type(string type, int value) {
  int i;
  if (member_array(type, values) != -1)
    return ;
  for (i=0;i<sizeof(values);i++)
    if (value < values[i+1]) {
      values = values[0..i-1]+({ type, value })+values[i..sizeof(values)];
      return ;
    }
  values += ({ type, value });
}

void remove_type(string type) {
  int i;
  if ((i=member_array(type, values)) == -1)
   return ;
  values = delete(values, i, 2);
}

mixed *query_values() { return values; }

int query_value(string type) {
  int i;

  if ((i=member_array(type, values)) == -1)
    return DEF_VALUE;
  return values[i+1];
}

int query_total_value(mixed m_array) {
  int i, amt;

  for (i=0;i<sizeof(m_array);i+=2)
    amt += m_array[i+1]*query_value(m_array[i]);
  return amt;
}

string money_string(mixed m_array) {
  string retval;
  int i, n;

  if (!sizeof(m_array))
    return "0 copper coins";
  retval = "";
  while (i<sizeof(m_array))
    if (!m_array[i+1])
      m_array = delete(m_array, i, 2);
    else
      i += 2;
  for (i=0;i<sizeof(m_array);i+=2) {
    retval += m_array[i+1]+" "+m_array[i];
    if (i == sizeof(m_array)-4)
      retval += " and ";
    else if (i != sizeof(m_array)-2)
      retval += ", ";
  }
  if(sizeof(m_array)>2 || m_array[1]>1)
    return retval+" coins";
  else
    return retval+" coin";
}

/* given a value return a useful money array for it.
 * least coinage one ;)
 */
mixed *create_money_array(int value) {
  int i, amt;
  mixed *m_a;

  if (!value)
    return ({ });
  m_a = ({ });
  for (i=sizeof(values)-2;i>=0;i-=2)
    if (value >= values[i+1]) { /* costs this much at least... */
      m_a += ({ values[i], (amt = value/values[i+1]) });
      value -= amt*values[i+1];
    }
  return m_a;
}

string money_value_string(int value) {
  return money_string(create_money_array(value));
}

/* ok this will calculate what to take off the person for the given
 * cost.  And also what to add to given areas.
 */

mixed *calc_change(int amt, mixed *values) {
  mixed retval;
  int i, num;

  retval = ({ });
  for (i=sizeof(values)-2;i>=0;i-=2)
    if (amt > values[i+1]) {
      num = amt/values[i+1];
      amt = amt%values[i+1];
      retval += ({ values[i], num });
      if (!amt)
        return retval;
    }
  return ({ });
}

mixed make_payment(string type, int amt, object ob) {
  mixed *m_array, retval;
  int i, j, total_amt, no;

  m_array = (mixed *)ob->query_money_array();
  if ((i=member_array(type,m_array)) != -1)
   /* ok the easy way, hopefully */
    if (amt <= m_array[i+1])
      return ({ ({ type, amt }), ({ }) });
  amt = query_value(type)*amt;
  total_amt = query_total_value(m_array);
  if (amt > total_amt)
    return 0;
/* better way....
 * use the calc_change method... BUT only work on his current
 * coins collection.
 */
  retval = ({ });
  for (i=sizeof(values)-2;i>=0;i-=2) {
    if ((j=member_array(values[i], m_array)) != -1)
      if (values[i+1] <= amt) {
        no = amt/values[i+1];
        if (no > m_array[j+1])
          no = m_array[j+1];
        amt -= no*values[i+1];
        retval += ({ values[i], no });
        if (!amt)
          return ({ retval, ({ }) });
      }
  }
/* ok we have gone through his collection... and we still have some
 * left over...
 * start from the bottom work up until we find a coin that is more than
 * we need.... subtract that, calculate_change.
 */
  for (i=0;i<sizeof(values);i+=2)
    if (member_array(values[i], m_array) != -1)
      if (values[i+1] >= amt) {
        if ((j=member_array(values[i], retval)) == -1)
          retval += ({ values[i], 1 });
        else
          retval[j+1] += 1;
        amt = values[i+1] - amt;
        return ({ retval, calc_change(amt, values) });
      }
/* big error, god knows what it is... but it sure is big */
  return 0;
}
