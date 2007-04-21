/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: history.c,v 1.12 2003/07/15 13:12:40 taffyd Exp $
 * $Log: history.c,v $
 * Revision 1.12  2003/07/15 13:12:40  taffyd
 * Fixed unused local variable.
 *
 * Revision 1.11  2003/06/05 17:26:55  pinkfish
 * Fix up the problem with history.
 *
 * Revision 1.10  2003/06/05 01:59:31  pinkfish
 * Fix it up to use strsrch rather than sscanf to avoid errors with
 * '%'
 *
 * Revision 1.9  2003/02/05 20:29:09  sojan
 * added a this_object()!=this_player(1) check
 * for the inform.
 *
 * Revision 1.8  2003/02/05 19:00:51  pinkfish
 * Add in a thingy so it prints out history printing attempts.
 *
 * Revision 1.7  2002/04/23 20:04:14  pinkfish
 * Allow liaison deputies to view the history.
 *
 * Revision 1.6  2001/11/08 02:20:18  pinkfish
 * Fix up some processing errors.
 *
 * Revision 1.5  1999/10/28 02:28:48  ceres
 * can't remember
 *
 * Revision 1.4  1999/02/11 14:08:13  wodan
 * moved the command to /cmds
 *
 * Revision 1.3  1999/02/03 23:29:15  ceres
 * Fixed runtime in print_history if history size == 0
 *
 * Revision 1.1  1998/01/06 04:54:05  ceres
 * Initial revision
 * 
*/
/*
 * The great and wonderful alias driver by me ;)
 * pinkfish.
 */

#define COLS (int)this_object()->query_cols()

nosave string *history;
nosave int hoffset, numoff;
nosave int in_alias_command;
nosave string ignore_history;

void history_commands() {}

string substitute_history(string arg) {
  string s1, s2;

  if (sscanf(arg, "%s^%s", s1, s2) != 2)
    return arg;
  sscanf(s2, "%s^", s2);
  if (hoffset)
    return replace(history[hoffset-1], s1, s2);
  else
    return replace(history[sizeof(history)-1], s1, s2);
}
 
protected string expand_history( string arg ) {
   int num, i;
   
   if (arg[0] == '.' || strlen(arg) == 0) {
      if (hoffset) {
         return history[hoffset-1]+arg[1..strlen(arg)];
      } else {
         return history[sizeof(history)-1]+arg[1..strlen(arg)];
      }
   }
   if (sscanf(arg,"%d",num)==1) {
      num = num % sizeof(history);
      if (num<0) {
         num += hoffset;
         if (num<0) {
            num += sizeof(history);
         }
      }
      return history[num];
   }
   for (i=hoffset-1;i>=0;i--) {
      if (stringp(history[i])) {
         if (strsrch(history[i],arg)==0) {
            return history[i];
         }
      }
   }
   for (i=sizeof(history)-1;i>hoffset;i--) {
      if (stringp(history[i])) {
         if (strsrch(history[i],arg)==0) {
            return history[i];
         }
      }
   }
   return arg;
} /* expand_history() */

void ignore_from_history(string str) {
  ignore_history = str;
}
 
void add_history( string arg ) {
   if (ignore_history) {
      if (ignore_history == arg) {
         ignore_history = 0;
         return ;
      }
      ignore_history = 0;
   }
   if (arg == "" || arg[0] == '.' || arg[0] == '^')
      return;
   if (in_alias_command)
      return;
   if (!history) {
      history = allocate(40);
      numoff = -40;
   }
   history[hoffset++] = arg;
   if (hoffset>=sizeof(history)) {
      numoff += sizeof(history);
      hoffset = 0;
   }
} /* add_history() */

nomask int print_history( string arg ) {
   int i, from, to, num;
   
   if(this_player()!=this_object()) { /* a call */
      if(!interactive(this_player())) {
         return 0;
      }
      if(!(master()->query_lord(geteuid(this_player()))) &&
         !master()->is_liaison_deputy(geteuid(this_player()))) {
         return 0;
      }
   }

  if(this_object()!=this_player(1)) {
   user_event( this_object(), "inform",
               "Print history called on " + this_object()->query_name() +
               " by " + this_player()->query_name(),
               "calls", this_player());
  }

   from = hoffset;
   num = sizeof(history);
   if (!arg) {
      arg = "";
   }
   if (sscanf(arg,"%d,%d",from,to)==2) {
      num = to-from;
      if (num<0) {
         from = from+num;
         num = -num;
      }
   } else if (sscanf(arg,",%d",to)==1) {
      from = hoffset+1+num;
      num = from - (to%sizeof(history))+sizeof(history);
   } else if (sscanf(arg,"%d,",from)==1) {
      from = from % sizeof(history);
      num = from - hoffset;
      if (num<0) {
         num += sizeof(history);
      }
   } else if (sscanf(arg,"%d",num)!=1) {
         num = sizeof(history);
   }
   if(sizeof(history)) {
     from = from % sizeof(history);
   } else {
     from = 0;
   }
   
   if (num>sizeof(history)) {
     num = sizeof(history);
   }
   if (num <= 0) {
     add_failed_mess("Range out of bounds.\n");
     return 0;
   }
   
   for (i=from;i<sizeof(history);i++,num--) {
     if (history[i]) {
       printf( "%s%-=*s\n", sprintf( "% 4d", i + numoff ) +": ", COLS - 6,
               history[ i ] );
     }
     if (!num) {
       return 1;
     }
   }
   for (i=0;i<from;i++,num--) {
     if (history[i]) {
       printf( "%s%-=*s\n", sprintf( "% 4d", i + numoff + sizeof( history ) ) +
               ": ", COLS - 6, history[ i ] );
     }
     if (!num) {
       return 1;
     }
   }
   return 1;
} /* print_history() */

nomask protected string *get_history() {
  return history;
}
