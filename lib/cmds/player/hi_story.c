int cmd(string arg){
   return this_player()->print_history(arg);
}

mixed query_patterns() {
   return ({ "", (: cmd("") :),
               "<string'number|number,number'>", (: cmd($4[0]) :) });
} /* query_patterns() */
