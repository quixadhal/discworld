/*
 * A nroff like object for formating text files.
 *
 * This creates a saveable file format thingy.
 */
#include "nroff.h"

mixed *nroffed_file;
string nroffed_file_name;
int modified_time;

void create() {
  nroffed_file = ({ });
  seteuid("Root"); /* Security risk?  nahhhhh */
} /* create() */

#define do_bounds(bing) (bing<0?bing+cols:bing)

/*
 * Creates a string from a nroffed file
 *
 * If update is non zero it will try and find the file_name and return
 * 0 from here if it is either not found, or non existant...
 */
string cat_file(string fname, int update) {
  int i, cols;
  string ret;

  if (!"/secure/master"->valid_read(fname, geteuid(previous_object()),
                                    "restore_object"))
    return 0;
  nroffed_file_name = 0;
  modified_time = 0;
  if (!restore_object(fname))
    return 0;
  if (!nroffed_file_name && update)
    return 0;
  if (update) {
    mixed *something;

    if (!"/secure/master"->valid_read(nroffed_file_name,
                                      geteuid(previous_object()),
                                      "restore_object"))
      return 0;
    if (file_size(nroffed_file_name) <= 0)
      return 0;
    something = stat(nroffed_file_name);
    if (something[1] > modified_time)
      return 0;
  }
  ret = "";
  cols = (int)this_player()->query_cols();
  for (i=0;i<sizeof(nroffed_file);i++)
    if (stringp(nroffed_file[i]))
      ret += nroffed_file[i];
    else
      switch (nroffed_file[i]) {
        case V_CENTER :
          ret += sprintf("%|=*s", cols, nroffed_file[i+1]);
          i++;
          break;
        case V_ALL :
          ret += sprintf("%-*s%|*s%*s\n", nroffed_file[i+1],
                                        nroffed_file[i+2],
                                        cols - nroffed_file[i+1]*2,
                                        nroffed_file[i+3],
                                        nroffed_file[i+1],
                                        nroffed_file[i+4]);
          i += 4;
          break;
        case V_INDENT :
          ret += sprintf("%*=s%-=*s\n", nroffed_file[i+1], "",
                                        cols-nroffed_file[i+1], 
                                        nroffed_file[i+2]);
          i += 2;
          break;
        case V_PARA :
          ret += sprintf("%*=s%-=*s%=*s\n", nroffed_file[i+1], "",
                                            cols-nroffed_file[i+1]-
                                           nroffed_file[i+2], nroffed_file[i+3],
                                            nroffed_file[i+2], "");
          i += 3;
          break;
        case V_LEFT :
          ret += sprintf("%-=*s", cols, nroffed_file[i+1]);
          i++;
          break;
        case V_TABLE :
          ret += sprintf("%-#*s", cols, nroffed_file[i+1]);
          i++;
          break;
        case V_COLUMN : {
          int j;

          switch (sizeof(nroffed_file[i+1])) {
            case 2 :
              for (j=0;j<sizeof(nroffed_file[i+2]);j++)
                ret += sprintf("%*-=s%*-=s\n", do_bounds(nroffed_file[i+1][0]),
                                             nroffed_file[i+2][j],
                                             do_bounds(nroffed_file[i+1][1]),
                                             nroffed_file[i+3][j]);
              i += 3;
              break;
            case 3 :
              for (j=0;j<sizeof(nroffed_file[i+2]);j++)
                ret += sprintf("%*-=s%*-=s%*-=s\n",
                                                do_bounds(nroffed_file[i+1][0]),
                                                nroffed_file[i+2][j],
                                                do_bounds(nroffed_file[i+1][1]),
                                                nroffed_file[i+3][j],
                                                do_bounds(nroffed_file[i+1][2]),
                                                nroffed_file[i+4][j]);
              i += 4;
              break;
          }
          break;
        }
      }
  return " \n"+ret;
}

static int new_string, force_string;

void add_array(mixed *i) {
  if (!sizeof(nroffed_file))
    nroffed_file += ({ i });
  else if (force_string)
    nroffed_file += ({ "", i });
  else
    nroffed_file += ({ i });
  force_string = 0;
  new_string = 0;
}

void add_int(int i) {
  if (!sizeof(nroffed_file))
    nroffed_file += ({ i });
  else if (force_string)
    nroffed_file += ({ "", i });
  else
    nroffed_file += ({ i });
  force_string = 0;
  new_string = 0;
}

void add_string(string s) {
  if (!sizeof(nroffed_file) || new_string)
    nroffed_file += ({ s });
  else if (stringp(nroffed_file[sizeof(nroffed_file)-1]))
    nroffed_file[sizeof(nroffed_file)-1] += s;
  else
    nroffed_file += ({ s });
  new_string = 0;
  force_string = 0;
}

