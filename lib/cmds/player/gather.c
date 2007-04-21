/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: gather.c,v 1.6 2003/07/14 22:28:50 pinkfish Exp $
 * $Log: gather.c,v $
 * Revision 1.6  2003/07/14 22:28:50  pinkfish
 * Fixed up the error message.
 *
 * Revision 1.5  2000/09/02 22:19:38  ceres
 * Fixed faulty error message
 *
 * Revision 1.4  2000/06/27 16:43:05  wodan
 * New version, Tannah changed something.
 *
 * Revision 1.3  2000/06/27 14:42:27  sojan
 * Tannah changed to use add_failed_mess
 *
 * Revision 1.2  2000/04/03 13:39:51  taffyd
 * Made it so that gathering does not cause items to be insta-pickeld
 *
 * Revision 1.1  1998/01/06 05:29:43  ceres
 * Initial revision
 * 
*/
#include <move_failures.h>

inherit "/cmds/base";

// #define NO_DECAY 1
#define TP this_player()

int cmd( string str ) {
  object *items, *moved = ({ }), *left = ({ });
  int i;

  if( !stringp(str) || (str == "" )) {
    add_failed_mess("Syntax: gather <plant>\n");
    return 0;
  }
    
  items = "/obj/handlers/gathering"->gather_item(str, TP);
  if (!sizeof(items)) {
    if (str[<1] != 's') {
       str = pluralize(str);
    }
    add_failed_mess( "You couldn't find any " + str + ".\n");
    return 0;
  }
    
  for (i = 0; i < sizeof(items); i++) {
    if (items[i]->move(TP) == MOVE_OK) {
    moved += ({ items[i] });
  }  else 
    if(items[i]->move(environment(TP)) == MOVE_OK) {
      left += ({ items[i] });
    } else {
      items[i]->move("/room/rubbish");
    }
  }
  if (sizeof(moved)) {
#ifdef NO_DECAY
    moved->set_decay_speed(0);
#endif
    add_succeeded_mess( "$N gather$s $I.\n", moved );
    event( environment( this_player() ), "gather", this_player(), str, 
        moved );
  }

  left -= ({ this_player() });
  
  if (sizeof(left))
    write("You found but couldn't get " + query_multiple_short(left) + ".\n");
  return 1;
}

mixed query_patterns() {
  return ({ "<string>", (: cmd( $4[0] ) :) });
}
