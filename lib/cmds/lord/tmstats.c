/*  -*- LPC -*-  */

inherit "/cmds/base";

#include <tasks.h>

#define STATS_DIR "/save/tasks/"

#define DIVISOR 100

int do_summary();

int cmd(string str) {
  string file, cmd, tmp;
  mapping awards, summary, obs;
  int lvl, i, total, files;

  return do_summary();

  if(!str)
    str="";
  tmp = "";
  
  obs = ([ ]);
  summary = ([ ]);
  foreach(file in get_dir(STATS_DIR+str+"*")) {
    file = file[0..sizeof(file)-3];

    files++;
    awards = TASKER->query_stats(file);
    foreach(lvl in keys(awards)) {
      foreach(cmd in keys(awards[lvl])) {
        total += awards[lvl][cmd];
        obs[cmd] += 1;
      }
    }
  }

  foreach(cmd in keys(obs)) {
    if(obs[cmd] > 100)
      i += 1;
  }
  printf("%s: Skills: %d, Total: %d: Ratio: %d, Objects: %d, "
         "Obs over 100: %d\n",
         str, files, total, total/files, sizeof(keys(obs)), i);

  return 1;
} /* cmd() */

int do_summary() {
  string skill;
  mapping sum, stats;
  int i;
  int tries, awards;
  
  stats = TASKER->query_stats("summary");
  sum = stats;
  foreach(skill in keys(sum)) {
    if("/std/skills"->query_skill(({ skill })) != skill)
      continue;
    printf("Skill: %s \n", skill);
    
    printf("  Level 0..49:       Tries: %d, Awards: %d, ",
           sum[skill][0][0], sum[skill][0][1]);
    if(sum[skill][0][1] == 0)
      printf("Ratio: ?\n");
    else
      printf("Ratio: 1/%d\n", sum[skill][0][0]/(sum[skill][0][1]));
    
    printf("  Level 50..149:     Tries: %d, Awards: %d, ",
           sum[skill][1][0], sum[skill][1][1]);
    if(sum[skill][1][1] == 0)
      printf("Ratio: ?\n");
    else
      printf("Ratio: 1/%d\n", sum[skill][1][0]/(sum[skill][1][1]));

    printf("  Level 150..299:    Tries: %d, Awards: %d, ",
           sum[skill][2][0], sum[skill][2][1]);
    if(sum[skill][2][1] == 0)
      printf("Ratio: ?\n");
    else
      printf("Ratio: 1/%d\n", sum[skill][2][0]/(sum[skill][2][1]));
    
    printf("  Level 300..599:    Tries: %d, Awards: %d, ",
           sum[skill][3][0], sum[skill][3][1]);
    if(sum[skill][3][1] == 0)
      printf("Ratio: ?\n");
    else
      printf("Ratio: 1/%d\n", sum[skill][3][0]/(sum[skill][3][1]));

    printf("  Level >= 600:      Tries: %d, Awards: %d, ",
           sum[skill][4][0], sum[skill][4][1]);
    if(sum[skill][4][1] == 0)
      printf("Ratio: ?\n");
    else
      printf("Ratio: 1/%d\n", sum[skill][4][0]/(sum[skill][4][1]));

    tries = 0;
    awards = 0;
    for(i=0; i<3; i++) {
      tries += sum[skill][i][0];
      awards += sum[skill][i][1];
    }
      
    printf("  Total:              Tries: %d, Awards: %d, ", tries, awards);
    if(!awards)
      printf("Ratio: ?\n");
    else
      printf("Ratio: 1/%d\n", tries/awards);
           
  }
  return 1;
}
