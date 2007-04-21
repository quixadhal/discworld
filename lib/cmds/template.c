/* Help on the commands daemon is available as "help global.command". */

inherit "/cmds/base";

/* The entry point.  This function is what gets called by the commands
   daemon. */

mixed cmd(string args) {
  return 0;
}

/* Return a helpfull text about your command here, or write a nroff
   like document and remove this function. */

string help() {
  return "No help defined for this command, bug a creator about it!\n";
}

/* If your command is going be a guild-race command, you can define
   the following function, so your command is teachable.

int teach(object ob) {
  return 0;
}
*/

/* If you want to use the add_command() parser for your command, define
   the following function.  The default given here does no parsing at all,
   so the above cmd() function would work with it.

mixed *query_patterns() {
  return ({ "<string>", (: cmd($4[0]) :) });
}
*/
