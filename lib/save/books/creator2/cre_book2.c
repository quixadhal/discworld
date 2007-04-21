inherit "/std/book";
#include <colour.h>


void setup() {

  set_name("Command Summary");
  add_alias( ({"book", "command book", "summary", "command summary"})  );
  add_adjective( ({ "small", "black", "small black", "command" }) );
  set_long("This is a pocket size black book, which looks as if it "+
           "has just been printed.\n");
   set_read_mess( "The Creator's Command Quick Reference." );
  set_no_pages(8);
  set_main_plural("small black books");
  add_plural("command summaries");
  add_plural("books");
  set_weight(11);   
  add_property("no recycling", 1);

  set_open_page( 1 );
  add_read_mess( "This book contains some information on basic calls that "
                 "most Creators should be aware of.\n"
                 "\n"
                 "                        Index.\n"
                 "\n"
                 "%^GREEN%^Show a help file to a player%^RESET%^...................2\n"
                 "%^GREEN%^Resurrect a player%^RESET%^.............................3\n"
                 "%^GREEN%^Replace a life%^RESET%^.................................3\n"
                 "%^GREEN%^Locate a player%^RESET%^................................4\n"
                 "%^GREEN%^Travel to a player%^RESET%^.............................4\n"
                 "%^GREEN%^Travel to a room%^RESET%^...............................4\n"
                 "%^GREEN%^Bring a player to you%^RESET%^..........................4\n"
                 "%^GREEN%^Check a player's inventory%^RESET%^.....................5\n"
                 "%^GREEN%^Replace a receipt for a player%^RESET%^.................5\n"
                 "%^GREEN%^Replace other items%^RESET%^............................5\n"
                 "%^GREEN%^Remove an item or receipt from player%^RESET%^..........5\n"
                 "%^GREEN%^Reproduce an item%^RESET%^..............................5\n"
                 "%^GREEN%^Find an item's path%^RESET%^............................5\n"
                 "%^GREEN%^See a player's list of quests%^RESET%^..................6\n"
                 "%^GREEN%^Grant a divorce%^RESET%^................................6\n"
                 "%^GREEN%^Reset a room or item%^RESET%^...........................7\n"
                 "%^GREEN%^Drop or move your Bulletin board%^RESET%^...............7\n"
                 "%^GREEN%^Talk or emote to all Creators%^RESET%^..................7\n"
                 "%^GREEN%^Changing your appearance, entry messages%^RESET%^.......8\n"
                 , 0, "common", 1);

  set_open_page( 2 );
  add_read_mess( "\n%^GREEN%^How to show a helpfile to a player%^RESET%^\n"
                 "\n"
                 "Sometimes the best way to answer a player's questions is "
                 "simply to send her the helpfile.  "
                 "This is accomplished by giving the command:\n"
                 "%^CYAN%^showhelp <player> <helpfile>%^RESET%^.\n"
                 "An example of this would be:\n"
                 "%^CYAN%^showhelp arby liaison%^RESET%^.\n"
                 , 0 , "common", 1);
  set_open_page( 3 );
  add_read_mess( "\n%^GREEN%^How to resurrect a player\n"
                 "How to add a life to that max%^RESET%^\n"
                 "\n"
                 "When a player dies through a bug, it is sometimes necessary to "
                 "resurrect her.  The simplest way to do this is with Oblilix's "
                 "Resurrector, found in your bookcase.  However, the actual call "
                 "can also be used:\n"
                 "%^CYAN%^call remove_ghost() <player>%^RESET%^.\n"
                 "For example:\n"
                 "%^CYAN%^call remove_ghost() ycore%^RESET%^.\n"
                 "In some cases, you may also want to replace, or adjust, the "
                 "maximum number of lives a player has.  The call for this is:\n"
                 "%^CYAN%^call adjust_max_deaths(number) <player>%^RESET%^.\n"
                 "For example:\n"
                 "%^CYAN%^call adjust_max_deaths(1) lagger%^RESET%^.\n" , 0, "common", 1);
  set_open_page( 4 );
  add_read_mess( "\n%^GREEN%^How to locate a player\n"
                 "How to go to a player\n"
                 "How to go to a room\n"
                 "How to bring a player to you%^RESET%^\n"
                 "\n"
                 "There are two commands that allow you to simply locate a player:\n"
                 "%^CYAN%^whereis <player>%^RESET%^\n"
                 "will give you the location of the named player, and the command:\n"
                 "%^CYAN%^people%^RESET%^\n"
                 "on its own will list the names and locations of all players.\n"
                 "To go to a player, simply use:\n"
                 "%^CYAN%^goto <player>'%^RESET%^.\n"
                 "Going to a room is a bit more complex.  You need to first know the "
                 "path of the room.  This can be found by using:\n"
                 "%^CYAN%^cd%^RESET%^ (change directory) and %^CYAN%^ls%^RESET%^ "
                 "(list directory) and moving through the directory structure until you find "
                 "the desired room.  You can also find the path you want by asking "
                 "another cre, by noting convenient spots as you walk around, and "
                 "writing them down, etc.  Once you know the path, the command is:\n"
                 "%^CYAN%^goto <path>%^RESET%^.\n"
                 "The best way to bring a player to you is with Ceres' Transporter, "
                 "since this keeps a portal open to the player's previous location.  "
                 "Otherwise, you can use:\n"
                 "%^CYAN%^trans <player>%^RESET%^.\n"
                 "%^RED%^***%^RESET%^ Remember to note the player's original location so "
                 "you can go there, and trans her back. %^RED%^***%^RESET%^", 0,
                 "common", 1);
  set_open_page( 5 );
  add_read_mess( "\n%^GREEN%^How to check a player's inventory\n"
                 "How to replace a lost item with a receipt\n"
                 "How to replace other items\n"
                 "How to remove items or receipts from a player\n"
                 "Duplicating items\n"
                 "Finding an item's path%^RESET%^\n"
                 "\n"
                 "You can check a player's current inventory by typing:\n"
                 "%^CYAN%^inv <player>%^RESET%^.\n"
                 "To replace an item that the player has a receipt for, first:\n"
                 "%^CYAN%^stat <item> in <player>%^RESET%^.\n"
                 "This will give you all sorts of information on the item that is "
                 "missing.  What you want is the path.  After you note this, type: "
                 "%^CYAN%^clone <path>%^RESET%^,\n"
                 "then hand the item to the player.  To remove the receipt, or any other "
                 "items from the player's inventory:\n"
                 "%^CYAN%^dest <item> in <player>%^RESET%^.\n"
                 "%^RED%^***%^RESET%^ Be cautious when using dest.  Do NOT %^CYAN%^dest "
                 "0 in <player>%^RESET%^ as this will delete her entire inventory. "  
                 "%^RED%^***%^RESET%^\n"                                        
                 "Most of the other items that can be replaced for a player can be done "
                 "using Presto's Wand. "
                 "To copy an item that a player already has one of, use:\n"
                 "%^CYAN%^duplicate <item> in <player>%^RESET%^.\n"
                 , 0, "common", 1);
  set_open_page( 6 );
  add_read_mess( "\n%^GREEN%^How to list the quests a player has done\n"
                 "How to grant a divorce%^RESET%^\n"                              
                 "\n" 
                 "Once in a while you may need to check on whether or not a player has"
                 " completed a certain quest.  This is accomplished with:\n"
                 "%^CYAN%^pquests <player>%^RESET%^.\n" 
                 "This will list all of the player's current quest information.\n" 
                 "In rare cases, a player may approach you to ask about a divorce.  "    
                 "Most often this occurs when a player's MUD spouse is no longer active "     
                 "on Discworld.  What this means is that it is impossible for them "
                 "to have a divorce granted via game channels.  To divorce a player:\n"
                 "%^CYAN%^call remove_property(\"spouse\") <player>\n"
                 "dest wedding ring in <player>%^RESET%^.\n" 
                 , 0, "common", 1);
  set_open_page( 7 );
  add_read_mess( "\n%^GREEN%^To reset a room or item\n"
                 "How to drop or move your Bulletin board\n"
                 "How to emote to other Creators%^RESET%^\n"
                 "\n"
                 "Sometimes, all you need to do to solve a player's problem is to "
                 "reset the room or item that seems to have a problem.  Use great care "
                 "when updating rooms, because this will clear out any items in the room "
                 "that don't belong there.  Update syntax is:\n"
                 "%^CYAN%^update <path> %^RESET%^(if you know the path to the item "
                 "or room.)\n"
                 "%^CYAN%^update <item> in <player> %^RESET%^(to update something in "
                 "the player's inv.)\n"
                 "%^CYAN%^update here %^RESET%^(to update the room you are in.)\n"
                 "If you've picked up your bulletin board, and would like to drop it, or "
                 "stow it in a pocket, or backpack, first use the call:\n"
                 "%^CYAN%^call set_drop() board%^RESET%^.\n"
                 "The command %^CYAN%^cre%^RESET%^ does a few things, depending on syntax:\n"
                 "%^CYAN%^cre <text>%^RESET%^ sends the text as a chat to all creators,\n"
                 "%^CYAN%^cre@ <text>%^RESET%^ sends the text as an emote to all creators,\n"
                 "%^CYAN%^cre?%^RESET%^ lists all the creators currently visible to you, "
                 "and online."
                 , 0, "common", 1);
  set_open_page( 8 ); 
  add_read_mess( "\n%^GREEN%^Changing your appearance%^RESET%^ "
                 "\n"
                 "As a creator, there are a few things you can do to customize "
                 "how you appear to players.  Many of these are listed with the "
                 "%^CYAN%^review%^RESET%^ command.  Review will show you what messages are "
                 "currently displayed when you enter or leave a room, either by walking or "
                 "by using %^CYAN%^goto%^RESET%^ to teleport there.\n"
                 "The exit/entry fields can be changed with:\n"
                 "%^CYAN%^call setmin(\"text\") me%^RESET%^ and \n"
                 "%^CYAN%^call setmout(\"text\") me%^RESET%^.\n"
                 "These will change the message given when you walk into a room.\n"
                 "To change the teleport messages, use:\n"
                 "%^CYAN%^call setmmin(\"text\") me%^RESET%^ and \n"
                 "%^CYAN%^call setmmout(\"text\") me%^RESET%^.\n"
                 "You can also change your description with:\n"
                 "%^CYAN%^call set_long(\"text\") me%^RESET%^.\n"
                 , 0, "common", 1);
  set_open_page( 9 );
  set_open_page( 0 );
}

/* setup */
int do_tear() { return 0; }
int query_binding_force() {return 100; }
