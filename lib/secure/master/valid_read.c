/*
 * Read permisions for all of the objects in the game.
 * This IS fun.
 */
int valid_read(string path, mixed euid, string func) {
  string *bing;
  string master;

  if (objectp(euid)) euid = geteuid(euid);
  if (query_lord(euid) || high_programmer(euid)) return 1;
  bing = explode(path, "/");
  if (bing)
    bing = bing - ({ "", "." });
  else
    return 1;
  if (!sizeof(bing)) return 1;

  if (euid == "tmp") {
    return bing[0] == "tmp";
  }

  if (check_permission(euid, bing, READ_MASK)) return 1;
  switch (bing[0]) {
    case "players" :
      return (sizeof(bing) < 3) || check_permission(euid, bing, READ_MASK);
    case "ims" :
      return (euid == "ims") || check_permission(euid, bing, READ_MASK);
    case "d" :
    case "w" :
      if (sizeof(bing) >= 2) {
        if ((bing[0]=="w"?bing[1]:capitalize(bing[1])) == euid) return 1;
        master = bing[0] + "/" + bing[1] + "/master";
        if (!find_object(master) && !checked_master[master]
            && catch(master->dead_frogs())) {
          checked_master[master] = 1;
          return 1;
        }
        if (checked_master[master]) return 1;
        return (int) master->valid_read(bing, euid, func);
      }
      return 1;
    case "save" :
      if (sizeof(bing) >= 2 && bing[1] == "post") {
        return (euid == "mailer");
      }
      return (euid == "Room");
  }
  return 1;
} /* valid_read() */
