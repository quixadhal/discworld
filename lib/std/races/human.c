inherit "/std/races/standard";

void setup() {
  set_long("The human race.  The standard by which all the races are set, "+
           "this is race can be considered the norm.\n");
   set_name("human");
}

string query_desc(object ob) {
  if ((int)ob->query_gender() == 1)
    return "A strapping young human lad.\n";
  return "A strapping young human lass.\n";
}

void new_set_level(mixed *bing) {
  if (!bing[1]) return ;
  bing[1]->add_attack("hands", 0, 100, 30, 0, 0, "blunt");
  bing[1]->add_ac("bing", "blunt", 15);
  bing[1]->add_ac("bing2", "sharp", 10);
  bing[1]->add_ac("bing3", "pierce", 12);
}
int query_dark(int light) {
  return (light < 20) || (light > 300);
}
