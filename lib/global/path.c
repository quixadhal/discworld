string home_dir;
static string current_path;

string invalid_path() {
    write("Invalid path.\n");
    return 0;
} /* invalid_path() */
                        
string get_path(string str) {
string *array, *array1, temp, temp1;
int i,j;
  if (!str) {
    if(!home_dir) {
      home_dir = "/w/" + this_player()->query_name();
    }
    str = home_dir;
  }
  if (str == "~") {
    str = "w/"+this_player()->query_name();
  }
  else {
  if(str[0] == '~') {
    if(str[1] == '/') {
      sscanf(str, "~%s", temp);
      str = "/w/" + this_player()->query_name() + temp;
    }
    else {
      string name;
      if (sscanf(str,"~%s/%s",name,str)!=2) {
        name = extract(str,1);
        str = "w/"+name;
      } else
/* cheat at this point and just assume they are a wizard. sigh i kno i know */
        str = "w/"+name+"/"+str;
    }
  } else
    if (str[0] != '/') 
      str = current_path+"/"+str+"/";
  if (str == "/")
    return "/"; 
  else
    array = explode(str,"/") - ({ "" }) ;
  for (i=0;i<sizeof(array);i++)
    if (array[i] == "..") {
      if (i<1)
        return invalid_path();
      if (i == 1)
        array1 = ({ "." });
      else
         array1 = array[0..i-2];
      if (i+1 <= sizeof(array)-1) 
        array1 += array[i+1..sizeof(array)-1];
      array = array1;
      i -=2;
    } else
      if (array[i] == ".")
        array[i] = 0;
 if (array)
   str = implode(array, "/");
 else
   str = "";
  }
  return "/"+str;
} /* get_path() */
 
static string *get_files(string str) {
    int loop, count;
    string *filenames, rmpath, temp1, temp2, *names, *files;
 
    names = explode(str, " ")- ({ "" });
    if(sizeof(names) == 0) {
      return ({ });
    }
    filenames = ({ });
    for(count = 0; count < sizeof(names); count++) {
      str = names[count];
      str = rmpath = get_path(str);
      if(rmpath == "/") {
        filenames += ({ "/" });
        continue;
      }
      if(sscanf(rmpath, "%s/%s", temp1, temp2) == 2) {
          string *path_elements;
 
          path_elements = explode(rmpath, "/") - ({ "" });
          rmpath = implode(path_elements[0..sizeof(path_elements) - 2], "/");
      }
      files = get_dir(str);

      if(files) {
        int loop2;
        for(loop2 = 0; loop2 < sizeof(files); loop2++) {
          filenames += ({ rmpath + "/" + files[loop2] });
        }
      }
    }
    for(loop = 0; loop < sizeof(filenames); loop++) {
      if(filenames[loop][0] != '/') {
        filenames[loop] = "/" + filenames[loop];
      }
    }
    return filenames;
}

/* Changed to be *.* so as to handle virtual wombles as well */
 string *get_cfiles(string str) {
  int loop;
  string temp, *names, temp2;

  names = explode(str, " ") - ({ "" });
  for(loop = 0; loop < sizeof(names); loop++) {
    if(sscanf(names[loop], "%s.%s", temp, temp2) != 2) {
      names[loop] += ".c";
    }
  }
  str = implode(names, " ");
  return get_files(str);
} /* get_cfiles() */

string query_current_path() { return current_path; }
