/*
 * If the capitalise variable is set it will cap the first number if
 * it is a number...
 */
varargs string query_multiple_short(mixed *obs, int capitalise) {
  int i, j, k;
  string *str;
  mixed *bity;

  bity = ({ });
  for (i=0;i<sizeof(obs);i++) {
    if (!stringp(obs[i])) {
      if (!pointerp(obs[i])) /* like a string.  First bit is the string.
                              * second the object */
        str = (string *)obs[i]->pretty_short();
      else {
        str = obs[i][0];
        obs[i] = obs[i][1];
      }
    } else
      str = obs[i];
    if (!str) /* ignore invis objects */
      continue;
    if (stringp(str))
      str = ({ str });
    for (j=0;j<sizeof(str);j++)
      if ((k=member_array(str[j], bity)) == -1)
        bity += ({ str[j], ({ obs[i] }) });
      else
        bity[k+1] = ({ obs[i] })+bity[k+1];
  }
  str = ({ });
  for (i=0;i<sizeof(bity);i+=2) {
    j = sizeof(bity[i+1]);
    if (stringp(bity[i+1][0]))
      if (j==1)
        str += ({ bity[i] });
      else if (!i && capitalise)
        str += ({ (capitalize(query_num(j,20))+
                        " "+pluralize(bity[i])) });
      else
        str += ({ (query_num(j,20)+
                        " "+pluralize(bity[i])) });
    else if (j == 1)
      if (bity[i+1][0]->query_property("determinate"))
        if (!i && capitalise)
          str += ({ capitalize((string)bity[i+1][0]->
                        query_property("determinate"))+bity[i] });
        else
          str += ({ bity[i+1][0]->query_property("determinate")+bity[i] });
      else if (!i && capitalise)
        str += ({ capitalize(add_a(bity[i])) });
      else
        str += ({ add_a(bity[i]) });
    else if (!i && capitalise)
      str += ({ (capitalize(query_num(j,20))+
                        " "+bity[i+1][0]->pretty_plural(bity[i])) });
    else
      str += ({ (query_num(j,20)+
                        " "+bity[i+1][0]->pretty_plural(bity[i])) });
  }
  if (!sizeof(str))
    return "";
  if (sizeof(str) == 1)
    return str[0];
  return implode(str[0..sizeof(str)-2],", ")+" and "+str[sizeof(str)-1];
} /* query_multiple_short() */
