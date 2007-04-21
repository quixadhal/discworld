/*  -*- LPC -*-  */
/*
 * $Id: valid_write.c,v 1.9 2000/02/06 00:20:34 turrican Exp $
 */
/*
 * This controls the copy permisions for everything in the mud.
 * It defaults to 0 if all else fails.
 * Any permisions can be overridden by the global permision giving/
 * changeing code.
 *
 * The master.c's in the domain and wizard dirs will be checked if
 * any access is done to them.  This is a very flexible system,
 * but perhaps a little hard for some people to use.
 */

int valid_copy(string path, mixed euid, string func) {
  object master_ob;
  string *bits = explode(path, "/") - ({ "", "." });
  mapping perms;
  
  perms = permission_match_path(permissions, path);
  
  if(sizeof(bits) < 2 || bits[0] == "open" || bits[0] == "doc" ||
     bits[0] == "log" || bits[0] == "mudlib" || bits[0] == "w")
    return check_permission(euid, func, path, perms, READ_MASK);
  
  master_ob = find_object("/d/" + bits[1] + "/master");

  if((master_ob && master_ob->copy_with_read(path, euid, func)) ||
     (!master_ob && master()->query_senior(euid)))
    return check_permission(euid, func, path, perms, READ_MASK);
  else
    return check_permission(euid, func, path, perms, WRITE_MASK);
}
