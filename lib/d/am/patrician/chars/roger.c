inherit "/obj/monster";

mapping emote_quest;

void setup() {
  emote_quest = ([ ]);
}

void init() {
  ::init();
  if (!this_player()->query_property("emote") &&
      interactive(this_player()) &&
      !sizeof(query_attacker_list())) {
    emote_quest[this_player()] = 1;
    call_out("start_quest", 0, this_player());
  }
}

void event_exit(object ob, string mess, object *avoid) {
  if (emote_quest[ob])
    emote_quest = m_delete(emote_quest, ob);
}

void start_quest(object ob) {
  say("Roger asks "+ob->query_cap_name()+" something.\n");
  tell_object(ob, 
sprintf("Roger asks you: %-=*s\n", (int)ob->query_cols() - 17,
"To get the skill of emote you must perform some things for me.  Will you "+
"first please say something loudly for me?"));
}

void event_person_say(object ob, string str, string mess) {
  string s1;

  if (sscanf(str, "%s loudly: ", s1))
    if (emote_quest[ob] == 1) call_out("second_question", 0, this_player());
}

void second_question(object ob)
{
  tell_room(environment(), "Roger whispers to " +
            ob->query_cap_name()+ ".\n", ({ ob }));
  tell_object(ob, sprintf("Roger whispers to you: %-=*s\n",
              (int)ob->query_cols() - 23,
              "Good work.  Now could you bing excitedly at me?"));
  emote_quest[ob] = 2;
}

void event_soul(object ob, string str, object *avoid)
{
  string fish;

  if(sscanf(str, "%s bings excitedly at you.\n", fish) && emote_quest[ob] == 2)
    call_out("give_emote", 0 , this_player());
}

void give_emote(object ob)
{
  tell_room(environment(this_object()), "Roger whispers something to " +
            ob->query_cap_name() + ".\n", ({ ob }));
  tell_object(ob, 
sprintf("Roger whispers to you: %-=*s\n", (int)ob->query_cols() - 23,
"Congratulations!  The emotion department here at the Patrician's office "+
"would like to wish you well in your future adventures.  You now have the "+
"ability to emote.\n"));
  emote_quest = m_delete(emote_quest, ob);
  ob->add_property("emote", 1);
}
