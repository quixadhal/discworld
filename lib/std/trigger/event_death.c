int event_death(object ob, object *obs) {
  mixed *bit;
  int i, ret;

  bit = (mixed *)this_object()->query_actions("event_death");
  for (i=0;i<sizeof(bit);i+=2)
    catch(ret += (int)call_other(bit[i+1][0], bit[i+1][1], ob, obs));
  return ret;
}
