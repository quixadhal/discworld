/*
 * This controls the write permisions for everything in the mud.
 * It defaults to 0 if all else fails.
 * Any permisions can be overridden by the global permision giving/
 * changeing code.
 *
 * The master.c's in the domain and wizard dirs will be checked if
 * any access is done to them.  This is a very flexible system,
 * but perhaps a little hard for some people to use.
 */
int valid_write(string path, mixed euid, string func) {
  string *bing;
  string  master;

  if (objectp(euid)) euid = geteuid(euid);
  if (high_programmer(euid)) return 1;
  bing = explode(path, "/");
  if (bing)
    bing = bing - ({ ".", "" });
  else
    return 0;

  if (euid == "tmp") {
      return bing[0] == "tmp";
  }
  if (bing[sizeof(bing)-1] == ERROR_LOG) return 1;

  if (check_permission(euid, bing, WRITE_MASK)) return 1;
  switch (bing[0]) {
    case "log" :
      if (func == "log_file") {
        return 1;
      }
      break;
    case "open" :
    case "tmp" :
      return 1;
    case "net" :
      return euid == "Network stuff";
    case "ims" :
    case "obj" :
      if (euid == "ims") {
        return 1;
      }
      break;
    case "w" :
    case "d" :
      if ((bing[0]=="w"?bing[1]:capitalize(bing[1])) == euid ||
        query_lord(euid) || sizeof(bing) == 1) {
        return 1;
      }
      master = bing[0] + "/" + bing[1] + "/master";
      if (find_object(master) && !checked_master[master]
          && !catch(master->dead_frogs())) {
        return (find_object(master) == previous_object() ||
               find_object(master)->valid_write(bing, euid, func));
      }
      checked_master[master] = 1;
      break;
    case "save" :
      if (euid == "Room" ||
	  (sizeof(bing) == 3 && euid == "mailer" && bing[1] == "post")) {
        return 1;
      }
  }
  return 0;
} /* valid_write() */
