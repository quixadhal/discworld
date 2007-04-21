/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: handler.c,v 1.1 1998/01/06 04:55:54 ceres Exp $
 * $Log: handler.c,v $
 * Revision 1.1  1998/01/06 04:55:54  ceres
 * Initial revision
 * 
*/
mapping cloned_objects;

create() {
  seteuid("Room");
  cloned_objects = ([ ]);
  unguarded((: restore_object, "/global/virtual/handler" :));
}

self_dest() {
  unguarded((: save_object, "/global/virtual/handler" :));
  destruct(this_object());
}

mapping query_cloned_objects() { return cloned_objects; }

destruct_virtual_object(object obj) {
  string name, trash;

  name = file_name(obj);
  if(sscanf(name, "%s#%s", name, trash) == 2)
    name = trash;
  if(cloned_objects[name])
    map_delete(cloned_objects, name);
}

object find_virtual_object(string name) {
  string trash;

  if(sscanf(name, "%s#%s", name, trash) == 2)
    name = trash;
  if(cloned_objects[name])
    return (object)cloned_objects[name];
  return (object)0;
}

int error(string str) {
  write(str+"\n");
  return 1;
}

int str_to_int(string str) {
  int i, j, k;

  for(i=strlen(str)-1,j=1;i>-1;k+=(str[i--]-48)*j,j*=10) ;
  return k;
}

mixed *sort_arguments(mixed *args) {
  int i;

  for(i=0;i<sizeof(args);i++) {
    if(pointerp(args[i])) 
      args[i] = sort_arguments(args[i]);
    else if(args[i][0..0] != "\"")
      args[i] = str_to_int(args[i]);
    else {
      args[i] = args[i][1..strlen(args[i])-2];
      if(args[i][strlen(args[i])-2..strlen(args[i])-1] == "\\n")
        args[i] = args[i][0..strlen(args[i])-3] + "\n";
    }
  }
  return args;
}

mixed *add_item(mixed *args, int m, string str) {
  if(m == 0)
    args += ({ str });
  else
    args[sizeof(args)-1] = add_item(args[sizeof(args)-1], m-1, str);
  return args;
}

mixed *add_level(mixed *args, int m) {
  if(m == 0) {
    args += ({ "" });
    args[sizeof(args)-1] = ({ });
  } else
    args[sizeof(args)-1] = add_level(args[sizeof(args)-1], m-1);
  return args;
}

object use_efun_clone(string file) {
  log_file("NON_VIRTUAL", file +"\n");
  return efun::clone_object(file);
}

/* This function will read a .c file in /d/ and try to make it */
/* virtual.  If it fails it will use the efun clone_object     */
object clone_virtual_object(string file) {
  int i, j, k, l, m, len;
  string data, *line, file_to_inherit, start, comment, rest;
  object obj;
  mixed *args;

  /* Notes:                                              */
  /* 1) path is stored in file_to_inherit to save memory */
  /* 2) line[0] is also used as a temporary variable     */

  /* If it is too big it's probably not virtual */
  if(file_size(file) > 5000)
    return use_efun_clone(file);

  /* Read the file */
  data = read_file(file);

  /* find the path to the file */
  line = explode(file, "/");
  file_to_inherit = "/" + implode(line[0..sizeof(line)-2], "/") + "/";

  /* read the include files */
  line = ({ (string)0 });
  while(sscanf(data, "%s#include \"%s\"%s", start, comment, rest) == 3) {
    if(!(line[0] = read_file(file_to_inherit + comment)))
      line[0] = read_file("/include/" + comment);
    if(line[0])
      data = start + line[0] + rest;
    else {
      error("Include file not found.");
      return (object)0;
    }
  }

  /* remove the comments */
  while(sscanf(data, "%s/*%s*/%s", start, comment, rest) == 3)
    data = start + rest;

