/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: help.c,v 1.9 2003/02/19 06:54:24 ceres Exp $
 */
#include "path.h"
#include <library.h>

inherit PATH+"inside";

object womble;

 
void setup() {
  set_zone( "Newbie" );
  set_short( "Help Room" );
  set_light( 75 );
  set_long("This room looks like a library.  There are shelves around the "
           "room crammed with reference books.  Around the room are a "
           "number of signs.\n"
           "The signs are labelled 'helpme', 'newbie', 'problems', "
           "'etiquette' and 'quit'.\n");

  add_item("floor", "The floor is made of polished marble so smooth that "
           "you can see your face in it - yeuch!\n");
  
  add_item("wall", "The walls are hidden behind row upon row of shelves.\n");
  add_item(({ "shelves", "shelf" }), "The shelves are filled with books.\n");
  add_item("book", "There are many, many reference books here, most are "
           "weighty tomes in black leather bindings.\n");
  
  add_sign("This is a simple white sign marked 'helpme'.\n",
           FOYER->format_sign(
                 "The Discworld help system is divided into two parts: "
                 "concept help which explains discworld concepts such as "
                 "magic and command help which explains how to use specific "
                 "commands.\n\nPerhaps the single most useful command is "
                 "'help concepts'.  This command will provide a list of "
                 "concepts for which there is an overview.  These overviews "
                 "will help you understand how things work and also what "
                 "commands to use to do things.\n\nTo see the list of "
                 "commands type 'help command_list'.  In addition, the "
                 "'essentials' help file covers all the basic commands "
                 "you're likely to need to get you started."),
           "sign marked 'helpme'",
           "helpme", "general");
  
   add_sign("This is a simple white sign marked 'problems'.\n",
            FOYER->format_sign(
            "If you have problems getting started or understanding help "
            "files, you can talk to a newbiehelper (type 'helpers' for "
            "a list of online newbiehelpers) or use the newbie channel.  "
            "If you have any problems with the mud not working as you "
            "expect, liaisons are available for giving help and for "
            "answering questions.  They can't provide quest details but "
            "will try to help you if they can.\n\n"
            "To see if any creators are logged on, and to get a list "
            "of all players logged on, use the command 'finger': "
            "Creators are marked by having a C next to their name.  "
            "Senior creators are marked with an S next to their name.  "
            "Directors are marked by having an D next to their name.  "
            "Trustees are marked by having an T next to their name.\n\n"
            "To find out which creators are in the liaison domain "
            "type 'finger liaison', and to see which of those are "
            "currently online, type 'liaisons'.  "
            "To report any bugs or typos that you may encounter, please "
            "make a report out for them. Please read 'help bug' or "
            "'help typo' for information on making reports."),
            "sign marked 'problems'",
            "problems", "general" );

   add_sign("This is a simple white sign marked 'newbie'.\n", 
            FOYER->format_sign(
            "This is part of the newbie area on Discworld, "
            "written to help you get acquainted with the game.  "
            "Please enter each room and find out about what "
            "each of Discworld's guilds has to offer.\n\n"
            "If you have any questions that aren't answered by the "
            "signs, first try 'help <subject>' if that doesn't work "
            "use 'newbie <question>' and hopefully someone will "
            "answer your question and 'hnewbie' to view the past couple "
            "of questions.\n\n"
            "Once you have visited each room you may " 
            "leave the newbie area to enter Discworld proper.  "
            "Before reaching the Disc, you'll be given the choice of "
            "which city you wish to start in.  Information is given about "
            "each region and city at that time."),
            "sign marked 'newbie'",
            "newbie", "general" );

   add_sign("This is a simple white sign marked 'etiquette'.\n",
            FOYER->format_sign(
            "Discworld is a very friendly mud and there are "
            "few written rules. The bottom line is be nice "
            "and don't cheat.  In particular: \n\n"
            "The use of foul language in shouting and the "
            "broadcasting of quest information are frowned upon.  "
            "Creators will remind you of this if provoked, and "
            "will take appropriate action if necessary. Playing "
            "more than one character at once is not permitted.\n\n"
            "If you wish to play more than one character you must "
            "never have them both logged on at the same time.  "
            "Harassment of other players, sexual or otherwise, "
            "is not acceptable and may lead to banishment."), 
            "sign marked 'etiquette'",
            "etiquette", "general" );

   add_sign("This is a simple white sign marked 'quit'.\n", 
            FOYER->format_sign(
            "To leave Discworld at any time, "
            "type 'quit' at the prompt.  Your "
            "player file will automatically be "
            "saved with all your equipment.  The only exception is if "
            "you are under 30 minutes old, in which case you won't save."),
            "sign marked 'quit'",
            "quit", "general" );
   
   add_exit( "foyer", PATH +"foyer", "door");
   add_alias( "southeast", "foyer");
   
  /* Make it so you cannot godmother out of here */
  add_property("no godmother", 1);
} /* setup() */

