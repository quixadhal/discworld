void event_light_change(object ob, int delta) {
  mixed *bit;
  int i;

  bit = (mixed *)this_object()->query_actions("light_change");
  for (i=0;i<sizeof(bit);i+=2)
    call_other(bit[i+1][0], bit[i+1][1], ob, delta);
}
