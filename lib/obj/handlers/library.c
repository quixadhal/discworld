/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: library.c,v 1.19 2003/03/25 23:13:15 drakkos Exp $
 * 
*/
/**
 * The library is a sort of information center.
 * Currently it supports only the recording on stories about players and
 * recording the quests that they have done.
 * @author Furball
 * @see /include/library.h
 */

#include <quest_handler.h>
#include <player_handler.h>

#define XP 10000
#define XP_QUESTS ({"womble's brooch", "balance quest", "easy post quest", \
                    "hard post quest", "apprentice baker" })
#define RESTORE_PATH  "save/library/"

#define MAX_CACHE_SIZE 100
#define CACHE_TIMEOUT 900

inherit "/std/object";
 
int *times;
string player_name, *quests, text_file;
mapping player_quest_info;

nosave mapping qps;
nosave int requests, cache_hits;

class qinfo {
  int cached;
  
  string player_name;
  string *quests;
  int *times;
  string text_file;
  mapping player_quest_info;
}

nosave mapping player_cache = ([ ]);

int new_top_quest();
int query_quest_time(string, string);

private void init_data(string pl_name) {
  player_name = pl_name;
  quests = ({ });
  times = ({ });
  text_file = "";
  player_quest_info = ([ ]);
} /* init_data() */

void clean_cache() {
  string name;
  
  foreach(name in keys(player_cache))
    if(player_cache[name]->cached < time() - CACHE_TIMEOUT)
      map_delete(player_cache, name);
}

private int get_data_file(string name) {
  int success;
  class qinfo tmp;
  
  requests++;

  if(player_cache[name]) {
    player_cache[name]->cached = time();
    cache_hits++;
    return 1;
  }

  success = unguarded((: restore_object, RESTORE_PATH+name[0..0]+"/"+name :));
  if(!success)
    init_data(name);

  tmp = new(class qinfo,
            cached : time(),
            player_name : player_name,
            quests : quests,
            times : times,
            text_file : text_file,
            player_quest_info : player_quest_info);
  player_cache[name] = tmp;
  
  if((sizeof(player_cache) > MAX_CACHE_SIZE) &&
     (find_call_out("clean_cache") == -1))
    call_out("clean_cache", 60);

  return success;
}
 
private void save_data_file(string name) {
  if(!player_cache[name])
    return;

  player_name = player_cache[name]->player_name;
  quests = player_cache[name]->quests;
  times = player_cache[name]->times;
  text_file = player_cache[name]->text_file;
  player_quest_info = player_cache[name]->player_quest_info;

  unguarded((: save_object, RESTORE_PATH+name[0..0]+"/"+name :));
  return ;
} /* save_data_file() */

/**
 * This method returns the players current title.
 * @param name the name of the player
 * @return their current title, 0 if no title
 * @see /obj/handlers/quest_handler.c
 */
string query_title( string name ) {
  string quest;
  
  get_data_file( name );
  if(!player_cache[name]->quests || !sizeof(player_cache[name]->quests)) {
    return 0;
  }

  quest = player_cache[name]->
    quests[random(sizeof(player_cache[name]->quests))];

  return (string)QUEST_HANDLER->query_quest_title(quest);
}

/**
 * This method returns the set of currently completed quests by the
 * player.
 * @param name the name of the player
 * @return the array of completed quests
 */
string *query_quests(string name) {
  get_data_file(name);
  if(!player_cache[name]->quests ) {
    return ({ });
  }
  return player_cache[name]->quests + ({ });
}

/**
 * This method returns the most recently completed quest by the player.
 * @param name the player name
 * @return the most recently completed quest
 */
string get_most_recent_quest(string name) {
  int loop, highest;
  string quest_name;
  string *quests;
  int *times;
  
  if(!get_data_file(name))
    return "Sorry";

  quests = player_cache[name]->quests;
  times = player_cache[name]->times;
  
  if(sizeof(quests) == 0)
    return "None";

  for(highest = loop = 0; loop < sizeof(quests); loop++) {
    if(times[loop] > highest) {
      highest = times[loop];
      quest_name = quests[loop];
    }
  }
  return quest_name;
}

/**
 * This method get sthe most recent time a quest was complete by the
 * player.
 * @param name the name of the player 
 * @return the time of the most recently completed quest
 */
int get_most_recent_time(string name) {
  int loop, hightime;
  string *quests;
  int *times;
  if(!get_data_file(name)) {
    return -1;
  }
  quests = player_cache[name]->quests;
  times = player_cache[name]->times;
  
  if(sizeof(quests) == 0) {
    return 0;
  }
  for(hightime = loop = 0; loop < sizeof(quests); loop++) {
    if(times[loop] > hightime) {
      hightime = times[loop];
    }
  }
  return hightime;
}

