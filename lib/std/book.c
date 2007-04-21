/*  RCS stuff...  -*- LPC -*-
 * $Locker:  $
 *
 * $Id: book.c,v 1.39 2003/03/23 03:30:02 presto Exp $
 *
 */
/**
 * A nice book object.  A book will be a collection of pages.
 * You can tear pages out of a book, you can add pages (though adding
 * a page isnt overly useful, unless you bind it in of course).
 *<p>
 * Each page is an object.  Pages that are torn out are handled
 * as empty spots in the array.
 *<p>
 * @change 26-Feb-96 Jeremy
 * I took out the "my book" checks in test_add() and test_remove()
 * because they were keeping written pages from loading.  If
 * anyone knows why the checks were necessary, let me know.
 * @change 25-Aug-96 Jeremy
 * Changed test_add() so that it won't add things held by players.
 * This is because if they "put" something in the book, it's inaccessible
 * (maybe this is why pages had the "my book" property?  I like this
 * solution better, but not much).
 * @author Pinkfish
 */
inherit "/std/object";

#include <move_failures.h>
#include <player.h>

private mixed *_pages;
private string _default_page_object;
private object _def_p_obj;
/* Open_page set to 0 means closed. */
private int _open_page;
private int _book_num;
/*
 * If this is set, we ignore the flag and only print the real
 * short of the book.
 */
private int _ignore_open_page;
/*
 * This should be used in conjuction with created books
 * that yuou wish the contents to update when changed.
 */
private int _ignore_saved_pages;
private nosave object _player;
private nosave int _num_torn_out = -1;

protected int do_open(int page);
protected int do_tear(int number);
protected int do_turn(int number);
protected int do_close();
object create_default_page_object();

void create() {
  _pages = ({ });
  _default_page_object = "/obj/misc/paper";
  load_object(_default_page_object);
  _def_p_obj = find_object(_default_page_object);
  ::create();
  add_help_file("book");
} /* create() */

/**
 * Tells us if this is a book object.
 * @return always returns 1
 */
int query_book() { return 1; }

/**
 * @ignore yes
 * Mess with the weight to make the pages taken into account as well.
 */
int query_weight() {
   if (!_def_p_obj) {
      load_object(_default_page_object);
      _def_p_obj = find_object(_default_page_object);
   }
   return ::query_weight() +
          sizeof(filter(_pages, (: objectp($1) :))) * _def_p_obj->query_weight();
} /* query_weight() */

/**
 * Adds the commands onto the player.  The commands are "read", "open",
 * "tear", "turn" and "close".
 * @ignore yes
 */
void init() {
  add_command("open", "<direct:object>", (: do_open(1) :));
  add_command("open", "<direct:object> to [page] <number>",
              (: do_open($4[1]) :));
  add_command("tear", "page from <direct:object>", (: do_tear(1) :));
  add_command("tear", "[all] pages from <direct:object>", (: do_tear(0) :));
  add_command("tear", "<number> [of] pages from <direct:object>",
              (: do_tear($4[0]) :));
  add_command("rip", "page from <direct:object>", (: do_tear(1) :));
  add_command("rip", "<number> [of] pages from <direct:object>",
              (: do_tear($4[0]) :));
  add_command("rip", "[all] pages from <direct:object>", (: do_tear(0) :));
  add_command("turn", "[a|1] page of <direct:object>", (: do_turn(1) :));
  add_command("turn", "<number> pages of <direct:object>",
              (: do_turn($4[0]) :));
  add_command("turn", "<direct:object> to [page] <number>",
              (: do_open($4[1]) :) );
  add_command("turn", "to page <number> of <direct:object>",
              (: do_open($4[0]) :) );
  add_command("close", "<direct:object>", (: do_close() :));
} /* init() */

/**
 * @ignore yes
 */
int add_weight( int number ) {
   adjust_weight( number );
   return 1;
} /* add_weight() */

/**
 * To see if things can be added to us.  This has a few nasty
 * cludges in it to stop a few things. 
 * @see /std/basic/misc
 * @param ob the object being added
 * @param flag the flag sent to this function
 * @return 1 if it can be added, 0 if it cannot
 * @ignore
 */
