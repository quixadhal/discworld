static mixed *rest;

/* If anyone can tell me what this does...
 * I would be most apprecative, Pinkfish... Yes yes ok i did write it.
 */
mixed find_match(string str, mixed ob) {
mixed *array, test, *ret;
int i, num, top, bot, bing, j;
string nick, type, *bits;
mapping rabbit;

  if (!ob || intp(ob))
    return ({ });
  if (str == "" || !str)
    return ({ });
  if (stringp(ob)) {
    ob -> dummy();
    ob = find_object(ob);
  }
  if (!pointerp(ob)) {
    ob = (mixed)ob->find_inv_match(str);
    if (!ob)
      return ({ });
  } else {
    array = ({ });
    for (i=0;i<sizeof(ob);i++)
      if ((test = (object *)ob[i]->find_inv_match(str)))
        array += test;
    ob = array;
  }
  bits = explode(implode(explode(str, " and "), ","), ",");
  ret = ({ });
  for (j=0;j<sizeof(bits);j++) {
    str = bits[j];
    nick = (string)this_object()->expand_nickname(str);
    if (!nick && this_player())
      nick = (string)this_player()->expand_nickname(str);
    if (nick && nick!="")
      str = nick;
    if (str == "it" || str == "them" || str == "him" || str == "her") {
      rest = all_inventory(this_player()) +
             all_inventory(environment(this_player()));
      array = (object *)this_player()->query_it_them();
      while (i<sizeof(array))
        if (member_array(array[i], rest) == -1)
          array = delete(array, i, 1);
        else
          i++;
      if (!sizeof(array))
        continue;
      if (str == "it") {
        ret += array[0..0];
        continue;
      }
      if (str == "her")
        if (living(array[0]) && (int)array[0]->query_gender() == 2) {
          ret += array[0..0];
          continue;
        } else
          continue;
      if (str == "him")
        if (living(array[0]) && (int)array[0]->query_gender() == 1) {
          ret += array[0..0];
          continue;
        } else
          continue;
      ret += array;
      continue;
    }
    test = explode(str, " ");
    sscanf(test[sizeof(test)-1], "%d", bing);
    test = ({ });
    rest = ({ });
    if (sscanf(str, "%d/%d of %s", top, bot, str) != 3)
      top = bot = 1;
    if (top <= 0)
      top = 1;
    if (bot < top)
      bot = top;
    parse_command(str,ob,"%i",test);
    if (!test || !sizeof(test))
      continue;
    if (bot != top) {
      num = test[0];
      rabbit = ([ ]);
      for (i=1;i<sizeof(test);i++)
        rabbit[test[i]] = 1;
      array = query_strange_inventory(keys(rabbit));
      test = ({ });
      for (i=0;i<sizeof(array);i+=2)
        test += map_array(array[i+1], "query_frac_simul_efun_id",
                           this_object(), ({ num, str, 0, sizeof(array[i+1]),
                                             top, bot }) );
      array = test + rest;
      ret += array;
      continue;
    }
    if (test[0] == 1 && bing)
      test[0] = -bing;
    rabbit = ([ ]);
    for (i=1;i<sizeof(test);i++)
      rabbit[test[i]] = 1;
    array = map_array(keys(rabbit), "query_simul_efun_id",
                           this_object(), ({ test[0], str }));
    array += rest;
    ret += array;
  }
  if (this_player() && sizeof(ret))
    return this_player()->set_it_them( ret - ({ 0 }) );
  return ret - ({ 0 });
} /* find_match() */

object query_simul_efun_id(object ob, mixed *arr) { 
  mixed ret;
  ret = (mixed)ob->query_parse_id(arr);
  if (!pointerp(ret))
    return ret;
  rest += ret[1..sizeof(ret)];
  return ret[0];
} /* query_simul_efun_id() */

object query_frac_simul_efun_id(object ob, mixed *arr) {
  mixed ret;
  ret = (mixed)ob->query_frac_parse_id(arr);
  if (!pointerp(ret))
    return ret;
  rest += ret[1..sizeof(ret)];
  return ret[0];
} /* query_frac_efun_id() */

int fixup_parse_command(mixed ob) {
  return objectp(ob);
} /* fixup_parse_command() */
