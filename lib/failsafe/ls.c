/* this does an ls, now that get_dir has been written */
string pad_space(string str, int i) {
  return str+extract("                                                    ",0,
           i-strlen(str));
}
string dir_entry(string path, string name) {
  int size;

  size = file_size(path+name);
  if (size == -2) {
    size = sizeof(get_dir(path+name+"/"));
    name = name + "/";
  } else
    size = (size / 1024) + 1;
  if (size < 1000)
    return extract("    ",0,3-strlen(size+""))+size+" "+name;
  return size+" "+name;
}
 
int ls(string str) {
  string *direc, *bit;
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
  write("Dir of: "+str+"\n");
     
  for (i=0;i<sizeof(direc);i++)
    if (strlen(direc[i]) > 30) {
      write(dir_entry(str, direc[i])+"\n");
      direc = delete(direc, i, 1);
      i--;
      continue;
    } else {
      direc[i] = dir_entry(str, direc[i]);
    if (strlen(direc[i]) > max_siz)
      max_siz = strlen(direc[i]);
    }
 
  max_siz = 75 / (max_siz+6);
  for (i=0;i<sizeof(direc);i++) {
    if ((i % max_siz) == (max_siz -1))
      write(direc[i]+"\n");
    else
    write(pad_space(direc[i], 75/max_siz));
  }
  if (i%max_siz)
    write("\n");
}
