string home_dir;
static string current_path;

string invalid_path() {
    write("Invalid path.\n");
    return 0;
    }
                        
string get_path(string str) {
string *array, *array1, temp, temp1;
int i,j;
  if (!str)
    str = home_dir;
  if (str == "~") {
    str = "w/"+this_player()->query_name();
  }
  else {
  if(sscanf(str, "~%s", temp) == 1) {
    if(sscanf(temp, "/%s", temp1) == 1) {
      str = home_dir + temp;
    }
    else {
      string name;
      if (sscanf(str,"~%s/%s",name,str)!=2) {
        name = extract(str,1);
        str = "/w/"+name;
      } else
/* cheat at this point and just assume they are a wizard. sigh i kno i know */
        str = "/w/"+name+"/"+str;
    }
  } else
    if (str[0] != '/') 
      str = current_path+"/"+str+"/";
  if (str == "/")
    return "/"; 
  else
    array = explode(str,"/");  
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
}
 
