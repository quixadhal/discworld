/*
 * returns the number of lines in a file.
 */
int file_length(string file_name) {
  int top, bottom;

  if ((top = file_size(file_name)) < 0) return top;
  top = top / 46;
  if(top < 2)
    top = 2;
  bottom = 1;
  while (read_file(file_name, top)) {
    top = top + top/2; /* top *= 1.5; */
  }
  while (top > bottom+1) {
    if (read_file(file_name, (top+bottom)/2)) {
      bottom = (top+bottom)/2;
    } else {
      top = (top+bottom)/2;
    }
  }
  return bottom-1;
} /* file_length() */
