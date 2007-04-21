inherit "/cmds/base";

#define TP this_player()
#define STATS ({"con", "dex", "int", "str", "wis"})

#define GUILDS ({"assassin", "priest", "thief", "warrior", "witch", "wizard"})
#define TREES ({"covert", "crafts", "faith", "fighting", "magic", "other" })

string get_stat(string s) {
  switch(s) {
  case "S": return "str";
  case "I": return "int";
  case "W": return "wis";
  case "D": return "dex";
  case "C": return "con";
  }
}

int cmd() {
  object user;
  string guild_ob, stat, str;
  mapping stats;
  int val, m, i;
  mapping tmp;
  mapping mode;
  
  stats = ([ ]);
  
  foreach(user in users()) {
    if(user->query_creator() || user->query_known_command("rearrange"))
      continue;
    
    tmp = ([]);
    guild_ob = user->query_guild_ob();

    if(guild_ob)
      guild_ob = guild_ob->query_name();
    
    if(stats[guild_ob])
      tmp = stats[guild_ob];
    else {
      foreach(stat in STATS) {
        tmp[stat] = ({ 0, 0, 0, 0 });
      }
    }

    if(!mode)
      mode = ([ ]);
    if(!mode[guild_ob])
      mode[guild_ob] = ([ ]);

    foreach(stat in STATS) {
      val = call_other(user, "query_real_" + stat);
      tmp[stat][0] += 1;
      tmp[stat][1] += val;
      if(tmp[stat][2] > val || !tmp[stat][2]) {
        tmp[stat][2] = val;
      }
      if(tmp[stat][3] < val) {
        tmp[stat][3] = val;
      }
      
      if(!mode[guild_ob][stat])
        mode[guild_ob][stat] = ([ val : 1 ]);
      else if(!mode[guild_ob][stat][val])
        mode[guild_ob][stat][val] = 1;
      else
        mode[guild_ob][stat][val] += 1;
    } 
    stats[guild_ob] = tmp;
  }

  write("stat: min max mode ave\n");
  foreach(guild_ob in keys(stats)) {
    if(!guild_ob)
      continue;

    printf("\n%s:\n", guild_ob);
    str = "";
    foreach(stat in STATS) {

      // Find the mode.
      m = 0;
      foreach(i in keys(mode[guild_ob][stat]))
        if(mode[guild_ob][stat][i] > m) {
          val = i;
          m = mode[guild_ob][stat][i];
        }

      str += sprintf("  %s: %2d %2d %2d %2d\n",
                     stat,
                     stats[guild_ob][stat][2], stats[guild_ob][stat][3],
                     val, stats[guild_ob][stat][1] / stats[guild_ob][stat][0]);
    }
    printf("%-#*s", this_player()->query_cols(), str);
  }

  printf("\n");
  return 1;
}

int guild(string gstr) {
  string user, stat, gob, tstr, guild;
  mapping count, total;
  int val, i;

  foreach(guild in GUILDS) {
    if(gstr && gstr != guild && gstr != guild+"s" &&
       gstr != guild+"es")
      continue;

    gob = "/std/guilds/" + guild;
    count = ([ ]);
    total = ([ ]);
    foreach(user in users()) {
      if(user->query_guild_ob() != gob || user->query_creator() ||
         user->query_known_command("rearrange"))
        continue;
      
      foreach(stat in STATS) {
        total[stat] += 1;
        val = call_other(user, "query_real_" + stat);
        if(!count[stat])
          count[stat] = ([ val : 1 ]);
        if(!count[stat][val])
          count[stat][val] = 1;
        else
          count[stat][val] += 1;
      }
    }
    
    if(!sizeof(count))
      return notify_fail("No players in that guild.\n");
    
    write("\nStats for " + guild + "\n");
    
    foreach(stat in STATS)
      printf("%-13s", "   " + capitalize(stat));
    write("\n");
    
    for(val = 8; val <= 23; val++) {
      foreach(stat in STATS) {
        tstr = "";
        for(i=0; i<((count[stat][val] * 10) / total[stat]); i++)
          tstr += "#";
        printf("%2d %-10s", val, tstr);
      }
      write("\n");
    }
  }
  return 1;
}

int stats(string sstr) {
  string user, guild, tstr, st;
  mapping count, total;
  int val, i;

  foreach(st in STATS) {
    
    if(sstr && st[0] != sstr[0])
      continue;
    count = ([ ]);
    total = ([ ]);
    
    foreach(user in users()) {
      if(user->query_creator() || user->query_known_command("rearrange"))
        continue;
      
      guild = replace(user->query_guild_ob(), "/std/guilds/", "");
      if(!guild)
        continue;
      
      val = call_other(user, "query_real_" + st);
      if(!count[guild])
        count[guild] = ([ val : 1 ]);
      else
        count[guild][val] += 1;
      total[guild] += 1;
    }
    
    if(!sizeof(count))
      return notify_fail("No players in that guild.\n");
  
    write("\nValues for " + capitalize(st) + "\n");
    foreach(guild in GUILDS)
      printf("%-13s", "   " + capitalize(guild));
    write("\n");
    
    for(val = 8; val <= 23; val++) {
      foreach(guild in GUILDS) {
        tstr = "";
        for(i=0; i<((count[guild][val] * 10) / total[guild]); i++) {
          tstr += "#";
        }
        printf("%2d %-10s", val, tstr);
      }
      write("\n");
    }
  }
  
  return 1;
}

int primaries() {
  string guild, skill, st, stats;
  mapping count;
  int i, total;
  string str;

  write("Guild Primaries\n");
  
  foreach(guild in GUILDS) {
    write(guild + "\n");
    count = ([ ]);
    total = 0;
    str = "";
    
    foreach(skill in ("/std/guilds/" + guild)->query_skills()) {
      stats = "/std/skills"->query_skill_stat(skill);
      total += sizeof(stats);
      for(i=0; i<sizeof(stats); i++) {
        st = get_stat(stats[i..i]);
        count[st] += 1;
      }
    }
    foreach(st in STATS) {
      str += sprintf("  %s: %2d%%\n",
                     capitalize(st), (count[st] * 100) / total);
    }
    printf("%-#*s", this_player()->query_cols(), str);
    write("\n");
  }

  return 1;
}

int skills() {
  string tree, skill, st, stats;
  mapping count;
  int i, total;
  string str;

  write("Guild Primaries\n");
  
  foreach(tree in TREES) {
    write(tree + "\n");
    count = ([ ]);
    total = 0;
    str = "";
    foreach(skill in ("/std/skills")->query_all_children(tree)) {
      stats = "/std/skills"->query_skill_stat(skill);
      total += sizeof(stats);
      for(i=0; i<sizeof(stats); i++) {
        st = get_stat(stats[i..i]);
        count[st] = 1;
      }
    }
    foreach(st in STATS) {
      str += sprintf("  %s: %2d%%\n",
                     capitalize(st), (count[st] * 100) / total);
    }
    printf("%-#*s", this_player()->query_cols(), str);
    write("\n");
  }
  
  return 1;
}

mixed *query_patterns() {
   return ({ "", (: cmd() :),
             "guild", (: guild(0) :),
             "guild <word'guild'>", (: guild($4[0]) :),
             "stat", (: stats(0) :),
             "stat <word'guild'>", (: stats($4[0]) :),
             "primaries", (: primaries() :),
             "skills", (: skills() :) });
} /* query_patterns() */
