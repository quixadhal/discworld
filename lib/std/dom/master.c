/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: master.c,v 1.2 2003/04/15 23:50:10 pinkfish Exp $
 * $Log: master.c,v $
 * Revision 1.2  2003/04/15 23:50:10  pinkfish
 * *** empty log message ***
 *
 * Revision 1.1  1998/01/06 04:03:45  ceres
 * Initial revision
 * 
*/
/*
 * Beware editing this file.
 * Make sure you know what you are doing.  Stuffing this up could be
 * fatal in some ways.
 */

inherit "/std/dom/base_master";

string query_lord() {
  return LORD;
}

string author_file(string *path) {
  return capitalize(DOMAIN);
}

int check_permission(string euid, string *path, int mask) {
  if (euid == query_lord())
    return 1;
  return ::check_permission(euid, path, mask);
}

/*
 * By default, use the valid_read()/valid_write() functions defined in the
 * inherited file /std/dom/base_master.c.
 * These default to reading allowed for everyone, writing only allowed
 * for independent creators and the domain lord.
 * This is all overridden by the check_permission() function
 * which is called first, which should be used instead.
 */
int valid_read(string *path, string euid, string funct);

int valid_write(string *path, string euid, string funct);

int query_member(string name) {
  return !undefinedp(members[name]) || name == LORD;
} /* query_member() */

string log_who(string where) {
  return LORD;
} /* log_who() */

/* Please change this to a text describing the domain, it will be used
   on the WWW pages and in finger info. */

string query_info() {
  return "";
}