int test_add(object ob, int flag) {
  return (object)ob->query_property("my book") == this_object();
  // This is a kludge to keep people from putting things in the book.
  // It's not foolproof, but it should do until I think of a better way.
/*
  if (!environment(ob) || !living(environment(ob)))
    return 1;
  return 0;
 */
} /* test_add() */

/**
 * To check to see if things can be removed from us.
 * @see /std/basic/misc
 * @param ob the object being removed
 * @param flag the flag sent to the function
 * @param dest where it is going to
 * @return 1 if it can be removed, 0 if it cannot
 * @ignore
 */
int test_remove( object ob, int flag, mixed dest ) {
  return ob->query_property("my book") != this_object();
  //return 1;
} /* test_remove() */

/**
 * Set the number of pages in the book.  If there are too many pages
 * in the book, then pages are removed from the top to create the
 * correct size and if there are too few pages then pages are
 * added onto the end of the pages array.
 * @param no the number of pages
 * @see query_pages()
 * @see query_num_pages()
 */
void set_no_pages(int no) {
  int i;
  int siz;

  siz = sizeof(_pages);
  if (no < siz) { /* removeing pages?  Oh well, if you insist. */
    _pages = _pages[0..no-1];
    if (_open_page >= no) {
      _open_page = no;
    }
  } else {
    /* Adding pages, more normal... */
    _pages = _pages + allocate(no-siz);
    for (i=siz;i<no;i++) {
      /* Set them empty and only make them real if someone wants them. */
      _pages[i] = ({ });
    }
  }
} /* set_no_pages() */

/**
 * Returns the pages array.
 * @return the array containing the pages information
 */
mixed *query_pages() { return _pages; }

/**
 * Sets the currently open page.  This does all the magic needed to make
 * the book appear as if it is the currently opened page.  If the open
 * page is 0 then the book is closed.  If it is out of the upper
 * bound then the book is opened to the last page.
 * @param i the page to open to
 * @see query_open_page()
 * @see query_current_page()
 */
void set_open_page(int i) {
  /* Valid page or already there. */
  if (i < 0 || i == _open_page) {
     return ;
  }

  if (!_open_page && i) {
    add_alias("page");
    add_plural("pages");
  }

  if (i > sizeof(_pages)) {
    _open_page = sizeof(_pages);
  } else {
    _open_page = i;
  }

  if (!_open_page) {
    remove_alias("page");
    remove_plural("pages");
  }
} /* set_open_page() */

/** 
 * What is the current open page.  Returns 0 if the book is not
 * open.
 * @return the current open page
 * @see set_open_page()
 * @see query_current_page()
 */
int query_open_page() {
   return _open_page;
} /* query_open_page() */

/**
 * This method checks to see if the current page is torn out.
 * @return 1 if torn out, 0 if not
 * @see query_current_page()
 * @see query_open_page()
 * @see is_page_torn_out()
 */
int is_current_page_torn_out() {
   if (!_open_page) {
      /* The cover cannot be torn out. */
      return 0;
   }
   if ( !_pages ) {
      return 0;
   }
     
   if (!_pages[_open_page-1]) {
      return 1;
   }
   return 0;
} /* is_current_page_torn_out() */

/**
 * This method checks to see if the specified page is torn out.
 * @param page the page number to check
 * @return 1 if it is torn out, - if not
 * @see is_current_page_torn_out()
 */
int is_page_torn_out(int page) {
   if (page < 1 || page > sizeof(_pages)) {
      return 0;
   }

   if (!_pages[page - 1]) {
      return 1;
   }
   return 0;
} /* is_page_torn_out() */

/**
 * Returns the object associated with the current open page.  If the
 * page does not actualy exist yet then the default base object
 * is returned.  This object should be handled carefully...  If the
 * page is torn out then the next readable page is returned or
 * 0 is returned.
 * @return the current page object
 * @see query_current_page_clone()
 * @see set_open_page()
 * @see query_open_page()
 * @see query_current_page_clone()
 */
object query_current_page() {
   int i;

   if (!_open_page) {
      return this_object(); /* The book itself... */
   }
   for (i = _open_page - 1; i < sizeof(_pages); i++) {
     if (_pages[i]) {
       if (!_def_p_obj) {
         load_object(_default_page_object);
         _def_p_obj = find_object(_default_page_object);
       }
       return _def_p_obj;
     }
   }
   return 0;
} /* query_current_page() */

