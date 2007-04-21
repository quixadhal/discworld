/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: compile.c,v 1.1 1998/01/06 05:10:49 ceres Exp $
 * $Log: compile.c,v $
 * Revision 1.1  1998/01/06 05:10:49  ceres
 * Initial revision
 * 
*/
/* compile command, trial out by Turrican for a commands daemon. */

inherit "/cmds/base";

mixed cmd(string str) {
  string bit, *bits, error_str;

  if (str)
    bits = this_player()->get_files(str);

  if (!str || !sizeof(bits))
    return notify_fail("Syntax: compile <files>\n");

#ifdef __RUNTIME_LOADING__
  foreach (bit in bits) {
    error_str = catch(generate_source(bit));
    if (!error_str)
      printf("Compiled %s Ok.\n", bit);
    else
      printf("Error compiling %s: %s\n", bit, error_str);
  }
#else
  error_str = catch(generate_source(bits));
  if (!error_str)
    printf("Compiled %@s OK.\n", bits);
  else
    printf("Error compiling %@s: %s\n", bits, error_str);
#endif
  return 1;
}
