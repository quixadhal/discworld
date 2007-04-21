/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: tcheck.c,v 1.1 1998/01/06 05:25:41 ceres Exp $
 * $Log: tcheck.c,v $
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
/* tcheck command. 
 * It doesn't do nearly all the checks I though it would, but it
 * still finds some.
 */

int cmd( string str ) {
   mixed  err, ret;
   object ob;
   string file, wiz_dir;
   
   if (!this_player()) return 0;
   seteuid(geteuid(this_player()));
   if (!str) {
      notify_fail("Usage: tcheck <file name>\n");
      return 0;
   }   
   wiz_dir = "/w/" + (string)this_player()->query_name();
   if (file_size(wiz_dir)!=-2) {
      notify_fail("Directory: " + wiz_dir + " does not exist.\n");
      return 0;
   }

   if ( str[0..0] != "/" )
     str = (string)this_player()->query_path() + "/" + str;

   if (str[<1..<1]=="0") {
      str=str[0..<2];
   }
   
   if (file_size(str)<0) {
      notify_fail("File: " + str + " does not exist or is a directory.\n");
      return 0;
   }
   file = wiz_dir + "/exec_tmp";
   if (find_object(file)) file->dest_me();
   if (file_size(file+".c")>0) rm(file+".c");
   write_file(file+".c",
              "#pragma strict_types\n" +
              "#pragma save_types\n" +
              "#include \""+ str +"\"\n" +
              "void dest_me_in_exec_temp() { destruct(this_object()); }\n" +
              "\n");
   err = catch(ret = (mixed) file->force_load());
   if (err==0) printf("%s loaded ok.\n", str);
   if (find_object(file)) file->dest_me_in_exec_temp();
   rm(file+".c");
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
