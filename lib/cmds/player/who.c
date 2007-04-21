/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: who.c,v 1.58 2003/03/03 00:58:25 pinkfish Exp $
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
    if(ob && ob->query_name())
      _nationalities[ob->query_name()] = base_name(ob);
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
                           "witches",   "witch",    
                           "wizards",     "wizard",
                           "creators",  "creator",  
                           "liaisons", "liaison",
                           "adventurers", "adventurer",
                           "fighters", "warrior", 
                           "fighter", "warrior",
                           "killers", "killer", 
                           "helpers", "helper",
                           "playtesters", "playtester", 
                           "friends", "friend",
                           "families", "family" }));
  guilds = explode(name, "&") - ({ "" });
  if (sizeof(guilds) > 1) {
    arr = filter_users(tarr, guilds[0], no_cres);
    foreach (guild in guilds[1..]) {
      arr &= filter_users(tarr, guild, no_cres);
    }
    return arr;
  }
  guilds = explode(name, ",") - ({ "", 0 });

  domains = "/secure/master"->query_domains();
  
  ret_arr = ({ });

  foreach (guild in guilds)  {
    start_guild = guild;
    guild = replace(guild, " ", "", "\t", "");
    if (guild[0] == '-') {
       not_tag = 1;
       guild = guild[1..];
    } else {
       not_tag = 0;
    }
    arr = ({ });
    if (guild == "adventurer")  {
      arr += filter(tarr, (: !$1->query_guild_ob()  &&
                           (!$(no_cres) || !$1->query_creator()) :));
    } else if (guild == "friend")  {
      arr += filter(tarr, (: !$1->query_login_ob() && this_player()->is_friend($1->query_name()) :));
    } else if (guild == "creator")  {
      arr += filter(tarr, (: $1->query_creator()  &&
                           $1->query_visible(this_player()) :) );
    } else if (guild == "helper") {
      arr += filter( tarr, (: !$1->query_creator() &&
                            $1->query_property( "newbie helper" ) :) );
    } else if (guild == "killer") {
      arr += filter(tarr, (: $1->query_player_killer() &&
                           (!$(no_cres) || !$1->query_creator()) :));
    } else if (guild == "family") {
      arr += filter(tarr, (: $1->query_family_name() :));
    } else if (guild == "playtester") {
      arr += filter(tarr, 
                    (: PLAYTESTER_HAND->query_playtester($1->query_name()) :));
    } else if (member_array(guild, domains) > -1)  {
      arr += filter(tarr, (: $1->query_creator()  &&
                           ("/d/"+$(guild)+"/master")->query_member($1->query_name())  &&
                           $1->query_visible(this_player()) :));
    } else if(member_array(guild, keys(_nationalities)) > -1) {
      arr += filter(tarr, (: $1->query_nationality() == $2 :),
                    _nationalities[guild]);
    } else  {
      if (file_size("/std/guilds/" + guild + ".c") > 0)  {
        arr += filter(tarr,
                      (: (!$(no_cres) || !$1->query_creator()) &&
                       stringp($1->query_guild_ob()) &&
                       $1->query_guild_ob() == "/std/guilds/" + $(guild) :));
      } else if (CLUB_HANDLER->is_family(start_guild)) {
        arr += filter(tarr, (: CLUB_HANDLER->is_member_of($(start_guild), $1) :));
      } else if (CLUB_HANDLER->is_club(start_guild)) {
        if (!CLUB_HANDLER->query_club_secret(start_guild) ||
            CLUB_HANDLER->is_member_of(start_guild, this_player()->query_name())) {
          arr += filter(tarr, (: CLUB_HANDLER->is_member_of($(start_guild), $1->query_name()) :));
        }
      } else {
        arr += filter(tarr, (: $1->query_deity() == $2 && 
                             (!$(no_cres) || !$1->query_creator()) :),
                      guild);
      }
    }
    if (not_tag) {
       ret_arr |= (tarr - arr);
    } else {
       ret_arr |= arr;
    }
  }

  return ret_arr;
} /* filter_users() */

int cmd(string str, int verbose)  {
  this_player()->more_string( who_string(this_player()->query_cols(),
                                         this_player()->query_creator(),
                                         verbose,
                                         str),
                              "Who", 1 );
  return 1;
} /* cmd() */


/**
 * This method returns the string used in the who command.
 * @return the line for the who line
 */
