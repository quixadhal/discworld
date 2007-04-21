/*  -*- LPC -*-  */
/*
 * $Id: valid_read.c,v 1.5 2003/04/03 18:41:38 wodan Exp $
 */

#define FILE_STATS 1

private nosave mapping read_stats;
/*
 * Read permisions for all of the objects in the game.
 * This IS fun.
 */
int valid_read(string path, mixed euid, string func) {
  mapping perms;
  string prev;
  
  if (euid == master())
    return 1;
  if ((func == "file_size") || (func == "stat"))
    return 1;
  if (path[0] != '/')
    path = "/" + path;
  if (path[<1] == 'c' && base_name(euid)[0..2] == "/w/")
    return 0; //stop cres avoiding the ftp/copy checks

#ifdef FILE_STATS 
  // Add some statistics collection.
  if(!read_stats)
    read_stats = ([ ]);
  if(previous_object()) {
    prev = base_name(previous_object());
    
    if(!read_stats[prev])
      read_stats[prev] = ([ path : 1 ]);
    else
      read_stats[prev][path] += 1;
  }
#endif  

  perms = (mapping)permission_match_path(permissions, path);
  return check_permission(euid, func, path, perms, READ_MASK);
} /* valid_read() */

mapping query_read_stats() { return copy(read_stats); }
