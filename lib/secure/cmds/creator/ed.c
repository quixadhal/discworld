/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: ed.c,v 1.7 2000/07/17 02:58:13 presto Exp $
 * $Log: ed.c,v $
 * Revision 1.7  2000/07/17 02:58:13  presto
 * Fix up str arg passed to do_edit when using "magic"
 *
 * Revision 1.6  1998/10/02 01:21:15  presto
 * changed so that "ed  bing" doesn't try to open a file named " bing"
 *
 * Revision 1.5  1998/03/25 16:01:58  turrican
 * Moved from /cmds/creator
 *
 * Revision 1.4  1998/01/19 00:04:06  presto
 * Put it back the way it was
 *
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
/* ed command, trial out by Turrican for a commands daemon. */

#include <creator.h>

inherit "/cmds/base";

object *people = ({ });

mixed cmd(string str) {
  string *filenames, spam;
  object *things;

  people += ({ this_player() });

  if ( this_player()->query_editor() == "magic" )  {
    // set str to " " so that 'ed' with no argument gives you the correct
    // command set
    if ( !str )  str = " ";
    this_player()->do_edit( 0, "fini_editor", 0, str );
    return 1;
  }
  if (!str) {
    this_player()->set_in_editor("(hidden)");
    ed("frog", "fini_editor");
    return 1;
  }
  /* dodgy idea, but allows 'ed here' or 'ed strawberry' */        
  if (sizeof(things = WIZ_PRESENT->wiz_present(str, this_player()))) {
    spam = file_name(things[0]);
    sscanf(spam, "%s#%*d", spam);
    if (file_size(spam) < 0)
      filenames = ({ spam+".c" });
    else
      filenames = ({ spam });
  } else
    filenames = this_player()->get_files(str);
  if (!sizeof(filenames)) { 
    str = this_player()->get_path(str);
  } else {
    if (sizeof(filenames) > 0) {
      str = filenames[0];
      if (sizeof(filenames) > 1) {
        int loop;

        loop = 0;
        while(loop < sizeof(filenames) && file_size(filenames[loop]) < 0)
          loop++;
        if(loop >= sizeof(filenames)) {
          printf("No such file.\n");
          people -= ({ this_player() });
          return 0;
        }
        else {
          str = filenames[loop];
        } 
        printf("Ambiguous, using : %s\n", str);
      }
    }
  }
  if (file_size(str) == -2) {
    printf("directory\n");
    people -= ({ this_player() });
    return 0;
  }
  this_player()->set_in_editor(str);
  printf("Editing: %s ", str);
  if (!master()->valid_write(str, geteuid(), "frog"))
    printf("[read only]\n");
  else
    printf("\n");
  ed(str, "fini_editor");
  return 1;
}

void fini_editor() {
  if (objectp(this_player()))  {
    people -= ({ this_player() });
    this_player()->set_in_editor(0);
  }
} /* fini_editor() */


int clean_up()  {
   people -= ({ 0 });
   if ( !sizeof( people ) )  {
      ::clean_up();
   }
   return 1;
}

void reset()  {
   people -= ({ 0 });
   if ( !sizeof( people ) )  {
      ::reset();
   }
   return;
}
