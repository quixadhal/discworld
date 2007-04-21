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
#define PAGES    8
inherit "/std/book";

void setup() {
  set_name( "book" );
  set_short( "newbie guide book");
  set_main_plural( "newbie guide books" );
  add_adjective( ( {"newbie", "guide"} ) );
  set_weight( 2 );
  set_value( 0 );
  set_no_pages( PAGES );
  set_long( "This is a guide book for the newbies who rushed through the "
            "newbie foyer too fast or are just absent-minded in general.  "
            "It contains valuable information on the commands, guilds "
            "and the city.  "
            "You may <read newbie book>, <open newbie book to page #> and "
            "<turn a page of newbie book>.\n" );
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
"   ___|  Syntaxes: <open book> <read book> <turn a page of book>   |___  \n"
"   \\  |____________        <open book to page #>       ____________|  /  \n"
"   /____/          \\__________________________________/          \\____\\ ",
"general");

            /* Artwork by Gototh, modified and 'transformed' by me */

  set_open_page( 1 );
  set_read_mess( "                                                \n"
 "                                                                \n"     
"                            %^B_RED%^TABLE OF CONTENTS%^RESET%^                        Page 1\n"
 "                    .o0o.o0o.o0o.o0o.o0o.o0o.o0o.o0o.       \n"
 "                                            \n"
 "         General information.....................................2-3\n"
 "         Commands................................................4-5\n"
 "         Guilds..................................................6-7\n"
 "         The lands around Ankh-Morpork.............................8\n"
 "\n\n"
 " %^B_RED%^NOTE:%^RESET%^ If you haven't played MUD's before, we advise you"
 " to read any of\n"
 " the FAQ's around (Frequently Asked Questions).  To read the "
 "Discworld FAQ,\n"
 " type 'help FAQ' or point your http browser at \n"
 " <http://discworld.imaginary.com/> and click on the \n"
 " 'Frequently Asked Questions' link.",
 "general", 100 );

  set_open_page( 2 );
  set_read_mess( "\n"
 "                            GENERAL INFORMATION                     Page 2\n"
 "                           -=-=-=-=-=-=-=-=-=-=-                 \n"
 "\n"
 " At any time during the course of playing you can get HELP on various \n"
 " topics by typing  '%^B_RED%^help concepts%^RESET%^' "
 "and  '%^B_RED%^help command_list%^RESET%^'. \n"
 " If you need to ask something else, or have problems of any kind, \n"
 " the LIAISON creators will kindly help you out, if you ask nicely. \n"
 " To see if any are logged on, simply type  '%^B_RED%^liaisons%^RESET%^'.\n"
 " Also, most of the players are usually very helpful when it comes to \n"
 " answering questions.                                \n"
 "\n"
 " As soon as you have made a choice you should join a GUILD where you can \n"
 " advance your skills (take a look at page 5 in this brochure for more \n"
 " information on the different guilds).  But while deciding, you can \n"
 " simply teach yourself.  Type  '%^B_RED%^help level%^RESET%^'  and "
 " '%^B_RED%^help teach%^RESET%^' \n"
 " to find out more.\n"
 "\n"
 " CONTINUES ON NEXT PAGE.",
 "general", 100 );
  
  set_open_page( 3 );
  set_read_mess( "\n"
 "                        MORE GENERAL INFORMATION                   Page 3\n"
 "                       -=-=-=-=-=-=-=-=-=-=-=-=- \n"
 "\n" 
 " In Ankh-Morpork there are GUIDES wandering around that can take you to \n"
 " certain destinations.  These are often waiting by the guilds or in the \n"
 " Mended Drum.  To use one you say in the same room as one, for example: \n"
 " 'take me to wizards guild please'.  \n"
 " Other places they can take you to, besides the guilds, are \n"
 " 'the post office',  'the bank',  'least gate' and  'hubwards gate'.\n"
 "                                                    \n"
 " You start off with a total of eight lives.  If you should happen to die,\n"
 " Harry at the Temple of Small Gods, can RAISE you from the dead and you \n"
 " are then set to go again (one life shorter).  Some player Priests also \n" 
 " have the ability to resurrect.\n"
" And please BUG report bugs and typos you see.  Do  '%^B_RED%^help bug%^RESET%^' "
 " and \n"
 " '%^B_RED%^help typo%^RESET%^'  for more information.", 
 "general", 100 );

  set_open_page( 4 );
  set_read_mess( " \n"
  "                                  COMMANDS                         Page 4\n"
  "                                -=-=-=-=-=-\n"
  "\n"
  " '%^B_RED%^who%^RESET%^' or '%^B_RED%^finger%^RESET%^'"
  "  Will show the people logged on.  \n"
  "                    You can also 'finger <playername>'\n"
  " '%^B_RED%^rearrange%^RESET%^'        Allows you to set your stats"
  " (You can only do this once).\n"
  " '%^B_RED%^score%^RESET%^'            Will show statistics.\n"
  " '%^B_RED%^score stats%^RESET%^'      More statistics.\n"
  " '%^B_RED%^i%^RESET%^'                Will show you your inventory.\n"
  " '%^B_RED%^skills%^RESET%^'           Will list your skills.\n"
  " '%^B_RED%^wimpy < % >%^RESET%^'      To automatically run away before "
  "you die.\n"
  " '%^B_RED%^monitor on%^RESET%^'       Will help you keep track of Hit"
  " and Guild points.\n"
  " '%^B_RED%^inform logon on/off%^RESET%^'    Gives you info on logon/out-"
  "events (once you've \n                    joined a guild).\n"
  " '%^B_RED%^.%^RESET%^'                Will repeat your last command.\n"
  "\n"
  " CONTINUES ON NEXT PAGE.",
  "general", 100 );  

  set_open_page( 5 );
  set_read_mess("\n"
  "                              MORE COMMANDS                        Page 5\n"
  "                             -=-=-=-=-=-=-=-\n"
  "\n"
  " '%^B_RED%^tactics%^RESET%^'         Will show your combat tactics.  Read\n"
  "                  'help tactics' for information on how to use it.\n"
  "\n"
  " Other commands worth checking out, or read the help files for, are: \n"
  " '%^B_RED%^alias%^RESET%^',  '%^B_RED%^earmuff%^RESET%^', "
  " '%^B_RED%^commands%^RESET%^', "
  "'%^B_RED%^nickname%^RESET%^',  '%^B_RED%^consider%^RESET%^'\n"
  " '%^B_RED%^gp%^RESET%^' and  '%^B_RED%^playerkilling%^RESET%^'.",
  "general", 100 );  

  set_open_page( 6 );
  set_read_mess( "\n"
 "                               THE GUILDS                           Page 6\n"
 "                             -=-=-=-=-=-=-\n"
 "\n"
 " Each player will usually join a guild so that they can learn skills\n"
 " and commands and so advance their abilities.   Being in a guild also \n"
 " adds character to your character.  These guilds are currently available:\n"
 " WIZARDS GUILD is located west of Sator Square.  To join you have to look\n"
 " around in the Great Hall and find a staircase.\n"
 " THIEVES GUILD is at the beginning of Alchemist street.\n"
 " FIGHTERS GUILD is on Filigree Street, just a stone's throw from Mended \n"
 " Drum.\n"
 " PRIESTS GUILD with its various Gods is located on Small Gods Street.\n"
 " The Gods are: Hat, Pishe, Fish, Sek, Gapp, Gufnork and Sandelfon.\n"
 " For more information, type 'info' in their guild rooms.\n"
 "\n"
 " CONTINUES ON NEXT PAGE.",
 "general", 100 );

 set_open_page( 7 );
 set_read_mess( "\n"
 "                               THE GUILDS                           Page 7\n"
 "                             -=-=-=-=-=-=-\n"
 "\n"
 " ASSASSINS GUILD is located at the eastern end of Filigree Street.  Fill \n"
 " in an application and give it to the secretary in the guild office.\n"
 " WITCHES GUILD is located at Granny Weatherwax' cottage in Lancre. \n"
 " See Gennie Applebottom at Dione Street in Ankh-Morpork if you have \n"
 " trouble finding the place.",
 "general", 100 );

  set_open_page( 8 );
  set_read_mess( "\n"
 "                        THE LANDS OUTSIDE ANKH-MORPORK              Page 8\n"
 "                       -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-                \n"
 "\n"
 " The lands around the city can sometimes be strange and dangerous \n"
 " places.  Through the northeastern (Least) gate is the path that goes \n"
 " through the plains, some small villages, and finally up to the \n"
 " mountains, also known as the Ramtops.\n"
 " The southeastern (Hubwards) gate takes you into the vast forest where \n"
 " innocent people are terrorised by wolves and other strange creatures \n"
 " that roam unfettered.  It's also a more direct route to the Ramtops\n"
 " (Carriages from Ankh-Morpork are a nice alternative to tired feet for \n"
 " travelling to and from the towns in the Sto Plains and Ramtops).\n"
 " Holywood is a great place for newbies venturing out of the city.  You \n"
 " get there via the Brass Bridge.\n"
 " The tour operator at Sator Square is ready to send you to the warm \n"
 " and distant continent of Klatch, if you have enough money.\n"
 " '%^B_RED%^help map%^RESET%^'  will give you a nice map of Discworld and "
 "there are more \n"
 " fantastic and exciting places to come.",
 "general", 100 );   
  set_open_page( 0 );

}

