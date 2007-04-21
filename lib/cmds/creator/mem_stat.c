/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: mem_stat.c,v 1.3 2003/03/11 22:48:10 wodan Exp $
 * $Log: mem_stat.c,v $
 * Revision 1.3  2003/03/11 22:48:10  wodan
 * reactivated.
 *
 * Revision 1.2  2002/02/24 06:24:47  taffyd
 * Disabled command
 *
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
#include <creator.h>

#define OBJ_LIMIT 50
#define TP this_player()

inherit "/cmds/base";

string help();

mixed cmd(string str) {
  int memtotal, limit, i = TP->query_cols() -7 -1, j;
  string ret, str2, str3;
   object *objlist;


  if (!str || str == "")
    return notify_fail(help());

  // Remove excess whitespace
  while (strlen(str) && str[0] == ' ')
    str = str[1..];

  if (sscanf(str, "-%s%d %s", str3, limit, str2) == 3) {
    if (str3[0..0] != "l")
      return notify_fail("Error: Only valid character following '-' is 'l' at "
                         "this time.\n");
    str = str2;
  }

  seteuid(geteuid(TP));
  // Get the list of corresponding objects
  objlist = WIZ_PRESENT->wiz_present(str, TP);

  if (!arrayp(objlist) || sizeof(objlist) < 1) {
    mixed *filelist;

    str = TP->get_path(str);

    notify_fail("Unable to find any objects or files corresponding"
                " to the given name.\n");

    if (sizeof(filelist = TP->get_files(str)) > 0) {
      filelist = filter(filelist, (: (sizeof($1) > 2) &&
                                   ($1[sizeof($1) - 2..] == ".c") :));

      if (sizeof(filelist) < 1)
        return 0;

      filelist = filter(filelist, (: find_object($1) :));
      objlist = map(filelist, (: find_object($1) :));
    } else
      return 0;
  }

  ret = sprintf("%-"+ i +"s %-7s\n", "Object name", "Memory");
  ret += sprintf("%-"+ i +"s %-7s\n", "------------", "-------");

  if (!limit)
    limit = OBJ_LIMIT;
  
  if (limit > sizeof(objlist))
    limit = sizeof(objlist);

  if (limit == 1)
    ret += sprintf("%-"+ i +"."+ i +"s %7.7d\n",
                   WIZ_PRESENT->desc_f_object(objlist[sizeof(objlist) - 1]),
                   memory_info(objlist[sizeof(objlist) - 1]));
  else
    for (j = 0; j < limit; j++)
      ret += sprintf("%-"+ i +"."+ i +"s %7.7d\n",
                     WIZ_PRESENT->desc_f_object(objlist[j]),
                     memory_info(objlist[j]));

  if (sizeof(objlist) > 1) {
    for (j = 0; j < sizeof(objlist); j++)
      memtotal += memory_info(objlist[j]);

    ret += sprintf("%-"+ i +"."+ i +"s %7.7s\n",
                   "", "=======");
    ret += sprintf("%-"+ i +"."+ i +"s %7.7d\n",
                   "Total memory used for all "+ sizeof(objlist) +
                   " object(s):", memtotal);
  }
  TP->more_string(ret);
  return 1;
}

string help() {
  return "Usage: memstat [-l<number>] [<filename(s)>|<wiz_present syntax>]\n\n"
"Memstat lists the guestimated amount of memory the specified object(s) use. "
"Using the -l option, a maximum amount of objects to be listed may be "
"specified otherwise a default limit of "+ OBJ_LIMIT +" objects are listed. "
"Paths including wildcards are taken as a way of specifying more than one "
"file.\n";
}

mixed *query_patterns() { 
    return ({ "<string>", (: cmd( $4[0] ) :) });
} /* query_patterns() */

