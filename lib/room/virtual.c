inherit "/std/room/basic_room";

int no_init(){
  return 1;
}

void init(){
  //no_init doesn't work at discworld itself yet, when it does this stops doing
  //anything :) npcs only get here when their rooms unloads anyway, since 
  //virtual moving is off 
  previous_object()->move("/room/rubbish");
}

string long(object npc){
  //duh people might scry us.
  if(query_verb() == "scry"){
    object ob;
    string where;
    ob = load_object(where = npc->query_true_location());
    if(!ob || environment(npc) == this_object()){
      return "This is where NPCs hide from players.";
      //stupid room didn't load, or it wasn't a room!
    }
    return environment(npc)->long();
  }
  return "Go away!";
}

