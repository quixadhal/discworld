#include "skills.h"
/* lists all of the players skills.
 * giving levels (default) or bonuses
 */
string rec_list(mixed *arr, int bonus, string path, int all, int lvl) {
  int i;
  string str;
 
  str = "";
  for (i=0;i<sizeof(arr);i+=SKILL_ARR_SIZE) {
    str += sprintf("%*'| 's%*'.'-s %4d %4d\n", (lvl-1)*2, "", 20-((lvl-1)*2),
                                             arr[i],
            (int)this_player()->query_skill(path[1..strlen(path)]+"."+arr[i]),
            (int)this_player()->query_skill_bonus(path[1..strlen(path)]+"."+arr[i]));
    if (sizeof(arr[i+SKILL_BIT]) && (all || 
   (int)this_player()->query_skill(path[1..strlen(path)]+"."+arr[i]) > 5*lvl))
      str += rec_list(arr[i+SKILL_BIT], bonus, path+"."+arr[i], all, lvl+1);
  }
  return str;
}
 
int skill_list(string str) {
  string *bits;
  int i;
  mixed *arr;
 
 
  bits = ({ });
  if (str) {
    bits = explode(str, " ");
    if (member_array("all", bits) == -1) {
      notify_fail("Usage: "+query_verb()+" <all>\n");
      return 0;
    }
  }
  printf("%#*-s\n", this_player()->query_cols(), rec_list(
                  (mixed *)SKILL_OB->query_skills(),
                  (member_array("bonus",bits)!=-1),
                  "", (member_array("all", bits) != -1), 1));
  return 1;
}
