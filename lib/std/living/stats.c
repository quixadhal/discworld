/* the stat data for the living objects... */
static int intbon,
       dexbon,
       conbon,
       strbon,
       wisbon;
int Dex,
    Int,
    Con,
    Str,
    Wis,
    inttmp,
    dextmp,
    contmp,
    wistmp,
    strtmp;
static int no_check;

int query_no_check() { return no_check;}
void set_no_check(int i) { no_check = i;}

int check(int i) {
  if (i>28)
    return 0;
  return 1;
} /* check() */

void reset_hp() {
  int skill_bon;

  if (no_check) return ;
  skill_bon = (int)this_object()->query_skill_bonus("fighting.points", 1);
  this_object()->set_max_hp(500+skill_bon*10);
} /* reset_hp() */

void reset_gp() {
  object guild_ob;

  if (no_check) return ;
  guild_ob = (object)this_object()->query_guild_ob();
  if (!guild_ob)
    this_object()->set_max_gp(50+(int)this_object()->query_skill_bonus(
                                  "magic.points"), 1);
  else
    guild_ob->set_gp(this_object());
} /* reset_gp() */

void reset_carry_cap() {
  int old_cap, cap,
      st;
  object ob, ob1;

  if (no_check) return ;
  old_cap = (int)this_object()->query_loc_weight();
  st = Str+strtmp+strbon;

  if (st <=8)
    cap = 500;
  else if (st >=28)
    cap = 3500+(st-28)*300;
  else
    cap = ({ 500, 550, 600, 650, 750, 850, 950, 1150, 1300, 1450, 1600,
             1750, 1900, 2100, 2300, 2500, 2700, 2950, 3200, 3500 })[st-8];
  this_object()->set_max_weight(cap);
  if (cap >= old_cap)
    return ;
  ob = first_inventory(this_object());
  while (ob) {
    ob1 = next_inventory(ob);
    if (ob->query_weight())
      if (ob->move(environment())) {
        old_cap -= (int)ob->query_weight();
        say(this_object()->query_cap_name()+" drops "+ob->short()+
                 " under strain.\n");
        write("Your fading strength makes you drop "+ob->short()+".\n");
      }
    if (cap >= old_cap)
      return ;
  ob = ob1;
  }
} /* reset_carry_cap() */

void reset_all() {
  call_out("zap_stat_cache", 0, "SDIWC");
  call_out("reset_all2", 0);
} /* reset_all() */

void reset_all2() {
/* make sure for reset_all's */
  no_check = 0;
  reset_hp();
  reset_gp();
  reset_carry_cap();
} /* reset_all2() */

int set_str(int i) {
  if (!check(i))
    i = 28;
  if (Str != i)
    call_out("zap_stat_cache", 0, "S");
  Str = i;
  reset_carry_cap();
  return 1;
} /* set_str() */

int set_con(int i) {
  if (!check(i))
    i = 28;
  if (Con != i)
    call_out("zap_stat_cache", 0, "C");
  Con = i;
  call_out("reset_hp", 2);
  return 1;
} /* set_con() */

int set_int(int i) {
  if (!check(i))
    i = 28;
  if (Int != i)
    call_out("zap_stat_cache", 0, "I");
  Int = i;
  call_out("reset_gp", 2);
  return 1;
} /* set_int() */

int set_dex(int i) {
  if (!check(i))
    i = 28;
  if (Dex != i)
    call_out("zap_stat_cache", 0, "D");
  Dex = i;
  return 1;
} /* set_dex() */

int set_wis(int i) {
  if (!check(i))
    i = 28;
  if (Wis != i)
    call_out("zap_stat_cache", 0, "W");
  Wis = i;
  call_out("reset_gp", 2);
  return 1;
} /* set_wis() */

int query_str() { return Str+strtmp+strbon; }
int query_int() { return Int+inttmp+intbon; }
int query_dex() { return Dex+dextmp+dexbon; }
int query_wis() { return Wis+wistmp+wisbon; }
int query_con() { return Con+contmp+conbon; }

int query_real_str() { return Str; }
int query_real_int() { return Int; }
int query_real_dex() { return Dex; }
int query_real_wis() { return Wis; }
int query_real_con() { return Con; }

int query_tmp_str() { return strtmp; }
int query_tmp_int() { return inttmp; }
int query_tmp_dex() { return dextmp; }
int query_tmp_wis() { return wistmp; }
int query_tmp_con() { return contmp; }