/**
 * This method sets the player as having done the quest and if we
 * should give them xp for it
 * This function should be called when a quest is finished.  It will then
 * call the quest_completed function on the quest handler and do all
 * assorted modifications to the player object etc.  The name should be the
 * players name and the quest should be the name of the quest that is
 * stored in the quest handler.
 * <p>
 * The include file <library.h> should be used for calls to this
 * handler.
 * @return 0 if the quest is already completed
 * @see /obj/handlers/quest_handler->quest_completed()
 * @param pl_name name of the player
 * @param qu_name name of the quest
 * @param no_xp do not give out xp
 * @example
 * // Set the player as completing the womble friend quest, they get
 * // xp for it.
 * LIBRARAY->set_quest(this_player()->query_name(), "womble friend", 0);
 */
int set_quest(string pl_name, string qu_name, int no_xp) {
  int qu_level;
  object ob;
  object card;
  int i;

  // guests can't do quests.
  if(!find_player(pl_name) || find_player(pl_name)->query_property("guest"))
    return 0;

  i = QUEST_HANDLER->query_quest_status(qu_name);
  // Inactive quests automatically fail.

  ob = find_player (pl_name);
  
  if (i == 0 && ob->query_playtester()) {
    tell_object (ob, "%^BOLD%^If this quest weren't inactive, you'd be "
      "showered in riches right about now!\n%^RESET%^");       
  }
  
  if(i < 1) {    
   user_event( "inform", pl_name +" completes "+ qu_name + " (inactive)", "quest");
    
    return 0;
  }
  
  // Playtesters with playtester protection enabled do not get the 
  // quest.
  if (ob && ob->advancement_restriction()) {
    log_file ("/log/secure/playtesters/protection_log", "%s: %s "
      "completed quest %s while advancement restricted active.\n", ctime(time()),
      pl_name, qu_name);
    return 0;
  }
  
  // clean out the qps cache so that we don't get invalid data.
  if(qps && qps[pl_name])
    map_delete(qps, pl_name);

  get_data_file(pl_name);

  /* Already done the quest? */
  if ( member_array( qu_name, player_cache[pl_name]->quests ) != -1 )
    return 0;

  player_cache[pl_name]->quests += ({ qu_name });
  player_cache[pl_name]->times += ({ time() });

  /* Make sure the quest exists */
  qu_level = (int)QUEST_HANDLER->query_quest_level(qu_name);
  if ( qu_level < 1 )
    return 0;

  QUEST_HANDLER->quest_completed( pl_name, qu_name, previous_object() );

  save_data_file( pl_name );
  
  if ( !ob ) {
    return 0;
  }

  if(member_array(qu_name, XP_QUESTS) != -1) {
    ob->adjust_xp(XP*qu_level, 0);
    call_out("save_them", 1, ob);
  }

  ob->set_title( "quest",
                 (string)QUEST_HANDLER->query_quest_title( qu_name ) );

  card = clone_object ("/d/underworld/creator_cards/creator_card");
  
  card->move (ob, "$N appear$s in your inventory with a flash.");
  return 1;
}

/**
 * This method removes a quest from the players list of completed quests.
 * @param pl_name the player name
 * @param qu_name the quest name
 * @return 0 if they have not done the quest, 1 if they habe
 * @see /obj/handlers/quest_handler.c
 */
int unset_quest(string pl_name, string qu_name) {
  int qu_level;

  get_data_file(pl_name);
  /* Already done the quest? */
  if ( member_array( qu_name, player_cache[pl_name]->quests ) == -1 )
    return 0;
  
  /* Make sure the quest exists */
  qu_level = (int)QUEST_HANDLER->query_quest_level(qu_name);
  if ( qu_level < 1 )
    return 0;
  
  player_cache[pl_name]->quests -= ({ qu_name });
  player_cache[pl_name]->times -= ({ time() });

  save_data_file( pl_name );
  return 1;
}

/**
 * This method causes the player to be saved.
 * @param thing the player to save
 */
void save_them( object thing ) {
  if ( thing )
    thing->save_me();
}

/**
 * This method  sets the information related to the players quest.
 * This information is used for quests which have several parts to them
 * and information needs to be stored about the player as they attempt
 * to complete it.
 * @param pl_name the name of the player
 * @param qu_info the quest name
 * @param details the information associated with the quest.
 */
void set_player_quest_info( string pl_name, string qu_info, mixed *details ) {
  get_data_file( pl_name );
  
  if(!player_cache[pl_name]->player_quest_info )
    player_cache[pl_name]->player_quest_info = ([ ]);
  player_cache[pl_name]->player_quest_info[qu_info] = details;
  save_data_file(pl_name);
}

