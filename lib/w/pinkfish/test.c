inherit "/std/object";

void setup() {
  set_debug_level(0);
  set_name("tester");
  set_long("Testing testing 1 2 3");
  reset_drop();
}

void init() {
  add_action("do_test", "test");
}

int do_test(string str) {
  string arg1, arg2, arg3;

  string *womble;
  int i;

  womble = explode("#"+read_file("/doc/driver/concepts/socket_efuns"), "\n.");
  for (i=0;i<sizeof(womble);i++)
    write(strlen(womble[i])+"\n");
  if (!str) {
    notify_fail("Bing!\n");
    return 0;
  }
  write(query_multiple_short(explode(str, " ")));
  "/net/nameserver"->lookup_mud("discworld", "blue");
  "/net/nameserver"->lookup_mud("bing", "blue");
  write("Parse:"+parse_command("bing with bing", this_player(),
                               "%s 'with' %s", arg1, arg2, arg3));
  printf("%O\n", arg1);
  printf("%O\n", arg2);
  printf("%O\n", arg3);
  return 1;
}

void blue(mixed bing, mixed bing2) {
  printf("Yes yes more moe!\n");
  tell_object(find_player("pinkfish"), sprintf("%O %O\n", bing, bing2));
} /* blue() */

void event_person_say(object ob, string str, string mess) {
  tell_room(environment(), "I got "+str+","+mess+"\n");
} /* event_person_say() */