  /* count the number of inherits */
  i = 0;
  file_to_inherit = data;
  while(sscanf(file_to_inherit, "%sinherit%s", start, file_to_inherit) == 2) {
    i++;
  }
  if(i != 1)
    return use_efun_clone(file);

  /* See if there is only one function than setup() */
  i = 0;
  file_to_inherit = data;
  /* first remove the array brackets */
  while(sscanf(file_to_inherit, "%s({%s", start, rest)  == 2)
    file_to_inherit = start + rest;
  while(sscanf(file_to_inherit, "%s{%s", start, file_to_inherit)  == 2)
    i++;
  if(i != 1)
    return use_efun_clone(file);

  /* make sure it's the setup */
  if(!(sscanf(data, "%ssetup()%s", start, rest) == 2))
    return use_efun_clone(file);

  /* Split it in lines */
  line = explode(data, "\n");

  /* Remove empty lines */
  line -= ({ "" });

  /* remove spaces and tabs from the start and end of the line */
  for(i=0;i<sizeof(line);i++) {
    len = strlen(line[i]);
    for(j=0;j<len;j++) {
      if((line[i][j..j] == " " || line[i][j..j] == "\t") && j == 0) {
        line[i] = line[i][1..len];
        j--;
      }
    }
    for(j=len-1;j>-1;j--) {
      if((line[i][j..j] == " " || line[i][j] == 0) && j == (len-1)) {
        line[i] = line[i][0..len-2];
        len--;
      }
    }
  }

  /* see if there are any variable declerations */
  for(i=0;i<sizeof(line);i++)
    if((line[i][0..3] == "int ") ||
       (line[i][0..5] == "mixed ") ||
       (line[i][0..6] == "object ") ||
       (line[i][0..6] == "string ") ||
       (line[i][0..7] == "mapping "))
      return use_efun_clone(file);

  /* Find and replace the defined stuff */
  for(i=0;i<sizeof(line);i++)
    if(line[i][0..7] == "#define ") {
      len = strlen(line[i]);
      for(j=8;j<len && line[i][j..j] != " ";j++);
      comment = line[i][8..j-1];
      file_to_inherit = line[i][j+1..len];
      line = delete(line, i--, 1);
      if(j < len)
        for(k=0;k<sizeof(line);k++)
          if(sscanf(line[k], "%s"+comment+"%s", start, rest) == 2)
            line[k] = start + file_to_inherit + rest;
    }

  /* remove spaces from the middle of lines       */
  /* split lines with more than one ;             */
  /* remove "+"                                   */
  /* and while we're at it join some of the lines */
  for(i=0;i<sizeof(line);i++) {
    for(j=0,k=0,l=0;j<strlen(line[i]);j++) {
      switch(line[i][j..j]) {
        case "\"" :
          if(!k) {
            k = 1;
            if(l && l+1 == j)
              if(l+2 < strlen(line[i]))
                line[i] = line[i][0..l-2] + line[i][l+2..strlen(line[i])];
              else {
                line[i] = line[i][0..l-2] + line[i+1];
                line = delete(line,i+1,1);
              }
          }
          else
            k = 0;
          break;
        case " " :
          if(!k)
            if(j)
              line[i] = line[i][0..--j] + line[i][j+2..strlen(line[i])];
            else
              line[i] = line[i][1..strlen(line[i])];
          break;
        case ";" :
          if(!k && j != strlen(line[i])-1) {
            line = line[0..i] + line[i][j+1..1000] + line[i+1..1000];
            line[i] = line[i][0..j];
            j=0;
          }
          break;
        case "+" :
          if(!k && j && line[i][j-1..j-1] == "\"")
            l = j;
          break;
      }
    }
    if((k)) {
      line[i] = line[i--] + line[i+2];
      line = delete(line,i+2,1);
    } 
    /* what happens if it's an }                                     */
    /*|| (!k && line[i][strlen(line[i])-1..strlen(line[i])-1] != ";")*/
  }

