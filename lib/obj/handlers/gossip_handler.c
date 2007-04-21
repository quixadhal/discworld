/*  -*- LPC -*-  */
/*
 * $Locker: terano $
 * $Id: gossip_handler.c,v 1.3 2000/01/04 12:03:25 rue Exp terano $
 * $Log: gossip_handler.c,v $
 * Revision 1.3  2000/01/04 12:03:25  rue
 * Added a couple of words to the filter
 *
 * Revision 1.2  1999/03/11 19:43:54  sin
 * Added a function to remove gossip
 *
 * Revision 1.1  1998/01/06 05:03:33  ceres
 * Initial revision
 * 
*/
/**
 * Deals out random gossip to people who request it.
 * <p>
 * Add rumours!
 * @author Pinkfish
 */
#include <gossip.h>

mixed *gossip;
mixed *rumours;
nosave mixed *filter_these;
private void save_stuff();

void create() {
   seteuid(getuid());
   filter_these = ({ "fuck", "fucking", "fucker", "cunt", "bitch", "shit" });
   gossip = ({ });
   rumours = ({ });
   unguarded( (: restore_object(GOSSIP_SAVE_FILE) :) );
   call_out((: save_stuff :), 10*60);
} /* create() */

private void save_stuff() {
   call_out((: save_stuff :), 10*60);
   if (sizeof(gossip) > GOSSIP_KEEP_LEVEL) {
      gossip = gossip[sizeof(gossip)-GOSSIP_KEEP_LEVEL..];
   }

   if (sizeof(rumours) > RUMOUR_KEEP_LEVEL) {
      rumours = rumours[sizeof(rumours)-RUMOUR_KEEP_LEVEL..];
   }

   unguarded( (: save_object(GOSSIP_SAVE_FILE) :) );
} /* save_stuff() */

/**
 * Add a juicy bit of gossip.  This is called by ethe gossip
 * effects on the npcs.
 * @see /std/effect/npc/gossip.c
 * @param name the person who said the gossip
 * @param mess what they said
 */
void add_gossip(string name, string mess) {
   string *bits;
   int i;

   bits = explode(mess, " ");
   for (i = 0; i < sizeof(bits); i++) {
      if (member_array(bits[i], filter_these) != -1) {
         return ;
      }
   }

   gossip += ({ ({ name, mess }) });
} /* add_gossip() */

/**
 * Sometimes you just gotta remove some gossip
 */
varargs string remove_gossip(string match, int force)
{
  int pos, index;

  if (!match || match == "") return "Must provide a match string";;

  pos = -1;
  for (index = sizeof(gossip); index; index--) {
    if (strsrch(gossip[index - 1][1], match) != -1) {
      if (pos == -1)
        pos = index - 1;
      else if (!force) {
        return "too many matches";
      }
    }
  }
  if (pos != -1) {
    string found = "found " + gossip[pos][1] + " by " + gossip[pos][0];
    gossip = delete(gossip, pos, 1);
    return found;
  } else
    return "No match";
}

/**
 * Rumours are another method of dealing out gossip.
 * Rumours are added by creators...
 * @param name the name of the person saying the rumour
 * @param mess the rumour message
 */
void add_rumour(string name, string mess) {
   rumours += ({ ({ name, mess }) });
   save_stuff();
} /* add_rumour() */

/**
 * Returns a random piece of gossip.  Finds a random bit of juicy
 * gossip and returns it.  The first element of the returned
 * array is the person who said the gossip and the second bit is
 * what they said.
 * @return a two element array containing the gossip
 */
string *query_random_gossip() {
   if (sizeof(rumours) && random(2)) {
      return rumours[random(sizeof(rumours))];
   } else {
      if (sizeof(gossip))
	 return gossip[random(sizeof(gossip))];
      else
	 return 0;
   }
} /* query_random_gossip() */

/**
 * Tell us all the gossip.  This returns the complete
 * array of gossip which the npcs are drawing from.
 * @return an array of two element arrays
 * @see query_random_gossip() 
 */
mixed *query_gossip() {
   return gossip;
} /* query_gossip() */

/**
 * The current words we are filtering.  If any of these words are in the
 * gossip, we filter it out and do not save it.
 * @return the current filter array
 */
string *query_filter() {
   return filter_these;
} /* query_filter() */

/**
 * All of the current rumours.
 * @return an array of teo element arrays
 * @see query_random_gossip()
 */
mixed *query_rumours() {
   return rumours;
} /* query_rumours() */
