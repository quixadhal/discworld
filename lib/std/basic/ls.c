/* this does an ls, now that get_dir has been written */
string pad_space(string str, int i) {
  return str+"                                                    "[0..
           i-strlen(str)];
} /* pad_space() */

string dir_entry(string path, string name) {
  int size;
  string *obs;

  size = file_size(path+name);
  if (size == -2) {
    obs = get_dir(path+name+"/");
    if (!obs)
      size = -1;
    else
      size = sizeof(obs);
    name += "/";
  } else {
    if (virtual_find_object(path+name))
      name = "*"+name;
    size = (size / 1024) + 1;
  }
  if (size < 1000)
    return "    "[0..3-strlen(size+"")]+size+" "+name;
  return size+" "+name;
} /* dir_entry() */
 
int ls(string str) {
  string *direc, *bit;
  string bing;
  int max_siz, i, size;
 
 
  if (file_size(str) == -2 && str[strlen(str)-1] != '/')
    str += "/";
  direc = get_dir(str);
  if (!direc) {
    write("No files.\n");
    return 0;
  }
  if (!sizeof(direc)) {
    write("No files.\n");
    return 0;
  }
  if (file_size(str) == -2) {
    if (str[strlen(str)-1] != '/')
      str += "/";
  } else {
    bit = explode(str, "/");
    bit = bit[0..sizeof(bit)-2];
    str = "/"+implode(bit,"/")+"/";
  }
  if(str == "//")
    str = "/";
  write("Dir of: "+str+"\n");
     
  bing = "";
  for (i=0;i<sizeof(direc);i++)
    if (strlen(direc[i]) > 30) {
      write(dir_entry(str, direc[i])+"\n");
      direc = delete(direc, i, 1);
      i--;
      continue;
    } else
      bing += dir_entry(str, direc[i])+"\n";
 
  printf( "%#-*s\n", this_player()->query_cols(), bing);
} /* ls() */
