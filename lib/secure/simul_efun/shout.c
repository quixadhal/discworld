varargs void shout(string str, object avoid) {
  object *ob;
  int i;

  ob = users();
  for (i=0;i<sizeof(ob);i++)
    if (ob[i] != avoid && !ob[i] -> query_earmuffs())
  if(ob[i] != this_player()) {
    ob[i]->event_say(previous_object(), str, ({ }));
  }
} /* shout() */
