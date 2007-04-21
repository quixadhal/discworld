/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: who.c,v 1.62 2003/07/25 18:11:56 pinkfish Exp $
 * 
 */
/* who command, trial out by Turrican for a commands daemon. */
#include <clubs.h>
#include <playtesters.h>
#include <config.h>

inherit "/cmds/base";

#include <player.h>

string who_string(int width, int cre, int verbose, string name);
string who_line(object ob, int cre, int width);

mapping _nationalities;

void create() {
  string file;
  object ob;
  
  ::create();

  _nationalities = ([ ]);
  foreach(file in get_dir("/std/nationality/*.c")) {
    ob = find_object("/std/nationality/" + file);
    if(ob && ob->query_name()) {
      _nationalities[ob->query_name()] = base_name(ob);
    }
  }
}

/**
 * This is used by the other who commands to do the object mapping
 * to figure out who to include in the list.
 * @param tarr the array to get the filtered list from
 * @param name the name of the guilds to filter on
 * @param no_cres do not include any creators
 * @return the array of filtered objects
 */
object *filter_users(object *tarr, string name, int no_cres)  {
  string *guilds;
  string guild;
  string start_guild;
  string *domains;
  object *ret_arr;
  object *arr;
  int not_tag;
  
  name = lower_case(name);
  name = replace(name, ({"assassins", "assassin", 
                           "priests", "priest",
                           "thieves",   "thief",    
                           "warriors",    "warrior",
     