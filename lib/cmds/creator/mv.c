/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: mv.c,v 1.1 1998/01/06 05:25:41 ceres Exp $
 * $Log: mv.c,v $
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
/* mv command, trial out by Turrican for a commands daemon. */

mixed cmd(string str) {
  int loop, fs, pos, multi;
  string *filenames, dest, *fnames, trash1, trash2, trash3, trash4;
  string path, *temp, start, end;

  seteuid(geteuid(this_player()));

  if ((!str) || (sscanf(str, "%s %s", trash1, trash2) != 2))
    return notify_fail("Usage : mv file [file|dir...]\n");
  fnames = explode(str, " ");
  filenames = this_player()->get_files(implode(fnames[0..sizeof(fnames) -2], " "));
  if(!sizeof(filenames))
    return notify_fail("Usage : mv file [file|dir...]\n");
  dest = fnames[sizeof(fnames) - 1];
  dest = this_player()->get_path(dest);
  if (!dest) {
    write("No destination\n");
    return 1;
  }
  if (sscanf(str, "%s*%s %s*%s", trash1, trash2, trash3, trash4) == 4) {
    multi = 1;
    temp = explode(dest, "/");
    path = implode(temp[0..sizeof(temp) - 2], "/") + "/";
    sscanf(temp[sizeof(temp)-1], "%s*%s", start, end);
    temp = explode(implode(fnames[0..sizeof(fnames) -2], " "), "/");
    sscanf(temp[sizeof(temp)-1], "%s*%s", trash1, trash2);
    pos = strlen(trash1);
  } 
  for (loop = 0; loop < sizeof(filenames); loop++) {
    str = filenames[loop];
    if(file_size(str) == -1) {
      write("No such file : " + str + "\n");
      continue;
    }
    if(multi) {
      temp = explode(str, "/");
      trash1 = temp[sizeof(temp)-1];
      fs  = strlen(trash1) - strlen(trash2) - 1;
      dest = path + start + trash1[pos..fs] + end;
      fs = file_size(dest);
    }
    else {
      fs = file_size(dest);
      if(fs == -2) {
        string *names;

        names = explode(str, "/");
        fs = file_size(dest + "/" + names[sizeof(names) - 1]);
      }
    }
    if(fs != -1) {
      write("File exists : " + dest + "\n");
      continue;
    }
    rename(str, dest);
  }
  write("Ok.\n");
  return 1;
}

void dest_me() {
  destruct(this_object());
}

void clean_up() {
  dest_me();
}

void reset() {
  dest_me();
}
