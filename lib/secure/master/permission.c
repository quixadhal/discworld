int add_lord(string str) {
  if ("/secure/login"->test_user(str) &&
    high_programmer(geteuid(previous_object()))) {
    positions[str] = LORD;
    save_object("/secure/master");
    return 1;
  }
  return 0;
} /* add_lord() */

int remove_lord(string str) {
  if (high_programmer(geteuid(previous_object())) &&
    positions[str] == LORD) {
    map_delete(positions, str);
    save_object("/secure/master");
    return 1;
  }
  return 0;
} /* remove_lord() */

int add_high_lord(string str) {
  if ("/secure/login"->test_user(str) &&
    high_programmer(geteuid(previous_object()))) {
    positions[str] = HIGH_LORD;
    save_object("/secure/master");
    return 1;
  }
  return 0;
} /* add_high_lord() */

int remove_high_lord(string str) {
  if (high_programmer(geteuid(previous_object())) &&
    positions[str] == HIGH_LORD) {
    map_delete(positions, str);
    save_object("/secure/master");
    return 1;
  }
  return 0;
} /* remove_high_lord() */


/* Permision handling stuff */

int check_permission(string euid, string *path, int mask) {
/*
 * Permission to /foo gives permission to /foo/bar/gnu too!
 */
  string p, master;
  int    i;

  p = "";
  if (path[0] == "d" && sizeof(path) >= 2) {
    master = "/d/"+path[1]+"/master";
    if (!find_object(master) && checked_master[master])
      return 0;
    if (!find_object(master) && !checked_master[master])
      if (catch(master->bright_blue())) {
        checked_master[master] = 1;
        return 0;
      }
    return (int)master->check_permission(euid, path, mask);
  }
  for (i=0;i<sizeof(path);i++) {
    if (permissions[p] && permissions[p][euid] & mask)
      return 1;
    p = p + "/" + path[i];
  }
  return (permissions[p] && permissions[p][euid] & mask);
} /* check_perminssion() */

mapping query_permissions() {
  string *doms, master;
  int i;
  mapping blue, tmp;

  doms = get_dir("/d/");
  blue = ([ ]);
  for (i=0;i<sizeof(doms);i++) {
    master = "/d/"+doms[i]+"/master";
    if (!find_object(master) && checked_master[master])
      continue;
    if (!find_object(master) && !checked_master[master])
      if (catch(master->bright_blue())) {
        checked_master[master] = 1;
        continue;
      }
    tmp = (mapping)master->query_access();
    if (mappingp(tmp))
      blue += tmp;
  }
  return permissions + blue;
} /* query_permissions() */

int add_permission(string euid, string path, int mask) {
  string *bits, master;

  if (high_programmer(geteuid(previous_object()))) {
    if (path[0..2] == "/d/") {
/* A domain...  */
      bits = explode(path, "/");
      if (sizeof(bits) >= 2) {
        master = "/d/"+bits[1]+"/master";
        if (!find_object(master) && checked_master[master])
          return 0;
        if (!find_object(master) && !checked_master[master])
          if (catch(master->bright_blue())) {
            checked_master[master] = 1;
            return 0;
          }
        return (int)master->add_permission(euid, path, mask);
      }
    }
    if (!permissions[path]) {
      permissions[path] = ([ euid : mask ]);
    } else {
      permissions[path][euid] |= mask;
    }
    save_object("/secure/master");
    return 1;
  }
  return 0;
} /* add_permission() */

int add_read_permission(string euid, string path) {
  if (add_permission(euid, path, READ_MASK)) {
    write("Added read permision for "+euid+" to "+path+".\n");
    return 1;
  }
  return 0;
} /* add_read_permission() */

int add_write_permission(string euid, string path) {
  if (add_permission(euid, path, WRITE_MASK)) {
    write("Added write permision for "+euid+" to "+path+".\n");
    return 1;
  }
  return 0;
} /* add_write_permission() */

int add_grant_permission(string euid, string path) {
  if (add_permission(euid, path, GRANT_MASK)) {
    write("Added write permision for "+euid+" to "+path+".\n");
    return 1;
  }
  return 0;
} /* add_grant_permission() */

int remove_permission(string euid, string path, int mask) {
  string *bits, master;

  if (high_programmer(geteuid(previous_object()))) {
    if (path[0..2] == "/d/") {
/* A domain...  */
      bits = explode(path, "/");
      if (sizeof(bits) >= 2) {
        master = "/d/"+path[1]+"/master";
        if (!find_object(master) && checked_master[master])
          return 0;
        if (!find_object(master) && !checked_master[master])
          if (catch(master->bright_blue())) {
            checked_master[master] = 1;
            return 0;
          }
        return (int)master->remove_permission(euid, path, mask);
      }
    }
    if (!permissions[path] || !permissions[path][euid])
      return 0;
    permissions[path][euid] &= ~mask;
    if (!permissions[path][euid]) {
      if (m_sizeof(permissions[path]) == 1) {
        map_delete(permissions, path);
      } else {
        map_delete(permissions[path], euid);
      }
    }
    save_object("/secure/master");
    return 1;
  }
  return 0;
} /* remove_permission() */

int remove_read_permission(string euid, string path) {
  if (remove_permission(euid, path, READ_MASK)) {
    write("Removed read permision for "+euid+" to "+path+".\n");
    return 1;
  }
  return 0;
} /* remove_read_permission() */

int remove_write_permission(string euid, string path) {
  if (remove_permission(euid, path, WRITE_MASK)) {
    write("Removed write permision for "+euid+" to "+path+".\n");
    return 1;
  }
  return 0;
} /* remove_write_permission() */

int remove_grant_permission(string euid, string path) {
  if (remove_permission(euid, path, GRANT_MASK)) {
    write("Removed write permision for "+euid+" to "+path+".\n");
    return 1;
  }
  return 0;
} /* remove_write_permission() */
