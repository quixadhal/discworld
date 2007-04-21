/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: ken_curse.c,v 1.1 1998/01/06 04:02:57 ceres Exp $
 * $Log: ken_curse.c,v $
 * Revision 1.1  1998/01/06 04:02:57  ceres
 * Initial revision
 * 
*/
#include "path.h"
#define MY_NAME "ken curse"

/*
 * The shout curse.  Makes people unable to shout...
 */
object my_player;

int init_curse(object pl) {
  string str, i;

  if (sscanf((str = file_name(this_object())), "%s#%d", str, i) != 2) {
/* A class, not a clone. */
    seteuid((string)"/secure/master"->creator_file(str));
    return (int)clone_object(str)->init_curse(pl);
  }
  my_player = pl;
  pl->add_curse(MY_NAME, str);
  if (!shadow(pl, 1)) {
    pl->remove_curse(MY_NAME);
    return 0;
  }
  return 1;
} /* init_curse() */

/*
 * No special requirements for removeing this curse...
 */
int query_remove(string name) {
  if (name != MY_NAME)
    return (int)my_player->query_remove(name);
  return 1;
} /* query_remove() */

/*
 * Called when the player logs on
 */
int player_start(object pl) {
  object ob;

  if (!pl)
    pl = this_player();
  ob = clone_object(HERE+"ken_curse");
  ob->init_curse(pl);
} /* player_start() */

/*
 * This gets called with the name of the curse we are getting rid of.
 */
int destruct_curse(string str) {
  if (str == MY_NAME)
    destruct(this_object());
  else
    my_player->destruct_curse(str);
} /* destruct_curse() */

int do_emote_all(string str) {
  return (int)my_player->do_emote_all((HERE+"ken_mangle")->do_transmute(str));
} /* do_emote_all() */

int do_shout(string str) {
  return (int)my_player->do_shout((HERE+"ken_mangle")->do_transmute(str));
} /* do_shout() */

int do_say(string str) {
  return (int)my_player->do_say((HERE+"ken_mangle")->do_transmute(str));
} /* do_say() */

int do_loud_say(string str) {
  return (int)my_player->do_loud_say((HERE+"ken_mangle")->do_transmute(str));
} /* do_loud_say() */

int do_emote(string str) {
  return (int)my_player->do_emote((HERE+"ken_mangle")->do_transmute(str));
} /* do_emote() */

int do_tell(string str) {
  string s1;

  if (sscanf(str, "%s %s", s1, str) != 2)
    return (int)my_player->do_tell(str);
  return (int)my_player->do_tell(s1+" "+(HERE+"ken_mangle")->do_transmute(str));
} /* do_tell() */