/**
 * This method returns all the quest information for a player.
 * This information is used for quests which have several parts to them
 * and information needs to be stored about the player as they attempt
 * to complete it.  The keys of the mapping are the quest names and the
 * values are the information associated with the quest.
 * @param pl_name the name of the player
 * @return the mapping containing all the quest info
 */
mapping query_all_player_quest_info( string pl_name ) {
  if(!get_data_file( pl_name ))
    return 0;
  if ( !player_cache[pl_name]->player_quest_info )
    return 0;
  return copy(player_cache[pl_name]->player_quest_info);
}

/**
 * This method returns the quest info for a specific quest.
 * This information is used for quests which have several parts to them
 * and information needs to be stored about the player as they attempt
 * to complete it.
 * @param pl_name the player name
 * @param qu_info the quest name
 * @return the information associated with the quest
 */
mixed *query_player_quest_info( string pl_name, string qu_info ) {
  if(!get_data_file( pl_name))
    return 0;
  if(!player_cache[pl_name]->player_quest_info)
    return 0;
  player_cache[pl_name]->player_quest_info =
    player_cache[pl_name]->player_quest_info + ([ ]);
  return player_cache[pl_name]->player_quest_info[qu_info];
}

/**
 * This method returns the time at which a quest is completed.
 * @param name the name of the player to get the time for
 * @param qu_name the name of the quest
 * @return the time at which it is completed
 */
int query_quest_time(string name, string qu_name) {
  int time;
 
  if(!get_data_file(name)) {
    return 0;
  }
  time = member_array(qu_name, player_cache[name]->quests);
  if(time < 0) {
    return 0;
  }
  return player_cache[name]->times[time];
}

/**
 * This method returns the story associated with the player.   This is
 * created from all the information about the quests they have done
 * joined together into a neato story.
 * @param name the player name
 * @return the story of the player
 * @see /obj/handlers/quest_handler->query_quest_story()
 */
string query_story(string name) {
  string story, *quests;
  int loop;
 
  if(!get_data_file(name)) {
    return "You can find nothing in the library on " + name + "\n";
  }
  story = "";
  if(player_cache[name]->text_file != "") {
    return unguarded((: read_file, text_file :));
  }
  quests = player_cache[name]->quests;
  if(!sizeof(quests)) {
    return "That person has lead a most unadventureous life";
  }
  for(loop = 0; loop < sizeof(quests); loop++) {
    story += capitalize(QUEST_HANDLER->query_quest_story(quests[loop])) +".\n";
  }
  return story;
}

/**
 * This method tells us if the player has completed the quest.
 * @param player the name of the player
 * @param quest the quest name
 * @return 1 if the quest has been done, 0 if it has not
 */ 
int query_quest_done(string player, string quest) {
  if(!get_data_file(player))
    return 0;
  
  return (member_array(quest, player_cache[player]->quests) != -1);
} /* query_quest_done() */

int flush_cache (string name) {
  qps[name] = 0;
}

/**
 * This method returns the current number of quest points gathered
 * by the player.
 * @param name the player
 * @return the current number of quest pointds
 * @see /obj/handlers/query_handler->query_quest_level()
 */
int query_quest_points( string name ) {
   int points;
   string word;
   string *quests;

   // qps is used to cache the count of quest points since it's expensive and
   // queried a lot.
   // the reason for storing points+1 is so that someone with 0 points still
   // comes up true under the test if(qps[name])
   if(!qps)
     qps = ([ ]);
   
   if(qps[name])
     return qps[name]-1;
   
   get_data_file( name );
   quests = player_cache[name]->quests;
   if ( !quests || !sizeof( quests ) )
      points = 0;
   else {
     foreach( word in quests ) {
       points += (int)QUEST_HANDLER->query_quest_level( word );
     }
   }
   
   qps[name] = points+1;
   
   return points;
} /* query_quest_points() */

/**
 * This method is called when a player refreshes totaly so they can
 * start again from scratch.
 * @param name the player name
 * @return 0 if they do not exists, 1 if they do
 */
int restart(mixed name) {
  class qinfo tmp;

  if(objectp(name))
    name = name->query_name();

  if(!PLAYER_HANDLER->test_user(name))
    return 0;
  
  init_data(name);
  tmp = new(class qinfo,
            cached : time(),
            player_name : player_name,
            quests : quests,
            times : times,
            text_file : text_file,
            player_quest_info : player_quest_info);
  player_cache[name] = tmp;
  save_data_file(name);
  return 1;
} /* restart() */

mixed *stats() {
  int percentage;

  if(requests)
    percentage = (cache_hits * 100) / requests;
  
  return ({
    ({ "cache size", sizeof(player_cache) }),
      ({ "requests", requests }),
      ({ "cache hits", cache_hits }),
      ({ "cache misses", requests - cache_hits }), 
      ({ "percentage hits", percentage }),
      });
}
 
