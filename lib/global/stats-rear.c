static int me_str, me_int, me_dex, me_con, me_wis;
static int pool;

void do_save();
void get_stat_bit(string str);

static private string b(int i) {
  return (string)this_object()->stat_string(i);
}

void display_stats() {
  printf("Str: %s Dex: %s Int: %s Con: %s Wis: %s Pool: %d\n", b(me_str),
         b(me_dex), b(me_int), b(me_con), b(me_wis), pool);
}

int rearrange() {
  if (this_object()->query_property("stats_rearranged")) {
    notify_fail("You have already arranged your stats.\n");
    return 0;
  }
  write("Rearanging stats.\n\n");
  me_str = (int)this_object()->query_real_str();
  me_dex = (int)this_object()->query_real_dex();
  me_int = (int)this_object()->query_real_int();
  me_con = (int)this_object()->query_real_con();
  me_wis = (int)this_object()->query_real_wis();
  pool = 0;
  if (me_str > 8) {
    pool += me_str-8;
    me_str = 8;
  }
  if (me_int > 8) {
    pool += me_int-8;
    me_int = 8;
  }
  if (me_wis > 8) {
    pool += me_wis-8;
    me_wis = 8;
  }
  if (me_con > 8) {
    pool += me_con-8;
    me_con = 8;
  }
  if (me_dex > 8) {
    pool += me_dex-8;
    me_dex = 8;
  }
  write("Entry...\n");
  write("Type \"d 8\" to add 8 points to your dexterity from the pool.  "+
        "or \"d -8\" to remove 8 points from dextreity and add them to "+
        "the pool. \"Save\" will save and quit at the current position, "+
        "\"reset\" will reset you back to your starting position "+
        "and \"quit\" will quit and not save your changes.\n");
  get_stat_bit("");
  return 1;
}

int get_stat_bit(string str) {
  string stat;
  int num;

  if (lower_case(str) == "save") {
    if (pool) {
      write("You still have some points in your pool are you sure you "+
            "want to save [y/n]? ");
      input_to("get_check");
      return 1;
    }
    write("Ok saving...\n");
    do_save();
    return 1;
  }
  if (lower_case(str) == "quit") {
    write("Ok... Quiting.\n");
    return 1;
  }
  if (lower_case(str) == "reset") {
    write("Ok, reseting back to defaults.\n");
    return rearrange();
  }
  sscanf(str, "%s %d", str, num);
  stat = lower_case(implode(explode(str, " "), ""));
  if (num > pool)
    if (!pool) {
      write("Cannot add to stat.. Need to put things into the pool first.\n");
      stat = "";
    } else {
      write("Pool has only "+pool+" points in it.. adding these to stat.\n");
      num = pool;
    }
  switch (stat[0]) {
    case 'd' :
      if ((me_dex-7) <= -num) {
        write("Cannot set your stats to less than 8.\n");
        num = 0;
      } else
        me_dex += num;
      if (me_dex > 28) {
        num -= me_dex-28;
        me_dex = 28;
      }
      break;
    case 'c' :
      if ((me_con-7) <= -num) {
        write("Cannot set your stat to less than 8.\n");
        num = 0;
      } else
        me_con += num;
      if (me_con > 28) {
        num -= me_con-28;
        me_con = 28;
      }
      break;
    case 'i' :
      if ((me_int-7) <= -num) {
        write("Cannot set your stat to less than 8.\n");
        num = 0;
      } else
        me_int += num;
      if (me_int > 28) {
        num -= me_int-28;
        me_int = 28;
      }
      break;
    case 'w' :
      if ((me_wis-7) <= -num) {
        write("Cannot set your stat to less than 8.\n");
        num = 0;
      } else
        me_wis += num;
      if (me_wis > 28) {
        num -= me_wis-28;
        me_wis = 28;
      }
      break;
    case 's' :
      if ((me_str-7) <= -num) {
        write("Cannot set your stat to less than 8.\n");
        num = 0;
      } else
        me_str += num;
      if (me_str > 28) {
        num -= me_str-28;
        me_str = 28;
      }
      break;
    default :
      num = 0;
      break;
  }
  pool -= num;
  display_stats();
  write("Entry [d|c|i|w|s|save|reset|quit] <num> : ");
  input_to("get_stat_bit");
  return 1;
}

void get_check(string str) {
  str = lower_case(str);
  if (str[0] != 'y') {
    write("Ok going back to stat entering.\n");
    get_stat_bit("");
    return;
  }
  do_save();
  write("Ok saved.\n");
  return ;
}

void do_save() {
  this_object()->set_con(me_con);
  this_object()->set_dex(me_dex);
  this_object()->add_property("stats_rearranged", 1);
  this_object()->set_int(me_int);
  this_object()->set_wis(me_wis);
  this_object()->set_str(me_str);
}
