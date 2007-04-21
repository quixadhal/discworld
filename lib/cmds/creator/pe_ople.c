/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: pe_ople.c,v 1.3 2001/05/02 03:39:24 ceres Exp $
 * $Log: pe_ople.c,v $
 * Revision 1.3  2001/05/02 03:39:24  ceres
 * Added domain sort
 *
 * Revision 1.2  1999/10/28 02:35:38  ceres
 * added more options
 * .my
 *
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
/* people command, trial out by Turrican for a commands daemon. */
inherit "/cmds/base";
#include <creator.h>
#include <peopler.h>

int num_sort(object first, object second) {
  int one, two;

  one = first->query_level();
  two = second->query_level();

  //  reset_eval_cost();
  if(one < two)
    return 1;
  if(one > two)
    return -1;
  return 0;
}

int guild_sort(object first, object second) {
  object g1, g2;

  g1 = first->query_guild_ob();
  g2 = second->query_guild_ob();
  
  return strcmp(g1 ? g1->query_name() : "",
                g2 ? g2->query_name() : "");
}

int domain_sort(object first, object second) {
  object d1, d2;

  d1 = environment(first);
  d2 = environment(second);
  
  return strcmp(d1 ? file_name(d1) : "", d2 ? file_name(d2) : "");
}

int age_sort(object first, object second) {
  int one, two;

  one = first->query_time_on();
  two = second->query_time_on();

  if(one < two)
    return -1;
  if(one > two)
    return 1;
  return 0;
}

mixed cmd(string str) {
  mixed *stuff;
  string bit, *bits;
  function sort_func;
  
  stuff = this_player()->query_property("people list");
  if (!stuff)
    stuff = P_DEFAULT;

  if(str) {
    bits = explode(str, " ");
    str = 0;
    foreach(bit in bits) {
      switch(bit) {
      case "-l":
        sort_func = (: num_sort :);
        break;
      case "-g":
        sort_func = (: guild_sort :);
        break;
      case "-a":
        sort_func = (: age_sort :);
        break;
      case "-d":
        sort_func = (: domain_sort :);
        break;
      default:
        if(str)
          str += " " + bit;
        else
          str = bit;
      }
    }
  }
  return (int)PEOPLER->do_command(stuff, str, sort_func);
} /* cmd() */

mixed *query_patterns() {
   return ({ "", (: cmd(0) :),
             "<string>", (: cmd($4[0]) :) });
} /* query_patterns() */
