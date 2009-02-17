/*
  INCLUDE  : Quest Info Utils
  Author   : Dek
  Date     : March 2001
  Function : General inheritable for use by anything really.
             Using quest_info stuff is a bit of a pain. This
             is just syntactic sugar to make it a lot more
             friendly.


Quest info is saved as a mapping, so you can index any info you like
under named keys.

Functions:
----------
set_quest_name(string quest_name)
   sets the name of the quest this item applies to.

void set_quest_param(mixed player, string key, mixed value)
   Sets the quest info value addressed by <key> to <value>
   Value can be of any type.

mixed query_quest_param(mixed player, string key)
   gets the quest info value addressed by <key>

void clear_quest_info(mixed player)
   Clears all the quest info associated with <player>
   for the current quest


*/

#include <library.h>

string _quest_name;

// call this function in setup. I'm making the assumption that one
// item only deals with one quest, but it's a fairly safe assumption.
void set_quest_name(string quest_name)

{
   _quest_name = quest_name;
}


void set_quest_param(mixed player, string key, mixed value)

{
   mixed info;
   
   if (!stringp(player))
      player = player->query_name();
   
   info = LIBRARY->query_player_quest_info(player, _quest_name);
   
   if (!mapp(info))
      info = ([ ]);
   
   info[key] = value;

   LIBRARY->set_player_quest_info(player, _quest_name, info);
}


mixed query_quest_param(mixed player, string key)

{
   mixed info;

   if (!stringp(player))
      player = player->query_name();

   info = LIBRARY->query_player_quest_info(player, _quest_name);

   if (!mapp(info))
      return 0;
      
   return info[key];
}


void clear_quest_info(mixed player)

{
   if (!stringp(player))
      player = player->query_name();

   LIBRARY->set_player_quest_info(player, _quest_name, 0);
}