/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: upper_case.c,v 1.1 1998/01/06 04:02:57 ceres Exp $
 * $Log: upper_case.c,v $
 * Revision 1.1  1998/01/06 04:02:57  ceres
 * Initial revision
 * 
*/
#include "path.h"
#define MY_NAME "UPPER CASE"

/*
 * The upper case curse.  Means everything you say comes out in upper
 * case.
 */
object my_player;

int init_curse(object pl) {
  string str;
  int i;

  if (sscanf((str = file_name(this_object())), "%s#%d", str, i) != 2) {
/* A class not a clone */
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
int query_remove() { return 1; }

/*
 * Called when the player logs on
 */
int player_start(object pl) {
  object ob;

  if (!pl)
    pl = this_player();
  ob = clone_object(file_name(this_object()));
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

protected string upper_case(string str) {
  string ret;
  int i;

  ret = "";
  for (i=0;i<strlen(str);i++)
    ret += capitalize(str[i..i]);
  return ret;
} /* upper_case() */

int do_say(string str) {
  return (int)my_player->do_say(upper_case(str));
} /* do_say() */

int do_tell(string str) {
  return (int)my_player->do_tell(upper_case(str));
} /* do_tell() */

int do_loud_say(string str) {
  return (int)my_player->do_loud_say(upper_case(str));
} /* do_loud_say() */

int do_echo(string str) {
  return (int)my_player->do_echo(upper_case(str));
} /* do_echo() */

int do_emote_all(string str) {
  return (int)my_player->do_echo_all(upper_case(str));
} /* do_emote_all() */

int do_emote(string str) {
  return (int)my_player->do_emote(upper_case(str));
} /* do_emote() */

int do_whisper(string str) {
  return (int)my_player->do_whisper(upper_case(str));
} /* do_whisper() */

int do_echo_to(string str) {
  return (int)my_player->do_echo_to(upper_case(str));
} /* do_echo_to() */

int do_shout(string str) {
  return (int)my_player->do_shout(upper_case(str));
} /* do_shout() */
