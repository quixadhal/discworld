/*
 * A nice book object.  A book will be a collection of pages.
 * You can tear pages out of a book, you can add pages (though adding
 * a page isnt overly useful, unless you bind it in of course).
 */

/*
 * Each page is an object.  Pages that are torn out are handled
 * as empty spots in the array.
 */
inherit "/std/object";
mixed *pages;
string default_page_object;
object def_p_obj;
/* Open_page set to 0 means closed. */
int open_page;

void create() {
  pages = ({ });
  default_page_object = "/obj/misc/paper";
  call_other(default_page_object, "??");
  def_p_obj = find_object(default_page_object);
  ::create();
} /* create() */

void init() {
  ::init();
  this_player()->add_command("read", this_object());
  this_player()->add_command("open", this_object());
  this_player()->add_command("open", this_object(), "%D 'to' %d");
  this_player()->add_command("open", this_object(), "%D 'to' 'page' %d");
  this_player()->add_command("tear", this_object(), "'page' 'from' %D");
  this_player()->add_command("turn", this_object(), "'a' 'page' 'of' %D");
  this_player()->add_command("close", this_object());
} /* init() */

int test_add(object ob, int flag) {
  return (object)ob->query_property("my book") == this_object();
} /* test_add() */

int test_remove(object ob, int flag) {
  return !ob->query_property("my book");
} /* test_remove() */

void set_no_pages(int no) {
  int i,
      siz;

  siz = sizeof(pages);
  if (no < siz) { /* removeing pages?  Oh well, if you insist. */
    for (i=no;i<siz;i++)
      if (objectp(pages[i]))
        pages[i]->dest_me();
    pages = pages[0..no-1];
    if (open_page >= no)
      open_page = no;
  } else {
/* Adding pages, more normal... */
    pages = pages+allocate(no-siz);
    for (i=siz;i<no;i++) {
/* Set them to 1 and only make them real if someone wants them. */
      pages[i] = 1;
    }
  }
} /* set_no_pages() */

object *query_pages() { return pages; }

void set_open_page(int i) {
  if (!open_page) {
    add_alias("page");
    add_plural("pages");
  }
  if (open_page > sizeof(pages))
    open_page = sizeof(pages);
  else
    open_page = i;
  if (!open_page) {
    remove_alias("page");
    remove_plural("pages");
  }
} /* set_open_page() */

int query_open_page() { return open_page; }

object query_current_page() {
  int i;

  if (!open_page)
    return this_object(); /* The book itself... */
  for (i=open_page-1;i<sizeof(pages);i++)
    if (pages[i])
      if (objectp(pages[i]))
        return pages[i];
      else
        return def_p_obj;
} /* query_current_page() */

string short(int flags) {
  if (!flags)
    return ::short(flags);
  if (open_page)
    return ::short(flags)+" (open at page "+open_page+")";
  return ::short(flags)+" (closed)";
} /* short() */

string long(string str, int dark) {
  string ret;
  int i;

  if (!open_page)
    return ::long(str, dark)+"It is closed.\n";
  ret = ::long(str, dark)+"It is open at page "+open_page+"\n";
  for (i=open_page-1;i<sizeof(pages) && !pages[i];i++)
    if (!pages[i])
      ret += "Page "+(i+1)+" has been torn out.\n";
  if (i >= sizeof(pages))
    ret += "All the rest of the pages have been torn out!\n";
  else {
    if (i != open_page -1) /* A page has been torn out... */
      ret += "You can see page "+(i+1)+" however,\n";
    if (objectp(pages[i]))
      ret += (string)pages[i]->long(str, dark);
    else
      ret += default_page_object->long(str, dark);
  }
  return ret;
} /* long() */

mixed do_read() {
  int i;
  object ob;
 
  if (!open_page)
    return ::do_read();
  i = open_page-1;
  while (i < sizeof(pages)) {
    if (pages[i]) {
      write("Reading page "+(i+1)+".\n");
      if (objectp(pages[i]))
        ob = pages[i];
      else
        ob = def_p_obj;
      if (!ob->do_read())
        write("Is blank, empty, nothing written on it.\n");
      return "page from "+short(0);
    }
    i++;
  }
  write("All the pages from "+open_page+" on seem to be torn out.\n");
  return 1;
} /* do_read() */

int do_open(object *indir, string id_match, string ind_match,
            mixed *args, string pattern) {
  int page;

  if (pattern == "%D")
    page = 1;
  else
    page = args[1];
  if (!page || page > sizeof(pages))
    return 0;
  if (!open_page) {
    add_alias("page");
    add_plural("pages");
  }
  open_page = page;
  this_player()->add_succeeded_mess(this_object(), "You open $D to page "+page+
                                    ".\n", ({ }));
  return 1;
} /* do_open() */

int do_turn() {
  int tmp;

  tmp = open_page;
  if (open_page >= sizeof(pages))
    set_open_page(0);
  else
    set_open_page(open_page+1);
  return tmp != open_page;
} /* do_turn() */

int do_close() {
  if (!open_page) return 0;
  remove_alias("page");
  remove_plural("pages");
  open_page = 0;
  return 1;
} /* do_close() */

