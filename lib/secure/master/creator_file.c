/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: creator_file.c,v 1.1 1998/01/06 05:12:03 ceres Exp $
 * $Log: creator_file.c,v $
 * Revision 1.1  1998/01/06 05:12:03  ceres
 * Initial revision
 * 
*/
/*
 * I ain't touching THIS one.. *quiver* // Z
 */
varargs mixed creator_file(string file, int author) {
  string *str;

  if (!file || !stringp(file)) return "NOONE";
  str = explode(file, "/") - ({ "" });
  if (sizeof(str)<2) return 0;
  switch (str[0]) {
   case "secure" :
     return get_root_uid();
   case "obj" :
     if (sizeof(str) > 2 && str[1] == "secure")
       return "ims";
   case "global" :
   case "std" :
   case "cmds" :
   case "room" :
     return get_bb_uid();
   case "net" :
     return "Network";
  case "www" :
    if (sizeof(str) > 2 && str[1] == "secure")
      return "Root";
    return "WWW";
   case "tmp" :
     if (str[1] != "mon-shad")
       return 0;
     return "monster";
   case "failsafe" :
     return "failsafe";
   case "d" :
     if (sizeof(str) < 3)
       return 0;
     if (!author || member_array(query_domains(), str) == -1)
       return capitalize(str[1]);
     return ("/d/"+str[1]+"/master")->author_file(str);
   case "w" :
     if (sizeof(str) < 3)
       if (str[1] == "common" || str[1] == "development" ||
           str[1] == "meeting")
         return "womble-frog";
       else
         return 0;
     return str[1];
  }
} /* creator_file() */

mixed author_file(mixed bing) {
  return creator_file(bing, 1);
} /* author_file() */

mixed domain_file(mixed bing) {
  string str;

  str = creator_file(bing);
  if (!str) return str;
  if (str[0] >= 'A' && str[0] <= 'Z') return str;
  return "Creator";
} /* domain_file() */
