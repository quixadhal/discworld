inherit "/cmds/base";

int cmd(string str) {
  mapping stats;
  string obj, file;
  int total, num;
  
  stats = "/secure/master"->query_read_stats();

  if(str) {
    if(!stats[str])
      return notify_fail("No such file\n");
    foreach(file in keys(stats[str])) {
      if(stats[str][file] > 1)
        printf("%-60s %5d\n", file, stats[str][file]);
    }
    return 1;
  }
  printf("%-60s %5s %5s\n", "Object", "Total", "Files");
  foreach(obj in keys(stats)) {
    total = 0;
    num = 0;
    foreach(file in keys(stats[obj])) {
      num++;
      total += stats[obj][file];
    }
    if(total > 200)
      printf("%-60s %5d %5d\n", obj, total, num);
  }
  return 1;
}
