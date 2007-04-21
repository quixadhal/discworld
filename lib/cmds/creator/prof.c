#include <housing.h>

inherit "/cmds/base";

#define MAX 500

int last_got;
mixed *data, *tot;

int total(mapping *info) {
  int ret;
  mapping item;
  
  foreach(item in info)
    ret += item["self"];
  return ret;
}

int cmd(string thing) {
  int i, j;
  mapping stuff;
  object ob;
  mixed foo;
  string ret;

  if(last_got < time() - 900) {
    data = map(objects((: !clonep($1) :)),
               (: ({ $1, function_profile($1) }) :));
    tot = allocate(sizeof(data));
    for(i=0; i<sizeof(data); i++) {
      tot[i] = ({ data[i][0], total(data[i][1]) });
    }
   
    for(i=0; i<24; i++) {
      reset_eval_cost();
      for (j = i; j < sizeof(tot); j++)
        if (tot[i][1] < tot[j][1]) {
          foo = tot[i];
          tot[i] = tot[j];
          tot[j] = foo;
        }
    }
    last_got = time();
  }

  ret = "";

  if(!thing) {
    // We could use sort_array() here, but that's an inefficient way
    // to get the top 10 when there are lots of objects.
    for (i = 0; i < 50 && i<sizeof(tot); i++) {
      if(tot[i][1])
        ret += sprintf("%60-O %i\n", tot[i][0], tot[i][1]);
    }
  } else {
    ob = find_object(thing);
    if(!ob)
      return notify_fail("No such object.\n");
    ret += sprintf("%30-s %10s %10s %10s\n", "function", "calls", "self", "children");
    i = 0;
    for(j=0; j<sizeof(data); j++)
      if(data[j][0] == ob) {
        i = 1;
        break;
      }

    if(!i)
      return notify_fail("No such object in data.\n");
    
    foreach(stuff in data[j][1]) {
      ret += sprintf("%30-s %10d %10d %10d\n", stuff["name"], stuff["calls"],
             stuff["self"], stuff["children"]);
    }
  }

  write("$P$Profile$P$" + ret);
  return 1;
}