/**
 * This function makes sure the page actually
 * exists.  If it does not exist, then it will clone one
 * up for us.  This one should be used as you will not end up with the
 * base object in this case.  If you are planning to modify the page,
 * then use this call.
 * @see query_current_page()
 * @return the object of the current page
 */
object query_current_page_clone() {
  return this_object();
} /* query_current_page_clone() */

/**
 * This method checks to see if the page is still the default page object
 * or if it is something else altogether.
 * @return 1 if it is the default page object, 0 if not
 */
int is_default_page(int num) {
   if (num > 0 && num <= sizeof(_pages)) {
      if (_pages[num - 1] &&
          !objectp(_pages[num - 1])) {
         return 1;
      }
   }
   return 0;
} /* is_default_page() */

/**
 * This method returns the contents of the selected page.  The number
 * must be greator than 0.  This will return the exact value of the
 * page, it will not search for non-torn out pages.  Becare with
 * the return value of this as the default page object might be
 * returned if the page does not actually exist.  If you need a
 * real object remember to use the clone version.
 * @param num the page to return
 * @return 0 on failure or if the page is torn out, the object on success
 */
object query_selected_page(int num) {
   if (!intp(num) || num <= 0 || num > sizeof(_pages)) {
      return 0;
   }

   if (_pages[num - 1]) {
      if (objectp(_pages[num - 1])) {
         return _pages[num - 1];
      } else {
         if (!_def_p_obj) {
            load_object(_default_page_object);
            _def_p_obj = find_object(_default_page_object);
         }
         return _def_p_obj;
      }
   }
   return 0;
} /* query_selected_page() */

/**
 * This method returns the contents of the selected page, if a default
 * object is returned a real clone for the page is created instead.
 * @param num the page to return
 * @return 0 on failure or if the page is torn out, the object on success
 */
object query_selected_page_clone(int num) {
  return this_object();
} /* query_selected_page_clone() */

/**
 * This method tears the current page out of the book and returns it
 * to us.  This object will be moved into the destination so that it is
 * no longer inside us.  If it cannot be moved of the page has already
 * been remove then the function will return 0.
 * @param dest the destination to move the page to
 * @return the torn out page, or 0 on failure
 */
object tear_current_page_out(object dest) {
  object page;

  if (is_current_page_torn_out()) {
    /* Too late... */
    return 0;
  }

  if (_pages[_open_page-1]) {
    page = clone_object(_default_page_object);
    page->add_alias("page");
    page->add_plural("pages");
    if(_pages[_open_page-1] != 1)
      page->set_read_mess(_pages[_open_page-1]);
   }

  // First remove the weight from the outside.
  if (page && page->move(dest) == MOVE_OK) {
    _pages[_open_page-1] = 0;
    return page;
  }
  return 0;
} /* tear_current_page_out() */

/**
 * This method adds a new page in after the selected page.  The pages
 * are numbered from 1, so adding a page after page 0 will place a
 * page on the start and after the last page onto the end.
 * @param page the page to add
 * @param after the page to add it after
 * @return 1 on success, 0 on failure
 * @see query_current_page()
 * @see query_num_pages()
 */
int add_page_after(object page, int after) {
   if (after < 0 || after > sizeof(_pages) + 1 ||
       !objectp(page) || !intp(after)) {
      return 0;
   }
   _pages = _pages[0..after - 1] + page->query_read_mess() + _pages[after..];
   page->move("/room/rubbish");
   return 1;
} /* add_page_after() */

/**
 * This method replaces the selected page with a new page.  The old page
 * is dested if it can be and replaced with a nice shiny new page object.
 * @param page the page to replace with
 * @param num the page number to replace
 * @return 1 on success, 0 on failure
 * @see add_page_after()
 */
int replace_page_with(object page, int num) {
   if (num < 1 || num > sizeof(_pages) ||
       !objectp(page) || !intp(num)) {
      return 0;
   }
   _pages[num-1] = page->query_read_mess();
   page->move("/room/rubbish");
} /* replace_page_with() */

/**
 * This method makes the selected page blank.  It defaults it back to a
 * default page object as if the book had just been created.
 * @param num the page to make blank
 * @see replace_page_with()
 * @see add_page_after()
 * @see add_blank_page_after()
 */
