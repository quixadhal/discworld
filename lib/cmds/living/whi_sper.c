/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: whi_sper.c,v 1.10 2003/01/30 07:59:04 pinkfish Exp $
 */

inherit "/cmds/base";
inherit "/cmds/speech";

mixed cmd( object *others, string last ) {
  class message mess;

  if (sizeof(others) == 1 && others[0] == this_player()) {
     add_failed_mess("Whispering to yourself?  Neat trick.\n");
     return 0;
  }

  mess = build_message(last, others, "whisper");
  mess->type = "whisper";
  return say_it_to(mess, others, 1, "whisper");
}

mixed *query_patterns() {
   return ({ "<string'message'> to <indirect:living>", (: cmd($1, $4[0]) :),
             "<indirect:living> <string'message'>", (: cmd($1, $4[1]) :) });
} /* query_patterns() */
