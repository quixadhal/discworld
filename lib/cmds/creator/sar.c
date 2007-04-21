/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: sar.c,v 1.3 1999/10/24 19:19:22 wodan Exp $
 */
/*
  Nanvaent Industries International
  
  LPC: Search and Replace

  Written by  : Weazel@Nanvaent Nov 92
  Modified by : Bill@Nanvaent Jan 93
  Modified by : Bill@Nanvaent+@Discworld Mar 93
*/
/* sar command, trial out by Turrican for a commands daemon. */

#include <creator.h>

mixed cmd(string str) {
  int i;
  string *files;
  string s1, s2, s3, s4;
  mixed *things;

  seteuid(geteuid(this_player()));
  if ( !str ) {
    notify_fail( "Usage: sar <search_string> <replace_string> <files>\n" );
    return 0;
  }
  s4 = str[ 0 .. 0 ];
  if ( sscanf( str, s4 +"%s"+ s4 +" "+ s4 +"%s"+ s4 +" %s", s1, s2, s3 ) != 3 )
    if ( sscanf( str, "%s %s %s", s1, s2, s3 ) != 3 ) {
      notify_fail( "Usage: sar <search_string> <replace_string> <files>\n" );
      return 0;
    }

  if(strsrch(s3, "*") == -1) {
    if(sizeof(things = WIZ_PRESENT->wiz_present(s3, this_player()))) {
      s3 = file_name(things[0]);
      sscanf(s3, "%s#%*d", s3);
      if (file_size(s3) <= 0)
        s3 += ".c";
    }
  }
  
  files = (string *)this_player()->get_files(s3);
  if(!sizeof(files))
  {
    notify_fail("File(s) " + s3 + " not found.\n");
    return 0;
  }
  s1 = replace( s1, "^M", "\n" );
  s2 = replace( s2, "^M", "\n" );
  for (i=0;i<sizeof(files);i++)
  {
    if (file_size(files[i]) <= 0) continue;
    printf("Looking at %s.\n", files[i]);
    s4 = read_file(files[i]);
    if (s4)
    {
      s4 = replace(s4, s1, s2);
      rm(files[i]);
      write_file(files[i], s4);
    }
    else
      printf("...failed...no file\n");
  }
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
