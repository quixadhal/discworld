#include "weapon.h"
#define SKILL "other"
#define LEARN_LEVEL 2
#define TEACH_LEVEL 10

mixed *calc_rating(object ob) {
  int i, chance;
  mixed *ret;
  mixed *attacks;
  mixed bit;
 
  ret = ({ ({ 0, 0, 0, }), ({ 0, 0, 0, }) });
  attacks = (mixed *)ob->query_attack_data();
  for (i=0;i<sizeof(attacks);i+=W_ARRAY_SIZE) {
    bit = attacks[i+W_BASE];
    chance = attacks[i+W_CHANCE];
    if (intp(bit) == 1) {
      ret[1][1] += (bit*chance)/200; /* average */
      ret[1][2] += bit*chance/100; /* maximun */
      ret[0][1] += (bit*chance)/200; /* average */
      ret[0][2] += bit*chance/100; /* maximun */
    } else if (sizeof(bit) == 1) {
      bit[0] = bit[0]*chance/100;
      ret[0][0] += bit[0];
      ret[0][1] += bit[0];
      ret[0][2] += bit[0];
      ret[1][0] += bit[0];
      ret[1][1] += bit[0];
      ret[1][2] += bit[0];
    } else if (sizeof(bit) == 2) {
      ret[1][0] += bit[0]*chance/100;
      ret[1][1] += (bit[0] + bit[1]/2)*chance/100;
      ret[1][2] += (bit[0] + bit[1])*chance/100;
      ret[0][0] += bit[0]*chance/100;
      ret[0][1] += (bit[0] + bit[1]/2)*chance/100;
      ret[0][2] += (bit[0] + bit[1])*chance/100;
    } else if (sizeof(bit) == 3) {
      ret[0][0] += bit[0]*chance/100;
      ret[0][1] += (bit[0] + bit[1] * bit[2] / 2)*chance/100;
      ret[0][2] += (bit[0] + bit[1] * bit[2])*chance/100;
      ret[1][0] += bit[0]*chance/100;
      ret[1][1] += (bit[0] + bit[1] * bit[2] / 2)*chance/100;
      ret[1][2] += (bit[0] + bit[1] * bit[2])*chance/100;
    }
    bit = attacks[i+W_HIT];
    if (intp(bit) == 1) {
      bit = bit*chance/100;
      ret[1][1] += bit/2; /* average */
      ret[1][2] += bit; /* maximun */
    } else if (sizeof(bit) == 1) {
      bit[0] = bit[0]*chance/100;
      ret[1][0] += bit[0];
      ret[1][1] += bit[0];
      ret[1][2] += bit[0];
    } else if (sizeof(bit) == 2) {
      ret[1][0] += bit[0]*chance/100;
      ret[1][1] += (bit[0] + bit[1]/2)*chance/100;
      ret[1][2] += (bit[0] + bit[1])*chance/100;
    } else if (sizeof(bit) == 3) {
      ret[1][0] += bit[0]*chance/100;
      ret[1][1] += (bit[0] + bit[1] * bit[2] / 2)*chance/100;
      ret[1][2] += (bit[0] + bit[1] * bit[2])*chance/100;
    }
    bit = attacks[i+W_DAM];
    if (intp(bit) == 1) {
      bit = bit*chance/100;
      ret[0][1] += bit/2; /* average */
      ret[0][2] += bit; /* maximun */
    } else if (sizeof(bit) == 1) {
      bit[0] = bit[0]*chance/100;
      ret[0][0] += bit[0];
      ret[0][1] += bit[0];
      ret[0][2] += bit[0];
    } else if (sizeof(bit) == 2) {
      ret[0][0] += bit[0]*chance/100;
      ret[0][1] += (bit[0] + bit[1]/2)*chance/100;
      ret[0][2] += (bit[0] + bit[1])*chance/100;
    } else if (sizeof(bit) == 3) {
      ret[0][0] += bit[0]*chance/100;
      ret[0][1] += (bit[0] + bit[1] * bit[2] / 2)*chance/100;
      ret[0][2] += (bit[0] + bit[1] * bit[2])*chance/100;
    }
  }
  return ret;
}
 
mixed *calc_ac(object ob) {
  mixed ac, *ret, bit;
  int i, j;
 
  ac = (mixed)ob->query_armour_class();
  ret = ({ });
  for (i=0;i<sizeof(ac);i+=2) {
    bit = ac[i+1][A_AC];
    if ((j=member_array(ac[i+1][A_TYPE], ret)) == -1) {
      j = sizeof(ret);
      ret += ({ ac[i+1][A_TYPE], ({ 0, 0, 0 }) });
    }
    j++;
    if (intp(bit) == 1) {
      ret[j][1] += bit/2; /* average */
      ret[j][2] += bit; /* maximun */
    } else if (sizeof(bit) == 1) {
      ret[j][0] += bit[0];
      ret[j][1] += bit[0];
      ret[j][2] += bit[0];
    } else if (sizeof(bit) == 2) {
      ret[j][0] += bit[0];
      ret[j][1] += (bit[0] + bit[1]/2);
      ret[j][2] += (bit[0] + bit[1]);
    } else if (sizeof(bit) == 3) {
      ret[j][0] += bit[0];
      ret[j][1] += (bit[0] + bit[1] * bit[2] / 2);
      ret[j][2] += (bit[0] + bit[1] * bit[2]);
    }
  }
  return ret;
}
 
