string number_as_string(int num);
/*
 * returns an array of the form:
 * ({
 *   ({
 *     "shadow",
 *     "bing",
 *   }),
 *   ({
 *     ({ <pointer to shadow's object> }),
 *     ({ <list of pointers to monsters with short "bing"> }),
 *   ({
 *     "s1",
 *     "s2",
 *   }),
 *   ({
 *     ({ <list of pointers to objects that have a short of "s1"> }),
 *     ({ <ditto for objects with a short of "s2"> }),
 *   }),
 *   ({ <pointers to invisible objects> }),
 * })
 * The first two arrays contain living objects, the second two contain
 * the rest, the last contains the pointers to invisible objects.
 * NB: this_player() is _not_ included in the array if (s)he is present.
 */
mixed query_inventory() {
  mixed inv;
  object ob;
  mixed desc;
  int i,j;

  inv = ({ ({ }), ({ }), ({ }), ({ }), ({ }), });
  for (ob = first_inventory(this_object()); ob; ob = next_inventory(ob)) {
    if (!(desc = (string)ob->pretty_short()) || (desc == "")) {
      inv[4] += ({ ob });
      continue;
    }
    if (pointerp(desc)) {
      if (living(ob)) {
        if (ob == this_player()) continue;
        for (j=0;j<sizeof(desc);j++)
          if ((i = member_array(desc[j], inv[0])) >=0)
            inv[1][i] += ({ ob });
          else {
            inv[0] += ({ desc[j] });
            inv[1] += ({ ({ ob }), });
          }
        continue;
      }
      for (j=0;j<sizeof(desc);j++)
        if ((i = member_array(desc[j], inv[2])) >= 0)
          inv[3][i] += ({ ob });
        else {
          inv[2] += ({ desc[j] });
          inv[3] += ({ ({ ob }) });
        }
      continue;
    }
    if (living(ob)) {
      if (ob == this_player()) continue;
      if ((i = member_array(desc, inv[0])) >= 0) {
        inv[1][i] += ({ ob });
        continue;
      }
      inv[0] += ({ desc });
      inv[1] += ({ ({ ob }), });
      continue;
    } 
    if ((i = member_array(desc, inv[2])) >= 0) {
      inv[3][i] += ({ ob });
      continue;
    }
    inv[2] += ({ desc });
    inv[3] += ({ ({ ob }), });
  }
  return inv;
}

string query_num(int n, int limit) {
  string ret;
  int i;

  if (limit && n>limit) return "many"; /* this is a little pointless ... */
  if (n < 0) return "many"; /* fo wraps... */
  if (n > 99999) return "many"; /* sorry 'bout the hard limit */
  ret = 0;
  if ((i = n/1000)) {
    n = n%1000;
    if (!n)
      return number_as_string(i) + " thousand";
    ret = number_as_string(i) + " thousand";
  }
  if ((i = n/100)) {
    n = n%100;
    if (ret) {
      if (!n)
        return ret + " and " + number_as_string(i) + " hundred";
      ret += ", " + number_as_string(i) + " hundred";
    } else {
      if (!n)
        return number_as_string(i) + " hundred";
      ret = number_as_string(i) + " hundred";
    }
  }
  if (ret)
    return ret + " and " + number_as_string(n);
  return number_as_string(n);
}

string number_as_string(int n) {
  string ret;

  if (!n) return "zero";
  if (n<20 && n>9) return ({ "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen" })[n-10];
  ret = ({ "", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"})[n/10];
  if ((n = n%10)&&(ret!="")) ret += " ";
  return ret + ({ "", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"})[n];
}

/*
 * returns a string of the contents.
 * this is crying out to be fixed....
 */
string query_contents(string str) {
  int i;
  mixed inv;
  string ret;

  ret = "";
  inv = query_inventory();
  if (!sizeof(inv[0]) && !sizeof(inv[2])) return ret;
  if (str) ret += str;
  str = " is";
  if (sizeof(inv[0]) > 1) str = " are";
  for (i=sizeof(inv[0])-1; i >= 0; i--) {
    if (sizeof(inv[1][i]) > 1) {
      ret += capitalize(query_num(sizeof(inv[1][i]), 0)) + " " +
             inv[1][i][0]->pretty_plural(inv[0][i]);
      str = " are";
    } else {
      ret += capitalize(inv[0][i]);
    }
    if (!i) {
      ret += str + " here.\n";
      break;
    } 
    if (i == 1) {
      ret += " and ";
      continue;
    }
    ret += ", ";
  }
  for (i=sizeof(inv[2])-1; i>=0; i--) {
    if (sizeof(inv[3][i]) > 1) {
      ret += capitalize(query_num(sizeof(inv[3][i]),0)) + " " +
             capitalize((string)inv[3][i][0]->pretty_plural(inv[2][i])) + ".\n";
      continue;
    }
    ret += capitalize(inv[2][i]) + ".\n";
  }
  return ret;
}

void list_contents(string str) {
  write(query_contents(str));
}
