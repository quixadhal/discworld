#ifndef _COMMAND_H
#define _COMMAND_H

#define CMD_D "/secure/command"
#define DIR_PLAYER_CMDS "/cmds/player"
#define DIR_SECURE_PLAYER_CMDS "/secure/cmds/player"
#define DIR_LIVING_CMDS "/cmds/living"
#define DIR_CREATOR_CMDS "/cmds/creator"
#define DIR_SECURE_CREATOR_CMDS "/secure/cmds/creator"
#define DIR_LORD_CMDS "/cmds/lord"
#define DIR_SECURE_LORD_CMDS "/secure/cmds/lord"
#define DIR_GUILD_CMDS "/cmds/guild-race"
#define DIR_PLAYTESTER_CMDS "/cmds/playtester"
#define DIR_SENIOR_PLAYTESTER_CMDS "/cmds/playtester/senior"
#define DIR_EXEC_PLAYTESTER_CMDS "/cmds/playtester/exec"

class command {
  string verb;
  string file;
  string args;
  string filepart;
}

class command_class {
    mapping patterns;
    string* order;
}

class command_data {
    int weight;
    mixed* calls;
}

#endif
