/*
 * The help module.
 * This will (should) give help on the required thingy.
 * share and enjoy.
 */

#include "nroff.h"
#define SOUL_OB "/obj/handlers/soul"
#define help_dirs ({ "/doc/helpdir/", "/doc/concepts/" })
#define creator_dirs ({ "/doc/lfun/", "/doc/creator/",\
"/doc/create/", "/doc/driver/lpc/types/", "/doc/driver/applies/", \
"/doc/driver/concepts/", "/doc/driver/lpc/constructs/", \
"/doc/driver/efuns/conglom/" })


string search_help(string str) {
  if (file_size(NROFF_DIR+str+".o") > 0)
    return NROFF_DIR+str;
  if (this_player()->query_creator() && file_size(CNROFF_DIR+str+".o") > 0)
    return CNROFF_DIR+str;
} /* search_help() */

string create_help(string str) {
  int i;
  mixed cross_ref;

  for (i=0;i<sizeof(help_dirs);i++)
    if (file_size(help_dirs[i]+str)>0) {
      NROFF_HAND->create_help(help_dirs[i], str);
      return NROFF_DIR+str;
      return help_dirs[i]+str;
    }
  if (this_player()->query_creator())
    for (i=0;i<sizeof(creator_dirs);i++)
      if (file_size(creator_dirs[i]+str)>0) {
        NROFF_HAND->create_chelp(creator_dirs[i], str);
        return CNROFF_DIR+str;
        return creator_dirs[i]+str;
      }

  cross_ref = read_file("/doc/cross_ref");
  cross_ref = explode(cross_ref, "%");
  if ((i=member_array(str,cross_ref))==-1)
    return 0;
  return extract(cross_ref[i+1],0,strlen(cross_ref[i+1])-2);
    /* use extract, not explode */
} /* create_help() */

int do_help(string str) {
  string filen, s;

  if (!str) {
    printf("General concepts:\n%-#*s\n", (int)this_player()->query_cols(),
           implode(delete(get_dir("/doc/concepts/*"), 0, 2), "\n"));
    printf("\nCommands:\n%-#*s\n", (int)this_player()->query_cols(),
           implode(delete(get_dir("/doc/helpdir/*"), 0, 2),"\n"));
    write("\nUsage : help <topic>\n");
    return 1;
  }
  filen = search_help(str);
  if (!filen)
    filen = create_help(str);
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
      this_player()->set_finish_func("end_of_help");
      this_player()->more_string(s);
      return 1;
    }
    for(loop = 0; loop < sizeof(objs); loop++) {
      string text;

      if(text = (string)objs[loop]->get_help()) {
        this_player()->more_string("Help on "+(string)objs[loop]->query_name()+" :\n"+
          text+"\n","Help");
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
  filen = (string)NROFF_HAND->cat_file(filen, 1);
  if (!filen) {
    filen = create_help(str);
    filen = (string)NROFF_HAND->cat_file(filen);
  }
  if (!filen) {
    notify_fail("Help file broken.  Please tell a creator.\n");
    log_file("HELP_FILES", "Help file "+str+" broken.\n");
    return 0;
  }
  this_player()->more_string(filen);
  return 1;
} /* do_help() */

int end_of_help() {
  return 1;
} /* end_of_help() */
