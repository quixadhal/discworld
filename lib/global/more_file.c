#include "nroff.h"
/*
 * inheritable more object. Just say bing!
 * The wonder new improved morer, based loosely on a more/less object
 * written by alice(ember). But this one is written by pinkfish like
 * most of the rest of this mud, bar what is not of course ;)
 */
static int fsize,
           topl,
           search_flag,
           botl;
static mixed mark_data;
static string fname,
              *mark_name,
              *filenames,
              finish_func,
              stat_line,
              last_search;

#define ROWS  ((int)this_player()->query_rows()-2) 

void display_file() {
  botl = topl + ROWS;
  if (botl > fsize) botl = fsize;
  cat(fname,topl,ROWS);
} /* display_file() */

void get_next_filename() {
  string str;

  fname = filenames[0];
/*
  write(fname + " : ");
/*
  fsize = file_size(fname)/10;
*/
  fsize = file_length(fname);
  if (fsize == 0) {
    write("Empty file.\n");
    if(sizeof(filenames) > 1) {
      filenames = filenames[1..sizeof(filenames) - 1];
      get_next_filename();
      return;
    }
    else {
      return;
    }
  }
  if (fsize == -1) {
    write("File does not exist.\n");
    if(sizeof(filenames) > 1) {
      filenames = filenames[1..sizeof(filenames) - 1];
      get_next_filename();
      return;
    }
    else {
      return;
    }
  }
  if (fsize == -2) {
    write("You cannot more a directory.\n");
    if(sizeof(filenames) > 1) {
      filenames = filenames[1..sizeof(filenames)-1];
      get_next_filename();
      return;
    }
    else {
      return;
    }
  }
  topl = 1;
  if (fsize < ROWS)
    botl = fsize;
  else
    botl = ROWS;
  if(sizeof(filenames) > 1) {
    filenames = filenames[1..sizeof(filenames) - 1];
  } else {
    filenames = ({ });
  }
  write("\n");
  str = read_file(fname, 1, 1);
  if (str[0] == '.') {
    string s2;

/*
 * Nroff file...  do horrible things!  Turn into a mushroom!  Blow up
 * all the hairy crater eating womble doovalacies...
 * In the case of a nroff file we push the file through nroff then shove it
 * through more_string...
 */
    str = "/tmp/nroffed_file";
    if (NROFF_HAND->create_nroff(fname, "/tmp/nroffed_file")) {
      s2 = NROFF_HAND->cat_file("/tmp/nroffed_file");
      rm("/tmp/nroffed_file.o");
      if (s2) {
        this_object()->more_string(s2, fname);
        fsize = ROWS-4;
        return ;
      }
    }
  }
  display_file();
} /* get_next_file_name() */

string *expand_path(string s) {
  string *s1;

  s1 = (string *)this_player()->get_files(s);
  if (!s1)
    return ({ s });
  return s1;
} /* expand_path() */

int set_mark(string s) {
  int i;
  if (!mark_name) {
    mark_name = ({ });
    mark_data = ({ });
  }
  if ((i=member_array(s,mark_name))!=-1)
    mark_data[i] = ({ fname, topl });
  else {
    mark_name += ({ s });
    mark_data += ({ ({ fname, topl }) });
  }
  return 1;
} /* set_mark() */

void status_line() {
  string *frog;
  string s;
  int i, j,percentage;

  if (!stat_line)
    stat_line = "$N From $T to $B of $S ($%%) - h for help. ";
  s = "";
  frog = explode(stat_line, "$");
  for (i=0;i<sizeof(frog);i++) {
    if (frog[i] == "") {
      s += "$";
      i ++;
    } else switch (frog[i][0]) {
      case 'N' :
          s += fname+frog[i][1..1000];
          break;
      case 'T' :
          s += topl+frog[i][1..1000];
          break;
      case 'B' :
          if (botl > fsize)
            s += fsize+frog[i][1..100];
          else
            s += botl+frog[i][1..100];
          break;
      case '%' : {
          percentage = (botl*100)/fsize;
          if(percentage > 100) percentage = 100;
          s += percentage+frog[i][1..100];
          break;
      }
      case 'S' :
         s += fsize+frog[i][1..1000];
         break;
    }
  }
  write(s);
  return;
  write(fname+" From "+topl+" to "+botl+" of "+fsize+"  ("+
       ((botl*100)/fsize)+"%)"+" - h for help. ");
} /* status_line() */