string who_string(int width, int cre, int verbose, string name) {
  object *arr;
  int number;
  string tmp;
  string tmp2;
  string prt;
  int x;
  
  if (name == "here") {
    arr = filter(all_inventory(environment(this_player())), (: userp($1) &&
                                                             $1->query_visible(this_player()) :) );
  } else {
    arr = filter(users(), (: strsrch(file_name($1), "secure/login") == -1 :));
    if (name && sizeof(arr))  {
      arr = filter_users(arr, name, 1);
    }
  }
  
  number = sizeof(arr);
  
  if (number == 0)  {
    if (name)  {
      return "There are no guild(s) or members of " + name + " online.\n";
    }
    return "There is no one on " + mud_name() + "?\n";
  }
  
  prt = sprintf("%|*'-'s\n", width-1, "==========]  " + mud_name() + "  [===========");
  arr = sort_array(arr, (: strcmp($1->query_name(), $2->query_name() ) :) );
  
  tmp2 = "";
  if(!name && verbose == 0) {
    int i;
    
    // remove all creators except liaisons.
#ifndef __DISTRIBUTION_LIB__
    arr = filter_array(arr, (: !$1->query_creator() ||
                             "/d/liaison/master"->query_member($1->query_name()) :) );
#endif
    
    for (i = 0; i < sizeof(arr); i++)  {
      tmp = (string)arr[i]->query_cap_name();
      if(!tmp) {
        --number;
      } else {
        x = 14;
        
        if (arr[i]->query_creator()) {
          if ("/secure/master"->query_trustee(arr[i]->query_name())) {
            tmp += " (%^RED%^T";
          } else if("/secure/master"->query_director(arr[i]->query_name())){
            tmp += " (%^RED%^D";
          } else if("/secure/master"->query_senior(arr[i]->query_name())){
            tmp += " (%^RED%^S";
          } else  {
            tmp += " (%^RED%^C";
          }
          x += 16;
          
          if ( "/d/liaison/master"->query_member(arr[i]->query_name())) {
            tmp += "%^YELLOW%^l";
            x += 10;
          }
          if(this_player()->is_friend(arr[i]->query_name())) {
            tmp += "%^GREEN%^F";
            x += 9;
          }
          tmp += "%^RESET%^)";
          
        } else if(this_player()->is_friend(arr[i]->query_name())) {
          tmp += " (%^GREEN%^F%^RESET%^)";
          x += 18;
        }

        tmp2 += sprintf("%-" + x + "s ", tmp);
        
        if((i+1) % (width/15) == 0  && tmp2 != "") {
          prt += this_player()->fix_string(" " + tmp2 + "\n", width, 10);
          tmp2 = "";
        }
      }
    }
    prt += this_player()->fix_string(" " + tmp2 + "\n", width, 10);
  } else {
    int i;
    
    for (i = 0; i < sizeof(arr); i++)  {
      tmp = who_line(arr[i], cre, width);
      if ( !tmp )  {
        --number;
      } else  {
        prt += this_player()->fix_string(tmp + "\n", width, 10);
      }
    }
  }
  
  if ( name == "here" )  {
    if (number == 1)
      tmp = "> You are all by yourself. <";
    else
      tmp = "> There are " + query_num(number, 500) + " people here. <";
  } else if ( name )  {
    tmp = "> There " + (number > 1 ? "are " : "is ") +
      query_num(number, 500) +
      (number > 1 ? " members of" : " member of");
    if (strsrch(name, ",") != -1) {
      tmp += " those guilds on the " + mud_name() + ". <";
    } else {
      tmp += " this guild on the " + mud_name() + ". <";
    }
  } else if (number < 2)  {
    tmp = "> You are all alone on the " + mud_name() + ". <";
  } else  {
    tmp = "> There are " + query_num(number, 500) +
      " mudders on the " + mud_name() + ". <";
  }
  prt += sprintf("%*'-'|s\n", width-1, tmp);
  return prt;
}

/**
 * This method returns the line used for each player in the who
 * command.
 * @return the who line
 * @param ob the player to show
 * @param cre are we a cre
 * @param width the width of the line
 */
string who_line(object ob, int cre, int width)  {
  string s;
  string tmp;

  tmp = ob->query_player_title();
  if (tmp) {
    tmp += " ";
  } else {
    tmp = "";
  }
  s = tmp + ob->query_cap_name();
  if ( !s )  {
    return 0;
  }
  tmp = ob->query_family_name();
  if (tmp) {
    s += " " + tmp;
  }

  if ( "/d/liaison/master"->query_member( (string)ob->query_name() ) )  {
    s += " (%^YELLOW%^Liaison%^RESET%^)";
  }
  if (ob->query_creator()) {
    if ("/secure/master"->query_trustee((string)ob->query_name()))  {
      s += " (%^RED%^Trustee%^RESET%^)";
    } else if("/secure/master"->query_director((string)ob->query_name())){
      s += " (%^RED%^Director%^RESET%^)";
    } else if("/secure/master"->query_senior((string)ob->query_name())) {
      s += " (%^RED%^Senior%^RESET%^)";
    } else  {
      s += " (%^RED%^Creator%^RESET%^)";
    }
    if (ob->query_invis() == 1) {
      s += " (Invisible)";
    } if (ob->query_invis() > 1) {
      s += " (Super Invisible)";
    }
  }
    

  if (cre)  {
    if ((tmp = (string)ob->query_in_editor()))  {
      s += " (editing: " + tmp + ")";
    }
  }
  if (ob->query_property("guest"))  {
    s += " guest of " + mud_name() + "";
  } else if (tmp = (string)ob->query_gtitle())  {
    s += " " + tmp;
  } else  {
    s += " the Adventurer";
  }
  if (tmp = (string)ob->query_property("player_title"))  {
    s += ", " + tmp;
  }
  tmp = (string)ob->query_title();
  if ( tmp && ( tmp != "" ) )  {
    s += ", " + tmp;
  }


  if ( PLAYTESTER_HAND->query_playtester( ob->query_name() ) ) {
    if ( PLAYTESTER_HAND->query_senior_playtester( ob->query_name() ) ) {
      s += ", (%^BOLD%^%^CYAN%^Senior Playtester%^RESET%^)";
    }
    else {
      s += ", (%^CYAN%^Playtester%^RESET%^)";
    }
  }
  if (interactive(ob))  {
    if (query_idle(ob) > 120)  {
      s += " (Idle: " + (query_idle(ob) / 60) + ")";
    }
  }
  return s + "%^RESET%^";
} /* who_line() */

mixed *query_patterns() {
  return ({ "", (: cmd(0, 0) :),
              "verbose", (: cmd(0, 1) :),
              "here", (: cmd("here", 1) :),
              "<string'guild|liaisons|creators|killers|helpers|playtesters|friends|families|nationality'>", (: cmd( $4[0], 0 ) :) });
} /* query_patterns() */
