inherit "/std/room";

void setup() {
  set_light(50);
  set_short("prison");
  set_long("Dark damp wet.\n");
}

void init() {
  add_action("do_comm", "*",10001);
}

int do_comm() 
{
  if (this_player(1)->query_lord()) return 0;
  write("You are in prison.\n");
  return 1;
}

int test_remove(object ob, int flag)  { 
  if (ob && ob->query_lord()) return 1;
  return 0; 
}
