#include "skills.h"
/* lists all of the players skills.
 * giving levels (default) or bonuses
 */
string rec_list(mapping sks, string path, int all, int lvl, int n_rec) {
  int i, sk;
  string str, tp;
  string *sks_i;
 
  str = "";
  sks_i = m_indices(sks);
  for (i=0; i<sizeof(sks_i); i++) {
    tp = path + "." + sks_i[i];
    sk = (int)this_player()->query_skill(tp);
    str += sprintf("%*'| 's%*'.'-s %4d %4d\n", (lvl-1)*2, "", 20-((lvl-1)*2),
                   sks_i[i], sk, (int)this_player()->query_skill_bonus(tp));
    if (sizeof(sks[sks_i[i]][SKILL_BIT]) && (all || sk > 5*lvl) && !n_rec)
      str += rec_list(sks[sks_i[i]][SKILL_BIT], tp, all, lvl+1, 1);
  }
  return str;
} /* rec_list() */
 
int skill_list(string str) {
  string *bits;
  mapping sks;
 
  bits = ({ });
  if (str) {
    sks = (mapping)SKILL_OB->query_skills();
    if (!sks[str]) {
      notify_fail("Usage: "+query_verb()+" <skill>\n");
      return 0;
    }
    sks = sks[str][SKILL_BIT];
    printf("%'='*-s\n", this_player()->query_cols(),
           "=======SKILLS=======Level/Bonus");
    printf("%#*-s\n", this_player()->query_cols(), rec_list(sks, str, 1, 1, 0));
    return 1;
  } else
    sks = (mapping)SKILL_OB->query_skills();
  printf("%'='*-s\n", this_player()->query_cols(),
         "=======SKILLS=======Level/Bonus");
  printf("%#*-s\n", this_player()->query_cols(),
                    rec_list(sks, "", (member_array("all", bits) != -1), 1, 1));
  return 1;
} /* skill_list() */
