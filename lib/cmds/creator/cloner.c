/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: cloner.c,v 1.4 2001/08/17 00:32:27 ceres Exp $
 */
inherit "/cmds/base";

#define CLONER "/global/cloner"
#define LIFE_TIME 7257600

int do_list(string file) {
  string *files, change;
  mapping changes;
  string ret;
  
  if(file && file != "") {
    files = this_player()->get_files(file);
  }
  
  changes = CLONER->query_changes();
  ret = "";
  foreach(change in keys(changes)) {
    if(files && member_array(change, files) == -1 &&
       strsrch(change, file) == -1) {
      continue;
    }
    
    ret += sprintf("%s:\n  %s until %s\n", change, changes[change][0],
           ctime(changes[change][1] + LIFE_TIME));
  }
  this_player()->more_string( ret, "Cloner list", 1 );
  return 1;
}

int do_move(string source, string dest) {
  string *sources, *tmp, destp, rcs;
  
  seteuid(geteuid(this_player()));

  sources = this_player()->get_files(source);
  if(!sizeof(sources))
    return notify_fail("No source files found.\n");

  destp = this_player()->get_path(dest);
  if(file_size(destp) != -2 && !mkdir(dest))
    return notify_fail("Destination must be a directory.\n");

  foreach(source in sources) {
    if(file_size(source) == -2 || file_size(source) == -1) {
      if(source[<4..] == "/RCS") {
        if(rename(source, source+".tmp"))
          return notify_fail("Error moving RCS directory to temp location.\n");
        else
          rcs = source+".tmp";
      } else
        write("Not moving directory: " + source + "\n");
    } else {
      tmp = explode(source, "/");
      dest = destp + "/" + tmp[sizeof(tmp)-1];
    
      if(rename(source, dest))
        return notify_fail("Error renaming [" + source + "] to [" + dest +
                           "]\n");
      CLONER->add_mapping(source, dest);
    }
  }

  if(rcs && rename(rcs, destp + "/RCS"))
    return notify_fail("Error moving RCS directory.\n");
    
  return 1;
}

int do_add(string source, string dest) {
  CLONER->add_mapping(source, dest);
  return 1;
}

int do_remove(string source) {
  CLONER->remove_mapping(source);
  return 1;
}

mixed *query_patterns() {
  return ({ "list", (: do_list(0) :),
            "list <word'file'>", (: do_list($4[0]) :),
            "move <word'source'> <word'dest'>", (: do_move($4[0], $4[1]) :),
            "add <word'source'> <word'dest'>", (: do_add($4[0], $4[1]) :),
            "remove <word'source'>", (: do_remove($4[0]) :), 
              });
}
