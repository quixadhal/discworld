/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: efuns.c,v 1.1 1998/01/06 04:55:54 ceres Exp $
 * $Log: efuns.c,v $
 * Revision 1.1  1998/01/06 04:55:54  ceres
 * Initial revision
 * 
*/
#define HANDLER "/global/virtual/handler"

/*
 * clone_object modified to try and make .c files in /d/ virtual
 */
object clone_object(string name) {
  if(name[0..2] == "/d/" || name[0..1] == "d/")
    return (object)HANDLER->clone_virtual_object(name);
  return efun::clone_object(name);
}

/*
 * destruct modified to cope with virtual objects
 */
void destruct(mixed name) {
  object obj;

  if(stringp(name)) {
    obj = find_object(name);
    if(obj->query_property("file_name"))
      HANDLER->destruct_virtual_object(obj);
    destruct(obj);
  } else if(objectp(name)) {
    if(name->query_property("file_name"))
      HANDLER->destruct_virtual_object(name);
    destruct(name);
  }
}

/*
 * file_name modified to cope with virtual objects
 */
string file_name(object obj) {
  string name, number, file;

  name = efun::file_name(obj);
  if(sscanf(name, "%s#%s", name, number) == 2) {
    if(file = (string)obj->query_property("file_name")) {
      return file + "#" + number;
    } else {
      return name + "#" + number;
    }
  }
  return name;
}

/*
 * find_object modified to cope with virtual objects
 */
object find_object(string str) {
  object obj;

  if(obj = efun::find_object(str))
    return obj;
  return (object)HANDLER->find_virtual_object(str);
}
