/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: sayto.c,v 1.6 2001/11/07 22:02:50 pinkfish Exp $
 */
#include <language.h>
#include <player.h>
#include <drinks.h>

inherit "cmds/base";
inherit "/cmds/speech";

#define TP this_player()

mixed cmd(object *obs, string str) {
  class message mess;

  obs = filter(obs, (: $1 && $1 != this_player() :));
  if(!sizeof(obs)) {
    return notify_fail("Be serious.\n");
  }
  
  mess = build_message(str, obs, "say");
  return say_it_to(mess, obs, 0, "person_say");
}

mixed *query_patterns() {
   return ({ "<indirect:living> <string>", (: cmd($1, $4[1]) :) });
} /* query_patterns() */
