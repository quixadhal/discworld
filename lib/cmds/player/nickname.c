/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: nickname.c,v 1.7 2002/02/10 19:07:37 tannah Exp $
 * $Log: nickname.c,v $
 * Revision 1.7  2002/02/10 19:07:37  tannah
 * Blocked nicknaming a, an and the.
 *
 * Revision 1.6  2001/11/08 01:46:18  pinkfish
 * Change the message a little.
 *
 * Revision 1.5  2001/07/16 23:52:12  ceres
 * Added lower_case for adding nicknames
 *
 * Revision 1.4  2000/07/13 00:54:53  pinkfish
 * Make it preint somethign when there are no nicknaes.
 *
 * Revision 1.3  2000/04/23 03:39:51  presto
 * Corrected mistake with doing 'nickname <foo>'
 *
 * Revision 1.2  2000/03/03 02:45:59  pinkfish
 * Do sorting and a few other things.
 *
 * Revision 1.1  1998/01/06 05:29:43  ceres
 * Initial revision
 *
*/
/* command, trial out by Turrican for a commands daemon. */

inherit "cmds/base";

int print_nicknames(mapping map_nicknames);
int add_nickname(string str, mapping map_nicknames);

int print_single_nickname(string str) {
  mapping map_nicknames;

  map_nicknames = previous_object()->query_nicknames();
  if (!map_nicknames) {
    map_nicknames = ([ ]);
  }

  str = lower_case(str);
  if(!map_nicknames[str]) {
    add_failed_mess("That nickname does not exist.\n");
    return 0;
  }
  write("Setting for nickname "+str+" is "+map_nicknames[str]+".\n");
  return 1;
} /* print_single_nickname() */

int print_nicknames(int sorted) {
  int i;
  int cols;
  string str;
  string str_long;
  string str_middle;
  string str_small;
  string bit;
  string *tmp;
  mapping map_nicknames;

  map_nicknames = previous_object()->query_nicknames();
  bit = "";
  str_long = "";
  str_middle = "";
  str_small = "";
  if (!map_nicknames) {
    map_nicknames = ([ ]);
  }

  if (!sizeof(map_nicknames)) {
     write("You do not have any nicknames.\n");
     return 1;
  }

  tmp = sort_array(keys(map_nicknames), 1);
  cols = (int)this_player()->query_cols();
  for (i=0;i<sizeof(tmp);i++) {
    str = tmp[i]+": "+map_nicknames[tmp[i]]+"  ";
    if (strlen(str) > 39 || sorted) {
      str_long += sprintf(tmp[i]+": %-=*s\n", cols - strlen(tmp[i]), map_nicknames[tmp[i]]);
    } else if (strlen(str) > 19) {
      str_middle += str+"\n";
    } else {
      str_small += str+"\n";
    }
  }
  if (strlen(str_middle)) {
    str_long += sprintf("%-#*s\n", cols, str_middle);
  }
  if (strlen(str_small)) {
    str_long += sprintf("%-#*s\n", cols, str_small);
  }
  this_player()->more_string(str_long, "Nicknames");
  return 1;
} /* print_nicknames() */

int add_nickname(string s1, string s2) {
  mapping map_nicknames;

  s1 = lower_case(s1);
  s2 = lower_case(s2);

  // nicknaming these things buggers up the parser
  if( member_array( s1, ({ "a", "an", "the" }) ) != -1 ) {
    add_failed_mess( "Using articles as nicknames confuses the parser.  "
                     "Choose another.\n" );
    return 0;
  }

  map_nicknames = previous_object()->query_nicknames();
  if (!map_nicknames) {
    map_nicknames = ([ ]);
  }
  if (!map_nicknames[s1]) {
    map_nicknames[s1] = s2;
    write("Added nickname "+s1+" for "+s2+".\n");
    return 1;
  }
  write("Changed nickname "+s1+" from "+map_nicknames[s1]+" to "+s2+".\n");
  map_nicknames[s1] = s2;

  this_player()->set_nicknames(map_nicknames);
  return 1;
} /* add_nickname() */

mixed* query_patterns() {
   return ({ "", (: print_nicknames(0) :),
             "sorted", (: print_nicknames(1) :),
             "<string'nickname'>", (: print_single_nickname($4[0]) :),
             "<string'nickname'> <string'realname'>", (: add_nickname($4[0], $4[1]) :)
           });
} /* query_patterns() */
