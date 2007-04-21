/**
 * This handler object deals with all the books that have been published.
 * It keeps a save file of every book and a current book number.
 * It will allow for easy finding again of preprinted books and for the
 * easy removal of any books that happen to contain things like quest
 * solutions and stuff.
 *
 * @author Pinkfish
 * @started Sun Feb  1 20:24:33 CST 1998
 * @see /std/book.c
 * @see /std/print_shop.c
 */

#include <book_handler.h>
#include <language.h>

#define SAVE_DIR "/save/book_handler/"
#define SAVE_FILE (SAVE_DIR "main_save")

#define OWNER_INDEX "owner"
#define PAGES_INDEX "pages"

int current_book_number;
mapping banished_books;

int compare_page(object page1, object page2);
void load_it();
void save_it();
int check_book_same(object book, int num);
protected string *query_pages(object book);
int compare_pages(string *pages1, string *pages2);
string *query_book_pages(int book_num);

void create() {
   banished_books = ([ ]);
   seteuid(getuid());
   load_it();
} /* create() */

/**
 * This tests to see if the given book number exists.
 * @param book_num the number to test.
 * @return 1 if it exists, 0 if it does not.
 */
int query_book_exists(int book_num) {
   return unguarded((: file_size(SAVE_DIR + $(book_num) + ".o") :)) > 0;
} /* query_book_exists() */

/**
 * Adds a newly minted book into our nice list of stuff.  This returns the
 * book number for this book.  You can use this to add a book if you already
 * know its book number, if it does not match to the saved book number then
 * it will return a new number.  If it does match it will return the
 * same number.
 * @param book the book to add
 * @param num the possible book number it already has
 * @see check_book_same()
 */
int add_book(object book, int num, string pl_name) {
   string *stuff;
   mapping bing;

   if (num) {
      if (check_book_same(book, num)) {
         return num;
      }
   }

   /* Make sure if the save file gets stuffed up we don't overwrite things. */
   while (query_book_exists(current_book_number)) {
      current_book_number++;
   }
   stuff = query_pages(book);
   bing = ([ OWNER_INDEX : pl_name, PAGES_INDEX : stuff ]);
   unguarded( (: write_file(SAVE_DIR + current_book_number + ".o",
              save_variable($(bing))) :) );
   num = current_book_number++;
   save_it();
   return num;
} /* add_book() */

/**
 * Checks to see if the passed in book is the same as the current book
 * number that is set on it.  We only look at the text and ignore the
 * type and language with which it is written.
 * @param book the book to check
 * @param num the book number to check against
 * @see compare_pages()
 * @see add_book()
 */
int check_book_same(object book, int num) {
   string *bing;
   string *fluff;
   int old_open_page;
   int same;

   old_open_page = book->query_open_page();
   same = 0;
   if (query_book_exists(num) > 0) {
      bing = query_book_pages(num);
      fluff = query_pages(book);
      if (compare_pages(bing, fluff) > 90) {
         same = 1;
      }
   } else {
      same = 0;
   }
   book->set_open_page(old_open_page);
   return same;
} /* check_book_same() */

/**
 * This method returns the pages associated with the book of a given
 * number.
 * @return the pages for the book
 * @param book_num the book number to get the pages for
 * @see query_book_owner()
 */
string *query_book_pages(int book_num) {
   mapping bing;
   string tmp;

   tmp = unguarded((: read_file(SAVE_DIR + $(book_num) + ".o") :));
   if (tmp) {
      bing = restore_variable(tmp);
      return bing[PAGES_INDEX];
   }
   return 0;
} /* query_book_pages() */

/**
 * This method returns the owner associated with the book of a given
 * number.
 * @return the owner of the book
 * @param book_num the book number to get the pages for
 * @see query_book_pages()
 */
string query_book_owner(int book_num) {
   mapping bing;
   string tmp;
 
   tmp = unguarded((: read_file(SAVE_DIR + $(book_num) + ".o") :));
   if (tmp) {
      bing = restore_variable(tmp);
      return bing[OWNER_INDEX];
   }
   return 0;
} /* query_book_owner() */

/**
 * This method returns the array of useful pages in a book.  It removes
 * torn out pages and pages which are added by specific things in the
 * game, like the print shop adding a fly leaf.  It just returns the text
 * for the pages, since this is what we are interested in.
 * @param book the book to get the pages from
 * @return the array of pages
 */
