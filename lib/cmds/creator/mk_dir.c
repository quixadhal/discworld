/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: mk_dir.c,v 1.1 1998/01/06 05:25:41 ceres Exp $
 * $Log: mk_dir.c,v $
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
/* mkdir command, trial out by Turrican for a commands daemon. */

mixed cmd(string str) {
   seteuid(geteuid(this_player()));

   if (!str || str == "") {
      notify_fail("Make what directory?\n");
      return 0;
      }
   str = this_player()->get_path(str);

   str = replace(str, " ", "_");
   
   if (!str)
      return 1;
 
   if (file_size(str) != -1) {
      notify_fail(str + " already exists.\n");
      return 0;
      }
 
   if (!mkdir(str)) {
      notify_fail("Couldn't make dir.\n");
      return 0;
   }
   write("Ok.\n");
   return 1;
}

void dest_me() {
  destruct(this_object());
}

void clean_up() {
  dest_me();
}

void reset() {
  dest_me();
}
