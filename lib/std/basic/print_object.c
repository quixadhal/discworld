varargs string print_object(mixed ob, int off) {
  int i;

  write("                   "[0..off]);
  if (intp(ob)) {
    write(ob+"\n");
    return "int";
  }
  if (stringp(ob)) {
    write("\""+ob+"\"\n");
    return "string";
  }
  if (objectp(ob)) {
    write(file_name(ob)+"  ");
    if (ob->short())
      write("(short) "+ob->short()+"\n");
    else if (ob->query_name())
      write("(name)  "+ob->query_name()+"\n");
    else
      write("(Argghhhh)\n");
    return "object";
  }
  if (pointerp(ob)) {
    if (!sizeof(ob))
      write("({ })\n");
    else {
      write("({  -- size "+sizeof(ob)+"\n");
      for (i=0;i<sizeof(ob);i++)
        print_object(ob[i], off+3);
      write("                       "[0..off]+"})\n");
    }
    return "pointer";
  }
  return "oh dear";
}
