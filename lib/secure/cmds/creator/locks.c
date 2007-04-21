#include <creator.h>

int cmd(string){
  string *bits, rcspath, *files, path, tmp;
  
  files = get_dir(this_player()->query_current_path()+"/");
  write("locked files in this directory:\n");
  foreach(path in files) {
    path = this_player()->query_current_path() + "/" + path;
    bits = explode(path, "/");
    
    // Figure out the RCS filename and check if it exists.
    if(sizeof(bits) > 2) {
      rcspath = "/" + implode(bits[0..sizeof(bits)-2], "/") + "/RCS/";
    } else if(sizeof(bits) == 2) {
      rcspath = "/" + bits[0] + "/RCS/";
    } else
      rcspath = "/";
    
    rcspath += bits[sizeof(bits)-1] + ",v";
    
    if(file_size(rcspath) > 0) { 
      tmp = read_file(rcspath, 4, 1);
      if(tmp == "locks\n") { // we've found a lock
        string lockname ;
        tmp = read_file(rcspath, 5, 1);
        sscanf(tmp, "\t%s:", lockname);
        printf("%s locked by %s\n", path, lockname);
      }
    }
  }
  return 1;
} /* valid_write() */

string help() {
  return "Displays the files that are locked in your current directory.";
}
