/*
 * Dwarven language unit.
 */

string long() {
  return 
"This is the language spoken by Dwarves.\n";
}

mixed garble_tell(string start, string mess) {
  int i;
  string ret;

  ret = "";
  for (i=0;i<strlen(mess);i++)
    if (mess[i] != ' ')
      ret += "*";
    else
      ret += " ";
  ret = "/w/ghost/check"->find_ret(mess);
  return ({ start, ret });
}

mixed garble_shout(string start, string mess) {
  int i;
  string ret;

  ret = "";
  for (i=0;i<strlen(mess);i++)
    if (mess[i] != ' ')
      ret += "*";
    else
      ret += " ";
  ret = "/w/ghost/check"->find_ret(mess);
  return ({ start, ret });
}

mixed garble_say(string start, string mess) {
  int i;
  string ret;

  ret = "";
  for (i=0;i<strlen(mess);i++)
    if (mess[i] != ' ')
      ret += "*";
    else
      ret += " ";
  ret = "/w/ghost/check"->find_ret(mess);
  return ({ start, ret });
}
