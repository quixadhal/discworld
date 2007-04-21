inherit "/cmds/base";

int cmd(object who) {
  if (!interactive(who))
    write(who->query_cap_name() + " is net-dead.\n");
  else
    write(who->query_cap_name() + " has been idle for " +
          "/secure/finger"->time_elapsed_string(query_idle(who)) + ".\n");
  return 1;
  
}

mixed *query_patterns() {
  return ({ "<indirect:player>", (: cmd($1[0]) :) });
}
