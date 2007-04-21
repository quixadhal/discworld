/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: newbie_book.c,v 1.4 2002/08/14 09:02:14 wyvyrn Exp $
 *
 * $Log: newbie_book.c,v $
 * Revision 1.4  2002/08/14 09:02:14  wyvyrn
 * Added general language
 *
 * Revision 1.3  2002/08/14 07:09:14  pinkfish
 *  Forcibly unlocked by wyvyrn
 *
 * Revision 1.2  2000/03/09 01:40:03  ceres
 * Made it valueless so the newbie shop doesn't end up full of them.
 *
 * Revision 1.1  2000/03/01 06:06:45  ceres
 * Initial revision
 *
 * Revision 1.1  1998/01/16 02:18:32  arby
 * Initial revision
 *
*/
/* Yikes!  Please don't read the code, as it has no punctation 
   whatsoever :) (I blame the colours)
   This newbie brochure is *completly* rewritten from the
   old /d/am/am/items/colourful brochure, found in the tray in the Mended
   Drum.
   
   Hobbes, July 96 

   Rewritten as a book in Mars 97
                                                     */

#include "path.h"

#define NEWBIE_SAVE "/d/liaison/NEWBIE/save/"

inherit "/std/book_dir";



void setup() {
  set_name( "book" );
  set_short( "newbie guide book");
  set_main_plural( "newbie guide books" );
  add_adjective( ({"newbie", "guide"}) );
  add_alias( "newbiebook" );
  set_weight( 2 );
  set_value( 0 );
  set_long( "This is a guide book for the newbies who rushed through the "
            "newbie foyer too fast or are just absent-minded in general.  "
            "It contains valuable information on the commands, guilds "
            "and the city of Ankh-Morpork.\n\n"
            "You may 'open book', 'read newbie book', 'turn page of "
            "newbie book' or 'open newbie book to page #'.\n" );
  set_book_language( "general" );
  set_book_dir( NEWBIE_SAVE + "newbie_book_page" );
  set_read_mess("\n"
"                          _______   nn   _______                      \n"
"                          \\____ _\\ [@@] / _____/                       \n"
"                             \\__ _\\/\\/\\/ ___/                        \n"
"                               \\__| [] |__/                            \n"
"                  ___              \\__/              ___             \n"
"                 / ..\\             /__\\             /.. \\          \n"
"               _/ \\___\\            (())            /___/ \\_        \n"
"             _/@  /V /V   _________=||=________    V\\ V\\  @\\_      \n"
"             \\   |_n_n    \\         ||        /     n_n_|   /          \n"
"              |   ____|    |                 |     |____   |           \n"
"              |-----|_____ |       The       | _____|------|           \n"
"             / __    \\ ___)|      NEWBIE     |(___ /     __ \\          \n"
"            | /  \\___ |    |    Guide book   |    | ____/  \\ |         \n"
"            | \\______)|    |      f o r      |    |(_______/ |         \n"
"            |         |    |    Discworld    |    |          |         \n"
"            |        /      \\    Mudders    /      \\         |         \n"
"             \\   _  /        \\_           _/        \\   _   /         \n"
"              | | | |          \\_       _/           | | | |           \n"
"              |_| |_|            \\_____/             |_| |_|           \n"
"       ____________________________________________________________     \n"
"   ___|  Syntaxes: 'open book' 'read book' 'turn a page of book'   |___  \n"
"   \\  |____________        'open book to page #'       ____________|  /  \n"
"   /____/          \\__________________________________/          \\____\\ ",
"general");

            /* Artwork by Gototh, modified and 'transformed' by me */
}