int make_page_blank(int num) {
   if (num < 1 || num > sizeof(_pages) || !intp(num)) {
      return 0;
   }
   _pages[num-1] = 1;
} /* make_page_blank() */

/**
 * @ignore yes
 */
string short(int flags) {
  if (!flags || _ignore_open_page) {
    return ::short(flags);
  }
  if (_open_page) {
    return "open " + ::short(flags);
  }
  return "closed " + ::short(flags);
} /* short() */

/**
 * This is over ridden here to allow the open/closed sttus of the book to be
 * matched.
 * @ignore yes 
 */
string *parse_command_adjectiv_id_list() {
   if (_open_page) {
      return ::parse_command_adjectiv_id_list() + ({ "open" });
   }
   return ::parse_command_adjectiv_id_list() + ({ "closed" });
} /* parse_command_adjectiv_id_list() */

/**
 * @ignore yes
 */
string long(string str, int dark) {
  string ret;
  int i;

  if (!_open_page) {
    return ::long(str, dark)+"It is closed.\n";
  }
  ret = ::long(str, dark)+"It is open at page " + _open_page + ".\n";
  for (i=_open_page-1;i<sizeof(_pages) && !_pages[i];i++) {
    if (!_pages[i]) {
      ret += "Page "+(i+1)+" has been torn out.\n";
    }
  }
  if (i >= sizeof(_pages)) {
    ret += "All the rest of the pages have been torn out!\n";
  } else {
    if (i != _open_page -1) { /* A page has been torn out... */
      ret += "You can see page "+(i+1)+" however.\n";
    }

    // Only show the page if they "look page of book" or somesuch.
    if(str && strsrch(str, "page") != -1) {
      ret += _default_page_object->long(str, dark);
    }
  }
  return ret;
} /* long() */

/**
 * Opens the book to the right place.  This is the add_command function.
 * @param indir the indirect objects (not used)
 * @param id_mactch the string matching the id (not used)
 * @param ind_match the string matching the indirect ob (not used)
 * @param args the arguments associated with the pattern
 * @param the pattern which was matched.
 * @see /global/new_parse->add_command()
 * @return 1 if it succeeded, 0 if it failed
 * @ignore yes
 */
protected int do_open(int page) {
  if (page <= 0)  {
     write("Oddly enough, the first page is page 1.\n");
     page = 1;
  }
  else if (page > sizeof(_pages))  {
     write("There " + (sizeof(_pages) == 1 ? "is only " : "are only ") +
           query_num(sizeof(_pages)) +
           (sizeof(_pages) == 1 ? " page in " : " pages in ") +
           the_short() + ".\n");
     page = sizeof(_pages);
  }
  if (query_open_page() == page) {
    add_failed_mess("The $D is already open at page " + page + ".\n");
  }
  if (_open_page > 0  &&  is_page_torn_out (page)) {
     add_failed_mess("The page " +  page + " in $D is torn out.\n");
     return 0;
  }
  _ignore_open_page = 1;
  call_out((: _ignore_open_page = 0 :), 4);
  set_open_page(page);
  add_succeeded_mess("$N $V $D to page " + page + ".\n", ({ }));
  return 1;
} /* do_open() */

/**
 * Turns a page of the book.
 * @return always returns 1
 * @ignore yes
 */
protected int do_turn(int number) {
  int tmp;

  tmp = query_open_page();
  if (tmp+number > sizeof(_pages)) {
    set_open_page(0);
    add_succeeded_mess("$N close$s $D.\n");
  } else {
    if (tmp == 0) {
       add_succeeded_mess("$N turn$s $D to page " + number + ".\n");
    }
    else add_succeeded_mess("$N turn$s " + number +
           (number > 1 ? " pages " : " page ") + "of $D.\n");
    set_open_page(tmp + number);
  }
  if (tmp == query_open_page()) {
     add_failed_mess("Unable to turn page of $D.\n", ({ }));
     return 0;
  }
  _ignore_open_page = 1;
  call_out((: _ignore_open_page = 0 :), 4);
  return 1;
} /* do_turn() */

/**
 * Closes the book.
 * @return 0 if it failed, 1 if it succeeded
 * @ignore yes
 */
