varargs string extract(string str, int start, int end) {
  return str[start..(end?end:10000)];
} /* extract() */
