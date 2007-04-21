/**
 * This file defines all the constants needed for the book handler and the
 * printing shop.
 * @author David Bennett
 * @started Sun Feb 22 22:02:08 CST 1998
 * @see /obj/handlers/book_handler.c
 * @see /std/print_shop.c
 */

#ifndef __BOOK_HANDLER_H
/** @ignore yes */
#define __BOOK_HANDLER_H

/**
 * This is the property to set to a non-zero value to make pages in books
 * be counted as ignored.  This should be done for things like fly leafs in
 * printed books and stuff like that.
 * @see /obj/handlers/book_handler.c
 * @see /std/print_shop.c
 */
#define BH_IGNORE_PAGE "generated page"
/**
 * This is the path to the book handler object.
 * @see /obj/handlers/book_handler.c
 */
#define BOOK_HANDLER "/obj/handlers/book_handler"
 

/*
 *
 * ---=== The defines after this point are used by the printing press. ===---
 *
 */

/**
 * This is the property used to set the copyright status of the book.  The
 * value of the property should be the copyright holder of the book.
 * If you wish to setup a book which no one can copy then set this
 * variable to the name of the creator, or a random banished name.
 * @example
 * book->add_property("copyright", "pinkfish");
 * @see /obj/handlers/book_handler.c
 * @see /std/print_shop.c
 */
#define BH_COPYRIGHT "copyright"

/**
 * This is the index into the collection array for the time at which it
 * ready to be picked up.
 * @see /std/print_shop.c
 */
#define COLLECT_TIME 0
/**
 * This is the index into the collection array for the number of copies
 * to print.
 * @see /std/print_shop.c
 */
#define COLLECT_NUM  1
/**
 * This is the index of the string name of the object which was asked to
 * be printed.
 * @see /std/print_shop.c
 */
#define COLLECT_OB_NAME 2
/**
 * This is the index of the language which will be used in the printing of the
 * the book.
 * @see /std/print_shop.c
 */
#define COLLECT_LANGUAGE 3
/**
 * This is the index into the collection array for the number associated with
 * the book.  This the save file number, the data for the book is stored
 * in a different location to the book handler stuff itself to save
 * memory.
 * @see /std/print_shop.c
 */
#define COLLECT_AUTO_LOAD_NUM 4
/**
 * This is the size of the array to allocate for the print shop information.
 * @see /std/print_shop.c
 */
#define COLLECT_ARR_SIZE 5

#endif
