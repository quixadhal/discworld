#include "skills.h"

int bonus;

varargs void display_hieracy(mixed *hieracy, string path, int indent) {
  int i;
  string new_path;

  for (i=0; i<sizeof(hieracy); i += SKILL_ARR_SIZE) {
    new_path = path + "." + hieracy[i+SKILL_NAM];
    printf("%"+indent+"s%-"+(20-indent)+"s %5d\n","",hieracy[i+SKILL_NAM],(bonus)
           ?(int)this_player()->query_skill_bonus(new_path)
           :(int)this_player()->query_skill(new_path)
    );
    display_hieracy(hieracy[i+SKILL_BIT], new_path, indent+2);
  }
}

int skill_list(string str) {
  int i, j;
  string *args;
  mixed *tree;

  if (!str || str == "") args = ({ }); else args = explode(str, " ");
  tree = (mixed *)this_player()->query_skills();
  if ((i = member_array("bonus", args)) != -1) {
    args = delete(args, i, 1);
    bonus = 1;
  } else bonus = 0;
  if (sizeof(args)) {
    if (sizeof(args) > 1) {
      notify_fail("USAGE: skills [bonus] [all|hieracy.name]\n");
      return 0;
    }
    if (args[0] != "all") {
      args = explode(args[0], ".");
      for (i=0; i<sizeof(args); i++) {
        if ((j = member_array(args[i], tree)) == -1) {
          notify_fail("Invalid leaf: "+args[i]+"\n");
          return 0;
        }
        if (i == sizeof(args)-1) {
          str = implode(args, ".");
          printf("%-20s %5d\n", str, (bonus)
                 ?(int)this_player()->query_skill_bonus(str)
                 :(int)this_player()->query_skill(str)
          );
        }
        tree = tree[j+SKILL_BIT];
      }
      display_hieracy(tree, str, 2);
      return 1;
    }
    display_hieracy(tree, "");
    return 1;
  }
  for (i=0; i<sizeof(tree); i += SKILL_ARR_SIZE) {
    printf("%-20s %5d\n", tree[i+SKILL_NAM], (bonus)
           ?(int)this_player()->query_skill_bonus(tree[i+SKILL_NAM])
           :(int)this_player()->query_skill(tree[i+SKILL_NAM])
    );
  }
  return 1;
}
