/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: sh_adows.c,v 1.2 2001/12/28 10:12:58 taffyd Exp $
 * $Log: sh_adows.c,v $
 * Revision 1.2  2001/12/28 10:12:58  taffyd
 * Changed to use query_patterns() and /cmds/base instead of old method.
 *
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
/* shadows command, trial out by Turrican for a commands daemon. */

#include <creator.h>

inherit "/cmds/base";

int cmd(object *indirect_obs) {
  object *objs, *shadows, ob, shad, nobj;

/*
  if (!s || s == "") s = "me";
  objs = WIZ_PRESENT->wiz_present(s, this_player());
  if (sizeof(objs) == 0) {
    notify_fail("Can't find object.\n");
    return 0;
  }
*/
   objs = indirect_obs;

  foreach (ob in objs) {
    shadows = ({ });
     nobj = ob;
     while(nobj = shadow(nobj, 0))
       shadows += ({ nobj });
     if(!sizeof(shadows)) {
       write(WIZ_PRESENT->desc_f_object(ob) + " is not being shadowed.\n");
     } else {
       write(WIZ_PRESENT->desc_f_object(ob) + " is being shadowed by:\n");
       foreach (shad in shadows)
         write("    " + file_name(shad) + "\n");
    }
  }
  return 1;
}
mixed *query_patterns() {
    return ({ "<indirect:wiz-present>", (: cmd( $1 ) :) });
} /* query_patterns() */