int query_bonus_str() { return strbon; }
int query_bonus_int() { return intbon; }
int query_bonus_dex() { return dexbon; }
int query_bonus_wis() { return wisbon; }
int query_bonus_con() { return conbon; }

int adjust_dex(int i) {
  if (check(i+Dex)) {
    Dex += i;
    if (i)
      call_out("zap_stat_cache", 0, "D");
  }
  return Dex;
} /* adjust_dex() */

int adjust_con(int i) {
  if (check(i+Con)) {
    Con += i;
    if (i)
      call_out("zap_stat_cache", 0, "C");
  }
  call_out("reset_hp", 2);
  return Con;
} /* adjust_con() */

int adjust_str(int i) {
  if (check(i+Str)) {
    Str += i;
    if (i)
      call_out("zap_stat_cache", 0, "S");
  }
  reset_carry_cap();
  return Str;
} /* adjustr_str() */

int adjust_wis(int i) {
  if (check(i+Wis)) {
    Wis += i;
    if (i)
      call_out("zap_stat_cache", 0, "W");
  }
  call_out("reset_gp", 2);
  return Wis;
} /* adjust_wis() */

int adjust_int(int i) {
  if (check(i+Int)) {
    Int += i;
    if (i)
      call_out("zap_stat_cache", 0, "I");
  }
  call_out("reset_gp", 2);
  return Int;
} /* adjust_int() */

int adjust_tmp_int(int i) {
  inttmp += i;
  if (i)
    call_out("zap_stat_cache", 0, "I");
  call_out("reset_gp", 2);
  return inttmp;
} /* adjust_tmp_int() */

int adjust_tmp_dex(int i) {
  dextmp += i;
  if (i)
    call_out("zap_stat_cache", 0, "D");
  return dextmp;
} /* adjust_tmp_dex() */

int adjust_tmp_wis(int i) {
  wistmp += i;
  if (i)
    call_out("zap_stat_cache", 0, "W");
  call_out("reset_gp", 2);
  return wistmp;
} /* adjust_tmp_wis() */

int adjust_tmp_str(int i) {
  strtmp += i;
  if (i)
    call_out("zap_stat_cache", 0, "S");
  reset_carry_cap();
  return strtmp;
} /* adjust_tmp_str() */

int adjust_tmp_con(int i) {
  contmp += i;
  if (i)
    call_out("zap_stat_cache", 0, "C");
  call_out("reset_hp", 2);
  return contmp;
} /* adjust_tmp_con() */

int adjust_bonus_int(int i) {
  intbon += i;
  if (i)
    call_out("zap_stat_cache", 0, "I");
  call_out("reset_gp", 2);
  return intbon;
} /* adjust_bonus_int() */

int adjust_bonus_dex(int i) {
  dexbon += i;
  if (i)
    call_out("zap_stat_cache", 0, "D");
  return dexbon;
} /* adjust_bonus_dex() */

int adjust_bonus_wis(int i) {
  wisbon += i;
  if (i)
    call_out("zap_stat_cache", 0, "W");
  call_out("reset_gp", 2);
  return wisbon;
} /* adjust_bonus_wis() */

int adjust_bonus_str(int i) {
  strbon += i;
  if (i)
    call_out("zap_stat_cache", 0, "S");
  reset_carry_cap();
  return strbon;
} /* adjust_bonus_str() */

int adjust_bonus_con(int i) {
  conbon += i;
  if (i)
    call_out("zap_stat_cache", 0, "C");
  call_out("reset_hp", 2);
  return conbon;
} /* adjust_bonus_con() */

void update_tmps() {
  if (strtmp)
    call_out("zap_stat_cache", 0, "S");
  strtmp = strtmp/2;
  if (contmp)
    call_out("zap_stat_cache", 0, "C");
  contmp = contmp/2;
  if (wistmp)
    call_out("zap_stat_cache", 0, "W");
  wistmp = wistmp/2;
  if (inttmp)
    call_out("zap_stat_cache", 0, "I");
  inttmp = inttmp/2;
  if (dextmp)
    call_out("zap_stat_cache", 0, "D");
  dextmp = dextmp/2;
  call_out("reset_hp", 2);
  call_out("reset_gp", 2);
  reset_carry_cap();
} /* update_tmps() */

string stat_string(int i) {
  if (i<19)
    return i +"";
  if (i>28)
    return (i-10)+"";
  if (i==28)
    return "18/00";
  return "18/"+(i-18)+"0";
} /* stat_string() */
