/* inheritable more object. Just say bing!
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
              last_search;

#define ROWS 20

string expand_path(string s) {
  string s1;

  s1 = (string)this_player()->get_path(s);
  if (!s1)
    return s;
  return s1;
}

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
}

void display_file() {
  botl = topl + ROWS;
  cat(fname,topl,ROWS);
}

void status_line() {
  write(fname+" From "+topl+" to "+botl+" of "+fsize+"  ("+
       ((botl*100)/fsize)+"%)"+" - h for help. ");
}

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
    topl += (ROWS-2)*num;
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
      topl -= (ROWS-2)*num;
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
    s1 = expand_path(s1);
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
    this_object()->end_of_more_func();
}

int more_file(string str) {
  str = expand_path(str);
  last_search = "";
  fsize = file_size(str)/10;
  if (fsize == 0) {
    notify_fail("Empty file.\n");
    return 0;
  }
  if (fsize == -1) {
    notify_fail("File does not exist.\n");
    return 0;
  }
  if (fsize == -2) {
    notify_fail("You cannot more a directory.\n");
    return 0;
  }
  topl = 0;
  if (fsize < ROWS)
    botl = fsize;
  else
    botl = ROWS;
  fname = str;
  display_file();
  if (fsize >= ROWS) {
    status_line();
    input_to("next_page");
  }
  return 1;
}
