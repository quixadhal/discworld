void write(mixed str) {
  if (!this_player())
    return ;
  if (intp(str))
    str = ""+str;
  this_player()->do_efun_write(str);
} /* write() */