protected int do_close() {
  if (!query_open_page()) {
    this_player()->add_failed_mess(this_object(), "$D is already closed.\n",
                                   ({}));
    return 0;
  }
  _ignore_open_page = 1;
  call_out((: _ignore_open_page = 0 :), 4);
  set_open_page(0);
  return 1;
} /* do_close() */

/**
 * Tears out the current page.
 * @return 0 if it failed, 1 if it succeded
 * @ignore yes
 */
int do_tear(int number) {
  int i;
 
  if (_ignore_saved_pages) {
    add_failed_mess("For some reason you cannot seem to tear any pages "
                    "from $D.\n");
    return 0;
  }
 
  if (!_open_page) {
    this_player()->add_failed_mess(this_object(), "$D is closed!\n",
                                   ({}));
    return 0;
  }

  // do all the pages.
  if(number == 0) {
    _open_page = 1;
    number = sizeof(_pages);
  }
  
  for(i=0; i<number; i++) {
    
    if(!tear_current_page_out(this_player()))
      break;
      
    if ( _open_page != sizeof( _pages ) ) {
        _open_page++;
    }
  }

  if(i) {
    if(i > 1)
      add_succeeded_mess("$N $V " + i + " pages from $D.\n");
    else
      add_succeeded_mess("$N $V a page from $D.\n");
    return 1;
  }

  return 0;
} /* do_tear() */

/* The stuff redefined for handleing books... */
/**
 * @ignore yes 
 */
varargs void set_read_mess(string str, string lang, int size) {

  if (_open_page) {
    if(pointerp(str)) {
      _pages[_open_page-1] = str;
      return;
    }
    if(str)
      _pages[_open_page-1] = ({ ({ str, 0, lang, size }) });
    else
      _pages[_open_page-1] = 1;
    return;
  }
  return ::set_read_mess(str, lang, size);
} /* set_read_mess() */

/**
 * @ignore yes 
 */
void add_read_mess(mixed str, string type, string lang, int size) {
  if(_open_page) {
    if(!arrayp(_pages[_open_page-1]))
      _pages[_open_page-1] = ({ });
    _pages[_open_page-1] += ({ ({ str, type, lang, size }) });
    return;
  }
  ::add_read_mess(str, type, lang, size);
} /* add_read_mess() */

/**
 * @ignore yes 
 */
mixed query_read_mess() {
  if (_open_page) {
    if(!_pages[_open_page-1])
      return ({ });
    return _pages[_open_page-1];
  }
  return ::query_read_mess();
} /* query_read_mess() */

/** @ignore yes */
mixed *stats() {
  return ({
    ({ "num pages" , sizeof(_pages) }),
    ({ "ignore saved pages" , _ignore_saved_pages }),
    ({ "default page ob" , _default_page_object }),
    ({ "open page", _open_page }),
    ({ "book number" , _book_num }),
  }) + ::stats();
} /* stats() */

/**
 * @ignore yes 
 */
void dest_me() {
  int i;

  for (i=0;i<sizeof(_pages);i++) {
    if (objectp(_pages[i])) {
      _pages[i]->dest_me();
    }
  }
  ::dest_me();
} /* dest_me() */

/**
 * @ignore yes 
 */
mixed query_static_auto_load() {
  if (file_name(this_object())[0..8] == "/std/book") {
    return int_query_static_auto_load();
  }
} /* query_static_auto_load() */

/**
 * @ignore yes 
 */
mapping query_dynamic_auto_load() {
  mapping bing;

  bing = ([
      "::" : ::query_dynamic_auto_load(),
      "default page object" : _default_page_object,
      "open page" : _open_page,
      "book num" : _book_num,
    ]);
  if (!_ignore_saved_pages) {
    bing["pages"] = _pages;
  }
  return bing;
} /* query_dynamic_auto_load() */

/**
 * @ignore yes 
 */
