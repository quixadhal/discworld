varargs string print_object(mixed arg, int off) {
  write(extract("                    ",0,off));
  if (stringp(arg)) {
    write("\""+arg+"\"\n");
    return "Ok";
  }
  if (intp(arg)) {
    write(arg+"\n");
    return "Ok";
  }
  if (objectp(arg)) {
    write(file_name(arg));
    if (arg->short())
      write("  (Short) \""+arg->short()+"\"\n");
    else if (arg->query_name())
      write("  (Name) - \""+arg->query_name()+"\"\n");
    else
      write("  (Arggghhh)\n");
    return "Ok";
  }
  if (pointerp(arg)) {
    int i;
    if (sizeof(arg) == 0) {
      write("({ })\n");
      return "Ok";
    }
    write("({ --- size "+sizeof(arg)+"\n");
    for (i=0;i<sizeof(arg);i++)
      print_object(arg[i], off+3);
    write(extract("                  ",0,off)+"})\n");
    return "Ok";
  }
  return "Oh dear we have failed";
}
