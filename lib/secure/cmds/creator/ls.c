/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: ls.c,v 1.11 2001/06/07 06:45:06 taffyd Exp $
 * 
 */
#include <ls.h>
#include <autodoc.h>

inherit "/cmds/base";

/* this does an ls, now that get_dir has been written 
 * Modified by Turrican 13-7-95 with nicked code from TMI-2's ftpd.
 * - Added different options.
 * Modified by Turrican 3-12-97.
 * - Made ls -CF the default format. The old 1 column format is available
 *   with the -1 flag.
 */

#define CREATOR (master()->author_file(sprintf("%s/%s", str, direc[i][0]))? \
                master()->author_file(sprintf("%s/%s", str, direc[i][0])): \
                "Root")
#define DOMAIN  (master()->domain_file(sprintf("%s/%s", str, direc[i][0]))? \
                master()->domain_file(sprintf("%s/%s", str, direc[i][0])): \
                "Root")
#define CREATOR_D (master()->author_file(sprintf("%s/%s/.", str, direc[i][0]))? \
                master()->author_file(sprintf("%s/%s/.", str, direc[i][0])): \
                "Root")
#define DOMAIN_D (master()->domain_file(sprintf("%s/%s/.", str, direc[i][0]))? \
                master()->domain_file(sprintf("%s/%s/.", str, direc[i][0])): \
                "Root")

int ls(string str, int mask);
int check_dots(mixed arg);

mixed cmd(string str) {
  string *bits, path;
  int mask, i;

  mask = (MASK_C|MASK_F);
  if (str) {
      if ( this_player()->query_property( LS_COMMAND_NICKNAME_PROPERTY ) ) {
         str = this_player()->expand_nickname( str );
      }

    bits = explode(str, " ") - ({"", 0});
    while (sizeof(bits) && bits[0][0] == '-') {
      i = strlen(bits[0]);
      while (i--) {
        if (bits[0][i] == '-') continue;
        switch (bits[0][i]) {
        case 't':
          mask |= MASK_T;
          break;
        case 'l':
          mask |= MASK_L;
          break;
        case 'C':
          mask |= MASK_C;
          break;
        case 'a':
          mask |= MASK_A;
          break;
        case 'd':
          mask |= MASK_D;
          break;
        case 'o':
          mask |= MASK_O;
          break;
        case 'F':
          mask |= MASK_F;
          break;
        case '1':
          mask &= ~MASK_C;
          break;
        case 'v':
          write("ls version 2.6 (c) 1995-1998 Turrican@Discworld\n");
          return 1;
        case 'h':
          printf("Usage: ls [OPTION]... [PATH]...\n\n"
                 "  -a      do not hide entries \"..\" and \".\"\n"
                 "  -d      list directory entries instead of contents\n"
                 "  -h      display this help and exit\n"
                 "  -l      use a long listing format\n"
                 "  -o      colorize entries according to type\n"
                 "  -v      print version information and exit\n"
                 "  -C      list entries by columns\n"
                 "  -F      append a character to entries according to type\n"
                 "  -1      list one file per line\n"
                 "  -t      sort by date\n"
            );
          return 1;
        default :
          printf("Unknown option -%c\nTry `ls -h' for more information.\n",
                 bits[0][i]);
          return 1;
        }
      }
      bits = delete(bits, 0, 1);
    }
    str = implode(bits, " ");
  }
  if (!str) {
    path = this_player()->query_path();
  } else {
    path = this_player()->get_path(str);
  }
  if (!path) {
    return notify_fail("No current directory.\n");
  }
  if (master()->valid_read(path+"/", geteuid(this_player()), "get_dir")) {
    ls(path, mask);
  } else {
    return notify_fail("$I$0=ls: "+ str + ": Permission denied.\n");
  }
  return 1;
} /* cmd() */

string dir_entry(string path, string name, int size, int mask) {
  string tmp;
  
  tmp = "";
  
  if(size == -2) {
    if(mask & MASK_F)
      name += "/";

    return "   - " + name;
    
  }
    
  if (find_object(path+name)) {
    if (mask & MASK_F) {
      tmp += "*";
    }
  }
    
  /* Autodoc stuff, so it shows up as in or out of the doc system. */
  if ((mask & MASK_F) && AUTODOC_HANDLER->is_autodoc_file(path + name)) {
    tmp += "@";
  }
  /* Put the extension bits on... */
  name += tmp;
  
  size = (size / 1024) + 1;

  if (size < 1000) {
    return "    "[0..3-strlen(size+"")]+size+" "+name;
  }

  return size + " "+name;
} /* dir_entry() */

