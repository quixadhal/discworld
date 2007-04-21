/*  -*- LPC -*-  */
/*
 * $Locker:  $
 */

#define MAX_FILES 1000

inherit "/cmds/base";

mapping _counts = ([ ]);

protected int rec_du(string path) {
  string *files;
  int i, size, tot;

  if (path[strlen(path)-1] != '/') {
    path += "/";
  }
  files = get_dir(path+"*");
  _counts[this_player()] += sizeof(files);
  for (i=0;i<sizeof(files);i++) {
    if (files[i] == "." || files[i] == "..") {
      continue;
    }
    size = file_size(path+files[i]);
    if (size > 0) {
      tot += size;
    } else if (size == -2 && (_counts[this_player()] < MAX_FILES)) {
      printf("%-30s %5d\n", path+files[i], (size = rec_du(path+files[i])));
      tot += size*1024;
    }
  }
  return (tot+1023)/1024;
}

int cmd( string path ) {
    if ( !path ) 
        return 0;

    _counts[this_player()] = 0;
    if ( master()->valid_read( path, geteuid( this_player() ) ) ) {
      printf( "%-30s %5d\n", "Total:", rec_du( path ) );
      if(_counts[this_player()] >= MAX_FILES)
        printf("Note: count truncated due to file limit\n");
      return 1;
    }
    add_failed_mess( "You must have read access to a path to use " 
                     "du on it.\n" );
    return 0;
} /* cmd() */

mixed *query_patterns() {
    return ({ "", (: cmd( this_player()->query_path() ) :),
        "<string'path'>", (: cmd( $4[0] ) :) });
} /* query_patterns() */
