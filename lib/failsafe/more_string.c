static int fsize,
    topl,
    botl;
static string last_search, *the_bit, finish_func;

#define ROWS 20
#define COLS 75

void display_file() {
  int i;

  botl = topl + ROWS;
  for (i=topl;i<botl && i<fsize;i++)
    write(the_bit[i]+"\n");
}

void more_status_line() {
  write("--- MORE From "+topl+" to "+botl+" of "+fsize+"  ("+
       ((botl*100)/fsize)+"%)"+" --- ? for help. ");
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
  str = extract(str,0,1);
/* case statements WEEEEEE */
  switch(str) {
  case "q" :
	     if (finish_func)
               call_other(this_object(),finish_func);
             return;
  case "" :
  case "f" :
/* go on a number of pages... */
    topl += ROWS-2;
    redraw = 1;
   break;
  case "/" :
/* sigh */
    i = topl+4;
    if (s1=="")
      s1 = last_search;
    for (i=topl+4;i<fsize;i++)
      if (sscanf(the_bit[k],"%s"+s1+"%s",s4,s5)==2)
        if (num--<=0)
          break;
    if (i==fsize)
      write("Sorry "+s1+" not found.\n");
    else
      topl = i-3;
    redraw = 1;
    break;
  case "?" :
    for (i=topl+2;i>0;i--)
      if (sscanf(the_bit[k],"%s"+s1+"%s",s4,s5)==2)
        if (num--<=0)
          break;
    if (i==fsize)
      write("Sorry "+s1+" not found.\n");
    else
      topl = i-3;
    redraw = 1;
    break;
  case "b" :
    if (topl>0) {
      topl -= ROWS-2;
      redraw = 1;
      if (topl<0)
        topl = 0;
    }
    break;
  case "g" :
    topl = num;
    if (topl>=fsize)
      topl = fsize-2;
    redraw = 1;
    break;
  case "G" :
    redraw = 1;
    if (noargs)
      topl = fsize - ROWS;
    else
      topl = num;
      if (topl>fsize)
        topl = fsize-2;
    break;
  }
  if (redraw)
    display_file();
  if (topl<fsize) {
    more_status_line();
    input_to("next_page");
  } else
    if (finish_func)
      call_other(this_object(),finish_func);
}

int more_string(mixed str) {

  last_search = "";
 
  if (!pointerp(str)) 
    the_bit = explode(str,"\n");
  fsize = sizeof(the_bit);
  if (fsize == 0) {
    notify_fail("Empty String.\n");
    return 0;
  }
  topl = 0;
  if (fsize < ROWS)
    botl = fsize;
  else
    botl = ROWS;
  display_file();
  if (fsize >= ROWS) {
    more_status_line();
    input_to("next_page");
  }
  return 1;
}