void init_dynamic_arg(mapping map, object player) {
  int i;
  object *tmp;

  if (!player) {
    player = _player;
    if (!player) {
       player = this_player();
    }
  }
  if (map["::"]) {
    ::init_dynamic_arg(map["::"]);
  }
  if (map["default page object"]) {
    _default_page_object = map["default page object"];
    load_object(_default_page_object);
    _def_p_obj = find_object(_default_page_object);
  }
  if (map["pages"] && !_ignore_saved_pages) {
    // Are we looking at an old-style book that needs converting?
    if(sizeof(map["pages"]) && sizeof(map["pages"][0]) == 3) {
      for(i=0; i<sizeof(_pages); i++) {
        tmp = (object *)player->load_auto_load_to_array(map["pages"][i],
                                                        this_object(), player);
        if(sizeof(tmp->query_read_mess()))
          _pages[i] = tmp->query_read_mess()[0];
        tmp->move("/room/rubbish");
      }
    } else
      _pages = map["pages"];
  }
  _book_num = map["book num"];
  set_open_page(map["open page"]);
} /* init_dynamic_arg() */

/**
 * Sets the object to use as the default page.  If this is not set the
 * the object /obj/misc/paper.c will be used.
 * @param ob the object to use as the default page
 */
void set_default_page_object(string obj) {
  load_object(obj);
  if (find_object(obj)) {
    _default_page_object = obj;
    _def_p_obj = find_object(obj);
  }
} /* set_default_page_object() */

/**
 * This method returns the default page object.
 * @return the default page object (as a string)
 */
string query_default_page_object() {
   return _default_page_object;
} /* query_default_page_object() */

/**
 * This method creates a new default page object for this book.  THis
 * can be used for whatever nefarious purposes you want.
 * @return a new default page object
 */
object create_default_page_object() {
   return clone_object(_default_page_object);
} /* create_default_page_object() */

/**
 * Queries the number of pages in the book.
 * @return the number of pages in the book
 * @see set_no_pages()
 */
int query_num_pages() {
  return sizeof(_pages);
} /* query_num_pages() */

/**
 * This method returns the current number of the book.  This is assigned
 * and used in conjunction with the book handler.
 * @see /obj/handlers/book_handler.c
 * @see /std/print_shop.c
 * @return the current book number
 */
int query_book_num() {
   return _book_num;
} /* query_book_num() */

/**
 * This method sets the current number of the book.  This is assigned
 * and used in conjunction with the book handler.
 * @see /obj/handlers/book_handler.c
 * @see /std/print_shop.c
 * @param num the new book number
 */
void set_book_num(int num) {
   _book_num = num;
} /* set_book_num() */

/**
 * This method returns the current setting of the ignore saved
 * pages variable.  If this is true then
 * the pages form the players auto_load stuff will be ignored totaly
 * and only the page creates in setup would be
 * used.
 * @see set_ignore_saved_pages()
 * @return the current ignore saved page attribute
 */
int query_ignore_saved_pages() {
   return _ignore_saved_pages;
} /* query_ignore_saved_pages() */

/**
 * This method sets the current setting of the ignore saved
 * pages variable.  If this is true then
 * the pages form the players auto_load stuff will be ignored totaly
 * and only the page creates in setup would be
 * used.  This can only be called from an object
 * which inherits this one (hence the protected method).
 * @param saved the new value of the saved attribute
 * @see query_ignore_saved_pages()
 */
protected void set_ignore_saved_pages(int saved) {
   _ignore_saved_pages = saved;
} /* set_ignore_saved_pages() */

/**
 * This method is called by the auto loading process to set the current
 * player on this object.
 * @param player the player to set
 * @see query_player()
 */
void set_player(object player) {
   _player = player;
} /* set_player() */

/**
 * This method will return  the player associated with the auto loading
 * process.
 * @return the player in use by the autoloading
 * @see set_player()
 */
object query_player() {
   return _player;
} /* query_player() */

/**
 * @ignore
 * This will determine the books read short, which is different to a
 * normal short because it says wild stuff about the cover and things.
 */
string query_read_short(object player, int ignore_labels) {
  if(!::query_read_short(player, 0))
    return 0;
  if (!_open_page) {
    return "the cover of " + ::query_read_short(player, 0);
  }

  return "page " + query_num(_open_page) + " of " +
    ::query_read_short(player, 1);
} /* query_read_short() */

/**
 * @ignore
 * This is overridden specifically to stop labels appearing on anything
 * but the cover.
 */
string query_readable_message(object player, int ignore_labels) {
   return ::query_readable_message(player, _open_page != 0);
} /* query_readable_message() */