/* Tears out the current page */
int do_tear() {
  if (!pages[open_page-1]) /* Too late... */
    return 0;
  if (objectp(pages[open_page-1]))
    pages[open_page-1]->remove_property("my book");
  else
    pages[open_page-1] = clone_object(default_page_object);
  if (!pages[open_page-1]->move(this_player())) {
    pages[open_page-1] = 0;
    return 1;
  }
  pages[open_page-1]->add_property("my book", this_object());
  pages[open_page-1]->move(this_object());
  return 0;
} /* do_tear() */

/* The stuff redefined for handleing books... */
set_read_mess(string str, string lang, int size) {
  if (open_page)
    if (!pages[open_page-1])
      return 0;
    else if (objectp(pages[open_page-1]))
      return pages[open_page-1]->set_read_mess(str, lang, size);
    else {
      pages[open_page-1] = clone_object(default_page_object);
      pages[open_page-1]->add_property("my book", this_object());
      pages[open_page-1]->move(this_object());
      return pages[open_page-1]->set_read_mess(str, lang, size);
    }
  return ::set_read_mess(str, lang, size);
} /* set_read_mess() */

void add_read_mess(string str, string type, string lang, int size) {
  if (open_page)
    if (!pages[open_page-1])
      return ;
    else if (objectp(pages[open_page-1]))
      return (void)pages[open_page-1]->add_read_mess(str, type, lang, size);
    else {
      pages[open_page-1] = clone_object(default_page_object);
      pages[open_page-1]->add_property("my book", this_object());
      pages[open_page-1]->move(this_object());
      pages[open_page-1]->add_read_mess(str, type, lang, size);
      return ;
    }
  ::add_read_mess(str, type, lang, size);
} /* add_read_mess() */

mixed query_read_mess() {
  if (open_page)
    if (!pages[open_page-1])
      return 0;
    else if (objectp(pages[open_page-1]))
      return (mixed *)pages[open_page-1]->query_read_mess();
    else
      return (mixed *)default_page_object->query_read_mess();
  return ::query_read_mess();
} /* query_read_mess() */

void set_max_size(int siz) {
  if (!open_page)
    return ::set_max_size(siz);
  if (!pages[open_page-1])
    return ;
  if (objectp(pages[open_page-1])) {
    pages[open_page-1]->set_max_size(siz);
    return ;
  }
  pages[open_page-1] = clone_object(default_page_object);
  pages[open_page-1]->add_property("my book", this_object());
  pages[open_page-1]->move(this_object());
  pages[open_page-1]->set_max_size(siz);
} /* query_max_size() */

void set_cur_size(int siz) {
  if (!open_page)
    return ::set_cur_size(siz);
  if (!pages[open_page-1])
    return 0;
  if (objectp(pages[open_page-1])) {
    pages[open_page-1]->set_siz_size(siz);
    return ;
  }
  pages[open_page-1] = clone_object(default_page_object);
  pages[open_page-1]->add_property("my book", this_object());
  pages[open_page-1]->move(this_object());
  pages[open_page-1]->set_siz_size(siz);
} /* set_cur_size() */

int query_max_size() {
  if (!open_page)
    return ::query_max_size();
  if (!pages[open_page-1])
    return 0;
  if (objectp(pages[open_page-1]))
    return (int)pages[open_page-1]->query_max_size();
  return (int)default_page_object->query_max_size();
} /* query_max_size() */

int query_cur_size() {
  if (!open_page)
    return ::query_cur_size();
  if (!pages[open_page-1])
    return 0;
  if (objectp(pages[open_page-1]))
    return (int)pages[open_page-1]->query_cur_size();
  return (int)default_page_object->query_cur_size();
} /* query_cur_size() */

void dest_me() {
  int i;

  for (i=0;i<sizeof(pages);i++)
    if (objectp(pages[i]))
      pages[i]->dest_me();
  ::dest_me();
} /* dest_me() */

mapping query_dynamic_auto_load() {
  int i;
  mixed *ret;

  ret = ({ });
  for (i=0;i<sizeof(pages);i++)
    if (intp(pages[i]))
      ret += ({ pages[i] });
    else
      ret += ({ "/global/player"->create_auto_load(pages[i..i]) });
  return ([
      "::" : ::query_dynamic_auto_load(),
      "pages" : ret,
      "default page object" : default_page_object,
      "open page" : open_page,
    ]);
} /* query_dynamic_auto_load() */

void init_dynamic_arg(mapping map) {
  int i;
  object *tmp;

  if (map["::"])
    ::init_dynamic_arg(map["::"]);
  if (map["default page object"]) {
    default_page_object = map["default page object"];
    call_other(default_page_object, "??");
    def_p_obj = find_object(default_page_object);
  }
  if (map["pages"]) {
    pages = ({ });
    for (i=0;i<sizeof(map["pages"]);i++)
      if (intp(map["pages"][i]))
        pages += ({ map["pages"][i] });
      else {
        tmp = (object *)"/global/player"->load_auto_load(map["pages"][i],
                                                           this_object());
        tmp[0]->add_property("my book", this_object());
        tmp[0]->move(this_object());
        pages += ({ tmp[0] });
      }
  }
  set_open_page(map["open page"]);
} /* init_dynamic_arg() */
