void event_say(object ob, string mess, mixed avoid) {
  mixed *bit;
  int i;

  if (pointerp(avoid)) {
    if (member_array(this_object(), avoid) != -1)
      return ;
  } else
    if (this_object() == avoid)
      return ;
  bit = (mixed *)this_object()->query_actions("say");
  for (i=0;i<sizeof(bit);i+=2)
    call_other(bit[i+1][0], bit[i+1][1], ob, mess, avoid);
}
