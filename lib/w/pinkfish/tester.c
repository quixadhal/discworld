inherit "/std/object";

void setup() {
  set_name("womble");
  add_alias("tester");
}

void init() {
  add_action("test", "test");
}

int test(string str) {
  mixed arg1,arg2,arg3,arg4,arg5,arg6;
  printf("Parse command %d\n",
         parse_command(str, environment(this_player()),
                       "%s %p %s",
                       arg1, arg2, arg3, arg4,arg5, arg6));
  if (arg1)
    printf("arg1:%O\n", arg1);
  if (arg2)
    printf("arg2:%O\n", arg2);
  if (arg3)
    printf("arg3:%O\n", arg3);
  if (arg4)
    printf("arg4:%O\n", arg4);
  if (arg5)
    printf("arg5:%O\n", arg5);
  if (arg6)
    printf("arg6:%O\n", arg6);
  return 1;
}