int ls(string str, int mask) {
  string *bit, *bing, bong, path;
  int i, j, k, size;
  mixed *direc;
 
  path = str;

  if (file_size(str) == -2 && str[strlen(str)-1] != '/' &&
      !(mask & MASK_D)) {
    path += "/";
  }
  if (mask & MASK_A) {
    path += "*";
  }
  direc = get_dir(path, -1);

  if(arrayp(direc)) {
    if (!(mask & MASK_A)) {
      direc = filter(direc, (: check_dots($1) :));
    }
  }
  if (!direc) {
    printf("No files.\n");
    return 0;
  }
  if (!sizeof(direc)) {
    if (file_size(str) == -2) {
      printf("No files.\n");
    } else {
      printf("ls: %s: No such file or directory.\n", str);
    }
    return 0;
  }
  if (file_size(path) == -2) {
    if (path[strlen(path)-1] != '/') {
      path += "/";
    }
  } else {
    bit = explode(path, "/");
    bit = bit[0..sizeof(bit)-2];
    path = "/"+implode(bit,"/")+"/";
  }
  if(path == "//")
    path = "/";
  
  bing = allocate(sizeof(direc));
  j = sizeof(direc);

  if (!(mask & MASK_C) && !(mask & MASK_L)) { // short, no colour
    if (mask & MASK_T)  {
      direc = sort_array(direc, (: $1[2] < $2[2] ? -1 :
                                   $1[2] > $2[2] ?  1 : 0 :));
    }
    if (!(mask & MASK_F) && !(mask & MASK_O)) {
      bong = sprintf("%-=*s\n", this_player()->query_cols(),
        implode(direc, "\n"));
      bing = explode(bong, "\n");
    } else for (i=0; i < j; i++) {
      if (direc[i][1] == -2 || direc[i][0] == "..") {
        if (mask & MASK_O) {
          bing[i] = sprintf("%s%-=*s", "%^GREEN%^",
            (int)this_player()->query_cols()+8, direc[i][0]+"%^RESET%^"+
            (mask & MASK_F?"/":""));
        } else  {
          bing[i] = sprintf("%-=*s", this_player()->query_cols(),
            direc[i][0]+"/");
        }
      } else if (find_object(path+direc[i][0])) {
        if (mask & MASK_O) {
          bing[i] = sprintf("%s%-=*s", "%^MAGENTA%^",
            (int)this_player()->query_cols()-1, direc[i][0]+"%^RESET%^"+
            (mask & MASK_F?"*":"")+
            ((mask & MASK_F) && AUTODOC_HANDLER->is_autodoc_file(path+direc[i][0])
             ?"@":""));
        } else {
          bing[i] = sprintf("%-=*s", this_player()->query_cols(),
            direc[i][0]+"*"+
            ((mask & MASK_F) && AUTODOC_HANDLER->is_autodoc_file(path+direc[i][0])
             ?"@":""));
        }
      } else {
        bing[i] = sprintf("%-=*s", this_player()->query_cols(), direc[i][0]+
          ((mask & MASK_F) && AUTODOC_HANDLER->is_autodoc_file(path+direc[i][0])
           ?"@":""));
      }
    }
    bong = implode(bing, "\n");
  } else if (!(mask & MASK_L)) { // short with colour
    if (mask & MASK_T)  {
      direc = sort_array(direc, (: $1[2] < $2[2] ? -1 :
                                   $1[2] > $2[2] ?  1 : 0 :));
    }
    j = sizeof(direc);
    for(i=0; i<j; i++)
      bing[i] = dir_entry(path, direc[i][0], direc[i][1],mask)+"\n";
    bong = sprintf( "%#-*s\n", this_player()->query_cols(),
                    implode(bing, ""));

    if (mask & MASK_O) {
      /* Boy, it's hard to work with colors and sprintf. *sigh* 
       *especially* in column mode :-( */
      i = j;
      while (i--) {
        if (direc[i][1] == -2 || direc[i][0] == "..") {
          bong = replace_string(bong, " "+direc[i][0],
            sprintf(" %s%s%s", "%^GREEN%^", direc[i][0], "%^RESET%^"));
        } else if (find_object(path+direc[i][0])) {
          bong = replace_string(bong, " "+direc[i][0], 
            sprintf(" %s%s%s", "%^MAGENTA%^", direc[i][0], "%^RESET%^"));
        } else {
          /* Believe me, it's needed. */
          bong = replace_string(bong, " "+direc[i][0],
            sprintf(" %s%s", "%^RESET%^", direc[i][0]));
        }
      }
    }
  } else { // long
    string tmp, tmp2;
    mixed *stats;
    int *count, current_time;

    /* if path is a directory get contents */
    if ( file_size( str ) == -2 && !(mask & MASK_D)) {
        if ( str[ strlen( str ) - 1 ] == '/' ) {
            str += "*";
        } else {
            str += "/*";
        }
    }

    /* begin long "list" */
    direc = get_dir( str, -1 );
    if (!(mask & MASK_A)) {
      direc = filter_array(direc, (: check_dots($1) :));
    }
    if (!direc || !(size = sizeof(direc))) {
        return 0;
    }

    if (mask & MASK_T)  {
      direc = sort_array(direc, (: $1[2] < $2[2] ? -1 :
                                   $1[2] > $2[2] ?  1 : 0 :));
    }
    bit = allocate(size);
    count = allocate(size);

    i = strsrch(str, '/', -1);
    if (i >= 0) {
        str = str[0..i];
    }

    current_time = time();
    for (i = 0; i < size; i++) {
      reset_eval_cost();
      /* process timestamp */
      tmp2 = ctime((direc[i])[2]); /* get last modified timestamp */
      if ((direc[i])[2] + (6 * 30 * 24 * 60 * 60) < current_time ||
          (direc[i])[2] - ( 60 * 60) > current_time ) {
        /* MMM DD  YYYY */
        tmp = sprintf("%s  %s", tmp2[4..9], tmp2[20..23]);
      } else {
        /* MMM DD hh:mm */
        tmp = tmp2[4..15];
      }

      j = (direc[i])[1];   /* get filesize */
      if (j == -2) {
        count[i] = 1;
        /* directory */
        bit[i] = sprintf("%-=*s", (this_player()->query_cols()+
          (mask & MASK_O?17:0)),
          sprintf("drwxr%cx%c%c%c %3d %-11.11s %-11.11s      0 %12s %s%s%s%s",
          (master()->valid_write(sprintf("%s%s/fl.uff",str,direc[i][0]),
          DOMAIN_D, "get_dir")?'w':'-'),
          (master()->valid_read(sprintf("%s%s",str,direc[i][0]),
          "NOBODY", "get_dir")?'r':'-'),
          (master()->valid_write(sprintf("%s%s/fl.uff",str,direc[i][0]),
          "NOBODY", "get_dir")?'w':'-'),
          (master()->valid_read(sprintf("%s%s",str,direc[i][0]),
          "NOBODY", "get_dir")?'x':'-'),
          0+(direc[i][0] == ".." &&
          str == "/"?2:0),
          CREATOR_D, DOMAIN_D, tmp, (mask & MASK_O?"%^GREEN%^":""),
          (direc[i])[0], (mask & MASK_O?"%^RESET%^":""),
          (mask & MASK_F?"/":"")));
      } else {
        /* file */
        count[i] = 0;
        stats = stat(str + (direc[i])[0]);
        bit[i] = sprintf("%-=*s", (this_player()->query_cols()+
          ((mask & MASK_O) && (k = sizeof(stats)) > 1 && stats[2]?19:0)), 
          sprintf("-rw%c%c%c-%c%c-   1 %-11.11s %-11.11s %6d %12s %s%s%s%s%s",
          k > 1 && stats[2] ? 'x' : '-',
          (master()->valid_read(sprintf("%s/%s",str,direc[i][0]),
          DOMAIN, "get_dir")?'r':'-'),
          (master()->valid_write(sprintf("%s/%s",str,direc[i][0]),
          DOMAIN, "get_dir")?'w':'-'),
          (master()->valid_read(sprintf("%s/%s",str,direc[i][0]),
          "NOBODY", "get_dir")?'r':'-'),
          (master()->valid_write(sprintf("%s/%s",str,direc[i][0]),
          "NOBODY", "get_dir")?'w':'-'),
          CREATOR, DOMAIN, j, tmp, 
          ((mask & MASK_O) && k > 1 && stats[2]?"%^MAGENTA%^":""),
          (direc[i])[0], ((mask & MASK_O) && k > 1 && 
          stats[2]?"%^RESET%^":""),
          (k > 1 && stats[2] && (mask & MASK_F)?"*":""),
          ((mask & MASK_F) &&
           AUTODOC_HANDLER->is_autodoc_file(str + direc[i][0])?"@":"")));
      }
      bong = implode(bit, "\n");
    }
  }
  this_player()->more_string( bong, str, 1 );
} /* ls() */

int check_dots(mixed arg) {
  return arg[0][0] != '.';
}

int is_dir(mixed arg, string path) {
  return (file_size(this_player()->get_path(sprintf("%s/%s", path, arg))) 
          == -2);
}
