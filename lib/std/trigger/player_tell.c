void event_person_tell(object ob, string start, string mess, string lang) {
  mixed *bit;
  int i;

  bit = (mixed *)this_object()->query_actions("player_tell");
  for (i=0;i<sizeof(bit);i+=2)
    call_other(bit[i+1][0], bit[i+1][1], ob, start, mess, lang);
} /* event_person_tell() */
