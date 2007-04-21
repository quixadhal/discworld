/*  -*- LPC -*-  */
/*
 * $Id: valid_write.c,v 1.10 2003/04/19 01:36:14 pinkfish Exp $
 */
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

//#define DEBUG

#define WRITE_FILE_STATS 1

private nosave mapping write_stats;

int valid_write(string path, mixed euid, string func) {
  mapping perms;
  string tmp;
  string *bits, rcspath; 
  string prev;

  if (path && (path[<2..] == ",v")) {
    /* No matter what, never allow direct manipulation of RCS files
     * from the mudlib.  If there is a problem, someone can fix it
     * with the shell account.
     */
    return 0;
  }

  if (func == "remove_file" && path[0..13] == "/save/players/") {
     // Log this for now.
     if (objectp(euid)) {
        log_file("/d/admin/log/DELETE_PLAYER", "Deleting " +
                path + " by " + file_name(previous_object()) +
                "->" + call_stack(2)[0] + "\n");
     } else {
        log_file("/d/admin/log/DELETE_PLAYER", "Deleting " +
                path + " by " + euid + " (" + file_name(previous_object()) +
                "->" + call_stack(2)[0] + ")\n");
     }
  }

  if (euid == master())
    return 1;
  if (path[0] != '/')
    path = "/" + path;

#ifdef WRITE_FILE_STATS 
  // Add some statistics collection.
  if(!write_stats)
    write_stats = ([ ]);
  if(previous_object()) {
    prev = base_name(previous_object());
    
    if(!write_stats[prev])
      write_stats[prev] = ([ path : 1 ]);
    else
      write_stats[prev][path] += 1;
  }
#endif  

  bits = explode(path, "/");
  
  // Added by Ceres to do locking of files for RCS
  // The checks are as follows:
  // If they have an EUID and
  // the file exists and
  // it's not rcsout trying to do the job (that would be bad :) and
  // it's not the ftp daemon and
  // its not a .o file (they aren't in RCS so don't bother checking) and
  // its not a log, save or player file (don't bother checking for these either
  // then lets go check the RCS directory and see if it's in there.
  if(euid && 
     (file_size(path) > 0) &&
     (!previous_object() ||
        file_name(previous_object()) != "/secure/cmds/creator/rcso_ut") &&
     (!objectp(euid) || file_name(euid) != "/secure/ftpd") &&
     path[<2..] != ".o" &&
     (bits[0] != "log" && bits[0] != "save" && bits[0] != "players") ) {
 

    // Figure out the RCS filename and check if it exists.
    if(sizeof(bits) > 2) {
      rcspath = "/" + implode(bits[0..sizeof(bits)-2], "/") + "/RCS/";
    } else if(sizeof(bits) == 2) {
      rcspath = "/" + bits[0] + "/RCS/";
    } else
      rcspath = "/";

    rcspath += bits[sizeof(bits)-1] + ",v";

    if(file_size(rcspath) > 0) { 
#ifdef DEBUG
      if(stringp(euid))
        tell_object(find_player("ceres"), sprintf("Checking perms: %s [%s](string)\n", rcspath, euid ));
      else if(objectp(euid))
        tell_object(find_player("ceres"), sprintf("Checking perms: %s [%s](object)\n", rcspath, file_name(euid )));
      else
        tell_object(find_player("ceres"), sprintf("Checking perms: %s [%O](unknown)\n", rcspath, euid ));
#endif
      tmp = read_file(rcspath, 4, 1);
      if(tmp == "locks\n") { // we've found a lock
        string lockname ;
        tmp = read_file(rcspath, 5, 1);
        sscanf(tmp, "\t%s:", lockname);

        if((objectp(euid) && euid->query_name() != lockname) ||
           (stringp(euid) && euid != lockname)) {
          int i, ok;
          object *stack;
          
          // Ok, so the euid doesn't match. Now we have to check the previous
          // object just in case.
          stack = previous_object(-1);
          i = sizeof(stack);
          while(--i) {
            if(geteuid(stack[i]) == lockname) {
              ok = 1;
            }
          }
          if(!ok) {
            // locked by someone else or no lock exists -- DENIED
#ifdef DEBUG      
          if(objectp(euid))
            tell_object(find_player("ceres"), sprintf("Write denied to %O or %O Locked by %s\n", euid, previous_object(-1), lockname));
          else
            tell_object(find_player("ceres"), sprintf("Write denied to %s or %O Locked by %s\n", euid, previous_object(-1), lockname));
#endif
          return 0;
          
          } else {
#ifdef DEBUG      
            tell_object(find_player("ceres"), "Write allowed to "+geteuid(stack[i])+" Locked by:"+lockname +" on file "+path+"\n");
#endif
            
          }
        } else {
          // continue with normal security checks.
#ifdef DEBUG
          if(objectp(euid))
            tell_object(find_player("ceres"), sprintf("Write allowed by %O Locked by %s\n", euid, lockname));
          else
            tell_object(find_player("ceres"), sprintf("Write allowed by %s Locked by %s\n", euid, lockname));
#endif
        }
      } else {
#ifdef DEBUG
          tell_object(find_player("ceres"), "No locks found\n");
#endif  
          return 0;
      }
    }
  }
  // end of code added by Ceres
  
  perms = permission_match_path(permissions, path);
  return check_permission(euid, func, path, perms, WRITE_MASK);
} /* valid_write() */

mapping query_write_stats() { return copy(write_stats); }

void reset() {
  write_stats = ([ ]);
  read_stats = ([ ]);
}