void next_page(string str) {
  int num,
      noargs,
      i,j,k,
      redraw;
  string s1, s2, *s3, s4, s5;

  if (!str)
    str = "";

  if (sscanf(str,"%d%s", num, str)!=2)
    noargs = 1;
  s1 = extract(str,1);
  str = extract(str,0,0);
/* case statements WEEEEEE */
  switch(str) {
  case "" :
  case " " :
  case "f" :
/* go on a number of pages... */
    if (noargs)
      num = 1;
    topl += (ROWS)*num;
    redraw = 1;
   break;
  case "q" :
    write("Ok\n");
    return ;
  case "/" :
/* sigh */
    j = topl+4;
    redraw = 1;
    if (s1[0] == '!') {
      search_flag = 1;
      s1 = extract(s1,1);
    }
    set_mark("'");
    if (s1=="")
      s1 = last_search;
    do {
      i = j;
      j = i+900;
      if (j>fsize)
        j = fsize;
      s3 = explode(read_file(fname,i,j),"\n");
      for (k=0;k<sizeof(s3);k++)
        if (!search_flag) {
          if (sscanf(s3[k],"%s"+s1+"%s",s4,s5)==2)
            if (num--<=0)
              break;
        } else
          if (sscanf(s3[k],"%s"+s1+"%s",s4,s5)!=2)
            if (num--<=0)
              break;
    } while (j<fsize && k == sizeof(s3));
    if (k==sizeof(s3))
      write("Sorry "+s1+" not found.\n");
    else
      topl = i+k-3;
    last_search = s1;
    redraw = 1;
    break;
  case "?" :
    i = topl;
    redraw = 1;
    if (s1[0] == '!') {
      s1 = extract(s1,1);
      search_flag = 3;
    } else
      search_flag = 2;
    set_mark("'");
    if (s1=="" || !s1)
      s1 = last_search;
    do {
      j = i-900;
      if (j<0)
        j = 0;
      s3 = explode(read_file(fname,j,i),"\n");
      for (k=sizeof(s3);k>0;k--)
        if (search_flag == 2) {
          if (sscanf(s3[k],"%s"+s1+"%s",s4,s5)==2)
            if (num--<=0)
              break;
        } else
          if (sscanf(s3[k],"%s"+s1+"%s",s4,s5)!=2)
            if (num--<=0)
              break;
      i = j;
    } while (j>0 && k == sizeof(s3));
    if (k==sizeof(s3))
      write("Sorry "+s1+" not found.\n");
    else
      topl = k+j;
    last_search = s1;
    redraw = 1;
    break;
  case "n" :
    switch (search_flag) {
      case 0 : next_page(num+"/");
               break;
      case 1 : next_page(num+"/!");
               break;
      case 2 : next_page(num+"?");
               break;
      case 3 : next_page(num+"?!");
               break;
    }
    return;
  case "b" :
    if (noargs)
      num = 1;
    if (topl>0) {
      topl -= (ROWS)*num;
      redraw = 1;
      if (topl<0)
        topl = 0;
    }
    break;
  case "G" :
    if (noargs)
      num = fsize-ROWS+2;
  case "g" :
    set_mark("'");
    topl = num;
    if (topl>=fsize)
      topl = fsize-2;
    redraw = 1;
    break;
  case "P" :
    if (noargs)
      num = 100;
  case "p" :
  case "%" :
    redraw = 1;
    set_mark("'");
    topl = (num*fsize)/100;
    if (topl+ROWS-2>fsize)
      topl -= ROWS-2;
    break;
  case "d" :
    if (noargs)
      num = ROWS/2;
    topl += num;
    redraw = 1;
    break;
  case "u" :
    if (noargs)
      num = ROWS/2;
    topl -= num;
    redraw = 1;
    break;
  case "r" :
    redraw = 1;
    break;
  case "m" :
    if (s1=="") {
      write("Sorry you must specify a name.\n");
      break;
    }
    set_mark(s1);
    write("Ok mark "+s1+" set.\n");
    break;
  case "'" :
    if (!mark_name) {
      write("Sorry must goto a mark.\n");
      break;
    }
    if ((i=member_array(s1,mark_name))!=-1) {
      if (fname != mark_data[i][0]) {
        fname = mark_data[i][0];
        redraw = 1;
      }
      if (topl != mark_data[i][1]) {
        topl = mark_data[i][1];
        redraw = 1;
      }
    } else
      write("Mark "+s1+" not set.\n");
    break;
  case "F" :
    filenames = expand_path(s1);
    if (file_size(s1)<=0)
      write("Invalid file name "+s1+"\n");
    else {
      fname = s1;
      fsize = file_size(s1)/10;
      redraw = 1;
    }
    break;
  case "h" :
    cat("/doc/helpdir/more");
    break;
  }
  if (redraw)
    display_file();
  if (topl <= fsize) {
    status_line();
    input_to("next_page");
  } else
    if(sizeof(filenames)) {
      get_next_filename();
      input_to("next_page");
    }
    else {
      if (finish_func)
        call_other(this_object(), finish_func);
    }
} /* next_page() */

int more_file(string str) {
  filenames = expand_path(str);
  if (!sizeof(filenames)) {
    notify_fail("No matching files.\n");
    return 0;
  }
  last_search = "";
  get_next_filename();
  if (fsize >= ROWS) {
    status_line();
    input_to("next_page");
  } else
    if (finish_func)
      call_other(this_object(), finish_func);
  return 1;
} /* more_file() */
