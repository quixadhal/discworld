/*  -*- LPC -*-  */
/*
 * $Id: dbxframe.c,v 1.2 2000/10/16 20:02:19 turrican Exp $
 */
inherit "/cmds/base";

string print_vars(mixed *vars) {
  string *result = allocate(sizeof(vars));
  int i;

  for (i=0; i<sizeof(vars); i++) {
    if (mapp(vars[i]))
      result[i] = "([ ... ])";
    else if (functionp(vars[i]))
      result[i] = "(: ... :)";
    else if (intp(vars[i])) {
      if (vars[i]) result[i]=vars[i]+"";
      else if (nullp(vars[i])) result[i]="NULL";
      else if (undefinedp(vars[i])) result[i]="UNDEFINED";
      else result[i]="0";
    }
    else if (stringp(vars[i]))
      result[i] = "\""+vars[i]+"\"";
    else if (pointerp(vars[i]))
      result[i] = "({ ... })";
    else if (floatp(vars[i]))
      result[i] = vars[i]+"";
    else if (bufferp(vars[i]))
      result[i] = "<BUFFER>";
  }
  return implode(result, ", ");
}

int cmd(string str) {
  mapping frame;
  object ob;
  string who;
  int num;
  string file;

  if (!str || (sscanf(str, "%d", num) != 1 &&
               sscanf(str, "%s %d", who, num) != 2)) {
    write("dbxframe [<who>] <frame>\n");
    return 1;
  }
  if (who) {
    who = this_player()->expand_nickname(who);
    ob = find_player(who);
    if (!ob) {
      write("No such player.\n");
      return 1;
    }
  }
  else ob = this_player();
  frame = ob->get_last_error();
  if (!frame) {
    write("No error.\n");
    return 1;
  }
  if (num < 1 || num > sizeof(frame["trace"]))
    return notify_fail("No such frame.\n");
  num--;
  frame = frame["trace"][num];
  file = frame["file"];
  if (file[0] != '/') {
    file = "/" + file;
  }
  
  printf("------%s:%i - %s(%s)\n", file, frame["line"],
         frame["function"],print_vars(frame["arguments"]));
  printf("locals: %s\n", print_vars(frame["locals"]));
  printf("----------------------------------------------------------------\n");
  write(sprintf("%s%%^YELLOW%%^=>%s%%^RESET%%^%s",
         read_file(file, frame["line"]-5, 5),
         read_file(file, frame["line"], 1),
         read_file(file, frame["line"]+1, 5)));
  return 1;
}