int create_nroff(string in_file, string out_file) {
  string text,
         tmp,
         *bits;
  string *bing;
  mixed  *cols;
  int strip_crs,
      col_mode,
      conv_tabs,
      i, j, k,
      num_cols;

  if (!"/secure/master"->valid_read(in_file, geteuid(previous_object()),
                                    "read_file"))
    return 0;
  if (!"/secure/master"->valid_write(out_file, geteuid(previous_object()),
                                    "save_object"))
    return 0;
  nroffed_file_name = in_file;
  modified_time = time();
  text = read_file(in_file);
  if (!text)
    return 0;
  bits = explode("#\n"+text, "\n.");
  bits[0] = bits[0][1..100000];
  nroffed_file = ({ 0 });
  if (strlen(bits[0]))
    add_string(bits[0]);
  for (i=1;i<sizeof(bits);i++) {
/*
    if (!strlen(bits[i])) {
      if (col_mode) {
        for (j=0;j<num_cols;j++)
          cols[j] += "\n";
      } else if (strip_crs)
        add_string("\n\n");
      else
        add_string("\n");
    } else if (bits[i][0] == '.') {
 */
        if (sscanf(bits[i], "%s\n%s", tmp, bits[i]) != 2) {
          tmp = bits[i];
          bits[i] = "";
        }
        switch (tmp[0..1]) {
          case "SI" : /* Start indent */
            add_int(V_INDENT);
            j = 0;
            sscanf(tmp[2..1000], "%d%s", j, tmp);
            add_int(j);
            force_string = 1;
            break;
          case "EI" : /* End paragraph */
            new_string = 1;
            break;
          case "SP" : /* Start paragraph */
            add_int(V_PARA);
            j = 0;
            sscanf(tmp[2..1000], "%d%s", j, tmp);
            add_int(j);
            j = 0;
            sscanf(tmp, " %d%s", j, tmp);
            add_int(j);
            force_string = 1;
            strip_crs = 1;
            break;
          case "EP" : /* End paragraph */
            new_string = 1;
            strip_crs = 0;
            break;
          case "SC" : /* start centering */
            add_int(V_CENTER);
            force_string = 1;
            break;
          case "EC" : /* End centering */
            new_string = 1;
            break;
          case "SL" : /* Start left justify */
            add_int(V_LEFT);
            force_string = 1;
            break;
          case "EL " : /* End left justify */
            new_string = 1;
            break;
          case "ST" : /* Start table mode (Turn tabs into new lines) */
            add_int(V_TABLE);
            force_string = 1;
            conv_tabs = 1;
            break;
          case "ET" : /* End table mode */
            new_string = 1;
            conv_tabs = 0;
            break;
          case "DT" : /* Do title.  Take the next three lines... */
            bing = explode(bits[i], "\n");
            if (sizeof(bing) < i+3)
              return 0; /* failed! */
            add_int(V_ALL);
            if (strlen(bing[0]) > strlen(bing[2]))
              add_int(strlen(bing[0]));
            else
              add_int(strlen(bing[2]));
            new_string = 1;
            add_string(bing[0]);
            new_string = 1;
            add_string(bing[1]);
            new_string = 1;
            add_string(bing[2]);
            new_string = 1;
            bits[i] = implode(bing[3..1000], "\n");
            break;
          case "SO" : /* starts column mode. The numbers after it
                       * are column size */
            num_cols = 0;
            tmp = tmp[2..1000];
            cols = ({ });
            while (sscanf(tmp, "%d%s", j, tmp) == 2) {
              cols += ({ j });
              num_cols++;
              while (strlen(tmp) && tmp[0] == ' ')
                tmp = tmp[1..1000];
            }
            add_int(V_COLUMN);
            if (sscanf(tmp, "%d", j) == 1) {
              cols += ({ j });
              num_cols++;
            }
            add_array(cols);
            cols = allocate(num_cols);
            for (j=0;j<num_cols;j++) cols[j] = ({ });
            col_mode = 1;
            break;
          case "EO" : /* End column mode */
            for (j=0;j<num_cols;j++) {
              add_array(cols[j]);
            }
            col_mode = 0;
            break;
        }
/* Now dis bit...  */
      if (!strlen(bits[i])) continue;
      if (conv_tabs)
        bits[i] = replace(bits[i], "\t", "\n");
      if (col_mode) {
        string *frog;

        frog = explode(bits[i], "\n");
        for (k=0;k<sizeof(frog);k++) {
          bing = explode("#"+frog[k], "\t");
          bing[0] = bing[0][1..1000];
          for (j=0;j<num_cols && j < sizeof(bing);j++)
            cols[j] += ({ bing[j] });
          for (j=sizeof(bing);j<num_cols;j++)
            cols[j] += ({ "\n" });
        }
      } else if (strip_crs) {
        bits[i] = replace_string(bits[i], "\n\n", "$%^NeW_LiNe^%$");
        bits[i] = replace_string(bits[i], "\n", " ");
        bits[i] = replace_string(bits[i], "$%^NeW_LiNe^%$", "\n\n");
        add_string(bits[i]+" ");
      } else
        add_string(bits[i]+"\n");
/*
    }
 */
  }
  new_string = 0;
    force_string = 0;
  save_object(out_file);
  return 1;
}

void create_help(string path, string file) {
  if (path[0..4] == "/doc/")
    this_object()->create_nroff(path+file, NROFF_DIR+file);
  else
    write("Invalid path. "+path+"\n");
}

void create_chelp(string path, string file) {
  if (path[0..4] == "/doc/")
    this_object()->create_nroff(path+file, CNROFF_DIR+file);
  else
    write("Invalid path.\n");
}