string *query_pages(object book) {
   int i;
   string *ret;
   mixed *stuff;
   mixed *group;
   string text;
   int old_open_page;
   object ob;

   ret = ({ });
   old_open_page = book->query_open_page();
   for (i = 0; i < book->query_num_pages(); i++) {
      book->set_open_page(i);
      ob = book->query_current_page();
      /* Make sure it is not torn out and that it is not auto generated. */
      if (!book->is_current_page_torn_out() &&
          !ob->query_property(BH_IGNORE_PAGE)) {
         stuff = ob->query_read_mess();
         text = "";
         foreach (group in stuff) {
            text += group[READ_MESS];
         }
         ret += ({ text  });
      }
   }
   book->set_open_page(old_open_page);
   return ret;
} /* query_pages() */

/**
 * This method will try and figure out the percentage changed between the
 * two page arrays.
 * @param pages1 the first page array
 * @param pages2 the second page array
 * @return the percentage the same
 */
int compare_pages(string *pages1, string *pages2) {
   int no_chars;
   int no_same;
   int pos;
   int start;
   string page;
   string *tmp;

   pos = 0;
   if (sizeof(pages1) > sizeof(pages2)) {
      tmp = pages1;
      pages1 = pages2;
      pages2 = tmp;
   }
   /* Now we go through the smaller array... */
   foreach (page in pages1) {
      /*
       * Ok, now see if we can find where this page might start in the
       * other book.
       *
       * We will assume that all books will start the same.  Therefor
       * if we cannot find a match for the first page...  The rest of the
       * book does not match.  We assume that the one stored in the
       * book handler is the complete one.
       */
      pos = 0;
      do {
         start = strsrch(page, pages2[pos]);
         if (start == -1) {
            start = strsrch(pages2[pos], page);
            if (start == -1) {
               pos++;
            } else {
               no_same += strlen(page);
            }
         } else {
            no_same += strlen(pages2[pos]);
         }
      } while ((start == -1) && (pos < sizeof(pages2)));
      no_chars += strlen(page);
   }

   /* Calculate the percentage the same and return it. */
   if (no_chars > 0) {
      return (no_same * 100) / no_chars;
   }
   return 0;
} /* compare_pages() */

/**
 * Compares the open pages of two books to see if the text is the
 * same.  It ignores language and style of writing.  This would also
 * work on two pieces of paper.
 * @param page1 the first book
 * @param page2 the second book
 * @see check_book_same()
 * @see add_book()
 */
int compare_page(string page1, string page2) {
   mixed *stuff1;
   mixed *stuff2;
   string text1;
   string text2;
   int i;

   stuff2 = page2->query_read_mess();
   text1 = "";
   for (i = 0; i < sizeof(stuff1); i++) {
      text1 += stuff1[READ_MESS];
   }

   text2 = "";
   for (i = 0; i < sizeof(stuff2); i++) {
      text2 += stuff2[READ_MESS];
   }

   return (text1 == text2);
} /* compare_page() */

/**
 * This method banishes a book number for a certain reason.
 * @param book_num the book number to banish
 * @param reason the reason it was banished
 * @return 1 on success and 0 on failure.
 * @see query_banished_book()
 * @see query_all_banished_books()
 */
int add_banished_book(int book_num, string reason) {
   if (!stringp(reason) || !intp(book_num)) {
      /* Need a reason. */
      return 0;
   }
   if (!query_book_exists(book_num)) {
      /* Does not exist. */
      return 0;
   }

   banished_books[book_num] = reason;
} /* banish_book_num() */

/**
 * This method returns the banish string for the book.
 * @param book_num the book number to check
 * @return the banish reason, or 0 if none
 * @see add_banished_book()
 * @see query_all_banished_books()
 */
string query_banished_book(int book_num) {
   return banished_books[book_num];
} /* query_banished_book() */

/**
 * This method returns all the banished books.  The keys are the
 * numbers which are banished and the values are the reason for
 * the banishment.
 * @return the banished books mapping
 * @see add_banished_book()
 * @see query_banished_book()
 */
mapping query_all_banished_books() {
   return banished_books;
} /* query_all_banished_books() */

/**
 * Saves the current state information to the save file.
 * @see save_it()
 */
void save_it() {
   unguarded((: save_object(SAVE_FILE) :));
} /* save_it() */

/**
 * Loads the state information from the save file.
 * @see load_it()
 */
void load_it() {
   unguarded((: restore_object(SAVE_FILE) :));
   if (!current_book_number) {
      current_book_number = 1;
   }
   if (!banished_books) {
      banished_books = ([ ]);
   }
} /* load_it() */
