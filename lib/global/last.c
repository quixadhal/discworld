string mtime(int i) {
  string str;

  str = efun::ctime(i);
  return str[0..strlen(str)-9];
}
string stime(int i) {
  return efun::ctime(i)[11..15];
}

string ntime(int i) {
  return sprintf("%02d:%02d", (i/3600), (i/60)%60);
}


int last(string str) {
  string file, s1, s2, s3, s4, s5, *sarr;
  int time, t2, i;
  int lines, fsize;

  if (!str || str == "") {
    notify_fail("Usage: last -N <name>\n");
    return 0;
  }
  if (sscanf(str, "-%s %s", s1, str) == 2)
    sscanf(s1, "%d", lines);
  if (str == "reboot") {
    fsize = file_length("/log/REBOOT");
    if (lines)
      file = read_file("/log/REBOOT", fsize-lines, fsize);
    else
      file = read_file("/log/REBOOT");
    sarr = explode(file, "\n");
    for (i=0;i<sizeof(sarr);i++)
      if (sscanf(sarr[i], "%s[%d]%s", s1, time, s2) == 3)
        printf("%15-s %s\n", "reboot", mtime(time));
    return 1;
  }
  fsize = file_length("/log/ENTER");
  if (lines)
    file = read_file("/log/ENTER", fsize-lines, fsize);
  else
    file = read_file("/log/ENTER");
  if (!file) {
    notify_fail("Log file non existant, or bad things are happening.  "+
                "(Bad vibes man!)\n");
    return 0;
  }
  str = (string)this_player()->expand_nickname(str);
  while (sscanf(file, "%sEnter : "+str+"%s[%d]%s",s1, s3, time, s2) == 4) {
    file = s2;
    if (sscanf(file, "%sExit  : "+str+"%s[%d]%s", s1, s5, t2, s2) != 4)
      if (!find_player(str))
        printf("%15-s %s - Destd (??:??)\n", str, mtime(time));
      else
        printf("%15-s %s - Not logged off\n", str, mtime(time));
    else {
      if (sscanf(file, "%sEnter : "+str+"%s", s3,s4) == 2)
        if (strlen(s1) > strlen(s3)) {
          printf("%15-s %s - %s (%s)\n", str, mtime(time), "Destd", "??:??");
          continue;
        }
      printf("%15-s %s - %s (%s)\n", str, mtime(time), stime(t2), ntime(t2-time));
      file = s2;
    }
  }
  return 1;
}
