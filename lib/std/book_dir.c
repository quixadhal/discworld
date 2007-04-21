/**
 * Make a book with all the files in a dir.
*
 * @author Pinkfish
 */

inherit "/std/book";

#include "nroff.h"

string language = 0;

/**
 * Sets the language of the book
 */
void set_book_language(string lang) {
   language = lang;
}
/**
 * Returns the language of the book
 */
string query_language() { return language; }

/**
 * Sets the directory to look for the book pages in.  It scans every
 * file in this dir to get the pages for the book.
 * This sets the default paper to /obj/misc/nroff_paper.c
 * @param dir the directory in which to search
 * @see /obj/misc/nroff_paper.c
 * @example
 * inherit "/std/book_dir";
 *
 * void setup() {
 *    ...
 *    set_book_dir("/save/books/dead/dead_page");
 *    ...
 * } /\* setup() *\/
 */
void set_book_dir(string dir) {
  int i, max;
  
  set_no_pages(10);
  // The line below should stop books loosing their pages - Gin
  set_ignore_saved_pages(1);
  
  max = 10;
  i = 1;
  set_default_page_object("/obj/misc/nroff_paper");
  set_open_page (0);
  
  while (file_size(dir+i) > 0) {
    set_open_page(i);
    set_read_mess("$$" + dir + i + "$$" , language, 0);
    i++;
    if (i >= max) {
      max += 10;
      set_no_pages(max);
    }
  }

  /* Reduce the number of pages down */
  set_no_pages(i-1);

  set_open_page(0);
} /* set_book_dir() */
