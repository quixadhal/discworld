/*
 * This is all the autoloading stuff.
 * please don't remove again - lynscar.
 */
void add_preload(string file) {
  if (previous_object() == this_object() ||
    query_lord(geteuid(previous_object()))) {

    if (!preload) {
      preload = ({ file });
    } else if (member_array(file, preload) == -1) {
      preload += ({ file });
    }
    save_object("/secure/master");
  }
} /* add_preload() */

void remove_preload(string file) {
  int i;

  if (previous_object() == this_object() ||
      query_lord(geteuid(previous_object()))) {

    if (preload) {
      i = member_array(file, preload);
      if (i >= 0) {
        preload = preload[0 .. i - 1] + preload[i + 1 .. 1000];
        save_object("/secure/master");
      }
    }
  }
} /* remove_preload() */

void add_call_out_preload(string file) {
  if (previous_object() == this_object() ||
    query_lord(geteuid(previous_object()))) {

    if (!call_out_preload) {
      call_out_preload = ({ file });
    } else if (member_array(file, call_out_preload) == -1) {
      call_out_preload += ({ file });
    }
    save_object("/secure/master");
  }
} /* add_call_out_preload() */

void remove_call_out_preload(string file) {
  int i;

  if (previous_object() == this_object() ||
      query_lord(geteuid(previous_object()))) {

    if (call_out_preload) {
      i = member_array(file, call_out_preload);
      if (i >= 0) {
        call_out_preload = delete(call_out_preload, i, 1);
        save_object("/secure/master");
      }
    }
  }
} /* remove_call_out_preload() */

void load_object() {
  if (!done) {
    done = 1;
    seteuid(ROOT);
    restore_object("/secure/master");
  }
} /* load_object() */

string *epilog() {
  int i;

  if (!preload) {
    load_object();
  }
  for (i=0;i<sizeof(call_out_preload);i++)
    call_out("preload", 2, call_out_preload[i]);
  return preload;
} /* epilog() */

void preload(string file) {
  int e;

  printf("Preloading: "+file+".\n");
  if ((e = catch(file->dummy()))) {
    printf("            "+e+"\n");
  }
} /* preload() */
