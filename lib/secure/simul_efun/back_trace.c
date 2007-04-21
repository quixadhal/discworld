/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: back_trace.c,v 1.2 1998/03/24 07:28:59 pinkfish Exp $
 * $Log: back_trace.c,v $
 * Revision 1.2  1998/03/24 07:28:59  pinkfish
 * Add some docs to make the simulefuns more usefully lookupable.
 *
 * Revision 1.1  1998/01/06 05:13:15  ceres
 * Initial revision
 * 
*/
/**
 * Cotains some simul_efuns.
 * @author Pinkfish
 */
/**
 * This method returns out the call stack traces.  It is usualy
 * called in the error handling routines in the master object
 * but it can be used elsewhere for debuggin purposes.  You
 * can see the results of this function in /log/runtime and
 * /log/catch.
 * @return call stack trace
 */
string back_trace() {
  string ret = "", *progs, *funcs, *obs, name;
  int i;

  progs = map(call_stack(0), (: $1[1..<3] :));
  obs = map(call_stack(1), (: file_name($1)[1..] :));
  funcs = call_stack(2);
  i = sizeof(progs);


  ret = "Time: " + ctime(time()) + "\n";
  
  while (i-- > 1) {
    if (objectp(obs[i]))
      name = obs[i]->query_name();
    if (!name)
      name = "null";
    if (obs[i] == progs[i])
      ret += sprintf("%s() in /%s (%s)\n", funcs[i], progs[i], name);
    else
      ret += sprintf("%s() in inherited file /%s in /%s (%s)\n", funcs[i],
                     progs[i], obs[i], name);
    name = 0;
  }
  return ret;
} /* back_trace() */
