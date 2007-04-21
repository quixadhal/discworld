/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: ca_ll.c,v 1.5 2001/06/08 02:16:26 presto Exp $
 * $Log: ca_ll.c,v $
 * Revision 1.5  2001/06/08 02:16:26  presto
 * Fixed type warnings
 *
 * Revision 1.4  2001/05/23 00:49:12  presto
 * Type error: retobj was object, should have been mixed
 *
 * Revision 1.3  2000/07/02 10:15:14  taffyd
 * Added a reference_allowed() call.
 *
 * Revision 1.2  1999/04/08 11:07:38  terano
 * Removed no score check from informs
 *
 * Revision 1.1  1998/01/06 05:10:49  ceres
 * Initial revision
 * 
*/
/* call command, trial out by Turrican for a commands daemon. */

#include <creator.h>
#include <parser.h>

#define LMASTER "/d/liaison/master"

inherit "/cmds/base";

mixed cmd(string str) {
  /* call fish(x,y,z) object */
  mixed *args, file, f;
  mixed retobj;
  string *s, s1, s2;
  string fn,os, obvarname;
  string *argv;
  object *ov;
  object fish, shad, ob;

  notify_fail("USAGE : call [obvar=]lfun(arg[,arg[,arg...]]) object[s]\n");
  if (!str)
    return 0;
  obvarname = (string)0;
  sscanf(str, "%s=%s", obvarname, str);
  s = explode("&"+str+"&", ")");
  if (sizeof(s) < 2 || sscanf(s[0], "%s(%s", s1, s2) != 2)
    return 0;
  fn = replace(s1[1..], " ", "");
  s[0] = s2;
  args = PARSER->parse_args(implode(s, ")"), ")");
  if (!args) return 1;
  argv = args[0];
  os = args[1][0..<2];
  while (strlen(os) && os[0] == ' ')
    os = os[1..];
  notify_fail("Can't find object "+os+".\n");
  ov = WIZ_PRESENT->wiz_present(os, this_player());
  
  ov = filter( ov, (: reference_allowed( $1, $( this_player() ) ) :) );  

  if (!sizeof(ov)) return 0;

  if (sizeof(argv) < 6) argv += allocate(6 - sizeof(argv));

  foreach (ob in ov) {
    fish = ob;
    while (shad = shadow(fish, 0)) {
      fish = shad;
      if (f = function_exists(fn, fish)) file = f;
    }
    if (!file) file = function_exists(fn, ob);
    if (file) {
  if ( ( ob != this_player() && ob->query_property( "player" ) ) ) {
        unguarded((: write_file,
                   (LMASTER->query_member(this_player()->query_name()) ?
                    "/d/admin/log/CALL_LIAISONS.log" :
                    "/d/admin/log/CALL_CREATORS.log" ),
                   sprintf("%s: %s (%O) called %s(%s) on %s (%O)\n",
                           ctime(time()),
                           this_player()->query_name(), this_player(),
                           fn, implode(argv - ({0}), ", "),
        ob->query_name(), ob) :));
        user_event("inform", sprintf("%s called %s(%s) on %s",
            this_player()->query_name(), fn, implode(argv - ({0}), ", "),
            ob->query_name()), "calls");
      }
      retobj = call_other(ob,fn,argv...);
/*      inform_of_call(ov[i], ({ fn }) + argv); */
      printf("*** function on '%s' found in %s ***\n",
          WIZ_PRESENT->desc_object(ob), file);
      write(sprintf("$P$Call result$P$Returned: %O\n", retobj));
      if (obvarname) {
        if (objectp(retobj)) this_player()->set_obvar(obvarname, retobj);
        /* others could be done ... like arrays etc, but phuck it */
      }
    } else
      printf("*** function on '%s' Not found ***\n",
          WIZ_PRESENT->desc_object(ob));
    file = 0;
  }
  return 1;
}
