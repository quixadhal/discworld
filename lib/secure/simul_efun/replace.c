varargs string replace(string str, mixed bing, string rep) {
  int i;

  if (pointerp(bing)) {
    for (i=0;i<sizeof(bing);i+=2)
        if (stringp(bing[i]) && stringp(bing[i+1]))
        str = replace_string(str, bing[i], bing[i+1]);
    return str;
  }
  return replace_string(str, bing, rep);
} /* replace() */
