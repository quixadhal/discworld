inherit "std/object";
 
#define RESTORE_PATH  "save/library/"
#include "quest_handler.h"
 
string player_name, player_title, *quests, text_file;
int *times, *levels, *highest, quest_points;
 
void init_data(string pl_name) {
  player_name = pl_name;
  quests = ({ });
  times = ({ });
  levels = ({ });
  text_file = "";
  highest = ({ });
  player_title = 0;
  quest_points = 0;
}

int get_data_file(string name) {
  int success;

  if(player_name != name) {
    success = restore_object(RESTORE_PATH+name);
    if(!success) {
      init_data(name);
    }
    return success;
  }
  return 1;
}
 
void save_data_file(string name) {
  return save_object(RESTORE_PATH+name);
}

string query_title(string name) {
  get_data_file(name);
  return player_title;
}
 
string *query_quests(string name) {
  get_data_file(name);
  if(!quests) {
    return ({ });
  }
  return quests + ({ });
}

string get_highest_quest(string name) {
  int loop, highest;
  string quest_name;
 
  if(!get_data_file(name)) {
    return "Sorry";
  }
  if(!sizeof(quests)) {
    return "None";
  }
  for(highest = loop = 0; loop < sizeof(quests); loop++) {
    if(QUEST_HANDLER->query_quest_level(quests[loop]) > highest) {
      highest = (int)QUEST_HANDLER->query_quest_level(quests[loop]);
      quest_name = quests[loop];
    }
  }
  return quest_name;
}
 
int get_highest_quest_level(string name) {
  int highest;
  string qu_name;
 
  if(!get_data_file(name)) {
    return 0;
  }
  if(!sizeof(quests)) {
    return 0;
  }
  qu_name = get_highest_quest(name);
  if((highest = member_array(qu_name, quests)) == -1) {
    return 0;
  }
  return levels[highest];
}
 
string get_most_recent_quest(string name) {
  int loop, highest;
  string quest_name;
 
  if(!get_data_file(name)) {
    return "Sorry";
  }
  if(sizeof(quests) == 0) {
    return "None";
  }
  for(highest = loop = 0; loop < sizeof(quests); loop++) {
    if(times[loop] > highest) {
      highest = times[loop];
      quest_name = quests[loop];
    }
  }
  return quest_name;
}
 
int get_most_recent_time(string name) {
  int loop, hightime;
 
  if(!get_data_file(name)) {
    return -1;
  }
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
 
int get_highest_quest_time(string name) {
  int hightime;
  string temp;
 
  if(!get_data_file(name)) {
    return -1;
  }
  if(sizeof(quests) == 0) {
    return 0;
  }
  temp = get_highest_quest(name);
  hightime = member_array(temp, quests);
  return times[hightime];
}
 
int new_top_quest();

int set_quest(string pl_name, string qu_name) {
  int qu_level, pos, tmp;
  object ob;

  get_data_file(pl_name);
  if(member_array(qu_name, quests) != -1) {
    return 0;
  }
  quests += ({ qu_name });
  times += ({ time() });
  qu_level = (int)QUEST_HANDLER->query_quest_level(qu_name);
  quest_points += qu_level;
  levels += ({ qu_level });
  if(new_top_quest()) {
    highest += ({ member_array(qu_name, quests) });
  }
  else {
    int smallest, loop;
 
    for(smallest = 9999, loop = 0; loop < sizeof(quests); loop++) {
      if(smallest > levels[loop] && (tmp = member_array(loop, highest)) != -1) {
        smallest = levels[loop];
        pos = tmp;
      }
    }
    if(qu_level > smallest) {
      highest[pos] = sizeof(quests) - 1;
    }
  }
  QUEST_HANDLER->quest_completed(pl_name, qu_name, previous_object());
  if(qu_level >= get_highest_quest_level(pl_name)) {
    player_title = (string)QUEST_HANDLER->query_quest_title(qu_name);
  }
  seteuid((string)"secure/master"->get_root_uid());
  save_data_file(pl_name);
  seteuid("Room");
  if(ob = find_player(pl_name)) {
    ob->set_title(player_title);
  }
  return 1;
}
 
int query_quest_time(string pl_name, string qu_name) {
  int time;
 
  if(!get_data_file(name)) {
    return 0;
  }
  time = member_array(qu_name, quests);
  if(time < 0) {
    return 0;
  }
  return times[time];
}
 
int new_top_quest() {
  if(!sizeof(highest)) {
    return 1;
  }
  if(sizeof(highest) == 2) {
    return 1;
  }
  if(sizeof(highest) == 4) {
    return 1;
  }
  if(sizeof(highest) == 6) {
    return 1;
  }
  if(sizeof(highest) == 8) {
    return 1;
  }
  if(!(sizeof(highest) % 10)) {
    return 1;
  }
  return 0;
}
 
string query_story(string name) {
  string story, *qu_names;
  int loop, how_many;
 
  if(!get_data_file(name)) {
    return "You can find nothing in the library on " + name + "\n";
  }
  story = "";
  if(text_file != "") {
    return read_file(text_file);
  }
  if(!sizeof(quests)) {
    return "That person has lead a most unadventureous life";
  }
  for(loop = 0; loop < sizeof(highest); loop++) {
    story += capitalize(player_name) +
             QUEST_HANDLER->query_quest_story(quests[highest[loop]]) + "\n";
  }
  return story;
}
 
void set_title(string who, string what) {
  if (!"/secure/login"->test_user(who))
    return ;
  get_data_file(who);
  player_title = what;
  save_data_file(who);
}

int query_quest_done(string player, string quest) {
  get_data_file(player);
  return (member_array(quest, quests) != -1);
}

int query_quest_points(string name) {
  get_data_file(name);
  return quest_points;
}
