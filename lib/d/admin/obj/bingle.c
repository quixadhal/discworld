inherit "/std/object";

void setup() {
  set_name("domainer");
  set_long(
"This object shows you all of the creators who are in each domain and those "+
"that don't have directorys, are not cretors and ones who are not in a "+
"domain at all.\nThe command to view all this is \"info\".\n");
} /* setup() */

void init() {
  add_action("do_info", "info");
} /* init() */

int do_info(string str) {
  string *creators,
         *domains,
         *mem,
         *cre_not,
         bit;
  mapping no_dir, not_creator;
  int i, j, cols;

  cols = (int)this_player()->query_cols();
  creators = get_dir("/w/");
  domains = get_dir("/d/");
  no_dir = ([ ]);
  not_creator = ([ ]);
  cre_not = ({ });
  for (i=0;i<sizeof(creators);i++)
    if (file_size("/w/"+creators[i]) != -2) {
      creators = delete(creators, i, 1);
      i--;
    } else if (!"/secure/login"->test_creator(creators[i])) {
      cre_not += ({ creators[i] });
      creators = delete(creators, i, 1);
      i--;
    }
  for (i=0;i<sizeof(domains);i++) {
    mem = (string *)("/d/"+domains[i]+"/master")->query_members() +
          ({ (string *)("/d/"+domains[i]+"/master")->query_lord() });
/* remove the members from the creators domain. */
    creators -= mem;
/* Check each one... */
    for (j=0;j<sizeof(mem);j++) {
      if (file_size("/w/"+mem[j]) != -2)
        no_dir[mem[j]] = domains[i];
      if (!"/secure/login"->test_creator(mem[j]))
        not_creator[mem[j]] = domains[i];
    }
/* Ok...  print the membership list. */
    bit = "Members of "+domains[i]+" are: ";
    printf("%s%-=*s\n", bit, cols-strlen(bit), implode(mem, ", ")+".");
  }
  if (sizeof(no_dir)) {
    write("These creators are in domains but do not have a directory:\n");
    printf("    %-=*s", cols-5, implode(keys(no_dir), ", ")+".\n");
  }
  if (sizeof(not_creator)) {
    write("These creators are in domains but are not actually creators:\n");
    printf("    %-=*s", cols-5, implode(keys(not_creator), ", ")+".\n");
  }
  if (sizeof(creators)) {
    write("These creators are not in any domain:\n");
    printf("    %-=*s", cols-5, implode(creators, ", ")+".\n");
  }
  if (sizeof(cre_not)) {
    write("These creators have a directory, but are not creators...:\n");
    printf("    %-=*s", cols-5, implode(cre_not, ", ")+".\n");
  }
  return 1;
} /* do_info() */
