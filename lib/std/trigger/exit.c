void event_exit(object ob, string mess) {
  mixed *bit;
  int i;

  bit = (mixed *)this_object()->query_actions("exit");
  for (i=0;i<sizeof(bit);i+=2)
    call_other(bit[i+1][0], bit[i+1][1], ob, mess);
}
