/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: creator.c,v 1.6 2001/06/16 05:09:29 presto Exp $
 * $Log: creator.c,v $
 * Revision 1.6  2001/06/16 05:09:29  presto
 * Fixed warning.  (Added dummy 'object' argument to query_object_type)
 *
 * Revision 1.5  2001/03/23 23:08:20  ceres
 * Added go_invis param
 *
 * Revision 1.4  1999/02/10 04:14:12  ceres
 * Modified to get creator news from the login handler
 *
 * Revision 1.3  1998/10/30 09:10:15  pinkfish
 * Fix up the name variable to be a method call.
 *
 * Revision 1.2  1998/04/13 11:37:08  pinkfish
 * Removed all the swap stuff fomr the creator object and
 * generly cut it back :)
 *
 * Revision 1.1  1998/01/06 04:54:05  ceres
 * Initial revision
 * 
 */
/* this is the creator player object */

/**
 * This is the creator player object.  It gives the creators the
 * bonus extra commands needed by happy creators.
 * @author Pinkfish
 * @see /global/player.c
 */
#include <login_handler.h>
inherit "/global/wiz_file_comm";

#define ECHO_ALLOWED

/** @ignore yes */
void move_player_to_start(string bong, int new_pl, string c_name, string ident, int go_invis) {
  if(!sscanf(file_name(previous_object()), "/secure/login#%*s") &&
     !sscanf(file_name(previous_object()), "/secure/nlogin#%*s"))
    return 0;
#ifndef NEW_DRIVER
  //enable_wizard();
#endif
  ::move_player_to_start(bong, new_pl, c_name, ident, go_invis);
  write(LOGIN_HANDLER->get_message("/doc/CREATORNEWS"));
  if(query_invis()) {
    tell_object(this_object(), "===> You are currently INVISIBLE! <===\n");
  }
} /* move_player_to_start() */

/**
 * This method tells us if the object is a creator or not.
 * @return 1 if the object is a creator, 0 if not.
 */
int query_creator() { return 1; }

/** @ignore yes */
string query_object_type(object) {
   if ( master()->query_senior( query_name() ) )
      return "S";
   return "C";
} /* query_object_type() */

/**
 * This method is used by the snoop efun to allow creator to see
 * what other players are doing.
 * @param bing the message to receive
 */
void receive_snoop(string bing) {
  tell_object(this_object(), "] "+bing);
} /* receive_snoop() */