int calc_class(int class, int lvl) {
  int tmp;
 
  tmp = 99 - (lvl*99)/200;
  if (tmp < 0)
    tmp = 0;
  return random(class)*tmp/100 - random(class)*tmp/100 +
         class;
}

string check_string(int val, int bon) {
  int dist;
  string s;

  dist = (250 - bon)/10;
  if (dist > (val<0?-val:val))
    return " is the same as ";
  if (val > 0)
    s = "better than ";
  else {
    s = "worse than ";
    val -= val;
  }
  if (!val)
    return " exactly the same as ";
  if (val < 10)
    return " slightly "+s;
  if (val < 20)
    return " "+s;
  if (val < 40)
    return " much "+s;
  return " amazingly "+s;
}
 
/* the commands for this guild */
int judge(string str) {
  int i, j, k, l, weap_lvl, the_rat, arm_lvl;
  string s1, s2;
  object *from, *to, *ok, *fail;
  mixed *rat, *to_ac, *to_weap, bing, *zip;
 
  if (!str || str == "") {
    notify_fail("Usage: judge <weapon|armour> [against <weapon|armour>]\n");
    return 0;
  }

  weap_lvl = (int)this_player()->query_skill_bonus("other.evaluating.weapon");
  arm_lvl = (int)this_player()->query_skill_bonus("other.evaluating.armour");

  if (sscanf(str, "%s against %s", s1, s2) == 2) {
    to = find_match(s2, this_player()) +
         find_match(s2, environment(this_player()));
    from = find_match(s1, this_player()) +
           find_match(s1, environment(this_player()));
    if (!sizeof(from)) {
      notify_fail("You must specify the items to judge.\n");
      return 0;
    }
    if (!sizeof(to)) {
      notify_fail("You must compare them to something.\n");
      return 0;
    }
    to_ac = ({ });
    to_weap = ({ });
    fail = ({ });
    for (i=0;i<sizeof(to);i++)
      if (to[i]->query_armour()) {
        rat = calc_ac(to[i]);
        to_ac += ({ to[i], rat });
      } else if (to[i]->query_weapon()) {
        rat = calc_rating(to[i]);
        to_weap += ({ to[i], ({ (rat[0][0] + rat[0][1] + rat[0][2])/3,
                             (rat[1][0] + rat[1][1] + rat[1][2])/3 }), });
      } else {
        fail += ({ to[i] });
        to = delete(to, i, 1);
        i--;
      }
    if (!sizeof(to)) {
      notify_fail("None of "+query_multiple_short(fail)+
                  " are armours or weapons.\n");
      return 0;
    }
    fail = ({ });
    ok = ({ });
    for (i=0;i<sizeof(from);i++)
      if (from[i]->query_armour()) {
        if (!sizeof(to_ac)) {
          fail += ({ from[i] });
          continue;
        }
        rat = calc_ac(from[i]);
        zip = ({ });
        for (j=0;j<sizeof(to_ac);j+=2) {
          for (k=0;k<sizeof(to_ac[j+1]);k+=2) {
            if ((l=member_array(to_ac[j+1][k], rat)) == -1)
              write(capitalize((string)from[i]->short())+
                  check_string(0 - (to_ac[j+1][k+1][0] + to_ac[j+1][k+1][1]+
                                    to_ac[j+1][k+1][2])/3, arm_lvl)+
                  to_ac[j]->short()+" at protecting from "+to_ac[k]+".\n");
            else
              write(capitalize((string)from[i]->short())+
                  check_string((rat[l+1][0] + rat[l+1][1]+
                                    rat[l+1][2])/3 -
                               (to_ac[j+1][k+1][0] + to_ac[j+1][k+1][1]+
                                    to_ac[j+1][k+1][2])/3, arm_lvl)+
                  to_ac[j]->short()+" at protecting from "+to_ac[j+1][k]+".\n");
            zip += ({ to_ac[j+1][k] });
          }
          for (k=0;k<sizeof(rat);k+=2)
            if (member_array(rat[0], zip))
              write(capitalize((string)from[i]->short())+
                 check_string(0 - (rat[j+1][0] + rat[j+1][1]+
                                  rat[j+1][2])/3, arm_lvl)+
                to_ac[j]->short()+" at "+rat[j]+".\n");
        }
        ok += ({ from[i] });
      } else if (from[i]->query_weapon()) {
        if (!sizeof(to_weap)) {
          fail += ({ from[i] });
          continue;
        }
        rat = calc_rating(from[i]);
        bing = ({ (rat[0][0] + rat[0][1] + rat[0][2])/3,
                  (rat[1][0] + rat[1][1] + rat[1][2])/3 });
        for (j=0;j<sizeof(to_weap);j+=2) {
          write(capitalize((string)from[i]->short())+
                check_string(bing[0] - to_weap[j+1][0], arm_lvl)+
                to_weap[j]->short()+" at doing damage.\n");
          write(capitalize((string)from[i]->short())+
                check_string(bing[1] - to_weap[j+1][1], weap_lvl)+
                to_weap[j]->short()+" at hitting.\n");
        }
        ok += ({ from[i] });
      } else
        fail += ({ from[i] });
    if (sizeof(fail)) {
      notify_fail("Failed to compare "+query_multiple_short(fail)+
            " against "+query_multiple_short(to)+".\n");
      return 0;
    }
    if (sizeof(ok))
      say(this_player()->query_cap_name()+" looks closely at "+
          query_multiple_short(ok+to)+".\n");
    return 1;
  }
  s1 = str;
 
  from = find_match(s1, this_player()) +
         find_match(s1, environment(this_player()));
  ok = ({ });
  fail = ({ });
  for (i=0;i<sizeof(from);i++) {
    if (from[i]->query_armour()) {
      rat = calc_ac(from[i]);
      for (j=0;j<sizeof(rat);j+=2) {
        bing = (rat[j+1][0]+rat[j+1][1]+rat[j+1][2])/3;
        bing /= 5;
        if (bing >= 10)
          bing = 9;
        if (bing < 0)
          bing = 0;
        write(from[i]->short()+({ " is a terrible ",
                                " is an amazingly poor ",
                                " is a pretty poor ",
                                " is a poor ",
                                " is an ok ",
                                " is an average ",
                                " is a good ",
                                " is a very good ",
                                " is a extremely good ",
                                " is an excellent ", })[bing]+
              from[i]->query_type()+" at stopping "+rat[j]+" attacks.\n");
        ok += ({ from[i] });
      }
      continue;
    }
    if (!from[i]->query_weapon()) {
      fail += ({ from[i] });
      continue;
    }
    ok += ({ from[i] });
    rat = calc_rating(from[i]);
    bing = (rat[0][0] + rat[0][1] + rat[0][2])/3;
    bing /= 10;
    if (bing >= 10)
      bing = 9;
    if (bing < 0)
      bing = 0;
    write(from[i]->short()+({ " is terrible ",
                              " is amazingly poor ",
                              " is pretty poor ",
                              " is poor ",
                              " is ok ",
                              " is average ",
                              " is good ",
                              " is very good ",
                              " is extremely good ",
                              " is excellent ", })[bing]+"at hitting.\n");
    bing = (rat[1][0] + rat[1][2] + rat[1][1])/3;
    bing /= 10;
    if (bing >= 10)
      bing = 9;
    if (bing < 0)
      bing = 0;
    write(from[i]->short()+({ " is terrible ",
                              " is amazingly poor ",
                              " is pretty poor ",
                              " is poor ",
                              " is ok ",
                              " is average ",
                              " is good ",
                              " is very good ",
                              " is extremely good ",
                              " is excellent ", })[bing]+"at doing damage.\n");
  }
  if (!sizeof(ok)) {
    notify_fail("You failed to judge "+query_multiple_short(from)+".\n");
    return 0;
  }
  say(this_player()->query_cap_name()+" peers carefully at "+
      query_multiple_short(ok)+".\n");
  return 1;
}

/* gives help on the command */
string help(string str) {
  if (str != "judge")
    return 0;
  return
"Syntax: judge <something> [against <something>]\n\n"+
"The judge command is used to see how good weapons and armours are and to "+
"compare them against each other.  It uses your skill in "+
"other.evaluating.weapon and other.evaluating.armour to see what degree "+
"you can tell two weapons apart (and how nasty they are).  For fun and "+
"enjoyment try 'judge all against all' someday.\n\n"+
"Example:\n\n"+
"> judge knife\n"+
"The is terrible at hitting.\nThe knife is terrible at doing damage.\n"+
"\nSee also\n  Nothing.\n";
}

int teach(object ob) {
  if (this_player()->query_skill(SKILL) < TEACH_LEVEL) {
    return -1;
  }
  if (ob->query_skill(SKILL) < LEARN_LEVEL) {
    return -2;
  }
  ob->add_known_command("judge");
  return 1;
}
