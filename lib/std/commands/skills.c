#include "skills.h"
/* lists all of the players skills.
 * giving levels (default) or bonuses
 */
string rec_list(mixed *arr, string path, int all, int lvl) {
  int i, sk;
  string str, tp;
 
  str = "";
  for (i=0;i<sizeof(arr);i+=SKILL_ARR_SIZE) {
    str += sprintf("%*'| 's%*'.'-s %4d %4d\n", (lvl-1)*2, "", 20-((lvl-1)*2),
                                             arr[i],
                   (sk=(int)this_player()->query_skill((tp=(path+"."+arr[i])))),
                   (int)this_player()->query_skill_bonus(tp));
    if (sizeof(arr[i+SKILL_BIT]) && (all || sk > 5*lvl))
      str += rec_list(arr[i+SKILL_BIT], path+"."+arr[i], all, lvl+1);
  }
  return str;
}
 
int skill_list(string str) {
  string *bits;
  int i;
  mixed *arr;
 
 
  bits = ({ });
  if (str) {
    arr = (mixed *)SKILL_OB->query_skills();
    if ((i=member_array(str, arr)) == -1) {
      notify_fail("Usage: "+query_verb()+" <skill>\n");
      return 0;
    }
    arr = arr[i+SKILL_BIT];
    printf("%'='*-s\n", this_player()->query_cols(),
           "=======SKILLS=======Level/Bonus");
    printf("%#*-s\n", this_player()->query_cols(), rec_list(
                  arr, str, 1, 1));
    return 1;
  } else
    arr = (mixed *)SKILL_OB->query_skills();
  printf("%'='*-s\n", this_player()->query_cols(),
         "=======SKILLS=======Level/Bonus");
  printf("%#*-s\n", this_player()->query_cols(), rec_list(
                  arr, "", (member_array("all", bits) != -1), 1));
  return 1;
}
