/*
 * The help module.
 * This will (should) give help on the required thingy.
 * share and enjoy.
 * dested by the player after use.
 */
inherit "std/object";
inherit "global/more_file";

#define SOUL_OB "/obj/handlers/soul"
#define help_dirs ({ "/doc/helpdir/", "/doc/concepts/" })
#define creator_dirs ({ "/doc/efun/", "/doc/lfun/", "/doc/creator/",\
"/doc/create" })

string cross_ref;

void setup() {
  set_name("help");
  set_short(0);
  set_long("This is the object that gives you all the help so frog on.\n");
}

string search_help(string str) {
  int i;
  mixed cross_ref;

  for (i=0;i<sizeof(help_dirs);i++)
    if (file_size(help_dirs[i]+str)>0)
      return help_dirs[i]+str;
  if (this_player()->query_creator())
    for (i=0;i<sizeof(creator_dirs);i++)
      if (file_size(creator_dirs[i]+str)>0)
        return creator_dirs[i]+str;

  cross_ref = read_file("/doc/cross_ref");
  cross_ref = explode(cross_ref, "%");
  if ((i=member_array(str,cross_ref))==-1)
    return 0;
  return extract(cross_ref[i+1],0,strlen(cross_ref[i+1])-2);
    /* use extract, not explode */
}

int help(string str) {
  string filen, s;
  object morer;

  if (!str) {
    printf("General concepts:\n%-#*s\n", (int)this_player()->query_cols(),
           implode(delete(get_dir("/doc/concepts/*"), 0, 2), "\n"));
    printf("\nCommands:\n%-#*s\n", (int)this_player()->query_cols(),
           implode(delete(get_dir("/doc/helpdir/*"),0,2),"\n"));
    write("\nUsage : help <topic>\n");
    return 1;
  }
  filen = search_help(str);
  if (!filen) {
    object *objs;
    int flag, loop;

    flag = 0;
    objs = find_match(str, this_player());
    if(!sizeof(objs)) {
      s = (string)SOUL_OB->help_soul(str);
      if (!s) {
        notify_fail("There is no help on "+str+" sorry.\n");
        return 0;
      }
      s = sprintf("%-=*s", this_player()->query_cols(), s);
      this_player()->set_finish_func(0);
      this_player()->more_string(s);
      return 1;
    }
    for(loop = 0; loop < sizeof(objs); loop++) {
      string text;

      if(text = (string)objs[loop]->get_help()) {
        write("Help on " + objs[loop]->query_name() + " :\n" + text + "\n");
        flag = 1;
      }
    }
    if(!flag) {
      if(sizeof(objs) > 1)
        write("No help on those objects.\n");
      else
        write("No help on that object.\n");
    }
    return 1;
  }
  write("Ok got "+filen+" back.\n");
  more_file(filen);
  return 1;
}