  /* join the remaining long lines */
  for(i=0;i<sizeof(line);i++) {
    len = strlen(line[i]);
    if(line[i][len-1..len-1] != ";")
      if(line[i][len-2..len-1] == "\"+") {
        if(line[i+1][0..0] == "\"")
          line[i] = line[i--][0..len-3] + line[i+2][1..1000];
        else
          line[i] = line[i--] + line[i+2];
        line = delete(line,i+2,1);
      }
  }

  /* Find the file to inherit */
  for(i=0;i<sizeof(line);i++)
    if(line[i][0..6] == "inherit")
      file_to_inherit = line[i][8..sizeof(line[i])-3];

  /* For now if it's /std/room  don't do it */
  if(file_to_inherit[0..8] == "/std/room")
    return use_efun_clone(file);

  /* try to clone the object and define it*/
  obj = clone_object(file_to_inherit);

  obj->add_property("file_name", file);
  start = file_name(obj);
  if(sscanf(start, "%s#%s", start, rest) == 2)
    cloned_objects[rest] = obj;
  else
    cloned_objects[start] = obj;

  for(i=0;i<sizeof(line);i++)
    if(line[i] == "setup(){" || line[i] == "voidsetup(){")
      break;
  
  for(i++;i<sizeof(line);i++) {
    if(line[i] != "}") {
      sscanf(line[i], "%s(%s", start, comment);
      for(j=0,l=0,args=({});j<strlen(comment);j++) {
        if(comment[j..j] == "(") {
          if(comment[j+1..j+1] == "{" || comment[j+1..j+1] == "[") {
            args = add_level(args, l++);
            comment = comment[j+2..1000];
            j = -1;
            continue;
          }
        }
        if(comment[j..j] == ")") {
          if(comment[j-1..j-1] == "}" || comment[j-1..j-1] == "]") {
            if(j-1)
              args = add_item(args, l--, comment[0..j-2]);
            comment = comment[j+1..1000];
            j = -1;
            continue;
          }
        }
        if(comment[j..j] == "\"") {
          if(k)
            k = 0;
          else
            k = 1;
        }
        if(comment[j..j] == "," && k == 0) {
          if(!l) {
            if(j)
              args = add_item(args, l, comment[0..j-1]);
            comment = comment[j+1..1000];
            j = -1;
            continue;
          } else {
            if(comment[j+1..j+1] == "}" || comment[j+1..j+1] == "]") {
              if(!j)
                comment = comment[j+1..1000];
              else
                comment = comment[0..j-1] + comment[j+1..1000];
            } else {
              if(j)
                args = add_item(args, l, comment[0..j-1]);
              comment = comment[j+1..1000];
            }
            j = -1;
            continue;
          }
        }
        if(comment[j..j] == ")" && k == 0 && l == 0) {
          if(comment[j-1..j-1] != "}" && comment[j-1..j-1] != "]") {
            args = add_item(args, l, comment[0..j-1]);
            comment = comment[j+1..1000];
            j = -1;
          }
        }
      }

      args = sort_arguments(args);
      
      switch(sizeof(args)) {
        case 1 : 
          call_other(obj, start, args[0]);
          break;
        case 2 : 
          call_other(obj, start, args[0], args[1]);
          break;
        case 3 : 
          call_other(obj, start, args[0], args[1], args[2]);
          break;
        case 4 : 
          call_other(obj, start, args[0], args[1], args[2], args[3]);
          break;
        case 5 : 
          call_other(obj, start, args[0], args[1], args[2], args[3], args[4]);
          break;
        case 6 : 
          call_other(obj, start, args[0], args[1], args[2], args[3], args[4], args[5]);
          break;
        case 7 : 
          call_other(obj, start, args[0], args[1], args[2], args[3], args[4], args[5], args[6]);
          break;
      }
    }
  }
  log_file("VIRTUAL", file +"\n");
  return obj;
}

