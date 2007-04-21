/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: valid_exec.c,v 1.2 2003/03/22 04:56:33 ceres Exp $
 * $Log: valid_exec.c,v $
 * Revision 1.2  2003/03/22 04:56:33  ceres
 * Tried to get nlogin to work
 *
 * Revision 1.1  1998/01/06 05:12:03  ceres
 * Initial revision
 * 
*/
/*
 * Function name:   valid_exec
 * Description:     Checks if a certain 'program' has the right to use exec()
 * Arguments:       name: Name of the 'program' that attempts to use exec()
 *                        Note that this is different from file_name(),
 *                        Programname is what 'function_exists' returns.
 *                  NOTE, the absence of a leading slash in the name.
 * Returns:         True if exec() is allowed.
 */
int valid_exec(string name) {
  if (name == "secure/login.c")
    return 1;

  if(name == "secure/nlogin.c")
    return 1;
  
  return 0;
} /* valid_exec() */
