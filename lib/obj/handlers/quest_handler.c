/*	/obj/handlers/quest_handler.c

    Change log:

	12/02/92 - Modified by Godot to add function prototypes and comments.

*/
inherit "std/object";

/*
    Global variables.
*/
string *quest_name;    /* quest names */
int *quest_level;      /* quest difficulty rating */
string *quest_title;    /* quest title for players */
string *quest_story;    /* string to be tacked onto players name for book */
string *last_done_by;   /* last person to do this quest */
int *num_times_done;    /* number of times quest completed */

#include "library.h"
#define QUEST_LOG "QUESTS"
#define SAVE_FILE "save/quests"
/*
    Function prototypes.
/*
int add_quest(string name, int level, string title, string story);
void create();
int delete_quest(string name);
void load_quests();
int query_quest_level(string name);
int *query_quest_levels();
string *query_quest_names();
string *query_quest_stories();
string query_quest_story(string name);
string query_quest_title(string name);
string *query_quest_titles();
void quest_completed(string name, int quest, object prev_ob);
void save_quests();

/*
    Functions.
*/
void load_quests() {
  restore_object(SAVE_FILE);
}

void save_quests() {
  seteuid((string)"secure/master"->get_root_uid());
  save_object(SAVE_FILE);
  seteuid("Room");
}

void create() {
  ::create();
  load_quests();
  if(!quest_name) {
    quest_name = ({ });
  }
  if(!quest_level) {
    quest_level = ({ });
  }
  if(!quest_title) {
    quest_title = ({ });
  }
  if(!quest_story) {
    quest_story = ({ });
  }
  if(!last_done_by) {
    last_done_by = ({ });
  }
  if(!num_times_done) {
    num_times_done = ({ });
  }
}

int add_quest(string name, int level, string title, string story) {
/*
	This function adds a new quest so we can make life tougher
	for players.
*/
  int temp;

  temp = member_array(name, quest_name);
  if(temp != -1) {
    return 0;
  }
  quest_name += ({ name });
  quest_level += ({ level });
  quest_title += ({ title });
  quest_story += ({ story });
  last_done_by += ({ "Nobody" });
  num_times_done += ({ 0 });
  log_file(QUEST_LOG, this_player()->query_name() + " added : " + name +
           ", " + level + ", " + title + ", " + story + "\n\n");
  save_quests();
  return 1;
}

int query_quest_level(string name) {
  int temp;

  temp = member_array(name, quest_name);
  if(temp == -1) {
    return -1;
  }
  return quest_level[temp];
}

string query_quest_story(string name) {
  int temp;

  temp = member_array(name, quest_name);
  if(temp == -1) {
    return "did nothing";
  }
  return quest_story[temp];
}

string query_quest_title(string name) {
  int temp;

  temp = member_array(name, quest_name);
  if(temp == -1 || quest_title[temp] == "") {
    return "the adventurer";
  }
  return quest_title[temp];
}

int delete_quest(string name) {
  int temp;

  log_file(QUEST_LOG, this_player()->query_name() + " removed : " +
                      name + "\n\n");
  temp = member_array(name, quest_name);
  if(temp == -1) {
    return 0;
  }
  quest_name = delete(quest_name, temp, 1);
  quest_level = delete(quest_level, temp, 1);
  quest_title = delete(quest_title, temp, 1);
  quest_story = delete(quest_story, temp, 1);
  save_quests();
  return 1;
}

string *query_quest_names() {
  return quest_name + ({ });
}

int *query_quest_levels() {
  return quest_level + ({ });
}

string *query_quest_titles() {
  return quest_title + ({ });
}

string *query_quest_stories() {
  return quest_story + ({ });
}

void quest_completed(string name, int quest, object prev_ob) {
  int temp;
  string prev_ob_name;

  if(file_name(previous_object()) == LIBRARY) {
    prev_ob_name = file_name(prev_ob);
  }
  else {
    prev_ob = previous_object();
    prev_ob_name = file_name(previous_object());
  }
  log_file(QUEST_LOG, ctime(time()) + " " + name + " completed " + quest + "\n");
  temp = member_array(quest, quest_name);
  log_file(QUEST_LOG, "Quest set by " + prev_ob_name +
                      " which is " + prev_ob->query_name() + "\n");
  if(temp == -1) {
    log_file(QUEST_LOG, "Non existant quest\n");
    return;
  }
  last_done_by[temp] = name;
  num_times_done[temp]++;
}

/* EOF */
