int event_fight_in_progress(object ob, object ob1, object ob2) {
  mixed *bit;
  int i, ret;

  bit = (mixed *)this_object()->query_actions("event_fight");
  for (i=0;i<sizeof(bit);i+=2)
    ret += (int)call_other(bit[i+1][0], bit[i+1][1], ob, ob1, ob2);
  return ret;
}
