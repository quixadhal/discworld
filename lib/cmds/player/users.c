/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: users.c,v 1.4 2001/03/23 05:15:32 ceres Exp $
 * $Log: users.c,v $
 * Revision 1.4  2001/03/23 05:15:32  ceres
 * Added morestring
 *
 * Revision 1.3  2001/02/01 17:30:43  pinkfish
 * Fix up some bugs.
 *
 * Revision 1.2  1998/04/14 02:47:44  pinkfish
 * Changed to use add_command.
 *
 * Revision 1.1  1998/01/06 05:29:43  ceres
 * Initial revision
 * 
*/
inherit "/cmds/base";

#include <login_handler.h>

int cmd() {
  int players, logins, login_q, first, creators, total;
  string output, *str, *logins_n;
  object pl;

  str = ({ });
  foreach(pl in users()) {
/* This is done in users() now.
    switch((int)pl->query_invis()) {
      case 2:
        if(!"/secure/master"->high_programmer(geteuid(this_player())))
          break;
      case 1:
        if(!this_player()->query_creator())
          break;
      default:
*/
    if (pl->query_login_ob())
      logins++;
    else {
      str += ({ pl->query_cap_name() });
      if (pl->query_creator())
        creators++;
      else
        players++;
    }
/*
        break;
    }
*/
  }

  logins_n = map(LOGIN_HANDLER->query_login_queue(),
                 (: $1->query_cap_name() :));
  logins_n = filter(logins_n, (: $1 :));
  logins_n = sort_array(logins_n,
                        (: strcmp(lower_case($1), lower_case($2)) :));
  login_q = sizeof(logins_n);
  if (login_q > 0) {
    first = 0;
    if (login_q > 1)
      write(sprintf(login_q+" currently queued: %-=*s\n",
                    this_player()->query_cols()-17,
                    implode(logins_n[0..<2], ", ")+" and "+
                    logins_n[<1]));
    else
      write(sprintf("Currently Queued: %-=*s\n",
                    this_player()->query_cols()-19,
                    logins_n[0]));
  }
  
  output = "There ";
  
  first = 1;
  if (creators > 0) {
    if (first)
      if (creators == 1)
        output += "is ";
      else
        output += "are ";
    if (creators == 1)
      output += "one creator";
    else
      output += creators +" creators";
    first = 0;
    if (logins - login_q > 0)
      output += ", ";
    else
      if ( !players )
        output += " logged on.\n";
      else
        output += " and ";
  }
  
  if (logins - login_q > 0) {
    if (first)
      if (logins - login_q == 1)
        output += "is ";
      else
        output += "are ";
    if (logins - login_q == 1)
      output += "one person logging in";
    else
      output += ( logins - login_q ) +" people logging in";
    first = 0;
    if ( !players )
      output += ".\n";
    else
      output += " and ";
  }

  if ( players ) {

    if (first)
      if (players == 1)
        output += "is ";
      else
        output += "are ";
    if (players == 1)
      output += "one player ";
    else
      output += players +" players ";
    
  }

  total = sizeof(users());
  output += "a total of "+ sizeof(users()) + " people logged on.\n";

  write( output );

  str = filter(str, (: $1 :));
  str = sort_array(str, (: strcmp(lower_case($1), lower_case($2)) :));
  login_q = sizeof(logins_n);
  if (sizeof(str) > 1) {
    this_player()->more_string(sprintf("Currently logged on: %-=*s\n",
                  this_player()->query_cols()-24,
                  implode(str[0..<2], ", ")+" and "+
                  str[<1]), "Who", 1 );
  } else {
    write("Currently logged on: "+str[0]+".\n");
  }
  return 1;
} /* cmd() */

mixed *query_patterns() {
   return ({ "", (: cmd() :) });
}  /* query_patterns() */
