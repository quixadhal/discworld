int cmd(string arg, int brief){
  function bing;

  bing = this_player()->get_htell_func();
  arg = this_player()->expand_nickname(arg);
  return evaluate(bing, arg, brief);
}


mixed *query_patterns()  {
    return ({ "brief search <string'name'>", (: cmd(lower_case($4[0]), 1) :),
              "search <string'name'>", (: cmd(lower_case($4[0]), 
                                     !this_player()->query_verbose("htell")) :),
              "brief", (: cmd("", 1) :),
              "verbose", (: cmd("", 0) :),
              "", (: cmd("", !this_player()->query_verbose("htell")) :)
           });
}
