/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: guildl_ist.c,v 1.1 1998/01/06 05:25:41 ceres Exp $
 * $Log: guildl_ist.c,v $
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
inherit "/cmds/base";

int cmd(string arg) {
  object ob;
  mapping guilds;
  string guild, *fields;

  guilds = ([ ]);
  
  foreach(ob in users()) {
    if(ob->query_creator())
      continue;
    
    if(!ob->query_guild_ob()) {
      guilds["none"]++;
    } else {
      fields = explode((string)ob->query_guild_ob(), "/");
      guilds[fields[2]]++;
    }
  }

  write("Guild memberships:\n");
  foreach(guild in keys(guilds))
    printf("  %s: %d\n", guild, guilds[guild]);
  
  return 1;
}
