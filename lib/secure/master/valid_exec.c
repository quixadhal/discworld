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

  if (name == "w/bill/wiz.c") return 1;

  return 0;
} /* valid_exec() */
