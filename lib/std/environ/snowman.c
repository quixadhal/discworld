inherit "std/object";

void setup () {
  set_short("Snowman");
  set_long("A wonderfull snowman with glowing eyes made of coal and twigs "+
           "for ears.\n");
  set_name("snowman");
  call_out("die",400);
  reset_get();
}

string long(string str, int dark) {
  int i;

  if ((i=find_call_out("die")) > 300)
    return ::query_long();
  return ::query_long()+"It is "+({ "mostly", "half", "slightly" })[i/100]+
         " melted.\n";
}

void die() {
  say("The snow man melts into the floor.\n");
  dest_me();
}
